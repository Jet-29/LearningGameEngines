#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Engine {

    namespace Utils {

        static GLenum TextureTarget(bool multiSampled) {
            return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multiSampled, uint32_t* outID, uint32_t count) {
            glCreateTextures(TextureTarget(multiSampled), count, outID);
        }

        static void BindTexture(bool multiSampled, uint32_t id) {
            glBindTexture(TextureTarget(multiSampled), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
            bool multiSampled = samples > 1;
            if (multiSampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);

        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
            bool multiSampled = samples > 1;
            if (multiSampled) {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            } else {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSampled), id, 0);

        }

        static bool IsDepthFormat(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::DEPTH24STENCIL8:return true;
            }
            return false;
        }

        static GLenum EngineTextureFormatToGL(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
                case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }

            ENGINE_CORE_ASSERT(false);
            return 0;
        }
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec) : m_Specification(spec) {

        for (auto format : m_Specification.Attachments.Attachments) {
            if (!Utils::IsDepthFormat(format.TextureFormat)) {
                m_ColorAttachmentSpecifications.emplace_back(format);
            } else {
                m_DepthAttachmentSpecification = format;
            }
        }

        Invalidate();
    }
    OpenGLFrameBuffer::~OpenGLFrameBuffer() {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);
    }
    const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const {
        return m_Specification;
    }
    void OpenGLFrameBuffer::Invalidate() {
        if (m_RendererID) {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
            glDeleteTextures(1, &m_DepthAttachment);
            m_ColorAttachments.clear();
            m_DepthAttachment = 0;
        }

        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        bool multiSample = m_Specification.Samples > 1;

        if (!m_ColorAttachmentSpecifications.empty()) {
            m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
                Utils::BindTexture(multiSample, m_ColorAttachments[i]);
                switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
                    case FrameBufferTextureFormat::RGBA8: Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
                        break;
                    case FrameBufferTextureFormat::RED_INTEGER: Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
                        break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
            Utils::CreateTextures(multiSample, &m_DepthAttachment, 1);
            Utils::BindTexture(multiSample, m_DepthAttachment);
            switch (m_DepthAttachmentSpecification.TextureFormat) {
                case FrameBufferTextureFormat::DEPTH24STENCIL8: Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
                    break;
            }
        }

        if (m_ColorAttachments.size() > 1) {
            ENGINE_CORE_ASSERT(m_ColorAttachments.size() <= 4, "Currently only support a max for 4 color attachments!");
            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(m_ColorAttachments.size(), buffers);
        } else if (m_ColorAttachments.empty()) {
            glDrawBuffer(GL_NONE);
        }

        ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }
    void OpenGLFrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }
    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixelData;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
        return pixelData;
    }
    void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
        auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];

        glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::EngineTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
    }
} // Engine
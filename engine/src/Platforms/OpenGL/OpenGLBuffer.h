#pragma once

#include "Engine/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Engine {

    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(float *vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual const BufferLayout &GetLayout() override { return m_Layout; };
        virtual void SetLayout(const BufferLayout &layout) override { m_Layout = layout; };

    private:
        uint32_t m_RenderID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
        virtual ~OpenGLIndexBuffer() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual uint32_t GetCount() const override { return m_Count; };

    private:
        uint32_t m_RenderID;
        uint32_t m_Count;
    };

} // Engine
#include "EnginePch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace Engine {

    VertexBuffer *VertexBuffer::Create(float *vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }
    IndexBuffer *IndexBuffer::Create(uint32_t *indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }
}
#include "EnginePch.h"
#include "FrameBuffer.h"

#include "Renderer.h"

#include <Platforms/OpenGL/OpenGLFrameBuffer.h>

namespace Engine {
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
        }
        ENGINE_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }
} // Engine
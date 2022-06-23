#include "EnginePch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace Engine {
    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::API::None: ENGINE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
        }
        ENGINE_CORE_ASSERT(false, "Unknown RenderAPI!");
        return nullptr;
    }
} // Engine
#include "EnginePch.h"
#include "RenderCommand.h"

#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Engine {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
} // Engine
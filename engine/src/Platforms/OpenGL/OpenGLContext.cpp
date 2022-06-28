#include "EnginePch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {
        ENGINE_CORE_ASSERT(m_WindowHandle, "Handle is null!");
    }
    void OpenGLContext::Init() {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        ENGINE_CORE_ASSERT(status, "Failed to initialize glad!");

        ENGINE_CORE_INFO("OpenGL Info:");
        ENGINE_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        ENGINE_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        ENGINE_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }
    void OpenGLContext::SwapBuffers() {
        glfwSwapBuffers(m_WindowHandle);
    }
} // Engine
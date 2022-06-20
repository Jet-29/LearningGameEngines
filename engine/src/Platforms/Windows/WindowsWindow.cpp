#include "WindowsWindow.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

#include "Platforms/OpenGL/OpenGLContext.h"

namespace Engine {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char *description) {
        ENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window *Window::Create(const WindowProps &props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props) {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() {
        Shutdown();
    }

    void WindowsWindow::OnUpdate() {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled) {
        if (enabled) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const {
        return m_Data.VSync;
    }

    void WindowsWindow::Init(const WindowProps &props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        ENGINE_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            ENGINE_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int) m_Data.Width, (int) m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = new OpenGLContext(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(
            m_Window, [](GLFWwindow *window, int width, int height) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent event(width, height);
                data.EventCallback(event);
            });

        glfwSetWindowCloseCallback(
            m_Window, [](GLFWwindow *window) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                WindowCloseEvent event;
                data.EventCallback(event);
            });

        glfwSetKeyCallback(
            m_Window, [](GLFWwindow *window, int key, int scancode, int action, int modifiers) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent event(key, 0);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent event(key);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent event(key, 1);
                        data.EventCallback(event);
                        break;
                    }
                    default: ENGINE_CORE_WARN("No key action was given");
                        break;
                }
            });
        glfwSetCharCallback(
            m_Window, [](GLFWwindow *window, unsigned int keycode) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });

        glfwSetMouseButtonCallback(
            m_Window, [](GLFWwindow *window, int button, int action, int modifiers) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                    default: ENGINE_CORE_WARN("No button action was given");
                        break;
                }
            });

        glfwSetScrollCallback(
            m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float) xOffset, (float) yOffset);
                data.EventCallback(event);
            });

        glfwSetCursorPosCallback(
            m_Window, [](GLFWwindow *window, double xPos, double yPos) {
                WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float) xPos, (float) yPos);
                data.EventCallback(event);
            });
    }
    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }

} // Engine
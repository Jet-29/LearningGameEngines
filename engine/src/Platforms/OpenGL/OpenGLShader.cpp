#include "EnginePch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine {
    OpenGLShader::OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc) {

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        const auto *source = (const GLchar *) vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);

        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(vertexShader);

            ENGINE_CORE_ERROR("{0}", infoLog.data());
            ENGINE_CORE_ASSERT(false, "Vertex shader compilation failure!");
            return;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        source = (const GLchar *) fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);

            ENGINE_CORE_ERROR("{0}", infoLog.data());
            ENGINE_CORE_ASSERT(false, "Fragment shader compilation failure!");
            return;
        }

        m_RendererID = glCreateProgram();

        glAttachShader(m_RendererID, vertexShader);
        glAttachShader(m_RendererID, fragmentShader);

        glLinkProgram(m_RendererID);

        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(m_RendererID);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            ENGINE_CORE_ERROR("{0}", infoLog.data());
            ENGINE_CORE_ASSERT(false, "Shader linking failure!");
            return;
        }

        glDetachShader(m_RendererID, vertexShader);
        glDetachShader(m_RendererID, fragmentShader);
    }
    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_RendererID);
    }
    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }
    void OpenGLShader::UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformFloat4(const std::string &name, const glm::vec4 &vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }
    void OpenGLShader::UploadUniformInt(const std::string &name, const int &value) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string &name, const float &value) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string &name, const glm::vec2 &vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, vector.x, vector.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string &name, const glm::vec3 &vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }
} // Engine
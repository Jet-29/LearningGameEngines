#include "EnginePch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Engine {

    static GLenum ShaderTypeFromString(const std::string& type) {
        if (type == "vertex") return GL_VERTEX_SHADER;
        if (type == "fragment") return GL_FRAGMENT_SHADER;
        ENGINE_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath) {
        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // extract name from file path
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
    }
    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : m_Name(name) {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(sources);
    }
    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(m_RendererID);
    }
    std::string OpenGLShader::ReadFile(const std::string& filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in) {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
        } else {
            ENGINE_CORE_ERROR("Could not open file, {0}", filepath);
        }
        return result;
    }
    std::unordered_map<uint32_t, std::string> OpenGLShader::PreProcess(const std::string& source) {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            ENGINE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }
        return shaderSources;
    }
    void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources) {

        GLuint program = glCreateProgram();
        ENGINE_CORE_ASSERT(shaderSources.size() < 3, "We only support 5 shaders per file")
        std::array<GLenum, 5> glShaderIDs;
        uint32_t glShaderIDIndex = 0;

        for (auto& kv : shaderSources) {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);

            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                ENGINE_CORE_ERROR("{0}", infoLog.data());
                ENGINE_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        m_RendererID = program;

        glLinkProgram(m_RendererID);

        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*) &isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(m_RendererID);

            for (auto id : glShaderIDs)
                glDeleteShader(id);

            ENGINE_CORE_ERROR("{0}", infoLog.data());
            ENGINE_CORE_ASSERT(false, "Shader linking failure!");
            return;
        }

        for (auto id : glShaderIDs)
            glDetachShader(m_RendererID, id);

    }
    void OpenGLShader::Bind() const {
        glUseProgram(m_RendererID);
    }
    void OpenGLShader::Unbind() const {
        glUseProgram(0);
    }
    void OpenGLShader::SetInt(const std::string& name, int value) {
        UploadUniformInt(name, value);
    }
    void OpenGLShader::SetIntArray(const std::string& name, int* value, uint32_t count) {
        UploadUniformIntArray(name, value, count);
    }
    void OpenGLShader::SetFloat(const std::string& name, float value) {
        UploadUniformFloat(name, value);
    }
    void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) {
        UploadUniformFloat2(name, value);
    }
    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
        UploadUniformFloat3(name, value);
    }
    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
        UploadUniformFloat4(name, value);
    }
    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
        UploadUniformMat4(name, value);
    }
    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
    void OpenGLShader::UploadUniformInt(const std::string& name, const int& value) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }
    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* value, uint32_t count) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1iv(location, count, value);
    }
    void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }
    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, vector.x, vector.y);
    }
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const {
        GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }
} // Engine
#pragma once

#include "Engine/Renderer/Shader.h"

namespace Engine {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        void UploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const;
        void UploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const;
        void UploadUniformInt(const std::string &name, const int &value) const;
        void UploadUniformFloat(const std::string &name, const float &value) const;
        void UploadUniformFloat2(const std::string &name, const glm::vec2 &vector) const;
        void UploadUniformFloat3(const std::string &name, const glm::vec3 &vector) const;
        void UploadUniformFloat4(const std::string &name, const glm::vec4 &vector) const;

    private:
        uint32_t m_RendererID;
    };

} // Engine
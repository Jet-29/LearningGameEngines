#pragma once

#include "Engine/Renderer/Shader.h"

namespace Engine {

    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetIntArray(const std::string& name, int* value, uint32_t count) override;

        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

        virtual const std::string& GetName() const override { return m_Name; };

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
        void UploadUniformInt(const std::string& name, const int& value) const;
        void UploadUniformIntArray(const std::string& name, int* value, uint32_t count) const;
        void UploadUniformFloat(const std::string& name, const float& value) const;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);

        void CompileOrGetVulkanBinaries(const std::unordered_map<uint32_t, std::string>& shaderSources);
        void CompileOrGetOpenGLBinaries();
        void CreateProgram();
        void Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData);

        uint32_t m_RendererID;
        std::string m_FilePath;
        std::string m_Name;

        std::unordered_map<uint32_t, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRV;

        std::unordered_map<uint32_t, std::string> m_OpenGLSourceCode;
    };

} // Engine
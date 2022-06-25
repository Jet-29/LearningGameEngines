#include "EnginePch.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace Engine {

    struct Renderer2DStorage {
        Ref<VertexArray> VertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage *s_Data;

    void Renderer2D::Init() {
        s_Data = new Renderer2DStorage();
        s_Data->VertexArray = VertexArray::Create();

        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
        };
        uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

        Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

        squareVB->SetLayout({
                                {ShaderDataType::Float3, "a_Position"},
                                {ShaderDataType::Float2, "a_TexCoord"}
                            });

        s_Data->VertexArray->AddVertexBuffer(squareVB);

        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        s_Data->VertexArray->SetIndexBuffer(indexBuffer);

        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextData, sizeof(whiteTextData));

    }
    void Renderer2D::Shutdown() {
        delete s_Data;
    }
    void Renderer2D::BeginScene(const OrthographicCamera &camera) {
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
    }
    void Renderer2D::EndScene() {

    }
    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad(glm::vec3(position, 0.0f), size, color, s_Data->WhiteTexture);
    }
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
        DrawQuad(position, size, color, s_Data->WhiteTexture);
    }
    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture) {
        DrawQuad(glm::vec3(position, 0.0f), size, glm::vec4(1.0f), texture);
    }
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture) {
        DrawQuad(position, size, glm::vec4(1.0f), texture);
    }
    void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const Ref<Engine::Texture2D> &texture) {
        DrawQuad(glm::vec3(position, 1.0), size, color, texture);
    }
    void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, const Ref<Engine::Texture2D> &texture) {
        texture->Bind(0);
        s_Data->TextureShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
        s_Data->TextureShader->SetMat4("u_Transform", transform);

        s_Data->VertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->VertexArray);
    }

} // Engine
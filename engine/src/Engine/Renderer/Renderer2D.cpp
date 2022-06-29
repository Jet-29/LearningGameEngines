#include "EnginePch.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"

namespace Engine {

    struct QuadVertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData {
        static const uint32_t MaxQuads = 1000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref < Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 0;

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init() {
        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(Engine::Renderer2DData::MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout({
                                               {ShaderDataType::Float3, "a_Position"},
                                               {ShaderDataType::Float4, "a_Color"},
                                               {ShaderDataType::Float2, "a_TexCoord"},
                                               {ShaderDataType::Float, "a_TexIndex"},
                                               {ShaderDataType::Float, "a_TexTilingFactor"}
                                           });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[Engine::Renderer2DData::MaxVertices];

        auto* quadIndices = new uint32_t[Engine::Renderer2DData::MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < Engine::Renderer2DData::MaxIndices; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Engine::Renderer2DData::MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextData, sizeof(whiteTextData));

        int32_t samplers[Engine::Renderer2DData::MaxTextureSlots];
        for (uint32_t i = 0; i < Engine::Renderer2DData::MaxTextureSlots; i++) {
            samplers[i] = i;
        }

        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Texture", samplers, Engine::Renderer2DData::MaxTextureSlots);

        s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 1.0f, 1.0f};
        s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 1.0f, 1.0f};
        s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 1.0f, 1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 1.0f, 1.0f};
    }
    void Renderer2D::Shutdown() {
        delete[] s_Data.QuadVertexBufferBase;
    }
    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
        glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 0;
    }
    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 0;
    }
    void Renderer2D::EndScene() {

        uint32_t dataSize = (uint8_t*) s_Data.QuadVertexBufferPtr - (uint8_t*) s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }
    void Renderer2D::Flush() {
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
            s_Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++;
    }
    void Renderer2D::FlushAndReset() {
        EndScene();

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 0;
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(glm::vec3(position, 0.0f), size, color, s_Data.WhiteTexture);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(position, size, color, s_Data.WhiteTexture);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawQuad(glm::vec3(position, 0.0f), size, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawQuad(position, size, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawQuad(glm::vec3(position, 0.0f), size, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawQuad(position, size, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Ref<Engine::Texture2D>& texture, float tilingFactor) {
        DrawQuad(glm::vec3(position, 1.0), size, color, texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<Engine::Texture2D>& texture, float tilingFactor) {
        auto temp = SubTexture2D::CreateFromCoords(texture, {0.0f, 0.0f}, {texture->GetWidth(), texture->GetHeight()});
        DrawQuad(position, size, color, temp, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawQuad(glm::vec3(position, 0.0f), size, color, texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        DrawQuad(transform, color, texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color, s_Data.WhiteTexture);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
        DrawRotatedQuad(position, size, rotation, color, s_Data.WhiteTexture);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(position, size, rotation, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(position, size, rotation, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color, texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor) {
        auto temp = SubTexture2D::CreateFromCoords(texture, {0.0f, 0.0f}, {texture->GetWidth(), texture->GetHeight()});
        DrawRotatedQuad(position, size, rotation, color, temp, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawRotatedQuad(glm::vec3(position, 0.0f), size, rotation, color, texture, tilingFactor);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        DrawQuad(transform, color, texture, tilingFactor);
    }
    Renderer2D::Statistics Renderer2D::GetStats() {
        return s_Data.Stats;
    }
    void Renderer2D::ResetStats() {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
        DrawQuad(transform, color, s_Data.WhiteTexture);
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor) {
        auto temp = SubTexture2D::CreateFromCoords(texture, {0.0f, 0.0f}, {texture->GetWidth(), texture->GetHeight()});
        DrawQuad(transform, glm::vec4(1.0f), temp, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& texture, float tilingFactor) {
        DrawQuad(transform, glm::vec4(1.0f), texture, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2D>& texture, float tilingFactor) {
        auto temp = SubTexture2D::CreateFromCoords(texture, {0.0f, 0.0f}, {texture->GetWidth(), texture->GetHeight()});
        DrawQuad(transform, color, temp, tilingFactor);
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture2D>& texture, float tilingFactor) {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
            FlushAndReset();
        }

        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
            if (*s_Data.TextureSlots[i] == *texture->GetTexture().get()) {
                textureIndex = (float) i;
                break;
            }
        }

        if (textureIndex == 0.0f) {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
                FlushAndReset();
            }
            textureIndex = (float) s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture->GetTexture();
            s_Data.TextureSlotIndex++;
        }

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[0];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[1];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[2];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = texture->GetTexCoords()[3];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadIndexCount += 6;
        s_Data.Stats.QuadCount++;
    }

} // Engine
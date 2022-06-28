#include "TileMapExampleLayer.h"

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
    "wwwwwwwwwwwwwwwwwwwwwwww"
    "wwwwwwwwwwwwwwwwwwwwwwww"
    "wwwwwwwwwddddwwwwwwwwwww"
    "wwwwwwdddddddddwwwwwwwww"
    "wwwwdddddddddddddwwwwwww"
    "wwwwwwdddddddddwwwwwwwww"
    "wwwwwdddddddddwwwwwwwwww"
    "wwwwwddddddddddwwwwwwwww"
    "wwdddddwdddddddwwwwwwwww"
    "wwwwddwwwddddddddwwwwwww"
    "wwdddddwwwddddwwwwwwwwww"
    "wwwwwdddddddddwwwwwwwwww"
    "wwwwwwwdddddwwwwwwwwwwww"
    "wwwwwwwwwddwwwwwwwwwwwww"
    "wwwwwwwwwwwwwwwwwwwwwwww"
    "wwwwwwwwwwwwwwwwwwwwwwww";

void TileMapExampleLayer::OnAttach() {
    m_SpriteSheet = Engine::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
    s_TextureMap['w'] = Engine::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128});
    s_TextureMap['d'] = Engine::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11}, {128, 128});

    m_MapWidth = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;
}
void TileMapExampleLayer::OnUpdate(Engine::TimeStep dt) {
    m_CameraController.OnUpdate(dt);

    Engine::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Engine::RenderCommand::Clear();

    Engine::Renderer2D::BeginScene(m_CameraController.GetCamera());
    for (uint32_t y = 0; y < m_MapHeight; y++) {
        for (uint32_t x = 0; x < m_MapWidth; x++) {
            char tileType = s_MapTiles[x + y * m_MapWidth];
            if (s_TextureMap.contains(tileType)) {
                Engine::Renderer2D::DrawQuad({x - m_MapWidth / 2.0f, y - m_MapHeight / 2.0f}, {1, 1}, s_TextureMap[tileType]);
            } else {
                ENGINE_WARN("Requesting tile type unknown");
            }
        }
    }
    Engine::Renderer2D::EndScene();

}
void TileMapExampleLayer::OnEvent(Engine::Event& event) {
    m_CameraController.OnEvent(event);
}

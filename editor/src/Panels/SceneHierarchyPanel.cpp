#include "SceneHierarchyPanel.h"
#include "Engine/Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <filesystem>

namespace Engine {
    extern const std::filesystem::path g_AssetPath;

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context) {
        SetContext(context);
    }
    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
        m_Context = context;
        m_SelectionContext = {};
    }
    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");
        m_Context->m_Registry.each([&](auto entityID) {
            Entity entity{entityID, m_Context.get()};
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_SelectionContext = {};
        }

        if (ImGui::BeginPopupContextWindow(nullptr, 1, false)) {
            if (ImGui::MenuItem("Create Empty Entity")) {
                m_Context->CreateEntity("Empty Entity");
            }
            ImGui::EndPopup();
        }

        ImGui::End();
        ImGui::Begin("Properties");
        if (m_SelectionContext) {
            DrawComponents(m_SelectionContext);
        }
        ImGui::End();
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entity) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool isOpened = ImGui::TreeNodeEx((void*) (uint64_t) (uint32_t) entity, flags, "%s", tag.c_str());
        if (ImGui::IsItemClicked()) {
            m_SelectionContext = entity;
        }
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity")) {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if (isOpened) {
            ImGui::TreePop();
        }

        if (entityDeleted) {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity) {
                m_SelectionContext = {};
            }
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float dragSpeed = 0.1f, float resetValue = 0.0f, float columnWidth = 100.0f) {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());
        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize)) {
            values.x = resetValue;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, dragSpeed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize)) {
            values.y = resetValue;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, dragSpeed);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize)) {
            values.z = resetValue;
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, dragSpeed);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns();
        ImGui::PopID();
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        if (entity.HasComponent<T>()) {
            T& component = entity.GetComponent<T>();

            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();

            bool opened = ImGui::TreeNodeEx((void*) typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if (ImGui::Button("+", ImVec2{lineHeight, lineHeight})) {
                ImGui::OpenPopup("ComponentSettings");
            }
            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings")) {
                if (ImGui::MenuItem("Remove Component")) {
                    removeComponent = true;
                }

                ImGui::EndPopup();
            }
            if (opened) {
                uiFunction(component);

                ImGui::TreePop();
            }
            if (removeComponent) {
                entity.RemoveComponent<T>();
            }
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity) {
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent")) {
            if (!m_SelectionContext.HasComponent<CameraComponent>()) {
                if (ImGui::MenuItem("Camera")) {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectionContext.HasComponent<SpriteRendererComponent>()) {
                if (ImGui::MenuItem("Sprite Renderer")) {
                    m_SelectionContext.AddComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }

            if (!m_SelectionContext.HasComponent<CircleRendererComponent>()) {
                if (ImGui::MenuItem("Circle Renderer")) {
                    m_SelectionContext.AddComponent<CircleRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }

            if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>()) {
                if (ImGui::MenuItem("Rigidbody 2D")) {
                    m_SelectionContext.AddComponent<Rigidbody2DComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }

            if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>()) {
                if (ImGui::MenuItem("Box Collider 2D")) {
                    m_SelectionContext.AddComponent<BoxCollider2DComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!m_SelectionContext.HasComponent<CircleCollider2DComponent>()) {
                if (ImGui::MenuItem("Circle Collider 2D")) {
                    m_SelectionContext.AddComponent<CircleCollider2DComponent>();
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
            DrawVec3Control("Translation", component.Translation);

            glm::vec3 rotation = glm::degrees(component.Rotation);
            DrawVec3Control("Rotation", rotation);
            component.Rotation = glm::radians(rotation);

            DrawVec3Control("Scale", component.Scale, 0.1f, 1.0f);
        });

        DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& component) {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
            const char* currentProjectionTypeString = projectionTypeStrings[(uint32_t) camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                for (int i = 0; i < 2; i++) {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType) i);
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                float fov = camera.GetPerspectiveFOV();
                if (ImGui::DragFloat("FOV", &fov, 0.01f)) {
                    camera.SetPerspectiveFOV(fov);
                }

                float nearCip = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near", &nearCip, 0.01f)) {
                    camera.SetPerspectiveNearClip(nearCip);
                }

                float farClip = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far", &farClip, 10.0f)) {
                    camera.SetPerspectiveFarClip(farClip);
                }
            }
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                float size = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &size, 0.01f)) {
                    camera.SetOrthographicSize(size);
                }

                float nearCip = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near", &nearCip, 0.01f)) {
                    camera.SetOrthographicNearClip(nearCip);
                }

                float farClip = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far", &farClip, 0.01f)) {
                    camera.SetOrthographicFarClip(farClip);
                }
                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
            }
        });

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](SpriteRendererComponent& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

            ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
                    const auto* path = (const wchar_t*) payload->Data;
                    std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
                    component.Texture = Texture2D::Create(texturePath.string());
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::DragFloat("TilingFactor", &component.TilingFactor, 0.1f);
        });

        DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](CircleRendererComponent& component) {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
            ImGui::DragFloat("Thickness", &component.Thickness, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Fade", &component.Fade, 0.001f, 0.0f, 1.0f);
        });

        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](Rigidbody2DComponent& component) {
            const char* bodyTypeStrings[] = {"Static", "Dynamic", "Kinematic"};
            const char* currentBodyTypeString = bodyTypeStrings[(int) component.Type];

            if (ImGui::BeginCombo("Body Type", currentBodyTypeString)) {
                for (int i = 0; i < 3; i++) {
                    bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
                        currentBodyTypeString = bodyTypeStrings[i];
                        component.Type = (Rigidbody2DComponent::BodyType) i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
        });

        DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](BoxCollider2DComponent& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f);
            ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.1f);
            ImGui::DragFloat("Density", &component.Density, 0.01f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.01f);
            ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f);
        });
        DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](CircleCollider2DComponent& component) {
            ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f);
            ImGui::DragFloat("Radius", &component.Radius, 0.1f);
            ImGui::DragFloat("Density", &component.Density, 0.01f);
            ImGui::DragFloat("Friction", &component.Friction, 0.01f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.01f);
            ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f);
        });

    }
    void SceneHierarchyPanel::SetSelectedEntity(Entity entity) {
        m_SelectionContext = entity;
    }
} // Engine
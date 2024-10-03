#include "Scene.hpp"
#include "Components.hpp"

namespace Dvimana 
{
    void Scene::OnUpdate(DviCore::TimeSteps deltaTime)
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
        {
            if(!nsc.Instance)
            {
                nsc.Instance = nsc.InstantiateScript();
                nsc.Instance->m_Entity = Entity{entity, this};
                nsc.Instance->OnCreate();
            }

            nsc.Instance->OnUpdate(deltaTime);
        });

        DviCore::Camera* primaryCamera{nullptr};
        glm::mat4 cameraTransform{glm::mat4(1.0f)};

        auto view = m_Registry.view<TransformComponent, CameraComponent>();
        for(auto entity : view)
        {
            auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if(camera.Primary)
            {
                primaryCamera = &camera.Camera;
                cameraTransform = transform.GetTransform();
                break;
            }
        }

        if(primaryCamera != nullptr)
        {
            DviCore::BatchRenderer::Begin(*primaryCamera, cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
            for(auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
                DviCore::BatchRenderer::Quad(transform.GetTransform(), sprite.Color);
            }

            DviCore::BatchRenderer::End();
        }
    }

    void Scene::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();
        for(auto entity : view)
        {
            auto& camera = view.get<CameraComponent>(entity);
            if(!camera.FixedAspectRatio)
            {
                camera.Camera.SetViewportSize(width, height);
            }
        }
    }

    Entity Scene::CreateEntity(const std::string & name)
    {
        Entity entity{m_Registry.create(), this};
        auto& tag = entity.AddComponent<TagComponent>(name);
        tag.Tag = name.empty() ? "unnamed" : name;
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }

    Entity::Entity(entt::entity handle, Scene * scene) :
        m_EntityHandle(handle), m_Scene(scene)
    
    {
    }

    ScenePanels::ScenePanels(const std::shared_ptr<Scene>& scene):
        m_Context(scene)
    {
    }

    void ScenePanels::SetContext(const std::shared_ptr<Scene>& scene){
        m_Context = scene;
    }

    void ScenePanels::Render(){
        ImGui::Begin("Scene Entities");

        m_Context->m_Registry.view<entt::entity>().each([this](auto entity)
        {
            Entity entt{entity, m_Context.get()};
            RenderNode(entt);
        });

        if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
        {
            m_SelectedEntity = { entt::null, nullptr };
        }

        if(ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
        {
            if(ImGui::MenuItem("Create Entity"))
            {
                m_Context->CreateEntity("Empty Entity");
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if(m_SelectedEntity)
        {
            RenderComponents(m_SelectedEntity);
        }
        ImGui::End();
    }

    void ScenePanels::RenderNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>();
        ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

        bool Opend = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.Tag.c_str());
        if(ImGui::IsItemClicked())
        {
            m_SelectedEntity = entity;
        }

        if(ImGui::BeginPopupContextItem())
        {
            if(ImGui::MenuItem("Delete Entity"))
            {
                m_Context->DestroyEntity(entity);
                if(m_SelectedEntity == entity)
                {
                    m_SelectedEntity = { entt::null, nullptr };
                    Opend = false;
                }
            }
            ImGui::EndPopup();
        }

        if(Opend)
        {
            ImGui::TreePop();
        }
    }

    static void Vec3DragController(const char* label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });

        float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
        float buffer[3] = { values.x, values.y, values.z };
        const char* component[] = { "X", "Y", "Z" };

        for(int i = 0; i < 3; i++)
        {
            ImGui::PushID(i);

            switch (i) 
            {
                case 0:
                    ImGui::PushStyleColor(ImGuiCol_Button,          { 0.8f, 0.1f, 0.15f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   { 0.9f, 0.2f, 0.2f,  1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    { 0.8f, 0.1f, 0.15f, 1.0f });
                    break;

                case 1:
                    ImGui::PushStyleColor(ImGuiCol_Button,          { 0.2f, 0.7f, 0.3f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   { 0.3f, 0.8f, 0.4f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    { 0.2f, 0.7f, 0.3f, 1.0f });
                    break;

                case 2:
                    ImGui::PushStyleColor(ImGuiCol_Button,          { 0.1f,  0.25f, 0.8f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,   { 0.2f,  0.35f, 0.2f, 1.0f });
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive,    { 0.15f, 0.25f, 0.8f, 1.0f });
                    break;
            };

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 5.0f });
            if (ImGui::Button(component[i], buttonSize)) {
                values[i] = resetValue;
            }

            ImGui::PopStyleColor(3);


            ImGui::SameLine();
            if(ImGui::DragFloat("##X", &buffer[i], 0.1f)){
                values[i] = buffer[i];
            }
            ImGui::PopItemWidth();
            ImGui::SameLine();
            ImGui::PopStyleVar();
            ImGui::PopID();
        }

        ImGui::PopStyleVar();
        ImGui::PopItemWidth();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    template<typename T, typename UIFunc>
    static void DrawComponentControls(const std::string& name, Entity entity, UIFunc uiFunc)
    {
        static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen| ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if(entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4.0f, 4.0f });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if(ImGui::Button("+", { lineHeight, lineHeight }))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if(ImGui::BeginPopup("ComponentSettings"))
            {
                if(ImGui::MenuItem("Delete Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if(open)
            {
                uiFunc(component);
                ImGui::TreePop();
            }

            if(removeComponent)
            {
                entity.RemoveComponent<T>();
            }
        }
    }

    void ScenePanels::RenderComponents(Entity entity)
    {          
        if(entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>();
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.Tag.c_str());

            if(ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag.Tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        
        if(ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponentPopup", ImGuiPopupFlags_MouseButtonLeft);

        if(ImGui::BeginPopup("AddComponentPopup", ImGuiWindowFlags_AlwaysAutoResize))
        {
            if(ImGui::MenuItem("Camera"))
            {
                m_SelectedEntity.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if(ImGui::MenuItem("Sprite"))
            {
                m_SelectedEntity.AddComponent<SpriteComponent>();  
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponentControls<TransformComponent>("Transform", entity, [](auto& component)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 10.0f, 0.0f });
            Vec3DragController("Translation", component.Translation);
            glm::vec3 rotation = glm::degrees(component.Rotation);
            Vec3DragController("Rotation", rotation);
            component.Rotation = glm::radians(rotation);
            Vec3DragController("Scale", component.Scale, 1.0f);
            ImGui::PopStyleVar();
        });

        DrawComponentControls<CameraComponent>("Camera", entity, [](auto& component)
        {
            auto& camera = component.Camera;
            ImGui::Checkbox("Primary", &component.Primary);

            const char* projectionTypeString[] = { "Orthographic", "Perspective" };
            const char* currentProjectionType = projectionTypeString[(int)camera.GetProjectionType()];

            if (ImGui::BeginCombo("Projection", currentProjectionType)) 
            {
                for (int i = 0; i < 2; i++) 
                {
                    bool isSelected = (currentProjectionType == projectionTypeString[i]);

                    if (ImGui::Selectable(projectionTypeString[i], isSelected)) 
                    {
                        currentProjectionType = projectionTypeString[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) 
            {
                float size = camera.GetOrthographicSize();
                if (ImGui::DragFloat("Size", &size))
                {
                    camera.SetOrthographicSize(size);
                }

                float nearClip = camera.GetOrthographicNearClip();
                if (ImGui::DragFloat("Near", &nearClip))
                {
                    camera.SetOrthographicNearClip(nearClip);
                }

                float farClip = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Far", &farClip))
                {
                    camera.SetOrthographicFarClip(farClip);
                }

                ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) 
            {
                float fov = camera.GetPerspectiveFov();
                if (ImGui::DragFloat("FOV", &fov))
                {
                    camera.SetPerspectiveFov(glm::radians(fov));
                }

                float nearClip = camera.GetPerspectiveNearClip();
                if (ImGui::DragFloat("Near", &nearClip))
                {
                    camera.SetPerspectiveNearClip(nearClip);
                }

                float farClip = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Far", &farClip))
                {
                    camera.SetPerspectiveFarClip(farClip);
                }
            }
        });

        DrawComponentControls<SpriteComponent>("SpriteRenderer", entity, [](auto& component)
        {
            ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
        });

    }
}

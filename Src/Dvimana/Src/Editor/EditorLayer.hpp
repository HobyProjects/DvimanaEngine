#pragma once 

#include <DviCore/DviCore.hpp>

#include "Scene.hpp"
#include "Components.hpp"
#include "Camera.hpp"

namespace Dvimana 
{
    class EditorLayer : public DviCore::Layer 
    {
        public:
            EditorLayer(const std::shared_ptr<DviCore::Window> & window, const std::shared_ptr<DviCore::ImGuiLayer> & imGuiLayer);
            virtual ~EditorLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(DviCore::TimeSteps deltaTime) override;
            virtual void OnEvent(DviCore::Event& event) override;
            virtual void OnImGuiRender() override;

        private:
            std::shared_ptr<DviCore::Window> m_Window{nullptr};
            std::shared_ptr<DviCore::ImGuiLayer> m_ImGuiLayer{nullptr};
            std::shared_ptr<DviCore::FrameBuffer> m_Framebuffer{nullptr};

            glm::vec2 m_ViewportSize{1280.0f, 720.0f};
            bool m_ViewportFocused{false}, m_ViewportHovered{false};

            Entity m_SquareEntity;
            Entity m_CameraEntity;

            std::shared_ptr<Scene> m_Scene{nullptr};
            ScenePanels m_ScenePanels;

            std::shared_ptr<DviCore::Texture> m_Texture{nullptr};
            glm::vec4 m_Color{0.2f, 0.3f, 0.8f, 1.0f};
    };
}
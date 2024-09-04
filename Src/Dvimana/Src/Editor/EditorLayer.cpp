#include "EditorLayer.hpp"

namespace Dvimana {
    EditorLayer::EditorLayer(const std::shared_ptr<Window>& window, const std::shared_ptr<ImGuiLayer>& imGuiLayer):
        Layer("EditorLayer"), m_Window(window), m_ImGuiLayer(imGuiLayer){}

    void EditorLayer::OnAttach() {
        FrameSpecifications frameSpec{};
        frameSpec.Width = 1280;
        frameSpec.Height = 720;
        m_Framebuffer = std::make_shared<FrameBuffer>(frameSpec);
        m_ViewportSize = glm::vec2((float)frameSpec.Width, (float)frameSpec.Height);
    }

    void EditorLayer::OnDetach(){

    }

    void EditorLayer::OnUpdate(TimeSteps deltaTime){
        FrameSpecifications frameSpec = m_Framebuffer->GetSpecification();
        if(m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (frameSpec.Width != m_ViewportSize.x || frameSpec.Height != m_ViewportSize.y)) {
            m_Framebuffer->ResizeFrame((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        m_Framebuffer->Bind();
        Renderer::ClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Renderer::Clear();
        m_Framebuffer->Unbind();

    }

    void EditorLayer::OnEvent(Event & event){
    }

    void EditorLayer::OnImGuiRender(){
        m_ImGuiLayer->CreateDockspace();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Scene");
        m_ImGuiLayer->AllowEvents(ImGui::IsWindowFocused() || ImGui::IsWindowHovered());
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if(viewportPanelSize.x != m_ViewportSize.x || viewportPanelSize.y != m_ViewportSize.y) {
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        ImGui::Image((ImTextureID)m_Framebuffer->FrameTextureID(), viewportPanelSize, {0, 1}, {1, 0});
        ImGui::End();
    }
}


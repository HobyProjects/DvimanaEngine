#include "EditorLayer.hpp"

namespace Dvimana 
{ 
    EditorLayer::EditorLayer(const std::shared_ptr<DviCore::Window>& window, const std::shared_ptr<DviCore::ImGuiLayer>& imGuiLayer):
        Layer("EditorLayer"), m_Window(window), m_ImGuiLayer(imGuiLayer){}

    class ScriptableCameraController : public ScriptableEntity 
    {
        public:
            ScriptableCameraController() = default;
            virtual ~ScriptableCameraController() = default;

            void OnCreate() 
            {
                DVI_INFO("ScriptableCameraController OnCreate");
            }

            void OnUpdate(DviCore::TimeSteps deltaTime) 
            {
                auto& transform = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if(DviCore::InputHandler::KeyPressed(DviCore::KEY_W))
                    transform.y += speed * deltaTime;

                if(DviCore::InputHandler::KeyPressed(DviCore::KEY_S))
                    transform.y -= speed * deltaTime;

                if(DviCore::InputHandler::KeyPressed(DviCore::KEY_A))
                    transform.x -= speed * deltaTime;

                if(DviCore::InputHandler::KeyPressed(DviCore::KEY_D))
                    transform.x += speed * deltaTime;
            }

            void OnDestroy() 
            {
                DVI_INFO("ScriptableCameraController OnDestroy");
            }
    };

    void EditorLayer::OnAttach() 
    {
        DviCore::FrameBufferSpecifications frameSpec{};
        frameSpec.Width = 1280;
        frameSpec.Height = 720;
        m_Framebuffer = std::make_shared<DviCore::FrameBuffer>(frameSpec);

        m_Scene = std::make_shared<Scene>();
        m_SquareEntity = m_Scene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        m_CameraEntity = m_Scene->CreateEntity("Camera");
        auto& camera = m_CameraEntity.AddComponent<CameraComponent>();
        camera.Primary = true;

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<ScriptableCameraController>();
        m_ScenePanels.SetContext(m_Scene);
        Serializer serializer(m_Scene);
        serializer.Serialize("Scene.yaml");
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(DviCore::TimeSteps deltaTime)
    {
        DviCore::FrameBufferSpecifications frameSpec = m_Framebuffer->GetFrameSpecification();
        if(m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && (frameSpec.Width != m_ViewportSize.x || frameSpec.Height != m_ViewportSize.y))
            m_Framebuffer->ResizeFrame((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        m_Framebuffer->Bind();
        DviCore::Renderer::ClearColor({0.243, 0.243, 0.243, 1.0f});
        DviCore::Renderer::Clear();
        DviCore::BatchRenderer::StatusReset();
        m_Scene->OnUpdate(deltaTime);
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnEvent(DviCore::Event & event)
    {
    }

    void EditorLayer::OnImGuiRender()
    {
        m_ImGuiLayer->CreateDockspace();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Performance");
        ImGui::Text("Frames Per Second    : %.1f fps", ImGui::GetIO().Framerate);
        ImGui::Text("Application Average  : %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("Viewport size        : %.1f, %.1f", m_ViewportSize.x, m_ViewportSize.y);
        ImGui::End();

        ImGui::Begin("Renderer Info");
        ImGui::Text("GL Renderer          : %s", DviCore::OpenGLInfo::GetRenderer().c_str());
        ImGui::Text("GL Vendor            : %s", DviCore::OpenGLInfo::GetVendor().c_str());
        ImGui::Text("GL Version           : %s", DviCore::OpenGLInfo::GetVersion().c_str());
        ImGui::Text("GLSL Version         : %s", DviCore::OpenGLInfo::GetGLSLVersion().c_str());
        ImGui::Text("Number Of Quads      : %d", DviCore::BatchRenderer::Status().QuadCount);
        ImGui::Text("Number Of DrawCalls  : %d", DviCore::BatchRenderer::Status().DrawCount);
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Scene");
        m_ImGuiLayer->AllowEvents(ImGui::IsWindowFocused() || ImGui::IsWindowHovered());
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if(viewportPanelSize.x != m_ViewportSize.x || viewportPanelSize.y != m_ViewportSize.y) {
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        ImGui::Image((ImTextureID)m_Framebuffer->GetColorAttachment(), viewportPanelSize, {0, 1}, {1, 0});
        ImGui::End();

        ImGui::PopStyleVar();

        m_ScenePanels.Render();
    }
}
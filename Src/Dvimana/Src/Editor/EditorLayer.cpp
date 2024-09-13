#include "EditorLayer.hpp"

namespace Dvimana {
    EditorLayer::EditorLayer(const std::shared_ptr<Window>& window, const std::shared_ptr<ImGuiLayer>& imGuiLayer):
        Layer("EditorLayer"), m_Window(window), m_ImGuiLayer(imGuiLayer){}

    class ScriptableCameraController : public ScriptableEntity {
        public:
            ScriptableCameraController() = default;
            virtual ~ScriptableCameraController() = default;

            void OnCreate() {

                DVI_INFO("ScriptableCameraController OnCreate");

            }

            void OnUpdate(TimeSteps deltaTime) {
                auto& transform = GetComponent<TransformComponent>().Translation;
                float speed = 5.0f;

                if(InputHandler::KeyPressed(KEY_W)) {
                    transform.y += speed * deltaTime;
                }
                if(InputHandler::KeyPressed(KEY_S)) {
                    transform.y -= speed * deltaTime;
                }

                if(InputHandler::KeyPressed(KEY_A)) {
                    transform.x -= speed * deltaTime;
                }
                if(InputHandler::KeyPressed(KEY_D)) {
                    transform.x += speed * deltaTime;
                }
            }

            void OnDestroy() {
                DVI_INFO("ScriptableCameraController OnDestroy");
            }
    };

    void EditorLayer::OnAttach() {
        FrameSpecifications frameSpec{};
        frameSpec.Width = 1280;
        frameSpec.Height = 720;
        m_Framebuffer = std::make_shared<FrameBuffer>(frameSpec);
        m_ViewportSize = glm::vec2((float)frameSpec.Width, (float)frameSpec.Height);

        m_Scene = std::make_shared<Scene>();
        m_SquareEntity = m_Scene->CreateEntity("Square");
        m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

        m_CameraEntity = m_Scene->CreateEntity("Camera");
        auto& camera = m_CameraEntity.AddComponent<CameraComponent>();
        camera.Primary = true;

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<ScriptableCameraController>();
        m_ScenePanels.SetContext(m_Scene);
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
        BatchRenderer::StatusReset();
        m_Scene->OnUpdate(deltaTime);
        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnEvent(Event & event){
    }

    void EditorLayer::OnImGuiRender(){
        m_ImGuiLayer->CreateDockspace();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Performance");
        ImGui::Text("Frames Per Second    : %.1f fps", ImGui::GetIO().Framerate);
        ImGui::Text("Application Average  : %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("Viewport size        : %.1f, %.1f", m_ViewportSize.x, m_ViewportSize.y);
        ImGui::End();

        ImGui::Begin("Renderer Info");
        ImGui::Text("GL Renderer          : %s", OpenGLInfo::GetRenderer().c_str());
        ImGui::Text("GL Vendor            : %s", OpenGLInfo::GetVendor().c_str());
        ImGui::Text("GL Version           : %s", OpenGLInfo::GetVersion().c_str());
        ImGui::Text("GLSL Version         : %s", OpenGLInfo::GetGLSLVersion().c_str());
        ImGui::Text("Number Of Quads      : %d", BatchRenderer::Status().QuadCount);
        ImGui::Text("Number Of DrawCalls  : %d", BatchRenderer::Status().DrawCount);
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("Scene");
        m_ImGuiLayer->AllowEvents(ImGui::IsWindowFocused() || ImGui::IsWindowHovered());
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if(viewportPanelSize.x != m_ViewportSize.x || viewportPanelSize.y != m_ViewportSize.y) {
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
        }

        ImGui::Image((ImTextureID)(uintptr_t)m_Framebuffer->FrameTextureID(), viewportPanelSize, {0, 1}, {1, 0});
        ImGui::End();

        ImGui::PopStyleVar();

        m_ScenePanels.Render();
    }
}
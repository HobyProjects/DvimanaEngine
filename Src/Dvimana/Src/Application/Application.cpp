#include "Application.hpp"
#include "EditorLayer.hpp"

namespace Dvimana {
    Application::Application(){
        m_Window = std::make_shared<Window>();
        m_LayerStack = std::make_shared<LayerStack>();
        m_ImGuiLayer = std::make_shared<ImGuiLayer>(m_Window, ImGuiColorScheme::Dark);

        EventReceiver::SetWindowCallback(m_Window, DVI_CALLBACK(Application::OnEvent));
        InputHandler::TargetWindow(m_Window);
        Renderer::Init();

        PushOverlay(m_ImGuiLayer);
        PushLayer(std::make_shared<EditorLayer>(m_Window, m_ImGuiLayer));
    }

    Application::~Application(){
        Renderer::Quit();
    }

    void Application::Run(){
        while(m_Window->GetSpecification().IsActive){
            if(m_Window->GetSpecification().State != WindowState::Minimized){
                float currentTime = (float)glfwGetTime();
                TimeSteps deltaTime = currentTime - m_LastFrameTime;
                m_LastFrameTime = currentTime;

                for(auto& layers : *m_LayerStack){
                    layers->OnUpdate(deltaTime);
                }
            }

            m_ImGuiLayer->Begin();
            for(auto& layers : *m_LayerStack){
                layers->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            m_Window->SwapBuffers();
            EventReceiver::PollEvents();
        }
    }

    void Application::OnEvent(Event & events){
        EventHandler handler(events);
        handler.Dispatch<WindowCloseEvent>(DVI_CALLBACK(Application::OnWindowClose));
        handler.Dispatch<WindowResizeEvent>(DVI_CALLBACK(Application::OnWindowResize));

        for(std::vector<std::shared_ptr<Layer>>::reverse_iterator it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it){
            if(events.Handled){
                break;
            }

            (*it)->OnEvent(events);
        }
    }

    void Application::PushLayer(std::shared_ptr<Layer> layer){
        layer->OnAttach();
        m_LayerStack->PushLayer(layer);
    }

    void Application::PushOverlay(std::shared_ptr<Layer> overlay){
        overlay->OnAttach();
        m_LayerStack->PushOverlay(overlay);
    }

    bool Application::OnWindowClose(WindowCloseEvent & windowClose){
        m_Window->GetSpecification().IsActive = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent & windowResize){
        if(m_Window->GetSpecification().State != WindowState::Minimized){
            Renderer::SetViewport(0, 0, windowResize.GetWidth(), windowResize.GetHeight());
        }

        return false;
    }
}
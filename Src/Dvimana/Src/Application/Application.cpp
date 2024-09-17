#include "Application.hpp"
#include "EditorLayer.hpp"

namespace Dvimana 
{
    Application::Application()
    {
        m_Window = std::make_shared<DviCore::Window>();
        m_LayerStack = std::make_shared<DviCore::LayerStack>();
        m_ImGuiLayer = std::make_shared<DviCore::ImGuiLayer>(m_Window, DviCore::ImGuiColorScheme::Dark);

        DviCore::EventReceiver::SetWindowCallback(m_Window, DVI_CALLBACK(Application::OnEvent));
        DviCore::InputHandler::TargetWindow(m_Window);
        DviCore::Renderer::Init();

        PushOverlay(m_ImGuiLayer);
        PushLayer(std::make_shared<EditorLayer>(m_Window, m_ImGuiLayer));
    }

    Application::~Application()
    {
        DviCore::Renderer::Quit();
    }

    void Application::Run()
    {
        while(m_Window->GetWindowSpecification().IsActive)
        {
            if(m_Window->GetWindowSpecification().State != DviCore::WindowState::Minimized)
            {
                float currentTime = (float)glfwGetTime();
                DviCore::TimeSteps deltaTime = currentTime - m_LastFrameTime;
                m_LastFrameTime = currentTime;

                for(auto& layers : *m_LayerStack)
                {
                    layers->OnUpdate(deltaTime);
                }
            }

            m_ImGuiLayer->Begin();

            for(auto& layers : *m_LayerStack)
            {
                layers->OnImGuiRender();
            }

            m_ImGuiLayer->End();

            m_Window->SwapBuffers();
            DviCore::EventReceiver::PollEvents();
        }
    }

    void Application::OnEvent(DviCore::Event & events)
    {
        DviCore::EventHandler handler(events);
        handler.Dispatch<DviCore::WindowCloseEvent>(DVI_CALLBACK(Application::OnWindowClose));
        handler.Dispatch<DviCore::WindowResizeEvent>(DVI_CALLBACK(Application::OnWindowResize));

        for(std::vector<std::shared_ptr<DviCore::Layer>>::reverse_iterator it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
        {
            if(events.Handled)
                break;

            (*it)->OnEvent(events);
        }
    }

    void Application::PushLayer(std::shared_ptr<DviCore::Layer> layer)
    {
        layer->OnAttach();
        m_LayerStack->PushLayer(layer);
    }

    void Application::PushOverlay(std::shared_ptr<DviCore::Layer> overlay)
    {
        overlay->OnAttach();
        m_LayerStack->PushOverlay(overlay);
    }

    bool Application::OnWindowClose(DviCore::WindowCloseEvent & windowClose)
    {
        m_Window->GetWindowSpecification().IsActive = false;
        return true;
    }

    bool Application::OnWindowResize(DviCore::WindowResizeEvent & windowResize)
    {
        if(m_Window->GetWindowSpecification().State != DviCore::WindowState::Minimized)
        {
            DviCore::Renderer::SetViewport(0, 0, windowResize.GetWidth(), windowResize.GetHeight());
        }

        return false;
    }
}
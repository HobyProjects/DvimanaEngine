#pragma once

#include <DviCore/DviCore.hpp>

namespace Dvimana 
{
    class Application 
    {
        public:
            Application();
            ~Application();

            void Run();
            void OnEvent(DviCore::Event& events);
            void PushLayer(std::shared_ptr<DviCore::Layer> layer);
            void PushOverlay(std::shared_ptr<DviCore::Layer> overlay);

        private:
            bool OnWindowClose(DviCore::WindowCloseEvent& windowClose);
            bool OnWindowResize(DviCore::WindowResizeEvent& windowResize);

        private:
            std::shared_ptr<DviCore::Window>        m_Window{nullptr};
            std::shared_ptr<DviCore::LayerStack>    m_LayerStack{nullptr};
            std::shared_ptr<DviCore::ImGuiLayer>    m_ImGuiLayer{nullptr};
            float m_LastFrameTime{0.0f};
    };
}
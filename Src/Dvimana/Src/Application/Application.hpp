#pragma once

#include <DviCore/DviCore.hpp>

namespace Dvimana {
    class Application {
        public:
            Application();
            ~Application();

            void Run();
            void OnEvent(Event& events);
            void PushLayer(std::shared_ptr<Layer> layer);
            void PushOverlay(std::shared_ptr<Layer> overlay);

        private:
            bool OnWindowClose(WindowCloseEvent& windowClose);
            bool OnWindowResize(WindowResizeEvent& windowResize);

        private:
            std::shared_ptr<Window> m_Window{nullptr};
            std::shared_ptr<LayerStack> m_LayerStack{nullptr};
            std::shared_ptr<ImGuiLayer> m_ImGuiLayer{nullptr};
            float m_LastFrameTime{0.0f};
    };
}
#pragma once

#include <ImGuiDocking/imgui.h>
#include <ImGuiDocking/imgui_impl_opengl3.h>
#include <ImGuiDocking/imgui_impl_glfw.h>
#include <ImGuiDocking/imgui_internal.h>

#include "Layer.hpp"
#include "Window.hpp"
#include "WindowEvent.hpp"
#include "KeyboardEvent.hpp"
#include "MouseEvent.hpp"
#include "TimeSteps.hpp"

namespace Dvimana {

    enum class ImGuiColorScheme {
        Light,
        Dark,
    };

    class ImGuiLayer final : public Layer {
        public:
            ImGuiLayer() : Layer("ImGuiLayer") {}
            ImGuiLayer(std::shared_ptr<Window> window, ImGuiColorScheme colorScheme = ImGuiColorScheme::Dark);
            virtual ~ImGuiLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnEvent(Event& events) override;

            void Begin();
            void End();
            void AllowEvents(bool allowEvents) { m_AllowEvents = allowEvents; }

            void CreateDockspace();
            void UseColorScheme(ImGuiColorScheme colorScheme);

        private:
            void UseColorDark();
            void UseColorLight();

        private:
            std::shared_ptr<Window> m_Window{nullptr};
            ImGuiColorScheme m_ColorScheme{ImGuiColorScheme::Dark};
            bool m_AllowEvents{false};
    };
}

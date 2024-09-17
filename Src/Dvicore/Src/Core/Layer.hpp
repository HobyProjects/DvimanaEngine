#pragma once 

#include <string>

#include "Event.hpp"
#include "TimeSteps.hpp"

namespace DviCore 
{
    class Layer 
    {
        public:
            Layer(const std::string& name): m_Name(name) {}
            virtual ~Layer() = default;

            virtual void OnAttach() {}
            virtual void OnDetach() {}
            virtual void OnUpdate(TimeSteps ts) {}
            virtual void OnEvent(Event& event) {}
            virtual void OnImGuiRender() {}

        private:
            std::string m_Name{std::string()};
    };
}
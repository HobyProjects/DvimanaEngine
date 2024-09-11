#include "Camera.hpp"

namespace Dvimana {
    CameraController::CameraController(float aspectRatio, bool rotation):
        m_AspectRatio(aspectRatio), m_Rotation(rotation),
        m_Bounds(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
        m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    {
    }
    void CameraController::OnUpdate(TimeSteps timeSteps){
        if(InputHandler::KeyPressed(KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * timeSteps;
        if(InputHandler::KeyPressed(KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * timeSteps;
        if(InputHandler::KeyPressed(KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * timeSteps;
        if(InputHandler::KeyPressed(KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * timeSteps;

        m_Camera.SetPosition(m_CameraPosition);

        if(m_Rotation){
            if(InputHandler::KeyPressed(KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * timeSteps;
            if(InputHandler::KeyPressed(KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * timeSteps;
            m_Camera.SetRotation(m_CameraRotation);
        }

        m_CameraTranslationSpeed = 100.0f;
    }

    void CameraController::OnEvent(Event & e){
        EventHandler handler(e);
        handler.Dispatch<MouseWheelEvent>(DVI_CALLBACK(CameraController::OnMouseWheelEvent));
        handler.Dispatch<WindowResizeEvent>(DVI_CALLBACK(CameraController::OnWindowResizeEvent));
    }


}
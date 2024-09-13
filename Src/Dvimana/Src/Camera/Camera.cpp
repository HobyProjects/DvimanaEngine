#include "Camera.hpp"

namespace Dvimana {
    CameraController::CameraController(float aspectRatio, bool rotation):
        m_AspectRatio(aspectRatio), m_RotationEnabled(rotation),
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

        if(m_RotationEnabled){
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

    void CameraController::OnWindowResize(uint32_t width, uint32_t height){
        m_AspectRatio = (float)width / (float)height;
        CalculateView();
    }

    void CameraController::CalculateView(){
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.left, m_Bounds.right,  m_Bounds.bottom, m_Bounds.top);
    }

    bool CameraController::OnMouseWheelEvent(MouseWheelEvent & e){
        m_ZoomLevel -= e.GetYOffset();
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        return false;
    }

    bool CameraController::OnWindowResizeEvent(WindowResizeEvent & e){
        OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }

    

    void SceneCamera::CalculateProjection(){
        switch(m_ProjectionType){
            case ProjectionType::Perspective:
            {
                m_Projection = glm::perspective(m_PerspectiveFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
                break;
            }
            case ProjectionType::Orthographic:
            {
                float left      = -m_OrthographicSize * m_AspectRatio * 0.5f;
                float right     =  m_OrthographicSize * m_AspectRatio * 0.5f;
                float top       =  m_OrthographicSize * 0.5f;
                float bottom    = -m_OrthographicSize * 0.5f;
                m_Projection    =  glm::ortho(left, right, bottom, top, m_OrthographicNear, m_OrthographicFar);
                break;
            }
            default:
            {
                break;
            }
        };
    }

    SceneCamera::SceneCamera(){
        CalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height){
        m_AspectRatio = (float)width / (float)height;
        CalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip){
        m_ProjectionType        = ProjectionType::Orthographic;
        m_OrthographicSize      = size;
        m_OrthographicNear      = nearClip;
        m_OrthographicFar       = farClip;
        CalculateProjection();
    }

    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip){
        m_ProjectionType    = ProjectionType::Perspective;
        m_PerspectiveFov    = fov;
        m_PerspectiveNear   = nearClip;
        m_PerspectiveFar    = farClip;
        CalculateProjection();
    }
}
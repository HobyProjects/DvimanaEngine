#pragma once

#include <DviCore/DviCore.hpp>

namespace Dvimana {

	struct CameraBounds {
		CameraBounds(float left, float right, float top, float bottom) : left(left), right(right), top(top), bottom(bottom) {};
		~CameraBounds() = default;

		float left{ 0.0f }, right{ 0.0f }, top{ 0.0f }, bottom{ 0.0f };

		float Width() const { return right - left; }
		float Height() const { return bottom - top; }
	};

	class CameraController {
		public:
			CameraController(float aspectRatio, bool rotation);
			~CameraController() = default;

			void OnUpdate(TimeSteps timeSteps);
			void OnEvent(Event& e);
			void OnWindowResize(uint32_t width, uint32_t height);

			glm::mat4 GetViewProjection() const { m_Camera.ViewProjectionMatrix();}
			const Camera2D& GetCamera() const { return m_Camera; }
			const CameraBounds& GetBounds() const { return m_Bounds; }
			const glm::vec3& GetCameraPosition() const { return m_CameraPosition; }
			void SetRotation(bool rotation) { m_Rotation = rotation; }
			void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		
		private:
			void CalculateView();
			bool OnMouseWheelEvent(MouseWheelEvent& e);
			bool OnWindowResizeEvent(WindowResizeEvent& e);

		private:
			float m_AspectRatio;
			float m_ZoomLevel = 1.0f;
			bool m_Rotation;
			Camera2D m_Camera;
			CameraBounds m_Bounds;

			glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
			float m_CameraRotation{0.0f};
			float m_CameraTranslationSpeed{100.0f};
			float m_CameraRotationSpeed{10.0f};
	};

	class SceneCamera : public Camera {
		public:
			enum class ProjectionType {
				Orthographic = 0,
				Perspective = 1
			};
		
		private:
			void CalculateProjection();

		public:
			SceneCamera();
			virtual ~SceneCamera() = default;

			ProjectionType GetProjectionType() const { return m_ProjectionType; }
			void SetProjectionType(ProjectionType type) { m_ProjectionType = type; CalculateProjection(); }

			float GetOrthographicSize() const { return m_OrthographicSize; }
			float GetOrthographicNearClip() const { return m_OrthographicNear; }
			float GetOrthographicFarClip() const { return m_OrthographicFar; }

			void SetOrthographicSize(float size) { m_OrthographicSize = size; CalculateProjection(); }
			void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; CalculateProjection(); }
			void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; CalculateProjection(); }

			float GetPerspectiveFov() const { return m_PerspectiveFov; }
			float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
			float GetPerspectiveFarClip() const { return m_PerspectiveFar; }

			void SetPerspectiveFov(float fov) { m_PerspectiveFov = fov; CalculateProjection(); }
			void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; CalculateProjection(); }
			void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; CalculateProjection(); }

			void SetViewportSize(uint32_t width, uint32_t height);
			void SetOrthographic(float size, float nearClip, float farClip);
			void SetPerspective(float fov, float nearClip, float farClip);

		private:
			ProjectionType m_ProjectionType{ProjectionType::Orthographic};
			float m_AspectRatio{1.0f};

			float m_OrthographicSize{10.0f};
			float m_OrthographicNear{-1000.0f};
			float m_OrthographicFar{1000.0f};

			float m_PerspectiveFov{45.0f};
			float m_PerspectiveNear{0.01f};
			float m_PerspectiveFar{1000.0f};
	};
}
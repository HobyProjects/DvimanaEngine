#pragma once

#include <DviCore/DviCore.hpp>

namespace Dvimana {

	struct CameraBounds 
	{
		float Left{ 0.0f }, Right{ 0.0f }, Top{ 0.0f }, Bottom{ 0.0f };

		CameraBounds(float left, float right, float top, float bottom) : Left(left), Right(right), Top(top), Bottom(bottom) {};
		~CameraBounds() = default;

		float Width() const { return Right - Left; }
		float Height() const { return Bottom - Top; }
	};

	class CameraController 
	{
		public:
			CameraController(float aspectRatio, bool rotationEnabled = false);
			~CameraController() = default;

			void OnUpdate(DviCore::TimeSteps timeSteps);
			void OnEvent(DviCore::Event& e);
			void OnWindowResize(uint32_t width, uint32_t height);

			glm::mat4 GetViewProjection() const { m_Camera.ViewProjectionMatrix();}
			const DviCore::Camera2D& GetCamera() const { return m_Camera; }
			const CameraBounds& GetBounds() const { return m_Bounds; }
			const glm::vec3& GetCameraPosition() const { return m_CameraPosition; }
			float GetCameraRotation() const { return m_CameraRotation; }
			void SetRotation(bool rotation) { m_CameraRotation = rotation; }
			void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		
		private:
			void CalculateView();
			bool OnMouseWheelEvent(DviCore::MouseWheelEvent& e);
			bool OnWindowResizeEvent(DviCore::WindowResizeEvent& e);

		private:
			float m_AspectRatio{0.0f};
			float m_ZoomLevel{1.0f};
			bool m_RotationEnabled{0.0f};
			DviCore::Camera2D m_Camera;
			CameraBounds m_Bounds;

			glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
			float m_CameraRotation{0.0f};
			float m_CameraTranslationSpeed{100.0f};
			float m_CameraRotationSpeed{10.0f};
	};

	class SceneCamera : public DviCore::Camera 
	{
		public:
			enum class ProjectionType 
			{
				Orthographic 	= 0,
				Perspective 	= 1
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
			void SetOrthographic(float size = 10.0f, float nearClip = -1.0f, float farClip = 1.0f);
			void SetPerspective(float fov = 45.0f, float nearClip = 0.01f, float farClip = 1000.0f);

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
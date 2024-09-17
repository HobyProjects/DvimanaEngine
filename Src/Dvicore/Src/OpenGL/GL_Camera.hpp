#pragma once 

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace DviCore {
    
    class Camera2D {
        public:
            Camera2D() = default;
            Camera2D(float left, float right, float bottom, float top);
            ~Camera2D() = default;

            void SetRotation(float rotation);
            void SetPosition(const glm::vec3& position);
            void SetProjection(float left, float right, float bottom, float top);

            const glm::mat4& ViewMatrix() const { return m_View; }
            const glm::mat4& ProjectionMatrix() const { return m_Projection; }
            const glm::mat4& ViewProjectionMatrix() const { return m_ViewProjection; }

        private:
            void RecalculateViewMatrix();

        private:
            glm::mat4 m_View{1.0f};
            glm::mat4 m_Projection{1.0f};
            glm::mat4 m_ViewProjection{1.0f};
            glm::vec3 m_Position{0.0f};
            float m_Rotation{0.0f};
    };

    class Camera {
        public:
            Camera() = default;
            Camera(const glm::mat4& projection): m_Projection(projection) {}
            virtual ~Camera() = default;

            const glm::mat4& GetProjectionMatirx() const { return m_Projection; }

        protected:
            glm::mat4 m_Projection{1.0f};
    };

}
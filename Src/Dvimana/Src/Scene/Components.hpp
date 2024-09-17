#pragma once 

#include "Scene.hpp"
#include "Camera.hpp"

namespace Dvimana 
{
    struct TagComponent 
    {
        std::string Tag{std::string()};
        bool Active{false};

        TagComponent() = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
        ~TagComponent() = default;
    };

    struct TransformComponent 
    {
        glm::vec3 Translation{0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation{0.0f, 0.0f, 0.0f};
        glm::vec3 Scale{1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        ~TransformComponent() = default;

        glm::mat4 GetTransform() const 
        {
            glm::mat4 T{glm::translate(glm::mat4(1.0f), Translation)};

            glm::mat4 R{glm::rotate(glm::mat4(1.0f), Rotation.x, glm::vec3(1, 0, 0)) *
                        glm::rotate(glm::mat4(1.0f), Rotation.y, glm::vec3(0, 1, 0)) *
                        glm::rotate(glm::mat4(1.0f), Rotation.z, glm::vec3(0, 0, 1))};
                        
            glm::mat4 S{glm::scale(glm::mat4(1.0f), Scale)};

            glm::mat4 TRS{ T * R * S };
            return TRS;
        }
    };

    struct SpriteComponent 
    {
        glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
        SpriteComponent() = default;
        SpriteComponent(glm::vec4 color) : Color(color) {}
        ~SpriteComponent() = default;
    };

    struct CameraComponent 
    {
        SceneCamera Camera;
        bool Primary{false};
        bool FixedAspectRatio{false};

        CameraComponent() = default;
        ~CameraComponent() = default;
    };

    struct NativeScriptComponent 
    {
        ScriptableEntity* Instance{nullptr};
        ScriptableEntity* (*InstantiateScript)() {nullptr};
        void(*DestroyScript)(NativeScriptComponent*) {nullptr};

        template<typename T>
        void Bind() {
            InstantiateScript = []() 
            {
                return static_cast<ScriptableEntity*>(new T());
            };

            DestroyScript = [](NativeScriptComponent* nsc) 
            {
                delete nsc->Instance;
                nsc->Instance = nullptr;
            };
        }

        NativeScriptComponent() = default;
        ~NativeScriptComponent() = default;
    };
        
}
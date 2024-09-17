#pragma once

#include <DviCore/DviCore.hpp>
#include <entt/entt.hpp>

namespace Dvimana 
{
    class Entity;
    class ScenePanels;

    class Scene
    {
        public:
            Scene() = default;
            ~Scene() = default;

            void OnUpdate(DviCore::TimeSteps deltaTime);
            void OnWindowResize(uint32_t width, uint32_t height);
            Entity CreateEntity(const std::string& name);

        private:
            entt::registry m_Registry;
            uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

            friend class ScenePanels;
            friend class Entity;
    };

    class Entity 
    {
        public:
            Entity() = default;
            Entity(entt::entity handle, Scene* scene);
            ~Entity() = default;

            template<typename T>
            bool HasComponent() const 
            {
                return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
            }

            template<typename T, typename... Args>
            T& AddComponent(Args&&... args) 
            {
                DVIMANA_ASSERT(!HasComponent<T>(), "Entity already has component!");
                return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
            }

            template<typename T>
            T& GetComponent() const 
            {
                DVIMANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
                return m_Scene->m_Registry.get<T>(m_EntityHandle);
            }

            template<typename T>
            void RemoveComponent() 
            {
                DVIMANA_ASSERT(HasComponent<T>(), "Entity does not have component!");
                m_Scene->m_Registry.remove<T>(m_EntityHandle);
            }

            operator bool() const { return m_EntityHandle != entt::null; }
            operator uint32_t() const { return (uint32_t)m_EntityHandle; }

            bool operator==(const Entity& other) const 
            {
                return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
            }

            bool operator!=(const Entity& other) const 
            {
                return !(*this == other);
            }

        private:
            entt::entity m_EntityHandle{entt::null};
            Scene* m_Scene{nullptr};
    };

    class ScriptableEntity 
    {
        public:
            ScriptableEntity() = default;
            virtual ~ScriptableEntity() = default;

            template<typename T>
            T& GetComponent() 
            {
                return m_Entity.GetComponent<T>();
            }

        protected:
            virtual void OnCreate() {} 
            virtual void OnUpdate(DviCore::TimeSteps deltaTime) {}
            virtual void OnDestroy() {}

        private:
            Entity m_Entity;
            friend class Scene;
    };

    class ScenePanels 
    {
        public:
            ScenePanels() = default;
            ScenePanels(const std::shared_ptr<Scene>& scene);
            ~ScenePanels() = default;

            void SetContext(const std::shared_ptr<Scene>& scene);
            void Render();

        private:
            void RenderNode(Entity entity);
            void RenderComponents(Entity entity);
        
        private:
            std::shared_ptr<Scene> m_Context{nullptr};
            Entity m_SelectedEntity;
    };
}
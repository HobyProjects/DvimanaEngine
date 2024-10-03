#include "Serializer.hpp"
#include "Components.hpp"

namespace Dvimana
{
    Serializer::Serializer(const std::shared_ptr<Scene> &scene):
        m_Scene(scene)
    {
    }

    static void SerializeEntt(YAML::Emitter& emitter, Entity& entity)
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Entity" << YAML::Value << "0x12891734D827HF7238178HBJKDHGKJKBSD";

        if(entity.HasComponent<TagComponent>())
            emitter << YAML::Key << "TagComponent" << YAML::Value << entity.GetComponent<TagComponent>().Tag;

        if(entity.HasComponent<TransformComponent>())
        {
            emitter << YAML::Key << "TransformComponent" << YAML::Value;
            emitter << YAML::BeginMap;

            auto& tc = entity.GetComponent<TransformComponent>();

            emitter << YAML::Key << "Translation" << YAML::Value << YAML::Flow;
            emitter << YAML::BeginSeq << tc.Translation.x << tc.Translation.y << tc.Translation.z;
            emitter << YAML::EndSeq;

            emitter << YAML::Key << "Rotation" << YAML::Value << YAML::Flow;
            emitter << YAML::BeginSeq << tc.Rotation.x << tc.Rotation.y << tc.Rotation.z << YAML::EndSeq;

            emitter << YAML::Key << "Scale" << YAML::Value << YAML::Flow;
            emitter << YAML::BeginSeq << tc.Scale.x << tc.Scale.y << tc.Scale.z;
            emitter << YAML::EndSeq;

            emitter << YAML::EndMap;
        }

        if(entity.HasComponent<CameraComponent>())
        {
            emitter << YAML::Key << "CameraComponent" << YAML::Value;
            emitter << YAML::BeginMap;

            auto& cameraCompo = entity.GetComponent<CameraComponent>();
            auto& camera = cameraCompo.Camera;

            emitter << YAML::Key << "Camera" << YAML::Value;
            emitter << YAML::BeginMap;
            emitter << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            emitter << YAML::Key << "ProjectionFOV" << YAML::Value << camera.GetPerspectiveFov();
            emitter << YAML::Key << "ProjectionNear" << YAML::Value << camera.GetPerspectiveNearClip();
            emitter << YAML::Key << "ProjectionFar" << YAML::Value << camera.GetPerspectiveFarClip();
            emitter << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            emitter << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
            emitter << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
            emitter << YAML::EndMap;

            emitter << YAML::Key << "Primary" << YAML::Value << cameraCompo.Primary;
            emitter << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraCompo.FixedAspectRatio;

            emitter << YAML::EndMap;
        }

        if(entity.HasComponent<SpriteComponent>())
        {
            emitter << YAML::Key << "SpriteComponent" << YAML::Value;
            emitter << YAML::BeginMap;

            auto& spriteCompo = entity.GetComponent<SpriteComponent>();
            emitter << YAML::Key << "Color" << YAML::Value << YAML::Flow;
            emitter << YAML::BeginSeq << spriteCompo.Color.r << spriteCompo.Color.g << spriteCompo.Color.b << spriteCompo.Color.a << YAML::EndSeq;
            
            emitter << YAML::EndMap;
        }

        emitter << YAML::EndMap;
    }

    void Serializer::Serialize(const std::filesystem::path &filepath)
    {
        YAML::Emitter emitter;
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "Scene" << YAML::Value << "Scene name not set";
        emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
        m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)
        {
            Entity entity{entityID, m_Scene.get()};
            if(!entity)
                return;

            SerializeEntt(emitter, entity);
        });

        emitter << YAML::EndSeq;
        emitter << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << emitter.c_str();
    }

    void Serializer::SerializeRunTime(const std::filesystem::path &filepath)
    {
        DVIMANA_ASSERT(false, "Not implemented");
    }

    bool Serializer::Deserialize(const std::filesystem::path &filepath)
    {
        return false;
    }

    bool Serializer::DeserializeRunTime(const std::filesystem::path &filepath)
    {
        DVIMANA_ASSERT(false, "Not implemented");
        return false;
    }
}
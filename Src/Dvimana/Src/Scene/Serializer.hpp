#pragma once

#include <DviCore/DviCore.hpp>
#include "Scene.hpp"

namespace Dvimana 
{
    class Serializer
    {
        public:
            Serializer(const std::shared_ptr<Scene>& scene);
            ~Serializer() = default;

            void Serialize(const std::filesystem::path& filepath);
            void SerializeRunTime(const std::filesystem::path& filepath);

            bool Deserialize(const std::filesystem::path& filepath);
            bool DeserializeRunTime(const std::filesystem::path& filepath);

        private:
            std::shared_ptr<Scene> m_Scene;
    };
}
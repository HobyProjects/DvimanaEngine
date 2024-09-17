#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace DviCore 
{
    class Shader 
    {
        public:
            Shader() = default;
            Shader(const std::string& shaderName, const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader);
            ~Shader();

            void Bind() const;
            void Unbind() const;

            uint32_t GetProgramID() const { return m_ProgramID; }
            const std::string& GetName() const { return m_Name; }
            uint32_t GetUniformLocation(const std::string& uniform);

            void Uniform(const std::string& uniform, uint32_t value);
            void Uniform(const std::string& uniform, float value);
            void Uniform(const std::string& uniform, const glm::vec2& value);
            void Uniform(const std::string& uniform, const glm::vec3& value);
            void Uniform(const std::string& uniform, const glm::vec4& value);
            void Uniform(const std::string& uniform, const glm::mat2& value);
            void Uniform(const std::string& uniform, const glm::mat3& value);
            void Uniform(const std::string& uniform, const glm::mat4& value);

        private:
            void CompileShaders(std::unordered_map<GLenum, std::string>& shaders);
            std::string ReadFile(const std::filesystem::path& path);

        private:
            uint32_t m_ProgramID{0};
            std::string m_Name{};
            std::unordered_map<std::string, uint32_t> m_UniformLocations{};
    };

    class ShaderContainer 
    {
        public:
            ShaderContainer() = default;
            ~ShaderContainer() = default;

            void EmplaceShader(const std::shared_ptr<Shader>& shader);
            std::shared_ptr<Shader> GetShader(const std::string& shaderName);
        
        private:
            std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders{};
    };
}
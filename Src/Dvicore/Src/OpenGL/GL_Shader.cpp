#include "GL_Shader.hpp"
#include "Assert.hpp"
#include "Log.hpp"

namespace DviCore 
{
    Shader::Shader(const std::string& shaderName, const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader) 
    {
        std::unordered_map<GLenum, std::string> shader_sources
        {
            { GL_VERTEX_SHADER,   ReadFile(vertexShader)  },
            { GL_FRAGMENT_SHADER, ReadFile(fragmentShader) }
        };

        CompileShaders(shader_sources);
        m_Name = shaderName;
    }

    Shader::~Shader() 
    {
        glDeleteProgram(m_ProgramID);
    }

    void Shader::Bind() const 
    {
        glUseProgram(m_ProgramID);
    }

    void Shader::Unbind() const 
    {
        glUseProgram(0);
    }

    uint32_t Shader::GetUniformLocation(const std::string& uniform) 
    {
        if(m_UniformLocations.find(uniform) != m_UniformLocations.end())
			return m_UniformLocations[uniform];

        uint32_t location = glGetUniformLocation(m_ProgramID, uniform.c_str());
        if (location >= 0) 
        {
            m_UniformLocations[uniform] = location;
            return location;
        }

        DVI_CORE_ERROR("Uniform '{}' not found in shader '{}'", uniform, m_Name);
        return -1;
    }

    void Shader::Uniform(const std::string& uniform, uint32_t value) 
    {
    	glUniform1i(GetUniformLocation(uniform), value);
    }

    void Shader::Uniform(const std::string& uniform, float value) 
    {
        glUniform1f(GetUniformLocation(uniform), value);
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec2& value) 
    {
        glUniform2fv(GetUniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec3& value) 
    {
        glUniform3fv(GetUniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec4& value) 
    {
        glUniform4fv(GetUniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat2& value) 
    {
        glUniformMatrix2fv(GetUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat3& value) 
    {
        glUniformMatrix3fv(GetUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat4& value) 
    {
        glUniformMatrix4fv(GetUniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::CompileShaders(std::unordered_map<GLenum, std::string>& shaders) 
    {
        uint32_t shader_program = glCreateProgram();
        if(!shader_program)
        {
            DVI_CORE_CRITICAL("Failed to create shader program!");
            return;
        }

        for (auto& source : shaders) {
            GLenum type = source.first;
            const std::string& src = source.second;
            uint32_t shader = glCreateShader(type);
            const char* src_cstr = src.c_str();
            glShaderSource(shader, 1, &src_cstr, nullptr);
            glCompileShader(shader);
            glAttachShader(shader_program, shader);
        }

        glLinkProgram(shader_program);
        glValidateProgram(shader_program);
        m_ProgramID = shader_program;
    }

    std::string Shader::ReadFile(const std::filesystem::path& path) 
    {
        std::string result{};
        std::ifstream in_file(path, std::ios::in | std::ios::binary);
        if (in_file) 
        {
            in_file.seekg(0, std::ios::end);
            result.resize(in_file.tellg());
            in_file.seekg(0, std::ios::beg);
            in_file.read(&result[0], result.size());
            in_file.close();

            DVI_CORE_INFO("Shader file read successfully : \n{0}", result);
            return result;
        }

        DVIMANA_ASSERT(false, "Failed to read shader file!");
        return std::string();
    }

    void ShaderContainer::EmplaceShader(const std::shared_ptr<Shader>& shader) 
    {
        DVIMANA_ASSERT(m_Shaders.find(shader->GetName()) == m_Shaders.end(), "Shader already exists!");
        m_Shaders[shader->GetName()] = shader;
    }

    std::shared_ptr<Shader> ShaderContainer::GetShader(const std::string& name) 
    {
		DVIMANA_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

}
#include "OpenGL.hpp"
#include "Log.hpp"
#include "Assert.hpp"

#ifdef _DEBUG

static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    const char* type_str        = nullptr;
    const char* severity_str    = nullptr;
    const char* source_str      = nullptr;
    
    switch(type){
        case GL_DEBUG_TYPE_ERROR:                   type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_ERROR); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:     type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:      type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR); break;
        case GL_DEBUG_TYPE_PORTABILITY:             type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_PORTABILITY); break;
        case GL_DEBUG_TYPE_PERFORMANCE:             type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_PERFORMANCE); break;
        case GL_DEBUG_TYPE_MARKER:                  type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_MARKER); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:              type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_PUSH_GROUP); break;
        case GL_DEBUG_TYPE_POP_GROUP:               type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_POP_GROUP); break;
        case GL_DEBUG_TYPE_OTHER:                   type_str = DVI_MACTOSTRING(GL_DEBUG_TYPE_OTHER); break;
        default:                                    type_str = "GL_DEBUG_TYPE_UNKNOWN"; break;
    }
    
    switch(severity){
        case GL_DEBUG_SEVERITY_HIGH:         severity_str = DVI_MACTOSTRING(GL_DEBUG_SEVERITY_HIGH); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       severity_str = DVI_MACTOSTRING(GL_DEBUG_SEVERITY_MEDIUM); break;
        case GL_DEBUG_SEVERITY_LOW:          severity_str = DVI_MACTOSTRING(GL_DEBUG_SEVERITY_LOW); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severity_str = DVI_MACTOSTRING(GL_DEBUG_SEVERITY_NOTIFICATION); break;
        default:                             severity_str = "GL_DEBUG_SEVERITY_UNKNOWN"; break;
    }

    switch(source){
        case GL_DEBUG_SOURCE_API:             source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_API); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_WINDOW_SYSTEM); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_SHADER_COMPILER); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_THIRD_PARTY); break;
        case GL_DEBUG_SOURCE_APPLICATION:     source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_APPLICATION); break;
        case GL_DEBUG_SOURCE_OTHER:           source_str = DVI_MACTOSTRING(GL_DEBUG_SOURCE_OTHER); break;
        default:                              source_str = "GL_DEBUG_SOURCE_UNKNOWN"; break;
    }

    DVI_CORE_CRITICAL("<---------< GL DEBUG MESSAGE >--------->");
    DVI_CORE_CRITICAL(" -> ID:         {0}", id);
    DVI_CORE_CRITICAL(" -> SOURCE:     {0}", source_str);
    DVI_CORE_CRITICAL(" -> TYPE:       {0}", type_str);
    DVI_CORE_CRITICAL(" -> SEVERITY:   {0}", severity_str);
    DVI_CORE_CRITICAL(" -> MESSAGE:    {0}", message);
    DVI_CORE_CRITICAL("<------------------------------------------>");

    if(severity == GL_DEBUG_SEVERITY_HIGH)
        DVIMANA_DEBUGBREAK();
}

#endif


namespace Dvimana {

    std::string OpenGLInfo::GetVendor(){
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        return std::string(vendor);
    }

    std::string OpenGLInfo::GetRenderer(){
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        return std::string(renderer);
    }

    std::string OpenGLInfo::GetVersion(){
        const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        return std::string(version);
    }

    std::string OpenGLInfo::GetGLSLVersion(){
        const char* version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        char GL_MAJOR = version[0];
        char GL_MINOR = version[2];
        char GL_PATCH = version[4];

        std::stringstream ss;
        ss << "#version " << GL_MAJOR << GL_MINOR << GL_PATCH << " core";
        return ss.str();
    }

    OpenGLContext::OpenGLContext(GLFWwindow * window) : m_Window(window){}

    bool OpenGLContext::MakeContext(){
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(1);

        if(GLenum status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); status != GL_TRUE){
            DVIMANA_ASSERT(false, "Failed to initialize GLAD!");
            return false;
        }

        DVI_CORE_INFO("GLEW initialized");
        return true;
    }

    void OpenGLContext::SwapBuffers(){
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwSwapBuffers(m_Window);
    }

    void OpenGLContext::SwapInterval(uint32_t interval){
        DVIMANA_ASSERT(m_Window, "GLFW window is null!");
        glfwSwapInterval(interval);
    }

    VertexBuffer::VertexBuffer(uint32_t size) {
        glGenBuffers(1, &m_BufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::VertexBuffer(float * vertices, uint32_t size) {
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

    VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_BufferID);
	}

	void VertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void * data, uint32_t size) const {
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	IndexBuffer::IndexBuffer(uint32_t * indices, uint32_t count) : m_Count(count) {
		glGenBuffers(1, &m_BufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_BufferID);
	}

	void IndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	}

	void IndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_VertexArrayID);
    }

    VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

    void VertexArray::Bind() const {
		glBindVertexArray(m_VertexArrayID);
	}

    void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}

    void VertexArray::EmplaceVtxBuffer(const std::shared_ptr<VertexBuffer>& buffer) {
        glBindVertexArray(m_VertexArrayID);
        buffer->Bind();
        const auto& layout = buffer->GetLayout();
        const auto& elements = layout.GetElements();
        uint32_t layout_index{ 0 };
        for (auto& element : elements) {
            glEnableVertexAttribArray(layout_index);
            glVertexAttribPointer(
                layout_index,
                static_cast<GLint>(element.Components),
                GL_FLOAT,
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)(element.Offset)
            );
        }

        m_VtxBuffers.emplace_back(buffer);
    }

    void VertexArray::EmplaceIdxBuffer(const std::shared_ptr<IndexBuffer>& buffer) {
        glBindVertexArray(m_VertexArrayID);
		buffer->Bind();
		m_IdxBuffer = buffer;
    }

    Shader::Shader(const std::string& name, const std::filesystem::path& vtx_path, const std::filesystem::path& frag_path) {
        std::unordered_map<GLenum, std::string> shader_sources{
            {GL_VERTEX_SHADER, ReadFile(vtx_path)},
            {GL_FRAGMENT_SHADER, ReadFile(frag_path)}
        };

        CompileShaders(shader_sources);
        m_Name = name;
    }

    Shader::~Shader() {
        glDeleteProgram(m_ProgramID);
    }

    void Shader::Bind() const {
        glUseProgram(m_ProgramID);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    uint32_t Shader::UniformLocation(const std::string& uniform) {
        if(m_UniformLocations.find(uniform) != m_UniformLocations.end())
			return m_UniformLocations[uniform];

        uint32_t location = glGetUniformLocation(m_ProgramID, uniform.c_str());
        if (location >= 0) {
            m_UniformLocations[uniform] = location;
            return location;
        }

        DVIMANA_ASSERT(false, "Uniform not found!");
        return -1;
    }

    void Shader::Uniform(const std::string& uniform, uint32_t value) {
    	glUniform1i(UniformLocation(uniform), value);
    }

    void Shader::Uniform(const std::string& uniform, float value) {
        glUniform1f(UniformLocation(uniform), value);
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec2& value) {
        glUniform2fv(UniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec3& value) {
        glUniform3fv(UniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::vec4& value) {
        glUniform4fv(UniformLocation(uniform), 1, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat2& value) {
        glUniformMatrix2fv(UniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat3& value) {
        glUniformMatrix3fv(UniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Uniform(const std::string& uniform, const glm::mat4& value) {
        glUniformMatrix4fv(UniformLocation(uniform), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::CompileShaders(std::unordered_map<GLenum, std::string>& shaders) {
        uint32_t shader_program = glCreateProgram();
        DVIMANA_ASSERT(shader_program, "Failed to create shader program!");
        std::vector<uint32_t> shader_ids;
        shader_ids.reserve(shaders.size());

        for (auto& source : shaders) {
            GLenum type = source.first;
            const std::string& src = source.second;
            uint32_t shader = glCreateShader(type);
            const char* src_cstr = src.c_str();
            glShaderSource(shader, 1, &src_cstr, nullptr);
            glCompileShader(shader);
            glAttachShader(shader_program, shader);
            shader_ids.emplace_back(shader);
        }

        glLinkProgram(shader_program);
        glValidateProgram(shader_program);

        for (auto& shader : shader_ids) {
            glDetachShader(shader_program, shader);
			glDeleteShader(shader);
		}

        m_ProgramID = shader_program;
    }

    std::string Shader::ReadFile(const std::filesystem::path& path) {
        std::string result{};
        std::ifstream in_file(path, std::ios::in | std::ios::binary);
        if (in_file) {
            in_file.seekg(0, std::ios::end);
            result.resize(in_file.tellg());
            in_file.seekg(0, std::ios::beg);
            in_file.read(&result[0], result.size());
            in_file.close();
            return result;
        }

        DVIMANA_ASSERT(false, "Failed to read shader file!");
        return std::string();
    }

    void ShaderContainer::EmplaceShader(const std::shared_ptr<Shader>& shader) {
        DVIMANA_ASSERT(m_Shaders.find(shader->Name()) == m_Shaders.end(), "Shader already exists!");
        m_Shaders[shader->Name()] = shader;
    }

    std::shared_ptr<Shader> ShaderContainer::GetShader(const std::string& name) {
		DVIMANA_ASSERT(m_Shaders.find(name) != m_Shaders.end(), "Shader not found!");
		return m_Shaders[name];
	}

    Texture::Texture(uint32_t width, uint32_t height) {
        m_Width = width;
        m_Height = height;
        m_Channels = 4;

        m_Data = new uint8_t[m_Width * m_Height * m_Channels];
        std::memset(m_Data, 255, m_Width * m_Height * m_Channels);

        GLenum internal_format = GL_RGBA8, data_format = GL_RGBA;
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_Width, m_Height, 0, data_format, GL_UNSIGNED_BYTE, m_Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        m_FromImageFile = false;
    }

    Texture::Texture(const std::filesystem::path& path, bool flip) {
        if(!std::filesystem::exists(path)){
			DVI_CORE_ERROR("{0} Texture file does not exist!", path.string());
			return;
		}

        stbi_set_flip_vertically_on_load(flip);
        m_Data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (!m_Data) {
            DVI_CORE_ERROR("Failed to load texture file!");
            return;
        }

        GLenum internal_format = 0, data_format = 0;
        if (m_Channels == 4) {
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		} else if (m_Channels == 3) {
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}

        glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_Width, m_Height, 0, data_format, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_FromImageFile = true;
    }

    Texture::~Texture() {
        (m_FromImageFile) ? stbi_image_free(m_Data) : delete[] m_Data;
        glDeleteTextures(1, &m_TextureID);
    }

    void Texture::Bind(uint32_t slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max) {
    	m_Texture = texture;
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    std::shared_ptr<SubTexture> SubTexture::MakeSubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size) {
        float tex_width = static_cast<float>(texture->Width());
        float tex_height = static_cast<float>(texture->Height());
        glm::vec2 min = { (coords.x * cell_size.x) / tex_width, (coords.y * cell_size.y) / tex_height };
        glm::vec2 max = { ((coords.x + sprite_size.x) * cell_size.x) / tex_width, ((coords.y + sprite_size.y) * cell_size.y) / tex_height };
        return std::make_shared<SubTexture>(texture, min, max);
    }

    FrameBuffer::FrameBuffer(const FrameSpecifications& specification) {
        m_Specification = specification;
		CreateFrame();
    }

    FrameBuffer::~FrameBuffer() {
        glDeleteFramebuffers(1, &m_FrameID);
        glDeleteRenderbuffers(1, &m_FrameRendererID);
        glDeleteTextures(1, &m_FrameTextureID);
    }

    void FrameBuffer::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void FrameBuffer::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::ResizeFrame(uint32_t width, uint32_t height) {
        m_Specification.Width = width;
		m_Specification.Height = height;

        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);

        glBindTexture(GL_TEXTURE_2D, m_FrameTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameTextureID, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_FrameRendererID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_FrameRendererID);

        DVIMANA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::CreateFrame() {
        glGenFramebuffers(1, &m_FrameID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameID);

        glGenTextures(1, &m_FrameTextureID);
		glBindTexture(GL_TEXTURE_2D, m_FrameTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FrameTextureID, 0);

		glGenRenderbuffers(1, &m_FrameRendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_FrameRendererID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_FrameRendererID);

		DVIMANA_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    static const uint32_t MAX_QUADS = 10000;
    static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
    static const uint32_t MAX_INDICES = MAX_QUADS * 6;
    static const uint32_t MAX_TEXTURE_SLOTS = 32;
    static const uint32_t MAX_QUAD_VERTEX_COUNT = 4;
    static const glm::vec4 DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const glm::vec2 DEFAULT_TEX_COORDS[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};


    struct Vertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
		float TilingFactor;
	};

    struct BatchData {
        uint32_t QuadVAO{ 0 };
        uint32_t QuadVBO{ 0 };
        uint32_t QuadIBO{ 0 };

        std::shared_ptr<Texture> PlainTexture{ nullptr };
        uint32_t PlainTextureSlot{ 0 };
        uint32_t IndexCount{ 0 };

        Vertex* QuadBuffer{ nullptr };
        Vertex* QuadBufferPtr{ nullptr };

        std::shared_ptr<Shader> BatchShader{ nullptr };
        std::array<std::shared_ptr<Texture>, MAX_TEXTURE_SLOTS> TextureSlots;
        uint32_t TextureSlotIndex{ 1 };

        BatchRenderer::RendererStatus Status;
        glm::vec4 QuadVertexPositions[MAX_QUAD_VERTEX_COUNT];

        bool CallWithInScope{false};
        bool BatchRestart{false};

    }; static BatchData s_BatchData;


    void BatchRenderer::Restart() {
        End();
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
        s_BatchData.IndexCount = 0;
        s_BatchData.TextureSlotIndex = 1;
    }

    void BatchRenderer::Init() {
        s_BatchData.QuadBuffer = new Vertex[MAX_VERTICES];
        glGenVertexArrays(1, &s_BatchData.QuadVAO);
        glBindVertexArray(s_BatchData.QuadVAO);

        glGenBuffers(1, &s_BatchData.QuadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, s_BatchData.QuadVBO);
        glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TilingFactor));

        uint32_t indices[MAX_INDICES]{};
        uint32_t offset = 0;
        for (uint32_t i = 0; i < MAX_INDICES; i += 6) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        glGenBuffers(1, &s_BatchData.QuadIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_BatchData.QuadIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(uint32_t), indices, GL_STATIC_DRAW);

        s_BatchData.PlainTexture = std::make_shared<Texture>(1, 1);
        s_BatchData.TextureSlots[0] = s_BatchData.PlainTexture;
        for(uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
			s_BatchData.TextureSlots[i] = nullptr;

        s_BatchData.BatchShader = std::make_shared<Shader>("BatchShader", "Shaders/BatchVertex.glsl", "Shaders/BatchFragment.glsl");

        s_BatchData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_BatchData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
        s_BatchData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
        s_BatchData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }

    void BatchRenderer::Quit() {
        delete[] s_BatchData.QuadBuffer;
		glDeleteVertexArrays(1, &s_BatchData.QuadVAO);
		glDeleteBuffers(1, &s_BatchData.QuadVBO);
		glDeleteBuffers(1, &s_BatchData.QuadIBO);
    }

    void BatchRenderer::Begin(const Camera2D& camera) {
        s_BatchData.BatchShader->Bind();
        s_BatchData.BatchShader->Uniform("u_MVP", camera.ViewProjectionMatrix());

        uint32_t texture_location = s_BatchData.BatchShader->UniformLocation("u_Textures");
        int32_t samplers[MAX_TEXTURE_SLOTS];
        for(uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) 
            samplers[i] = i;
        glUniform1iv(texture_location, MAX_TEXTURE_SLOTS, samplers);
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
        s_BatchData.CallWithInScope = true;
    }

    void BatchRenderer::Begin(const Camera& camera, const glm::mat4& transform) {
        glm::mat4 MVP = camera.GetProjectionMatirx() * glm::inverse(transform);
        s_BatchData.BatchShader->Bind();
        s_BatchData.BatchShader->Uniform("u_MVP", MVP);

        uint32_t texture_location = s_BatchData.BatchShader->UniformLocation("u_Textures");
        int32_t samplers[MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
            samplers[i] = i;
        glUniform1iv(texture_location, MAX_TEXTURE_SLOTS, samplers);
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
        s_BatchData.CallWithInScope = true;
    }

    void BatchRenderer::End() {
        GLsizeiptr size = (uint8_t*)s_BatchData.QuadBufferPtr - (uint8_t*)s_BatchData.QuadBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, s_BatchData.QuadVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_BatchData.QuadBuffer);
        Flush();
    }

    void BatchRenderer::Flush() {
        for(uint32_t i = 0; i < s_BatchData.TextureSlotIndex; i++) {
            s_BatchData.TextureSlots[i]->Bind(i);
		}

		glBindVertexArray(s_BatchData.QuadVAO);
		glDrawElements(GL_TRIANGLES, s_BatchData.IndexCount, GL_UNSIGNED_INT, nullptr);
        s_BatchData.Status.DrawCount++;
		s_BatchData.IndexCount = 0;
		s_BatchData.TextureSlotIndex = 1;

        if(s_BatchData.BatchRestart)
            s_BatchData.CallWithInScope = true;
        else
            s_BatchData.CallWithInScope = false;
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        Quad({ position.x, position.y, 0.0f }, size, color, s_BatchData.PlainTexture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
        Quad({ position.x, position.y, 0.0f }, size, color, s_BatchData.PlainTexture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture) {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation) {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor) {
        Quad({ position.x, position.y, 0.0f }, size, color, texture, 0.0f, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor) {
        Quad({ position.x, position.y, 0.0f }, size, color, texture, rotation, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture) {
		Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
	}

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation) {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor) {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
    	Quad(position, size, color, s_BatchData.PlainTexture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
        Quad(position, size, color, s_BatchData.PlainTexture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture) {
    	Quad(position, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation) {
        Quad(position, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor) {
    	Quad(position, size, color, texture, 0.0f, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor) {           
        DVIMANA_ASSERT(s_BatchData.CallWithInScope, "BatchRenderer::Begin() must be called before rendering quads!");
        
        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
            s_BatchData.BatchRestart = true;
            Restart();
            s_BatchData.BatchRestart = false;
        }

        float texture_index = 0.0f;
        for(uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) {
			if (s_BatchData.TextureSlots[i] == texture) {
				texture_index = static_cast<float>(i);
				break;
			}
		}

        if(texture_index == 0.0f) {
			texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
			s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture;
			s_BatchData.TextureSlotIndex++;
		}

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)     * 
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })    * 
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for(uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) {
			s_BatchData.QuadBufferPtr->Position             = transform * s_BatchData.QuadVertexPositions[i];
			s_BatchData.QuadBufferPtr->Color                = color;
			s_BatchData.QuadBufferPtr->TexCoords            = DEFAULT_TEX_COORDS[i];
			s_BatchData.QuadBufferPtr->TexIndex             = texture_index;
			s_BatchData.QuadBufferPtr->TilingFactor         = tiling_factor;
			s_BatchData.QuadBufferPtr++;
		}

		s_BatchData.IndexCount += 6;
		s_BatchData.Status.QuadCount++;
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture) {
        Quad(position, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation) {
        Quad(position, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor) {
        DVIMANA_ASSERT(s_BatchData.CallWithInScope, "BatchRenderer::Begin() must be called before rendering quads!");

        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
			s_BatchData.BatchRestart = true;
			Restart();
			s_BatchData.BatchRestart = false;
		}

        const glm::vec2* tex_coords = texture->GetTexCoords();
        float texture_index = 0.0f;

        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) {
            if (s_BatchData.TextureSlots[i] == texture->TexturePtr()) {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if(texture_index == 0.0f) {
			texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
			s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture->TexturePtr();
			s_BatchData.TextureSlotIndex++;
		}

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)     * 
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })    * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) {
			s_BatchData.QuadBufferPtr->Position         = transform * s_BatchData.QuadVertexPositions[i];
			s_BatchData.QuadBufferPtr->Color            = color;
			s_BatchData.QuadBufferPtr->TexCoords        = tex_coords[i];
			s_BatchData.QuadBufferPtr->TexIndex         = texture_index;
			s_BatchData.QuadBufferPtr->TilingFactor     = tiling_factor;
			s_BatchData.QuadBufferPtr++;
		}

		s_BatchData.IndexCount += 6;
		s_BatchData.Status.QuadCount++;
    }

    void BatchRenderer::Quad(const glm::mat4& transform, const glm::vec4& color) {
        Quad(transform, s_BatchData.PlainTexture, color, 1.0f);
    }

    void BatchRenderer::Quad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint, float tiling) {
        DVIMANA_ASSERT(s_BatchData.CallWithInScope, "BatchRenderer::Begin() must be called before rendering quads!");

        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
            s_BatchData.BatchRestart = true;
            Restart();
            s_BatchData.BatchRestart = false;
        }

        float texture_index = 0.0f;
        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) {
            if (s_BatchData.TextureSlots[i] == texture) {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if (texture_index == 0.0f) {
            texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
            s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture;
            s_BatchData.TextureSlotIndex++;
        }


        for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) {
            s_BatchData.QuadBufferPtr->Position         = transform * s_BatchData.QuadVertexPositions[i];
            s_BatchData.QuadBufferPtr->Color            = tint;
            s_BatchData.QuadBufferPtr->TexCoords        = DEFAULT_TEX_COORDS[i];
            s_BatchData.QuadBufferPtr->TexIndex         = texture_index;
            s_BatchData.QuadBufferPtr->TilingFactor     = tiling;
            s_BatchData.QuadBufferPtr++;
        }

        s_BatchData.IndexCount += 6;
        s_BatchData.Status.QuadCount++;
    }

    void BatchRenderer::Quad(const glm::mat4& transform, const std::shared_ptr<SubTexture>& texture, const glm::vec4& tint, float tiling) {
        DVIMANA_ASSERT(s_BatchData.CallWithInScope, "BatchRenderer::Begin() must be called before rendering quads!");

        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
            s_BatchData.BatchRestart = true;
            Restart();
            s_BatchData.BatchRestart = false;
        }

        const glm::vec2* tex_coords = texture->GetTexCoords();
        float texture_index = 0.0f;

        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) {
            if (s_BatchData.TextureSlots[i] == texture->TexturePtr()) {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if (texture_index == 0.0f) {
            texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
            s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture->TexturePtr();
            s_BatchData.TextureSlotIndex++;
        }


        for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) {
            s_BatchData.QuadBufferPtr->Position             = transform * s_BatchData.QuadVertexPositions[i];
            s_BatchData.QuadBufferPtr->Color                = tint;
            s_BatchData.QuadBufferPtr->TexCoords            = tex_coords[i];
            s_BatchData.QuadBufferPtr->TexIndex             = texture_index;
            s_BatchData.QuadBufferPtr->TilingFactor         = tiling;
            s_BatchData.QuadBufferPtr++;
        }

        s_BatchData.IndexCount += 6;
        s_BatchData.Status.QuadCount++;
    }

    const BatchRenderer::RendererStatus& BatchRenderer::Status() {
        return s_BatchData.Status;
    }

    void BatchRenderer::StatusReset() {
        s_BatchData.Status.DrawCount = 0;
        s_BatchData.Status.QuadCount = 0;
    }

    void Renderer::Init() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

        #ifdef DVIMANA_DEBUG

        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

        #endif

        BatchRenderer::Init();
    }

    void Renderer::Quit() {
        BatchRenderer::Quit();
    }

    void Renderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::ClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::SetViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) {
	    glViewport(x, y, width, height);
    }
}
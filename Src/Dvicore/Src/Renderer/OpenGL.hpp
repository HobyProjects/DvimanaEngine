#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <initializer_list>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <algorithm>

#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <StbImage/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Log.hpp"
#include "Assert.hpp"

namespace Dvimana {

    class OpenGLInfo {
        public:
            static std::string GetVendor();
            static std::string GetRenderer();
            static std::string GetVersion();
            static std::string GetGLSLVersion();
    };

    class OpenGLContext {
        public:
            OpenGLContext() = default;
            OpenGLContext(GLFWwindow* window);
            ~OpenGLContext() = default;

            bool MakeContext();
            void SwapBuffers();
            void SwapInterval(uint32_t interval);
        
        private:
            GLFWwindow* m_Window{nullptr};
    };

    enum class BufferComponents {
        XY      = 2,
        UV      = 2,
        XYZ     = 3,
        RGB     = 3,
        RGBA    = 4,
        XYZW    = 4,
    };

    enum class BufferStride {
        BOOLEAN     = 1,    F1          = 4,    F2          = 8,
        F3          = 12,   F4          = 16,   I1          = 4,
        I2          = 8,    I3          = 12,   I4          = 16,
        UI1         = 4,    UI2         = 8,    UI3         = 12,
        UI4         = 16,   MAT2        = 16,   MAT3        = 36,   MAT4        = 64,
    };

    struct BufferElements {
        BufferElements() = default;
        BufferElements(const std::string& name, BufferComponents components, BufferStride stride, bool normalized) : 
                Name(name), Components(components), Stride(stride), Normalized(normalized) {}
        ~BufferElements() = default;

        std::string Name{};
        BufferComponents Components{};
        BufferStride Stride{};
        int32_t Offset{0};
        bool Normalized{false};
    };

    class BufferLayout {
        public:
            BufferLayout() = default;
            BufferLayout(std::initializer_list<BufferElements> elements) : m_Elements(elements) { CalStride(); }
            ~BufferLayout() = default;

            uint32_t GetStride() const { return m_Stride; }
            const std::vector<BufferElements>& GetElements() const { return m_Elements; }

            std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
            std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
            std::vector<BufferElements>::const_iterator begin() const { return m_Elements.begin(); }
            std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); }

        private:
            void CalStride(){
                m_Stride = 0;
                int32_t offset = 0;
                for(auto& element : m_Elements){
                    element.Offset = offset;
                    m_Stride += static_cast<uint32_t>(element.Stride);
                    offset += static_cast<uint32_t>(element.Stride);
                }
            }
        
        private:
            std::vector<BufferElements> m_Elements{};
            uint32_t m_Stride{0};
    };

    class VertexBuffer {
        public:
            VertexBuffer(uint32_t size);
            VertexBuffer(float* vertices, uint32_t size);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;
            uint32_t ID() const { return m_BufferID; }

            void SetData(const void* data, uint32_t size) const;
            void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
            const BufferLayout& GetLayout() const { return m_Layout; }

        private:
            uint32_t m_BufferID{0};
            BufferLayout m_Layout;
    };

    class IndexBuffer {
        public:
            IndexBuffer(uint32_t* indices, uint32_t count);
            ~IndexBuffer();

            void Bind() const;
            void Unbind() const;
            uint32_t ID() const { return m_BufferID; }
            uint32_t GetCount() const { return m_Count; }

        private:
            uint32_t m_BufferID{0};
            uint32_t m_Count{0};
    };

    class VertexArray {
        public:
            VertexArray();
            ~VertexArray();

            void Bind() const;
            void Unbind() const;
            uint32_t ID() const { return m_VertexArrayID; }

            void EmplaceVtxBuffer(const std::shared_ptr<VertexBuffer>& buffer);
            void EmplaceIdxBuffer(const std::shared_ptr<IndexBuffer>& buffer);
            const std::vector<std::shared_ptr<VertexBuffer>>& GetVtxBuffers() const { return m_VtxBuffers; }
            const std::shared_ptr<IndexBuffer>& GetIdxBuffer() const { return m_IdxBuffer; }
        
        private:
            uint32_t m_VertexArrayID{0};
            std::vector<std::shared_ptr<VertexBuffer>> m_VtxBuffers{};
            std::shared_ptr<IndexBuffer> m_IdxBuffer{nullptr};
    };

    class Shader {
        public:
            Shader() = default;
            Shader(const std::string& name, const std::filesystem::path& vtx_path, const std::filesystem::path& frag_path);
            ~Shader();

            void Bind() const;
            void Unbind() const;

            uint32_t ProgramID() const { return m_ProgramID; }
            const std::string& Name() const { return m_Name; }
            uint32_t UniformLocation(const std::string& uniform);

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

    class ShaderContainer {
        public:
            ShaderContainer() = default;
            ~ShaderContainer() = default;

            void EmplaceShader(const std::shared_ptr<Shader>& shader);
            std::shared_ptr<Shader> GetShader(const std::string& name);
        
        private:
            std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders{};
    };

    class Texture {
        public:
            Texture(uint32_t width, uint32_t height);
            Texture(const std::filesystem::path& path, bool flip = true);
            ~Texture();

            void Bind(uint32_t slot = 0) const;
            void Unbind() const;

            uint32_t ID() const { return m_TextureID; }
            int32_t Width() const { return m_Width; }
            int32_t Height() const { return m_Height; }
            int32_t Channels() const { return m_Channels; }
            void* TextureData() const { return m_Data; }

            bool operator==(const Texture& other) const { return m_TextureID == other.m_TextureID; }

        private:
            int32_t m_Width{0}, m_Height{0}, m_Channels{0};
            void* m_Data{nullptr};
            uint32_t m_TextureID{0};
            bool m_FromImageFile{false};
    };

    class SubTexture {
        public:
            SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max);
            ~SubTexture() = default;

            const std::shared_ptr<Texture>& TexturePtr() { return m_Texture; }
            const glm::vec2* GetTexCoords() const { return m_TexCoords; }
            static std::shared_ptr<SubTexture> MakeSubTexture(const std::shared_ptr<Texture>& texture, 
                const glm::vec2& coords, const glm::vec2& cell_size, const glm::vec2& sprite_size = {1.0f, 1.0f});

        private:
            static const uint32_t NUMBER_OF_TEXTURE_COORDS = 4;
            std::shared_ptr<Texture> m_Texture;
            glm::vec2 m_TexCoords[NUMBER_OF_TEXTURE_COORDS];
    };

    struct FrameSpecifications {
        uint32_t Width{0}, Height{0};
        bool SwapChainTarget{false};
    };

    class FrameBuffer {
        public:
            FrameBuffer(const FrameSpecifications& specification);
            ~FrameBuffer();

            void Bind() const;
            void Unbind() const;
            void ResizeFrame(uint32_t width, uint32_t height);

            uint32_t FrameID() const { return m_FrameID; }
            uint32_t FrameTextureID() const { return m_FrameTextureID; }
            FrameSpecifications& GetSpecification() { return m_Specification; }

        private:
            void CreateFrame();

        private:
            uint32_t m_FrameID{0};
            uint32_t m_FrameTextureID{0};
            uint32_t m_FrameRendererID{0};
            FrameSpecifications m_Specification;
    };

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

    class BatchRenderer {
        private:
            BatchRenderer() = default;
            ~BatchRenderer() = default;

            static void Restart();

        public:
            static void Init();
            static void Quit();

            static void Begin(const Camera2D& camera);
            static void Begin(const Camera& camera, const glm::mat4& transform);
            static void End();
            static void Flush();

            static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor = 1.0f);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor = 1.0f);
            
            static void Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation);
            static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor = 1.0f);

            static void Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor = 1.0f);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor = 1.0f);
            
            static void Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation);
            static void Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor = 1.0f);

            static void Quad(const glm::mat4& transform, const glm::vec4& color);
            static void Quad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tiling = 1.0f);
            static void Quad(const glm::mat4& transform, const std::shared_ptr<SubTexture>& texture, const glm::vec4& tint = glm::vec4(1.0f), float tiling = 1.0f);

            struct RendererStatus {
                uint32_t DrawCount{0};
                uint32_t QuadCount{0};
            };

            static const RendererStatus& Status();
            static void StatusReset();
    };

    class Renderer {
        private:
            Renderer() = default;
			~Renderer() = default;

        public:
            static void Init();
            static void Quit();

            static void Clear();
            static void ClearColor(const glm::vec4& color);
            static void SetViewport(int32_t x, int32_t y, uint32_t width, uint32_t height);
    };
}
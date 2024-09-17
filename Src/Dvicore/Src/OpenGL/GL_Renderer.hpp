#pragma once 

#include <array>

#include "GL_VertexArray.hpp"
#include "GL_Buffers.hpp"
#include "GL_Texture.hpp"
#include "GL_Shader.hpp"
#include "GL_Camera.hpp"
#include "GL_Debug.hpp"

namespace DviCore 
{
    class BatchRenderer 
    {
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

            struct RendererStatus 
            {
                uint32_t DrawCount{0};
                uint32_t QuadCount{0};
            };

            static const RendererStatus& Status();
            static void StatusReset();
    };

    class Renderer 
    {
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
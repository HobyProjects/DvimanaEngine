#pragma once 

#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glad/glad.h>

#include <StbImage/stb_image.h>

namespace DviCore
{
    class Texture 
    {
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
            GLenum GetInternalFormat() const { return m_InternalFormat; }
            GLenum GetDataFormat() const { return m_DataFormat; }
            void* TextureData() const { return m_Data; }
            bool operator==(const Texture& other) const { return m_TextureID == other.m_TextureID; }

        private:
            int32_t m_Width{0}, m_Height{0}, m_Channels{0};
            void* m_Data{nullptr};
            uint32_t m_TextureID{0};
            bool m_FromImageFile{false};
            GLenum m_InternalFormat{0}, m_DataFormat{0};
    };

    class SubTexture 
    {
        public:
            SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max);
            ~SubTexture() = default;

            const std::shared_ptr<Texture>& TexturePtr() { return m_Texture; }
            const glm::vec2* GetTexCoords() const { return m_TexCoords; }
            GLenum GetInternalFormat() const { return m_InternalFormat; }
            GLenum GetDataFormat() const { return m_DataFormat; }
            static std::shared_ptr<SubTexture> MakeSubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = {1.0f, 1.0f});

        private:
            static const uint32_t NUMBER_OF_TEXTURE_COORDS = 4;
            std::shared_ptr<Texture> m_Texture;
            glm::vec2 m_TexCoords[NUMBER_OF_TEXTURE_COORDS];
            GLenum m_InternalFormat{0}, m_DataFormat{0};
    };
}
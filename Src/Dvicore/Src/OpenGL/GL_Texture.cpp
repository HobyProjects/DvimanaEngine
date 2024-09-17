#include "GL_Texture.hpp"
#include "Log.hpp"

namespace DviCore {

    Texture::Texture(uint32_t width, uint32_t height) 
    {
        m_Width = width;
        m_Height = height;
        m_Channels = 4;

        m_Data = new uint8_t[m_Width * m_Height * m_Channels];
        std::memset(m_Data, 255, m_Width * m_Height * m_Channels);

        m_InternalFormat = GL_RGBA8; 
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, m_Data);
        glGenerateMipmap(GL_TEXTURE_2D);

        m_FromImageFile = false;
    }

    Texture::Texture(const std::filesystem::path& path, bool flip) 
    {
        if(!std::filesystem::exists(path))
        {
			DVI_CORE_ERROR("{0} Texture file does not exist!", path.string());
			return;
		}

        stbi_set_flip_vertically_on_load(flip);
        m_Data = stbi_load(path.string().c_str(), &m_Width, &m_Height, &m_Channels, 0);

        if (!m_Data) 
        {
            DVI_CORE_ERROR("Failed to load texture file -> {0}!", path.string());
            return;
        }

        if (m_Channels == 4) 
        {
			m_InternalFormat    = GL_RGBA8;
			m_DataFormat        = GL_RGBA;
		} 
        else
        {
			m_InternalFormat    = GL_RGB8;
			m_DataFormat        = GL_RGB;
		}

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);
		m_FromImageFile = true;
    }

    Texture::~Texture() 
    {
        (m_FromImageFile) ? stbi_image_free(m_Data) : delete[] m_Data;
        glDeleteTextures(1, &m_TextureID);
    }

    void Texture::Bind(uint32_t slot) const 
    {
        glBindTextureUnit(slot, m_TextureID);
    }

    void Texture::Unbind() const 
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& min, const glm::vec2& max) 
    {
    	m_Texture = texture;
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    std::shared_ptr<SubTexture> SubTexture::MakeSubTexture(const std::shared_ptr<Texture>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize) 
    {
        float textureWidth = static_cast<float>(texture->Width());
        float textureHeight = static_cast<float>(texture->Height());
        glm::vec2 min = { (coords.x * cellSize.x) / textureWidth, (coords.y * cellSize.y) / textureHeight };
        glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / textureWidth, ((coords.y + spriteSize.y) * cellSize.y) / textureHeight };
        return std::make_shared<SubTexture>(texture, min, max);
    }
}
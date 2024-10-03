#include "GL_Renderer.hpp"
#include "Assert.hpp"

namespace DviCore 
{
    static const uint32_t MAX_QUADS                 = 10000;
    static const uint32_t MAX_VERTICES              = MAX_QUADS * 4;
    static const uint32_t MAX_INDICES               = MAX_QUADS * 6;
    static const uint32_t MAX_TEXTURE_SLOTS         = 32;
    static const uint32_t MAX_QUAD_VERTEX_COUNT     = 4;
    static const glm::vec4 DEFAULT_COLOR            = { 1.0f, 1.0f, 1.0f, 1.0f };
    static const glm::vec2 DEFAULT_TEX_COORDS[]     = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    struct Vertex 
    {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoords;
		float TexIndex;
		float TilingFactor;
	};

    struct BatchData 
    {
        uint32_t QuadVAO{0};
        uint32_t QuadVBO{0};
        uint32_t QuadIBO{0};

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

    }; static BatchData s_BatchData;


    void BatchRenderer::Restart() 
    {
        End();
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
        s_BatchData.IndexCount = 0;
        s_BatchData.TextureSlotIndex = 1;
    }

    void BatchRenderer::Init() 
    {
        s_BatchData.QuadBuffer = new Vertex[MAX_VERTICES];

        glCreateVertexArrays(1, &s_BatchData.QuadVAO);
        glBindVertexArray(s_BatchData.QuadVAO);
        {
            glCreateBuffers(1, &s_BatchData.QuadVBO);
            glBindBuffer(GL_ARRAY_BUFFER, s_BatchData.QuadVBO);
            glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
            {
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexIndex));

                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TilingFactor));
            }

            int32_t indices[MAX_INDICES];
            uint32_t offset = 0;
            for(uint32_t i = 0; i < MAX_INDICES; i += 6){
                indices[i + 0] = offset + 0;
                indices[i + 1] = offset + 1;
                indices[i + 2] = offset + 2;

                indices[i + 3] = offset + 2;
                indices[i + 4] = offset + 3;
                indices[i + 5] = offset + 0;

                offset += 4;
            }

            glCreateBuffers(1, &s_BatchData.QuadIBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_BatchData.QuadIBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(uint32_t), indices, GL_STATIC_DRAW);

            for(uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
                s_BatchData.TextureSlots[i] = nullptr;

            s_BatchData.PlainTexture = std::make_shared<Texture>(1, 1);
            s_BatchData.TextureSlots[0] = s_BatchData.PlainTexture;

            s_BatchData.BatchShader = std::make_shared<Shader>("BatchShader", "Shaders/BatchVertex.glsl", "Shaders/BatchFragment.glsl");

            s_BatchData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            s_BatchData.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
            s_BatchData.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
            s_BatchData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
        }
        glBindVertexArray(0);
    }

    void BatchRenderer::Quit() 
    {
        delete[] s_BatchData.QuadBuffer;
    }

    void BatchRenderer::Begin(const Camera2D& camera) 
    {
        s_BatchData.BatchShader->Bind();
        s_BatchData.BatchShader->Uniform("u_MVP", camera.ViewProjectionMatrix());

        uint32_t texture_location = s_BatchData.BatchShader->GetUniformLocation("u_Textures");
        int32_t samplers[MAX_TEXTURE_SLOTS];
        for(uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++) 
            samplers[i] = i;

        glUniform1iv(texture_location, MAX_TEXTURE_SLOTS, samplers);
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
    }

    void BatchRenderer::Begin(const Camera& camera, const glm::mat4& transform) 
    {
        glm::mat4 MVP = camera.GetProjectionMatirx() * glm::inverse(transform);
        s_BatchData.BatchShader->Bind();
        s_BatchData.BatchShader->Uniform("u_MVP", MVP);

        uint32_t texture_location = s_BatchData.BatchShader->GetUniformLocation("u_Textures");
        int32_t samplers[MAX_TEXTURE_SLOTS];
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
            samplers[i] = i;
            
        glUniform1iv(texture_location, MAX_TEXTURE_SLOTS, samplers);
        s_BatchData.QuadBufferPtr = s_BatchData.QuadBuffer;
    }

    void BatchRenderer::End() 
    {
        GLsizeiptr size = (uint8_t*)s_BatchData.QuadBufferPtr - (uint8_t*)s_BatchData.QuadBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, s_BatchData.QuadVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_BatchData.QuadBuffer);
        Flush();
    }

    void BatchRenderer::Flush() 
    {
        for(uint32_t i = 0; i < s_BatchData.TextureSlotIndex; i++) 
        {
            s_BatchData.TextureSlots[i]->Bind(i);
		}

		glBindVertexArray(s_BatchData.QuadVAO);
		glDrawElements(GL_TRIANGLES, s_BatchData.IndexCount, GL_UNSIGNED_INT, nullptr);

        s_BatchData.Status.DrawCount++;
		s_BatchData.IndexCount = 0;
		s_BatchData.TextureSlotIndex = 1;
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) 
    {
        Quad({ position.x, position.y, 0.0f }, size, color, s_BatchData.PlainTexture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) 
    {
        Quad({ position.x, position.y, 0.0f }, size, color, s_BatchData.PlainTexture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture) 
    {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation) 
    {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor) 
    {
        Quad({ position.x, position.y, 0.0f }, size, color, texture, 0.0f, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor) 
    {
        Quad({ position.x, position.y, 0.0f }, size, color, texture, rotation, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture) 
    {
		Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
	}

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation) 
    {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor) 
    {
        Quad({ position.x, position.y, 0.0f }, size, DEFAULT_COLOR, texture, rotation, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) 
    {
    	Quad(position, size, color, s_BatchData.PlainTexture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) 
    {
        Quad(position, size, color, s_BatchData.PlainTexture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture) 
    {
    	Quad(position, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, float rotation) 
    {
        Quad(position, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float tiling_factor) 
    {
    	Quad(position, size, color, texture, 0.0f, tiling_factor);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<Texture>& texture, float rotation, float tiling_factor) 
    {           
        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) 
        {
            Restart();
        }

        float texture_index = 0.0f;
        for(uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) 
        {
			if (s_BatchData.TextureSlots[i] == texture) 
            {
				texture_index = static_cast<float>(i);
				break;
			}
		}

        if(texture_index == 0.0f) 
        {
			texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
			s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture;
			s_BatchData.TextureSlotIndex++;
		}

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)     * 
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })    * 
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for(uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) 
        {
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

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture) 
    {
        Quad(position, size, DEFAULT_COLOR, texture, 0.0f, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const std::shared_ptr<SubTexture>& texture, float rotation) 
    {
        Quad(position, size, DEFAULT_COLOR, texture, rotation, 1.0f);
    }

    void BatchRenderer::Quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const std::shared_ptr<SubTexture>& texture, float rotation, float tiling_factor) 
    {
        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) 
        {
			Restart();
		}

        const glm::vec2* tex_coords = texture->GetTexCoords();
        float texture_index = 0.0f;

        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) 
        {
            if (s_BatchData.TextureSlots[i] == texture->TexturePtr()) {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if(texture_index == 0.0f) 
        {
			texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
			s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture->TexturePtr();
			s_BatchData.TextureSlotIndex++;
		}

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)     * 
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })    * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) 
        {
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

    void BatchRenderer::Quad(const glm::mat4& transform, const glm::vec4& color) 
    {
        Quad(transform, s_BatchData.PlainTexture, color, 1.0f);
    }

    void BatchRenderer::Quad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint, float tiling) 
    {
        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) {
            Restart();
        }

        float texture_index = 0.0f;
        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) 
        {
            if (s_BatchData.TextureSlots[i] == texture) 
            {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if (texture_index == 0.0f) 
        {
            texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
            s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture;
            s_BatchData.TextureSlotIndex++;
        }


        for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) 
        {
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

    void BatchRenderer::Quad(const glm::mat4& transform, const std::shared_ptr<SubTexture>& texture, const glm::vec4& tint, float tiling) 
    {
        if (s_BatchData.IndexCount >= MAX_INDICES || s_BatchData.TextureSlotIndex >= MAX_TEXTURE_SLOTS) 
        {
            Restart();
        }

        const glm::vec2* tex_coords = texture->GetTexCoords();
        float texture_index = 0.0f;

        for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++) 
        {
            if (s_BatchData.TextureSlots[i] == texture->TexturePtr()) 
            {
                texture_index = static_cast<float>(i);
                break;
            }
        }

        if (texture_index == 0.0f) 
        {
            texture_index = static_cast<float>(s_BatchData.TextureSlotIndex);
            s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = texture->TexturePtr();
            s_BatchData.TextureSlotIndex++;
        }


        for (uint32_t i = 0; i < MAX_QUAD_VERTEX_COUNT; i++) 
        {
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

    const BatchRenderer::RendererStatus& BatchRenderer::Status() 
    {
        return s_BatchData.Status;
    }

    void BatchRenderer::StatusReset() 
    {
        s_BatchData.Status.DrawCount = 0;
        s_BatchData.Status.QuadCount = 0;
    }

    void Renderer::Init() 
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);

#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(DebugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        BatchRenderer::Init();
    }

    void Renderer::Quit() 
    {
        BatchRenderer::Quit();
    }

    void Renderer::Clear() 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::ClearColor(const glm::vec4& color) 
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Renderer::SetViewport(int32_t x, int32_t y, uint32_t width, uint32_t height) 
    {
	    glViewport(x, y, width, height);
    }
}
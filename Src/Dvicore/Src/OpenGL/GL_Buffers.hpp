#pragma once 

#include <string>
#include <vector>
#include <initializer_list>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace DviCore 
{
    enum class BufferComponents 
    {
        X       = 1,
        XY      = 2,
        UV      = 2,
        XYZ     = 3,
        RGB     = 3,
        RGBA    = 4,
        XYZW    = 4,
        MAT3    = 3,
        MAT4    = 4,
    };

    enum class BufferStride 
    {
        BOOLEAN     = sizeof(bool),        
        F1          = sizeof(float),       
        F2          = sizeof(float) * 2,
        F3          = sizeof(float) * 3,   
        F4          = sizeof(float) * 4, 
        MAT3        = sizeof(float) * 3 * 3,   
        MAT4        = sizeof(float) * 4 * 4,
    };

    struct BufferElements 
    {
        int32_t Offset{0};
        std::string Name{};
        BufferStride Stride{};
        bool Normalized{false};
        BufferComponents Components{};

        BufferElements() = default;
        BufferElements(const std::string& name, BufferComponents components, BufferStride stride, bool normalized) : 
                Name(name), Components(components), Stride(stride), Normalized(normalized) {}
        ~BufferElements() = default;
    };

    class BufferLayout 
    {
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
            void CalStride()
            {
                m_Stride        = 0;
                int32_t offset  = 0;

                for(auto& element : m_Elements)
                {
                    element.Offset = offset;
                    m_Stride += static_cast<uint32_t>(element.Stride);
                    offset += static_cast<uint32_t>(element.Stride);
                }
            }
        
        private:
            std::vector<BufferElements> m_Elements{};
            uint32_t m_Stride{0};
    };

    class VertexBuffer 
    {
        public:
            VertexBuffer() = default;
            VertexBuffer(uint32_t size);
            VertexBuffer(float* vertices, uint32_t size);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;
            uint32_t ID() const { return m_VertexBufferID; }

            void SetData(const void* data, uint32_t size) const;
            void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
            const BufferLayout& GetLayout() const { return m_Layout; }

        private:
            uint32_t m_VertexBufferID{0};
            BufferLayout m_Layout;
    };

    class IndexBuffer 
    {
        public:
            IndexBuffer() = default;
            IndexBuffer(uint32_t* indices, uint32_t count);
            ~IndexBuffer();

            void Bind() const;
            void Unbind() const;
            uint32_t ID() const { return m_IndexBufferID; }
            uint32_t GetCount() const { return m_Count; }

        private:
            uint32_t m_IndexBufferID{0};
            uint32_t m_Count{0};
    };
}
#include "GL_VertexArray.hpp"
#include "Log.hpp"

namespace DviCore 
{    
    VertexArray::VertexArray() 
    {
        glCreateVertexArrays(1, &m_VertexArrayID);
    }

    VertexArray::~VertexArray() 
    {
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

    void VertexArray::Bind() const 
    {
		glBindVertexArray(m_VertexArrayID);
	}

    void VertexArray::Unbind() const 
    {
		glBindVertexArray(0);
	}

    static GLenum GetDataType(BufferStride component) 
    {
        switch (component) 
        {
            case BufferStride::F1: return GL_FLOAT;
            case BufferStride::F2: return GL_FLOAT;
            case BufferStride::F3: return GL_FLOAT;
            case BufferStride::F4: return GL_FLOAT;
            case BufferStride::MAT3: return GL_FLOAT;
            case BufferStride::MAT4: return GL_FLOAT;
            default: return 0;
        }

        DVI_CORE_ERROR("Unknown component type");
        return -1;
    }

    void VertexArray::EmplaceVtxBuffer(const std::shared_ptr<VertexBuffer>& buffer) 
    {
        glBindVertexArray(m_VertexArrayID);
        buffer->Bind();
        const auto& layout = buffer->GetLayout();
        const auto& elements = layout.GetElements();
        uint32_t layout_index{ 0 };

        for (auto& element : elements) 
        {
            glEnableVertexAttribArray(layout_index);
            glVertexAttribPointer(
                layout_index,
                static_cast<GLint>(element.Components),
                GL_FLOAT,
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset
            );
        }

        m_VtxBuffers.emplace_back(buffer);
    }

    void VertexArray::EmplaceIdxBuffer(const std::shared_ptr<IndexBuffer>& buffer) 
    {
        glBindVertexArray(m_VertexArrayID);
		m_IdxBuffer = buffer;
		m_IdxBuffer->Bind();
    }
}
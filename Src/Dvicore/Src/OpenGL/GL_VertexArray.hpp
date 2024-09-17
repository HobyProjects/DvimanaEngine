#pragma once 

#include <memory>
#include "GL_Buffers.hpp"

namespace DviCore
{
    class VertexArray 
    {
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
}

#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace DviCore 
{
    struct FrameBufferSpecifications 
    {
        uint32_t Width{0}, Height{0};
        bool SwapChainTarget{false};
    };

    class FrameBuffer 
    {
        public:
            FrameBuffer(const FrameBufferSpecifications& specification);
            ~FrameBuffer();

            void Bind() const;
            void Unbind() const;
            void ResizeFrame(uint32_t width, uint32_t height);

            uint32_t GetFrameBufferID() const { return m_FrameBufferID; }
            uint32_t GetColorAttachment() const { return m_ColorAttachment; }
            FrameBufferSpecifications& GetFrameSpecification() { return m_Specification; }

        private:
            void CreateFrame();

        private:
            uint32_t m_FrameBufferID{0};
            uint32_t m_ColorAttachment{0};
            uint32_t m_DepthAttachment{0};

            FrameBufferSpecifications m_Specification;
    };
}
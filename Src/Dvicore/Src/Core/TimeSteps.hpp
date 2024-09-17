#pragma once

namespace DviCore 
{
    class TimeSteps 
    {
        public:
            TimeSteps(float deltaTime) : m_DeltaTime(deltaTime) {}
            ~TimeSteps() = default;

            float GetDeltaTime() const { return m_DeltaTime; }
            float GetDeltaTimeSeconds() const { return m_DeltaTime; }
            float GetDeltaTimeMilliseconds() const { return m_DeltaTime * 1000.0f; }

            operator float() const { return m_DeltaTime; }

        private:
            float m_DeltaTime{0.0f};
    };
}
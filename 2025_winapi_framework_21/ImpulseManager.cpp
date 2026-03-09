#include "pch.h"
#include "ImpulseManager.h"

void ImpulseManager::Update()
{
    if (m_isShaking)
    {
        int x = (rand() % 100 - 50) * m_impulse / 50;
        int y = (rand() % 100 - 50) * m_impulse / 50;
        m_offset = { x, y };
        m_duration -= fDT;
        m_impulse -= m_decrease * fDT;
        if (m_duration < 0.f)
        {
            m_isShaking = false;
            m_offset = { 0, 0 };
        }
    }
}

void ImpulseManager::ApplyImpulse(float _impulse, float _duration)
{
    m_isShaking = true;
    m_impulse = _impulse;
    m_duration = _duration;
    m_decrease = m_impulse / m_duration;
}
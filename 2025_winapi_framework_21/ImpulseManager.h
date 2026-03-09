#pragma once
class ImpulseManager
{
    DECLARE_SINGLE(ImpulseManager);
public:
    void Update();
public:
    Vec2 GetOffset() { return m_offset; }
    void ApplyImpulse(float _intensity, float _duration);
private:
    Vec2 m_offset;
    bool m_isShaking;
    float m_impulse;
    float m_duration;
    float m_decrease;
};
#pragma once
#include "pch.h"
#include <functional>
#include <utility>

// 간단한 트윈 이징 타입
enum class EaseType
{
    Linear,
    EaseInQuad,
    EaseOutQuad,
    EaseInOutQuad,

    EaseInExpo,
    EaseOutExpo,
    EaseInOutExpo,
};

inline float EaseEval(EaseType type, float t)
{
    t = std::clamp(t, 0.f, 1.f);
    switch (type)
    {
        case EaseType::Linear:        return t;
        case EaseType::EaseInQuad:    return t * t;
        case EaseType::EaseOutQuad:   return t * (2.f - t);
        case EaseType::EaseInOutQuad: return t < 0.5f ? 2.f * t * t : -1.f + (4.f - 2.f * t) * t;
        case EaseType::EaseInExpo:
            return (t == 0.f) ? t : std::pow(2.f, 10.f * t - 10.f);
        case EaseType::EaseOutExpo:
            return (t == 1.f) ? t : (-std::pow(2.f, -10.f * t) + 1.f);
        case EaseType::EaseInOutExpo:
            return  (t == 0.f) ? 0 : (t == 1.f) ? 1 : (t < 0.5f) ? std::pow(2, 20 * t - 10) / 2  : (2 - std::pow(2, -20 * t + 10)) / 2;
        default: return t;
    }
}

// 내부 업데이트용 인터페이스
class ITween
{
public:
    virtual ~ITween() = default;
    // true 반환 시 완료되어 제거됨
    virtual bool Update(float dt) = 0;
};

// 보간 유틸
template<typename T>
inline T Lerp(const T& a, const T& b, float t)
{
    return a + (b - a) * t; // Vec2, float 지원
}

// 특수화: float은 동일 구현이지만 명확히 표시
template<>
inline float Lerp<float>(const float& a, const float& b, float t)
{
    return a + (b - a) * t;
}

// 특수화: Vec2는 연산자 오버로드가 const가 아니므로 직접 계산
template<>
inline Vec2 Lerp<Vec2>(const Vec2& a, const Vec2& b, float t)
{
    return Vec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

// 트윈 본체 (값 타입 T)
template<typename T>
class Tween : public ITween
{
public:
    using Getter = std::function<T()>;
    using Setter = std::function<void(const T&)>;
    using Callback = std::function<void()>;

public:
    Tween(Getter getter, Setter setter, const T& to, float duration, EaseType ease)
        : m_getter(std::move(getter))
        , m_setter(std::move(setter))
        , m_start{}
        , m_to(to)
        , m_elapsed(0.f)
        , m_duration(max(FLT_EPSILON, duration))
        , m_ease(ease)
        , m_started(false)
    {
    }

    void SetOnComplete(Callback cb) { m_onComplete = std::move(cb); }

    virtual bool Update(float dt) override
    {
        if (!m_started)
        {
            m_start = m_getter ? m_getter() : T{};
            m_started = true;
        }

        m_elapsed += dt;
        float t = std::clamp(m_elapsed / m_duration, 0.f, 1.f);
        float et = EaseEval(m_ease, t);
        T cur = Lerp<T>(m_start, m_to, et);
        if (m_setter)
            m_setter(cur);

        if (m_elapsed >= m_duration)
        {
            if (m_setter)
                m_setter(m_to); // 최종값 스냅
            if (m_onComplete)
                m_onComplete();
            return true; // 완료
        }
        return false; // 계속
    }

private:
    Getter   m_getter;
    Setter   m_setter;
    T        m_start;
    T        m_to;
    float    m_elapsed;
    float    m_duration;
    EaseType m_ease;
    bool     m_started;
    Callback m_onComplete;
};

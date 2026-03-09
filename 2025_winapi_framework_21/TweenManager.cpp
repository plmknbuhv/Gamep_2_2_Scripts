#include "pch.h"
#include "TweenManager.h"
#include "Object.h"

void TweenManager::Update(float dt)
{
    // 새로 추가된 트윈을 안전하게 반영
    if (!m_pending.empty())
    {
        for (auto& it : m_pending)
            m_items.emplace_back(std::move(it));
        m_pending.clear();
    }

    // 업데이트 및 완료 제거
    for (size_t i = 0; i < m_items.size();)
    {
        auto& tw = m_items[i];
        bool done = tw->Update(dt);
        if (done)
        {
            // 빠른 제거: swap-pop
            if (i != m_items.size() - 1)
                m_items[i] = std::move(m_items.back());
            m_items.pop_back();
        }
        else
        {
            ++i;
        }
    }
}

ITween* TweenManager::MoveTo(Object* obj, const Vec2& to, float duration, EaseType ease, std::function<void()> onComplete)
{
    if (obj == nullptr)
        return nullptr;
    auto getter = [obj]() -> Vec2 { return obj->GetPos(); };
    auto setter = [obj](const Vec2& v) { obj->SetPos(v); };
    return Start<Vec2>(getter, setter, to, duration, ease, onComplete);
}
ITween* TweenManager::ScaleTo(Object* obj, const Vec2& to, float duration, EaseType ease, std::function<void()> onComplete)
{
    if (obj == nullptr)
        return nullptr;
    auto getter = [obj]() -> Vec2 { return obj->GetSize(); };
    auto setter = [obj](const Vec2& v) { obj->SetSize(v); };
    return Start<Vec2>(getter, setter, to, duration, ease, onComplete);
}


void TweenManager::Clear()
{
    m_items.clear();
    m_pending.clear();
}

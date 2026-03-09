#pragma once
#include "pch.h"
#include "Tween.h"
#include <functional>
#include <utility>
#include "Object.h"

// 간단한 전역 트윈 관리자
class TweenManager
{
    DECLARE_SINGLE(TweenManager);
public:
    void Update(float dt);

    // 제네릭 트윈 시작 API
    template<typename T>
    ITween* Start(std::function<T()> getter,
                  std::function<void(const T&)> setter,
                  const T& to,
                  float duration,
                  EaseType ease,
                  std::function<void()> onComplete = {})
    {
        auto tw = std::make_unique<Tween<T>>(std::move(getter), std::move(setter), to, duration, ease);
        if (onComplete)
            tw->SetOnComplete(std::move(onComplete));
        ITween* raw = tw.get();
        m_pending.emplace_back(std::move(tw));
        return raw;
    }

    // Object 위치 이동 편의 함수
    ITween* MoveTo(Object* obj, const Vec2& to, float duration, EaseType ease, std::function<void()> onComplete = {});
    ITween* ScaleTo(Object* obj, const Vec2& to, float duration, EaseType ease, std::function<void()> onComplete = {});

    // 트윈 제거 함수
    void RemoveTween(ITween* tween)
    {
        if (!tween) return;

        auto it = std::find_if(m_items.begin(), m_items.end(),
            [tween](const std::unique_ptr<ITween>& tw_ptr)
            {
                return tw_ptr.get() == tween;
            });

        if (it != m_items.end())
        {
            if (it != m_items.end() - 1)
                *it = std::move(m_items.back());
            m_items.pop_back();
        }
    }

    // 전체 정리(선택)
    void Clear();

private:
    std::vector<std::unique_ptr<ITween>> m_items;
    std::vector<std::unique_ptr<ITween>> m_pending; // 업데이트 중 추가 안전 처리
};

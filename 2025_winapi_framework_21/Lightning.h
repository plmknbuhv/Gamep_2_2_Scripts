#pragma once
#include "Animator.h"
#include "Object.h"
#include "Texture.h"

class Lightning : public Object
{
    public:
    Lightning();
    ~Lightning();
    void Update() override;
    void Render(HDC _hdc) override;
    void LateUpdate() override;
    void SetWaitTime(float _waitTime);
    public:
    private:
    Texture* m_lTexture;
    Texture* m_wTexture;
    Vec2 m_position;
    float m_timer,m_WaitTime;
    bool m_isLightning,waiting;
    Animator* m_animator;
    
};


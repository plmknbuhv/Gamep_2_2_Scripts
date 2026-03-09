#pragma once
#include "Scene.h"
class TitleScene : public Scene
{
public:
    void Init() override;
    void Update() override;
    void Render(HDC _hdc) override;
private:
    // 타이틀 연출용 오프셋 (슬라이드 인)
    float m_titleOffsetY = 0.f;
    Texture* m_texture;
};


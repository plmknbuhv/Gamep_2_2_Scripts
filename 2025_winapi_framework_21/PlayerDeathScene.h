#pragma once
#include "Scene.h"

class PlayerDeathScene : public Scene
{
public:
    void Init() override;
    void Update() override;
    void Render(HDC _hdc) override;
private:
    Texture* m_texture = nullptr;
};

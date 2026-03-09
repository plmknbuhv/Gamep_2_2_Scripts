#pragma once
#include "Object.h"
#include "Texture.h"

class SceneChangeObj : public Object
{
public:
    SceneChangeObj();
    ~SceneChangeObj();
    void Update() override;
    void Render(HDC _hdc) override;
private:
    Texture* m_texture;
    int m_alpa;
    bool m_isDone;
    
};

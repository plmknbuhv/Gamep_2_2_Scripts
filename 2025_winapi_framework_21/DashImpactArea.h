#pragma once
#include "Object.h"
#include "Vec2.h"

class DashImpactArea :
    public Object
{
public:
    DashImpactArea();

public:
    void Update() override;
    void Render(HDC _hdc) override;
    void DrawArea(Vector2 _fillFrom, Vector2 _fillTo, float _fillDuration);
    void SetFixedHeight(float h) { m_fixedHeight = h; }
    void Hide() { m_visible = false; }

private:
    bool m_visible = false;
    float m_fixedHeight = 0.f;

    bool  m_active = false;
    bool  m_growRight = true;
    float m_duration = 0.f;
    float m_elapsed = 0.f;
    float m_targetWidth = 0.f;
    Vector2 m_from = {0.f, 0.f};
    Vector2 m_to = {0.f, 0.f};
};


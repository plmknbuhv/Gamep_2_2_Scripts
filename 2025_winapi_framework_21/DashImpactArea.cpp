#include "pch.h"
#include "DashImpactArea.h"
#include <algorithm>
#include "TweenManager.h"
#include "SceneManager.h"

DashImpactArea::DashImpactArea()
{
    GET_SINGLE(SceneManager)->GetCurScene()->AddObject(this, Layer::BACKGROUND);
}

void DashImpactArea::DrawArea(Vector2 _fillFrom, Vector2 _fillTo, float _fillDuration)
{
    m_from = _fillFrom;
    m_to = _fillTo;
    m_duration = max(0.0001f, _fillDuration);
    m_elapsed = 0.f;

    float dx = std::abs(_fillTo.x - _fillFrom.x);
    m_targetWidth = dx;
    m_growRight = (_fillTo.x >= _fillFrom.x);

    SetSize(Vector2(0.f, m_fixedHeight));
    SetPos(Vector2(_fillFrom.x, _fillFrom.y));

    m_visible = true;
    m_active = true;
}

void DashImpactArea::Update()
{
    Object::Update();

    if (!m_active || !m_visible)
        return;

    m_elapsed += fDT;
    float t = m_elapsed / m_duration;
    if (t < 0.f) t = 0.f; else if (t > 1.f) t = 1.f;

    float inv = 1.f - t;
    float eased = 1.f - inv * inv;

    float curW = m_targetWidth * eased;
    curW = max(curW, 0.f);

    SetSize(Vector2(curW, m_fixedHeight));

    float halfW = curW * 0.5f;
    float cx = m_growRight ? (m_from.x + halfW) : (m_from.x - halfW);
    SetPos(Vector2(cx, m_from.y));

    if (t >= 1.f)
    {
        SetSize(Vector2(m_targetWidth, m_fixedHeight));
        float finalCenterX = (m_from.x + m_to.x) * 0.5f;
        SetPos(Vector2(finalCenterX, m_from.y));
        m_active = false;
    }
}

void DashImpactArea::Render(HDC _hdc)
{
    if (!m_visible)
        return;

    HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(_hdc, hRedPen);

    HBRUSH hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hRedBrush);

    Vector2 pos = GetPos();
    Vector2 size = GetSize();

    float left = pos.x - size.x / 2.f;
    float top = pos.y - size.y / 2.f;
    float right = pos.x + size.x / 2.f;
    float bottom = pos.y + size.y / 2.f;

    Rectangle(_hdc, (int)left, (int)top, (int)right, (int)bottom);

    SelectObject(_hdc, hOldPen);
    DeleteObject(hRedPen);
    SelectObject(_hdc, hOldBrush);
    DeleteObject(hRedBrush);
}
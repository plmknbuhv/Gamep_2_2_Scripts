#include "pch.h"
#include "Button.h"

#include "InputManager.h"
#include "Object.h"
#include "Defines.h"
#include "GDISelector.h"
#include "Enums.h"
#include "ResourceManager.h"

Button::Button() : m_texture(nullptr) , m_Stexture(nullptr)
{
    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"ButtonBottom");
    m_Stexture = GET_SINGLE(ResourceManager)->GetTexture(L"ButtonBottom_Seclect");
}

void Button::Init()
{
    m_isHover = false;
    m_isPressed = false;
    m_pressStartedInside = false;
}

bool Button::HitTest(const POINT& pt) const
{
    const Object* owner = GetOwner();
    if (owner == nullptr)
        return false;

    const Vec2& pos = owner->GetPos();
    const Vec2& size = owner->GetSize();
    const float left = pos.x - size.x * 0.5f;
    const float top = pos.y - size.y * 0.5f;
    const float right = pos.x + size.x * 0.5f;
    const float bottom = pos.y + size.y * 0.5f;

    return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;
}

void Button::LateUpdate()
{
    const POINT& mouse = GET_MOUSEPOS;
    const bool over = HitTest(mouse);
    m_isHover = over;

    // 마우스 버튼 처리
    if (GET_KEYDOWN(KEY_TYPE::LBUTTON))
    {
        m_isPressed = over;
        m_pressStartedInside = over;
    }

    // 누르고 있는 동안 상태 유지
    if (GET_KEY(KEY_TYPE::LBUTTON))
    {
        // 눌린 상태는 유지, hover는 마우스 위치에 따라 갱신
    }

    if (GET_KEYUP(KEY_TYPE::LBUTTON))
    {
        bool clicked = (m_pressStartedInside && over);
        m_isPressed = false;
        m_pressStartedInside = false;

        if (clicked)
        {
            // 구독자 호출
            for (auto& cb : m_onClicks)
            {
                if (cb) cb();
            }
        }
    }
}

void Button::Render(HDC _hdc)
{
    const Object* owner = GetOwner();
    if (owner == nullptr)
        return;

    const Vec2& pos = owner->GetPos();
    const Vec2& size = owner->GetSize();


    Texture* tex = m_texture;
    if (m_isPressed)
        tex =m_Stexture;
    else if (m_isHover)
        tex =m_Stexture;

    ::TransparentBlt(_hdc
        , (int)(pos.x - size.x / 2)
        , (int)(pos.y)
        , size.x, size.y,
        tex->GetTextureDC(),
        0, 0, tex->GetWidth(), tex->GetHeight(), RGB(255,0,255));

    // 텍스트 출력 (중앙 정렬)
    GDISelector font(_hdc, FontType::UI);
    ::SetTextColor(_hdc, m_textColor);

    RECT rc{};
    rc.left = (LONG)(pos.x - size.x * 0.5f);
    rc.top = (LONG)(pos.y - size.y * 0.5f - 10.f);
    rc.right = (LONG)(pos.x + size.x * 0.5f);
    rc.bottom = (LONG)(pos.y + size.y * 0.5f - 10.f);
    ::DrawTextW(_hdc, m_text.c_str(), (int)m_text.length(), &rc,
        DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

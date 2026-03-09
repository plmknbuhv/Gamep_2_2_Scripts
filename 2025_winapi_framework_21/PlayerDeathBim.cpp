#include "pch.h"
#include "PlayerDeathBim.h"

#include "ResourceManager.h"
#include "Texture.h"

PlayerDeathBim::PlayerDeathBim()
{
    m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDeadBim");
    
}

PlayerDeathBim::~PlayerDeathBim()
{
}

void PlayerDeathBim::Render(HDC _hdc)
{
    Object::Render(_hdc);
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    int width = m_pTexture->GetWidth();
    int height = m_pTexture->GetHeight();

    StretchBlt(_hdc
    , (int)(pos.x - size.x / 2)
    , (int)(pos.y - size.y / 2)
    , size.x
    , size.y
    , m_pTexture->GetTextureDC()
    , 0, 0, width,height,SRCCOPY);
    ComponentRender(_hdc);
}

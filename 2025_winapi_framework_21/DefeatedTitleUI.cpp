#include "pch.h"
#include "DefeatedTitleUI.h"

#include "ResourceManager.h"
#include "Texture.h"

DefeatedTitleUI::DefeatedTitleUI()
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"DefeatedText");
}

DefeatedTitleUI::~DefeatedTitleUI()
{
}

void DefeatedTitleUI::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    int width = m_pTexture->GetWidth();
    int height = m_pTexture->GetHeight();
    /*ELLIPSE_RENDER(_hdc, pos.x, pos.y
        , size.x, size.y);*/
    ::TransparentBlt(_hdc
    , (int)(pos.x - size.x / 2)
    , (int)(pos.y - size.y / 2)
    , size.x, size.y,
    m_pTexture->GetTextureDC(),
    0, 0, width, height, RGB(255,0,255));
    ComponentRender(_hdc);
}

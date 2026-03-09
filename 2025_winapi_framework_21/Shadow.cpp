#include "pch.h"
#include "Shadow.h"
#include "ResourceManager.h"
#include "Texture.h"

Shadow::Shadow()
	: m_offset()
	, m_size()
	, m_texture(nullptr)
    , m_owner(nullptr)
{
}

Shadow::~Shadow()
{
}

void Shadow::SetUpShadow(Vec2 _offset, Vec2 _size, Object* owner)
{
    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"Shadow");
    m_offset = _offset;
    m_size = _size;
    m_owner = owner;
}

void Shadow::LateUpdate()
{
}

void Shadow::Render(HDC _hdc)
{
    const int srcWidth = m_texture->GetWidth();
    const int srcHeight = m_texture->GetHeight();
    const int destWidth = (int)m_size.x;
    const int destHeight = (int)m_size.y;
    Vec2 pos = m_owner->GetPos();
    pos += m_offset;
    // 1. 임시 DC에 원본 텍스처를 그리면서 분홍색(RGB(255, 0, 255)) 배경을 투명하게 처리합니다.
    TransparentBlt(_hdc,
        (int)(pos.x - destWidth / 2.f),
        (int)(pos.y - destHeight / 2.f),
        destWidth, destHeight,
        m_texture->GetTextureDC(),
        0, 0, srcWidth, srcHeight, RGB(255, 0, 255));
}


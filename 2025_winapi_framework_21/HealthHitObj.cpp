#include "pch.h"
#include "HealthHitObj.h"

#include "ResourceManager.h"
#include "TweenManager.h"

HealthHitObj::HealthHitObj()
{
    m_isDestroy = false;
    m_changeImage = false;
    m_DestroyTex = GET_SINGLE(ResourceManager)->GetTexture(L"BossHPDesstroyUI");
    m_HitTex = GET_SINGLE(ResourceManager)->GetTexture(L"BossHpHitUI");
    m_texSize = m_HitTex->GetWidth();
    m_spawnedTime = 0;
}

HealthHitObj::~HealthHitObj()
{
}

void HealthHitObj::Update()
{
    m_spawnedTime += fDT;
    if (!m_changeImage)
    {
        if (m_spawnedTime >= 0.15)
        {
            m_changeImage = true;
        }
    }

}

void HealthHitObj::Render(HDC _hdc)
{
    if (m_changeImage)
    {
        StretchBlt(_hdc
        , GetPos().x
        , GetPos().y
        , GetSize().x
        ,GetSize().y
        , m_DestroyTex->GetTextureDC()
        , 0, 0, m_texSize,m_texSize,SRCCOPY); // 짜피 사이즈 같음
    }else
    {
        StretchBlt(_hdc
        , GetPos().x
        , GetPos().y
        , GetSize().x
        ,GetSize().y
        , m_HitTex->GetTextureDC()
        , 0, 0, m_texSize,m_texSize,SRCCOPY); // 짜피 사이즈 같음 
    }

}

void HealthHitObj::SetPosScale(Vector2 _pos, Vector2 _size)
{
    SetPos(_pos);
    SetSize(_size);
    
    GET_SINGLE(TweenManager)->MoveTo(this, Vec2(_pos.x, _pos.y - 15), 0.3f, EaseType::Linear,[this]()
    {
        SetDestroy();
    });
}

bool HealthHitObj::GetDestroy()
{
    return m_isDestroy;
}

void HealthHitObj::SetDestroy()
{
    m_isDestroy = true;
}

#include "pch.h"
#include "SceneChangeObj.h"

#include "ResourceManager.h"
#include "SceneManager.h"

SceneChangeObj::SceneChangeObj()
{
    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDeadBackground");
    m_alpa = 0;
    m_isDone = false;
}

SceneChangeObj::~SceneChangeObj()
{
}

void SceneChangeObj::Update()
{
    if (m_alpa < 255)
    {
        m_alpa += 5;
        if (m_alpa >= 255)
        {
            m_isDone = true;
            m_alpa = 255;
            GET_SINGLE(SceneManager)->GetCurScene()->SetNextScene(L"PlayerDeathScene");
        }
    }
    Object::Update();
}

void SceneChangeObj::Render(HDC _hdc)
{
    BLENDFUNCTION bf = {0};
    bf.BlendOp = AC_SRC_OVER;     
    bf.BlendFlags = 0;            
    bf.SourceConstantAlpha = m_alpa;  // 0~ 255 Åõ¸íµµ
    bf.AlphaFormat = 0;
    AlphaBlend(_hdc
    ,0
    ,0
    ,WINDOW_WIDTH
    ,WINDOW_HEIGHT
    ,m_texture->GetTextureDC()
    ,0
    ,0
    ,m_texture->GetWidth()
    ,m_texture->GetHeight()
    ,bf);
}

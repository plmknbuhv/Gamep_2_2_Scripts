#include "pch.h"
#include "EndingScene.h"

#include "Button.h"
#include "ResourceManager.h"

void EndingScene::Init()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    GET_SINGLE(ResourceManager)->Play(L"ClearBGM");
    GET_SINGLE(ResourceManager)->Play(L"ClearSound");

    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDeadBackground");
    
    Object* exitObj = new Object();
    exitObj->SetSize({220,60});
    // 오른쪽 바깥에서 슬라이드 인
    exitObj->SetPos({WINDOW_WIDTH - 110.f, WINDOW_HEIGHT * 0.7f });
    auto* Exitbtn = exitObj->AddComponent<Button>();
    Exitbtn->SetOwner(exitObj);
    Exitbtn->SetText(L"Exit");
    Exitbtn->SetTextColor(RGB(255, 255, 255));
    Exitbtn->AddOnClick([]() {
        ::PostQuitMessage(0);
    });

    AddObject(exitObj, Layer::UI);
}

void EndingScene::Update()
{
    Scene::Update();
}

void EndingScene::Render(HDC _hdc)
{
    ::StretchBlt(_hdc
, 0
, 0
, WINDOW_WIDTH, WINDOW_HEIGHT,
m_texture->GetTextureDC(),
0, 0, m_texture->GetWidth(), m_texture->GetHeight(), SRCCOPY); // Background
    GDISelector titleFont(_hdc, FontType::TITLE);
    ::SetTextColor(_hdc, RGB(255,255,255));

    RECT titleRc{};
    titleRc.left = 0;
    titleRc.top = (LONG)(WINDOW_HEIGHT * 0.22f - 40 + -120.f);
    titleRc.right = WINDOW_WIDTH;
    titleRc.bottom = titleRc.top + 80;
    ::DrawTextW(_hdc, L"클리어", -1, &titleRc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    Scene::Render(_hdc);
    for (UINT i = 0; i < (UINT)Layer::END; ++i)
    {
        const auto& vec = GetLayerObjects((Layer)i);
        for (auto* obj : vec)
        {
            if (obj && !obj->GetIsDead())
                obj->ComponentRender(_hdc);
        }
    }
    
}

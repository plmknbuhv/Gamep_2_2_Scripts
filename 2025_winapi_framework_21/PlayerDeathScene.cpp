#include "pch.h"
#include "PlayerDeathScene.h"

#include "Button.h"
#include "DefeatedTitleUI.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TweenManager.h"

void PlayerDeathScene::Init()
{
    GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
    GET_SINGLE(ResourceManager)->Play(L"LoseSound");
    GET_SINGLE(ResourceManager)->Play(L"LoseBGM");

    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"PlayerDeadBackground");
    DefeatedTitleUI* title = Spawn<DefeatedTitleUI>(Layer::UI,{-738,50},{492,99});
    GET_SINGLE(TweenManager)->MoveTo(title, Vec2(215 ,50), 3.0f, EaseType::EaseOutExpo);

    //PlayerDeathBim* bim = Spawn<PlayerDeathBim>(Layer::UI,{WINDOW_WIDTH/2,WINDOW_HEIGHT/2},{500,500});
    Object* exitObj = new Object();
    exitObj->SetSize({220,60});
    // 오른쪽 바깥에서 슬라이드 인
    exitObj->SetPos({WINDOW_WIDTH - 110.f, WINDOW_HEIGHT * 0.7f });
    auto* Exitbtn = exitObj->AddComponent<Button>();
    Exitbtn->SetOwner(exitObj);
    Exitbtn->SetText(L"Exit");
    Exitbtn->SetTextColor(RGB(255, 255, 255));
    Exitbtn->AddOnClick([]() {
        GET_SINGLE(SceneManager)->GetCurScene()->SetExitGame();
    });

    AddObject(exitObj, Layer::UI);
}

void PlayerDeathScene::Update()
{
    Scene::Update();
}

void PlayerDeathScene::Render(HDC _hdc)
{
    ::StretchBlt(_hdc
    , 0
    , 0
    , WINDOW_WIDTH, WINDOW_HEIGHT,
    m_texture->GetTextureDC(),
    0, 0, m_texture->GetWidth(), m_texture->GetHeight(), SRCCOPY); // Background
    for (UINT i = 0; i < (UINT)Layer::END; ++i)
    {
        const auto& vec = GetLayerObjects((Layer)i);
        for (auto* obj : vec)
        {
            if (obj && !obj->GetIsDead())
                obj->ComponentRender(_hdc);
        }
    }
    Scene::Render(_hdc);
    
}

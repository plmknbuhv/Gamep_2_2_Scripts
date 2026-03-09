#include "pch.h"
#include "TitleScene.h"

// 추가 헤더
#include "Object.h"
#include "Button.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TweenManager.h"

void TitleScene::Init()
{
    GET_SINGLE(ResourceManager)->Play(L"TitleBGM");

    m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"BackGround");
    const Vec2 btnSize = { 220.f, 60.f };
    const float centerY = WINDOW_HEIGHT * 0.5f;
    const float gap = 80.f;

    // 타이틀 슬라이드 인을 위한 초기 오프셋 (위에서 내려오도록 음수)
    m_titleOffsetY = -120.f;

    Object* startObj = new Object();
    startObj->SetSize(btnSize);
    // 왼쪽 바깥에서 슬라이드 인
    startObj->SetPos({ WINDOW_WIDTH + 0.f, centerY });

    auto* btn = startObj->AddComponent<Button>();
    btn->SetText(L"Start");
    btn->SetTextColor(RGB(255, 255, 255));
    btn->AddOnClick([]() {
        GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
        
        GET_SINGLE(SceneManager)->GetCurScene()->SetNextScene(L"GameScene");
    });

    AddObject(startObj, Layer::DEFAULT);

    Object* exitObj = new Object();
    exitObj->SetSize(btnSize);
    // 오른쪽 바깥에서 슬라이드 인
    exitObj->SetPos({WINDOW_WIDTH + 0.f, centerY + gap });

    auto* Exitbtn = exitObj->AddComponent<Button>();
    Exitbtn->SetText(L"Exit");
    Exitbtn->SetTextColor(RGB(255, 255, 255));
    Exitbtn->AddOnClick([]() {
        ::PostQuitMessage(0);
    });

    AddObject(exitObj, Layer::DEFAULT);

    // 버튼 위치 트윈 (EaseOut으로 부드럽게 진입)
    GET_SINGLE(TweenManager)->MoveTo(startObj, Vec2(WINDOW_WIDTH - btnSize.x/2, centerY), 0.6f, EaseType::EaseOutQuad);
    GET_SINGLE(TweenManager)->MoveTo(exitObj, Vec2(WINDOW_WIDTH - btnSize.x / 2, centerY + gap), 0.8f, EaseType::EaseOutQuad);

    // 타이틀 오프셋 트윈: 위에서 제자리로
    auto getter = [this]() { return m_titleOffsetY; };
    auto setter = [this](const float& v) { m_titleOffsetY = v; };
    GET_SINGLE(TweenManager)->Start<float>(getter, setter, 0.f, 0.9f, EaseType::EaseOutQuad, [this]() {
        // 미세한 호흡감 (위아래로 천천히 반복)
        auto upGetter = [this]() { return m_titleOffsetY; };
        auto upSetter = [this](const float& v) { m_titleOffsetY = v; };
        // 위로 살짝
        GET_SINGLE(TweenManager)->Start<float>(upGetter, upSetter, -6.f, 1.2f, EaseType::EaseInOutQuad, [this]() {
            auto downGetter = [this]() { return m_titleOffsetY; };
            auto downSetter = [this](const float& v) { m_titleOffsetY = v; };
            // 아래로 다시
            GET_SINGLE(TweenManager)->Start<float>(downGetter, downSetter, 0.f, 1.2f, EaseType::EaseInOutQuad, [this]() {
                // 다시 위로 올라가도록 재귀적으로 시작 (간단한 루프)
                auto upGetter2 = [this]() { return m_titleOffsetY; };
                auto upSetter2 = [this](const float& v) { m_titleOffsetY = v; };
                GET_SINGLE(TweenManager)->Start<float>(upGetter2, upSetter2, -6.f, 1.2f, EaseType::EaseInOutQuad, [this]() {
                    auto downGetter2 = [this]() { return m_titleOffsetY; };
                    auto downSetter2 = [this](const float& v) { m_titleOffsetY = v; };
                    GET_SINGLE(TweenManager)->Start<float>(downGetter2, downSetter2, 0.f, 1.2f, EaseType::EaseInOutQuad);
                });
            });
        });
    });
}

void TitleScene::Update()
{
    Scene::Update();
}

void TitleScene::Render(HDC _hdc)
{
    int width = m_texture->GetWidth();
    int height = m_texture->GetHeight();

    ::StretchBlt(_hdc, 0, 0	, WINDOW_WIDTH, WINDOW_HEIGHT,
        m_texture->GetTextureDC(), 0, 0, width, height, SRCCOPY);
    GDISelector titleFont(_hdc, FontType::TITLE);
    ::SetTextColor(_hdc, RGB(10, 10, 10));

    RECT titleRc{};
    titleRc.left = 0;
    titleRc.top = (LONG)(WINDOW_HEIGHT * 0.22f - 40 + m_titleOffsetY);
    titleRc.right = WINDOW_WIDTH;
    titleRc.bottom = titleRc.top + 80;
    ::DrawTextW(_hdc, L"Core Strike", -1, &titleRc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

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

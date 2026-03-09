#include "pch.h"
#include "Lightning.h"
#include "Animation.h"
#include "Animator.h"
#include "BulletShooter.h"
#include "Overlap.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "TweenManager.h"
#include "Player.h"
#include "Projectile.h"
#include "ImpulseManager.h"


Lightning::Lightning() : m_position({0,0}),m_lTexture(nullptr),m_wTexture(nullptr), m_WaitTime(0)
{
    m_isLightning = true;
    waiting = true;
    m_timer = 0.f;
    SetSize({0,0});
    m_lTexture = GET_SINGLE(ResourceManager)->GetTexture(L"Lightning");
    m_wTexture = GET_SINGLE(ResourceManager)->GetTexture(L"Warring");
    m_animator = AddComponent<Animator>();
    m_animator->CreateAnimation(L"lightning_Fly",m_lTexture,
        {0,0},
        {128,256},
        {128,0},
        5,
        0.1f);
    
}

Lightning::~Lightning()
{
    
}

void Lightning::Update()
{
    if (waiting)
    {
        m_timer += fDT;
        if (m_timer > m_WaitTime)
        {
            m_timer = 0.f;
            m_isLightning = false;
            waiting = false;
            GET_SINGLE(TweenManager)->ScaleTo(this,{130,130},1.5f,EaseType::EaseOutExpo);
            GET_SINGLE(ResourceManager)->Play(L"LightningWarring");
            
        }
    }
    if (!m_isLightning)
    {
        m_timer += fDT;
        if (m_timer >= 1.3f)
        {
            m_isLightning = true;
            GET_SINGLE(ImpulseManager)->ApplyImpulse(4.f, 0.5f);
            vector<Collider*> colliders;
            if (Overlap::OverlapBox(GetPos(), { 140,140 }, Layer::PLAYER, colliders))
            {
                SetPos({ GetPos().x,GetPos().y - 150 });
                for (Collider* collider : colliders)
                {
                    Object* obj = collider->GetOwner();
                    Player* plr = static_cast<Player*>(obj);
                    cout << "Case" << endl;
                    plr->ApplyDamage(10, nullptr);
                }
            }

            float angle = 0.f;
            for (int i = 0; i < 18; ++i)
            {
                Projectile* proj = new Projectile(10, Layer::PLAYER, this);
                Vec2 pos = GetPos();
                proj->SetPos(pos);
                proj->SetSpeed(12.5f);
                proj->SetSize({ 20.f,20.f });
                proj->SetAngle(angle * PI / 180);
                proj->SetDir({ cos(angle * PI / 180),sin(angle * PI / 180) });
                GET_SINGLE(SceneManager)->GetCurScene()
                    ->AddObject(proj, Layer::DEFAULT);
                angle += 20.f;
            }

            m_animator->Play(L"lightning_Fly",PlayMode::Once,0,1.f,[this]()
            {
                SetDead();
                GET_SINGLE(SceneManager)->RequestDestroy(this);
            });
            GET_SINGLE(ResourceManager)->Play(L"Lightning");
            m_animator->GetCurrent()->SetScale(1.5f);
        }
    }
    Object::Update();
}

void Lightning::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    ComponentRender(_hdc);
    if (((int)size.x % 2) == 0) return;
    int width = m_wTexture->GetWidth();
    int height = m_wTexture->GetHeight();
    if (m_isLightning )return;
    ::TransparentBlt(_hdc
    , (int)(pos.x - size.x / 2)
    , (int)(pos.y - size.y / 2)
    , size.x, size.y,
    m_wTexture->GetTextureDC(),
    0, 0, width, height, RGB(255,0,255));
}

void Lightning::LateUpdate()
{
    Object::LateUpdate();
}

void Lightning::SetWaitTime(float _waitTime)
{
    m_WaitTime = _waitTime;
}

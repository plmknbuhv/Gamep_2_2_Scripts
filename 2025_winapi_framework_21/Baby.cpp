#include "pch.h"
#include "Baby.h"
#include "Animation.h"
#include "BulletShooter.h"
#include "Overlap.h"
#include "Player.h"
#include "Projectile.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Texture.h"
#include "TweenManager.h"
#include "Health.h"
#include "ImpulseManager.h"

class Projectile;

Baby::Baby(Entity* summoner, bool shoot, bool isCanHit) 
    : m_position({0,0}),m_bTexture(nullptr), m_WaitTime(0),
    m_shootProjectile(shoot), m_summoner(summoner)
{
    m_isSpawned = true;
    waiting = true;
    m_timer = 0.f;
    m_bTexture = GET_SINGLE(ResourceManager)->GetTexture(L"BabyWalk");
    m_animator = AddComponent<Animator>();

    SetSize({0,0});

    Collider* col = AddComponent<Collider>();
    col->SetName(L"Enemy");

    m_shooter = AddComponent<BulletShooter>();

    m_animator->CreateAnimation(L"BabyWalk",m_bTexture,
        {0,0},
        {123,134},
        {123,0},
        12,
       0.1f);

    // m_animator->CreateAnimation(L"BabyDie", m_bTexture,)
}

void Baby::Update()
{
    m_timer += fDT;
    m_position = GetPos();

    if (waiting)
    {
        if (m_timer > m_WaitTime)
        {
            m_timer = 0.f;
            m_isSpawned = false;
            waiting = false;
            
            m_animator->Play(L"BabyWalk",PlayMode::Loop,1,5.f);
            m_animator->GetCurrent()->SetFlipX(true);
            m_animator->GetCurrent()->SetScale(2.f);
        }
    }else
    {
        m_position.x -= 500 * fDT;
        SetPos(m_position);
        if (m_position.x <= -20)
        {
            // SetDead();
            // GET_SINGLE(SceneManager)->RequestDestroy(this);
            return;
        }
    }
    if (!m_isSpawned)
    {
        m_isSpawned = true;
        //GET_SINGLE(ResourceManager)->Play(L"Lightning");
    }
    else if (!waiting)
    {
        vector<Collider*> colliders;
        if (Overlap::OverlapBox(m_position, { 50,50 }, Layer::PLAYER, colliders))
        {
            for (Collider* collider : colliders)
            {
                Object* obj = collider->GetOwner();
                Player* plr = dynamic_cast<Player*>(obj);
                if (plr == nullptr)
                    continue;
                plr->ApplyDamage(10, m_summoner);
            }
        }
        if (m_timer > 0.8f && m_shootProjectile)
        {
            m_timer = 0.f;
            float angle = 0.f;
            for (int i = 0; i < 18; ++i)
            {
                Projectile* proj = new Projectile(10, Layer::PLAYER, this);
                proj->SetPos(m_position);
                proj->SetSpeed(12.5f);
                proj->SetSize({ 20.f,20.f });
                proj->SetAngle(angle * PI / 180);
                proj->SetDir({ cos(angle * PI / 180),sin(angle * PI / 180) });
                GET_SINGLE(SceneManager)->GetCurScene()
                    ->AddObject(proj, Layer::ENEMY);
                angle += 20.f;
            }
        }
    }
    Entity::Update();
}

void Baby::Render(HDC _hdc)
{
    Vec2 pos = GetPos();
    Vec2 size = GetSize();
    Entity::Render(_hdc);
    if (GetComponent<Collider>() != nullptr)
        GetComponent<Collider>()->Render(_hdc);
    if (((int)size.x % 2) == 0) return;
    int width = m_bTexture->GetWidth();
    int height = m_bTexture->GetHeight();
    if (m_isSpawned )return;
    ::TransparentBlt(_hdc
    , (int)(pos.x - size.x / 2)
    , (int)(pos.y - size.y / 2)
    , size.x, size.y,
    m_bTexture->GetTextureDC(),
    0, 0, width, height, RGB(255,0,255));
}

void Baby::ApplyDamage(int _damage, Entity* _dealer)
{
    Entity::ApplyDamage(_damage, _dealer);
    if (m_health->GetCurrentHealth() <= 0)
    {
        // m_animator->Play(L"BabyDie", PlayMode::Once, 1, 1.f, [this]()
            // {
                GET_SINGLE(SceneManager)->RequestDestroy(this);
            // });
    }
}

void Baby::SetWaitTime(float _waitTime)
{
}

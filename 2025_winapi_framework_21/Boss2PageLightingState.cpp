#include "pch.h"
#include "Boss2PageLightingState.h"

#include "Animation.h"
#include "BulletShooter.h"
#include "Lightning.h"
#include "SceneManager.h"
#include "ImpulseManager.h"

Boss2PageLightingState::Boss2PageLightingState(Boss* bs) : State(bs)
{
    m_owner = bs;
    //m_collider = m_owner->GetComponent<Collider>();
    m_animator = m_owner->GetComponent<Animator>();
}

void Boss2PageLightingState::EnterState()
{
    m_isEnd = false;
    m_skillTimer = 0;
    
    m_animator->Play(L"boss_Smash",PlayMode::Once,1,1.f,[this]()
    {
        m_owner->GetComponent<BulletShooter>()->ShootAround(150.f, 10, Layer::PLAYER);
        m_animator->Play(L"boss_Idle",PlayMode::Loop,2,1);
        m_animator->GetCurrent()->SetFlipX(true);
        m_animator->GetCurrent()->SetScale(6);
        m_isEnd = true;
        
    }, 12, 1, [this]() { GET_SINGLE(ImpulseManager)->ApplyImpulse(3.f, 1.f); });
    m_animator->GetCurrent()->SetFlipX(true);
    m_animator->GetCurrent()->SetScale(6);

    for (int i = 0; i < 4; ++i)
    {
        Lightning* light = new Lightning;
        light->SetWaitTime(i);
        int idx = std::rand() % 200;
        if (idx % 2 == 0) idx *= -1;
        Vector2 pos = m_owner->GetPos();
        pos.x += idx;
        idx = std::rand() % 200;
        if (idx % 2 == 0) idx *= -1;
        pos.y += idx;
        light->SetPos(pos);
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(light,Layer::ENEMY_PROJECTILE);
    }
    
    
}

void Boss2PageLightingState::LateUpdate()
{
    if (m_isEnd)
    {
        m_skillTimer += fDT;
        if (m_skillTimer >= 1.f)
        {
            m_owner->ChangeState("Idle");
        }
    }
}

void Boss2PageLightingState::ExitState()
{
    State::ExitState();
}

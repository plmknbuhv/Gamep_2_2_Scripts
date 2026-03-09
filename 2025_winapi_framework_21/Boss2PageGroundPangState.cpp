#include "pch.h"
#include "Boss2PageGroundPangState.h"
#include "Animation.h"
#include "BulletShooter.h"
#include "Overlap.h"
#include "ResourceManager.h"
#include "ImpulseManager.h"

Boss2PageGroundPangState::Boss2PageGroundPangState(Boss* bs)
    : State(bs)
{
    m_owner = bs;
    //m_collider = m_owner->GetComponent<Collider>();
    m_animator = m_owner->GetComponent<Animator>();
}

void Boss2PageGroundPangState::EnterState()
{
    m_isEnd = false;
    m_skillTimer = 0;
    m_animator->Play(L"boss_Smash",PlayMode::Once,1,2,[this]()
    {
        m_owner->GetComponent<BulletShooter>()->ShootAround(150.f, 10, Layer::PLAYER);
        m_animator->Play(L"boss_Idle",PlayMode::Loop,1,1);
        m_animator->GetCurrent()->SetFlipX(true);
        m_animator->GetCurrent()->SetScale(6);
        
        m_isEnd = true;
        m_skillTimer = 0;
    }, 12, 1, [this]() { GET_SINGLE(ImpulseManager)->ApplyImpulse(3.f, 1.f); });
    m_animator->GetCurrent()->SetFlipX(true);
    m_animator->GetCurrent()->SetScale(6);
}

void Boss2PageGroundPangState::LateUpdate()
{
    if (m_isEnd)
    {
        m_skillTimer += fDT;
        if (m_skillTimer >= 1.5f)
        {
            m_owner->ChangeState("Idle");
        }
    }else
    {
        m_skillTimer += fDT;
        if (m_skillTimer >= 0.4f)
        {
            cout << "Pang" << endl;
            m_skillTimer = -999999;
            GET_SINGLE(ResourceManager)->Play(L"BossSpawn");
            vector<Collider*> colliders;
            if (Overlap::OverlapBox(m_owner->GetPos(), { 170,170 }, Layer::PLAYER, colliders))
            {
                for (Collider* collider : colliders)
                {
                    Object* obj = collider->GetOwner();
                    Player* plr = static_cast<Player*>(obj);
                    cout << "Case" << endl;
                    plr->ApplyDamage(10,m_owner);
                }
            }
        }
    }
}

void Boss2PageGroundPangState::ExitState()
{
    State::ExitState();
    
}

#include "pch.h"
#include "BossSpawnState.h"
#include "Animation.h"
#include "Boss.h"
#include "Animator.h"
#include "ResourceManager.h"
#include "StateMachine.h"
#include "ImpulseManager.h"

BossSpawnState::BossSpawnState(Boss* bs)
    : State(bs), m_delayTimer(0.f), m_spawnStart(false), m_spawnTimer(0.f)
{
    m_owner = bs;
    m_animator = bs->GetComponent<Animator>();
}

void BossSpawnState::EnterState()
{
    m_owner->GetComponent<Collider>()->SetEnable(false);
    m_delayTimer = m_spawnTimer = 0;
    m_spawnStart = false;
    State::EnterState();
}

void BossSpawnState::LateUpdate()
{
    State::LateUpdate();
    
    if (m_delayTimer >= 1.5f)
    {
        if (!m_spawnStart)
        {
            m_spawnStart = true;
            GET_SINGLE(ResourceManager)->Play(L"BossSpawn");
            GET_SINGLE(ImpulseManager)->ApplyImpulse(3.f, 1.f);
            m_animator->Play(L"boss_Smash_End",PlayMode::Once,0,1.5f,[this]()
            {
                m_owner->ChangeState("Idle");
            });
            m_animator->GetCurrent()->SetScale(6);
            m_animator->GetCurrent()->SetFlipX(true);
        }else
        {
            m_spawnTimer += fDT;
            if (m_spawnTimer >= 0.5f)
            {
                //m_owner->GetComponent<StateMachine>()->ChangeState("Idle");
            }
        }
    }else
    {
        m_delayTimer+= fDT;
    }
}

void BossSpawnState::ExitState()
{
    State::ExitState();
    m_owner->GetComponent<Collider>()->SetEnable(true);
}


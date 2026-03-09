#include "pch.h"
#include "Boss2PageSpawnBabyState.h"
#include "Animation.h"
#include "Baby.h"
#include "SceneManager.h"

Boss2PageSpawnBabyState::Boss2PageSpawnBabyState(Boss* bs): State(bs)
{
    m_owner = bs;
    m_animator = m_owner->GetComponent<Animator>();
}

void Boss2PageSpawnBabyState::EnterState()
{
    m_isEnd = false;
    m_skillTimer = 0;
    
    m_animator->Play(L"boss_Buff",PlayMode::Once,1,1.f,[this]()
    {
        m_animator->Play(L"boss_Idle",PlayMode::Loop,2,1);
        m_animator->GetCurrent()->SetFlipX(true);
        m_animator->GetCurrent()->SetScale(6);
        m_isEnd = true;
        
    });
    m_animator->GetCurrent()->SetFlipX(true);
    m_animator->GetCurrent()->SetScale(6);
    
    int centerY = WINDOW_HEIGHT / 2;
    for (int i = 0; i < 2; ++i)
    {
        Baby* baby = new Baby(GetOwner<Boss>());
        int randomY;

        if (i == 0)
            randomY = std::rand() % centerY;
        else
            randomY = centerY + (std::rand() % centerY);

        baby->SetPos({ (float)WINDOW_WIDTH + 20.f, (float)randomY });
        GET_SINGLE(SceneManager)->GetCurScene()->AddObject(baby, Layer::ENEMY_PROJECTILE);
    }
}

void Boss2PageSpawnBabyState::LateUpdate()
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

void Boss2PageSpawnBabyState::ExitState()
{
    State::ExitState();
}

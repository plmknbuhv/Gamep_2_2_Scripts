#include "pch.h"
#include "Boss2PageIdleState.h"
#include "Boss.h"
#include "Animator.h"
#include "Animation.h"

Boss2PageIdleState::Boss2PageIdleState(Boss* bs) : State(bs)
    , m_player(nullptr)
{
    m_owner = bs;
    m_animator = bs->GetComponent<Animator>();
}

void Boss2PageIdleState::EnterState()
{
    m_animator->Play(L"boss_Idle_page2",PlayMode::Loop,1,2);
    m_animator->GetCurrent()->SetFlipX(true);

    State::EnterState();
}

void Boss2PageIdleState::LateUpdate()
{
    State::LateUpdate();
}

void Boss2PageIdleState::ExitState()
{
    State::ExitState();
}

#include "pch.h"
#include "BossIdleState.h"
#include "Animation.h"
#include "Animator.h"
#include "Boss.h"

BossIdleState::BossIdleState(Boss* bs)
	: State(bs), m_skillTimer(0.f)
{
	m_owner = bs;
}

void BossIdleState::EnterState()
{
	Animator* anim = m_owner->GetComponent<Animator>();
	anim->Play(L"boss_Idle");
	anim->GetCurrent()->SetScale(6);
	anim->GetCurrent()->SetFlipX(true);
	cout << "Boss Idle State Entered" << endl;
	//GetOwner<Boss>()->GetComponent<Animator>()->Play(L"boss_Idle");
}

void BossIdleState::LateUpdate()
{
	if (!m_owner->m_initialized)
	{
		cout << "Boss not initialized yet." << endl;
		return;
	}
	if (m_owner->m_skillCooldown <= 0)
	{
		cout << "Boss skill cooldown is zero or negative." << endl;
		return;
	}

	m_skillTimer += fDT;
	if (m_skillTimer >= m_owner->m_skillCooldown)
	{
		m_skillTimer = 0.f;
		m_owner->ChangeState("BaseSkill");
	}
}

void BossIdleState::ExitState()
{
	cout << "Boss Idle State Exited" << endl;
}
#include "pch.h"
#include "BossChangePhaseState.h"
#include "StateMachine.h"
#include "Animation.h"
#include "ImpulseManager.h"

BossChangePhaseState::BossChangePhaseState(Boss* bs)
	: State(bs)
{
	m_animator = GetOwner<Boss>()->GetComponent<Animator>();
}

void BossChangePhaseState::EnterState()
{
	m_animator->Play(L"boss_ChangePhase_Hit", PlayMode::Once, 1, 1.f, [this]()
		{
			m_animator->Play(L"boss_ChangePhase_1", PlayMode::Once, 1, 1.f, [this]()
				{
					m_animator->Play(L"boss_ChangePhase_2", PlayMode::Once, 1, 1.f, [this]()
						{
							GetOwner<Boss>()->CreateAnimator(BossPhase::Phase2);
							GetOwner<Boss>()->GetComponent<StateMachine>()->ChangeState("Idle");
						});
					m_animator->GetCurrent()->SetFlipX(true);
					m_animator->GetCurrent()->SetScale(6);
					GET_SINGLE(ImpulseManager)->ApplyImpulse(6.f, 0.5f);
				});
			m_animator->GetCurrent()->SetFlipX(true);
			m_animator->GetCurrent()->SetScale(6);
		});
	m_animator->GetCurrent()->SetFlipX(true);
	m_animator->GetCurrent()->SetScale(6);
}

void BossChangePhaseState::ExitState()
{
	GetOwner<Boss>()->SetApplyDamage(true);
}

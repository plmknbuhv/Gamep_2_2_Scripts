#include "pch.h"
#include "Boss1PageSmashSkillState.h"
#include "Boss.h"
#include "Animator.h"
#include "Animation.h"
#include "Overlap.h"
#include "Collider.h"
#include "Player.h"
#include "SceneManager.h"
#include "TweenManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "ImpulseManager.h"

Boss1PageSmashSkillState::Boss1PageSmashSkillState(Boss* _bs)
	: State(_bs), m_damage(20.f), m_initialPos(0.f, 0.f), m_smashTime(7.5f), m_currentHitCount(0), 
	m_cancelSkillHitCount(10), m_currentTime(0.f), m_smashSize({ 200.f, 200.f}), m_updateTask(true)
{
	m_animator = GetOwner<Boss>()->GetComponent<Animator>();
	m_bulletShooter = GetOwner<Boss>()->GetComponent<BulletShooter>();
}

void Boss1PageSmashSkillState::EnterState()
{
	State::EnterState();
	m_currentHitCount = 0;
	m_initialPos = GetOwner<Boss>()->GetPos();
	m_animator->Play(L"boss_Buff");
	m_animator->GetCurrent()->SetFlipX(true);
	m_animator->GetCurrent()->SetScale(6);
	m_incHitCntCallback = GetOwner<Boss>()->OnDamageApplied = [this](Boss* b, Entity* dealer)
		{
			m_currentHitCount++;
		};
	GET_SINGLE(ImpulseManager)->ApplyImpulse(3.f, m_smashTime);
}

void Boss1PageSmashSkillState::LateUpdate()
{
	State::LateUpdate();

	if (!m_updateTask)
		return;
	
	Boss* boss = GetOwner<Boss>();

	m_currentTime += fDT;
	if (m_currentHitCount >= m_cancelSkillHitCount)
	{
		m_updateTask = false;
		boss->ApplyDamage(20, nullptr);
		m_animator->Play(L"boss_Groggy", PlayMode::Once, 1, 2.f, [this, boss]()
			{
				GET_SINGLE(ImpulseManager)->ApplyImpulse(4.f, 0.3f);
				boss->ChangeState("Idle");
			});
		m_animator->GetCurrent()->SetFlipX(true);
		m_animator->GetCurrent()->SetScale(6);
	}
	else if (m_currentTime >= m_smashTime)
	{
		m_updateTask = false;
		boss->HitEnable(false);
		m_animator->Play(L"boss_Disappear", PlayMode::Once, 1, 1.2f, [this, boss]()
			{
				Player* target = GET_SINGLE(SceneManager)->GetCurScene()->GetNearbyObject<Player>(GetOwner<Boss>(), Layer::PLAYER);
				GET_SINGLE(ResourceManager)->Play(L"BossSpawn");
				if (target != nullptr)
				{
					boss->SetPos({ target->GetPos().x, target->GetPos().y });
					std::vector<Collider*> detected;
					if (Overlap::OverlapBox(boss->GetPos(), m_smashSize, Layer::PLAYER, detected, true))
					{
						for (auto col : detected)
						{
							Entity* entity = dynamic_cast<Entity*>(col->GetOwner());
							if (entity == nullptr)
								continue;
							entity->ApplyDamage(m_damage, boss);
						}
					}
				}
				else
				{
					cout << "Can not find Player in BossSmashSkill" << '\n';
					boss->SetPos(m_initialPos);
				}
				m_bulletShooter->ShootAround(150, 15, Layer::PLAYER, 75.f);
				boss->HitEnable(true);
				GET_SINGLE(ImpulseManager)->ApplyImpulse(6.f, 3.f);
				m_animator->Play(L"boss_Smash_End", PlayMode::Once, 1, 1.f, [this, boss]()
					{
						boss->ChangeState("Idle");
					});
					m_animator->GetCurrent()->SetFlipX(true);
					m_animator->GetCurrent()->SetScale(6);
			});
			m_animator->GetCurrent()->SetFlipX(true);
		    m_animator->GetCurrent()->SetScale(6);
	}
}

void Boss1PageSmashSkillState::ExitState()
{
	State::ExitState();
	GET_SINGLE(TweenManager)->MoveTo(GetOwner<Boss>(), m_initialPos, 0.5f, EaseType::EaseInOutQuad);
	m_currentTime = 0.f;
	m_currentHitCount = 0;
	m_currentTime = 0.f;
	m_updateTask = true;
	m_incHitCntCallback = {};
}
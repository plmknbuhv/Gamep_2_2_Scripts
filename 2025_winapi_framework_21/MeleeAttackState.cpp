#include "pch.h"
#include "MeleeAttackState.h"
#include "SceneManager.h"
#include "ImpulseManager.h"
#include "Animator.h"
#include "EffectManager.h"
#include "PlayerBulletEffect.h"
#include "Player.h"
#include "Overlap.h"
#include "Health.h"

MeleeAttackState::MeleeAttackState(Player* _player)
	: PlayerState(_player)
	, m_timer(0)
{

}

MeleeAttackState::~MeleeAttackState()
{
}

void MeleeAttackState::EnterState()
{
	m_timer = 0;
	m_animator->Play(L"attack", PlayMode::Once, 1, 1.f, [this]()
		{
			m_player->ChangeState("IDLE");
		}, 5, 4, [this](){	Attack(); }); 
}

void MeleeAttackState::Attack()
{
	const Vec2 offset = Vec2(75.f, 35.f);
	Vec2 pos = m_player->GetPos();
	pos += offset;

	vector<Collider*> colliders;
	if (Overlap::OverlapBox(
		pos, { 200, 185 }, Layer::ENEMY_PROJECTILE, colliders, false))
	{
		for (auto col : colliders)
		{
			// 만약 적 총알이면
			if (L"EnemyBullet" == col->GetName()) 
			{
				GET_SINGLE(EffectManager)->SpawnEffect<PlayerBulletEffect>(col->GetOwner()->GetPos());
				GET_SINGLE(SceneManager)->
					RequestDestroy(col->GetOwner());
			}
		}
	}
	

	if (Overlap::OverlapBox(
		pos, { 200, 185 }, Layer::ENEMY, colliders, false))
	{
		for (auto col : colliders)
		{
			if (L"Enemy" == col->GetName())
			{
				auto entity = dynamic_cast<Entity*>(col->GetOwner());
				if (std::find(m_alreadyHitEntity.begin(), m_alreadyHitEntity.end(), entity) == m_alreadyHitEntity.end())
				{
					GET_SINGLE(ImpulseManager)->ApplyImpulse(5.f, 0.45f);
					m_alreadyHitEntity.push_back(entity);
					entity->ApplyDamage(m_player->GetDamage(), m_player);
				}
			}
		}
	}

	if (Overlap::OverlapBox(
		pos, { 200, 185 }, Layer::BABY, colliders, false))
	{
		for (auto col : colliders)
		{
			if (L"Enemy" == col->GetName())
			{
				auto entity = dynamic_cast<Entity*>(col->GetOwner());
				if (std::find(m_alreadyHitEntity.begin(), m_alreadyHitEntity.end(), entity) == m_alreadyHitEntity.end())
				{
					GET_SINGLE(EffectManager)->SpawnEffect<PlayerBulletEffect>(entity->GetPos());
					m_alreadyHitEntity.push_back(entity);
					entity->ApplyDamage(m_player->GetDamage(), m_player);
				}
			}
		}
	}

	GET_SINGLE(ImpulseManager)->ApplyImpulse(3.f, 0.3f);
}

void MeleeAttackState::ExitState()
{
	m_alreadyHitEntity.clear();
}

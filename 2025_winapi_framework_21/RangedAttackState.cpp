#include "pch.h"
#include "SceneManager.h"
#include "RangedAttackState.h"
#include "PlayerBullet.h"
#include "Animator.h"
#include "Player.h"

RangedAttackState::RangedAttackState(Player* _player)
	: PlayerState(_player)
	, m_timer(0)
	, m_attackCount(0)
{
}

RangedAttackState::~RangedAttackState()
{
}

void RangedAttackState::EnterState()
{
	m_animator->Play(L"attack", PlayMode::Once, 1, 1.f, [this]()
		{
			m_player->ChangeState("IDLE");
		});

	m_timer = 0;
	m_attackCount = 0;
}

void RangedAttackState::LateUpdate()
{
	const Vec2 secondMuzzle = Vec2(40.f, 15.f);
	const Vec2 firstMuzzle = Vec2(-35.f, 30.f);
	Vec2 basePos = m_player->GetPos();

	m_timer += fDT;

	if (m_timer >= 0.105f)
	{
		auto bullet = new PlayerBullet(m_player);
		bullet->SetPos(basePos + (m_attackCount % 2 == 0
			? firstMuzzle : secondMuzzle));
		bullet->SetSize({ 50, 45 });
		GET_SINGLE(SceneManager)->GetCurScene()->AddObject(bullet, Layer::PLAYER_PROJECTILE);

		m_attackCount++;
		m_timer = 0;
	}
}

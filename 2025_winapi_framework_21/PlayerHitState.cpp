#include "pch.h"
#include "PlayerHitState.h"
#include "Animator.h"
#include "Player.h"

PlayerHitState::PlayerHitState(Player* _player)
	: PlayerState(_player)
	, m_timer(0)
{
}

PlayerHitState::~PlayerHitState()
{
}

void PlayerHitState::EnterState()
{
	m_player->SetCanHit(false);

	m_timer = 0;
	m_animator->Play(L"hit", PlayMode::Once, 1, 1.f, [this]()
		{
			m_player->ChangeState("IDLE");
		});
}

void PlayerHitState::LateUpdate()
{
	m_timer += fDT;
}

void PlayerHitState::ExitState()
{
	m_player->SetCanHit(true);
}

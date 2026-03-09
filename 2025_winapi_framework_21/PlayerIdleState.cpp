#include "pch.h"
#include "PlayerIdleState.h"
#include "Player.h"
#include "Animator.h"

PlayerIdleState::PlayerIdleState(Player* _player)
	: PlayerState(_player)
{
}

PlayerIdleState::~PlayerIdleState()
{
}

void PlayerIdleState::EnterState()
{
	m_animator->Play(L"idle");
}

void PlayerIdleState::LateUpdate()
{
	Vec2 dir = Vec2();
	m_player->MoveInput(dir);
	if (dir != Vec2(0, 0))
		m_player->ChangeState("MOVE");
}

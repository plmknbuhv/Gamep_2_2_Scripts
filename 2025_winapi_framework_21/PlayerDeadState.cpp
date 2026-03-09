#include "pch.h"
#include "PlayerDeadState.h"
#include "GameManager.h"
#include "Collider.h"
#include "Player.h"

PlayerDeadState::PlayerDeadState(Player* _player)
	: PlayerState(_player)
{
}

PlayerDeadState::~PlayerDeadState()
{
}

void PlayerDeadState::EnterState()
{
	m_player->SetCanHit(false);
	auto collider = m_player->GetComponent<Collider>();
	collider->SetEnable(false);

	m_animator->Play(L"dead", PlayMode::Once, 1, 1, [this]()
		{
			GET_SINGLE(GameManager)->AddDeadCount();
		});
}                             

#include "pch.h"
#include "PlayerState.h"
#include "Player.h"

PlayerState::PlayerState(Player* _player) 
	: State(_player)
{
	m_player = _player;
	assert(_player != nullptr, "플레이어가 아닌뎅");

	m_animator = _player->GetComponent<Animator>();
	assert(m_animator != nullptr, "에니메이터가 없는뎅");
}

PlayerState::~PlayerState()
{
}

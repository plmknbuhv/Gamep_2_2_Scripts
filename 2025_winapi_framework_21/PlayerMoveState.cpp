#include "pch.h"
#include "PlayerMoveState.h"
#include "Player.h"
#include "Animator.h"
#include <algorithm>

PlayerMoveState::PlayerMoveState(Player* _player) 
	: PlayerState(_player)
	, m_moveSign(0)
	, m_moveSpeed(0)
{
	m_moveSpeed = m_player->GetMoveSpeed();
}

PlayerMoveState::~PlayerMoveState()
{
}

void PlayerMoveState::EnterState()
{
	m_moveSign = 0;
}

void PlayerMoveState::LateUpdate()
{
	Vec2 dir = {};
	m_player->MoveInput(dir);

	if (dir == Vec2(0, 0))
		m_player->ChangeState("IDLE");

	dir.Normalize();
	m_player->Translate(
		{ dir.x * m_moveSpeed * fDT, dir.y * m_moveSpeed * fDT });

	ClampPlayerPosition();
	HandleAnimation(dir);
}

void PlayerMoveState::ClampPlayerPosition()
{
	Vec2 pos = m_player->GetPos();
	Vec2 halfSize = m_player->GetSize();
	halfSize /= 2.f;

	if (pos.x > WINDOW_WIDTH - halfSize.x + 20)
		pos.x = WINDOW_WIDTH - halfSize.x + 20;
	else if (pos.x < 0 + halfSize.x)
		pos.x = 0 + halfSize.x;

	if (pos.y > WINDOW_HEIGHT - halfSize.y)
		pos.y = WINDOW_HEIGHT - halfSize.y;
	else if (pos.y < 0 + halfSize.y - 20)
		pos.y = 0 + halfSize.y - 20;

	m_player->SetPos(pos);
}

void PlayerMoveState::HandleAnimation(Vec2 _dir)
{
	if (_dir.x > 0) // 앞으로
		AnimMoveFront();
	else if (_dir.x < 0) // 뒤로
		AnimMoveBack();
	else if (_dir.y > 0) // 앞으로
		AnimMoveFront();
	else if (_dir.y < 0) // 뒤로
		AnimMoveBack();
}

void PlayerMoveState::AnimMoveFront()
{
	if (m_moveSign == 1) return;

	m_animator->Play(L"moveFront");
	m_moveSign = 1;
}

void PlayerMoveState::AnimMoveBack()
{
	if (m_moveSign == -1) return;

	m_animator->Play(L"moveBack");
	m_moveSign = -1;
}

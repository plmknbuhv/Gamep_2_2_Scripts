#include "pch.h"
#include "BossMoveState.h"
#include "Animation.h"
#include "Animator.h"
#include "Boss.h"
#include <cstdlib>

BossMoveState::BossMoveState(Boss* _bs)
	: State(_bs)
{
}

void BossMoveState::EnterState()
{
	Boss* owner = GetOwner<Boss>();
	owner->m_targetPos = owner->GetPos();
	owner->m_moveTimer = 0.f;
	owner->m_moveTimer = owner->m_moveInterval;
	owner->GetComponent<Animator>()->Play(L"boss_Move");
}

void BossMoveState::LateUpdate()
{
	Boss* owner = GetOwner<Boss>();
	Vec2 pos = owner->GetPos();

	owner->m_moveTimer += fDT;
	if (owner->m_moveTimer >= owner->m_moveInterval)
	{
		owner->m_moveTimer = 0.f;

		Vec2 bossSize = owner->GetSize();
		float halfW = bossSize.x * 0.5f;
		float halfH = bossSize.y * 0.5f;

		float rangeX = max(0.f, WINDOW_WIDTH - bossSize.x);
		float rangeY = max(0.f, WINDOW_HEIGHT - bossSize.y);

		float rx = halfW + (rangeX > 0.f ? (static_cast<float>(rand()) / RAND_MAX) * rangeX : 0.f);
		float ry = halfH + (rangeY > 0.f ? (static_cast<float>(rand()) / RAND_MAX) * rangeY : 0.f);

		owner->m_targetPos = { rx, ry };
	}

	Vec2 dir = owner->m_targetPos - pos;
	float dist = dir.Length();
	if (dist <= 1.f)
		return;

	dir.Normalize();
	Vec2 move = dir * min(owner->m_moveSpeed * fDT, dist);
	owner->Translate(move);
}

void BossMoveState::ExitState()
{
}
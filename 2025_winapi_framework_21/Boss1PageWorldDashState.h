#pragma once
#include "State.h"
#include "Animator.h"
#include "DashImpactArea.h"

class Boss;
class Boss1PageWorldDashState
	: public State
{
public:
	Boss1PageWorldDashState(Boss* bs);
	~Boss1PageWorldDashState() override;

public:
	virtual void EnterState() override;
	virtual void LateUpdate() override;
	virtual void ExitState() override;

public:
	int m_currentDashCount;
	int m_maxMoveX;
	int m_damage;
	float m_dashDuration;
	float m_nextDashTime;
	float m_currentTime;
	bool m_enableCalculateTime;
	bool m_alreadyDrawArea;
	DashImpactArea* m_impactArea;
	Vector2 m_initialPos;
	// 출발 위치와 도착 위치를 저장하는 벡터
	std::vector<std::pair<Vector2, Vector2>> m_dashInfo;
	Animator* m_animator;
	bool m_isDashing;
};


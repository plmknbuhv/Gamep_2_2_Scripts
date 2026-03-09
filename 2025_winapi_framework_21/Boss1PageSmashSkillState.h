#pragma once
#include "State.h"
#include "Animator.h"
#include "BulletShooter.h"

class Boss;
class Boss1PageSmashSkillState 
	: public State
{
public:
	Boss1PageSmashSkillState(Boss* _bs);

public:
	virtual void EnterState() override;
	virtual void LateUpdate() override;
	virtual void ExitState() override;
	void IncreaseHitCount()
	{
		m_currentHitCount++;
	}
public:
	Vec2 m_initialPos;
	int m_cancelSkillHitCount;
	int m_currentHitCount;
	Vec2 m_smashSize;
	float m_smashTime;
	float m_currentTime;
	float m_damage;
	bool m_updateTask;
	Animator* m_animator;
	BulletShooter* m_bulletShooter;
	std::function<void(Boss* b, Entity* d)> m_incHitCntCallback;
};


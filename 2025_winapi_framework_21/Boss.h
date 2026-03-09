#pragma once
#include "Entity.h"
#include "Animator.h"
#include <functional>
#include "Collider.h"
#include "Enums.h"

class Texture;
class Boss : public Entity
{
public:
	Boss();
	void CreateState();
	void CreateAnimator(BossPhase phase);
	void CreateSkill(StateMachine* stateMachine);
	~Boss();

	std::function<void(Boss* boss, Entity* dealer)> OnDamageApplied;
	std::function<void(Boss* boss)> OnBossDie;
	void SetStat(float _skillCooldown);
	void ApplyDamage(int damage, Entity* dealer) override;

public:
	void EnterCollision(Collider* _other) override;
	Health* GetHealthCompo();
	void SetApplyDamage(bool apply = true)
	{
		m_applyDamage = apply;
	}

public:
	void HitEnable(bool enable = true)
	{
		GetComponent<Collider>()->SetEnable(enable);
	}
	void ChangePhase(BossPhase phase);
	void StopAll();
	BossPhase GetCurrentPhase() const
	{
		return m_currentBossPhase;
	}

private:
	Texture* m_pTexture_page1;
	Texture* m_pTexture_page2;
	Animator* m_pAnimator;
	Vector2 m_size;
	bool m_applyDamage;

public:
	bool m_initialized;
	BossPhase m_currentBossPhase;

#pragma region IdleState Stat

public:
	float m_skillCooldown;

#pragma endregion

#pragma region MoveState Stat

public:
	float m_moveTimer;
	float m_moveInterval;
	Vec2  m_targetPos;
	float m_moveSpeed;

#pragma endregion
};

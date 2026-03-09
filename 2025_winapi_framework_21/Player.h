#pragma once
#include "Object.h"
#include "Entity.h"
#include "Animator.h"

class Texture;
class Aniamtor;
class Player 
	: public Entity
{
public:
	Player(wstring _textureName, float _moveSpeed,
		int _health, int _damage);
	virtual ~Player() override;
public:
	void Update() override;
	void Render(HDC _hdc) override;
protected:
	virtual void CreateAnimation();
	virtual void CreateState();
	void HandleAttack();
public:
	virtual void MoveInput(Vec2& _dir) abstract;
	void ApplyDamage(int _damage, Entity* _dealer) override;
	int GetDamage() { return m_damage; }
public:
	float GetMoveSpeed()
	{
		return m_moveSpeed;
	};
	PlayerType GetPlayerType()
	{
		return m_playerType;
	}
protected:
	Texture* m_pTexture;
	Animator* m_animator;
	float m_moveSpeed;
	PlayerType m_playerType;
	const int m_damage;
	const float m_renderSize = 2;
};


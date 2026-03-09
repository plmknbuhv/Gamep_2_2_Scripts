#pragma once
#include "Object.h"

class Health;
class StateMachine;
class Entity 
	: public Object
{
public:
	Entity();
	virtual ~Entity() override;
public:
	void virtual ApplyDamage(int _damage, Entity* _dealer);
	void ChangeState(std::string _stateName);
	void SetCanHit(bool _isCanHit) { m_isCanHit = _isCanHit; }
protected:
	Health* m_health;
	StateMachine* m_stateMachine;
	bool m_isCanHit;
};


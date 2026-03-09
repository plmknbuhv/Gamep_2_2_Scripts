#pragma once
#include "Component.h"
#include "Entity.h"

class Health 
	: public Component
{
public:
	Health();
	~Health();
public:
public:
	void SetUpHealth(int _health);
	void ApplyDamage(int _damage, Entity* _dealer);
	int GetCurrentHealth();
	int GetMaxHealth();
private:
	int m_maxHealth;
	int m_currentHealth;
};


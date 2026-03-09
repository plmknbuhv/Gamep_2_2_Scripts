#include "pch.h"
#include "Health.h"
#include "SceneManager.h"

Health::Health()
	: m_maxHealth(0)
	, m_currentHealth(0)
{
}

Health::~Health()
{
}

void Health::SetUpHealth(int _health)
{
	m_maxHealth = _health;
	m_currentHealth = _health;
}

void Health::ApplyDamage(int _damage, Entity* _dealer)
{
	m_currentHealth -= _damage;
	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;
	}
}

int Health::GetCurrentHealth()
{
	return m_currentHealth;
}

int Health::GetMaxHealth()
{
	return m_maxHealth;
}

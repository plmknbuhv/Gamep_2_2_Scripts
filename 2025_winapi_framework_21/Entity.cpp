#include "pch.h"
#include "Entity.h"
#include "Health.h"
#include "StateMachine.h"

Entity::Entity()
	: m_health(nullptr)
	, m_stateMachine(nullptr)
	, m_isCanHit(true)
{
	m_health = AddComponent<Health>();
}

Entity::~Entity()
{
}

void Entity::ApplyDamage(int _damage, Entity* _dealer)
{
	if (m_isCanHit == false) return;

	m_health->ApplyDamage(_damage, _dealer);
}

void Entity::ChangeState(std::string _stateName)
{
	m_stateMachine->ChangeState(_stateName);
}
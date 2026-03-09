#include "pch.h"
#include "State.h"

State::State(Entity* _entity)
	: m_owner(_entity)
{
}

void State::EnterState()
{
}

void State::LateUpdate()
{
}

void State::ExitState()
{
}

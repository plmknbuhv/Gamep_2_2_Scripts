#include "pch.h"
#include "StateMachine.h"
#include "State.h"

StateMachine::StateMachine()
	: m_stateList(NULL), m_currentState(NULL), m_previousState(NULL)
{
}

StateMachine::~StateMachine()
{
	ResetStateList();
}

void StateMachine::ChangeState(std::string _stateName)
{
	if(m_stateList.find(_stateName) == m_stateList.end())
	{
		assert(false && L"State not found in StateMachine");
		return;
	}
	if (m_currentState != nullptr)
	{
		m_previousState = m_currentState;
		m_previousState->ExitState();
	}
	m_currentState = m_stateList[_stateName];
	m_currentState->EnterState();
}

State* StateMachine::GetCurrentState()
{
	return m_currentState;
}

void StateMachine::AddState(std::string _stateName, State* _state)
{
	m_stateList[_stateName] = _state;
}

void StateMachine::ResetStateList()
{
	for (auto [name, statePtr] : m_stateList)
	{
		SAFE_DELETE(statePtr)
		//delete statePtr;
	}
	m_stateList.clear();
}

void StateMachine::Init()
{
}

void StateMachine::LateUpdate()
{
	if (m_currentState != nullptr)
	{
		m_currentState->LateUpdate();
	}
}

void StateMachine::Render(HDC _hdc)
{
}

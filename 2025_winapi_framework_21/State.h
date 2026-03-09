#pragma once
#include <type_traits>
#include "Entity.h"

class StateMachine;

class State
{
public:
	virtual ~State() = default;
	State(Entity* _entity);
public:
	virtual void EnterState();
	virtual void LateUpdate();
	virtual void ExitState();
	template<typename T>
	T* GetOwner()
	{
		static_assert(std::is_base_of<Entity, T>::value, "Object?¥ê??? ?????? ??");
		T* bs = dynamic_cast<T*>(m_owner);
		if(bs == nullptr)
		{
			assert(false && L"State?? ???? ????? ?????");
		}
		return bs;
	}
private:
	Entity* m_owner;
};
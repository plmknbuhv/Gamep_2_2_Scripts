#include "pch.h"
#include "Object.h"
#include "InputManager.h"
#include "Component.h"
#include "Animator.h"
Object::Object()
	: m_pos{0,0}
	, m_size{0,0}
{
	m_isDie = false;
}

Object::~Object()
{
	for (Component* com : m_vecComponents)
		SAFE_DELETE(com)
	m_vecComponents.clear();
}

void Object::Update()
{
}

void Object::LateUpdate()
{
	if (m_vecComponents.empty()) return;
	for (Component* com : m_vecComponents)
	{
		if (com != nullptr)
		{
			com->LateUpdate();
		}
	}

}

void Object::Render(HDC _hdc)
{
	auto* anim = GetComponent<Animator>();
	if(anim != nullptr)
	{
		anim->Render(_hdc);
	}
}

void Object::ComponentRender(HDC _hdc)
{
	for (Component* com : m_vecComponents)
	{
		if (com != nullptr)
			com->Render(_hdc);
	}
}

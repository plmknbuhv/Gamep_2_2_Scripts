#include "pch.h"
#include "Collider.h"
#include "Object.h"
UINT Collider::m_nextID = 0;
Collider::Collider()
		: m_ID(m_nextID++)
		, m_showDebug(false)
		,m_enable(true)
{
	m_enable = true;
}
Collider::~Collider()
{
}
void Collider::EnterCollision(Collider* _other)
{
	if (m_enable != true) return;
	m_showDebug = true;
	GetOwner()->EnterCollision(_other);

}
void Collider::StayCollision(Collider* _other)
{
	if (m_enable != true) return;
	GetOwner()->StayCollision(_other);
}

void Collider::ExitCollision(Collider* _other)
{
	if (m_enable != true) return;
	m_showDebug = false;
	GetOwner()->ExitCollision(_other);
}
void Collider::Render(HDC _hdc)
{
	// 이거 나중에 다 주석처리 해야함
	//PenType colorPen = PenType::GREEN;
	//if(m_showDebug)
	//	colorPen = PenType::RED;
	//GDISelector pen(_hdc, colorPen);
	//GDISelector brush(_hdc, BrushType::HOLLOW);

	//RECT_RENDER(_hdc, m_updatedPos.x, m_updatedPos.y
	//	, m_size.x, m_size.y);
}

void Collider::Init()
{
	m_size = { 30.f,30.f };
	m_updatedPos = { 0.f,0.f };
	m_offsetPos = { 0.f,0.f };
}

void Collider::LateUpdate()
{
	Object* owner = GetOwner();
	Vec2 pos = owner->GetPos();
	m_updatedPos = pos + m_offsetPos;
}
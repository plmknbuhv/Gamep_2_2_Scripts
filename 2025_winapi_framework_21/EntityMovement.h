#pragma once
#include "Component.h"
class EntityMovement : public Component
{
public:
	EntityMovement();
	~EntityMovement();
public:
	void Init() override;
	void LateUpdate() override;
	void Render(HDC _hdc) override;
public:

private:
	Vec2 m_moveDir;
};


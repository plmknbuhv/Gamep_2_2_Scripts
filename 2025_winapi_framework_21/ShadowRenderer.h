#pragma once
#include "Component.h"

class Texture;
class ShadowRenderer
	: public Component
{
public:
	ShadowRenderer();
	~ShadowRenderer();
public:
	void Init() override;
	void LateUpdate() override;
	void Render(HDC _hdc) override;
public:
	void SetUpShadow(Vec2 _offset, Vec2 _size);
private:
	Vec2 m_offset;
	Vec2 m_size;
	Texture* m_texture;
};


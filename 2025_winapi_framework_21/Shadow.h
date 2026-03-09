#pragma once
#include "Object.h"

class Texture;
class Object;
class Shadow :
    public Object
{
public:
	Shadow();
	~Shadow();
public:
	void LateUpdate() override;
	void Render(HDC _hdc) override;
public:
	void SetUpShadow(Vec2 _offset, Vec2 _size, Object* owner);
private:
	Vec2 m_offset;
	Vec2 m_size;
	Texture* m_texture;
	Object* m_owner;
};


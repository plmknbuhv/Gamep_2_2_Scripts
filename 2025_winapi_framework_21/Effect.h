#pragma once
#include "Object.h"

class Texture;
class Animator;
class Effect
	: public Object
{
public:
	Effect(std::wstring _textureName);
	virtual ~Effect();
public:
	void PlayEffect(Vec2 _pos);
	virtual void CreateEffectAnim() abstract;
protected:
	const std::wstring animationName = L"effect";
	
	Texture* m_pTexture;
	Animator* m_animator;
};


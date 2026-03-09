#include "pch.h"
#include "Effect.h"
#include "Animator.h"
#include "SceneManager.h"
#include "ResourceManager.h"

Effect::Effect(std::wstring _textureName)
	: m_pTexture(nullptr)
	, m_animator(nullptr)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(_textureName);

	m_animator = AddComponent<Animator>();
}

Effect::~Effect()
{
}

void Effect::PlayEffect(Vec2 _pos)
{
	SetPos(_pos);
	m_animator->Play(animationName, PlayMode::Once, 1, 1, [this]()
		{
			GET_SINGLE(SceneManager)->RequestDestroy(this);
		});
}

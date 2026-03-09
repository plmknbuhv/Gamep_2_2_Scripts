#include "pch.h"
#include "PlayerBulletEffect.h"
#include "Animator.h"
#include "Animation.h"

PlayerBulletEffect::PlayerBulletEffect()
	: Effect(L"playerBulletEffect")
{
	CreateEffectAnim();
}

PlayerBulletEffect::~PlayerBulletEffect()
{
}

void PlayerBulletEffect::CreateEffectAnim()
{
	m_animator->CreateAnimation
		(animationName
			, m_pTexture
			, { 0.f,0.f }
			, { 32.f,32.f }
			, { 32.f,0.f }
	, 4, 0.1f)->SetScale(2);
}

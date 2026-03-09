#include "pch.h"
#include "RangedPlayer.h"
#include "InputManager.h"
#include "Animator.h"
#include "Animation.h"
#include "StateMachine.h"
#include "RangedAttackState.h"
#include "ICanAttackState.h"

RangedPlayer::RangedPlayer() 
	: Player(L"rangedPlayer", 220.f, 110, 2)
{
	Player::m_playerType = PlayerType::Ranged;
	CreateAnimation();
	CreateState();

	m_stateMachine->ChangeState("IDLE");
}

RangedPlayer::~RangedPlayer()
{
}

void RangedPlayer::Update() 
{
	Player::Update();
	if (GET_KEY(KEY_TYPE::SPACE))
		HandleAttack();
}

void RangedPlayer::CreateAnimation()
{
	Player::CreateAnimation();

	m_animator->CreateAnimation
	(L"attack"
		, m_pTexture
		, { 0.f, 1199.f }
		, { 148.f, 108.f }
		, { 148.f,0.f }
	, 4, 0.11f)->SetScale(m_renderSize);
}

void RangedPlayer::CreateState()
{
	Player::CreateState();

	m_stateMachine->AddState("ATTACK", new RangedAttackState(this));
}

void RangedPlayer::MoveInput(Vec2& _dir)
{
	if (GET_KEY(KEY_TYPE::A)) _dir.x -= 1.f;
	if (GET_KEY(KEY_TYPE::D)) _dir.x += 1.f;
	if (GET_KEY(KEY_TYPE::W)) _dir.y -= 1.f;
	if (GET_KEY(KEY_TYPE::S)) _dir.y += 1.f;
}

void RangedPlayer::ApplyDamage(int _damage, Entity* _dealer)
{
	Player::ApplyDamage(_damage, _dealer);
}
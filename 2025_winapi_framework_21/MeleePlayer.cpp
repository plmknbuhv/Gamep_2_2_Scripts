#include "pch.h"
#include "MeleePlayer.h"
#include "InputManager.h"
#include "StateMachine.h"
#include "Animator.h"
#include "Animation.h"
#include "MeleeAttackState.h"

MeleePlayer::MeleePlayer() 
	: Player(L"meleePlayer", 310.f, 160, 8)
{
	Player::m_playerType = PlayerType::Melee;
	CreateAnimation();
	CreateState();

	m_stateMachine->ChangeState("IDLE");
}

MeleePlayer::~MeleePlayer()
{
}

void MeleePlayer::Update()
{
	Player::Update();
	if (GET_KEYDOWN(KEY_TYPE::ENTER))
		HandleAttack();
}

void MeleePlayer::CreateAnimation()
{
	Player::CreateAnimation();

	m_animator->CreateAnimation
	(L"attack"
		, m_pTexture
		, { 0.f,872.f }
		, { 148.f, 109.f }
		, { 148.f,0.f }
	, 10, 0.06f)->SetScale(m_renderSize);
}

void MeleePlayer::CreateState()
{
	Player::CreateState();

	m_stateMachine->AddState("ATTACK", new MeleeAttackState(this));
}

void MeleePlayer::MoveInput(Vec2& _dir)
{
	if (GET_KEY(KEY_TYPE::LEFT)) _dir.x -= 1.f;
	if (GET_KEY(KEY_TYPE::RIGHT)) _dir.x += 1.f;
	if (GET_KEY(KEY_TYPE::UP)) _dir.y -= 1.f;
	if (GET_KEY(KEY_TYPE::DOWN)) _dir.y += 1.f;
}

void MeleePlayer::ApplyDamage(int _damage, Entity* _dealer)
{
	Player::ApplyDamage(_damage, _dealer);
}
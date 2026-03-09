#include "pch.h"
#include "Player.h"
#include "StateMachine.h"
#include "Texture.h"
#include "Collider.h"
#include "Health.h"
#include "Shadow.h"
#include "Animator.h"
#include "Animation.h"
#include "PlayerIdleState.h"
#include "PlayerMoveState.h"
#include "PlayerHitState.h"
#include "PlayerDeadState.h"
#include "ResourceManager.h"
#include "ImpulseManager.h"
#include "SceneManager.h"

Player::Player(wstring _textureName, float _moveSpeed,
	int _health, int _damage)
	: m_pTexture(nullptr)
	, m_animator(nullptr)
	, m_moveSpeed(_moveSpeed)
	, m_damage(_damage)
{
	auto collider = AddComponent<Collider>();
	collider->SetSize({ 62, 62 });
	collider->SetOffSetPos({ -17, 10 });

	m_stateMachine = AddComponent<StateMachine>();
	
	
	auto shadow = GET_SINGLE(SceneManager)->GetCurScene()->
		Spawn<Shadow>(Layer::SHADOW, { 0, 0 }, { 0, 0 });
	shadow->SetUpShadow({ -10, 50 }, { 60, 60 }, this);

	m_health->SetUpHealth(_health);

	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(_textureName);

	m_animator = AddComponent<Animator>();
	m_animator->Init();

}

Player::~Player()
{

}

void Player::Update()
{
	Entity::Update();
}

void Player::Render(HDC _hdc)
{
	Entity::Render(_hdc);

	ComponentRender(_hdc);
}

void Player::CreateAnimation()
{
	m_animator->CreateAnimation
	(L"idle"
		, m_pTexture
		, { 0.f,763.f }
		, { 148.f,109.f }
		, { 148.f,0.f }
	, 8, 0.1f)->SetScale(m_renderSize);

	m_animator->CreateAnimation
	(L"moveFront"
		, m_pTexture
		, { 0.f,981.f }
		, { 148.f,109.f }
		, { 148.f,0.f }
	, 10, 0.1f)->SetScale(m_renderSize);

	m_animator->CreateAnimation
	(L"moveBack"
		, m_pTexture
		, { 1332.f,981.f }
		, { 148.f,109.f }
		, { -148.f,0.f }
	, 10, 0.1f)->SetScale(m_renderSize);

	m_animator->CreateAnimation
	(L"hit"
		, m_pTexture
		, { 1480.f,763.f }
		, { 148.f,109.f }
		, { 148.f,0.f }
	, 8, 0.1f)->SetScale(m_renderSize);

	m_animator->CreateAnimation
	(L"dead"
		, m_pTexture
		, { 0.f,2071.f }
		, { 148.f,109.f }
		, { 148.f,0.f }
	, 16, 0.1f)->SetScale(m_renderSize);
}

void Player::CreateState()
{
	m_stateMachine->AddState("IDLE", new PlayerIdleState(this));
	m_stateMachine->AddState("MOVE", new PlayerMoveState(this));
	m_stateMachine->AddState("HIT", new PlayerHitState(this));
	m_stateMachine->AddState("DAED", new PlayerDeadState(this));
}

void Player::HandleAttack()
{
	auto canAttackState = dynamic_cast<ICanAttackState*>
		(m_stateMachine->GetCurrentState());

	if (canAttackState != nullptr) // ���� �ɽ�Ʈ �����ߴٸ� ���� ������ State
		m_stateMachine->ChangeState("ATTACK");
}

void Player::ApplyDamage(int damage, Entity* dealer)
{
	if (m_isCanHit == false) return;

	Entity::ApplyDamage(damage, dealer);
	GET_SINGLE(ImpulseManager)->ApplyImpulse(7.5f, 0.8f);

	if (m_health->GetCurrentHealth() > 0)
		ChangeState("HIT");
	else
		ChangeState("DAED");
}
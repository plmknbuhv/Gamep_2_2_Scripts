#include "pch.h"
#include "PlayerBullet.h"
#include "ResourceManager.h"
#include "EffectManager.h"
#include "PlayerBulletEffect.h"
#include "SceneManager.h"
#include "ImpulseManager.h"
#include "Collider.h"
#include "Animator.h"
#include "Entity.h"
#include "Player.h"
#include "Animation.h"
#include "Texture.h"

PlayerBullet::PlayerBullet(Player* _owner)
	: m_owner(_owner)
	, m_pTexture(nullptr)
	, m_animator(nullptr)
	, m_timer(0)
{
	auto com = AddComponent<Collider>();
	com->SetName(L"PlayerBullet");
	com->SetTrigger(true);

	m_pTexture = GET_SINGLE(ResourceManager)
		->GetTexture(L"playerBullet");

	m_animator = AddComponent<Animator>();

	m_animator->CreateAnimation
	(L"idle"
		, m_pTexture
		, { 0.f,0.f }
		, { 16.f,16.f }
		, { 16.f,0.f }
	, 4, 0.1f)->SetScale(1.75f);

	m_animator->Play(L"idle");
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Update()
{
	Object::Update();

	Move();

	m_timer += fDT;
	if (m_timer >= m_lifeTime)
	{
		GET_SINGLE(SceneManager)->RequestDestroy(this);
	}
}

void PlayerBullet::Move()
{
	const float xSpeed = 1200.f;
	const float ySpeed = 100.f;

	Translate(Vec2(xSpeed * fDT, ySpeed * fDT)); // 오른쪽 고정
}


void PlayerBullet::Render(HDC _hdc)
{
	// 임시로 콜라이더 보이게
	Object::Render(_hdc);
	// ComponentRender(_hdc);
}

void PlayerBullet::EnterCollision(Collider* _other)
{
	if (_other->GetName() != L"Enemy")
		return;

	auto entity = dynamic_cast<Entity*>(_other->GetOwner());
	if (entity == nullptr) return; // 만약 엔티티 아니면 빠꾸

	entity->ApplyDamage(m_owner->GetDamage(), m_owner);

	GET_SINGLE(EffectManager)->SpawnEffect<PlayerBulletEffect>(GetPos());
	GET_SINGLE(ImpulseManager)->ApplyImpulse(2.3f, 0.2f);
	GET_SINGLE(SceneManager)->RequestDestroy(this);

}
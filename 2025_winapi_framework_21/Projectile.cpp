#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Collider.h"
#include "Entity.h"
#include "Health.h"
#include "TweenManager.h"
#include "Overlap.h"
#include "SceneManager.h"

Projectile::Projectile(int _damage, Layer _targetLayer, Object* _dealer)
	: // m_dir(-1.f)
	m_angle(0.f), m_damage(_damage), m_targetLayer(_targetLayer), m_dealer(_dealer), m_maxSpeed(500.f)
{
	m_pTexture = GET_SINGLE(ResourceManager)->GetTexture(L"BlueBullet");
	SetSize({40.f, 40.f});
	auto* com = AddComponent<Collider>();
	com->SetSize({ 30,30 });
	com->SetName(L"EnemyBullet");
	com->SetTrigger(true);
	m_speed = 0;

	auto getter = [this]() { return m_speed; };
	auto setter = [this](const float& v) { m_speed = v; };
	GET_SINGLE(TweenManager)->Start<float>(getter,setter, m_maxSpeed, 1.5f, EaseType::EaseOutExpo);
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	//Translate({ cosf(m_angle)*500.f* fDT,  sinf(m_angle) * 500.f * fDT});
	Translate({ m_dir.x*m_speed* fDT,  m_dir.y * m_speed * fDT});
	
	vector<Collider*> hits;
	if (Overlap::OverlapBox(GetPos(), GetSize(), m_targetLayer, hits))
	{
		for (auto hit : hits)
		{
			Entity* e = dynamic_cast<Entity*>(hit->GetOwner());
			Entity* dealer = static_cast<Entity*>(m_dealer);
			if (e == nullptr || dealer == nullptr)
				continue;
			e->ApplyDamage(m_damage, dealer);
		}
		GET_SINGLE(SceneManager)->GetCurScene()->RequestDestroy(this);
	}
}

void Projectile::Render(HDC _hdc)
{
	Vec2 pos = GetPos();
	Vec2 size = {GetSize().x* 1.5f,GetSize().y *1.5f};
	int width = m_pTexture->GetWidth();
	int height = m_pTexture->GetHeight();
	/*ELLIPSE_RENDER(_hdc, pos.x, pos.y
		, size.x, size.y);*/
		::TransparentBlt(_hdc
		, (int)(pos.x - size.x / 2)
		, (int)(pos.y - size.y / 2)
		, size.x , size.y,
		m_pTexture->GetTextureDC(),
		0, 0, width, height, RGB(255,0,255));
		ComponentRender(_hdc);
}

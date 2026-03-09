#include "pch.h"
#include "GameScene.h"
#include "CollisionManager.h"
#include "RangedPlayer.h"
#include "MeleePlayer.h"
#include "Boss.h"
#include "BossUIManager.h"
#include "Lightning.h"
#include "ResourceManager.h"

void GameScene::Init()
{
	GET_SINGLE(CollisionManager)->CheckLayer(Layer::PLAYER_PROJECTILE, Layer::ENEMY);

	Spawn<RangedPlayer>
		(Layer::PLAYER, { WINDOW_WIDTH / 6, WINDOW_HEIGHT / 2 }, { 150, 150 });
	Spawn<MeleePlayer>
		(Layer::PLAYER, { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 }, { 150, 150	});

	Boss* bs = Spawn<Boss>
		(Layer::ENEMY, { WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT / 2 }, { 128,128 });
	bs->SetStat(3.f);
	
	Spawn<BossUIManager>(Layer::UI, { WINDOW_WIDTH / 2, 50 }, { 1220, 35 })->SetBoss(bs);
	GET_SINGLE(ResourceManager)->Play(L"BGM");

	m_texture = GET_SINGLE(ResourceManager)->GetTexture(L"BackGround");
}

void GameScene::Update()
{
	Scene::Update();
}

void GameScene::Render(HDC _hdc)	
{
	RenderBackGround(_hdc);

	Scene::Render(_hdc);
}

void GameScene::RenderBackGround(HDC _hdc)
{
	int width = m_texture->GetWidth();
	int height = m_texture->GetHeight();

	::StretchBlt(_hdc, 0, 0	, WINDOW_WIDTH, WINDOW_HEIGHT,
		m_texture->GetTextureDC(), 0, 0, width, height, SRCCOPY);

	//::BitBlt(_hdc, 0, 0, 1020, 720,
	//	m_texture->GetTextureDC(), 0, 0, SRCCOPY);
}

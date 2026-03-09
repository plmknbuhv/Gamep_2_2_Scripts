#pragma once
#include "Scene.h"
class GameScene 
	: public Scene
{
public:
	void Init() override;
	void Update() override;
	void Render(HDC _hdc) override;
private:
	void RenderBackGround(HDC _hdc);
private:
	Texture* m_texture;
};

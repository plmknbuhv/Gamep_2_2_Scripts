#include "pch.h"
#include "GameManager.h"

#include "ResourceManager.h"
#include "SceneChangeObj.h"
#include "SceneManager.h"

void GameManager::Init(HWND _hWnd)
{
	// 더블버퍼링
	m_hShadowBit = ::CreateCompatibleBitmap(::GetDC(_hWnd), WINDOW_WIDTH, WINDOW_HEIGHT);
	m_hShadowDC = ::CreateCompatibleDC(::GetDC(_hWnd));
	m_deadCount = 0;
	m_isEnd = false;
}

void GameManager::AddDeadCount()
{
	if (m_isEnd)return;
	m_deadCount++;
	if (m_deadCount >= 2) // 둘 다 죽었다면
	{
		m_isEnd = true;
		GET_SINGLE(SceneManager)->GetCurScene()->Spawn<SceneChangeObj>(Layer::FADE, Vec2{ WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f }, Vec2{ WINDOW_WIDTH, WINDOW_HEIGHT });
	}
}

void GameManager::Release()
{
	::DeleteDC(m_hShadowDC);
	::DeleteObject(m_hShadowBit);
}

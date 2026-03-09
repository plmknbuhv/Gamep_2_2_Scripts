#include "pch.h"
#include "Core.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "TweenManager.h"
#include "ImpulseManager.h"
bool Core::Init(HWND _hWnd)
{
	m_hWnd = _hWnd;
	m_hDC = ::GetDC(m_hWnd);
	
	// 더블버퍼링
	// 1. 생성
	m_hBackBit = ::CreateCompatibleBitmap(m_hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_hBackDC = ::CreateCompatibleDC(m_hDC);

	// 2. 연결
	::SelectObject(m_hBackDC, m_hBackBit);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init();
	GET_SINGLE(GameManager)->Init(_hWnd);
	if (!GET_SINGLE(ResourceManager)->Init())
		return false;
	GET_SINGLE(SceneManager)->Init();

	return true;
}

void Core::MainUpdate()
{
    GET_SINGLE(TimeManager)->Update();
    {
        static float accumulator = 0.f;
        const float fixedDT = 1.f / 60.f; // 60fps 기준 물리 프레임

		accumulator += fDT;
		while (accumulator >= fixedDT)
		{
			GET_SINGLE(SceneManager)->FixedUpdate(fixedDT);
			GET_SINGLE(CollisionManager)->Update();
			accumulator -= fixedDT;
		}
    }
    // 트윈 업데이트 (프레임 델타 기반)
    GET_SINGLE(TweenManager)->Update(fDT);
    GET_SINGLE(InputManager)->Update();
    GET_SINGLE(ResourceManager)->FmodUpdate();
    GET_SINGLE(SceneManager)->Update();
    GET_SINGLE(ImpulseManager)->Update();
    //GET_SINGLE(CollisionManager)->Update(); 
}

void Core::MainRender()
{ 
	//::Rectangle(m_hBackDC, -1, -1, WINDOW_WIDTH +1 , WINDOW_HEIGHT +1 );
	::PatBlt(m_hBackDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, WHITENESS);
	GET_SINGLE(SceneManager)->Render(m_hBackDC);
	Vec2 offset = GET_SINGLE(ImpulseManager)->GetOffset();
	::BitBlt(m_hDC, offset.x, offset.y, WINDOW_WIDTH, WINDOW_HEIGHT, m_hBackDC, 0, 0, SRCCOPY);
}



void Core::GameLoop()
{
	//time(NULL);
	//clock();
	//FrameSync();

	/*static int cnt = 0;
	++cnt;
	static int prev = GetTickCount64();
	int cur = GetTickCount64();
	if (cur - prev >= 1000)
	{
	    prev = cur;
	    cnt = 0;
	}*/

	MainUpdate();
	MainRender();
	GET_SINGLE(SceneManager)->GetCurScene()->FlushEvent();
}


void Core::CleanUp()
{
	::DeleteDC(m_hBackDC);
	::DeleteObject(m_hBackBit);
	::ReleaseDC(m_hWnd, m_hDC);
	GET_SINGLE(ResourceManager)->Release();
	GET_SINGLE(GameManager)->Release();
}

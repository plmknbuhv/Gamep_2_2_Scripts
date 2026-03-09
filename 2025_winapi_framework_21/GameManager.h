#pragma once
class GameManager
{
	DECLARE_SINGLE(GameManager);
public:
	void Init(HWND _hWnd);
	void Release();
public:
	HBITMAP GetShadowBit() { return m_hShadowBit; }
	HDC GetShadowDC() { return m_hShadowDC; }
	void AddDeadCount();
private:
	int m_deadCount = 0;
	HBITMAP m_hShadowBit;
	HDC  m_hShadowDC;
	bool m_isEnd;
};


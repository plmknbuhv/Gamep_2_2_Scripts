#pragma once
#include "Component.h"

#include <functional>
#include <vector>
#include <string>

#include "ResourceManager.h"
#include "Texture.h"

class Button : public Component
{
public:
	Button();
	void Init() override;
	void LateUpdate() override;
	void Render(HDC _hdc) override;
public:
	void SetText(const std::wstring& t) { m_text = t; }
	void SetTextColor(COLORREF c) { m_textColor = c; }
	void AddOnClick(const std::function<void()>& cb) { m_onClicks.push_back(cb); }
private:
	bool HitTest(const POINT& pt) const; 
private:
	Texture* m_texture;
	Texture* m_Stexture;
	bool m_isHover = false;
	bool m_isPressed = false;
	bool m_pressStartedInside = false;
	std::wstring m_text = L"Button";
	COLORREF m_textColor = RGB(0, 0, 0);
	std::vector<std::function<void()>> m_onClicks;

private:

};


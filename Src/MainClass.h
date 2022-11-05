#pragma once

#include "../pch.h"

#include "DeviceResources.h"
#include "Renderer.h"

#define DEF_WIN_W 1600
#define DEF_WIN_H 900

static HINSTANCE m_hInstance;
static std::wstring m_windowClassName;

class MainClass
{
private:
	HMENU m_hMenu;
	RECT m_rc;
	HWND m_hWnd;

public:
	MainClass();
	~MainClass() {}

	HRESULT CreateDesktopWindow();

	HWND GetWindowHandle() { return m_hWnd; }

	static LRESULT CALLBACK StaticWindowProc(HWND hWnd,
											 UINT uMsg,
											 WPARAM wParam,
											 LPARAM lParam);

	HRESULT Run(std::shared_ptr<DeviceResources> deviceResources,
				std::shared_ptr<Renderer> renderer);
};

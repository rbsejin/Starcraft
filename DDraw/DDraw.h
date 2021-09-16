#pragma once

#include <ddraw.h>

class DDraw
{
public:
	DDraw();
	~DDraw();

	BOOL Initialize(HWND hwnd);
	BOOL BeginDraw();
	BOOL EndDraw();
	void Clear();
	void OnDraw();
	void DrawRect(int x, int y, int width, int height, DWORD color);
	void CleanUp();

private:
	LPDIRECTDRAW mDDraw;
	LPDIRECTDRAW7 mDDraw7;
	LPDIRECTDRAWSURFACE7 mDDPrimary;
	LPDIRECTDRAWSURFACE7 mDDBack;
	LPDIRECTDRAWCLIPPER mClipper;
	
	HWND mHwnd;

	char* mBackBuffer;
	DWORD mBackBufferPitch;
	DWORD mWidth;
	DWORD mHeight;
	RECT mWindowRect;
};

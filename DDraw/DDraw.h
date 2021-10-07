#pragma once

#include <ddraw.h>
#include "../Common/typedef.h"

class CImageData;

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
	BOOL DrawBitmap(int x, int y, int width, int height, char* bits);
	BOOL CalcClipArea(INT_VECTOR2* pivOutSrcStart, INT_VECTOR2* pivOutDestStart, INT_VECTOR2* pivOutDestSize, const INT_VECTOR2* pivPos, const INT_VECTOR2* pivImageSize);
	BOOL DrawImageData(int sx, int sy, const CImageData* pImgData);

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

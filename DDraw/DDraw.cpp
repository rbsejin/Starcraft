#include <windows.h>
#include <string>

#include "DDraw.h"
#include "../Util/Util.h"
#include "../ImageData/ImageData.h"

DDraw::DDraw()
	: mDDraw(nullptr),
	mDDraw7(nullptr),
	mDDPrimary(nullptr),
	mDDBack(nullptr),
	mClipper(nullptr),
	mHwnd(nullptr),
	mBackBuffer(nullptr),
	mBackBufferPitch(0),
	mWidth(0),
	mHeight(0),
	mWindowRect{}
{
}

DDraw::~DDraw()
{
	CleanUp();
}

BOOL DDraw::Initialize(HWND hwnd)
{
	mHwnd = hwnd;

	DDSURFACEDESC2 ddsd = { 0, };
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	HRESULT hr = DirectDrawCreate(nullptr, &mDDraw, nullptr);
	if (FAILED(hr))
	{
		return FALSE;
	}

	mDDraw->QueryInterface(IID_IDirectDraw7, (LPVOID*)&mDDraw7);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Fail to Create DirectDraw 7", L"Error", MB_OK);
		return FALSE;
	}

	hr = mDDraw7->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed to Set CooperativeLevel", L"ERROR", MB_OK);
		return FALSE;
	}

	hr = mDDraw7->CreateSurface(&ddsd, &mDDPrimary, nullptr);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed to CreateSurface", L"ERROR", MB_OK);
		return FALSE;
	}

	hr = mDDraw->CreateClipper(0, &mClipper, nullptr);
	if (FAILED(hr))
	{
		MessageBox(hwnd, L"Failed to Create Clipper", L"ERROR", MB_OK);
		return FALSE;
	}

	mClipper->SetHWnd(0, hwnd);
	mDDPrimary->SetClipper(mClipper);

	//OnUpdateWindowPos()
	{
		GetClientRect(mHwnd, &mWindowRect);
		::ClientToScreen(mHwnd, (POINT*)&mWindowRect.left);
		::ClientToScreen(mHwnd, (POINT*)&mWindowRect.right);
	}

	mWidth = mWindowRect.right - mWindowRect.left;
	mHeight = mWindowRect.bottom - mWindowRect.top;

	// CreateBackBuffer(DWORD width, DOWRD height)
	{
		DDSURFACEDESC2 ddsc = {};
		ddsc.dwSize = sizeof(DDSURFACEDESC2);
		ddsc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		ddsc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsc.dwWidth = mWidth;
		ddsc.dwHeight = mHeight;

		hr = mDDraw7->CreateSurface(&ddsc, &mDDBack, nullptr);
		if (FAILED(hr))
		{
			return FALSE;
		}

		mDDBack->GetSurfaceDesc(&ddsc);
	}

	return TRUE;
}

BOOL DDraw::BeginDraw()
{
	DDSURFACEDESC2 ddsc = {};
	ddsc.dwSize = sizeof(DDSURFACEDESC2);

	HRESULT hr = mDDBack->Lock(nullptr, &ddsc, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);
	if (FAILED(hr))
	{
		return FALSE;
	}

	mBackBuffer = (char*)ddsc.lpSurface;
	mBackBufferPitch = ddsc.lPitch;

	return TRUE;
}

BOOL DDraw::EndDraw()
{
	HRESULT hr = mDDBack->Unlock(nullptr);
	if (FAILED(hr))
	{
		return FALSE;
	}

	mBackBuffer = nullptr;
	mBackBufferPitch = 0;
	return TRUE;
}

void DDraw::Clear()
{
	if (mBackBuffer == nullptr)
	{
		return;
	}

	for (DWORD y = 0; y < mHeight; y++)
	{
		memset(mBackBuffer + y * mBackBufferPitch, 0, 4 * mWidth);
	}
}

void DDraw::OnDraw()
{
	mDDPrimary->Blt(&mWindowRect, mDDBack, nullptr, DDBLT_WAIT, nullptr);
}

void DDraw::DrawRect(int x, int y, int width, int height, DWORD color)
{
	int startX = max(0, x);
	int startY = max(0, y);
	int endX = min(x + width, (int)mWidth);
	int endY = min(y + height, (int)mHeight);

	for (int y = startY; y < endY; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			DWORD* pDest = (DWORD*)(mBackBuffer + x * 4 + y * mBackBufferPitch);
			*pDest = color;
		}
	}
}

BOOL DDraw::DrawBitmap(int x, int y, int width, int height, char* bits)
{
	INT_VECTOR2 srcStart = {};
	INT_VECTOR2 destStart = {};

	INT_VECTOR2 position = { x, y };
	INT_VECTOR2 imageSize = { width, height };
	INT_VECTOR2 destSize = {};

	if (!CalcClipArea(&srcStart, &destStart, &destSize, &position, &imageSize))
	{
		return FALSE;
	}

	char* src = bits + (srcStart.x + srcStart.y * width) * 4;
	char* dest = mBackBuffer + (destStart.x * 4) + destStart.y * mBackBufferPitch;

	for (int y = 0; y < destSize.y; y++)
	{
		for (int x = 0; x < destSize.x; x++)
		{
			*(DWORD*)dest = *(DWORD*)src;
			src += 4;
			dest += 4;
		}

		src -= destSize.x * 4;
		src += width * 4;
		dest -= destSize.x * 4;
		dest += mBackBufferPitch;
	}

	return TRUE;
}

BOOL DDraw::CalcClipArea(INT_VECTOR2* outSrcStart, INT_VECTOR2* outDestStart, INT_VECTOR2* outDestSize, const INT_VECTOR2* position, const INT_VECTOR2* imageSize)
{
	INT_VECTOR2	bufferSize = { (int)mWidth, (int)mHeight };
	return ::CalcClipArea(outSrcStart, outDestStart, outDestSize, position, imageSize, &bufferSize);
}

BOOL DDraw::DrawImageData(int sx, int sy, const CImageData* pImgData)
{
#ifdef _DEBUG
	if (!mBackBuffer)
		__debugbreak();
#endif

	int iScreenWidth = (int)mWidth;

	int iBitmapWidth = (int)pImgData->GetWidth();
	int iBitmapHeight = (int)pImgData->GetHeight();

	INT_VECTOR2	ivSrcStart = {};
	INT_VECTOR2	ivDestStart = {};

	INT_VECTOR2	ivPos = { sx, sy };
	INT_VECTOR2	ivImageSize = { iBitmapWidth, iBitmapHeight };
	INT_VECTOR2 ivDestSize = {};

	if (!CalcClipArea(&ivSrcStart, &ivDestStart, &ivDestSize, &ivPos, &ivImageSize))
	{
		return FALSE;
	}

	const COMPRESSED_LINE* pLineDesc = pImgData->GetCompressedImage(ivSrcStart.y);
	char* pDestPerLine = mBackBuffer + (ivDestStart.y) * mBackBufferPitch;

	for (int y = 0; y < ivDestSize.y; y++)
	{
		for (DWORD i = 0; i < pLineDesc->dwStreamNum; i++)
		{
			PIXEL_STREAM* pStream = pLineDesc->pPixelStream + i;
			DWORD	dwPixelColor = pStream->dwPixel;
			int		iPixelNum = (int)pStream->wPixelNum;

			int dest_x = sx + (int)pStream->wPosX;
			if (dest_x < 0)
			{
				iPixelNum += dest_x;
				dest_x = 0;
			}
			if (dest_x + iPixelNum > iScreenWidth)
			{
				iPixelNum = iScreenWidth - dest_x;
			}
			char* pDest = pDestPerLine + (DWORD)(dest_x * 4);
			for (int x = 0; x < iPixelNum; x++)
			{
				*(DWORD*)pDest = dwPixelColor;
				pDest += 4;
			}
		}
		pLineDesc++;
		pDestPerLine += mBackBufferPitch;
	}

	return TRUE;
}


void DDraw::CleanUp()
{
	if (mDDBack != nullptr)
	{
		mDDBack->Release();
		mDDBack = nullptr;
	}

	if (mClipper != nullptr)
	{
		mClipper->Release();
		mClipper = nullptr;
	}

	if (mDDPrimary != nullptr)
	{
		mDDPrimary->Release();
		mDDPrimary = nullptr;
	}

	if (mDDraw7 != nullptr)
	{
		mDDraw7->Release();
		mDDraw7 = nullptr;
	}

	if (mDDraw != nullptr)
	{
		mDDraw->Release();
		mDDraw = nullptr;
	}
}

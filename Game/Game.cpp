#include <windows.h>

#include "Game.h"
#include "../DDraw/DDraw.h"
#include "../util/JPGImage.h"
#include "../ImageData/ImageData.h"

Game::Game()
	: mDDraw(nullptr)
	, mHwnd(nullptr)
	, mBackImage(nullptr)
	, mMarineImageData(nullptr)
{
}

Game::~Game()
{
	CleanUp();
}

void Game::Initialize(HWND hwnd)
{
	mHwnd = hwnd;

	mDDraw = new DDraw();
	mDDraw->Initialize(hwnd);

	JPGImage* marineImage = new JPGImage();
	if (marineImage->LoadJPGImage("image/Marine.png"))
	{
		DWORD marineColorKey = marineImage->GetPixel(5, 0);

		mMarineImageData = new CImageData();
		char* rawImage = marineImage->GetRawImage();
		int width = marineImage->getWidth();
		int height = marineImage->getHeight();

		mMarineImageData->Create(rawImage, width, height, marineColorKey);
	}

	delete marineImage;
	marineImage = nullptr;

	mBackImage = new JPGImage();
	mBackImage->LoadJPGImage("image/background.png");
}

void Game::Process()
{
	// DrawScene
	mDDraw->BeginDraw();

	if (mBackImage != nullptr)
	{
		int width = mBackImage->getWidth();
		int height = mBackImage->getHeight();
		char* rawImage = (char*)mBackImage->GetRawImage();
		mDDraw->DrawBitmap(0, 0, width, height, rawImage);
		//mDDraw->DrawRect(0, 0, width, height, 0xffffffff);
	}
	else
	{
		mDDraw->Clear();
	}

	mDDraw->DrawImageData(0, 0, mMarineImageData);

	mDDraw->EndDraw();

	mDDraw->OnDraw();
}

void Game::CleanUp()
{
	delete mBackImage;
	mBackImage = nullptr;

	delete mDDraw;
	mDDraw = nullptr;
}

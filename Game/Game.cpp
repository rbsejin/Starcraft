#include <math.h>
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
	, mPrvCounter{}
	, mPrvGameFrameTick(0)
	, TICKS_PER_GAME_FRAME(16.6f)
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

INT_VECTOR2 destnation;
float positionX = 0.f;
float positionY = 0.f;

void Game::OnRButtonDown(int x, int y)
{
	destnation.x = x;
	destnation.y = y;
}

void Game::UpdateUnitPositions()
{
	int speed = 2.f;

	int diffX = destnation.x - positionX;
	int diffY = destnation.y - positionY;

	float distance = sqrtf(diffX * diffX + diffY * diffY);

	// ¹æÇâº¤ÅÍ

	if (distance > 0)
	{
		float directionX = diffX / distance;
		positionX += directionX * speed;

		float directionY = diffY / distance;
		positionY += directionY * speed;
	}
}

void Game::Process()
{
	// update
	UpdateUnitPositions();

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

	mDDraw->DrawImageData((int)positionX, (int)positionY, mMarineImageData);

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

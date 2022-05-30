#pragma once

class DDraw;
class JPGImage;
class CImageData;

class Game
{
public:
	Game();
	~Game();

	void Initialize(HWND hwnd);
	void OnRButtonDown(int x, int y);
	void UpdateUnitPositions();
	void Process();
	void CleanUp();

private:
	DDraw* mDDraw;
	HWND mHwnd;

	JPGImage* mBackImage;
	CImageData* mMarineImageData;
	LARGE_INTEGER mPrvCounter;
	ULONGLONG mPrvGameFrameTick = 0;
	const float TICKS_PER_GAME_FRAME;
};

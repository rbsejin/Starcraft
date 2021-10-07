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
	void Process();
	void CleanUp();

private:
	DDraw* mDDraw;
	HWND mHwnd;

	JPGImage* mBackImage;
	CImageData* mMarineImageData;
};

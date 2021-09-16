#pragma once

class DDraw;

class Game
{
public:
	Game();
	~Game();

	void Initialize(HWND hwnd);
	void Process();
	void CleanUp();

private:
	DDraw* mDDraw = nullptr;
	HWND mHwnd = nullptr;
};

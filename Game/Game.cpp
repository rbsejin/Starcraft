#include <windows.h>

#include "Game.h"
#include "../DDraw/DDraw.h"

Game::Game()
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
}

void Game::Process()
{
	// DrawScene
	mDDraw->BeginDraw();

	mDDraw->Clear();

	// draw
	mDDraw->DrawRect(0, 0, 100, 100, 0xff00ff00);

	mDDraw->EndDraw();

	mDDraw->OnDraw();
}

void Game::CleanUp()
{
	delete mDDraw;
	mDDraw = nullptr;
}

#include "GameWindow.h"



GameWindow::GameWindow()
{
}


GameWindow::~GameWindow()
{
}

const void GameWindow::Init()
{
	WinWindow::Init();

	_testtimer = new Utils::Timer(true);

	return void();
}

const void GameWindow::Frame()
{
	Utils::ConsoleLog::DumpToConsole("FrameDelta: %f", _testtimer->TotalTime());
	return void();
}

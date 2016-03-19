#include "GameWindow.h"



GameWindow::GameWindow()
{
}


GameWindow::~GameWindow()
{
	SAFE_DELETE(_testtimer);
}

const void GameWindow::Init()
{
	WinWindow::Init();

	_testtimer = new Utils::Timer(true);

	return void();
}

const void GameWindow::Frame()
{

	if (_input->Escape())
		_running = false;

	Utils::ConsoleLog::DumpToConsole("FrameDelta: %f", _testtimer->TotalTime());
	return void();
}

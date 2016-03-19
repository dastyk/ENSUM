#include "GameWindow.h"
#include "Safe_Delete.h"


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

	_testtimer = new Core::Timer(true);
	_input->Rebind(Input::Keys::Escape, Input::Keys::A);
	return void();
}

const void GameWindow::Frame()
{

	if (_input->IsKeyDown(Input::Keys::Escape))
		_running = false;

	Utils::ConsoleLog::DumpToConsole("FrameDelta: %f", _testtimer->TotalTime());
	return void();
}

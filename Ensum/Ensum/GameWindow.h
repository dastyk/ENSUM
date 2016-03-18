#ifndef _GAME_WINDOW_H_
#define _GAME_WINDOW_H_
#pragma once
#include "Ensum_core\WindowsWindow.h"
#include "Ensum_utils\ConsoleLog.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_coreD.lib")
#pragma comment(lib, "Ensum_componentsD.lib")
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_core.lib")
#pragma comment(lib, "Ensum_components.lib")
#pragma comment(lib, "Ensum_utils.lib")
#endif
using namespace Ensum;
class GameWindow : public Core::WinWindow
{
public:
	GameWindow();
	~GameWindow();

	const void Init();
private:
	const void Frame();

	Utils::Timer* _testtimer;
};

#endif
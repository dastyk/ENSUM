#include <Windows.h>
#include <windowsx.h>



#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG




#include "Ensum_core\WindowsWindow.h"
#include "Ensum_components\EntityManager.h"
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

#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Memoryleak detection.
	//															  _crtBreakAlloc = 172;
#endif
	Utils::ConsoleLog::CreateInstance();

	Core::Window* w = Core::Window::CreateWin(new GameWindow);


	Components::EntityManager manager;

	Components::Entity ent = manager.Create();


	w->Start();


	Core::Window::DeleteInstance();
	Utils::ConsoleLog::DeleteInstance();

	return 0;
}

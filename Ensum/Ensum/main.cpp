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
#include "Ensum_components\SceneManager.h"
#include "Ensum_components\NullScene.h"
#include "Ensum_utils\ConsoleLog.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_coreD.lib")
#pragma comment(lib, "Ensum_componentsD.lib")
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_inputD.lib")
#else
#pragma comment(lib, "Ensum_core.lib")
#pragma comment(lib, "Ensum_components.lib")
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_input.lib")
#endif

using namespace Ensum;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Memoryleak detection.
	//														  _crtBreakAlloc = 6456;
#endif
	Utils::ConsoleLog::CreateInstance();

	Components::SceneManager sceneManager;
	Core::Window* w = Core::Window::CreateWin(new Core::WinWindow(sceneManager));
	
	sceneManager.CreateScene(new Components::NullScene(sceneManager.GetEntityManager(), w->GetInput()));


	w->Start();


	Core::Window::DeleteInstance();
	Utils::ConsoleLog::DeleteInstance();

	return 0;
}

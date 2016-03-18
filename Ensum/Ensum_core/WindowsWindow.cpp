#include "Ensum_core\WindowsWindow.h"

namespace Ensum
{
	namespace Core
	{


		WinWindow::WinWindow() : Window()
		{

		}


		WinWindow::~WinWindow()
		{
		}
		Window * WinWindow::CreateWin()
		{
			try
			{
				if (!_instance) _instance = new WinWindow();

				if (!_instance) Exception("Could not create the window.");

				_instance->Init();

				Utils::ConsoleLog::DumpToConsole("Window Created.");
			}
			catch (const Utils::Exce& e)
			{
				Window::DeleteInstance();
				e.Print();
				throw e;
			}

			return _instance;
		}
		const void WinWindow::Init()
		{
			_timer = new Utils::Timer();
			if (!_timer) Exception("Could not create the window timer.");


			return void();
		}
	}
}
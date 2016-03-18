#include "Ensum_core\Window.h"

namespace Ensum
{
	namespace Core
	{
		Window* Window::_instance = nullptr;

		Window::Window() : _timer(nullptr)
		{
		}


		Window::~Window()
		{
			SAFE_DELETE(_timer);
		}
		Window * Window::CreateWin(Window * window)
		{
			try
			{
				if (!window) Exception("No window was specified.");
				_instance = window;
				_instance->Init();

				Utils::ConsoleLog::DumpToConsole("Window Created.");
			}
			catch (const Utils::Exce& e)
			{
				Window::DeleteInstance();
				e.Print();
			}

			return _instance;
		}
		Window * Window::GetInstance()
		{
			if (!_instance)Exception("No instance of window in GetInstance");
			return _instance;
		}
		void Window::DeleteInstance()
		{
			try
			{
				SAFE_DELETE(_instance);
			}
			catch (const Utils::Exce& e)
			{
				e.Print();

			}
			
		}
		const void Window::BindRenderer(void * renderer)
		{
			return void();
		}
	}
}
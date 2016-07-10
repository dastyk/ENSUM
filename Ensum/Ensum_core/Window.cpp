#include "Ensum_core\Window.h"
#include "Safe_Delete.h"
#include "Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"

namespace Ensum
{
	namespace Core
	{
		Window* Window::_instance = nullptr;

		Window::Window(Components::SceneManager& sceneManager) : _timer(nullptr),_sceneManager(sceneManager), _running(false), _wndCaption(L"Ensum")
		{
			Utils::Options::Subscribe(Delegate<const void()>::Make<Window, &Window::_Resize>(this));
		}



		Window::~Window()
		{
			Utils::Options::UnSubscribe(Delegate<const void()>::Make<Window, &Window::_Resize>(this));
			SAFE_DELETE(_input);
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
			catch (const Exce& e)
			{
				e.Print();
				Window::DeleteInstance();
				throw e;
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
			catch (const Exce& e)
			{
				e.Print();
				throw e;
			}
			
		}

		const void Window::Start()
		{

			_running = true;

			_timer->Start();
			while (_running)
			{
				if (!_paused)
				{
					_input->Frame();

					_MessageHandling();

					FrameStart();
					//Utils::ConsoleLog::DumpToConsole("Delta: %.5f", _timer->Delta());
					// Do the frame processing.
					_Frame();
				}
				else
					_MessageHandling();
			}

		}

		const void Window::Pause(bool paused)
		{
			_paused = paused;
			paused ? _timer->Stop() : _timer->Start();
			return void();
		}

		Input::Input * Window::GetInput()
		{
			return _input;
		}
	}
}
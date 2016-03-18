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
		Window * Window::GetInstance()
		{
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
				throw e;
			}
			
		}
		const void Window::Start()
		{
			return void();
		}
		const void Window::BindRenderer(void * renderer)
		{
			return void();
		}
	}
}
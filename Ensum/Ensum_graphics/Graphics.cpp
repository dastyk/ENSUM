#include "Ensum_graphics\Graphics.h"
#include "Exception.h"
#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Options.h"
#include "Safe_Delete.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif
namespace Ensum
{
	namespace Graphics
	{
		Graphics* Graphics::_instance = nullptr;

		DWORD WINAPI ThreadEntryPoint(LPVOID lpParam)
		{
			Graphics* g = (Graphics*)lpParam;

				g->Start();

			return 1;
		}

		Graphics::Graphics(const string& type) : _shutdown(false), _threadHandle(nullptr), _type(type)
		{
			_optionsChangeMutex = CreateMutex(NULL, FALSE, NULL);
			Utils::Options::Subscribe(Delegate<const void()>::Make<Graphics, &Graphics::_OnOptionsChange>(this));

			_fullscreen = Utils::Options::GetBooleanOption("Screen", "Fullscreen", false);
			_width =(unsigned) Utils::Options::GetIntegerOption("Screen", "Width", 800);
			_height = (unsigned)Utils::Options::GetIntegerOption("Screen", "Height", 640);
			_vsync = Utils::Options::GetBooleanOption("Graphics", "Vsync", false);
		}


		Graphics::~Graphics()
		{
			Utils::Options::UnSubscribe(Delegate<const void()>::Make<Graphics, &Graphics::_OnOptionsChange>(this));
			CloseHandle(_optionsChangeMutex);
		}
		const void Graphics::CreateInstance(Graphics * g)
		{
			if (!g) Exception("No instance specified.");
			try
			{
				if (!_instance)
				{

					_instance = g;

					_instance->OnCreateDevice();

					_instance->StartThread();

					Utils::ConsoleLog::DumpToConsole("Graphics instance created. Type: %s", _instance->GetGraphicsType().c_str());
				}
			}
			catch (const Exce& e)
			{
				e.Print();
				Graphics::DeleteInstance();
			}
		}
		const void Graphics::DeleteInstance()
		{
			if (_instance)
			{
				Utils::ConsoleLog::DumpToConsole("Deleting the graphics instance.");
				_instance->StopThread();
				_instance->OnDestroyDevice();
				SAFE_DELETE(_instance);
			}
		}
		const void Graphics::Start()
		{
			try
			{
				while (!_shutdown)
				{
					WaitForSingleObject(_optionsChangeMutex, INFINITE);
			
					_BeginFrame();
					_Frame();
					_EndFrame();

					ReleaseMutex(_optionsChangeMutex);
				}
			}
			catch (const Exce& e)
			{
				e.Print();
				OnDestroyDevice();
			}
		}
		const string & Graphics::GetGraphicsType()
		{
			return _type;
		}
		const void Graphics::StartThread()
		{
			_threadHandle = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				ThreadEntryPoint,       // thread function name
				this,          // argument to thread function 
				0,                      // use default creation flags 
				&_threadAddress);   // returns the thread identifier 
		}
		const void Graphics::StopThread()
		{
			_shutdown = true;
			WaitForSingleObject(_threadHandle, INFINITE);
			CloseHandle(_threadHandle);
		}
	}
}
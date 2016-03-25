#include "Ensum_graphics\D3D11.h"
#include "Safe_Delete.h"
#include "Exception.h"
#include "Ensum_utils\ConsoleLog.h"


#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif
namespace Ensum
{
	namespace Graphics
	{

		DWORD WINAPI ThreadEntryPoint(LPVOID lpParam)
		{
			Graphics* g = (Graphics*)lpParam;

			g->Start();

			return 1;
		}

		D3D11::D3D11(HWND hwnd) : Graphics()
		{

		
		}


		D3D11::~D3D11()
		{
		
		}
		void D3D11::CreateInstance(HWND hwnd)
		{
			try 
			{
				if (!_instance)
				{
					
					_instance = new D3D11(hwnd);
					_instance->Init();

					Utils::ConsoleLog::DumpToConsole("Creating graphics instance. Type: DirectX11");
				}
			}
			catch (const Exce& e)
			{
				e.Print();
				if(_instance) _instance->Shutdown();
				SAFE_DELETE(_instance);
			}
		}
		void D3D11::DeleteInstance()
		{
			if (!_instance) Exception("No instance of the grahpics found.");

			Utils::ConsoleLog::DumpToConsole("Deleting the graphics instance.");
			_instance->Shutdown();
			SAFE_DELETE(_instance);

		}
		const void D3D11::Init()
		{
			_threadHandle = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				ThreadEntryPoint,       // thread function name
				this,          // argument to thread function 
				0,                      // use default creation flags 
				&_threadAddress);   // returns the thread identifier 
		}
		const void D3D11::Shutdown()
		{
			_shutdown = true;
			WaitForSingleObject(_threadHandle, INFINITE);
			CloseHandle(_threadHandle);
		}
		const void D3D11::_Frame()
		{
			
			return void();
		}
	}
}
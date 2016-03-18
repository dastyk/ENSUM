#include "Ensum_utils\ConsoleLog.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\Safe_Delete.h"
namespace Ensum
{
	namespace Utils
	{

		DWORD WINAPI ThreadEntryPoint(LPVOID lpParam)
		{
			ConsoleLog* p = (ConsoleLog*)lpParam;


			p->Write();

			return 1;
		}
		ConsoleLog* ConsoleLog::_instance = nullptr;
	
		ConsoleLog::ConsoleLog() : _toPrint(nullptr)
		{
			if (!AllocConsole()) Exception("Failed to alloc console.");
			freopen("conin$", "r", stdin);
			freopen("conout$", "w", stdout);
			freopen("conout$", "w", stderr);

			printf("<----||Console Initialized||---->\n\n");


			_toPrint = new std::deque<WriteData>;
			if (!_toPrint) Exception("Could not create print que.");


			_writeMutex = CreateMutex(NULL, FALSE, NULL);

			_shutdown = false;

			_threadHandle = CreateThread(
				NULL,                   // default security attributes
				0,                      // use default stack size  
				ThreadEntryPoint,       // thread function name
				this,          // argument to thread function 
				0,                      // use default creation flags 
				&_threadAddress);   // returns the thread identifier 

			SetThreadPriority(_threadHandle, THREAD_MODE_BACKGROUND_BEGIN);

		}



		ConsoleLog::~ConsoleLog()
		{
			_shutdown = true;
			WaitForSingleObject(_threadHandle, INFINITE);
			CloseHandle(_threadHandle);
			CloseHandle(_writeMutex);
			SAFE_DELETE(_toPrint);
			FreeConsole();
		}
		const void ConsoleLog::CreateInstance()
		{
			try
			{
				if (!_instance) _instance = new ConsoleLog();

				if (!_instance) Exception("Could not create consolelog.");
			}
			catch (const Utils::Exce& e)
			{
				ConsoleLog::DeleteInstance();
				e.Print();
				throw e;
			}
			


		}
		const void ConsoleLog::DeleteInstance()
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
		const void ConsoleLog::DumpToConsole(const char* message, ...)
		{
			if (_instance)
			{
				va_list args;
				va_start(args, message);
				_instance->AddToQue(std::move(WriteData(message, args)));

			}
		}
		const void ConsoleLog::AddToQue(const WriteData& data)
		{
			WaitForSingleObject(_writeMutex, INFINITE);
			_toPrint->push_back(data);
			ReleaseMutex(_writeMutex);
		}
		const void ConsoleLog::Write()
		{
			

			while (!_shutdown)
			{
				if (_toPrint->size())
				{
					WaitForSingleObject(_writeMutex, INFINITE);

					WriteData& d = _toPrint->front();
					int len = vsnprintf(NULL, 0, d.msg, d.args) + 1;
					// instance found, add message to que.
					char* buffer = new char[len];

					vsprintf(buffer, d.msg, d.args);
					va_end(d.args);

					printf(buffer);
					printf("\n");
					_toPrint->pop_front();
					ReleaseMutex(_writeMutex);
				}
			}

			return void();
		}
	}
}
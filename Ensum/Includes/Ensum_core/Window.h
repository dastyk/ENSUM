#ifndef _WINDOWS_H_
#define _WINDOWS_H_
#pragma once

#include "dll_export.h"

#include "Ensum_utils\Event.h"
#include "Ensum_utils\Timer.h"
#include "Ensum_utils\Exception.h"
#include "Ensum_utils\Safe_Delete.h"
#include "Ensum_utils\ConsoleLog.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif

namespace Ensum
{
	namespace Core
	{


		ENSUM_CORE_TEMPLATE template class ENSUM_CORE_EXPORT Utils::Event<const void()>;

		// Fully abstract class for interfacting with the actual window.
		class ENSUM_CORE_EXPORT Window
		{
		protected:
			Window();

			static Window* _instance;
			Utils::Timer* _timer;


			virtual const void Frame() = 0;
		public:
			virtual ~Window();

			static Window* CreateWin(Window * window);
			static Window* GetInstance();
			static void DeleteInstance();

			virtual const void Init() = 0;
			virtual const void Start() = 0;

			Utils::Event<const void()> FrameStart;
		};
	}
}


#endif
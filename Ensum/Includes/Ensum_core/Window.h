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

		public:
			virtual ~Window();

			//static Window* CreateWindow(); A somewhat abstract thing
			static Window* GetInstance();
			static void DeleteInstance();

			virtual const void Init() = 0;
			virtual const void Start();


			const void BindRenderer(void* renderer);

			Utils::Event<const void()> FrameStart;
		};
	}
}


#endif
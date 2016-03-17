#ifndef _WINDOWS_H_
#define _WINDOWS_H_
#pragma once

#include "dll_export.h"

#include "Ensum_utils\Event.h"

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

			const static Window* _instance;
		public:
			virtual ~Window();

			static Window* GetInstance();


			Utils::Event<const void()> FrameStart;
		};
	}
}


#endif
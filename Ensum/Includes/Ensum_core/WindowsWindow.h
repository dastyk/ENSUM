#ifndef _WINDOWS_WINDOW_H_
#define _WINDOWS_WINDOW_H_
#pragma once
#include "Ensum_core\Window.h"
namespace Ensum
{
	namespace Core
	{

		class ENSUM_CORE_EXPORT WinWindow :
			public Window
		{
		public:
			WinWindow();
			~WinWindow();

			static Window* CreateWin();
			const void Init();
		};

	}
}
#endif
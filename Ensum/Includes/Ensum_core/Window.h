#ifndef _WINDOWS_H_
#define _WINDOWS_H_
#pragma once

#include "dll_export.h"

#include "Event.h"

#include "Ensum_core\Timer.h"
#include "Ensum_input\Input.h"
#include "Event.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_inputD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_input.lib")
#endif

namespace Ensum
{
	namespace Core
	{


		ENSUM_CORE_TEMPLATE template class ENSUM_CORE_EXPORT Event<const void()>;
		/** Fully abstract class for interfacting with the actual window.
		*
		*/
		class ENSUM_CORE_EXPORT Window
		{
		protected:
			Window(Delegate<const void()>& sceneManagerFrame);

			/** The frame function.
			* Put the gamelogic here.
			*/
			virtual const void _Frame() = 0;
			/** Handle messages.
			*
			*/
			virtual const void _MessageHandling() = 0;
			/** Called when options have changed.
			*
			*/
			virtual const void _Resize() = 0;
		public:
			virtual ~Window();

			/** Saves the given window and initializes it.
			*
			*/
			static Window* CreateWin(Window * window);
			/** Returns a pointer to the window.
			*
			*/
			static Window* GetInstance();
			/** Deletes the window.
			* This also deletes all members
			*/
			static void DeleteInstance();

			/** Initialization for the window.
			*
			*/
			virtual const void Init() = 0;
			/** Start the Game loop.
			*
			*/
			virtual const void Start();
			/** Function for pausing and unpausing app.
			*
			*/
			virtual const void Pause(bool paused);
			/** Returns a pointer to the input.
			*
			*/
			Input::Input* GetInput();
		public:
			Event<const void()> FrameStart;

		protected:
			bool _running;
			bool _paused;
			LPWSTR _wndCaption;

			static Window* _instance;

			Timer* _timer;

			Input::Input* _input;

			Event<const void()> _sceneManagerFrame;
		};
	}
}


#endif
#ifndef _WINDOWS_H_
#define _WINDOWS_H_
#pragma once

#include "dll_export.h"

#include "Event.h"

#include "Ensum_core\Timer.h"
#include "Ensum_input\Input.h"
#include "Ensum_components\SceneManager.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_inputD.lib")
#pragma comment(lib, "Ensum_componentsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_input.lib")
#pragma comment(lib, "Ensum_components.lib")
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
			Window(Components::SceneManager& sceneManager);

			/** The frame function.
			* Put the gamelogic here.
			*/
			virtual const void Frame() = 0;
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
			/** Start the message loop.
			*
			*/
			virtual const void Start() = 0;

			/** Returns a pointer to the input.
			*
			*/
			Input::Input* GetInput();
		public:
			Event<const void()> FrameStart;

		protected:

			bool _running;			
			LPWSTR _wndCaption;

			static Window* _instance;

			Timer* _timer;

			Input::Input* _input;

			Components::SceneManager& _sceneManager;
		};
	}
}


#endif
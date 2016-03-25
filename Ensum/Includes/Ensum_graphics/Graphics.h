#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#pragma once
#include "dll_export.h"
#include <thread>
#include <process.h>
#include <Windows.h>
#include "ensumstring.h"

namespace Ensum
{
	namespace Graphics

	{	/** The graphics interface.
		* When an instance of the graphics is created it will connect with a sceneprovider
		* which in turn provides the graphics with render jobs.
		* The graphics is multithreaded and will only interact with the rest when gathering jobs.
		*/
		class ENSUM_GRAPHICS_EXPORT Graphics
		{
		protected:
			Graphics(const string& type);
	

			static Graphics* _instance;

			HANDLE _optionsChangeMutex;
			HANDLE _threadHandle;
			DWORD _threadAddress;

			bool _shutdown;
			bool _fullscreen;
			unsigned _width;
			unsigned _height;
			bool _vsync;
			string _type;
		public:
			virtual ~Graphics();

			/** Takes the pointer and initializes the graphics.
			* This function calls the OnCreateDevice then the StartThread function.
			*/
			static const void CreateInstance(Graphics* g);
			/** Deletes the graphics instance.
			*
			*/
			static const void DeleteInstance();

			/** Enters the render loop.
			* This should only be called inside the thread.
			*/
			virtual const void Start();

			/** Implements the device creation.
			*
			*/
			virtual const void OnCreateDevice() = 0;
			/** Implements the device destruction.
			*
			*/
			virtual const void OnDestroyDevice() = 0;
			/** Returns what graphics type the current instance is.
			*
			*/
			virtual const string& GetGraphicsType();
			/** Creates the thread and executes it.
			*
			*/
			virtual const void StartThread();
			/** Stops the thread.
			*
			*/
			virtual const void StopThread();
		protected:
			/** Called when the options have changed.
			*
			*/
			virtual const void _OnOptionsChange() = 0;

			/** Begin the rendering frame.
			*
			*/
			virtual const void _BeginFrame(void) = 0;
			/** End the rendering frame.
			*
			*/
			virtual const void _EndFrame(void) = 0;
			/** What happens between BeginFrame and EndFrame.
			*
			*/
			virtual const void _Frame() = 0;

		};
	}
}
#endif
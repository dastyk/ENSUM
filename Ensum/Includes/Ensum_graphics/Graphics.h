#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_
#pragma once
#include "dll_export.h"
#include <thread>
#include <process.h>
#include <Windows.h>

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
			Graphics();
	

			static Graphics* _instance;


			HANDLE _threadHandle;
			DWORD _threadAddress;

			bool _shutdown;
		public:
			virtual ~Graphics();

			virtual const void Start();
			virtual const void Init() = 0;
			virtual const void Shutdown() = 0;
		protected:
			virtual const void _Frame() = 0;
		
		};
	}
}
#endif
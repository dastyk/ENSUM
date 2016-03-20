#ifndef _TIMER_H_
#define _TIMER_H_
#pragma once
#include <time.h>
#include "dll_export.h"



namespace Ensum
{
	namespace Core
	{
		/** Keeps track of time.
		* 
		*/
		class ENSUM_CORE_EXPORT Timer
		{
		public:
			Timer(bool startImmediately = false);
			virtual ~Timer();

			/** Reset all the timer values.
			* Specify startImmediately if the timer should start immediately after being reset.
			*/
			const void Reset(bool startImmediately = false);
			/** Start the timer.
			* 
			*/
			const void Start();
			/** Returns the timers total runtime, then stops the timer.
			* 
			*/
			const float GetTotalTimeAndStop();
			/** Stops the timer.
			* 
			*/
			const void Stop();
			/** Returns the timers total runtime.
			*
			*/
			const float TotalTime();
			/** Returns the time between last frame and this frame (delta time).
			*
			*/
			const float Delta();
			/** The tick of the timer.
			* If a window is created when the timer is created the timer will automaticly subscribe to the Framestart event of the window.
			*/
			const void Tick();
		private:



			clock_t _startTime;

			clock_t _prevTime;
			clock_t _currTime;

			clock_t _stopTime;
			clock_t _pausedTime;

			double _deltaTime;
			double _secondsPerCount;
			bool _stopped;
		};



	}
}
#endif
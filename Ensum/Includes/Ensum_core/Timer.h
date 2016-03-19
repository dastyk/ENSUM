#ifndef _TIMER_H_
#define _TIMER_H_
#pragma once
#include <time.h>
#include "dll_export.h"



namespace Ensum
{
	namespace Core
	{
		// Fully abstract class for interfacting with the actual timer.
		class ENSUM_CORE_EXPORT Timer
		{
		public:
			Timer(bool startImmediately = false);
			virtual ~Timer();

			const void Reset(bool startImmediately = false);
			const void Start();
			const float GetTotalTimeAndStop();
			const void Stop();
			const float TotalTime();
			const float Delta();

		private:
			const void Tick();


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
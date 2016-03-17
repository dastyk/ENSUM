#ifndef _TIMER_H_
#define _TIMER_H_
#pragma once
#include <time.h>
#include "dll_export.h"

#include "Window.h"

namespace Ensum
{
	namespace Utils
	{
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
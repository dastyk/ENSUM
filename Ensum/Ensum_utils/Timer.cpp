#include "Ensum_utils\Timer.h"

#include "Ensum_core\Window.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_coreD.lib")
#else
#pragma comment(lib, "Ensum_core.lib")
#endif

namespace Ensum
{
	namespace Utils
	{
		Timer::Timer(bool startImmediately) :
			_startTime(startImmediately ? clock() : 0),
			_deltaTime(0.0), 
			_prevTime(startImmediately ? _startTime : 0),
			_currTime(startImmediately ? _startTime : 0),
			_stopTime(0),
			_pausedTime(0),
			_secondsPerCount(static_cast<double>(1.0f / CLOCKS_PER_SEC)),
			_stopped(!startImmediately)
		{
			auto w = Core::Window::GetInstance();
			if (w)
				Core::Window::GetInstance()->FrameStart += Delegate<const void()>::Make<Timer, &Timer::Tick>(this);
		}


		Timer::~Timer()
		{
			auto w = Core::Window::GetInstance();
			if (w)
				Core::Window::GetInstance()->FrameStart -= Delegate<const void()>::Make<Timer, &Timer::Tick>(this);
		}
		const void Timer::Reset(bool startImmediately)
		{
			_startTime = startImmediately ? clock() : 0;
			_deltaTime = 0.0;
			_prevTime = startImmediately ? _startTime : 0;
			_currTime = startImmediately ? _startTime : 0; 
			_stopTime = 0;
			_pausedTime = 0;
			_secondsPerCount = static_cast<double>(1.0f / CLOCKS_PER_SEC);
			_stopped = !startImmediately;
			return void();
		}
		const void Timer::Start()
		{

			_startTime = clock();

			if (_stopped)
			{
				_pausedTime += _startTime - _stopTime;

				_prevTime = _startTime;

				_stopTime = 0;
				_stopped = false;
			}

			return void();
		}
		const float Timer::GetTotalTimeAndStop()
		{
			if (_stopped)
			{
				return static_cast<float>(((_stopTime - _pausedTime) - _startTime)*_secondsPerCount);
			}
			else
			{
				_stopTime = clock();
				_stopped = true;
				return static_cast<float>(((_currTime - _pausedTime) - _startTime)*_secondsPerCount);
			}
		}
		const void Timer::Stop()
		{
			if (!_stopped)
			{
				_stopTime = clock();
				_stopped = true;
			}
			return void();
		}
		const float Timer::TotalTime()
		{
			if (_stopped)
			{
				return static_cast<float>(((_stopTime - _pausedTime) - _startTime)*_secondsPerCount);
			}
			else
			{
				return static_cast<float>(((_currTime - _pausedTime) - _startTime)*_secondsPerCount);
			}
		}
		const float Timer::Delta()
		{
			return static_cast<float>(_deltaTime);
		}
		const void Timer::Tick()
		{
			if (_stopped)
			{
				_deltaTime = 0.0;
				return;
			}

			// Get time this frame
			_currTime = clock();

			// Time diff between this and previous frame.
			_deltaTime = (_currTime - _prevTime)*_secondsPerCount;

			// Prepare for next frame
			_prevTime = _currTime;

			if (_deltaTime < 0.0)
				_deltaTime = 0.0;

			return void();
		}
	}
}
#include "pch.h"
#include "Timer.h"


Timer::Timer()
{
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondPerCount = 1.0 / (double)countsPerSec;
}


Timer::~Timer()
{

}

float Timer::deltaTime() const
{
	return (float)m_DeltaTime;
}

float Timer::totalTime() const
{
	if (m_Stopped)
	{
		return (float)(((m_StopTime - m_PauseTime) - m_BaseTime)*m_SecondPerCount);
	}
	else
	{
		return (float)(((m_CurTime - m_PauseTime) - m_BaseTime)* m_SecondPerCount);
	}
}

void Timer::reset()
{
	__int64 curTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	m_BaseTime = curTime;
	m_PrevTime = curTime;
	m_StopTime = 0;
	m_Stopped = false;
}

void Timer::start()
{
	__int64 startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_Stopped)
	{
		m_PauseTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		
		m_StopTime = 0;
		m_Stopped = false;
	}

}

void Timer::stop()
{
	if (!m_Stopped)
	{
		__int64 curTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		m_StopTime = curTime;
		m_Stopped = true;
	}
}

void Timer::tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	__int64 curTime = 0;
	
	// 몇번의 카운트가 있었는지 알려주는 함수
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	m_CurTime = curTime;

	// 몇번의 카운트가 이전 시간에 비해서 생겼는지 계산한다음
	// 한번의 카운트가 몇초가 걸리는지 곱해줌
	m_DeltaTime = (m_CurTime - m_PrevTime)*m_SecondPerCount;

	m_PrevTime = m_CurTime;

	if (m_DeltaTime < 0.0f)
	{
		m_DeltaTime = 0.0;
	}


}

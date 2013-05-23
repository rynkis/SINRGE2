/*
** SINRGE2
** Copyright (C) 2013 Shy07
** Gernischt@gmail.com
**
** SINRGE2 Class Timer
*/
#include "sin_timer.h"
#include <windows.h>

namespace
{
	static int GetTimeofDay(struct timeval * val, struct timezone * zone)
	{
		if (val)
		{
			LARGE_INTEGER liTime, liFreq;
			QueryPerformanceFrequency( &liFreq );
			QueryPerformanceCounter( &liTime );
			val->tv_sec     = (long)( liTime.QuadPart / liFreq.QuadPart );
			val->tv_usec    = (long)( liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0 );
		}
		return 0;
	}

	static unsigned int SysGetTick()
	{
		static struct timeval	start;
		static unsigned char	uninited = 1;
		unsigned int			ticks;
		struct timeval			now;

		if (uninited)
		{
			uninited = 0;
			GetTimeofDay(&start, NULL);
		}

		GetTimeofDay(&now, NULL);
		ticks = (now.tv_sec-start.tv_sec) * 1000 + (now.tv_usec-start.tv_usec) / 1000;
		return ticks;
	}
}

CTimer::CTimer()
	: m_started(false)
	, m_paused(false)
	, m_startTicks(0)
	, m_pausedTicks(0)
{
}

unsigned int CTimer::GetTicks()
{
	if (m_started)
	{
		if (m_paused)
			return m_pausedTicks;
		else
			return SysGetTick() - m_startTicks;
	}
   	return 0;
}

void CTimer::Start()
{
	m_started = true;
	m_paused = false;
	m_startTicks = SysGetTick();
}

void CTimer::Stop()
{
	m_started = false;
	m_paused = false;
}

void CTimer::Pause()
{
	if (m_started && !m_paused)
	{
		m_paused = true;
		m_pausedTicks = SysGetTick() - m_startTicks;
	}
}

void CTimer::Resume()
{
	if(m_paused)
	{
		m_paused = false;
		m_startTicks = SysGetTick() - m_pausedTicks;
		m_pausedTicks = 0;
	}
}
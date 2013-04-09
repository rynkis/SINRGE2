#include "nge_timer.h"
#include <stdlib.h>
#include <string.h>

static uint32_t TM_get_ticks(nge_timer* timer);
static uint32_t TM_is_started(nge_timer* timer);
static uint32_t TM_is_paused(nge_timer* timer);
static void TM_start(nge_timer* timer);
static void TM_stop(nge_timer* timer);
static void TM_pause(nge_timer* timer);
static void TM_unpause(nge_timer* timer);

nge_timer* nge_timer_create()
{
	nge_timer* timer = (nge_timer*)malloc(sizeof(nge_timer));
	memset(timer,0,sizeof(nge_timer));
	timer->get_ticks = TM_get_ticks;
	timer->is_started = TM_is_started;
	timer->is_paused = TM_is_paused;
	timer->start = TM_start;
	timer->stop = TM_stop;
	timer->pause = TM_pause;
	timer->unpause = TM_unpause;
	return timer;
}

void nge_timer_free(nge_timer* timer)
{
	SAFE_FREE(timer);
}

static uint32_t TM_get_ticks(nge_timer* timer)
{
	//If the timer is running
	if( timer->started == 1 )
	{
		//If the timer is paused
		if( timer->paused == 1 )
			//Return the number of ticks when the the timer was paused
			return timer->pausedTicks;
		else
			//Return the current time minus the start time
			return nge_get_tick() - timer->startTicks;
	}
   	return 0;
}
static uint32_t TM_is_started(nge_timer* timer)
{
	return timer->started;
}
static uint32_t TM_is_paused(nge_timer* timer)
{
	return timer->paused;
}
static void TM_start(nge_timer* timer)
{
	timer->started = 1;
	timer->paused = 0;
	timer->startTicks = nge_get_tick();
}
static void TM_stop(nge_timer* timer)
{
   timer->started = 0;
   timer->paused = 0;
}
static void TM_pause(nge_timer* timer)
{
	//If the timer is running and isn't already paused
	if( ( timer->started == 1 ) && ( timer->paused == 0 ) )
	{
	//Pause the timer
	timer->paused = 1;
	//Calculate the paused ticks
	timer->pausedTicks = nge_get_tick() - timer->startTicks;
	}
}
static void TM_unpause(nge_timer* timer)
{
	//If the timer is paused
	if( timer->paused == 1 )
	{
	//Unpause the timer
	timer->paused = 0;
	//Reset the starting ticks
	timer->startTicks = nge_get_tick() - timer->pausedTicks;
	//Reset the paused ticks
	timer->pausedTicks = 0;
	}
}

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
/*
 * Structure used in select() call, taken from the BSD file sys/time.h.
 */
//struct timeval {
//        long    tv_sec;         /* seconds */
//        long    tv_usec;        /* and microseconds */
//};

int getTimeofday(struct timeval * val, struct timezone * zone)
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

uint32_t nge_get_tick()
{
	static struct timeval start;
	static uint8_t uninited = 1;
	uint32_t ticks;
	struct timeval now;

	if (uninited)
	{
		uninited = 0;
		getTimeofday(&start, NULL);
	}

	getTimeofday(&now, NULL);
	ticks = (now.tv_sec-start.tv_sec) * 1000 + (now.tv_usec-start.tv_usec) / 1000;
	return ticks;
}

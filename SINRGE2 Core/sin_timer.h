#ifndef __SIN_TIMER_H__
#define __SIN_TIMER_H__

class CTimer
{
public:
	CTimer();
	~CTimer() {}
	
	unsigned int			GetTicks();

	void					Start();
	void					Stop();
	void					Pause();
	void					Resume();

	inline bool				IsStarted() const { return m_started; }
	inline bool				IsPaused() const { return m_paused; }

private:
	bool					m_started;
	bool					m_paused;
	unsigned int			m_startTicks;
	unsigned int			m_pausedTicks;
};

#endif // __SIN_TIMER_H__

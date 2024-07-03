#include "EngineTime.h"

void Time::Tick()
{
	clock::duration duration = clock::now() - m_startTime; // Subtract start time from the present to know how long has passed since the start
	m_time = duration.count() / (float) clock::duration::period::den; // Figure it out in seconds, period::den is how many duration in a second

	duration = clock::now() - m_frameTime; // Get the delta time
	m_deltaTime = duration.count() / (float)clock::duration::period::den;

	m_frameTime = clock::now();
}

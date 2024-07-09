#pragma once
#include <chrono> // More precise time measurement than milliseconds

class Time
{
private:
	using clock = std::chrono::high_resolution_clock; // so I can use "clock" instead of this

	float m_time{ 0 }; // Time elapsed since program started
	float m_deltaTime{ 0 }; // Time elapsed since last frame

	clock::time_point m_startTime; // Temporal coordinate
	clock::time_point m_frameTime; 

public:
	Time() : m_startTime{ clock::now() }, m_frameTime{ clock::now() } {}

	void Tick();
	void Reset() { m_startTime = clock::now(); } // Reset the time to NOW

	float GetTime() const { return m_time; }
	float GetDeltaTime() const { return m_deltaTime; }
};
#include "Time.h"

#include <SDL.h>

float Time::m_Time = 0.0f;
float Time::m_LastFrameTime = 0.0f;
float Time::m_TimeMultiplier = 1.0f;

void Time::Init(float time)
{
	m_Time = time;
	m_LastFrameTime = 0.0f;
}

void Time::Tick()
{
	float time = SDL_GetTicks();

	m_Time = time - m_LastFrameTime;

	m_LastFrameTime = time;
}
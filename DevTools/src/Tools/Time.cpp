#include "Time.h"

#include <SDL.h>

float Time::m_Time = 0.0f;
float Time::m_LastFrameTime = 0.0f;
float Time::m_TimeMultiplier = 1.0f;

void Time::Init()
{
	m_Time = (float)SDL_GetTicks();
	m_LastFrameTime = (float)SDL_GetTicks();
}

void Time::Tick()
{
	float time = (float)SDL_GetTicks();

	m_Time = MillisecondsToSeconds(time - m_LastFrameTime);

	m_LastFrameTime = time;
}

float Time::TotalTime()
{
	return MillisecondsToSeconds((float)SDL_GetTicks());
}

float Time::FPS()
{
	static float timeElapsed = 0.0f;
	static int frames = 0;
	static float FPS = 0.0f;
	static float interval = 0.1f;

	timeElapsed += Time::DeltaTime();
	frames++;

	if (timeElapsed >= interval)
	{
		FPS = (float)frames / timeElapsed;
		if (timeElapsed >= 1.0f)
		{
			timeElapsed = 0.0F;
			frames = 0;
		}
	}

	return FPS;
}
#pragma once

class Time
{
public:
	static void Init();
	static void Tick();

	inline static void SetTimeMultiplier(float multiplier) { m_TimeMultiplier = multiplier; }
	inline static float GetTimeMultiplier() { return m_TimeMultiplier; }

	inline static float SecondsToMilliseconds(float value) { return value * 1000.0f; }
	inline static float MillisecondsToSeconds(float value) { return value / 1000.0f; }

	inline static float DeltaTime() { return m_Time; }
	static float TotalTime();
	static float FPS();
private:
	static float m_Time;
	static float m_LastFrameTime;

	static float m_TimeMultiplier;
};
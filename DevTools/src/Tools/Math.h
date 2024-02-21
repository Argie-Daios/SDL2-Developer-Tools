#pragma once

#include <glm/glm.hpp>

namespace Math
{
	constexpr float Clamp(float x, float start = 0.0f, float end = 1.0f)
	{
		if (x < start) return start;
		if (x > end) return end;

		return x;
	}

	constexpr float Lerp(float start, float end, float t)
	{
		if (t >= 1)
		{
			return end;
		}

		if (t == 0)
		{
			return start;
		}

		return start + (end - start) * t;
	}

	constexpr glm::vec2 Lerp(glm::vec2 start, glm::vec2 end, float t)
	{
		return start + (end - start) * t;
	}

	constexpr glm::vec3 Lerp(glm::vec3 start, glm::vec3 end, float t)
	{
		return start + (end - start) * t;
	}

	constexpr float SmoothStep(float start, float end, float t)
	{
		t = Clamp(t);

		return Lerp(start, end, t * t * (3.0f - 2.0f * t));
	}

	constexpr glm::vec2 SmoothStep(glm::vec2 start, glm::vec2 end, float t)
	{
		t = Clamp(t);

		return Lerp(start, end, t * t * (3.0f - 2.0f * t));
	}

	constexpr glm::vec3 SmoothStep(glm::vec3 start, glm::vec3 end, float t)
	{
		t = Clamp(t);

		return Lerp(start, end, t * t * (3.0f - 2.0f * t));
	}
}
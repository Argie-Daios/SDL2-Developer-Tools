#pragma once

#include <glm/glm.hpp>

namespace Math
{
	glm::vec2 Lerp(glm::vec2 start, glm::vec2 end, float t)
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

	glm::vec3 Lerp(glm::vec3 start, glm::vec3 end, float t)
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
}
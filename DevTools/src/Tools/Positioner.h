#pragma once

#include <glm/glm.hpp>

enum class PositionType
{
	LEFT_UP,
	UP,
	RIGHT_UP,
	LEFT,
	CENTER,
	RIGHT,
	LEFT_DOWN,
	DOWN,
	RIGHT_DOWN
};

class Positioner
{
public:
	static glm::vec2 PositionItemInRect(glm::vec2 rectXY, glm::vec2 rectWH, glm::vec2 itemXY, glm::vec2 itemWH, PositionType positioning = PositionType::CENTER);
};
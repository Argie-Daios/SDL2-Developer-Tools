#pragma once

#include "ECS/Entity.h"

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
	static void PositionItemInWindow(Entity entity, PositionType positioning = PositionType::CENTER);
	static void PositionCameraToEntity(Entity entity, PositionType positioning = PositionType::CENTER);
};
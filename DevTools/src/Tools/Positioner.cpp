#include "Positioner.h"

glm::vec2 Positioner::PositionItemInRect(glm::vec2 rectXY, glm::vec2 rectWH, glm::vec2 itemXY, glm::vec2 itemWH,
	PositionType positioning)
{
	switch (positioning)
	{
	case PositionType::LEFT_UP:
	{
		itemXY = rectXY;
		break;
	}
	case PositionType::UP:
	{
		itemXY.x = (2.0f * rectXY.x + rectWH.x - itemWH.x) / 2.0f;
		itemXY.y = rectXY.y;
		break;
	}
	case PositionType::RIGHT_UP:
	{
		itemXY.x = rectXY.x + rectWH.x - itemWH.x;
		itemXY.y = rectXY.y;
		break;
	}
	case PositionType::LEFT:
	{
		itemXY.x = rectXY.x;
		itemXY.y = (2.0f * rectXY.y + rectWH.y - itemWH.y) / 2.0f;
		break;
	}
	case PositionType::CENTER:
	{
		itemXY = (2.0f * rectXY + rectWH - itemWH) / 2.0f;
		break;
	}
	case PositionType::RIGHT:
	{
		itemXY.x = rectXY.x + rectWH.x - itemWH.x;
		itemXY.y = (2.0f * rectXY.y + rectWH.y - itemWH.y) / 2.0f;
		break;
	}
	case PositionType::LEFT_DOWN:
	{
		itemXY.x = rectXY.x;
		itemXY.y = rectXY.y + rectWH.y - itemWH.y;
		break;
	}
	case PositionType::DOWN:
	{
		itemXY.x = (2.0f * rectXY.x + rectWH.x - itemWH.x) / 2.0f;
		itemXY.y = rectXY.y + rectWH.y - itemWH.y;
		break;
	}
	case PositionType::RIGHT_DOWN:
	{
		itemXY = rectXY + rectWH - itemWH;
		break;
	}
	}

	return itemXY;
}
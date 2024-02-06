#include "Positioner.h"

#include "Core/Application.h"

void Positioner::PositionItemInWindow(Entity entity, PositionType positioning)
{
	auto& transformComponent = entity.GetComponent<Transform>();
	auto& meshComponent = entity.GetComponent<Mesh>();

	int width = 1, height = 1;

	if (meshComponent.GetTexture())
	{
		width = meshComponent.GetSize().x;
		height = meshComponent.GetSize().y;
	}

	glm::vec2 itemXY = transformComponent.GetPosition();
	glm::vec2 itemWH = glm::vec2(width, height);

	glm::vec2 rectXY = glm::ivec2(0, 0);
	glm::vec2 rectWH = glm::ivec2(Application::Get()->GetWindow()->GetWidth(),
		Application::Get()->GetWindow()->GetHeight());

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

	transformComponent.SetPosition(itemXY);
}
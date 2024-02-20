#include "Positioner.h"

#include "Core/Application.h"

void Positioner::PositionItemInWindow(Entity entity, PositionType positioning)
{
	auto& transformComponent = entity.GetComponent<Transform>();
	auto& spriteComponent = entity.GetComponent<SpriteRenderer>();

	float width = transformComponent.GetSize().x;
	float height = transformComponent.GetSize().y;

	glm::vec2 itemXY = transformComponent.GetPosition();
	glm::vec2 itemWH = glm::vec2(width, height);

	glm::vec2 rectXY = glm::ivec2(0, 0);
	glm::vec2 rectWH = glm::ivec2(Application::GetWindow()->GetWidth(),
		Application::GetWindow()->GetHeight());

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

void Positioner::PositionCameraToEntity(Entity entity, PositionType positioning)
{
	auto& transformComponent = entity.GetComponent<Transform>();
	auto& spriteComponent = entity.GetComponent<SpriteRenderer>();

	float width = transformComponent.GetSize().x;
	float height = transformComponent.GetSize().y;

	glm::vec2 itemXY = transformComponent.GetPosition();
	glm::vec2 itemWH = glm::vec2(width, height);

	auto& cameraTransformComponent = Application::GetCurrentScene()->GetCamera()->transform();

	glm::vec2 cameraXY = cameraTransformComponent.GetPosition();
	glm::vec2 cameraWH = glm::ivec2(Application::GetWindow()->GetWidth(),
		Application::GetWindow()->GetHeight());

	switch (positioning)
	{
	case PositionType::LEFT_UP:
	{
		cameraXY = itemXY;
		break;
	}
	case PositionType::UP:
	{
		cameraXY.x = (2.0f * itemXY.x + itemWH.x - cameraWH.x) / 2.0f;
		cameraXY.y = itemXY.y;
		break;
	}
	case PositionType::RIGHT_UP:
	{
		cameraXY.x = itemXY.x + itemWH.x - cameraWH.x;
		cameraXY.y = itemXY.y;
		break;
	}
	case PositionType::LEFT:
	{
		cameraXY.x = itemXY.x;
		cameraXY.y = (2.0f * itemXY.y + itemWH.y - cameraWH.y) / 2.0f;
		break;
	}
	case PositionType::CENTER:
	{
		cameraXY = (2.0f * itemXY + itemWH - cameraWH) / 2.0f;
		break;
	}
	case PositionType::RIGHT:
	{
		cameraXY.x = itemXY.x + itemWH.x - cameraWH.x;
		cameraXY.y = (2.0f * itemXY.y + itemWH.y - cameraWH.y) / 2.0f;
		break;
	}
	case PositionType::LEFT_DOWN:
	{
		cameraXY.x = itemXY.x;
		cameraXY.y = itemXY.y + itemWH.y - cameraWH.y;
		break;
	}
	case PositionType::DOWN:
	{
		cameraXY.x = (2.0f * itemXY.x + itemWH.x - cameraWH.x) / 2.0f;
		cameraXY.y = itemXY.y + itemWH.y - cameraWH.y;
		break;
	}
	case PositionType::RIGHT_DOWN:
	{
		cameraXY = itemXY + itemWH - cameraWH;
		break;
	}
	}

	cameraTransformComponent.SetPosition(cameraXY);
}
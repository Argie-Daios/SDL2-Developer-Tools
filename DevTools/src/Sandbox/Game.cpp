#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"

Game::Game()
	: Application()
{
	{
		m_Entity1 = CreateRef<Entity>();
		auto& transformComponent = m_Entity1->transform();
		auto& spriteComponent = m_Entity1->AddComponent<SpriteRenderer>("assets/textures/X.png");
		transformComponent.SetPosition(glm::vec2(500, 200));
		transformComponent.SetScale(glm::vec2(0.5f, 0.5f));
	}

	{
		m_Entity2 = CreateRef<Entity>();
		auto& transformComponent = m_Entity2->transform();
		auto& spriteComponent = m_Entity2->AddComponent<SpriteRenderer>("assets/textures/bounce_ball.png");
		auto& animationComponent = m_Entity2->AddComponent<Animation>("assets/textures/bounce_ball.png", 8, 70);
		
		transformComponent.SetPosition(glm::vec2(0, 0));
	}

	{
		m_Entity3 = CreateRef<Entity>();
		auto& transformComponent = m_Entity3->transform();
		auto& textComponent = m_Entity3->AddComponent<Text>("Sample Text", "assets/fonts/arial.ttf", 65, SDL_Color{ 255,0,0,255 });

		transformComponent.SetPosition(glm::vec2(600, 0));
	}
}

void Game::Update()
{
	Application::Update();

	// Camera Position to Entity
#if 0
		if (Input::IsKeyDown(Key::NUM7))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::LEFT_UP);
		}
		if (Input::IsKeyDown(Key::NUM8))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::UP);
		}
		if (Input::IsKeyDown(Key::NUM9))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::RIGHT_UP);
		}
		if (Input::IsKeyDown(Key::NUM4))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::LEFT);
		}
		if (Input::IsKeyDown(Key::NUM5))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::CENTER);
		}
		if (Input::IsKeyDown(Key::NUM6))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::RIGHT);
		}
		if (Input::IsKeyDown(Key::NUM1))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::LEFT_DOWN);
		}
		if (Input::IsKeyDown(Key::NUM2))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::DOWN);
		}
		if (Input::IsKeyDown(Key::NUM3))
		{
			Positioner::PositionCameraToEntity(*m_Entity1, PositionType::RIGHT_DOWN);
		}
#endif
}
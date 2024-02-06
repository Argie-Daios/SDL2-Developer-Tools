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
		auto& transformComponent = m_Entity1->GetComponent<Transform>();
		auto& spriteComponent = m_Entity1->AddComponent<SpriteRenderer>("assets/textures/X.png");
		transformComponent.SetPosition(glm::vec2(500, 200));
		transformComponent.SetScale(glm::vec2(0.5f, 0.5f));
	}

	{
		m_Entity2 = CreateRef<Entity>();
		auto& transformComponent = m_Entity2->GetComponent<Transform>();
		auto& spriteComponent = m_Entity2->AddComponent<SpriteRenderer>("assets/textures/bounce_ball.png");
		auto& animationComponent = m_Entity2->AddComponent<Animation>("assets/textures/bounce_ball.png", 8, 70);
		
		transformComponent.SetPosition(glm::vec2(0, 0));
	}

	{
		m_Entity3 = CreateRef<Entity>();
		auto& transformComponent = m_Entity3->GetComponent<Transform>();
		auto& textComponent = m_Entity3->AddComponent<Text>("Sample Text", "assets/fonts/arial.ttf", 65, SDL_Color{ 255,0,0,255 });

		transformComponent.SetPosition(glm::vec2(600, 0));
	}
}



void Game::Update()
{
	Application::Update();
}
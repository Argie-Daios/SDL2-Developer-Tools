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
		transformComponent.SetZValue(2.0f);
	}

	{
		m_Entity2 = CreateRef<Entity>();
		auto& transformComponent = m_Entity2->transform();
		auto& animationComponent = m_Entity2->AddComponent<Animator>(
			std::initializer_list({
				AnimationNode("Idle", CreateRef<Animation>("assets/textures/NightBorne.png", 9, 0, 23, 5)),
				AnimationNode("Run", CreateRef<Animation>("assets/textures/NightBorne.png", 6, 1, 23, 5))
			})
		);

		animationComponent.AddTwoSideEdge("Idle", "Run", false, false);

		animationComponent.AddBoolParameter("Running", false);

		animationComponent.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
		animationComponent.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

		transformComponent.SetPosition(glm::vec2(0, 0));
		transformComponent.SetScale(glm::vec2(5, 5));
		transformComponent.SetZValue(1.0f);
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

		if (Input::IsKeyPressed(Key::A))
		{
			m_Entity2->transform().Translate(glm::vec2(-300 * Time::DeltaTime(), 0));
			m_Entity2->transform().SetFlip(SDL_FLIP_HORIZONTAL);
			m_Entity2->GetComponent<Animator>().ChangeBoolParameterValue("Running", true);
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_Entity2->transform().Translate(glm::vec2(300 * Time::DeltaTime(), 0));
			m_Entity2->transform().SetFlip(SDL_FLIP_NONE);
			m_Entity2->GetComponent<Animator>().ChangeBoolParameterValue("Running", true);
		}
		if (!Input::IsKeyPressed(Key::A) && !Input::IsKeyPressed(Key::D))
		{
			m_Entity2->GetComponent<Animator>().ChangeBoolParameterValue("Running", false);
		}

}
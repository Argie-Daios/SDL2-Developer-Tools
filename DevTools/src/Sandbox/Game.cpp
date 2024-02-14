#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"

#include <sstream>

static float startTime;

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
		auto& animationComponent = m_Entity2->AddComponent<Animator>(
			std::initializer_list({
				std::pair<std::string, Ref<Animation>>("Idle", CreateRef<Animation>("assets/textures/NightBorne.png", 9, 0, 23, 5)),
				std::pair<std::string, Ref<Animation>>("Run", CreateRef<Animation>("assets/textures/NightBorne.png", 6, 1, 23, 5)),
				std::pair<std::string, Ref<Animation>>("Attack", CreateRef<Animation>("assets/textures/NightBorne.png", 12, 2, 23, 5, 100, false))
			})
		);

		animationComponent.AddTwoSideEdge("Idle", "Run", false, false);
		animationComponent.AddTwoSideEdge("Idle", "Attack", false, false);
		animationComponent.AddTwoSideEdge("Run", "Attack", false, false);

		animationComponent.AddBoolParameter("Running", false);
		animationComponent.AddBoolParameter("Attacking", false);

		animationComponent.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
		animationComponent.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

		animationComponent.AddConditionOnEdgeBool("Attack", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);
		animationComponent.AddConditionOnEdgeBool("Attack", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);

		animationComponent.AddConditionOnEdgeBool("Attack", "Idle", "Attacking", Operation::OperationFunc::BOOL_IS, false);
		animationComponent.AddConditionOnEdgeBool("Attack", "Run", "Attacking", Operation::OperationFunc::BOOL_IS, false);

		animationComponent.AddConditionOnEdgeBool("Idle", "Attack", "Attacking", Operation::OperationFunc::BOOL_IS, true);
		animationComponent.AddConditionOnEdgeBool("Run", "Attack", "Attacking", Operation::OperationFunc::BOOL_IS, true);

		transformComponent.SetPosition(glm::vec2(0, 0));
		transformComponent.SetScale(glm::vec2(5, 5));
	}
}

static float timeElapsed = 0.0f;
static float duration = 4.0f;
static bool start = false;

IEnumerator Game::Attack()
{
	m_Entity2->GetComponent<Animator>().ChangeParameterValue("Attacking", ValueToVoidPtr(true));

	yield_return NewReturnType<WaitForSeconds>(Time::MillisecondsToSeconds(12 * 100));

	m_Entity2->GetComponent<Animator>().ChangeParameterValue("Attacking", ValueToVoidPtr(false));
}

template <typename T>
static std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return std::move(out).str();
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

		if (Input::IsKeyPressed(Key::D))
		{
			m_Entity2->transform().Translate(glm::vec2(300 * Time::DeltaTime(), 0));
			m_Entity2->transform().SetFlip(SDL_FLIP_NONE);
			m_Entity2->GetComponent<Animator>().ChangeParameterValue("Running", ValueToVoidPtr(true));
		}
		if (Input::IsKeyPressed(Key::A))
		{
			m_Entity2->transform().Translate(glm::vec2(-300 * Time::DeltaTime(), 0));
			m_Entity2->transform().SetFlip(SDL_FLIP_HORIZONTAL);
			m_Entity2->GetComponent<Animator>().ChangeParameterValue("Running", ValueToVoidPtr(true));
		}
		if (!Input::IsKeyPressed(Key::D) && !Input::IsKeyPressed(Key::A))
		{
			m_Entity2->GetComponent<Animator>().ChangeParameterValue("Running", ValueToVoidPtr(false));
		}

		if (Input::IsKeyDown(Key::SPACE))
		{
			CoroutineManager::StartCoroutine(Attack());
		}
}
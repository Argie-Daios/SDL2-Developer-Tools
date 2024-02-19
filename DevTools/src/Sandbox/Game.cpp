#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"

Game::Game()
	: Application()
{
	{
		m_Entity1 = CreateRef<Entity>("Wizard");
		auto& transformComponent = m_Entity1->transform();
		auto& animationComponent = m_Entity1->AddComponent<Animator>(
			std::initializer_list({
				AnimationNode("Idle", CreateRef<Animation>("assets/textures/Wizard/Idle.png", 6, 0, 6, 1)),
				AnimationNode("Run", CreateRef<Animation>("assets/textures/Wizard/Run.png", 8, 0, 8, 1)),
				AnimationNode("Attack", CreateRef<Animation>("assets/textures/Wizard/Attack1.png", 8, 0, 8, 1))
			})
		);

		animationComponent.AddTwoSideEdge("Idle", "Run", false, false);
		animationComponent.AddTwoSideEdge("Attack", "Run", false, false);
		animationComponent.AddTwoSideEdge("Attack", "Idle", false, false);

		animationComponent.AddBoolParameter("Running", false);
		animationComponent.AddBoolParameter("Attack", false);

		animationComponent.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
		animationComponent.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

		animationComponent.AddConditionOnEdgeBool("Idle", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);
		animationComponent.AddConditionOnEdgeBool("Run", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);

		animationComponent.AddConditionOnEdgeBool("Attack", "Idle", "Attack", Operation::OperationFunc::BOOL_IS, false);
		animationComponent.AddConditionOnEdgeBool("Attack", "Run", "Attack", Operation::OperationFunc::BOOL_IS, false);

		animationComponent.AddConditionOnEdgeBool("Attack", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);
		animationComponent.AddConditionOnEdgeBool("Attack", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);

		transformComponent.SetPosition(glm::vec2(0, 0));
		transformComponent.SetScale(glm::vec2(4, 4));
		transformComponent.SetZValue(3.0f);
	}
}

IEnumerator Game::Attack()
{
	m_Entity1->GetComponent<Animator>().ChangeBoolParameterValue("Attack", true);
	yield_return NewReturnType<WaitForSeconds>(0.1 * 8);
	m_Entity1->GetComponent<Animator>().ChangeBoolParameterValue("Attack", false);
}

void Game::Update()
{
	Application::Update();

	if (Input::IsKeyDown(Key::P))
	{
		ChangeScene((currentScene == "Start Scene" ? "Rofl Scene" : "Start Scene"));
	}
}
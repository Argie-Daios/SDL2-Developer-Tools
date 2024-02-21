#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"
#include "Script.h"
#include "Script2.h"

Game::Game()
	: Application()
{
	AddScene("Second Scene");

	GetCurrentScene()->AddEntity("Wizard");

	auto& transformComponent = GetCurrentScene()->GetEntity("Wizard").GetComponent<Transform>();
	auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard").AddComponent<Animator>(
		std::initializer_list({
			AnimationNode("Idle", CreateRef<Animation>("assets/textures/Wizard/Idle.png", 6, 0, 6, 1)),
			AnimationNode("Run", CreateRef<Animation>("assets/textures/Wizard/Run.png", 8, 0, 8, 1)),
			AnimationNode("Attack", CreateRef<Animation>("assets/textures/Wizard/Attack1.png", 8, 0, 8, 1, 100, false))
		})
	);

	animatorComponent.AddTwoSideEdge("Idle", "Run", false, false);
	animatorComponent.AddTwoSideEdge("Attack", "Run", false, false);
	animatorComponent.AddTwoSideEdge("Attack", "Idle", false, false);

	animatorComponent.AddBoolParameter("Running", false);
	animatorComponent.AddBoolParameter("Attack", false);

	animatorComponent.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
	animatorComponent.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

	animatorComponent.AddConditionOnEdgeBool("Idle", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);
	animatorComponent.AddConditionOnEdgeBool("Run", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);

	animatorComponent.AddConditionOnEdgeBool("Attack", "Idle", "Attack", Operation::OperationFunc::BOOL_IS, false);
	animatorComponent.AddConditionOnEdgeBool("Attack", "Run", "Attack", Operation::OperationFunc::BOOL_IS, false);

	animatorComponent.AddConditionOnEdgeBool("Attack", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);
	animatorComponent.AddConditionOnEdgeBool("Attack", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);

	transformComponent.SetPosition(glm::vec2(0, 0));
	transformComponent.SetScale(glm::vec2(4, 4));
	transformComponent.SetZValue(3.0f);

	GetCurrentScene()->GetEntity("Wizard").AddComponent<Behaviour>().Bind<Wizard>();

	GetCurrentScene()->AddEntity("X");

	GetCurrentScene()->GetEntity("X").AddComponent<Behaviour>().Bind<Rotated>();

	auto& spriteRenderer = GetCurrentScene()->GetEntity("X").AddComponent<Animation>("assets/textures/Effects/Vortex.png", 48, 0, 9, 7, 60);

	GetCurrentScene()->GetEntity("X").transform().SetScale(glm::vec2(0.5f, 0.5f));
}

static bool Default = false;

void Game::Update()
{
	Application::Update();

	// Better use scripts for entity behaviour

	if (Input::IsKeyDown(Key::P))
	{
		ChangeScene((Default ? "Second Scene" : "Default Scene"));
		Default = !Default;
	}
}
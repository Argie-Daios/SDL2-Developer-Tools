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
	AssetManager::LoadTexture("Wizard_Idle", "assets/textures/Wizard/Idle.png");
	AssetManager::LoadTexture("Wizard_Run", "assets/textures/Wizard/Run.png");
	AssetManager::LoadTexture("Wizard_Attack", "assets/textures/Wizard/Attack1.png");
	AssetManager::LoadTexture("X", "assets/textures/X.png");
	AssetManager::LoadTexture("Vortex", "assets/textures/Effects/Vortex.png");
	AssetManager::LoadFont("Arial", "assets/fonts/arial.ttf", 50);

	AddScene("Second Scene");

	GetCurrentScene()->AddEntity("Wizard");

	auto& transformComponent = GetCurrentScene()->GetEntity("Wizard").GetComponent<Transform>();
	auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard").AddComponent<Animator>(
		std::initializer_list({
			AnimationNode("Idle", CreateRef<Animation>("Wizard_Idle", 6, 0, 6, 1)),
			AnimationNode("Run", CreateRef<Animation>("Wizard_Run", 8, 0, 8, 1)),
			AnimationNode("Attack", CreateRef<Animation>("Wizard_Attack", 8, 0, 8, 1, 100, true))
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

	auto& spriteRenderer = GetCurrentScene()->GetEntity("X").AddComponent<Animation>("Vortex", 48, 0, 9, 7, 20);

	GetCurrentScene()->AddEntity("Entity 1");
	GetCurrentScene()->GetEntity("Entity 1").AddComponent<Text>("Text1", "EXW PESEI", "Arial", glm::vec3(255, 0, 0));
	GetCurrentScene()->GetEntity("Entity 1").transform().Translate(glm::vec2(700, 100));

	AssetManager::CreatePrefab("Prefab_Vortex", GetCurrentScene()->GetEntity("X"));
	GetCurrentScene()->RemoveEntity("X");
}

static bool Default = false;

void Game::Update()
{
	Application::Update();

	// Better use scripts for entity behaviour

	if (Input::IsKeyDown(Key::P))
	{
		Renderer::SetRender(false);
	}
}
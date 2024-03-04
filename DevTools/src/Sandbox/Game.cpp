#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"
#include "Script.h"
#include "Tools/Color.h"

Game::Game()
	: Application()
{
	AssetManager::LoadTexture("Wizard_Idle", "assets/textures/Wizard/Idle.png");
	AssetManager::LoadTexture("Wizard_Run", "assets/textures/Wizard/Run.png");
	AssetManager::LoadTexture("Wizard_Attack", "assets/textures/Wizard/Attack1.png");
	AssetManager::LoadTexture("X", "assets/textures/X.png");
	AssetManager::LoadTexture("Vortex", "assets/textures/Effects/Vortex.png");
	AssetManager::LoadTexture("Mystic_Magic", "assets/textures/Effects/Mystic_Magic.png");
	AssetManager::LoadAnimation("Vortex_Animation", AnimationProperties{ "Vortex", 63, 9, 7, 0, 48 });
	AssetManager::LoadAnimation("Wizard_Idle_Animation", AnimationProperties{ "Wizard_Idle", 6, 6, 1, 0, 6 });
	AssetManager::LoadAnimation("Wizard_Run_Animation", AnimationProperties{ "Wizard_Run", 8, 8, 1, 0, 8 });
	AssetManager::LoadAnimation("Wizard_Attack_Animation", AnimationProperties{ "Wizard_Attack", 8, 8, 1, 0, 8 });
	AssetManager::LoadAnimation("Mystic_Magic_Animation", AnimationProperties{ "Mystic_Magic", 30, 5, 6, 0, 30 });
	AssetManager::LoadFont("Arial", "assets/fonts/arial.ttf", 50);
	AssetManager::LoadFont("Old_English", "assets/fonts/OldEnglish.ttf", 60);

	GetCurrentScene()->AddEntity("Wizard");

	auto& transformComponent = GetCurrentScene()->GetEntity("Wizard").GetComponent<Transform>();

	AnimationController animationController;
	animationController.AddAnimation("Idle", AnimationNode{ "Wizard_Idle_Animation" });
	animationController.AddAnimation("Run", AnimationNode{ "Wizard_Run_Animation" });
	animationController.AddAnimation("Attack", AnimationNode{ "Wizard_Attack_Animation" });

	animationController.AddTwoSidedEdge("Idle", "Run", false, false);
	animationController.AddTwoSidedEdge("Attack", "Run", false, false);
	animationController.AddTwoSidedEdge("Attack", "Idle", false, false);

	animationController.AddBoolParameter("Running", false);
	animationController.AddBoolParameter("Attack", false);

	animationController.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
	animationController.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

	animationController.AddConditionOnEdgeBool("Idle", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);
	animationController.AddConditionOnEdgeBool("Run", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);

	animationController.AddConditionOnEdgeBool("Attack", "Idle", "Attack", Operation::OperationFunc::BOOL_IS, false);
	animationController.AddConditionOnEdgeBool("Attack", "Run", "Attack", Operation::OperationFunc::BOOL_IS, false);

	animationController.AddConditionOnEdgeBool("Attack", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);
	animationController.AddConditionOnEdgeBool("Attack", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);

	AssetManager::CreateAnimationController("Animation_Controller", animationController);

	auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard").AddComponent<Animator>("Animation_Controller");

	transformComponent.SetPosition(glm::vec2(0, 0));
	transformComponent.SetScale(glm::vec2(4, 4));
	transformComponent.SetZValue(3.0f);

	GetCurrentScene()->GetEntity("Wizard").AddComponent<Behaviour>().Bind<Wizard>();

	{
		GetCurrentScene()->AddEntity("Wizard2");
		auto& transformComponent = GetCurrentScene()->GetEntity("Wizard2").GetComponent<Transform>();
		auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard2").AddComponent<Animator>("Animation_Controller");
		transformComponent.SetPosition(glm::vec2(400, 0));
	}

	GetCurrentScene()->AddEntity("X");
	//GetCurrentScene()->GetEntity("X").transform().Translate(glm::vec2(650, 100));
	GetCurrentScene()->GetEntity("X").transform().SetZValue(5.0f);

	auto& spriteRenderer = GetCurrentScene()->GetEntity("X").AddComponent<Animation>("Mystic_Magic_Animation", 30);

	GetCurrentScene()->AddEntity("Entity 1");
	GetCurrentScene()->GetEntity("Entity 1").AddComponent<Text>("Text1", "AC/DC", "Old_English", Color::PURPLE);
	GetCurrentScene()->GetEntity("Entity 1").transform().Translate(glm::vec2(700, 100));

	//GetCurrentScene()->GetEntity("Wizard").AddChild(GetCurrentScene()->GetCamera().handle());

	AssetManager::CreatePrefab("Prefab_Wizard", GetCurrentScene()->GetEntity("Wizard"));
	AssetManager::CreatePrefab("Prefab_Vortex", GetCurrentScene()->GetEntity("X"));
	GetCurrentScene()->DeleteEntity("X");

	Entity entity = GetCurrentScene()->AddEntity("FPS counter");
	entity.AddComponent<Text>("TextFPS", "0", "Arial", glm::vec3(0, 255, 0));
}

void Game::Update()
{
	Application::Update();

	if (Input::IsKeyDown(Key::P))
	{
		Renderer::SetRender(false);
	}

	Entity entity = GetCurrentScene()->GetEntity("FPS counter");
	auto& textComponent = entity.GetComponent<Text>();

	int fps = (int)Time::FPS();
	if (std::stoi(textComponent.GetLabel()) != fps)
	{
		textComponent.ChangeLabel(std::to_string((int)Time::FPS()));
		if (fps >= 400)
		{
			if (textComponent.GetColor() != glm::vec3(0.0f, 255.0f, 0.0f))
			{
				std::cout << "Changed to Green" << std::endl;
				textComponent.ChangeColor(glm::vec3(0.0f, 255.0f, 0.0f));
			}
		}
		else if (fps >= 300)
		{
			if (textComponent.GetColor() != glm::vec3(255.0f, 165.0f, 0.0f))
			{
				std::cout << "Changed to Orange" << std::endl;
				textComponent.ChangeColor(glm::vec3(255.0f, 165.0f, 0.0f));
			}
		}
		else if(textComponent.GetColor() != glm::vec3(255.0f, 0.0f, 0.0f))
		{
			std::cout << "Changed to Red" << std::endl;
			textComponent.ChangeColor(glm::vec3(255.0f, 0.0f, 0.0f));
		}
	}
}
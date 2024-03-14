#include "Game.h"

#include "ECS/Entity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Positioner.h"
#include "Script.h"
#include "Tools/Color.h"

#include <SDL_ttf.h>
#include <SDL_image.h>

Game::Game()
	: Application()
{
	//AssetManager::LoadTextureAndShrinkToFit("Wiz", "assets/textures/Wizard/Wiz.png");
	//AssetManager::LoadTexture("Wizard_Idle", "assets/textures/Wizard/Idle.png");
	//AssetManager::LoadTexture("Wizard_Run", "assets/textures/Wizard/Run.png");
	//AssetManager::LoadTexture("Wizard_Attack", "assets/textures/Wizard/Attack1.png");
	//AssetManager::LoadTexture("X", "assets/textures/X.png");
	//AssetManager::LoadTexture("Vortex", "assets/textures/Effects/Vortex.png");
	//AssetManager::LoadTexture("Mystic_Magic", "assets/textures/Effects/Mystic_Magic.png");
	//AssetManager::LoadTexture("Blue_Magic", "assets/textures/Effects/Blue_Magic.png");
	//AssetManager::LoadAnimation("Vortex_Animation", AnimationProperties{ "Vortex", 63, 9, 7, 0, 48 });
	//AssetManager::LoadAnimation("Wizard_Idle_Animation", AnimationProperties{ "Wizard_Idle", 6, 6, 1, 0, 6 });
	//AssetManager::LoadTextureAndShrinkToFit("Mystic_Magic", "assets/textures/Effects/MG.png");
	AssetManager::LoadSpritesheetAndShrinkToFit("WizardSS", "assets/textures/Wizard/Attack1.png", 8, 1);
	AssetManager::LoadAnimation("Wizard_Anim", "WizardSS", 0, 8);
	//AssetManager::LoadAnimation("Wizard_Run_Animation", AnimationProperties{ "Wizard_Run", 8, 8, 1, 0, 8 });
	//AssetManager::LoadAnimation("Wizard_Attack_Animation", AnimationProperties{ "Wizard_Attack", 8, 8, 1, 0, 8 });
	//AssetManager::LoadAnimation("Mystic_Magic_Animation", AnimationProperties{ "Mystic_Magic", 30, 5, 6, 0, 30 });
	//AssetManager::LoadAnimation("Blue_Magic_Animation", AnimationProperties{ "Blue_Magic", 20, 5, 4, 0, 20 });
	//AssetManager::LoadFont("Arial", "assets/fonts/arial.ttf", 50);
	//AssetManager::LoadFont("Old_English", "assets/fonts/OldEnglish.ttf", 60);

	//GetCurrentScene()->AddEntity("Wizard");

	//auto& transformComponent = GetCurrentScene()->GetEntity("Wizard").GetComponent<Transform>();

	//AnimationController animationController;
	//animationController.AddAnimation("Idle", AnimationNode{ "Wizard_Idle_Animation" });
	//animationController.AddAnimation("Run", AnimationNode{ "Wizard_Run_Animation" });
	//animationController.AddAnimation("Attack", AnimationNode{ "Wizard_Attack_Animation" });

	//animationController.AddTwoSidedEdge("Idle", "Run", false, false);
	//animationController.AddTwoSidedEdge("Attack", "Run", false, false);
	//animationController.AddTwoSidedEdge("Attack", "Idle", false, false);

	//animationController.AddBoolParameter("Running", false);
	//animationController.AddBoolParameter("Attack", false);

	//animationController.AddConditionOnEdgeBool("Idle", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);
	//animationController.AddConditionOnEdgeBool("Run", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);

	//animationController.AddConditionOnEdgeBool("Idle", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);
	//animationController.AddConditionOnEdgeBool("Run", "Attack", "Attack", Operation::OperationFunc::BOOL_IS, true);

	//animationController.AddConditionOnEdgeBool("Attack", "Idle", "Attack", Operation::OperationFunc::BOOL_IS, false);
	//animationController.AddConditionOnEdgeBool("Attack", "Run", "Attack", Operation::OperationFunc::BOOL_IS, false);

	//animationController.AddConditionOnEdgeBool("Attack", "Idle", "Running", Operation::OperationFunc::BOOL_IS, false);
	//animationController.AddConditionOnEdgeBool("Attack", "Run", "Running", Operation::OperationFunc::BOOL_IS, true);

	//AssetManager::CreateAnimationController("Animation_Controller", animationController);

	//auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard").AddComponent<Animator>("Animation_Controller");

	//transformComponent.SetPosition(glm::vec2(0, 0));
	//transformComponent.SetScale(glm::vec2(4, 4));
	//transformComponent.SetZValue(-7.0f);

	//GetCurrentScene()->GetEntity("Wizard").AddComponent<Behaviour>().Bind<Wizard>();

	//{
	//	GetCurrentScene()->AddEntity("Wizard2");
	//	auto& transformComponent = GetCurrentScene()->GetEntity("Wizard2").GetComponent<Transform>();
	//	auto& animatorComponent = GetCurrentScene()->GetEntity("Wizard2").AddComponent<Animator>("Animation_Controller");
	//	transformComponent.SetPosition(glm::vec2(400, 0));
	//}

	//GetCurrentScene()->AddEntity("Logo");
	//GetCurrentScene()->GetEntity("Logo").transform().Translate(glm::vec2(1000.0f, 500.0f));
	//GetCurrentScene()->GetEntity("Logo").AddComponent<SpriteRenderer>("X");

	//GetCurrentScene()->AddEntity("X");
	////GetCurrentScene()->GetEntity("X").transform().Translate(glm::vec2(650, 100));
	//GetCurrentScene()->GetEntity("X").transform().SetZValue(5.0f);

	//auto& spriteRenderer = GetCurrentScene()->GetEntity("X").AddComponent<Animation>("Mystic_Magic_Animation", 30);

	//GetCurrentScene()->AddEntity("Entity 1");
	//GetCurrentScene()->GetEntity("Entity 1").AddComponent<Text>("Text1", "AC/DC", "Old_English", Color::PURPLE);
	//GetCurrentScene()->GetEntity("Entity 1").transform().Translate(glm::vec2(700, 100));

	////GetCurrentScene()->GetEntity("Wizard").AddChild(GetCurrentScene()->GetCamera().handle());

	//AssetManager::CreatePrefab("Prefab_Wizard", GetCurrentScene()->GetEntity("Wizard"));
	//AssetManager::CreatePrefab("Prefab_Vortex", GetCurrentScene()->GetEntity("X"));
	//GetCurrentScene()->DeleteEntity("X");

	//Entity entity = GetCurrentScene()->AddEntity("FPS counter");
	//entity.AddComponent<Text>("TextFPS", "0", "Arial", Color::GREEN);

	//TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 50);
	//if (font == nullptr) std::cout << "WTF NIGGA BITCH" << std::endl;


	// Need fix

	GetCurrentScene()->AddEntity("Logo");
	auto& spriteRenderer = GetCurrentScene()->GetEntity("Logo").AddComponent<Animation>("Wizard_Anim", 300.0f);
	//auto& spriteRenderer = GetCurrentScene()->GetEntity("Logo").AddComponent<SpriteRenderer>("Wiz");
	auto& transformComponent = GetCurrentScene()->GetEntity("Logo").transform();
	transformComponent.SetScale(glm::vec2(3.0f, 3.0f));
	//GetCurrentScene()->GetEntity("Logo").transform().SetScale(glm::vec2(3.0f, 3.0f));
	//GetCurrentScene()->GetEntity("Logo").transform().SetSize(glm::vec2(w - 2 * decX, h - 2 * decY));
	//rect = SDL_Rect{ decX, decY, w - 2 * decX, h - 2 * decY };
}

void Game::Update()
{
	Application::Update();

	/*Entity entity = GetCurrentScene()->GetEntity("FPS counter");
	auto& textComponent = entity.GetComponent<Text>();

	int fps = (int)Time::FPS();
	if (std::stoi(textComponent.GetLabel()) != fps)
	{
		textComponent.ChangeLabel(std::to_string((int)Time::FPS()));
		if (fps >= 400)
		{
			if (!Color::Compare(textComponent.GetColor(), Color::GREEN))
			{
				textComponent.ChangeColor(Color::GREEN);
			}
		}
		else if (fps >= 300)
		{
			if (!Color::Compare(textComponent.GetColor(), Color::ORANGE))
			{
				textComponent.ChangeColor(Color::ORANGE);
			}
		}
		else if(!Color::Compare(textComponent.GetColor(), Color::RED))
		{
			textComponent.ChangeColor(Color::RED);
		}
	}*/
}

void Game::Draw()
{
	Application::Draw();
	
	/*glm::vec2 position = GetCurrentScene()->GetEntity("Wizard").transform().GetPosition();
	glm::vec2 size = GetCurrentScene()->GetEntity("Wizard").transform().GetSize() * GetCurrentScene()->GetEntity("Wizard").transform().GetScale();
	Renderer::DrawRect(SDL_Rect{ (int)position.x, (int)position.y, (int)size.x, (int)size.y});*/

	auto& transformComponent = GetCurrentScene()->GetEntity("Logo").transform();
	SDL_Rect rect = SDL_Rect{ (int)transformComponent.GetPosition().x, (int)transformComponent.GetPosition().y,
	(int)(transformComponent.GetSize().x * transformComponent.GetScale().x),
	(int)(transformComponent.GetSize().y * transformComponent.GetScale().y) };
	Renderer::DrawRect(rect, Color::RED);
}
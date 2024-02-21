#include "Application.h"

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"

#include <cpproutine/Coroutine.h>

Application* Application::s_Instance = nullptr;
Ref<Window> Application::window = nullptr;
std::unordered_map<std::string, Ref<Scene>> Application::s_Scenes;
std::string Application::currentScene = "-";

Application::Application()
{
	s_Instance = this;
	
	window = CreateRef<Window>("Application", 1440, 810);

	Renderer::Init(window->Get());

	AddScene("Default Scene");
	ChangeScene("Default Scene");
}

Application::~Application()
{
	Renderer::Shutdown();
}

void Application::Event(SDL_Event* event)
{
	SDL_PollEvent(event);

	switch (event->type)
	{
	case SDL_QUIT:
	{
		m_Running = false;
		break;
	}
	}
}

void Application::AddScene(const std::string& scene)
{
	GAME_ASSERT(s_Scenes.find(scene) == s_Scenes.end(), "Scene already exists");

	s_Scenes.emplace(scene, CreateRef<Scene>());
	std::string temp = currentScene;
	currentScene = scene;
	s_Scenes[currentScene]->m_Camera = CreateRef<Entity>("Camera");
	currentScene = temp;
}

void Application::ChangeScene(const std::string& scene)
{
	GAME_ASSERT(s_Scenes.find(scene) != s_Scenes.end(), "This scene does not exist");

	currentScene = scene;
}

void Application::UpdateAnimations()
{
	auto view = s_Scenes[currentScene]->m_Registry.view<Animation>();

	for (auto entity : view)
	{
		Entity e = { entity };

		auto& animationComponent = e.GetComponent<Animation>();

		animationComponent.Animate();
	}
}

void Application::UpdateAnimators()
{
	auto view = s_Scenes[currentScene]->m_Registry.view<Animator>();

	for (auto entity : view)
	{
		Entity e = { entity };

		auto& animatorComponent = e.GetComponent<Animator>();

		animatorComponent.Update();
	}
}

void Application::UpdateColliders()
{
	auto view = s_Scenes[currentScene]->m_Registry.view<Collider>();

	for (auto entity : view)
	{
		Entity e = { entity };
		auto& transformComponent = e.transform();
		auto& colliderComponent = e.GetComponent<Collider>();

		for (auto checkedEntity : view)
		{
			if (checkedEntity == entity) continue;

			Entity e2 = { checkedEntity };
			auto& checkedEntityTransformComponent = e2.transform();
			auto& checkedEntityColliderComponent = e2.GetComponent<Collider>();

			SDL_Rect A = { (int)(transformComponent.GetPosition().x + colliderComponent.GetOffset().x),
				(int)(transformComponent.GetPosition().y + colliderComponent.GetOffset().y),
				(int)colliderComponent.GetSize().x,
				(int)colliderComponent.GetSize().y };

			SDL_Rect B = { (int)(checkedEntityTransformComponent.GetPosition().x + checkedEntityColliderComponent.GetOffset().x),
				(int)(checkedEntityTransformComponent.GetPosition().y + checkedEntityColliderComponent.GetOffset().y),
				(int)checkedEntityColliderComponent.GetSize().x,
				(int)checkedEntityColliderComponent.GetSize().y };

			bool AABB = A.x + A.w > B.x && A.y + A.h > B.y && B.x + B.w > A.x && B.y + B.h > A.y;

			if (AABB)
			{
				if (colliderComponent.onCollision) colliderComponent.onCollision(e2);
				colliderComponent.SetCollision(true);
			}
			else
			{
				colliderComponent.SetCollision(false);
			}
		}
	}
}

void Application::Update()
{
	s_Scenes[currentScene]->m_Registry.view<Behaviour>().each([=](auto entity, auto& behaviour)
	{
		if (!behaviour.Instance)
		{
			behaviour.Instance = behaviour.InstantiateScript();
			behaviour.Instance->m_Entity = Entity{ entity };
			behaviour.Instance->OnCreate();
		}

		if (s_Scenes[currentScene]->m_Registry.valid(entity))
			behaviour.Instance->OnUpdate();
	});

	UpdateAnimations();
	UpdateAnimators();
	UpdateColliders();
}

void Application::Draw()
{
	Renderer::Draw(s_Scenes[currentScene]->m_Registry);
}

void Application::Run()
{
	while (m_Running)
	{
		Time::Tick();

		Event(&Input::Event());
		cpproutine::CoroutineManager::Update();
		Update();

		Renderer::Begin();
		Draw();
		Renderer::End();
	}

}
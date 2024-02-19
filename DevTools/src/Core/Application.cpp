#include "Application.h"

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"

#include <cpproutine/Coroutine.h>

Application* Application::s_Instance = nullptr;
Ref<Window> Application::window = nullptr;
Ref<Entity> Application::s_Camera = nullptr;

Application::Application()
{
	s_Instance = this;
	
	window = CreateRef<Window>("Application", 1440, 810);

	Renderer::Init(window->Get());

	s_Camera = CreateRef<Entity>("Camera");
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

void Application::UpdateAnimations()
{
	auto view = m_Registry.view<Animation>();

	for (auto entity : view)
	{
		Entity e = { entity };

		auto& animationComponent = e.GetComponent<Animation>();

		animationComponent.Animate();
	}
}

void Application::UpdateAnimators()
{
	auto view = m_Registry.view<Animator>();

	for (auto entity : view)
	{
		Entity e = { entity };

		auto& animatorComponent = e.GetComponent<Animator>();

		animatorComponent.Update();
	}
}

void Application::UpdateColliders()
{
	auto view = m_Registry.view<Collider>();

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

			SDL_Rect A = { transformComponent.GetPosition().x + colliderComponent.GetOffset().x,
				transformComponent.GetPosition().y + colliderComponent.GetOffset().y,
				colliderComponent.GetSize().x,
				colliderComponent.GetSize().y };

			SDL_Rect B = { checkedEntityTransformComponent.GetPosition().x + checkedEntityColliderComponent.GetOffset().x,
				checkedEntityTransformComponent.GetPosition().y + checkedEntityColliderComponent.GetOffset().y,
				checkedEntityColliderComponent.GetSize().x,
				checkedEntityColliderComponent.GetSize().y };

			bool AABB = A.x + A.w > B.x && A.y + A.h > B.y && B.x + B.w > A.x && B.y + B.h > A.y;

			if (AABB)
			{
				if (colliderComponent.onCollision) colliderComponent.onCollision(e2);
				colliderComponent.collides = true;
			}
			else
			{
				colliderComponent.collides = false;
			}
		}
	}
}

void Application::Update()
{
	m_Registry.view<Behaviour>().each([=](auto entity, auto& behaviour)
	{
		if (!behaviour.Instance)
		{
			behaviour.Instance = behaviour.InstantiateScript();
			behaviour.Instance->m_Entity = Entity{ entity };
			behaviour.Instance->OnCreate();
		}

		behaviour.Instance->OnUpdate();
	});

	UpdateAnimations();
	UpdateAnimators();
	UpdateColliders();
}

void Application::Draw()
{
	Renderer::Draw(m_Registry);
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
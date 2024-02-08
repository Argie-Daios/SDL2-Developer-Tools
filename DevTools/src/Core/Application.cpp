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

	s_Camera = CreateRef<Entity>();
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
}

void Application::Run()
{
	while (m_Running)
	{
		Time::Tick();

		cpproutine::CoroutineManager::Update();

		Event(&Input::Event());
		Update();

		Renderer::Begin();
		Renderer::Draw(m_Registry);
		Renderer::End();
	}
}
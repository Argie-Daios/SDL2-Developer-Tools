#pragma once

#include "Macros.h"
#include "Tools/Time.h"
#include "Renderer/Renderer.h"
#include "entt.hpp"

#include <vector>

class Entity;

class Application
{
public:
	Application();
	~Application();

	virtual void Event(SDL_Event* event);
	virtual void Update();
	virtual void Run();

	inline static Application* Get() { return s_Instance; }
private:
	void UpdateAnimations();
protected:
	static Application* s_Instance;

	Ref<Window> window;
	
	Ref<Entity>m_Entity;

	entt::registry m_Registry;

	bool m_Running = true;

	friend class Entity;
	friend class Renderer;
	friend class MoveEntity;
	friend class Solitaire;
};
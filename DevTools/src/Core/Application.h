#pragma once

#include "Macros.h"
#include "Tools/Time.h"
#include "Renderer/Renderer.h"
#include "entt.hpp"

#include <vector>

class Entity;
struct Transform;

class Application
{
public:
	Application();
	~Application();

	virtual void Event(SDL_Event* event);
	virtual void Update();
	virtual void Draw();
	virtual void Run();

	Ref<Window> GetWindow() const { return window; }
	Ref<Entity> GetCamera() { return s_Camera; }

	inline static Application* Get() { return s_Instance; }
private:
	void UpdateAnimations();
	void UpdateAnimators();
	void UpdateColliders();
protected:
	static Application* s_Instance;

	static Ref<Window> window;
	static Ref<Entity> s_Camera;

	entt::registry m_Registry;

	bool m_Running = true;

	friend class Entity;
	friend struct Transform;
	friend class Renderer;
	friend class MoveEntity;
};
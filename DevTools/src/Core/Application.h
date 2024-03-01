#pragma once

#include "Macros.h"
#include "Scene.h"
#include "Tools/Time.h"
#include "Renderer/Renderer.h"

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

	static void Shutdown() { s_Instance->m_Running = false; }

	static Ref<Window> GetWindow() { return window; }

	static Ref<Scene> GetCurrentScene() { return s_Scenes[currentScene]; }
	static void AddScene(const std::string& scene);
	static void ChangeScene(const std::string& scene);

	inline static Application* Get() { return s_Instance; }
private:
	void UpdateAnimations();
	void UpdateAnimators();
	void UpdateColliders();
private:
	static std::unordered_map<std::string, Ref<Scene>> s_Scenes;
	static std::string currentScene;
protected:
	static Application* s_Instance;

	static Ref<Window> window;

	bool m_Running = true;

	friend class Entity;
	friend struct Transform;
	friend class Renderer;
	friend class MoveEntity;
};
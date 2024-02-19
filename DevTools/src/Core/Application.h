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

	Ref<Window> GetWindow() const { return window; }
	static Ref<Scene> GetCurrentScene() { return s_Scenes[currentScene]; }
	static void ChangeScene(const std::string& scene);

	inline static Application* Get() { return s_Instance; }
private:
	void UpdateAnimations();
	void UpdateAnimators();
	void UpdateColliders();
protected:
	static Application* s_Instance;

	static Ref<Window> window;
	static std::unordered_map<std::string, Ref<Scene>> s_Scenes;
	static std::string currentScene;

	bool m_Running = true;

	friend class Entity;
	friend struct Transform;
	friend class Renderer;
	friend class MoveEntity;
};
#pragma once

#include "Core/Macros.h"

#include <entt.hpp>

class Entity;
struct Transform;

class Scene
{
public:
	Scene();
	Scene(const Scene&);

	void AddEntity(const std::string& name);

	Ref<Entity> GetCamera() { return m_Camera; }
	Entity GetEntity(const std::string& name);
private:
	entt::entity FindEntity(const std::string& name);
private:
	Ref<Entity> m_Camera;

	entt::registry m_Registry;

	friend class Application;
	friend class Entity;
	friend struct Transform;
};
#pragma once

#include "Core/Macros.h"

#include <entt.hpp>
#include <glm/glm.hpp>

class Entity;
struct Transform;

class Scene
{
public:
	Scene();
	Scene(const Scene&);

	void AddEntity(const std::string& name);
	void RemoveEntity(const std::string& name);
	Entity Instantiate(Entity object, const glm::vec2& position);
	void DeleteEntity(Entity entity);

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
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

	Entity AddEntity(const std::string& name);
	Entity Instantiate(Entity object, const glm::vec2& position);
	void DeleteEntity(const std::string& name);
	void DeleteEntity(Entity entity);

	Entity GetCamera();
	Entity GetEntity(const std::string& name);

	void Sort();
private:
	entt::entity FindEntity(const std::string& name);
	bool nameExists(const std::string& name);
	void InsertSorted(entt::entity entity);
private:
	entt::registry m_Registry;
	std::vector<entt::entity> m_EntitiesVector;

	friend class Application;
	friend class Entity;
	friend struct Transform;
	friend struct SpriteRenderer;
	friend struct Animation;
	friend struct Animator;
	friend struct Text;
	friend struct Collider;
	friend class Renderer;
};
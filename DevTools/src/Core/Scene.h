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

	Ref<Entity> GetCamera() { return m_Camera; }
private:
	Ref<Entity> m_Camera;

	entt::registry m_Registry;

	friend class Application;
	friend class Entity;
	friend struct Transform;
};
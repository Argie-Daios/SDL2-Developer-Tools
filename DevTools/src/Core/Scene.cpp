#include "Scene.h"

#include "ECS/Entity.h"

Scene::Scene()
{
	
}

Scene::Scene(const Scene& scene)
{
	// TODO
}

void Scene::AddEntity(const std::string& name)
{
	CreateRef<Entity>(name);
}

Entity Scene::GetEntity(const std::string& name)
{
	entt::entity entity = FindEntity(name);
	GAME_ASSERT(entity != entt::null, "Entity does not exist");

	return Entity{ entity };
}

entt::entity Scene::FindEntity(const std::string& name)
{
	auto view = m_Registry.view<Transform>();

	for (auto entity : view)
	{
		Entity en = { entity };
		if (en.name() == name)
		{
			return entity;
		}
	}

	return entt::null;
}
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

void Scene::RemoveEntity(const std::string& name)
{
	entt::entity entity = FindEntity(name);
	GAME_ASSERT(entity != entt::null, "Entity does not exist");

	m_Registry.destroy(entity);
}

Entity Scene::Instantiate(Entity object, const glm::vec2& position)
{
	Entity ent = object.CreateEntityAndCopyComponents();
	// Need to make Copy Constructors for every component
	ent.transform().SetPosition(position);

	return ent;
}

void Scene::DeleteEntity(Entity entity)
{
	GAME_ASSERT(m_Registry.valid(entity.handle()), "There is no such entity");

	m_Registry.destroy(entity.handle());
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
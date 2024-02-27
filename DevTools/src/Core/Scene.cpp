#include "Scene.h"

#include "ECS/Entity.h"

Scene::Scene()
{
	
}

Scene::Scene(const Scene& scene)
{
	// TODO
}

Entity Scene::AddEntity(const std::string& name)
{
	GAME_ASSERT(!nameExists(name), "There is already an entity with this name");
	Entity ent(m_Registry.create(), this);
	auto& informationComponent = ent.AddComponent<Information>();
	informationComponent.name = name;
	ent.AddComponent<Children>();
	ent.AddComponent<Transform>();

	return ent;
}

void Scene::RemoveEntity(const std::string& name)
{
	entt::entity entity = FindEntity(name);
	GAME_ASSERT(entity != entt::null, "Entity does not exist");

	m_Registry.destroy(entity);
}

Entity Scene::Instantiate(Entity object, const glm::vec2& position)
{
	Entity ent = Entity::CreateEntityWithComponentsOf(object);
	ent.transform().SetPosition(position);

	return ent;
}

void Scene::DeleteEntity(Entity entity)
{
	GAME_ASSERT(m_Registry.valid(entity.handle()), "There is no such entity");

	m_Registry.destroy(entity.handle());
}

Entity Scene::GetCamera() 
{ 
	return GetEntity("Camera"); 
}

Entity Scene::GetEntity(const std::string& name)
{
	entt::entity entity = FindEntity(name);
	GAME_ASSERT(entity != entt::null, "Entity does not exist");

	return Entity{ entity, this };
}

entt::entity Scene::FindEntity(const std::string& name)
{
	auto view = m_Registry.view<Transform>();

	for (auto entity : view)
	{
		Entity en = { entity, this };
		if (en.name() == name)
		{
			return entity;
		}
	}

	return entt::null;
}

bool Scene::nameExists(const std::string& name)
{
	auto view = m_Registry.view<Information>();
	for (auto entity : view)
	{
		Entity en = { entity, this };
		if (en.name() == name)
		{
			return true;
		}
	}

	return false;
}
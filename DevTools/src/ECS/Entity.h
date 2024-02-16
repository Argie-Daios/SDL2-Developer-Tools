#pragma once

#include "Core/Application.h"

#include "Components.h"

#define REGISTRY Application::Get()->m_Registry

class Entity;

class Entity
{
public:
	Entity()
	{
		m_EntityHandle = REGISTRY.create();

		AddComponent<Children>();
		AddComponent<Transform>();

		REGISTRY.sort<Transform>([](const Transform& left, const Transform& right) {return left.GetZValue() < right.GetZValue(); });
	}

	Entity(entt::entity handle)
	{
		m_EntityHandle = handle;
	}

	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		GAME_ASSERT(!HasComponent<T>(), "Entity already has this component!");
		 
		recentEntity = m_EntityHandle;

		T& component = REGISTRY.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

		return component;
	}

	template<typename T, typename... Args>
	T& AddorReplaceComponent(Args&&... args)
	{
		T& component = REGISTRY.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);

		return component;
	}

	template<typename T>
	T& AddIfNotExistsOrGet()
	{
		return (HasComponent<T>())? GetComponent<T>() : AddComponent<T>();
	}

	template<typename T>
	T& GetComponent() const
	{
		GAME_ASSERT(HasComponent<T>(), "Entity does not have this component!");

		return REGISTRY.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return REGISTRY.all_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		GAME_ASSERT(HasComponent<T>(), "Entity does not have this component!");

		REGISTRY.remove<T>(m_EntityHandle);
	}

	Transform& transform()
	{
		return GetComponent<Transform>();
	}

	void AddChild(entt::entity handle)
	{
		GAME_ASSERT(handle != m_EntityHandle, "An Entity cannot have child itself!");;

		GetComponent<Children>().children.push_back(handle);
	}

	entt::entity handle() { return m_EntityHandle; }

	static entt::entity recentEntity;
private:
	entt::entity m_EntityHandle = entt::null;
};
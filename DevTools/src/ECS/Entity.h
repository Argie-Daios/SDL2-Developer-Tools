#pragma once

#include "Core/Application.h"

#include "Components.h"

#define REGISTRY Application::GetCurrentScene()->m_Registry

class Entity
{
public:
	Entity(const std::string& name)
	{
		m_EntityHandle = REGISTRY.create();

		auto& childComponent = AddComponent<Children>();
		auto& infoComponent = AddComponent<Information>();
		auto& transformComponent = AddComponent<Transform>();

		SetName(name);

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

	std::string name()
	{
		return GetComponent<Information>().name;
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

	std::list<entt::entity> GetChildren()
	{
		return GetComponent<Children>().children;
	}

	void SetName(const std::string& name)
	{
		GAME_ASSERT(!nameExists(name), "Name already exists!");

		auto& infoComponent = GetComponent<Information>().name = name;
	}

	void AddTag(const std::string& tag)
	{
		auto& infoComponent = GetComponent<Information>();
		GAME_ASSERT(std::find(infoComponent.tags.begin(), infoComponent.tags.end(), tag) == infoComponent.tags.end(),
		"Entity " + infoComponent.name + " already contains this tag : (" + tag + ")");

		infoComponent.tags.push_back(tag);
	}

	void RemoveTag(const std::string& tag)
	{
		auto& infoComponent = GetComponent<Information>();
		auto it = std::find(infoComponent.tags.begin(), infoComponent.tags.end(), tag);
		GAME_ASSERT(it != infoComponent.tags.end(),
			"Entity " + infoComponent.name + " does not contain this tag : (" + tag + ")");

		infoComponent.tags.erase(it);
	}

	entt::entity handle() { return m_EntityHandle; }

	static entt::entity recentEntity;
private:
	struct Children
	{
		std::list<entt::entity> children;
	};

	struct Information
	{
		std::string name;
		std::vector<std::string> tags;
	};

	bool nameExists(const std::string& name)
	{
		auto view = REGISTRY.view<Information>();
		for (auto entity : view)
		{
			if (entity == m_EntityHandle) continue;

			Entity en = { entity };
			if (en.name() == name)
			{
				return true;
			}
		}

		return false;
	}

	entt::entity m_EntityHandle = entt::null;
};
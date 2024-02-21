#pragma once

#include "Core/Application.h"
#include "Components.h"

#define REGISTRY Application::GetCurrentScene()->m_Registry

class Entity
{
public:
	Entity() = default;
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

	Entity(const Entity& other)
	{
		m_EntityHandle = other.m_EntityHandle;

	}

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

	bool HasChild()
	{
		return HasComponent<Children>();
	}

	Entity CreateEntityAndCopyComponents()
	{
		std::string name = randomStringGenerator(30);
		Entity ent(name);
		ent.RemoveComponent<Children>();
		ent.RemoveComponent<Transform>();
		
		ent.CopyChildren(*this);
		ent.CopyTransform(*this);
		ent.CopySpriteRenderer(*this);
		ent.CopyAnimation(*this);
		ent.CopyText(*this);
		ent.CopyBehaviour(*this);
		ent.CopyCollider(*this);

		return ent;
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
		Children() {};
		Children(const Children& childrenComponent)
		{
			children = childrenComponent.children;
		}
		std::list<entt::entity> children;
	};

	struct Information
	{
		std::string name;
		std::vector<std::string> tags;
	};

	void CopyChildren(Entity ent)
	{
		if (!ent.HasComponent<Children>()) return;

		auto& childrenComponentSrc = ent.GetComponent<Children>();
		auto& childrenComponent = AddComponent<Children>();

		for (auto entity : childrenComponentSrc.children)
		{
			childrenComponent.children.push_back(entity);
		}
	}

	void CopyTransform(Entity ent)
	{
		if (!ent.HasComponent<Transform>()) return;

		auto& transformComponentSrc = ent.GetComponent<Transform>();

		auto position = transformComponentSrc.GetPosition();
		auto rotation = transformComponentSrc.GetRotation();
		auto scale = transformComponentSrc.GetScale();
		auto size = transformComponentSrc.GetSize();
		auto zValue = transformComponentSrc.GetZValue();
		auto flip = transformComponentSrc.GetFlip();

		auto& transformComponent = AddComponent<Transform>(position,
			zValue, rotation, scale);

		transformComponent.SetSize(size);
		transformComponent.SetFlip(flip);
	}

	void CopySpriteRenderer(Entity ent)
	{
		if (!ent.HasComponent<SpriteRenderer>()) return;

		auto& spriteRendererSrc = ent.GetComponent<SpriteRenderer>();

		auto texPath = spriteRendererSrc.GetTexturePath();
		auto color = spriteRendererSrc.GetColor();
		auto source = spriteRendererSrc.GetSource();

		auto& spriteRenderer = AddComponent<SpriteRenderer>(texPath);
		spriteRenderer.SetTintColor(color);
		spriteRenderer.SetSource(source);
	}

	void CopyAnimation(Entity ent)
	{
		if (!ent.HasComponent<Animation>()) return;

		auto& animationSrc = ent.GetComponent<Animation>();

		auto image_path = animationSrc.GetTexturePath();
		auto currentFrames = animationSrc.GetCurrentFrames();
		auto currentRow = animationSrc.GetCurrentRow();
		auto totalFrames = animationSrc.GetTotalFrames();
		auto totalRows = animationSrc.GetTotalRows();
		auto delay = animationSrc.GetDelay();
		auto loop = animationSrc.GetLoop();

		auto& animation = AddComponent<Animation>(image_path,
			currentFrames, currentRow, totalFrames, totalRows,
			delay, loop);
	}

	void CopyAnimator(Entity ent)
	{
		// TODO
	}

	void CopyText(Entity ent)
	{
		if (!ent.HasComponent<Text>()) return;

		auto& textSrc = ent.GetComponent<Text>();

		auto label = textSrc.GetLabel();
		auto font_path = textSrc.GetFontPath();
		auto font_size = textSrc.GetFontSize();
		auto color = textSrc.GetColor();

		auto& text = AddComponent<Text>(label, font_path, font_size, color);
	}

	void CopyBehaviour(Entity ent)
	{
		if (!ent.HasComponent<Behaviour>()) return;

		auto& behaviourSrc = ent.GetComponent<Behaviour>();
		auto& behaviour = AddComponent<Behaviour>();
		behaviour.Instance = nullptr;
		behaviour.InstantiateScript = behaviourSrc.InstantiateScript;
		behaviour.DestroyInstanceScript = behaviourSrc.DestroyInstanceScript;
	}

	void CopyCollider(Entity ent)
	{
		if (!ent.HasComponent<Collider>()) return;

		auto& colliderSrc = ent.GetComponent<Collider>();

		auto offset = colliderSrc.GetOffset();
		auto trigger = colliderSrc.GetTrigger();

		auto& collider = AddComponent<Collider>(offset, trigger);
	}

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
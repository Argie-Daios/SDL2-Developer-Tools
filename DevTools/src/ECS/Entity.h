#pragma once

#include "Core/Application.h"
#include "Components.h"

struct EntityStructure
{
	entt::entity handle = entt::null;
	Scene* scene = nullptr;
};

class Entity
{
public:
	Entity() = default;
	/*Entity(const std::string& name)
	{
		

		REGISTRY.sort<Transform>([](const Transform& left, const Transform& right) {return left.GetZValue() < right.GetZValue(); });
	}*/

	Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
		
	}

	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		GAME_ASSERT(!HasComponent<T>(), "Entity already has this component!");
		 
		recentEntity = { m_EntityHandle, m_Scene };

		T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

		return component;
	}

	template<typename T, typename... Args>
	T& AddorReplaceComponent(Args&&... args)
	{
		T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);

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

		return m_Scene->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		GAME_ASSERT(HasComponent<T>(), "Entity does not have this component!");

		m_Scene->m_Registry.remove<T>(m_EntityHandle);
	}

	bool HasChild()
	{
		return HasComponent<Children>();
	}

	Entity CreatePrefabAndCopyComponents()
	{
		std::string name = randomStringGenerator(30);
		Entity ent = AssetManager::s_PrefabDummyScene->AddEntity(name);
		ent.RemoveComponent<Children>();
		ent.RemoveComponent<Transform>();

		CopyChildren(*this, ent);
		CopyTransform(*this, ent);
		CopySpriteRenderer(*this, ent);
		CopyAnimation(*this, ent);
		CopyAnimator(*this, ent);
		CopyText(*this, ent);
		CopyBehaviour(*this, ent);
		CopyCollider(*this, ent);

		return ent;
	}
	
	static Entity CreateEntityWithComponentsOf(Entity entity)
	{
		std::string name = randomStringGenerator(30);
		Entity ent = Application::GetCurrentScene()->AddEntity(name);
		ent.RemoveComponent<Children>();
		ent.RemoveComponent<Transform>();

		CopyChildren(entity, ent);
		CopyTransform(entity, ent);
		CopySpriteRenderer(entity, ent);
		CopyAnimation(entity, ent);
		CopyAnimator(entity, ent);
		CopyText(entity, ent);
		CopyBehaviour(entity, ent);
		CopyCollider(entity, ent);

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
		GAME_ASSERT(!m_Scene->nameExists(name), "Name already exists!");

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

	static EntityStructure recentEntity;
private:
	static void CopyChildren(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Children>()) return;

		auto& childrenComponentSrc = entitySrc.GetComponent<Children>();
		auto& childrenComponent = entityDst.AddComponent<Children>();

		for (auto entity : childrenComponentSrc.children)
		{
			childrenComponent.children.push_back(entity);
		}
	}

	static void CopyTransform(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Transform>()) return;

		auto& transformComponentSrc = entitySrc.GetComponent<Transform>();

		auto position = transformComponentSrc.GetPosition();
		auto rotation = transformComponentSrc.GetRotation();
		auto scale = transformComponentSrc.GetScale();
		auto size = transformComponentSrc.GetSize();
		auto zValue = transformComponentSrc.GetZValue();
		auto flip = transformComponentSrc.GetFlip();

		auto& transformComponent = entityDst.AddComponent<Transform>(position, zValue, rotation, scale);

		transformComponent.SetSize(size);
		transformComponent.SetFlip(flip);
	}

	static void CopySpriteRenderer(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<SpriteRenderer>()) return;

		auto& spriteRendererSrc = entitySrc.GetComponent<SpriteRenderer>();

		auto texID = spriteRendererSrc.GetTextureID();
		auto color = spriteRendererSrc.GetColor();
		auto source = spriteRendererSrc.GetSource();

		auto& spriteRenderer = entityDst.AddComponent<SpriteRenderer>(texID);
		spriteRenderer.SetTintColor(color);
		spriteRenderer.SetSource(source);
	}

	static void CopyAnimation(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Animation>()) return;

		auto& animationSrc = entitySrc.GetComponent<Animation>();

		auto animationID = animationSrc.GetAnimationID();
		auto delay = animationSrc.GetDelay();
		auto loop = animationSrc.GetLoop();

		auto& animation = entityDst.AddComponent<Animation>(animationID, delay, loop);
	}

	static void CopyAnimator(Entity entitySrc, Entity entityDst)
	{
		// TODO
		if (!entitySrc.HasComponent<Animator>()) return;

		auto& animatorComponent = entityDst.AddComponent<Animator>();
		animatorComponent.Copy(entitySrc.GetComponent<Animator>());
	}

	static void CopyText(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Text>()) return;

		auto& textSrc = entitySrc.GetComponent<Text>();

		std::string name = randomStringGenerator(10);
		auto label = textSrc.GetLabel();
		auto font = textSrc.GetFont();
		auto color = textSrc.GetColor();

		auto& text = entityDst.AddComponent<Text>(name, label, font, color);
	}

	static void CopyBehaviour(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Behaviour>()) return;

		auto& behaviourSrc = entitySrc.GetComponent<Behaviour>();
		auto& behaviour = entityDst.AddComponent<Behaviour>();
		behaviour.Instance = nullptr;
		behaviour.InstantiateScript = behaviourSrc.InstantiateScript;
		behaviour.DestroyInstanceScript = behaviourSrc.DestroyInstanceScript;
	}

	static void CopyCollider(Entity entitySrc, Entity entityDst)
	{
		if (!entitySrc.HasComponent<Collider>()) return;

		auto& colliderSrc = entitySrc.GetComponent<Collider>();

		auto offset = colliderSrc.GetOffset();
		auto trigger = colliderSrc.GetTrigger();

		auto& collider = entityDst.AddComponent<Collider>(offset, trigger);
	}

	entt::entity m_EntityHandle = entt::null;
	Scene* m_Scene = nullptr;

	friend class AnimationController;
};
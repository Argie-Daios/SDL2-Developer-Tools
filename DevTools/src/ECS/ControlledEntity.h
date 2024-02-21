#pragma once

#include "Entity.h"

class ControlledEntity
{
public:
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return m_Entity.AddComponent<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return m_Entity.GetComponent<T>();
	}

	Entity Instantiate(Entity object, const glm::vec2& position)
	{
		return Application::GetCurrentScene()->Instantiate(object, position);
	}

	void DeleteEntity(Entity entity)
	{
		Application::GetCurrentScene()->DeleteEntity(entity);
	}

	Entity GetEntity(const std::string& name)
	{
		return Application::GetCurrentScene()->GetEntity(name);
	}

	Entity GameObject()
	{
		return m_Entity;
	}
protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
private:
	Entity m_Entity;
	friend class Application;
};
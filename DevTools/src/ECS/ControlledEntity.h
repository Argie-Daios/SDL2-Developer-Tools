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
protected:
	virtual void OnCreate() {}
	virtual void OnDestroy() {}
	virtual void OnUpdate() {}
private:
	Entity m_Entity;
	friend class Application;
};
#pragma once

#include "Core/Application.h"

class Game : public Application
{
public:
	Game();

	virtual void Update() override;
private:
	float cameraSpeed = 2.0f;

	Ref<Entity> m_Entity1;
	Ref<Entity> m_Entity2;
	Ref<Entity> m_Entity3;
	Ref<Entity> m_Entity4;
	Ref<Entity> m_Entity5;
};
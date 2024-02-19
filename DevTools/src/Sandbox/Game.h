#pragma once

#include "Core/Application.h"
#include "cpproutine/Coroutine.h"

using namespace cpproutine;

class Game : public Application
{
public:
	Game();

	virtual void Update() override;
private:
	IEnumerator Attack();
private:
	Ref<Entity> m_Entity1;
};
#pragma once

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"
#include "Tools/Time.h"
#include <cpproutine/Coroutine.h>

using namespace cpproutine;

class Rotated : public ControlledEntity
{
public:
	void OnCreate()
	{
		transform = &GetComponent<Transform>();
	}

	void OnUpdate()
	{
		transform->SetRotation(transform->GetRotation() + 1);
	}
private:
	Transform* transform;
};
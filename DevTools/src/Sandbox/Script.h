#pragma once

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"
#include <cpproutine/Coroutine.h>

using namespace cpproutine;

class Wizard : public ControlledEntity
{
public:
	void OnCreate()
	{
		transform = &GetComponent<Transform>();
		animator = &GetComponent<Animator>();
	}

	void OnUpdate()
	{
		if (Input::IsKeyPressed(Key::A))
		{
			animator->ChangeBoolParameterValue("Running", true);
			transform->SetFlip(SDL_FLIP_HORIZONTAL);
			transform->Translate(glm::vec2(-300.0f * Time::DeltaTime(), 0.0f));
		}
		if (Input::IsKeyPressed(Key::D))
		{
			animator->ChangeBoolParameterValue("Running", true);
			transform->SetFlip(SDL_FLIP_NONE);
			transform->Translate(glm::vec2(300.0f * Time::DeltaTime(), 0.0f));
		}
		if (!Input::IsKeyPressed(Key::A) && !Input::IsKeyPressed(Key::D))
		{
			animator->ChangeBoolParameterValue("Running", false);
		}
		if (Input::IsKeyDown(Key::SPACE))
		{
			CoroutineManager::StartCoroutine(Attack());
		}
	}

	IEnumerator Attack()
	{
		animator->ChangeBoolParameterValue("Attack", true);
		yield_return NewReturnType<WaitForSeconds>(0.1f * 8.0f);
		animator->ChangeBoolParameterValue("Attack", false);
	}
private:
	Transform* transform;
	Animator* animator;
};
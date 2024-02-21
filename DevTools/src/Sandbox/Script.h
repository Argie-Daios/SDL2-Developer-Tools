#pragma once

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Time.h"
#include <cpproutine/Coroutine.h>

using namespace cpproutine;

class Wizard : public ControlledEntity
{
public:
	void OnCreate()
	{
		transform = &GetComponent<Transform>();
		animator = &GetComponent<Animator>();
		X = GetEntity("X");
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
		if (Input::IsKeyDown(Key::SPACE) && !Attacking)
		{
			CoroutineManager::StartCoroutine(Attack());
		}
	}

	IEnumerator Attack()
	{
		animator->ChangeBoolParameterValue("Attack", true);
		Attacking = true;
		yield_return NewReturnType<WaitForSeconds>(0.1f * 7.0f);
		animator->ChangeBoolParameterValue("Attack", false);
		Entity ent = Instantiate(X, GetComponent<Transform>().GetPosition() + glm::vec2(400.0f, 220.0f));
		CoroutineManager::StartCoroutine(Shoot(ent));
		Attacking = false;
	}

	IEnumerator Shoot(Entity ent)
	{
		float duration = 2.0f;
		float elapsedTime = 0.0f;
		glm::vec2 src = ent.transform().GetPosition();
		glm::vec2 dst = ent.transform().GetPosition() + glm::vec2(700.0f, 0.0f);
		while (elapsedTime < duration)
		{
			glm::vec2 newPos = Math::Lerp(src, dst, elapsedTime / duration);
			ent.transform().SetPosition(newPos);
			elapsedTime += Time::DeltaTime();
			yield_return nullptr;
		}

		yield_return nullptr;
	}
private:
	Transform* transform;
	Animator* animator;
	Entity X;
	bool Attacking = false;
};
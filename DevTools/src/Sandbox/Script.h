#pragma once

#include "ECS/ControlledEntity.h"
#include "Tools/Input.h"
#include "Tools/Math.h"
#include "Tools/Time.h"
#include <cpproutine/Coroutine.h>

using namespace cpproutine;

enum class ABILITY
{
	FRIST,
	SECOND,
	THIRD
};

class Wizard : public ControlledEntity
{
public:
	void OnCreate()
	{
		transform = &GetComponent<Transform>();
		animator = &GetComponent<Animator>();
		vortexPrefab = AssetManager::Prefab("Prefab_Vortex");
	}

	void OnUpdate()
	{
		if (Input::IsKeyPressed(Key::A) && !Attacking)
		{
			animator->ChangeBoolParameterValue("Running", true);
			transform->SetFlip(SDL_FLIP_HORIZONTAL);
			transform->Translate(glm::vec2(-300.0f * Time::DeltaTime(), 0.0f));
		}
		if (Input::IsKeyPressed(Key::D) && !Attacking)
		{
			animator->ChangeBoolParameterValue("Running", true);
			transform->SetFlip(SDL_FLIP_NONE);
			transform->Translate(glm::vec2(300.0f * Time::DeltaTime(), 0.0f));
		}
		if (!Input::IsKeyPressed(Key::A) && !Input::IsKeyPressed(Key::D))
		{
			animator->ChangeBoolParameterValue("Running", false);
		}
		if (Input::IsKeyDown(Key::J) && !Attacking)
		{
			CoroutineManager::StartCoroutine(Attack(ABILITY::FRIST));
		}
		if (Input::IsKeyDown(Key::I) && !Attacking)
		{
			CoroutineManager::StartCoroutine(Attack(ABILITY::SECOND));
		}
		if (Input::IsKeyDown(Key::K) && !Attacking)
		{
			CoroutineManager::StartCoroutine(Attack(ABILITY::THIRD));
		}
	}

	IEnumerator Attack(ABILITY ability)
	{
		animator->ChangeBoolParameterValue("Attack", true);
		Attacking = true;
		yield_return NewReturnType<WaitForSeconds>(0.1f * 7.0f);
		animator->ChangeBoolParameterValue("Attack", false);

		switch (ability)
		{
		case ABILITY::FRIST:
		{
			Entity ent = Instantiate(vortexPrefab, transform->GetPosition() + glm::vec2(500, 200));
			CoroutineManager::StartCoroutine(Shoot(ent));
			break;
		}
		case ABILITY::SECOND:
		{
			CoroutineManager::StartCoroutine(Ability());
			break;
		}
		case ABILITY::THIRD:
		{
			Entity ent = Instantiate(vortexPrefab, transform->GetPosition() + glm::vec2(700, 200));
			CoroutineManager::StartCoroutine(Circle(ent));
			break;
		}
		}

		Attacking = false;
	}

	IEnumerator Shoot(Entity ent)
	{
		float duration = 0.02f * 35;
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
		DeleteEntity(ent);
		
		yield_return nullptr;
	}

	IEnumerator Ability()
	{
		glm::vec2 pos = transform->GetPosition();

		CoroutineManager::StartCoroutine(Summon(pos + glm::vec2(600, 200)));
		yield_return NewReturnType<WaitForSeconds>(0.5f);

		CoroutineManager::StartCoroutine(Summon(pos + glm::vec2(950, 200)));
		yield_return NewReturnType<WaitForSeconds>(0.5f);

		CoroutineManager::StartCoroutine(Summon(pos + glm::vec2(1300, 200)));
		yield_return NewReturnType<WaitForSeconds>(0.5f);
	}


	IEnumerator Summon(const glm::vec2& pos)
	{
		Entity ent = Instantiate(vortexPrefab, pos);
		yield_return NewReturnType<WaitUntil>([&]() {
			if (ent.GetComponent<Animation>().isComplete())
			{
				std::cout << "Complete" << std::endl;
				return true;
			}

			return false;
		});
		DeleteEntity(ent);
	}

	IEnumerator Circle(Entity ent)
	{
		float r = 250.0f;
		float angle = 0.0f;
		int circles = 3;

		glm::vec2 pos = ent.transform().GetPosition();

		while (angle < circles * 360.0f)
		{
			float x = r * glm::sin(glm::radians(angle));
			float y = r * glm::cos(glm::radians(angle));
			
			ent.transform().SetPosition(glm::vec2(x, y) + pos);
			angle += 300 * Time::DeltaTime();
			yield_return nullptr;
		}
		DeleteEntity(ent);

		yield_return nullptr;
	}
private:
	Transform* transform;
	Animator* animator;
	Entity vortexPrefab;
	bool Attacking = false;
};
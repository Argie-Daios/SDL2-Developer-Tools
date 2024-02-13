#include "Renderer.h"

#include "Core/Macros.h"
#include "ECS/Entity.h"
#include "Tools/Positioner.h"
#include "Core/Application.h"

#include <SDL_image.h>

SDL_Renderer* Renderer::s_Renderer = nullptr;

void Renderer::Init(SDL_Window* window)
{
	s_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
}

void Renderer::Shutdown()
{
	SDL_DestroyRenderer(s_Renderer);
}

void Renderer::Begin()
{
	SDL_RenderClear(s_Renderer);
}

void Renderer::Draw(entt::registry& reg)
{
	auto view = reg.view<Transform, Mesh>();

	for (auto entity : view)
	{
		Entity e = { entity };

		auto& transformComponent = e.GetComponent<Transform>();
		auto& meshComponent = e.GetComponent<Mesh>();

		if (meshComponent.GetTexture() == nullptr) continue;

		glm::vec2 position = transformComponent.GetPosition();
		glm::vec2 size = meshComponent.GetSize();

		SDL_Rect source = meshComponent.GetSource();

		auto cameraTransformComponent = Application::Get()->GetCamera()->GetComponent<Transform>();

		SDL_Rect destination = { (int)position.x - cameraTransformComponent.GetPosition().x,
			(int)position.y - cameraTransformComponent.GetPosition().y, (int)size.x, (int)size.y};
		destination.w *= transformComponent.GetScale().x;
		destination.h *= transformComponent.GetScale().y;

		SDL_RenderCopyEx(s_Renderer, meshComponent.GetTexture(), &source, &destination, 0, nullptr, transformComponent.GetFlip());
	}
}

void Renderer::End()
{
	SDL_RenderPresent(s_Renderer);
}

SDL_Texture* Renderer::CreateTexture(const std::string filepath)
{
	SDL_Texture* texture = IMG_LoadTexture(s_Renderer, filepath.c_str());

	GAME_ASSERT(texture, "Failed to load texture!!");

	return texture;
}

SDL_Texture* Renderer::CreateTextureFromSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(s_Renderer, surface);

	GAME_ASSERT(texture, "Failed to load texture!!");

	return texture;
}

glm::ivec2 Renderer::GetTextureDimensions(SDL_Texture* texture)
{
	glm::ivec2 dimensions;

	SDL_QueryTexture(texture, nullptr, nullptr, &dimensions.x, &dimensions.y);

	return dimensions;
}
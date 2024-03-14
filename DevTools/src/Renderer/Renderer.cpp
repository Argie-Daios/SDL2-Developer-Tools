#include "Renderer.h"

#include "Core/Macros.h"
#include "ECS/Entity.h"
#include "Tools/Positioner.h"
#include "Core/Application.h"

#include <SDL_image.h>

SDL_Renderer* Renderer::s_Renderer = nullptr;
SDL_Color Renderer::s_RenderDrawColor = SDL_Color{ 0, 0, 0, 255 };

void Renderer::Init(SDL_Window* window)
{
	s_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawColor(s_Renderer, s_RenderDrawColor.r, s_RenderDrawColor.g, s_RenderDrawColor.b, s_RenderDrawColor.a);
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
	for (auto entity : Application::GetCurrentScene()->m_EntitiesVector)
	{
		Entity e = { entity, Application::GetCurrentScene().get() };

		if (!e.HasComponent<SpriteRenderer>()) continue;

		auto& transformComponent = e.transform();
		auto& spriteComponent = e.GetComponent<SpriteRenderer>();

		glm::vec2 position = transformComponent.GetPosition();
		glm::vec2 size = transformComponent.GetSize();

		SDL_Rect source = spriteComponent.GetSource();

		auto cameraTransformComponent = Application::GetCurrentScene()->GetCamera().GetComponent<Transform>();

		// glm::vec2 centeredPos = position - (size * transformComponent.GetScale()) / glm::vec2(2.0f, 2.0f);

		SDL_Rect destination = { (int)(position.x - cameraTransformComponent.GetPosition().x),
			(int)(position.y - cameraTransformComponent.GetPosition().y),
			(int)(size.x * transformComponent.GetScale().x),
			(int)(size.y * transformComponent.GetScale().y) };

		SDL_Texture* texture = AssetManager::GetTexture(spriteComponent.GetTextureID()).texture;

		SDL_Color color = spriteComponent.GetSDLColor();
		if(color.r != 255 || color.g != 255 || color.b != 255)
			SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

		SDL_RenderCopyEx(s_Renderer, texture, &source, &destination, transformComponent.GetRotation(), nullptr, transformComponent.GetFlip());
	}
}

void Renderer::DrawQuad(SDL_Rect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, 255);

	SDL_RenderFillRect(s_Renderer, &rect);

	SDL_SetRenderDrawColor(s_Renderer, s_RenderDrawColor.r, s_RenderDrawColor.g, s_RenderDrawColor.b, s_RenderDrawColor.a);
}

void Renderer::DrawRect(SDL_Rect rect, SDL_Color color)
{
	SDL_SetRenderDrawColor(s_Renderer, color.r, color.g, color.b, 255);

	SDL_RenderDrawRect(s_Renderer, &rect);

	SDL_SetRenderDrawColor(s_Renderer, s_RenderDrawColor.r, s_RenderDrawColor.g, s_RenderDrawColor.b, s_RenderDrawColor.a);
}

void Renderer::End()
{
	SDL_RenderPresent(s_Renderer);
}

void Renderer::Resize(const glm::ivec2& dims)
{
	SDL_RenderSetViewport(s_Renderer, NULL);
	SDL_RenderSetLogicalSize(s_Renderer, dims.x, dims.y);
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
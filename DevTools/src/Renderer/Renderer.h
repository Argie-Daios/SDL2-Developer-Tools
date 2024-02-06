#pragma once

#include "Window.h"
#include "entt.hpp"

#include <glm/glm.hpp>

class Renderer
{
public:
	static void Init(SDL_Window* window);
	static void Shutdown();

	static void Begin();

	static void Draw(entt::registry& reg);

	static void End();

	static SDL_Texture* CreateTexture(const std::string filepath);
	static SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
	static glm::ivec2 GetTextureDimensions(SDL_Texture* texture);
private:
	static SDL_Renderer* s_Renderer;
};
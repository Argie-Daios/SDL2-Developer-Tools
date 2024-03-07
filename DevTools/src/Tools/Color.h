#pragma once

#include <glm/glm.hpp>

struct SDL_Color;

struct Color
{
	static const SDL_Color BLACK;
	static const SDL_Color WHITE;
	static const SDL_Color RED;
	static const SDL_Color BLUE;
	static const SDL_Color GREEN;
	static const SDL_Color ORANGE;
	static const SDL_Color YELLOW;
	static const SDL_Color PINK;
	static const SDL_Color PURPLE;

	static bool Compare(const SDL_Color& color1, const SDL_Color& color2);
};
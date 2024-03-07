#include "Color.h"

#include <SDL.h>

const SDL_Color Color::BLACK = SDL_Color(0, 0, 0);
const SDL_Color Color::WHITE = SDL_Color(255, 255, 255);
const SDL_Color Color::RED = SDL_Color(255, 0, 0);
const SDL_Color Color::BLUE = SDL_Color(0, 0, 255);
const SDL_Color Color::GREEN = SDL_Color(0, 255, 0);
const SDL_Color Color::ORANGE = SDL_Color(255, 165, 0);
const SDL_Color Color::YELLOW = SDL_Color(255, 255, 0);
const SDL_Color Color::PINK = SDL_Color(255, 192, 203);
const SDL_Color Color::PURPLE = SDL_Color(160, 32, 240);

bool Color::Compare(const SDL_Color& color1, const SDL_Color& color2)
{
	return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}
#pragma once

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>

class Window
{
public:
	Window(const std::string& title, int width, int height, int posX = SDL_WINDOWPOS_CENTERED, int posY = SDL_WINDOWPOS_CENTERED, Uint32 flags = 0);
	~Window();

	inline SDL_Window* Get() { return m_Window; }

	inline void SetDimensions(const glm::ivec2& dims) { m_Width = dims.x; m_Height = dims.y; }

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
private:
	SDL_Window* m_Window;

	int m_Width;
	int m_Height;
};
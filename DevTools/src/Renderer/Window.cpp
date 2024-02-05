#include "Window.h"

#include "Core/Macros.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

Window::Window(const std::string& title, int width, int height, int posX, int posY, Uint32 flags)
{
    GAME_ASSERT(SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize the SDL2 library");
    GAME_ASSERT(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG, "Failed to initialize the SDL2_image library");
    GAME_ASSERT(TTF_Init() != -1, "Failed to initialize the SDL2_ttf library");

    m_Window = SDL_CreateWindow(title.c_str(), posX, posY, width, height, flags);

    GAME_ASSERT(m_Window, "Failed to create window");

    m_Width = width;
    m_Height = height;
}

Window::~Window()
{
	SDL_DestroyWindow(m_Window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
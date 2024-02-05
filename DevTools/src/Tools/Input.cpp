#include "Input.h"

#include <iostream>

const Uint8* Input::s_Keystates = nullptr;
SDL_Event Input::s_Event = SDL_Event();

static SDL_Keycode ScancodeToKeycode(Key key)
{
	switch (key)
	{
	case Key::D: return SDLK_d;
	case Key::F: return SDLK_f;
	case Key::G: return SDLK_g;
	case Key::SPACE: return SDLK_SPACE;
	case Key::UP: return SDLK_UP;
	case Key::DOWN: return SDLK_DOWN;
	case Key::LEFT: return SDLK_LEFT;
	case Key::RIGHT: return SDLK_RIGHT;
	}
}

bool Input::IsKeyPressed(Key key)
{
	if (!s_Keystates)
		s_Keystates = SDL_GetKeyboardState(NULL);
	
	return s_Keystates[(SDL_Scancode)key];
}

bool Input::IsKeyDown(Key key)
{
	return s_Event.type == SDL_KEYDOWN && s_Event.key.repeat == 0 && s_Event.key.keysym.sym == ScancodeToKeycode(key);
}

bool Input::IsMouseButtonPressed(Mouse button)
{
	int mouseX, mouseY;
	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	switch (button) {
	case Mouse::Left:
		return (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
	case Mouse::Right:
		return (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
	case Mouse::Middle:
		return (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
	}

	return false;
}

bool Input::IsMouseButtonUp(Mouse button)
{
	return s_Event.type == SDL_MOUSEBUTTONUP && s_Event.button.button == (Uint8)button;
}

glm::ivec2 Input::GetMousePosition()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	return glm::ivec2(mouseX, mouseY);
}

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
	case Key::NUM0: return SDLK_KP_0;
	case Key::NUM1: return SDLK_KP_1;
	case Key::NUM2: return SDLK_KP_2;
	case Key::NUM3: return SDLK_KP_3;
	case Key::NUM4: return SDLK_KP_4;
	case Key::NUM5: return SDLK_KP_5;
	case Key::NUM6: return SDLK_KP_6;
	case Key::NUM7: return SDLK_KP_7;
	case Key::NUM8: return SDLK_KP_8;
	case Key::NUM9: return SDLK_KP_9;
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

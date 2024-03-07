#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

enum class Key
{
	A = SDL_SCANCODE_A,
	B = SDL_SCANCODE_B,
	C = SDL_SCANCODE_C,
	D = SDL_SCANCODE_D,
	E = SDL_SCANCODE_E,
	F = SDL_SCANCODE_F,
	G = SDL_SCANCODE_G,
	H = SDL_SCANCODE_H,
	I = SDL_SCANCODE_I,
	J = SDL_SCANCODE_J,
	K = SDL_SCANCODE_K,
	L = SDL_SCANCODE_L,
	M = SDL_SCANCODE_M,
	N = SDL_SCANCODE_N,
	O = SDL_SCANCODE_O,
	P = SDL_SCANCODE_P,
	Q = SDL_SCANCODE_Q,
	R = SDL_SCANCODE_R,
	S = SDL_SCANCODE_S,
	T = SDL_SCANCODE_T,
	U = SDL_SCANCODE_U,
	V = SDL_SCANCODE_V,
	W = SDL_SCANCODE_W,
	X = SDL_SCANCODE_X,
	Y = SDL_SCANCODE_Y,
	Z = SDL_SCANCODE_Z,
	SPACE = SDL_SCANCODE_SPACE,
	UP = SDL_SCANCODE_UP,
	DOWN = SDL_SCANCODE_DOWN,
	LEFT = SDL_SCANCODE_LEFT,
	RIGHT = SDL_SCANCODE_RIGHT,
	NUM0 = SDL_SCANCODE_KP_0,
	NUM1 = SDL_SCANCODE_KP_1,
	NUM2 = SDL_SCANCODE_KP_2,
	NUM3 = SDL_SCANCODE_KP_3,
	NUM4 = SDL_SCANCODE_KP_4,
	NUM5 = SDL_SCANCODE_KP_5,
	NUM6 = SDL_SCANCODE_KP_6,
	NUM7 = SDL_SCANCODE_KP_7,
	NUM8 = SDL_SCANCODE_KP_8,
	NUM9 = SDL_SCANCODE_KP_9
};

enum class Mouse
{
	Left = SDL_BUTTON_LEFT,
	Right = SDL_BUTTON_RIGHT,
	Middle = SDL_BUTTON_MIDDLE
};

class Input
{
public:
	static bool IsKeyPressed(Key key);
	static bool IsKeyDown(Key key);
	static bool IsKeyHold(Key key, float duration);
	static bool IsMouseButtonPressed(Mouse button);
	static bool IsMouseButtonUp(Mouse button);
	static glm::ivec2 GetMousePosition();
	static bool DoesMouseHover(const SDL_Rect& rect);
	static bool IsWindowResized();
	static glm::ivec2 GetWindowDimensions();

	static float GetTimePressed();
	static SDL_Event& Event() { return s_Event; }
private:
	static const Uint8* s_Keystates;
	static float s_PressedTime;
	static SDL_Event s_Event;
};
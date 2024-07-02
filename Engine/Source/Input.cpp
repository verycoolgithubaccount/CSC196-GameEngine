#include "Input.h"
#include <SDL.h>

bool Input::Initialize()
{
	int numKeys;
	const uint8_t* keyboardState = SDL_GetKeyboardState(&numKeys); // get amount of keys by passing in a reference and the current state of the keybord by the return

	m_keyboardState.resize(numKeys); // Resize the keyboard vector to the number of keys

	// Copy current keyboard state into m_keyboardState
	std::copy(keyboardState, keyboardState + numKeys, m_keyboardState.begin());

	m_prevKeyboardState = m_keyboardState;

	return true;
}

void Input::Shutdown()
{
}

void Input::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event); // Update SDL

	// KEYBOARD INPUT
	m_prevKeyboardState = m_keyboardState;

	const uint8_t* keyboardState = SDL_GetKeyboardState(nullptr); // Pass in a nullptr because we don't need to get the number of keys
	std::copy(keyboardState, keyboardState + m_keyboardState.size(), m_keyboardState.begin());

	// MOUSE INPUT
	int x, y;
	uint32_t buttonState = SDL_GetMouseState(&x, &y); // need to pass in 2 references for it to change x and y mouse position
	// ^ Returns which mouse buttons are being pressed by representing each button with a bit (001 if left button, 101 if both left and right, etc)

	// 000 <- button state -> 001
	// 001 <- button mask  -> 001
	// &? no, 000             &? yes, 001 

	m_mousePosition = { x, y };

	m_prevMouseButtonState = m_mouseButtonState;

	m_mouseButtonState[0] = buttonState & SDL_BUTTON_LMASK; // Use a bit mask to check a specific bit in the variable
	m_mouseButtonState[1] = buttonState & SDL_BUTTON_MMASK;
	m_mouseButtonState[2] = buttonState & SDL_BUTTON_RMASK;
}

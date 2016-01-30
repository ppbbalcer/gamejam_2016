#include "PlayerInput.h"
#include <SDL.h>



/*
#define PLAYER_1_MOVE_UP	SDL_SCANCODE_UP
#define PLAYER_1_MOVE_DOWN	SDL_SCANCODE_DOWN
#define PLAYER_1_MOVE_LEFT	SDL_SCANCODE_LEFT
#define PLAYER_1_MOVE_RIGHT	SDL_SCANCODE_RIGHT
#define PLAYER_1_SHOOT		SDL_SCANCODE_RCTRL
#define PLAYER_1_USE		SDL_SCANCODE_RALT

#define PLAYER_2_MOVE_UP	SDL_SCANCODE_W
#define PLAYER_2_MOVE_DOWN	SDL_SCANCODE_S
#define PLAYER_2_MOVE_LEFT	SDL_SCANCODE_A
#define PLAYER_2_MOVE_RIGHT	SDL_SCANCODE_D
#define PLAYER_2_SHOOT		SDL_SCANCODE_F
#define PLAYER_2_USE		SDL_SCANCODE_G
*/

PlayerInput::PlayerInput() {
	setBinding(PLAYER_1_MOVE_UP, SDL_SCANCODE_UP);
	setBinding(PLAYER_1_MOVE_DOWN, SDL_SCANCODE_DOWN);
	setBinding(PLAYER_1_MOVE_LEFT, SDL_SCANCODE_LEFT);
	setBinding(PLAYER_1_MOVE_RIGHT, SDL_SCANCODE_RIGHT);
	setBinding(PLAYER_1_SHOOT, SDL_SCANCODE_RCTRL);
	setBinding(PLAYER_1_USE, SDL_SCANCODE_RALT);
}

void PlayerInput::update() {
	const unsigned char *currentKeyStates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		input_state[i] = currentKeyStates[input_bindings[i]];
	}
}


int PlayerInput::getState(enum input_type type) {
	return input_state[type];
}

void PlayerInput::setBinding(enum input_type type, unsigned char key) {
	input_bindings[type] = key;
}

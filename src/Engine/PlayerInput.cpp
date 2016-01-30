#include "PlayerInput.h"
#include <SDL.h>

PlayerInput::PlayerInput() {
	setBinding(GAME_QUIT, SDL_SCANCODE_ESCAPE);
	setBinding(GAME_RESET, SDL_SCANCODE_R);
	setBinding(PLAYER_1_MOVE_UP, SDL_SCANCODE_UP);
	setBinding(PLAYER_1_MOVE_DOWN, SDL_SCANCODE_DOWN);
	setBinding(PLAYER_1_MOVE_LEFT, SDL_SCANCODE_LEFT);
	setBinding(PLAYER_1_MOVE_RIGHT, SDL_SCANCODE_RIGHT);
	setBinding(PLAYER_1_SHOOT, SDL_SCANCODE_RCTRL);
	setBinding(PLAYER_1_USE, SDL_SCANCODE_RALT);
}

void PlayerInput::addController(int id) {
	if (SDL_IsGameController(id)) {
		SDL_GameController *gc = SDL_GameControllerOpen(id);

		if (gc) {
			//SDL_Joystick *j = SDL_GameControllerGetJoystick(gc);
			//int instance = SDL_JoystickInstanceID(j);
			controllers[id] = gc;
		}
		printf("add gc\n");
	}
}

void PlayerInput::removeController(int id) {
	SDL_GameController *gc = controllers[id];
	SDL_GameControllerClose(gc);
	printf("remove gc\n");
}

void PlayerInput::handleControllerButton(const SDL_ControllerButtonEvent sdlEvent) {
	printf("gc button press\n");
}

void PlayerInput::handleControllerAxis(const SDL_ControllerAxisEvent sdlEvent) {
	printf("gc axis move\n");
}

void PlayerInput::update() {
	const unsigned char *currentKeyStates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		input_state[i] = currentKeyStates[input_bindings[i]];
	}


	SDL_Event e;
	while( SDL_PollEvent( &e ) ) {
		switch( e.type ) {
		case SDL_QUIT:
			input_state[GAME_QUIT] = 1;
		break;
		case SDL_CONTROLLERDEVICEADDED:
			addController(e.cdevice.which);
		break;
		case SDL_CONTROLLERDEVICEREMOVED:
			removeController(e.cdevice.which);
		break;
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			handleControllerButton(e.cbutton);
		break;
		case SDL_CONTROLLERAXISMOTION:
			handleControllerAxis(e.caxis);
		break;
		}
	}

}


int PlayerInput::getState(enum input_type type) {
	return input_state[type];
}

void PlayerInput::setBinding(enum input_type type, unsigned char key) {
	input_bindings[type] = key;
}

#include "PlayerInput.h"
#include <SDL.h>

PlayerInput::PlayerInput() {
	setKeyboardBinding(GAME_QUIT, SDL_SCANCODE_ESCAPE);
	setKeyboardBinding(GAME_RESET, SDL_SCANCODE_R);
	setKeyboardBinding(PLAYER_1_MOVE_UP, SDL_SCANCODE_UP);
	setKeyboardBinding(PLAYER_1_MOVE_DOWN, SDL_SCANCODE_DOWN);
	setKeyboardBinding(PLAYER_1_MOVE_LEFT, SDL_SCANCODE_LEFT);
	setKeyboardBinding(PLAYER_1_MOVE_RIGHT, SDL_SCANCODE_RIGHT);
	setKeyboardBinding(PLAYER_1_SHOOT, SDL_SCANCODE_RCTRL);
	setKeyboardBinding(PLAYER_1_USE, SDL_SCANCODE_RALT);

	setControllerBinding(GAME_QUIT, SDL_CONTROLLER_BUTTON_INVALID);
	setControllerBinding(GAME_RESET, SDL_CONTROLLER_BUTTON_INVALID);
	setControllerBinding(PLAYER_1_MOVE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	setControllerBinding(PLAYER_1_MOVE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	setControllerBinding(PLAYER_1_MOVE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	setControllerBinding(PLAYER_1_MOVE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	setControllerBinding(PLAYER_1_SHOOT, SDL_CONTROLLER_BUTTON_A);
	setControllerBinding(PLAYER_1_USE, SDL_CONTROLLER_BUTTON_B);
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
	// unused
}

void PlayerInput::handleControllerAxis(const SDL_ControllerAxisEvent sdlEvent) {
	// unused
}

void PlayerInput::update() {
	const unsigned char *currentKeyStates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		input_state[i] = currentKeyStates[keyboard_input_bindings[i]];
	}

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		for (std::pair<int, SDL_GameController *> gc : controllers) {
			input_state[i] = SDL_GameControllerGetButton(gc.second, controller_input_bindings[i]);
		}
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

void PlayerInput::setKeyboardBinding(enum input_type type, unsigned char key) {
	keyboard_input_bindings[type] = key;
}

void PlayerInput::setControllerBinding(enum input_type type, SDL_GameControllerButton btn) {
	controller_input_bindings[type] = btn;
}

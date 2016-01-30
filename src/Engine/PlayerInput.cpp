#include "PlayerInput.h"
#include <SDL.h>

#define DEFAULT_INPUT_DELAY 50

PlayerInput::PlayerInput() {
	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		input_delay[i].current = 0;
		input_delay[i].delay = -1;
	}

	setKeyboardBinding(INPUT_MENU_UP, SDL_SCANCODE_UP);
	setKeyboardBinding(INPUT_MENU_DOWN, SDL_SCANCODE_DOWN);
	setKeyboardBinding(INPUT_MENU_LEFT, SDL_SCANCODE_LEFT);
	setKeyboardBinding(INPUT_MENU_RIGHT, SDL_SCANCODE_RIGHT);
	setKeyboardBinding(INPUT_MENU_ENTER, SDL_SCANCODE_RETURN);
	setKeyboardBinding(INPUT_MENU_BACK, SDL_SCANCODE_BACKSPACE);

	setControllerBinding(INPUT_MENU_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	setControllerBinding(INPUT_MENU_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	setControllerBinding(INPUT_MENU_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	setControllerBinding(INPUT_MENU_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	setControllerBinding(INPUT_MENU_ENTER, SDL_CONTROLLER_BUTTON_A);
	setControllerBinding(INPUT_MENU_BACK, SDL_CONTROLLER_BUTTON_B);

	setKeyboardBinding(INPUT_GAME_QUIT, SDL_SCANCODE_ESCAPE);
	setKeyboardBinding(INPUT_GAME_RESET, SDL_SCANCODE_R);
	setKeyboardBinding(INPUT_MOVE_UP, SDL_SCANCODE_UP);
	setKeyboardBinding(INPUT_MOVE_DOWN, SDL_SCANCODE_DOWN);
	setKeyboardBinding(INPUT_MOVE_LEFT, SDL_SCANCODE_LEFT);
	setKeyboardBinding(INPUT_MOVE_RIGHT, SDL_SCANCODE_RIGHT);
	setKeyboardBinding(INPUT_SHOOT, SDL_SCANCODE_RCTRL);
	setKeyboardBinding(INPUT_USE, SDL_SCANCODE_RALT);

	setControllerBinding(INPUT_GAME_QUIT, SDL_CONTROLLER_BUTTON_INVALID);
	setControllerBinding(INPUT_GAME_RESET, SDL_CONTROLLER_BUTTON_INVALID);
	setControllerBinding(INPUT_MOVE_UP, SDL_CONTROLLER_BUTTON_DPAD_UP);
	setControllerBinding(INPUT_MOVE_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	setControllerBinding(INPUT_MOVE_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	setControllerBinding(INPUT_MOVE_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	setControllerBinding(INPUT_SHOOT, SDL_CONTROLLER_BUTTON_A);
	setControllerBinding(INPUT_USE, SDL_CONTROLLER_BUTTON_B);

	setInputDelay(INPUT_MENU_DOWN, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_MENU_UP, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_MENU_LEFT, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_MENU_RIGHT, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_MENU_ENTER, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_MENU_BACK, DEFAULT_INPUT_DELAY);
	setInputDelay(INPUT_USE, DEFAULT_INPUT_DELAY);
}

void PlayerInput::reset() {
	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		input_state[i] = 0;
	}
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

void PlayerInput::setInputDelay(enum input_type type, int time)
{
	input_delay[type].delay = time;
}

void PlayerInput::update(int time) {
	const unsigned char *currentKeyStates = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		if (input_delay[i].delay != -1) {
			input_delay[i].current -= time;
			if (input_delay[i].current < 0) {
				input_state[i] = currentKeyStates[keyboard_input_bindings[i]];
				input_delay[i].current = input_delay[i].delay;
			}
		} else {
			input_state[i] = currentKeyStates[keyboard_input_bindings[i]];
		}
	}

	for (int i = 0; i < MAX_INPUT_TYPE; ++i) {
		if (input_state[i])
			continue;

		for (std::pair<int, SDL_GameController *> gc : controllers) {
			if (input_delay[i].delay != -1) {
				input_delay[i].current -= time;
				if (input_delay[i].current < 0) {
					input_state[i] = SDL_GameControllerGetButton(gc.second, controller_input_bindings[i]);
					input_delay[i].current = input_delay[i].delay;
				}
			} else {
				input_state[i] = SDL_GameControllerGetButton(gc.second, controller_input_bindings[i]);
			}
		}
	}

	SDL_Event e;
	while( SDL_PollEvent( &e ) ) {
		switch( e.type ) {
		case SDL_QUIT:
			input_state[INPUT_GAME_QUIT] = 1;
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

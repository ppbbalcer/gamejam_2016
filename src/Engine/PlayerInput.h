#ifndef __PLAYER_INPUT_H
#define __PLAYER_INPUT_H

#include <map>
#include <SDL.h>

enum input_type {
	/* general inputs */
	INPUT_GAME_QUIT,
	INPUT_GAME_RESET,

	/* menu scene inputs */
	INPUT_MENU_UP,
	INPUT_MENU_DOWN,
	INPUT_MENU_LEFT,
	INPUT_MENU_RIGHT,
	INPUT_MENU_ENTER,
	INPUT_MENU_BACK,

	/* game scene inputs */
	INPUT_MOVE_UP,
	INPUT_MOVE_DOWN,
	INPUT_MOVE_LEFT,
	INPUT_MOVE_RIGHT,
	INPUT_SHOOT,
	INPUT_USE,

	MAX_INPUT_TYPE
};

class PlayerInput {
public:
	PlayerInput();
	~PlayerInput() {};

	void update(int time);
	void reset();

	int getState(enum input_type type);
	void setKeyboardBinding(enum input_type type, unsigned char key);
	void setControllerBinding(enum input_type type, SDL_GameControllerButton btn);
	void setInputDelay(enum input_type type, int time);

private:
	int input_state[MAX_INPUT_TYPE];
	struct {
		int current;
		int delay;
	} input_delay[MAX_INPUT_TYPE];
	unsigned char keyboard_input_bindings[MAX_INPUT_TYPE];
	SDL_GameControllerButton controller_input_bindings[MAX_INPUT_TYPE];

	std::map<int, SDL_GameController*> controllers;

	void addController(int id);
	void removeController(int id);
	void handleControllerButton(const SDL_ControllerButtonEvent sdlEvent);
	void handleControllerAxis(const SDL_ControllerAxisEvent sdlEvent);
};

#endif

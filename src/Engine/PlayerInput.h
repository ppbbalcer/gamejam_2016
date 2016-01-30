#ifndef __PLAYER_INPUT_H
#define __PLAYER_INPUT_H

#include <map>
#include <SDL.h>

enum input_type {
	/* general inputs */
	GAME_QUIT,
	GAME_RESET,

	/* game scene inputs */
	PLAYER_1_MOVE_UP,
	PLAYER_1_MOVE_DOWN,
	PLAYER_1_MOVE_LEFT,
	PLAYER_1_MOVE_RIGHT,
	PLAYER_1_SHOOT,
	PLAYER_1_USE,

	MAX_INPUT_TYPE
};

class PlayerInput {
public:
	PlayerInput();
	~PlayerInput() {};

	void update();

	int getState(enum input_type type);
	void setBinding(enum input_type type, unsigned char key);

private:
	int input_state[MAX_INPUT_TYPE];
	unsigned char input_bindings[MAX_INPUT_TYPE];

	std::map<int, SDL_GameController*> controllers;

	void addController(int id);
	void removeController(int id);
	void handleControllerButton(const SDL_ControllerButtonEvent sdlEvent);
	void handleControllerAxis(const SDL_ControllerAxisEvent sdlEvent);
};

#endif

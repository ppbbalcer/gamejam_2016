#ifndef __PLAYER_INPUT_H
#define __PLAYER_INPUT_H

enum input_type {
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
};

#endif

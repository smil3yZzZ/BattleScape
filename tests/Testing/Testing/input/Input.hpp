#pragma once
#ifndef INPUT_H
#define INPUT_H

#define NUM_KEYS 6

#define INPUT_ESC 0
#define INPUT_UP 1
#define INPUT_RIGHT 2
#define INPUT_DOWN 3
#define INPUT_LEFT 4
#define INPUT_SPACE 5

#endif

class Input {
private:
	int keys[NUM_KEYS];

public:
	Input();
	int getKeyState(int key);
	void setKeyState(int key, int state);
};

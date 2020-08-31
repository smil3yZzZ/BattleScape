#pragma once
#ifndef INPUT_H
#define INPUT_H
#endif

#define INPUT_ESC 0
#define INPUT_UP 1
#define INPUT_RIGHT 2
#define INPUT_DOWN 3
#define INPUT_LEFT 4
#define INPUT_SPACE 5

class Input {
private:
	int keys[6];

public:
	Input();
	int getKeyState(int key);
	void setKeyState(int key, int state);
};


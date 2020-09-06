#include "Input.hpp"

Input::Input() {
	/*
	int keys[6] = {
		0, //INPUT_ESC
		0, //INPUT_UP
		0, //INPUT_RIGHT
		0, //INPUT_DOWN
		0, //INPUT_LEFT
		0  //INPUT_SPACE
	};
	*/
	Input::keys[INPUT_ESC] = 0;
	Input::keys[INPUT_UP] = 0; 
	Input::keys[INPUT_RIGHT] = 0; 
	Input::keys[INPUT_DOWN] = 0; 
	Input::keys[INPUT_LEFT] = 0; 
	Input::keys[INPUT_SPACE] = 0;
}

void Input::setKeyState(int key, int state) {
	keys[key] = state;
}

int Input::getKeyState(int key) {
	return keys[key];
}
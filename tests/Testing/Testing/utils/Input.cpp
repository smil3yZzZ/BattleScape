#include "Input.hpp"

Input::Input() {
	keys = {
		0, //CONTROL_ESC
		0, //CONTROL_UP
		0, //CONTROL_RIGHT
		0, //CONTROL_DOWN
		0, //CONTROL_LEFT
		0  //CONTROL_SPACE
	}
}

void Input::setKeyState(int key, int state) {
	keys[key] = state;
}

int Input::getKeyState(int key) {
	return keys[key];
}
#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
/*
#include "utils/SquareProperties.hpp"
#include "engine/Engine.hpp"

#include <map>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <tuple>
#include <stack>
#include <ctime>
#include <vector>
#include <chrono>
*/

#endif

/* MAIN CONSTANTS */

const int S = 0;
const int M = 1;
const int L = 2;
const int XL = 3;
const int XXL = 4;
const int BORDERS_SIZE_IN_MATRIX = 2;
const int FRAMES_PER_SECOND = 60;

const float X_ORIGIN = -100.0f;
const float Y_ORIGIN = -50.0f;

const int TOP_WALL_SHIFT = 6;
const int RIGHT_WALL_SHIFT = 5;
const int BOTTOM_RIGHT_WALL_SHIFT = 4;
const int BOTTOM_WALL_SHIFT = 3;
const int BOTTOM_LEFT_WALL_SHIFT = 2;
const int LEFT_WALL_SHIFT = 1;

//The result of (size - 1)/2 has to be odd

const std::map<int, int> size = { {S, 11}, {M, 19}, {L, 39}, {XL, 71}, {XXL, 155} };

/* ENGINE CONSTANTS */

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float VIEWPORT_WIDTH = 200.0f;
const float VIEWPORT_HEIGHT = 150.0f;
const float Z_NEAR = 0.0f;
const float Z_FAR = 1000.0f;

const int NUMBER_OF_RGBA_CHANNELS = 4;
const int PLATFORM_BUFFER_VERTEX_SIZE = 5;
const int WALL_BUFFER_VERTEX_SIZE = 5;
const int PLATFORM_VERTICES_PER_QUAD = 4;
const int WALL_VERTICES_PER_QUAD = 8;
const int PLATFORM_INDICES_PER_QUAD = 6;
const int WALL_INDICES_PER_QUAD = 12;

const int QUAD_WIDTH = 64;
const int QUAD_HEIGHT = 64;

const int PLATFORM_TEXTURE_ROWS = 9;
const int PLATFORM_TEXTURE_COLS = 1;

const int WALL_TEXTURE_ROWS = 1;
const int WALL_TEXTURE_COLS = 16;

const char* const SQUARE_VERTEX_SHADER_PATH = "resources/shaders/mapQuad.vs";
const char* const SQUARE_FRAGMENT_SHADER_PATH = "resources/shaders/mapQuad.fs";

const char* const WALL_TEXTURE_PATH = "resources/wall_sampled.png";

const char* const CHARACTER_TEXTURE_PATH = "resources/character/mario_example_tests_4_transparent_big.png";
const int CHARACTER_TEXTURE_ROWS = 4;
const int CHARACTER_TEXTURE_COLS = 5;
const int CHARACTER_BUFFER_VERTEX_SIZE = 5;
const int CHARACTER_VERTICES_PER_QUAD = 4;
const int CHARACTER_INDICES_PER_QUAD = 6;

const float CHARACTER_INITIAL_X = 100.0f;
const float CHARACTER_INITIAL_Y = 100.0f;

const char* const CHARACTER_VERTEX_SHADER_PATH = "resources/shaders/character.vs";
const char* const CHARACTER_FRAGMENT_SHADER_PATH = "resources/shaders/character.fs";

const float PLATFORMS_Z_OFFSET = 5.0f;
const float WALLS_Z_OFFSET = 10.0f;
const float CHARACTER_Z_OFFSET = 15.0f;

const int SPRITE_DRAWING_OBJECT_DIMENSION = 1;

/* CHARACTER MOVEMENT CONSTANTS */

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;
const int UP_MOVEMENT = 4;
const int UP_RIGHT_MOVEMENT = 5;
const int RIGHT_MOVEMENT = 6;
const int RIGHT_DOWN_MOVEMENT = 7;
const int DOWN_MOVEMENT = 8;
const int DOWN_LEFT_MOVEMENT = 9;
const int LEFT_MOVEMENT = 10;
const int LEFT_UP_MOVEMENT = 11;

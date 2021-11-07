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
const int FRAMES_PER_SECOND = 120;

const float X_ORIGIN = 0.0f;
const float Y_ORIGIN = 0.0f;

const int TOP_WALL_SHIFT = 6;
const int RIGHT_WALL_SHIFT = 5;
const int BOTTOM_RIGHT_WALL_SHIFT = 4;
const int BOTTOM_WALL_SHIFT = 3;
const int BOTTOM_LEFT_WALL_SHIFT = 2;
const int LEFT_WALL_SHIFT = 1;

//The result of (size - 1)/2 has to be odd

const std::map<int, int> size = { {S, 11}, {M, 19}, {L, 39}, {XL, 71}, {XXL, 155} };
/* ENGINE CONSTANTS */

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
const float VIEWPORT_WIDTH = 320.0f;
const float VIEWPORT_HEIGHT = 200.0f;
const float Z_NEAR = 0.0f;
const float Z_FAR = 10000.0f;

const int NUMBER_OF_RGBA_CHANNELS = 4;

const int PLATFORM_BUFFER_VERTEX_SIZE = 5;
const int SHADOW_BUFFER_VERTEX_SIZE = 5;
const int WALL_BUFFER_VERTEX_SIZE = 5;
const int FOG_BUFFER_VERTEX_SIZE = 3;

const int PLATFORM_VERTICES_PER_QUAD = 4;
const int SHADOW_VERTICES_PER_QUAD = 4;
const int WALL_VERTICES_PER_QUAD = 8;
const int FOG_VERTICES_PER_QUAD = 4;

const int PLATFORM_INDICES_PER_QUAD = 6;
const int SHADOW_INDICES_PER_QUAD = 6;
const int WALL_INDICES_PER_QUAD = 12;
const int FOG_INDICES_PER_QUAD = 6;

const int WALL_TEXTURE_LOAD_SUBSAMPLING_FACTOR = 16;
const int SPRITE_TEXTURE_LOAD_SUBSAMPLING_FACTOR = 4;

const int ORIGINAL_QUAD_WIDTH = 256;
const int ORIGINAL_QUAD_HEIGHT = 256;

const int ORIGINAL_SHADOW_WIDTH = 140;
const int ORIGINAL_SHADOW_HEIGHT = 15;

const int QUAD_WIDTH = 64;
const int QUAD_HEIGHT = 64;

const int PLATFORM_TEXTURE_ROWS = 9;
const int PLATFORM_TEXTURE_COLS = 1;

const int WALL_TEXTURE_ROWS = 1;
const int WALL_TEXTURE_COLS = 16;

const float MOVEMENT_SPEED = 1.0f;

const char* const SQUARE_VERTEX_SHADER_PATH = "resources/shaders/mapQuad.vs";
const char* const SQUARE_FRAGMENT_SHADER_PATH = "resources/shaders/mapQuad.fs";

const char* const WALL_TEXTURE_PATH = "resources/wall_sampled_awesome.png";

const char* const CHARACTER_TEXTURE_PATH = "resources/character/mario_example_tests_4_transparent_awesome_inverted_clamped.png";
const int CHARACTER_TEXTURE_ROWS = 4;
const int CHARACTER_TEXTURE_COLS = 5;
const int CHARACTER_BUFFER_VERTEX_SIZE = 5;
const int CHARACTER_VERTICES_PER_QUAD = 4;
const int CHARACTER_INDICES_PER_QUAD = 6;

const float CHARACTER_INITIAL_X = 64.0f;
const float CHARACTER_INITIAL_Y = 64.0f;

const char* const CHARACTER_VERTEX_SHADER_PATH = "resources/shaders/character.vs";
const char* const CHARACTER_FRAGMENT_SHADER_PATH = "resources/shaders/character.fs";
const char* const CHARACTER_SHADOW_FRAGMENT_SHADER_PATH = "resources/shaders/ellipsis.fs";

const char* const FOG_VERTEX_SHADER_PATH = "resources/shaders/fog.vs";
const char* const FOG_FRAGMENT_SHADER_PATH = "resources/shaders/fog.fs";

const float PLATFORMS_Z_OFFSET = 5.0f;
const float WALLS_Z_OFFSET = 10.0f;
const float CHARACTER_SHADOW_Z_OFFSET = 8.0f;
const float CHARACTER_Z_OFFSET = 15.0f;
const float FOG_Z_OFFSET = 50.0f;
const int SPRITE_DRAWING_OBJECT_DIMENSION = 1;

const float UNITARY_COMPONENT = 1.0f;
const float CAMERA_Z_POS = 100.0f;

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

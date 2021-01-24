#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "utils/SquareProperties.hpp"
#include "Engine.hpp"

#include <map>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <tuple>
#include <stack>
#include <ctime>
#include <vector>
#include <chrono>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float VIEWPORT_WIDTH = 400.0f;
const float VIEWPORT_HEIGHT = 300.0f;
const float X_ORIGIN = -100.0f;
const float Y_ORIGIN = -50.0f;

const int S = 0;
const int M = 1;
const int L = 2;
const int XL = 3;
const int XXL = 4;
const int BORDERS_SIZE_IN_MATRIX = 2;
const int FRAMES_PER_SECOND = 60;

const int NUMBER_OF_PLATFORMS = 8;
const int NUMBER_OF_RGBA_CHANNELS = 4;
const int PLATFORM_BUFFER_VERTEX_SIZE = 5;
const int WALL_BUFFER_VERTEX_SIZE = 8;
const int PLATFORM_INDICES_PER_QUAD = 6;
const int WALL_INDICES_PER_QUAD = 12;
const int PLATFORM_VERTICES_PER_QUAD = 4;
const int WALL_VERTICES_PER_QUAD = 8;
const int QUAD_WIDTH = 64;
const int QUAD_HEIGHT = 64;

const int TOP_WALL_SHIFT = 6;
const int RIGHT_WALL_SHIFT = 5;
const int BOTTOM_RIGHT_WALL_SHIFT = 4;
const int BOTTOM_WALL_SHIFT = 3;
const int BOTTOM_LEFT_WALL_SHIFT = 2;
const int LEFT_WALL_SHIFT = 1;

//The result of (size - 1)/2 has to be odd

const std::map<int, int> size = { {S, 11}, {M, 19}, {L, 39}, {XL, 71}, {XXL, 155} };

#endif

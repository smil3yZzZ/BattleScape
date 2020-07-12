#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <map>
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <stack>
#include <ctime>
#include <vector>
#include <chrono>

const int S = 0;
const int M = 1;
const int L = 2;
const int XL = 3;
const int XXL = 4;
const int BORDERS_SIZE_IN_MATRIX = 2;
const int FRAMES_PER_SECOND = 60;



//The result of (size - 1)/2 has to be odd

const std::map<int, int> size = { {S, 11}, {M, 19}, {L, 39}, {XL, 71}, {XXL, 155} };

#endif

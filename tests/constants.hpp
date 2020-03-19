#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <iostream>
#include <cstdlib>
#include <tuple>
#include <stack>

const int SMALL = 0;
const int MEDIUM = 1;
const int BIG = 2;
const int ENORMOUS = 3;
const std::map<int,int> size = {{SMALL, 11}, {MEDIUM, 19}, {BIG, 37}, {ENORMOUS, 73}};

#endif

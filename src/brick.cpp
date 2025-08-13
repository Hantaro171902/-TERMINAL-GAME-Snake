#include "brick.cpp"
#include "ultils.hpp"
#include "color.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

Brick::Brick(int w, int h) : width(w), height(h) {
    // generateBricks(1); // Generate bricks for level 1 by default
    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
}

void Brick::generateBricks(int level) {
    clearBricks(); // Clear existing bricks
    int brickCount = 1 + rand() % (level * 2); // Increase number of bricks with level  
    if (brickCount > 10) {
        brickCount = 10;    // limit to 10
    }

    for (int i = 0; i < brickCount; i++) {
        Position pos;
        pos.x = rand() % width;
        pos.y = rand() % height;
        bricks.push_back(pos);
    }
}

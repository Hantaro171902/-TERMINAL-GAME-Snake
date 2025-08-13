#include "brick.cpp"
#include "ultils.hpp"
#include "color.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace std;

vector<Position> Brick::getBricks() {
    return bricks;
}

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
        pos.x = rand() % (width - 2) + 1; // Ensure bricks are within bounds
        pos.y = rand() % (height - 2) + 1;


        // Ensure no duplicate position 
        bool duplicate = false;
        for (auto& b : bricks) {
            if (b.x == pos.x && b.y == pos.y) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            bricks.push_back(pos);
        }
    }
}

void Brick::drawBrick() {
    for (auto& b : bricks) {
        move_cursor(b.x, b.y);
        // setTextColor(RED); // Set color for bricks
        cout << "#" // Draw brick
        // resetTextColor();
    }
}

void Brick::isBrick(int x, int y) {
    for (auto& b : bricks) {
        if (b.x == x && b.y == y) {
            return true; // Brick found
        }
    }
    return false; // No brick at position
}

void Brick::clearBricks() {
    bricks.clear();
}


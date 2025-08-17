#include "brick.hpp"
#include "ultils.hpp"
#include "color.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<Position> Brick::getBricks() const {
    return bricks;
}

Brick::Brick(int w, int h) : width(w), height(h), active(false) {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator
}

void Brick::generateBricks(int level) {
    clearBricks(); // Clear existing bricks
    int brickCount = 1 + rand() % (level * 2); // Increase number of bricks with level  
    if (brickCount > 20) {
        brickCount = 20;    // limit bricks
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
    active = true;
}

void Brick::drawBricks() {
    if (!active) return;
    
    setTextColor(34); // Blue color for bricks
    for (auto& b : bricks) {
        move_cursor(b.y + 3, b.x + 2); // +3 for title, +2 for border offset
        cout << "■"; // Use block character for bricks
    }
    resetTextColor();
}

bool Brick::isBrick(int x, int y) const {
    if (!active) return false;
    
    for (auto& b : bricks) {
        if (b.x == x && b.y == y) {
            return true; // Brick found
        }
    }
    return false; // No brick at position
}

bool Brick::checkCollision(int x, int y) const {
    return isBrick(x, y);
}

void Brick::clearBricks() {
    bricks.clear();
    active = false;
}

void Brick::addBrick(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        Position newBrick(x, y);
        
        // Check for duplicates
        auto it = find(bricks.begin(), bricks.end(), newBrick);
        if (it == bricks.end()) {
            bricks.push_back(newBrick);
            active = true;
        }
    }
}

void Brick::removeBrick(int x, int y) {
    auto it = remove_if(bricks.begin(), bricks.end(), 
                       [x, y](const Position& b) { return b.x == x && b.y == y; });
    bricks.erase(it, bricks.end());
    
    if (bricks.empty()) {
        active = false;
    }
}

void Brick::addRandomBrick() {
    if (!active) return;
    
    int newX, newY;
    bool valid_position;
    int attempts = 0;
    const int max_attempts = 100;
    
    do {
        valid_position = true;
        newX = rand() % (width - 2) + 1;
        newY = rand() % (height - 2) + 1;
        
        // Check if position is already occupied by a brick
        if (isBrick(newX, newY)) {
            valid_position = false;
            continue;
        }
        
        attempts++;
        if (attempts > max_attempts) {
            return; // Give up if we can't find a valid position
        }
        
    } while (!valid_position);
    
    addBrick(newX, newY);
}

bool Brick::isValidPosition(int x, int y, const vector<Position>& snakeHead, 
                           const vector<Position>& snakeTail, 
                           const Position& fruit) const {
    // Check bounds
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    // Check if position conflicts with snake head
    for (const auto& head : snakeHead) {
        if (head.x == x && head.y == y) {
            return false;
        }
    }
    
    // Check if position conflicts with snake tail
    for (const auto& tail : snakeTail) {
        if (tail.x == x && tail.y == y) {
            return false;
        }
    }
    
    // Check if position conflicts with fruit
    if (fruit.x == x && fruit.y == y) {
        return false;
    }
    
    // Check if position conflicts with existing bricks
    if (isBrick(x, y)) {
        return false;
    }
    
    return true;
}


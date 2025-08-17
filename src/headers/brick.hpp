#pragma once

#include <iostream>
#include <vector>

struct Position 
{
    int x;
    int y;
    
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

class Brick {
private:
    int width, height;
    std::vector<Position> bricks; // Store positions of all bricks
    bool active; // Whether bricks are active in the game
    
public:
    Brick(int w, int h);

    void generateBricks(int level);
    void drawBricks();
    bool isBrick(int x, int y) const;
    bool checkCollision(int x, int y) const;
    void clearBricks();
    void addBrick(int x, int y);
    void removeBrick(int x, int y);
    
    // Hit block functionality
    void addRandomBrick();
    bool isValidPosition(int x, int y, const std::vector<Position>& snakeHead, 
                        const std::vector<Position>& snakeTail, 
                        const Position& fruit) const;
    
    // Getters
    std::vector<Position> getBricks() const;
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    int getBrickCount() const { return bricks.size(); }
};

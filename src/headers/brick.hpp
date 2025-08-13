#pragma once

#include <iostream>
#include <vector>

struct Position 
{
    int x;
    int y;
};

class Brick {
private:
    int width, height;
    std::vector<Position> bricks; // Store positions of all bricks
public:
    Brick(int w, int h);

    void generateBricks(int level);
    void drawBrick();
    bool isBrick(int x, int y);
    void clearBricks();

    std::vector<Position> getBricks();
};

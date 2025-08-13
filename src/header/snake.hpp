#pragma once 

#include <iostream>
#include <vector>

enum snake_direction {STOP = 0, UP, DOWN, LEFT, RIGHT };

class Snake {
private:
    int width, height;
    int x, y; // Snake head position
    int fruitX, fruitY; // Fruit position
    int score;
    std::vector<int> tailX, tailY; // Snake tail positions
    int nTail; // Length of the snake tail
    snake_direction dir;
    bool game_over;

public:
    Snake(int w, int h);
    void setup();
    void draw();
    void update_game();
    void input();
    void logic();
    bool game_over();
    // int getScore();
    int set_difficulty();
};

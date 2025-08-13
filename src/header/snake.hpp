#pragma once 

#include <iostream>
#include <vector>

enum SnakeDirection {STOP = 0, UP, DOWN, LEFT, RIGHT };

class Snake {
private:
    int width, height;
    int x, y; // Snake head position
    int fruitX, fruitY; // Fruit position
    int score;
    std::vector<int> tailX, tailY; // Snake tail positions
    int nTail; // Length of the snake tail
    SnakeDirection dir;
    bool game_over;

public:
    Snake(int w, int h);
    void Setup();
    void Draw();
    void UpdateGame();
    void Input();
    // void Logic();
    bool GameOver();
    int GetScore();
    int SetDifficulty();
};

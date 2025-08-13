#include "snake.hpp"
#include "ultils.hpp"
#include <vector>
#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <termios.h>

using namespace std;

Snake::Snake(int w, int h) : width(w), height(h), score(0), nTail(0), game_over(false) {
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    dir = STOP;
}

void Snake::setup() {
    game_over = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Snake::draw() {
    clearTerminal();
    clearScreen();

    

}

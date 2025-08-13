#include "snake.hpp"
#include "ultils.hpp"
#include "color.hpp"
#include <vector>
#include <iostream>
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

void Snake::Setup() {
    game_over = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

int Snake::GetScore() {
    return score;
}

void Snake::Draw() {
    clearTerminal();
    clearScreen();

    setTextColor(33);

    cout << R"(
                                 _          
                                | |         
                 ___ ____   ____| |  _ ____ 
                /___)  _ \ / _  | | / ) _  )
                |___ | | | ( ( | | |< ( (/ / 
                (___/|_| |_|\_||_|_| \_)____)
                             
            )" << '\n';

    resetTextColor();

    // Top border
    cout << SYMBOL_DOUBLE_TOP_LEFT;
    for (int i = 0; i < 2 + width; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL;
    }
    cout << SYMBOL_DOUBLE_TOP_RIGHT << endl;

    for (int i = 0; i < height; i++) {
        cout << SYMBOL_DOUBLE_VERTICAL; // Left border
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                cout << "O"; // Snake head
            } else if (i == fruitY && j == fruitX) {
                cout << "*"; // Fruit
            } else {
                bool isTailPart = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Snake tail
                        isTailPart = true;
                        break;
                    }
                }
                if (!isTailPart) {
                    cout << " "; // Empty space
                }
            }
        }
        cout << SYMBOL_DOUBLE_VERTICAL << endl; // Right border
    }

    //Bottom border
    cout << SYMBOL_DOUBLE_BOTTOM_LEFT;
    for (int i = 0; i < 2 + width; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL;
    }
    cout << SYMBOL_DOUBLE_BOTTOM_RIGHT << endl;

    // Display score
    cout << "Score: " << score << endl;
}

// Function for update the game state ]

void Snake::UpdateGame() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;  // Update the head position in the tail array
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    // Check for snake's collision with the walls (|) 
    if (x >= width || x < 0 || y >= height || y < 0)
        game_over = true;  

    // Check for snake's collision with itself    
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            game_over = true;  
    }

    // Check if the snake has eaten the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;  // Increase score
        fruitX = rand() % width;  // Generate new fruit position
        fruitY = rand() % height;
        nTail++;  // Increase the length of the snake
    }
}

int Snake::SetDifficulty() {
    int dfc, choice;
    cout << "Select difficulty level:  " << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    
    cin >> choice;

    switch (choice) {
        case 1: dfc = 200; break;
        case 2: dfc = 100; break;
        case 3: dfc = 50; break;
        default: dfc = 100; break;
    }
    return dfc;
}

void Snake::Input() {
    if (kbhit()) {
        switch (getch()) {
            case 'a':
            case 'A':
                dir = LEFT;
                break;
            case 'd':
            case 'D':
                dir = RIGHT;
                break;
            case 'w':
            case 'W':
                dir = UP;
                break;
            case 's':
            case 'S':
                dir = DOWN;
                break;
            case 'x':
            case 'X':
                game_over = true; // Exit the game
                break;
        }
    }
}
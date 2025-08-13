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
#include <csignal>

using namespace std;

#define HEAD_BLOCK  "\u2589" // Full left half block
#define TAIL_BLOCK  "\u2584" // Lower half block

namespace {
    termios saved_termios;

    void set_non_blocking_input() {
        tcgetattr(STDIN_FILENO, &saved_termios);

        termios t = saved_termios;
        t.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // Non-blocking

        // Restore terminal on Ctrl+C
        signal(SIGINT, [](int) {
            tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios);
            _exit(0);
        });
    }
    
    void reset_input() {
        tcsetattr(STDIN_FILENO, TCSANOW, &saved_termios); // Restore original terminal
    }
}

Snake::~Snake() {
    reset_input();
}

Snake::Snake(int w, int h) : width(w), height(h) {
    set_non_blocking_input();
    x = width;
    y = height;
    fruitX = rand() % width;
    fruitY = rand() % height;
    dir = STOP;
    tailX.resize(0);
    tailY.resize(0);
}

void Snake::Setup() {
    // set_non_blocking_input();

    game_over = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tailX.resize(0);
    tailY.resize(0);
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
    for (int i = 0; i < width; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL;
    }
    cout << SYMBOL_DOUBLE_TOP_RIGHT << endl;

    for (int i = 0; i < height; i++) {
        cout << SYMBOL_DOUBLE_VERTICAL; // Left border
        for (int j = 0; j < width; j++) {
            if (i == y && j == x) {
                setTextColor(32); // Green for head
                cout << HEAD_BLOCK; // Snake head
                resetTextColor();
            } else if (i == fruitY && j == fruitX) {
                cout << "*"; // Fruit
            } else {
                bool isTailPart = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        setTextColor(32); // Green for tail
                        cout << TAIL_BLOCK;
                        resetTextColor();
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
    for (int i = 0; i < width; i++) {
        cout << SYMBOL_DOUBLE_HORIZONTAL;
    }
    cout << SYMBOL_DOUBLE_BOTTOM_RIGHT << endl;

    // Display score
    cout << "Score: " << score << endl; 
}

// Function for update the game state 

void Snake::UpdateGame() {
    int prevX, prevY, prev2X, prev2Y;

    // Only update tail if nTail > 0
    if (nTail > 0) {
        prevX = tailX[0];
        prevY = tailY[0];
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
        tailX.resize(nTail);
        tailY.resize(nTail);
    }
}

int Snake::SetDifficulty() {
    int choice;
    cout << "Select difficulty level:  " << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    
    cin >> choice;

    switch (choice) {
        case 1: return 200000; // 200ms
        case 2: return 150000; // 150ms
        case 3: return 50000;  // 50ms
        default: return 100000; // 100ms
    }
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

bool Snake::GameOver() {
    return game_over;
}
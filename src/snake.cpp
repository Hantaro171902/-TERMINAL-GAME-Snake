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
#include <algorithm>

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
    if (alternate_screen_active) {
        HideAlternateScreen();
    }
}

Snake::Snake(int w, int h, bool danger) : width(w), height(h), danger_mode(danger) {
    set_non_blocking_input();
    x = width;
    y = height;
    fruitX = rand() % width;
    fruitY = rand() % height;
    dir = STOP;
    tailX.resize(0);
    tailY.resize(0);
    obstacleX.resize(0);
    obstacleY.resize(0);
    nObstacles = 0;
    paused = false;
    alternate_screen_active = false;
}

void Snake::Setup() {
    game_over = false;
    paused = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
    tailX.resize(0);
    tailY.resize(0);
    
    // Clear obstacles
    obstacleX.resize(0);
    obstacleY.resize(0);
    nObstacles = 0;
    
    ShowAlternateScreen();
}

void Snake::ShowAlternateScreen() {
    cout << "\033[?1049h\033[2J\033[H"; // Enter alternate screen, clear, home
    alternate_screen_active = true;
    hideCursor();
}

void Snake::HideAlternateScreen() {
    cout << "\033[2J\033[H\033[?1049l"; // Clear, home, exit alternate screen
    alternate_screen_active = false;
    showCursor(); // Show cursor when exiting alternate screen
}

void Snake::ShowGameOverScreen() {
    HideAlternateScreen();
    
    setTextColor(32); // Green
    cout << R"(
                      __    __    __    __
                     /  \  /  \  /  \  /  \
____________________/  __\/  __\/  __\/  __\_____________________________
___________________/  /__/  /__/  /__/  /________________________________
                   | / \   / \   / \   / \  \____
                   |/   \_/   \_/   \_/   \    o \
                                           \_____/--<
)" << endl;
    resetTextColor();
    
    cout << "\n\nThanks for playing ";
    setTextColor(32);
    cout << "C++ Snake";
    resetTextColor();
    cout << "!\n\n";
    
    setTextColor(35); // Magenta
    cout << "This game was enhanced with features from Shell Snake!\n";
    resetTextColor();
    
    cout << "Score: ";
    setTextColor(33); // Yellow
    cout << score;
    resetTextColor();
    cout << "   ";
    setTextColor(32);
    cout << "◀⚬⚬⚬";
    resetTextColor();
    cout << "\n\n";
}

int Snake::GetScore() {
    return score;
}

std::string Snake::GetHeadSymbol() const {
    switch (dir) {
        case UP: return "▲";
        case DOWN: return "▼";
        case LEFT: return "◀";
        case RIGHT: return "▶";
        default: return "●";
    }
}

std::string Snake::GetTailSymbol() const {
    return "⚬";
}

void Snake::TogglePause() {
    paused = !paused;
}

void Snake::AddObstacle() {
    if (!danger_mode) return;
    
    int newX, newY;
    bool valid_position;
    
    do {
        valid_position = true;
        newX = rand() % width;
        newY = rand() % height;
        
        // Don't place on snake head
        if (newX == x && newY == y) {
            valid_position = false;
            continue;
        }
        
        // Don't place on fruit
        if (newX == fruitX && newY == fruitY) {
            valid_position = false;
            continue;
        }
        
        // Don't place on snake tail
        for (int i = 0; i < nTail; i++) {
            if (tailX[i] == newX && tailY[i] == newY) {
                valid_position = false;
                break;
            }
        }
        
        // Don't place on existing obstacles
        for (int i = 0; i < nObstacles; i++) {
            if (obstacleX[i] == newX && obstacleY[i] == newY) {
                valid_position = false;
                break;
            }
        }
    } while (!valid_position);
    
    obstacleX.push_back(newX);
    obstacleY.push_back(newY);
    nObstacles++;
}

void Snake::DrawObstacles() {
    if (!danger_mode) return;
    
    setTextColor(34); // Blue
    for (int i = 0; i < nObstacles; i++) {
        move_cursor(obstacleY[i] + 3, obstacleX[i] + 2); // +3 for title, +2 for border
        cout << "■";
    }
    resetTextColor();
}

bool Snake::CheckObstacleCollision(int x, int y) {
    for (int i = 0; i < nObstacles; i++) {
        if (obstacleX[i] == x && obstacleY[i] == y) {
            return true;
        }
    }
    return false;
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
                cout << GetHeadSymbol(); // Directional snake head
                resetTextColor();
            } else if (i == fruitY && j == fruitX) {
                setTextColor(33); // Yellow for fruit
                cout << "☺"; // Fruit symbol
                resetTextColor();
            } else {
                bool isTailPart = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        setTextColor(32); // Green for tail
                        cout << GetTailSymbol();
                        resetTextColor();
                        isTailPart = true;
                        break;
                    }
                }
                if (!isTailPart) {
                    // Check for obstacles
                    bool isObstacle = false;
                    for (int k = 0; k < nObstacles; k++) {
                        if (obstacleX[k] == j && obstacleY[k] == i) {
                            setTextColor(34); // Blue for obstacles
                            cout << "■";
                            resetTextColor();
                            isObstacle = true;
                            break;
                        }
                    }
                    if (!isObstacle) {
                        cout << " "; // Empty space
                    }
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

    // Display score and status
    setTextColor(33); // Yellow
    cout << "Score: " << score;
    if (paused) {
        cout << " [PAUSED]";
    }
    if (danger_mode) {
        cout << " [DANGER MODE]";
    }
    cout << endl;
    resetTextColor();
}

void Snake::UpdateGame() {
    if (paused) return;
    
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

    // Check for snake's collision with the walls
    if (x >= width || x < 0 || y >= height || y < 0)
        game_over = true;  

    // Check for snake's collision with obstacles
    if (CheckObstacleCollision(x, y))
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
        
        // In danger mode, add an obstacle when eating fruit
        if (danger_mode) {
            AddObstacle();
        }
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
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
            case 'S':
                if (dir != UP) dir = DOWN;
                break;
            case 'p':
            case 'P':
                TogglePause();
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
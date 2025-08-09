#include <iostream>
#include <unistd.h>   // for usleep
#include <termios.h>  // for terminal settings
#include <fcntl.h>    // for non-blocking input
#include <cstdlib>    // for rand
#include <ctime>      // for seeding rand

using namespace std;

const int width = 40;
const int height = 20;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum snakeDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
snakeDirection dir;
bool isGameOver;

// Enable non-blocking keyboard input
void enableNonBlockingInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // disable canonical mode & echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // non-blocking input
}

// Reset terminal settings when exiting
void disableNonBlockingInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, 0);
}

void Setup() {
    isGameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    cout << "\033[2J\033[1;1H"; // clear screen and move cursor to top left

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";

            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "@";
            else {
                bool isTailPart = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        isTailPart = true;
                        break;
                    }
                }
                if (!isTailPart) cout << " ";
            }

            if (j == width - 1) cout << "#";
        }
        cout << "\n";
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    cout << "Score: " << score << "\n";
}

void UpdateGame() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
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
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
        default: break;
    }

    if (x >= width || x < 0 || y >= height || y < 0)
        isGameOver = true;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            isGameOver = true;
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int setDifficulty() {
    int choice;
    cout << "Select difficulty level (1-3):\n";
    cout << "1. Easy\n";
    cout << "2. Medium\n";
    cout << "3. Hard\n";
    cout << "Choice: ";
    cin >> choice;
    if (choice == 1) return 200000; // microseconds
    if (choice == 3) return 50000;
    return 100000;
}

void Input() {
    char key;
    if (read(STDIN_FILENO, &key, 1) > 0) {
        switch (key) {
            case 'a': case 'A': dir = LEFT; break;
            case 'd': case 'D': dir = RIGHT; break;
            case 'w': case 'W': dir = UP; break;
            case 's': case 'S': dir = DOWN; break;
            case 'x': case 'X': isGameOver = true; break;
        }
    }
}

int main() {
    srand(time(0));
    Setup();
    int dfc = setDifficulty();
    enableNonBlockingInput();

    while (!isGameOver) {
        Draw();
        Input();
        UpdateGame();
        usleep(dfc);
    }

    disableNonBlockingInput();
    cout << "Game Over! Final score: " << score << "\n";
    return 0;
}

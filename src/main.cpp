#include <iostream>
#include <unistd.h>   // for usleep
#include <termios.h>  // for terminal settings
#include <fcntl.h>    // for non-blocking input
#include <cstdlib>    // for rand
#include <ctime>      // for seeding rand
#include "ultils.hpp"

using namespace std;

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
    
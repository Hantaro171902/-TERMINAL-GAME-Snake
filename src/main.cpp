#include <iostream>
#include <unistd.h>   // for usleep
#include <termios.h>  // for terminal settings
#include <fcntl.h>    // for non-blocking input
#include <cstdlib>    // for rand
#include <ctime>      // for seeding rand
#include "ultils.hpp"
#include "snake.hpp"

using namespace std;

int main() {
    srand(time(nullptr));
    Snake snake(40, 20);
    snake.Setup();
    int dfc = snake.SetDifficulty();
    // enableNonBlockingInput();

    while (!snake.GameOver()) {
        snake.Draw();
        snake.Input();
        snake.UpdateGame();
        usleep(dfc);
    }

    // disableNonBlockingInput();
    cout << "Game Over! Final score: " << snake.GetScore() << endl;
    return 0;
}
    
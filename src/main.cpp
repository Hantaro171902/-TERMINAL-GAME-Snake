#include <iostream>
#include <unistd.h>   // for usleep
#include <termios.h>  // for terminal settings
#include <fcntl.h>    // for non-blocking input
#include <cstdlib>    // for rand
#include <ctime>      // for seeding rand
#include <string>
#include "ultils.hpp"
#include "snake.hpp"

using namespace std;

void show_help() {
    cout << "C++ Snake is the classic game of snake, enhanced with features from Shell Snake!\n\n";
    cout << "Usage: ./snake [options]\n\n";
    cout << "Options:\n";
    cout << "  -h, --help     Show this help message\n";
    cout << "  -d, --danger   Enable danger mode with obstacles\n";
    cout << "  -b, --brick    Enable brick mode with static obstacles\n\n";
    cout << "Controls:\n";
    cout << "  WASD or Arrow Keys - Move the snake\n";
    cout << "  P - Pause/Unpause the game\n";
    cout << "  X - Exit the game\n\n";
    cout << "Features:\n";
    cout << "  - Directional snake head (▲▼▶◀)\n";
    cout << "  - Pause functionality\n";
    cout << "  - Danger mode with obstacles\n";
    cout << "  - Brick mode with static obstacles\n";
    cout << "  - Better visual feedback\n";
    cout << "  - Improved game over screen\n\n";
    cout << "I hope wherever you are, you're having a good day!\n\n";
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    
    bool danger_mode = false;
    bool brick_mode = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            show_help();
            return 0;
        } else if (arg == "-d" || arg == "--danger") {
            danger_mode = true;
        } else if (arg == "-b" || arg == "--brick") {
            brick_mode = true;
        } else {
            cerr << "Unknown option: " << arg << endl;
            cerr << "Use -h or --help for usage information." << endl;
            return 1;
        }
    }
    
    Snake snake(50, 20, danger_mode, brick_mode);
    
    snake.Setup();
    int dfc = snake.SetDifficulty();

    while (!snake.GameOver()) {
        snake.Draw();
        snake.Input();
        snake.UpdateGame();
        usleep(dfc);
    }

    // Show game over screen
    snake.ShowGameOverScreen();
    
    return 0;
}
    
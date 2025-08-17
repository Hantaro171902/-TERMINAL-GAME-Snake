// snake.hpp
#pragma once 

#include <iostream>
#include <vector>
#include <string>

enum SnakeDirection {STOP = 0, UP, DOWN, LEFT, RIGHT };

class Snake {
private:
    int width, height;
    int x, y; // Snake head position
    int fruitX, fruitY; // Fruit position
    int score;
    std::vector<int> tailX;
    std::vector<int> tailY; // Snake tail positions
    int nTail; // Length of the snake tail
    SnakeDirection dir;
    bool game_over;
    bool paused;
    bool danger_mode;
    
    // Obstacles for danger mode
    std::vector<int> obstacleX;
    std::vector<int> obstacleY;
    int nObstacles;
    
    // Terminal state
    bool alternate_screen_active;

public:
    Snake(int w, int h, bool danger = false);
    ~Snake();
    
    void Setup();
    void Draw();
    void UpdateGame();
    void Input();
    bool GameOver();
    int GetScore();
    int SetDifficulty();
    
    // New features
    void TogglePause();
    bool IsPaused() const { return paused; }
    void SetDangerMode(bool enabled) { danger_mode = enabled; }
    bool IsDangerMode() const { return danger_mode; }
    
    // Terminal management
    void ShowAlternateScreen();
    void HideAlternateScreen();
    void ShowGameOverScreen();
    
    // Obstacle management
    void AddObstacle();
    void DrawObstacles();
    bool CheckObstacleCollision(int x, int y);
    
    // Directional head symbols
    std::string GetHeadSymbol() const;
    std::string GetTailSymbol() const;
};

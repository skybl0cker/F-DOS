#include <iostream>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <atomic>  // Added to use atomic for inputActive flag

namespace snake {

    const int width = 40;
    const int height = 20;
    const char snakeChar = '#';
    const char foodChar = '*';
    const char emptyChar = ' ';

    int score = 0;
    bool quit = false;
    int snakeDirX = 1, snakeDirY = 0;  // Initial snake direction: moving right
    std::deque<std::pair<int, int>> snake;  // Stores the snake's body as (x, y) pairs
    int foodX, foodY;

    std::atomic<bool> inputActive(true);  // Atomic flag to control input processing

    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    void draw() {
        clearScreen();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                    std::cout << '#';  // Draw walls
                } else if (x == foodX && y == foodY) {
                    std::cout << foodChar;  // Draw food
                } else {
                    bool snakePart = false;
                    for (auto& part : snake) {
                        if (part.first == x && part.second == y) {
                            std::cout << snakeChar;
                            snakePart = true;
                            break;
                        }
                    }
                    if (!snakePart) std::cout << emptyChar;
                }
            }
            std::cout << '\n';
        }
        std::cout << "Score: " << score << '\n';
    }

    void input() {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        char c;
        while (inputActive) {
            c = getchar();
            if (c == 'w' && snakeDirY != 1) { snakeDirX = 0; snakeDirY = -1; }
            if (c == 's' && snakeDirY != -1) { snakeDirX = 0; snakeDirY = 1; }
            if (c == 'a' && snakeDirX != 1) { snakeDirX = -1; snakeDirY = 0; }
            if (c == 'd' && snakeDirX != -1) { snakeDirX = 1; snakeDirY = 0; }
            if (c == 'q') quit = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    void spawnFood() {
        foodX = rand() % (width - 2) + 1;
        foodY = rand() % (height - 2) + 1;
    }

    void logic() {
        // Get the current head position
        int headX = snake.front().first;
        int headY = snake.front().second;

        // Move the head in the current direction
        headX += snakeDirX;
        headY += snakeDirY;

        // Check for wall collisions
        if (headX <= 0 || headX >= width - 1 || headY <= 0 || headY >= height - 1) {
            quit = true;
        }

        // Check for self-collision
        for (auto& part : snake) {
            if (part.first == headX && part.second == headY) {
                quit = true;
            }
        }

        // Add new head to the snake
        snake.push_front({headX, headY});

        // Check if the snake eats food
        if (headX == foodX && headY == foodY) {
            score++;
            spawnFood();  // Generate new food
        } else {
            // Remove the last part of the snake if no food was eaten
            snake.pop_back();
        }
    }

    void runSnake() {
        srand(time(0));  // Seed the random number generator
        snake.clear();
        snake.push_front({width / 2, height / 2});  // Start the snake in the middle
        spawnFood();  // Generate the first food

        std::thread inputThread(input);

        while (!quit) {
            draw();
            logic();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));  // Adjust speed here
        }

        // Stop the input thread after the game ends
        inputActive = false;
        inputThread.join();

        clearScreen();
        std::cout << "Game Over! Final Score: " << score << '\n';
    }

} // namespace snake

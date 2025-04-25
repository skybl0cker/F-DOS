#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>

namespace pong {

    const int width = 60;
    const int height = 20;
    const char ballChar = 'O';
    const char paddleChar = '|';

    int ballX, ballY;
    int ballDirX = 1, ballDirY = 1;
    int paddle1Y, paddle2Y;
    int score1 = 0, score2 = 0;
    bool quit = false;
    bool singlePlayer = false;

    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }

    void draw() {
        clearScreen();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width - 1) {
                    std::cout << '#';
                } else if (y == 0 || y == height - 1) {
                    std::cout << '#';
                } else if (x == 2 && y >= paddle1Y - 2 && y <= paddle1Y + 2) {
                    std::cout << paddleChar;
                } else if (x == width - 3 && y >= paddle2Y - 2 && y <= paddle2Y + 2) {
                    std::cout << paddleChar;
                } else if (x == ballX && y == ballY) {
                    std::cout << ballChar;
                } else {
                    std::cout << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << "Player 1: " << score1 << " | Player 2: " << score2 << '\n';
    }

    void input() {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

        char c;
        while (!quit) {
            c = getchar();
            if (c == 'w' && paddle1Y - 3 > 0) paddle1Y -= 3;
            if (c == 's' && paddle1Y + 3 < height - 1) paddle1Y += 3;
            if (!singlePlayer) {
                if (c == 'i' && paddle2Y - 3 > 0) paddle2Y -= 3;
                if (c == 'k' && paddle2Y + 3 < height - 1) paddle2Y += 3;
            }
            if (c == 'q') quit = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    void logic() {
        ballX += ballDirX;
        ballY += ballDirY;

        // Bounce off top/bottom
        if (ballY <= 1 || ballY >= height - 2)
            ballDirY = -ballDirY;

        // Bounce off paddles
        if (ballX == 3 && ballY >= paddle1Y - 2 && ballY <= paddle1Y + 2)
            ballDirX = 1;
        else if (ballX == width - 4 && ballY >= paddle2Y - 2 && ballY <= paddle2Y + 2)
            ballDirX = -1;

        // Score left
        if (ballX <= 1) {
            score2++;
            ballX = width / 2;
            ballY = height / 2;
            ballDirX = 1;
            ballDirY = (ballDirY == 0 ? 1 : ballDirY);  // Ensure ballDirY isn't set to 0
        }

        // Score right
        if (ballX >= width - 2) {
            score1++;
            ballX = width / 2;
            ballY = height / 2;
            ballDirX = -1;
            ballDirY = (ballDirY == 0 ? 1 : ballDirY);  // Ensure ballDirY isn't set to 0
        }

        // CPU AI
        if (singlePlayer) {
            if (ballY < paddle2Y && paddle2Y > 3) paddle2Y--;
            if (ballY > paddle2Y && paddle2Y < height - 4) paddle2Y++;
        }

        // Win condition
        if (score1 >= 10 || score2 >= 10)
            quit = true;
    }

    void runPong() {
        clearScreen();
        std::string mode;
        std::cout << "Welcome to Terminal Pong!\n";
        std::cout << "1 or 2 players? ";
        std::cin >> mode;
        singlePlayer = (mode == "1");

        ballX = width / 2;
        ballY = height / 2;
        paddle1Y = paddle2Y = height / 2;

        std::thread inputThread(input);

        while (!quit) {
            draw();
            logic();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        inputThread.join();

        clearScreen();
        if (score1 >= 10)
            std::cout << "Player 1 wins!\n";
        else if (score2 >= 10)
            std::cout << (singlePlayer ? "CPU wins!\n" : "Player 2 wins!\n");
        else
            std::cout << "Game quit early.\n";
    }

} // namespace pong

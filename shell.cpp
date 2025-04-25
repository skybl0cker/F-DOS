#include "shell.h"
#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "pong.h" // Include pong.h
#include "snake.h"

// Simulated file list (for now, just a few files)
std::vector<std::string> files = {
    "autoexec.bat",
    "config.sys",
    "fdos.sys",
    "README.txt",
    "COMMAND.COM"
};

// Function to simulate the `dir` command
void Shell::listFiles() {
    std::cout << "Listing files in the current directory:\n";
    for (const auto& file : files) {
        std::cout << file << "\n";
    }
}

// Function to print the F-DOS and copyright message
void Shell::printTag() {
    std::cout << "F-DOS\n";
    std::cout << "Copyright Idiots Inc. 2025\n";
}

// Function to clear the screen but keep the F-DOS and copyright message
void Shell::clearScreen() {
    // Clear the screen (Linux-based, works for Unix-like systems)
    std::cout << "\033[2J\033[1;1H";  // ANSI escape codes for clear screen
    printTag();  // Print the F-DOS and copyright message
    std::cout << "\n";  // Add space after the tag
}

// Function to display the help menu
void Shell::printHelp() {
    std::cout << "Help Menu:\n";
    std::cout << "cls - Clear the screen\n";
    std::cout << "dir - List files in the directory\n";
    std::cout << "exit - Exit the shell\n";
    std::cout << "pong - Play the pong game\n"; // Added pong to help
}

// Function to print the prompt (C:\>)
void Shell::printPrompt() {
    std::cout << "C:\\> ";
}

// Boot sequence: BIOS info, RAM check, HDD check, etc.
void Shell::showBootSequence() {
    clearScreen();  // Clear the screen and show F-DOS and copyright tag
    std::cout << "BIOS Version 1.0.0\n";
    std::cout << "Copyright (c) 2025 Idiots Inc.\n";
    std::cout << "Initializing memory...\n";

    // Simulate RAM check by counting up from 0 KB to 640000 K
    for (int i = 0; i <= 640000; i += 13857) {
        usleep(100000);  // Sleep to simulate progress (100ms delay)
        std::cout << "RAM Check: " << i << " K\r" << std::flush;
    }
    std::cout << "\nRAM check complete.\n";

    // Simulate HDD check for all IDE drives
    std::cout << "Checking IDE Master 1... ";
    usleep(1000000);  // Simulate 1 second delay for checking
    std::cout << "Detected\n";

    std::cout << "Checking IDE Slave 1... ";
    usleep(1000000);  // Simulate 1 second delay for checking
    std::cout << "Not Detected\n";

    std::cout << "Checking IDE Master 2... ";
    usleep(1000000);  // Simulate 1 second delay for checking
    std::cout << "Not Detected\n";

    std::cout << "Checking IDE Slave 2... ";
    usleep(1000000);  // Simulate 1 second delay for checking
    std::cout << "Not Detected\n";

    // Display the booting message
    std::cout << "Booting F-DOS...\n\n";
}

// Function to handle commands typed by the user
void Shell::handleCommand(const std::string& command) {
    if (command == "cls") {
        clearScreen();
    } else if (command == "help") {
        printHelp();
    } else if (command == "dir") {
        listFiles();
    } else if (command == "exit") {
        std::cout << "Exiting F-DOS...\n";
        exit(0);  // Exit the program
    } else if (command == "pong") {
        pong::runPong(); // Call the function from the pong namespace
    } else if (command == "snake") {
        snake::runSnake();
    } else {
        std::cout << "Unknown command: " << command << "\n";
    }
}

// Function to start the shell
void Shell::start() {
    showBootSequence(); // Show the boot sequence
    clearScreen();
    while (true) {
        printPrompt();
        std::string command;
        std::getline(std::cin, command);
        handleCommand(command);
    }
}

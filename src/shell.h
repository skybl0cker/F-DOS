// shell.h
#ifndef SHELL_H
#define SHELL_H

#include <string>

namespace pong { // Forward declaration of the pong namespace
    void runPong();
}

class Shell {
public:
    // Start the shell
    void start();
    
    // Handle commands entered by the user
    void handleCommand(const std::string& command);
    
    // Print the DOS-like prompt
    void printPrompt();
    
    // Clear the screen while keeping the F-DOS and copyright message visible
    void clearScreen();
    
    // Print the help menu
    void printHelp();
    
    // Show the boot sequence (BIOS, RAM check, HDD check, etc.)
    void showBootSequence();

    // List the files in the current directory (for `dir` command)
    void listFiles();

    void printTag();
    
    // Run the pong game
    void runPong();

    void runSnake();
};


#endif  // SHELL_H

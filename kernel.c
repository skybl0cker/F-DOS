#define VGA_WIDTH 80
#define VGA_HEIGHT 25

char *video_memory = (char *)0xb8000;

// Simple delay function (for controlling frame rate)
void delay(int cycles) {
    for (volatile int i = 0; i < cycles; i++);
}

// Clear the screen by filling it with spaces
void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07; // Light grey on black
    }
}

// Print a string at a specific (x, y) position on the screen
void print_at(const char *str, int x, int y) {
    int offset = (y * VGA_WIDTH + x) * 2;
    while (*str) {
        video_memory[offset] = *str++;
        video_memory[offset + 1] = 0x07; // Light grey on black
        offset += 2;
    }
}

// Boot sequence with startup messages
void boot_sequence() {
    clear_screen();
    print_at("TetrisOS v1.0", 34, 10);
    print_at("Booting...", 34, 12);
    delay(10000000); // Wait for a moment to show booting
    clear_screen();
    print_at("Ready", 37, 12);
}

// Key press reading (scancode mapping for simplicity)
char read_key() {
    char key;
    asm volatile("inb $0x60, %0" : "=a"(key));
    return key;
}

// Function to handle Tetris game logic
void tetris_game() {
    int piece_x = 4, piece_y = 0;
    char piece[4][4] = {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    int frame_counter = 0;

    while (1) {
        if (frame_counter++ % 10 == 0) { // Slow down the piece's falling speed
            piece_y++;
            if (piece_y > VGA_HEIGHT - 4) piece_y = 0; // Reset to top when piece reaches bottom
        }

        // Clear the screen to update the game
        clear_screen();
        print_at("Tetris - Q to quit", 0, 0);

        // Draw the current piece
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (piece[y][x]) {
                    int pos_x = piece_x + x + 35; // Center the piece on the screen
                    int pos_y = piece_y + y + 2;  // Offset to prevent piece from being on the top row
                    if (pos_y < VGA_HEIGHT && pos_x < VGA_WIDTH) {
                        int offset = (pos_y * VGA_WIDTH + pos_x) * 2;
                        video_memory[offset] = '#';
                        video_memory[offset + 1] = 0x07; // Light grey color
                    }
                }
            }
        }

        // Handle key press (check if Q is pressed to quit)
        char key = read_key();
        if (key == 0x10) break; // 'Q' to quit
    }
}

// Basic string comparison function (for command handling in shell)
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// Shell that allows command input
void shell() {
    char input[20];
    int pos = 0;

    while (1) {
        clear_screen();
        print_at("TetrisOS> ", 0, 0);
        print_at(input, 10, 0);

        char key = read_key(); // Get the key pressed

        if (key == 0x1C) { // Enter key pressed (scancode for Enter)
            input[pos] = 0; // Null terminate the input string
            if (strcmp(input, "tetris") == 0) {
                tetris_game(); // Run the Tetris game
            } else if (strcmp(input, "cls") == 0) {
                clear_screen(); // Clear screen if "cls" command is typed
            } else if (pos > 0) {
                print_at("Bad command", 0, 1); // Handle invalid commands
            }
            pos = 0; // Reset input position after processing command
        } else if (key >= 0x02 && key <= 0x0A) {
            // Add more key mappings for the letters 'a' to 'z'
            // This is a simple way to map a few characters to keys
            char char_map[] = "abcdefghijklmnopqrstuvwxyz";
            if (key >= 0x02 && key <= 0x1F) {
                input[pos++] = char_map[key - 0x02];
            }
        }

        // Prevent buffer overflow by limiting input length
        if (pos >= 19) pos = 19;
    }
}

// The main kernel function, where the system starts
void kernel_main() {
    print_at("Reached Kernel_main()", 34, 10);
    boot_sequence(); // Show boot sequence
    shell(); // Enter the shell for user interaction
}

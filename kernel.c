#define VGA_WIDTH 80
#define VGA_HEIGHT 25

char *video_memory = (char *)0xb8000;

void delay(int cycles) {
    for (volatile int i = 0; i < cycles; i++);
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x07; // Light grey on black
    }
}

void print_at(const char *str, int x, int y) {
    int offset = (y * VGA_WIDTH + x) * 2;
    while (*str) {
        video_memory[offset] = *str++;
        video_memory[offset + 1] = 0x07;
        offset += 2;
    }
}

void boot_sequence() {
    clear_screen();
    print_at("TetrisOS v1.0", 34, 10);
    
    print_at("Booting...", 34, 12);
    
    clear_screen();
    print_at("Ready", 37, 12);
    
}

void tetris_game() {
    int piece_x = 4, piece_y = 0;
    char piece[4][4] = {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    while (1) {
        clear_screen();
        print_at("Tetris - Q to quit", 0, 0);
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (piece[y][x]) {
                    int pos_x = piece_x + x + 35;
                    int pos_y = piece_y + y + 2;
                    if (pos_y < VGA_HEIGHT && pos_x < VGA_WIDTH) {
                        int offset = (pos_y * VGA_WIDTH + pos_x) * 2;
                        video_memory[offset] = '#';
                        video_memory[offset + 1] = 0x07;
                    }
                }
            }
        }
        piece_y++;
        if (piece_y > VGA_HEIGHT - 4) piece_y = 0;

        char key;
        asm volatile("inb $0x60, %0" : "=a"(key));
        if (key == 0x10) break; // Q to quit

       
    }
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void shell() {
    char input[20];
    int pos = 0;

    while (1) {
        clear_screen();
        print_at("TetrisOS> ", 0, 0);
        print_at(input, 10, 0);

        char key;
        asm volatile("inb $0x60, %0" : "=a"(key));

        if (key == 0x1C) { // Enter
            input[pos] = 0;
            if (strcmp(input, "tetris") == 0) {
                tetris_game();
            } else if (strcmp(input, "cls") == 0) {
                clear_screen();
            } else if (pos > 0) {
                print_at("Bad command", 0, 1);
                
            }
            pos = 0;
        } else if (key == 0x14) input[pos++] = 't'; // Basic key mapping
        else if (key == 0x12) input[pos++] = 'e';
        else if (key == 0x13) input[pos++] = 'r';
        else if (key == 0x17) input[pos++] = 'i';
        else if (key == 0x1F) input[pos++] = 's';
        else if (key == 0x2E) input[pos++] = 'c';
        else if (key == 0x26) input[pos++] = 'l';
        if (pos >= 19) pos = 19;

    }
}

void kernel_main() {
    print_at("Reached Kernel_main()", 34, 10);
    boot_sequence();
    shell();
}
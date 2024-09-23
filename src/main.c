#include <conio.h>

char* screen = (char*)(1024);
unsigned char* color = (unsigned char*)(55296);

char* joystick = (char*)(56321);
unsigned char buffer[1000];
#define block_char 224
#define empty_char 96
#define place_color 2
#define block_color 5

int main() {
    short i;
    unsigned char num_neighbors;
    unsigned short placement_index;
    int input_delay;
    char left;
    char right;
    char up;
    char down;
    char fire;

    *((unsigned char*)53281) = 0;
    clrscr();
    placement_index = 0;
    screen[placement_index] = block_char;
    color[placement_index] = place_color;
    input_delay = -1;
    left = 0;
    right = 0;
    up = 0;
    down = 0;
    fire = 0;
    for (i = 0; i < 1000; ++i) {
        buffer[i] = empty_char;
    }
    while (placement_index < 1000) {
        if ((*joystick & 0x01) == 0) {
            if (up == 0) {
                if (color[placement_index] != block_color) {
                    screen[placement_index] = empty_char;
                }
                placement_index -= 40;
                if (color[placement_index] != block_color) {
                    screen[placement_index] = block_char;
                    color[placement_index] = place_color;
                }
                up = 1;
            }
        } else {
            up = 0;
        }
        if ((*joystick & 0x02) == 0) {
            if (down == 0) {
                if (color[placement_index] != block_color) {
                    screen[placement_index] = empty_char;
                }
                placement_index += 40;
                if (color[placement_index] != block_color) {
                    screen[placement_index] = block_char;
                    color[placement_index] = place_color;
                }
                down = 1;
            }
        } else {
            down = 0;
        }
        if ((*joystick & 0x04) == 0) {
            if (left == 0) {
                if (color[placement_index] != block_color) {
                    screen[placement_index] = empty_char;
                }
                placement_index -= 1;
                if (color[placement_index] != block_color) {
                    screen[placement_index] = block_char;
                    color[placement_index] = place_color;
                }
                left = 1;
            }
        } else {
            left = 0;
        }
        if ((*joystick & 0x08) == 0) {
            if (right == 0) {
                if (color[placement_index] != block_color) {
                    screen[placement_index] = empty_char;
                }
                placement_index += 1;
                if (color[placement_index] != block_color) {
                    screen[placement_index] = block_char;
                    color[placement_index] = place_color;
                }
                right = 1;
            }
        } else {
            right = 0;
        }
        if ((*joystick & 0x10) == 0) {
            if (fire == 0) {
                if (screen[placement_index] == block_char) {
                    screen[placement_index] = empty_char;
                } else {
                    screen[placement_index] = block_char;
                    color[placement_index] = block_color;
                }
                fire = 1;
            }
        } else {
            fire = 0;
        }
    }
    for (;;) {
        for (i = 0; i < 1000; ++i) {
            num_neighbors = 0;
            if (screen[i - 41] == block_char) num_neighbors++;
            if (screen[i - 40] == block_char) num_neighbors++;
            if (screen[i - 39] == block_char) num_neighbors++;
            if (screen[i - 1] == block_char) num_neighbors++;
            if (screen[i + 1] == block_char) num_neighbors++;
            if (screen[i + 39] == block_char) num_neighbors++;
            if (screen[i + 40] == block_char) num_neighbors++;
            if (screen[i + 41] == block_char) num_neighbors++;
            if (num_neighbors < 2) {
                buffer[i] = empty_char;
            } else if (num_neighbors > 3) {
                buffer[i] = empty_char;
            } else if (num_neighbors == 3) {
                buffer[i] = block_char;
            } else {
                buffer[i] = screen[i];
            }
        }
        for (i = 0; i < 1000; ++i) {
            screen[i] = buffer[i];
            if (buffer[i] == block_char) color[i] = block_color;
        }
    }

    return 0;
}

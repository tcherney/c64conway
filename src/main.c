#include <conio.h>

char* screen = (char*)(1024);
unsigned char* color = (unsigned char*)(55296);

char* joystick = (char*)(56321);

short live_list[1000];
short neighbor_list[1000];
short neighbor_list_length;
char neighbor_values[1000];
short live_list_length;
#define block_char 224
#define empty_char 96
#define place_color 2
#define block_color 5
#define empty_color 0
#define EVER \
    ;        \
    ;

inline void add_self(short indx) {
    if (indx < 0)
        indx = 1000 + indx;
    else if (indx >= 1000)
        indx -= 1000;
    if (neighbor_values[indx] == 0) {
        neighbor_list[neighbor_list_length] = indx;
        neighbor_list_length++;
        neighbor_values[indx] = (char)-1;
    }
}

inline void add_neighbor(short indx) {
    if (indx < 0)
        indx = 1000 + indx;
    else if (indx >= 1000)
        indx -= 1000;
    if (neighbor_values[indx] == 0) {
        neighbor_list[neighbor_list_length] = indx;
        neighbor_list_length++;
    }
    if (neighbor_values[indx] == (char)-1) {
        neighbor_values[indx] = 0;
    }
    neighbor_values[indx]++;
}

int main() {
    short i;

    unsigned short placement_index;
    char left;
    char right;
    char up;
    char down;
    char fire;

    *((unsigned char*)53281) = 0;
    clrscr();
    placement_index = 0;
    for (i = 0; i < 1000; ++i) {
        color[i] = empty_color;
    }
    screen[placement_index] = block_char;
    color[placement_index] = place_color;
    for (EVER) {
        while (placement_index < 1000) {
            if ((*joystick & 0x01) == 0) {
                if (up == 0) {
                    if (color[placement_index] != block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
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
                        color[placement_index] = empty_color;
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
                        color[placement_index] = empty_color;
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
                        color[placement_index] = empty_color;
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
                    if (screen[placement_index] == block_char &&
                        color[placement_index] == block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
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
        live_list_length = 0;
        for (i = 0; i < 1000; ++i) {
            if (screen[i] == block_char && color[i] == block_color) {
                live_list[live_list_length] = i;
                live_list_length++;
            }
            neighbor_values[i] = 0;
            neighbor_list[i] = -1;
        }
        for (EVER) {
            neighbor_list_length = 0;
            for (i = 0; i < live_list_length; ++i) {
                if (live_list[i] % 40 == 0) {
                    add_neighbor((live_list[i] + 39));
                    add_neighbor((live_list[i] - 1));
                    add_neighbor((live_list[i] + 79));

                    add_neighbor((live_list[i] + 41));
                    add_neighbor((live_list[i] + 1));
                    add_neighbor((live_list[i] - 39));
                } else if (live_list[i] % 40 == 39) {
                    add_neighbor((live_list[i] - 41));
                    add_neighbor((live_list[i] - 1));
                    add_neighbor((live_list[i] + 39));

                    add_neighbor((live_list[i] - 39));
                    add_neighbor((live_list[i] - 79));
                    add_neighbor((live_list[i] + 1));
                } else {
                    add_neighbor((live_list[i] - 41));
                    add_neighbor((live_list[i] - 39));
                    add_neighbor((live_list[i] - 1));
                    add_neighbor((live_list[i] + 1));
                    add_neighbor((live_list[i] + 39));
                    add_neighbor((live_list[i] + 41));
                }

                add_neighbor((live_list[i] - 40));
                add_neighbor((live_list[i] + 40));
                add_self(live_list[i]);
            }
            live_list_length = 0;
            for (i = 0; i < neighbor_list_length; i++) {
                if (neighbor_values[neighbor_list[i]] < 2) {
                    screen[neighbor_list[i]] = empty_char;
                    color[neighbor_list[i]] = empty_color;
                } else if (neighbor_values[neighbor_list[i]] > 3) {
                    screen[neighbor_list[i]] = empty_char;
                    color[neighbor_list[i]] = empty_color;
                } else if (neighbor_values[neighbor_list[i]] == 3) {
                    screen[neighbor_list[i]] = block_char;
                    color[neighbor_list[i]] = block_color;
                    live_list[live_list_length] = neighbor_list[i];
                    live_list_length++;
                } else {
                    if (screen[neighbor_list[i]] == block_char) {
                        live_list[live_list_length] = neighbor_list[i];
                        live_list_length++;
                    }
                }
                neighbor_values[neighbor_list[i]] = 0;
            }
            if ((*joystick & 0x10) == 0) {
                if (fire == 0) {
                    placement_index = 0;
                    fire = 1;
                    break;
                }
            } else {
                fire = 0;
            }
        }
    }

    return 0;
}

#include <conio.h>

char* screen = (char*)(1024);
char* color = (char*)(55296);

char* joystick = (char*)(56321);

short live_list[1000];
short neighbor_list[1000];
short neighbor_list_length;
char neighbor_values[1000];
char last_buffer[1000];
short live_list_length;
char block_char = 224;
char empty_char = 96;
char place_color = 2;
char block_color = 5;
char empty_color = 0;
short i;
unsigned short placement_index;
char left;
char right;
char up;
char down;
char fire;

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

inline void naive_sim() {
    char num_neighbors;
    char* buffer;

    buffer = neighbor_values;
    for (i = 0; i < 1000; ++i) {
        buffer[i] = screen[i];
    }
    for (EVER) {
        for (i = 0; i < 1000; ++i) {
            last_buffer[i] = buffer[i];
        }
        for (i = 0; i < 1000; ++i) {
            num_neighbors = 0;
            if (last_buffer[i - 41] == block_char) num_neighbors++;
            if (last_buffer[i - 40] == block_char) num_neighbors++;
            if (last_buffer[i - 39] == block_char) num_neighbors++;
            if (last_buffer[i - 1] == block_char) num_neighbors++;
            if (last_buffer[i + 1] == block_char) num_neighbors++;
            if (last_buffer[i + 39] == block_char) num_neighbors++;
            if (last_buffer[i + 40] == block_char) num_neighbors++;
            if (last_buffer[i + 41] == block_char) num_neighbors++;
            if (num_neighbors < 2) {
                screen[i] = empty_char;
                buffer[i] = empty_char;
            } else if (num_neighbors > 3) {
                screen[i] = empty_char;
                buffer[i] = empty_char;
            } else if (num_neighbors == 3) {
                screen[i] = block_char;
                color[i] = block_color;
                buffer[i] = block_char;
            } else {
                screen[i] = last_buffer[i];
            }
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

inline void neighbor_list_sim() {
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

int main() {
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
                    if ((color[placement_index] & 0x0F) != block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
                    }
                    placement_index -= 40;
                    if ((color[placement_index] & 0x0F) != block_color) {
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
                    if ((color[placement_index] & 0x0F) != block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
                    }
                    placement_index += 40;
                    if ((color[placement_index] & 0x0F) != block_color) {
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
                    if ((color[placement_index] & 0x0F) != block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
                    }
                    placement_index -= 1;
                    if ((color[placement_index] & 0x0F) != block_color) {
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
                    if ((color[placement_index] & 0x0F) != block_color) {
                        screen[placement_index] = empty_char;
                        color[placement_index] = empty_color;
                    }
                    placement_index += 1;
                    if ((color[placement_index] & 0x0F) != block_color) {
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
                        (color[placement_index] & 0x0F) == block_color) {
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
            if (screen[i] == block_char && (color[i] & 0x0F) == block_color) {
                live_list[live_list_length] = i;
                live_list_length++;
            }
            neighbor_values[i] = 0;
            neighbor_list[i] = -1;
        }
        neighbor_list_sim();
    }

    return 0;
}

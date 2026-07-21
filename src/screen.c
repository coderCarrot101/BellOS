#include "ports.h"
#include "screen.h"

#define SCREEN_COLS 80
#define SCREEN_ROWS 25
#define BUFFER_ROWS 200 // Hold 200 lines of history


#define SCREEN_COLS 80
#define TOTAL_ROWS  50
#define BYTES_PER_SLOT 2
#define SCROLL_BUFFER_MAX (SCREEN_COLS * TOTAL_ROWS * BYTES_PER_SLOT)
//don't forget to limit this ya fool
// Holds 8K bytes worth of scroll memory
unsigned char scroll_buffer[SCROLL_BUFFER_MAX];
int scroll_buffer_index_front = 0;
int scroll_buffer_index_end = 0;


//implement ring buffer for scroll memory... (index mod max) for looping is interesting.

//static int scrollcountmax = 0;


#define video_memory ((volatile char*)0xB8000)

const int SCREEN_SIZE = 80 * 25;

//sets the curser to a position on the screen from 0 - 2000. 
//Each row is 80 characters long
void set_cursor_offset(int offset) {
    port_byte_out(VGA_CTRL_PORT, 14);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset >> 8)); 

    port_byte_out(VGA_CTRL_PORT, 15);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset & 0xFF));
}

//returns current curser position in memory minus the start of screen memory, 0xB8000
int get_cursor_offset() {


    int offset = 0;

    port_byte_out(VGA_CTRL_PORT, 14);
    offset = port_byte_in(VGA_DATA_PORT) << 8;

    port_byte_out(VGA_CTRL_PORT, 15);
    offset += port_byte_in(VGA_DATA_PORT);
    return offset;
    
}

//clears the screen (not the video memory)
void clear_screen() {
    char blank = 0x07;
    
    for (int i = 0; i < SCREEN_SIZE; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = blank;
    }
    set_cursor_offset(0);
}

//prints text to kernel screen
void kprint (char *message, unsigned char color) {
    char blank = 0x07;
    int char_loc = get_cursor_offset();

    while (*message != '\0') {
        if (*message == '\n') {
            char_loc = (char_loc / 80 + 1) * 80;
        } else if (*message == '\b') {
            if (char_loc > 0) {
                char_loc--;
                video_memory[char_loc * 2] = ' ';
                video_memory[char_loc * 2 + 1] = blank;
            }
        } else {
            if (char_loc < SCREEN_SIZE) {
                video_memory[char_loc * 2] = *message;
                video_memory[(char_loc * 2) + 1] = color;
                char_loc++;
            }
        }
        message++;
    }
    if (char_loc >= SCREEN_SIZE) {
        for (int i = 0; i < 80; i++) {
            scroll_buffer[scroll_buffer_index_end * 2] = video_memory[i * 2];
            scroll_buffer[scroll_buffer_index_end * 2 + 1] = video_memory[i * 2 + 1];
            scroll_buffer_index_end = (scroll_buffer_index_end + 1) % SCROLL_BUFFER_MAX;
            if (scroll_buffer_index_end < 0) {
                scroll_buffer_index_end = SCROLL_BUFFER_MAX - 1;
            }
            
        }
        for (int i = 80; i < SCREEN_SIZE; i++) {
            video_memory[(i - 80) * 2] = video_memory[i * 2];
            video_memory[(i - 80) * 2 + 1] = video_memory[i * 2 + 1];
        }
        for (int i = SCREEN_SIZE - 80; i < SCREEN_SIZE; i++) {
            video_memory[i * 2] = ' ';
            video_memory[i * 2 + 1] = blank;
        }
        char_loc = 1920;
    } else if (char_loc < 80) {
        
        for (int i = SCREEN_SIZE - 81; i >= 0; i--) {
            video_memory[(i + 80) * 2] = video_memory[i * 2];
            video_memory[(i + 80) * 2 + 1] = video_memory[i * 2 + 1];
        }



        for (int i = 0; i < 80; i++) {
            video_memory[i * 2] = scroll_buffer[scroll_buffer_index_front * 2];
            video_memory[i * 2 + 1] = scroll_buffer[scroll_buffer_index_front * 2 + 1];
            scroll_buffer_index_front = (scroll_buffer_index_front + 1) % SCROLL_BUFFER_MAX;
            if (scroll_buffer_index_front < 0) {
                scroll_buffer_index_front = SCROLL_BUFFER_MAX - 1;
            }
        }

        char_loc = 80;
    }

    set_cursor_offset(char_loc);
}

//prints text to screen and newlines after
void kprintln(char *message, unsigned char color) {
    kprint(message, color);
    kprint("\n", color);
}
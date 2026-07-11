#include "ports.h"
#include "screen.h"

#define SCREEN_COLS 80
#define SCREEN_ROWS 25
#define BUFFER_ROWS 200 // Hold 200 lines of history


#define SCREEN_COLS 80
#define TOTAL_ROWS  50
#define BYTES_PER_SLOT 2

// Holds 8K bytes worth of scroll memory
unsigned char scrollback_buffer[SCREEN_COLS * TOTAL_ROWS * BYTES_PER_SLOT];
static int scrollcountmin = 0;
static int scrollcountmax = 0;


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

    if (char_loc >= SCREEN_SIZE) {
        for (int i = 80; i < SCREEN_SIZE - 80; i+= 80) {
            for (int j = i + 80; j > i; j-= 1) {

                video_memory[j * 2] = video_memory[(j+80) * 2];
                video_memory[(j * 2) + 1] = video_memory[((j+80) * 2) + 1];                     
            }
        }
        for (int i = 1920; i <= 2000; i+= 1) {
            scrollback_buffer[scrollcountmin * 2] = video_memory[i * 2];
            scrollback_buffer[scrollcountmin * 2 + 1] = video_memory[i * 2 + 1];
            scrollcountmin++;
            video_memory[i * 2] = ' ';
            video_memory[i * 2 + 1] = blank;
        }
        char_loc = 1920;
        set_cursor_offset(char_loc);
    }
    if (char_loc < 10) {
        for (int i = SCREEN_SIZE; i > 80; i-= 80) {
            for (int j = i + 80; j > i; j-= 1) {

                video_memory[j * 2] = video_memory[(j-80) * 2];
                video_memory[(j * 2) + 1] = video_memory[((j-80) * 2) + 1];                     
            }
        }
        if (scrollcountmin > 0) {
            for (int i = 80; i >= 0; i--) {
                video_memory[i * 2] = scrollback_buffer[scrollcountmin * 2];
                video_memory[i * 2 + 1] = scrollback_buffer[scrollcountmin * 2 + 1];
                scrollcountmin--;
            }
        } else {
            for (int i = 1920; i <= 2000; i+= 1) {
                video_memory[i * 2] = ' ';
                video_memory[i * 2 + 1] = blank;
            }
        }
        char_loc = 11;
        set_cursor_offset(char_loc);
    }


    while (*message != '\0') {
        if (*message == '\n') {
            char_loc = (char_loc / 80 + 1) * 80;
        } else if (*message == '\b') {
            char_loc--;
            video_memory[char_loc * 2] = ' ';
            
        } else {
            video_memory[(char_loc * 2) + 1] = color;
            video_memory[char_loc * 2] = *message;
            char_loc++;
        }
        if (char_loc >= SCREEN_SIZE) {
            for (int i = 80; i < SCREEN_SIZE - 80; i+= 80) {
                for (int j = i + 80; j > i; j-= 1) {

                    video_memory[j * 2] = video_memory[(j+80) * 2];
                    video_memory[(j * 2) + 1] = video_memory[((j+80) * 2) + 1];                     
                }
            }
            for (int i = 1920; i <= 2000; i+= 1) {
                scrollback_buffer[scrollcountmin * 2] = video_memory[i * 2];
                scrollback_buffer[scrollcountmin * 2 + 1] = video_memory[i * 2 + 1];
                scrollcountmin++;
                video_memory[i * 2] = ' ';
                video_memory[i * 2 + 1] = blank;
            }
            char_loc = 1920;
            set_cursor_offset(char_loc);
        }
        if (char_loc < 10) {
            for (int i = SCREEN_SIZE; i > 80; i-= 80) {
                for (int j = i + 80; j > i; j-= 1) {

                    video_memory[j * 2] = video_memory[(j-80) * 2];
                    video_memory[(j * 2) + 1] = video_memory[((j-80) * 2) + 1];                     
                }
            }
            if (scrollcountmin > 0) {
                for (int i = 80; i >= 0; i--) {
                    video_memory[i * 2] = scrollback_buffer[scrollcountmin * 2];
                    video_memory[i * 2 + 1] = scrollback_buffer[scrollcountmin * 2 + 1];
                    scrollcountmin--;
                }
            } else {
                for (int i = 1920; i <= 2000; i+= 1) {
                    video_memory[i * 2] = ' ';
                    video_memory[i * 2 + 1] = blank;
                }
            }
            char_loc = 11;
            set_cursor_offset(char_loc);
        }
        message++;
        
    }
    
    set_cursor_offset(char_loc);
}

//prints text to screen and newlines after
void kprintln(char *message, unsigned char color) {
    kprint(message, color);
    kprint("\n", color);
}

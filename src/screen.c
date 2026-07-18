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
//don't forget to limit this ya fool
unsigned char scrollforward_buffer[SCREEN_COLS * TOTAL_ROWS * BYTES_PER_SLOT];
int scrollcountmin = 0;
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
// void kprint (char *message, unsigned char color) {
//     char blank = 0x07;
//     int char_loc = get_cursor_offset();
//     while (*message != '\0') {
//         if (*message == '\n') {
//             char_loc = (char_loc / 80 + 1) * 80;
//         } else if (*message == '\b') {
//             char_loc--;
//             video_memory[char_loc * 2] = ' ';

//         } else {
//             video_memory[(char_loc * 2) + 1] = color;
//             video_memory[char_loc * 2] = *message;
//             char_loc++;
//         }

//         message++;

//     }

//     if (char_loc >= SCREEN_SIZE - 80) {
//         for (int i = 0; i <= 80; i+= 1) {
//             scrollback_buffer[scrollcountmin * 2] = video_memory[i * 2];
//             scrollback_buffer[scrollcountmin * 2 + 1] = video_memory[i * 2 + 1];
//             scrollcountmin++;
//         }   
//         for (int i = 80; i < SCREEN_SIZE - 80; i+= 80) {

//             for (int j = i + 80; j > i; j-= 1) {

//                 video_memory[j * 2] = video_memory[(j+80) * 2];
//                 video_memory[(j * 2) + 1] = video_memory[((j+80) * 2) + 1];                     
//             }
//         }
//         for (int i = 1920; i <= 2000; i+= 1) {
//             video_memory[i * 2] = ' ';
//             video_memory[i * 2 + 1] = blank;
//         }
//         char_loc = SCREEN_SIZE - 160;
//         set_cursor_offset(char_loc);
//     }
//     if (char_loc < 80 && scrollcountmin > 0) {
        
//         for (int i = SCREEN_SIZE; i > 80; i-= 80) {
//             for (int j = i + 80; j > i; j-= 1) {

//                 video_memory[j * 2] = video_memory[(j-80) * 2];
//                 video_memory[(j * 2) + 1] = video_memory[((j-80) * 2) + 1];
//             }
//         }
//         for (int i = 80; i >= 0; i--) {
//             video_memory[i * 2] = scrollback_buffer[scrollcountmin * 2];
//             video_memory[i * 2 + 1] = scrollback_buffer[scrollcountmin * 2 + 1];
//             scrollcountmin--;
//         }
        
//         char_loc = 81;
//         set_cursor_offset(char_loc);
        
//     }
    
//     set_cursor_offset(char_loc);
// }

void kprint (char *message, unsigned char color) {
    char blank = 0x07;
    int char_loc = get_cursor_offset();

    // 1. Process the incoming string
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
            // Prevent writing past the screen during standard printing
            if (char_loc < SCREEN_SIZE) {
                video_memory[char_loc * 2] = *message;
                video_memory[(char_loc * 2) + 1] = color;
                char_loc++;
            }
        }
        message++;
    }

    // 2. SCROLL DOWN (Content moves UP) 
    // Triggered when cursor hits or passes the bottom row
    if (char_loc >= SCREEN_SIZE) {
        // Save the top row (row 0) to scrollback buffer before it's overwritten
        // (Assuming scrollcountmin tracks the total characters stored)
        for (int i = 0; i < 80; i++) {
            scrollback_buffer[scrollcountmin * 2] = video_memory[i * 2];
            scrollback_buffer[scrollcountmin * 2 + 1] = video_memory[i * 2 + 1];
            scrollcountmin++; 
        }

        // Shift rows 1 through 24 UP by one row (into rows 0 through 23)
        for (int i = 80; i < SCREEN_SIZE; i++) {
            video_memory[(i - 80) * 2] = video_memory[i * 2];
            video_memory[(i - 80) * 2 + 1] = video_memory[i * 2 + 1];
        }

        // Clear the newly created bottom row (row 24)
        for (int i = SCREEN_SIZE - 80; i < SCREEN_SIZE; i++) {
            video_memory[i * 2] = ' ';
            video_memory[i * 2 + 1] = blank;
        }

        // Place cursor at the start of the blank bottom row
        char_loc = SCREEN_SIZE - 80;
    }
    
    // 3. SCROLL UP (Content moves DOWN)
    // Triggered if the cursor backs up past the top row and history exists
    else if (char_loc < 80 && scrollcountmin >= 80) {
        
        // Shift rows 0 through 23 DOWN by one row (into rows 1 through 24)
        // We must loop backwards from the bottom up to avoid overwriting data
        for (int i = SCREEN_SIZE - 81; i >= 0; i--) {
            video_memory[(i + 80) * 2] = video_memory[i * 2];
            video_memory[(i + 80) * 2 + 1] = video_memory[i * 2 + 1];
        }

        // Rewind scrollback tracking by exactly one full row (160 bytes of data)
        scrollcountmin -= 80;

        // Restore history into the newly freed top row (row 0)
        for (int i = 0; i < 80; i++) {
            video_memory[i * 2] = scrollback_buffer[(scrollcountmin + i) * 2];
            video_memory[i * 2 + 1] = scrollback_buffer[(scrollcountmin + i) * 2 + 1];
        }

        // Keep the cursor clamped safely to the second row 
        char_loc = 80;
    }

    // 4. Update VGA hardware cursor position
    set_cursor_offset(char_loc);
}

//prints text to screen and newlines after
void kprintln(char *message, unsigned char color) {
    kprint(message, color);
    kprint("\n", color);
}
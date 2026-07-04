#include "ports.h"
#include "screen.h"

#define video_memory ((volatile char*)0xB8000)
const int SCREEN_SIZE = 80 * 25;

void set_cursor_offset(int offset) {
    port_byte_out(VGA_CTRL_PORT, 14);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset >> 8)); 

    port_byte_out(VGA_CTRL_PORT, 15);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset & 0xFF));
}

int get_cursor_offset() {


    int offset = 0;

    port_byte_out(VGA_CTRL_PORT, 14);
    offset = port_byte_in(VGA_DATA_PORT) << 8;

    port_byte_out(VGA_CTRL_PORT, 15);
    offset += port_byte_in(VGA_DATA_PORT);
    return offset;
    
}

void clear_screen() {
    char blank = 0x07;
    
    for (int i = 0; i < SCREEN_SIZE; i++) {
        video_memory[i * 2] = ' ';
        video_memory[i * 2 + 1] = blank;
    }
    set_cursor_offset(0);
}
void kprint (char *message) {
    int char_loc = get_cursor_offset();
    while (*message != '\0') {
        if (*message == '\n') {
            char_loc = (char_loc / 80 + 1) * 80;
        } else if (*message == '\b') {
            char_loc--;
            video_memory[char_loc * 2] = ' ';
            
        } else {
            video_memory[char_loc * 2] = *message;
            char_loc++;
        }
        message++;
        
    }
    set_cursor_offset(char_loc);
}

void kprintln(char *message) {
    kprint(message);
    kprint("\n");
}
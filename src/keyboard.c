#include "ports.h"
#include "screen.h"
#include "keyboard.h"

const char scancode_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

const char scancode_map_shifted[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};
unsigned char ascii_char_array[256];

//allows user input into the kernal
char *kscan(unsigned char color) {
    int breaker = 1;
    unsigned char scancode;
    unsigned char *p_ascii_char_array = ascii_char_array;
    static int is_shift_pressed = 0;
    while (breaker == 1) {
        if (port_byte_in(0x64) & 0x01) {
            
            scancode = port_byte_in(0x60);
            if (scancode == 0x2A || scancode == 0x36) {
                is_shift_pressed = 1;
                continue;
            }else if (scancode == 0xAA || scancode == 0xB6) {
                is_shift_pressed = 0;
                continue;
            }else if (scancode == 0x4B) {
                set_cursor_offset((get_cursor_offset() - 1));
                continue;
            } else if (scancode == 0x4D) {
                set_cursor_offset((get_cursor_offset() + 1));
                continue;
            } else if (scancode == 0x48) {
                set_cursor_offset((get_cursor_offset() - 80));
            } else if (scancode == 0x50) {
                set_cursor_offset((get_cursor_offset() + 80));
            }
            if (scancode <= 0x39) {
                
                char ascii_char;
                if (is_shift_pressed) {
                    ascii_char = scancode_map_shifted[scancode];
                } else {
                    ascii_char = scancode_map[scancode];
                }
                if (ascii_char == 27) {
                    *p_ascii_char_array = '\0';
                    breaker = 0;
                    return ascii_char_array;
                }  else if (ascii_char == '\b') {
                    *p_ascii_char_array--;
                    *p_ascii_char_array =  0;
                    char char_str[2] = {'\b', '\0'};
                    kprint(char_str, color);
                } else if (ascii_char != 0) {
                    
                    *p_ascii_char_array = ascii_char;
                    char char_str[2] = {ascii_char, '\0'};
                    kprint(char_str,color);
                    p_ascii_char_array++;
                }
            }
        }
    }
}

//kascan but newlines after
char *kscanln(unsigned char color) {
    char *out = kscan(color);
    kprint("\n", color);
    return out;
}
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
unsigned char ascii_char_array[256];

char *kscan() {
    unsigned char scancode;
    unsigned char *p_ascii_char_array = ascii_char_array;

    while (1) {
        if (port_byte_in(0x64) & 0x01) {
            scancode = port_byte_in(0x60);
            if (scancode <= 0x39) {
                char ascii_char = scancode_map[scancode];
                if (ascii_char == 27) {
                    *p_ascii_char_array = '\0';
                    return ascii_char_array;
                }else if (ascii_char != 0) {
                    
                    *p_ascii_char_array = ascii_char;
                    char char_str[2] = {ascii_char, '\0'};
                    kprint(char_str);
                    p_ascii_char_array++;
                }
            }
        }
    }
}
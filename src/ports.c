#include "ports.h"

//returns data from the specified port
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

//sends data into the specified port 
void port_byte_out(unsigned short port, unsigned char data) {
    __asm__ volatile("outb %1, %0" : : "dN" (port), "a" (data));
}
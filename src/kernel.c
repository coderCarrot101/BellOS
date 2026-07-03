#include "ports.h"
#include "screen.h"
#include "keyboard.h"

//REMEMBER TO INCREASE DISK SPACE AS PROGRAM GROWS OTHERWISE STUFF GETS WILD

void kernel_main() {
    
    clear_screen();

    kprint("Welcome to BellOS\n");
    kprint("Lo and Behold!\n");
    char *input = kscanln();
    kprintln(input);
    while(1) {
       __asm__ volatile("hlt");
    }
}

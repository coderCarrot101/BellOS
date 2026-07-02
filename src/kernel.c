#include "ports.h"
#include "screen.h"
#include "keyboard.h"
void kernel_main() {
    
    clear_screen();

    kprint("Welcome to BellOS\n");
    kprint("Lo and Behold!\n");
    char *input = kscan();
    kprintln(input);
    while(1) {
        // Infinite loop to keep the kernel alive
    }
}

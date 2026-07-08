#include "ports.h"
#include "screen.h"
#include "keyboard.h"
void display_banner(unsigned char banner_color);
//REMEMBER TO INCREASE DISK SPACE AS PROGRAM GROWS OTHERWISE STUFF GETS WILD

//Kernel enters here!
void kernel_main() {
    
    clear_screen();

    display_banner(0x0D);
    kprintln("Appreciating Annabella... OK\nBooting to disk... OK\nEntering 32-bit protected mode... OK\nUser input... OK", COLOR_GREEN);
    char *input = kscanln(COLOR_GREEN);
    kprintln(input, COLOR_GREEN);
    while(1) {
       __asm__ volatile("hlt");
    }
}

//displays the welcome message in the kernel, mostly for novelty
void display_banner(unsigned char banner_color) {


    kprint(" __      __  _                     _        \n", banner_color);
    kprint(" \\ \\    / /__| |__ ___ _ __  ___  | |_ ___  \n", banner_color);
    kprint("  \\ \\/\\/ / -_) / _/ _ \\ '  \\/ -_) |  _/ _ \\ \n", banner_color);
    kprint("   \\_/\\_/\\___|_\\__\\___/_|_|_\\___|  \\__\\___/ \n", banner_color);
    kprint("                                            \n", banner_color);
kprint("\xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB              \xB0\xDB\xDB \xB0\xDB\xDB   \xB0\xDB\xDB\xDB\xDB\xDB\xDB     \xB0\xDB\xDB\xDB\xDB\xDB\xDB   \n", banner_color);
    kprint("\xB0\xDB\xDB    \xB0\xDB\xDB             \xB0\xDB\xDB \xB0\xDB\xDB  \xB0\xDB\xDB   \xB0\xDB\xDB   \xB0\xDB\xDB   \xB0\xDB\xDB  \n", banner_color);
    kprint("\xB0\xDB\xDB    \xB0\xDB\xDB   \xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xB0\xDB\xDB \xB0\xDB\xDB \xB0\xDB\xDB     \xB0\xDB\xDB \xB0\xDB\xDB         \n", banner_color);
    kprint("\xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xB0\xDB\xDB    \xB0\xDB\xDB \xB0\xDB\xDB \xB0\xDB\xDB \xB0\xDB\xDB     \xB0\xDB\xDB  \xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \n", banner_color);
    kprint("\xB0\xDB\xDB     \xB0\xDB\xDB \xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB \xB0\xDB\xDB \xB0\xDB\xDB \xB0\xDB\xDB     \xB0\xDB\xDB         \xB0\xDB\xDB \n", banner_color);
    kprint("\xB0\xDB\xDB     \xB0\xDB\xDB \xB0\xDB\xDB        \xB0\xDB\xDB \xB0\xDB\xDB  \xB0\xDB\xDB   \xB0\xDB\xDB   \xB0\xDB\xDB   \xB0\xDB\xDB  \n", banner_color);
    kprint("\xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB   \xB0\xDB\xDB\xDB\xDB\xDB\xDB\xDB  \xB0\xDB\xDB \xB0\xDB\xDB   \xB0\xDB\xDB\xDB\xDB\xDB\xDB     \xB0\xDB\xDB\xDB\xDB\xDB\xDB   \n", banner_color);
    kprint("                                                       \n", banner_color);
}
#ifndef PORTS_H
#define PORTS_H
/**
 * --- VGA TEXT MODE DISPLAY PORTS ---
 * Used to control the text-mode screen cursor and display features.
 * * Port 0x3D4 (VGA Control/Address Register): Accepts the ID of the internal register you want to access.
 * Port 0x3D5 (VGA Data Register): Accepts the data you want to write to (or read from) that internal register.
 * * Target Register Codes for Port 0x3D4:
 * - Register 14 (0x0E): Cursor Location High Byte
 * - Register 15 (0x0F): Cursor Location Low Byte
 */
#define VGA_CTRL_PORT 0x3D4 // sets the register
#define VGA_DATA_PORT 0x3D5 // pushes or pulls data to set register

/**
 * --- PS/2 KEYBOARD CONTROLLER PORTS ---
 * Used to detect when a key is pressed or released.
 * * Port 0x60 (Data Port): Read to get the raw "scancode" of the pressed key.
 * Port 0x64 (Status/Command Port): Read to check if keyboard data is ready to be consumed.
 */
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STAT_PORT 0x64

/**
 * --- PRIMARY ATA/IDE HARD DISK PORTS ---
 * Used to read and write raw sectors directly from the hard drive without the BIOS.
 * * Port 0x1F0: Data Register (Reading/Writing actual data blocks)
 * Port 0x1F2: Sector Count Register (How many sectors to read/write)
 * Port 0x1F3: LBA Low Register (Sector numbers 0-7)
 * Port 0x1F4: LBA Mid Register (Sector numbers 8-15)
 * Port 0x1F5: LBA High Register (Sector numbers 16-23)
 * Port 0x1F6: Drive/Head Register (Selects master/slave drive and sector bits 24-27)
 * Port 0x1F7: Command/Status Register (Send 0x20 to read sectors, 0x30 to write sectors)
 */
#define ATA_DATA_PORT 0x1F0
#define ATA_CMD_PORT  0x1F7

/**
 * --- PROGRAMMABLE INTERVAL TIMER (PIT) PORTS ---
 * Used to generate hardware interrupts at a specific frequency (crucial for OS multitasking/scheduling).
 * * Port 0x40: Channel 0 Data Port (Used to set the clock frequency divider)
 * Port 0x43: Mode/Command Register (Sets the timer mode)
 */
#define PIT_DATA_PORT 0x40
#define PIT_CMD_PORT  0x43
unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

#define COLOR_BLACK  0x00
#define COLOR_BLUE   0x01
#define COLOR_GREEN  0x02
#define COLOR_CYAN   0x03
#define COLOR_RED    0x04
#define COLOR_WHITE  0x0F

#endif

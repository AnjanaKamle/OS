#include "../Headers/keyboard.h"
#include "../Headers/ports.h"

unsigned int data_avail(void){
    unsigned char status = inb(0x64);
    return (status & 0x01);
}

unsigned int Scancode_Keyboard(void){
    return inb(0x60);
}

/* Scancode Set 1 -> ASCII lookup table. Index = scancode. 0 = unmapped. */
static unsigned char scancode_ascii[] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 8,   9,   'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13,  0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n',
    'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

unsigned char ScancodeToASCII(unsigned char scancode){
    /* Break codes (release) have bit 7 set. Ignore them. */
    if (scancode & 0x80) {
        return 0;
    }
    if (scancode < sizeof(scancode_ascii)) {
        return scancode_ascii[scancode];
    }
    return 0;
}
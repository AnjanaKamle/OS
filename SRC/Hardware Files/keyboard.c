#include "../Headers/keyboard.h"
#include "../Headers/ports.h"

unsigned int data_avail(void){
    unsigned char status = inb(0x64);
    return (status & 0x01);
}

unsigned int Scancode_Keyboard(void){
    return inb(0x60);
}


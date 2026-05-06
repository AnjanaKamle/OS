#include "../Headers/ports.h"

unsigned char inb(unsigned short port){
    unsigned char code;
    asm volatile("inb %1, %0" : "=a" (code) : "dN" (port));
    return code;
}
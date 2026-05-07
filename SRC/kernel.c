
#include "Headers/ports.h"
#include "Headers/vga.h"
#include "Headers/keyboard.h"


static unsigned short *vidmem = (unsigned short*) 0xb8000;
static unsigned int cursor = 0;
typedef char string[256];
unsigned int string_index = 0;

void printClr(unsigned char *string, unsigned int color){
	for (unsigned char *ch = string; *ch; ch++){
        switch (*ch) {
            case '\n':
                cursor = ((cursor / 80)+1)*80;
                continue;
            default:
                if (cursor >= 80 * 24) {
                    for (int i = 0; i < 80 * 24; i++) {
                        vidmem[i] = vidmem[i + 80];
                    }
                    cursor = 80 * 22;
                }
                vidmem[cursor++] = (unsigned char) *ch | color;
        }
	}
}

void putChar(unsigned char c, unsigned int color) {
    if (c == 13) {
        cursor = ((cursor / 80) + 1) * 80;
        return;
    }
    if (cursor >= 80 * 24) {
        for (int i = 0; i < 80 * 24; i++) {
            vidmem[i] = vidmem[i + 80];
        }
        cursor = 80 * 22;
    }
    vidmem[cursor++] = c | color;
}


extern void kernel_main(){
    unsigned char *str = "OS Project Made By Vinu And Anjana\n";
    printClr(str, COLOR_LIGHT_CYAN);
    printClr((unsigned char*)"\nPress Keys:\n", COLOR_RED);
    
    while (1) {
        // Wait for keyboard data
        while (!data_avail()) {
            // Busy wait
        }
        
        // Read scancode
        unsigned char scancode = Scancode_Keyboard();
        
        // Convert scancode to ASCII
        unsigned char ascii = ScancodeToASCII(scancode);
        if (ascii) {
            putChar(ascii, COLOR_WHITE);
        }
    }
}
#include "Headers/keyboard.h"
#include "Headers/ports.h"
#include "Headers/vgaColors.h"


static unsigned short *vidmem = (unsigned short*) 0xb8000;
static unsigned int cursor = 0;

void printClr(unsigned char *string, unsigned int color){
	for (unsigned char *ch = string; *ch; ch++){
        if (*ch == '\n'){
            cursor = ((cursor / 80)+1)*80;
            continue;
        }
        if (cursor < 80 * 25) {
            vidmem[cursor++] = (unsigned char) *ch | color;
        }
        else {
            // Scroll the screen
            for (int i = 0; i < 80 * 24; i++) {
                vidmem[i] = vidmem[i + 80];
            }
            // Clear the last line
            for (int i = 80 * 24; i < 80 * 25; i++) {
                vidmem[i] = ' ' | color;
            }
            // Print the character
            vidmem[cursor++] = (unsigned char) *ch | color;
        }
	}
}

void hex_to_string(unsigned char value, unsigned char *buffer) {
    char* hex_digits = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    buffer[2] = hex_digits[(value >> 4) & 0x0F];
    buffer[3] = hex_digits[value & 0x0F];
    buffer[4] = '\0';
}

extern void kernel_main(){
    unsigned char *str = "Radhe Radhe Vinu\n";
    printClr(str, VGA_COLOR_LIGHT_CYAN);
    printClr((unsigned char*)"\nPress Keys:\n", VGA_COLOR_RED);

    while(1){
        while (!data_avail());
        unsigned char scancode = Scancode_Keyboard();
        unsigned char hex_buf[5];
        hex_to_string(scancode, hex_buf);
        printClr(hex_buf, VGA_COLOR_WHITE);
        printClr(" ", VGA_COLOR_WHITE);
    }
    return;
}
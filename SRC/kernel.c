
/** ** MODIFIED: Cleaned up includes - removed duplicates and old vgaColors.h reference ** **/
#include "Headers/ports.h"
#include "Headers/vga.h"
#include "Headers/keyboard.h"
#include "Headers/shell.h"

static unsigned int cursor = 0;
static unsigned short *vidmem = (unsigned short*) 0xb8000;

void printClr(unsigned char *string, unsigned int color){
	for (unsigned char *ch = string; *ch; ch++){
        switch (*ch) {
            case '\n':
                cursor = ((cursor / Width)+1)*Width;
                continue;
            default:
                if (cursor >= Width * Height) {
                    for (int i = 0; i < Width * Height; i++) {
                        vidmem[i] = vidmem[i + Width];
                    }
                    for (int i = Width * (Height - 1); i < Width * Height; i++) {
                        vidmem[i] = ' ' | COLOR_BLACK;
                    }
                    cursor = Width * (Height - 1);
                }
                vidmem[cursor++] = (unsigned char) *ch | color;
        }
	}
}

void putChar(unsigned char c, unsigned int color) {
    if (c == '^') {
        cursor = 0;
        return;
    }
    if (c == 13) {
        cursor = ((cursor / Width) + 1) * Width;
        return;
    }
    if (c == 8) {
        if (cursor > 0) cursor--;
        vidmem[cursor] = ' ' | color;   
        return;
    } 
    if (cursor >= Width * Height) {
        for (int i = 0; i < Width * Height; i++) {
            vidmem[i] = vidmem[i + Width];
        }
        for (int i = Width * (Height - 1); i < Width * Height; i++) {
            vidmem[i] = ' ' | COLOR_BLACK;
        }
        cursor = Width * (Height - 1);
    }
    vidmem[cursor++] = c | color;
}


extern void kernel_main(){
    unsigned char *str = "OS Project Made By Vinu And Anjana\n";
    printClr(str, COLOR_LIGHT_CYAN);
    printClr((unsigned char*)"\nShell Starts Here:\n", COLOR_RED);
    
    shell_init();
    shell_run();
}
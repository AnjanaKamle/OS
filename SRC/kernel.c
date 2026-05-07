
/** ** MODIFIED: Cleaned up includes - removed duplicates and old vgaColors.h reference ** **/
#include "Headers/ports.h"
#include "Headers/vga.h"
#include "Headers/keyboard.h"
#include "Headers/shell.h"


static unsigned short *vidmem = (unsigned short*) 0xb8000;
static unsigned int cursor = 0;
typedef unsigned char string[256];
unsigned int string_index = 0;
string log;

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
    if (c == 13) {
        cursor = ((cursor / Width) + 1) * Width;
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
    /** ** MODIFIED: Replaced keyboard loop with shell initialization ** **/
    shell_init();
    shell_run();
    return;
}
/** ** NEW FILE: Shell header for basic terminal commands ** **/
#ifndef SHELL_H
#define SHELL_H

void shell_init(void);
void shell_run(void);
void putChar(unsigned char c, unsigned int color);
void printClr(unsigned char *string, unsigned int color);


#endif

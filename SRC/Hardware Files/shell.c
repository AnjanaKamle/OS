/** ** NEW FILE: Basic shell implementation with cd, ls, cat, touch, pwd, clear
 * ** **/
#include "../Headers/shell.h"
#include "../Headers/keyboard.h"
#include "../Headers/ports.h"
/** ** MODIFIED: Updated to use new vga.h header (teammate's changes) ** **/
#include "../Headers/vga.h"

#define MAX_CMD_LEN 256
#define MAX_PATH_LEN 256
#define MAX_FILES 32

typedef struct {
  char name[64];
  char content[512];
  int is_dir;
} File;

typedef struct {
  File files[MAX_FILES];
  int file_count;
  char current_dir[MAX_PATH_LEN];
} Filesystem;

static unsigned short *vidmem = (unsigned short *)0xb8000;
static unsigned int cursor = 0;
static Filesystem fs;

void shell_putchar(char c, unsigned int color) {
  if (c == '\n') {
    cursor = ((cursor / 80) + 1) * 80;
    return;
  }
  if (cursor < 80 * 25) {
    vidmem[cursor++] = (unsigned char)c | color;
  }
}

void shell_print(const char *str, unsigned int color) {
  for (const char *ch = str; *ch; ch++) {
    shell_putchar(*ch, color);
  }
}

void shell_println(const char *str, unsigned int color) {
  shell_print(str, color);
  shell_putchar('\n', color);
}

void shell_clear_screen(void) {
  for (int i = 0; i < 80 * 25; i++) {
    vidmem[i] = ' ' | COLOR_WHITE;
  }
  cursor = 0;
}

/** ** MODIFIED: Expanded keyboard scancode mapping to support more keys ** **/
void shell_read_line(char *buffer, int max_len) {
  int len = 0;
  shell_putchar('>', COLOR_LIGHT_GREEN);
  shell_putchar(' ', COLOR_LIGHT_GREEN);

  while (len < max_len - 1) {
    while (!data_avail())
      ;
    unsigned char scancode = Scancode_Keyboard();

    if (scancode == 0x1C) { // Enter key
      buffer[len] = '\0';
      shell_putchar('\n', COLOR_LIGHT_GREEN);
      break;
    } else if (scancode == 0x0E) { // Backspace
      if (len > 0) {
        len--;
        shell_putchar('\b', COLOR_LIGHT_GREEN);
        shell_putchar(' ', COLOR_LIGHT_GREEN);
        shell_putchar('\b', COLOR_LIGHT_GREEN);
      }
    } else if (scancode == 0x39) { // Space
      buffer[len++] = ' ';
      shell_putchar(' ', COLOR_LIGHT_GREEN);
    }
    // Numbers 1-9, 0
    else if (scancode >= 0x02 && scancode <= 0x0D) {
      char nums[] = "1234567890";
      buffer[len++] = nums[scancode - 0x02];
      shell_putchar(nums[scancode - 0x02], COLOR_LIGHT_GREEN);
    }
    // Letters Q-P (QWERTY)
    else if (scancode >= 0x10 && scancode <= 0x19) {
      char keys[] = "qwertyuiop";
      buffer[len++] = keys[scancode - 0x10];
      shell_putchar(keys[scancode - 0x10], COLOR_LIGHT_GREEN);
    }
    // Letters A-L (ASDF row)
    else if (scancode >= 0x1E && scancode <= 0x28) {
      char keys[] = "asdfghjkl";
      buffer[len++] = keys[scancode - 0x1E];
      shell_putchar(keys[scancode - 0x1E], COLOR_LIGHT_GREEN);
    }
    // Letters Z-M (ZXCV row)
    else if (scancode >= 0x2C && scancode <= 0x32) {
      char keys[] = "zxcvbnm";
      buffer[len++] = keys[scancode - 0x2C];
      shell_putchar(keys[scancode - 0x2C], COLOR_LIGHT_GREEN);
    }
    // Special characters
    else if (scancode == 0x34) { // Period/dot
      buffer[len++] = '.';
      shell_putchar('.', COLOR_LIGHT_GREEN);
    } else if (scancode == 0x35) { // Forward slash
      buffer[len++] = '/';
      shell_putchar('/', COLOR_LIGHT_GREEN);
    } else if (scancode == 0x0C) { // Minus/hyphen
      buffer[len++] = '-';
      shell_putchar('-', COLOR_LIGHT_GREEN);
    } else if (scancode == 0x33) { // Comma
      buffer[len++] = ',';
      shell_putchar(',', COLOR_LIGHT_GREEN);
    }
  }
}

void cmd_pwd(void) {
  shell_print("Current Dir: ", COLOR_YELLOW);
  shell_println(fs.current_dir, COLOR_LIGHT_CYAN);
}

void cmd_ls(void) {
  if (fs.file_count == 0) {
    shell_println("(empty)", COLOR_LIGHT_MAGENTA);
    return;
  }
  for (int i = 0; i < fs.file_count; i++) {
    shell_print(fs.files[i].name, COLOR_LIGHT_CYAN);
    if (fs.files[i].is_dir) {
      shell_print("/", COLOR_LIGHT_CYAN);
    }
    shell_putchar(' ', COLOR_LIGHT_CYAN);
  }
  shell_putchar('\n', COLOR_LIGHT_CYAN);
}

void cmd_ls_l(void) {
  if (fs.file_count == 0) {
    shell_println("(empty)", COLOR_LIGHT_MAGENTA);
    return;
  }
  for (int i = 0; i < fs.file_count; i++) {
    if (fs.files[i].is_dir) {
      shell_print("d", COLOR_LIGHT_BLUE);
    } else {
      shell_print("-", COLOR_LIGHT_CYAN);
    }
    shell_print(" ", COLOR_LIGHT_CYAN);
    shell_print(fs.files[i].name, COLOR_LIGHT_CYAN);
    shell_putchar('\n', COLOR_LIGHT_CYAN);
  }
}

void cmd_touch(const char *filename) {
  if (fs.file_count >= MAX_FILES) {
    shell_println("ERROR: Too many files", COLOR_RED);
    return;
  }
  for (int i = 0; i < fs.file_count; i++) {
    if (filename[0] != '\0' && ((char *)filename)[0] != '\0' &&
        fs.files[i].name[0] == filename[0]) {
      shell_println("ERROR: File exists", COLOR_RED);
      return;
    }
  }
  int idx = fs.file_count++;
  int j = 0;
  while (filename[j] && j < 63) {
    fs.files[idx].name[j] = filename[j];
    j++;
  }
  fs.files[idx].name[j] = '\0';
  fs.files[idx].content[0] = '\0';
  fs.files[idx].is_dir = 0;
  shell_println("File created", COLOR_GREEN);
}

void cmd_cat(const char *filename) {
  if (filename[0] == '\0') {
    shell_println("ERROR: No filename", COLOR_RED);
    return;
  }
  for (int i = 0; i < fs.file_count; i++) {
    if (fs.files[i].name[0] == filename[0] && fs.files[i].is_dir == 0) {
      shell_println(fs.files[i].content, COLOR_LIGHT_CYAN);
      return;
    }
  }
  shell_println("ERROR: File not found", COLOR_RED);
}

void cmd_cd(const char *dirname) {
  if (dirname[0] == '.' && dirname[1] == '.' && dirname[2] == '\0') {
    fs.current_dir[0] = '/';
    fs.current_dir[1] = '\0';
  } else {
    shell_println("ERROR: Invalid directory", COLOR_RED);
  }
}

void shell_parse_command(const char *cmd) {
  char command[64];
  char arg[256];
  int i = 0, j = 0;

  while (cmd[i] && cmd[i] != ' ') {
    command[j++] = cmd[i++];
  }
  command[j] = '\0';

  if (cmd[i] == ' ')
    i++;
  j = 0;
  while (cmd[i]) {
    arg[j++] = cmd[i++];
  }
  arg[j] = '\0';

  if (command[0] == 'p' && command[1] == 'w' && command[2] == 'd') {
    cmd_pwd();
  } else if (command[0] == 'l' && command[1] == 's') {
    if (arg[0] == '-' && arg[1] == 'l') {
      cmd_ls_l();
    } else {
      cmd_ls();
    }
  } else if (command[0] == 't' && command[1] == 'o' && command[2] == 'u' &&
             command[3] == 'c' && command[4] == 'h') {
    cmd_touch(arg);
  } else if (command[0] == 'c' && command[1] == 'a' && command[2] == 't') {
    cmd_cat(arg);
  } else if (command[0] == 'c' && command[1] == 'd') {
    cmd_cd(arg);
  } else if (command[0] == 'c' && command[1] == 'l' && command[2] == 'e' &&
             command[3] == 'a' && command[4] == 'r') {
    shell_clear_screen();
  } else if (command[0] == 'h' && command[1] == 'e' && command[2] == 'l' &&
             command[3] == 'p') {
    shell_println("=== Basic Shell Commands ===", COLOR_LIGHT_MAGENTA);
    shell_println("pwd        - Print working directory",
                  COLOR_LIGHT_MAGENTA);
    shell_println("ls         - List files", COLOR_LIGHT_MAGENTA);
    shell_println("ls -l      - Detailed file list", COLOR_LIGHT_MAGENTA);
    shell_println("touch FILE - Create empty file", COLOR_LIGHT_MAGENTA);
    shell_println("cat FILE   - Display file contents", COLOR_LIGHT_MAGENTA);
    shell_println("cd ../     - Change directory", COLOR_LIGHT_MAGENTA);
    shell_println("clear      - Clear screen", COLOR_LIGHT_MAGENTA);
  } else if (command[0] != '\0') {
    shell_print("ERROR: Unknown command '", COLOR_RED);
    shell_print(command, COLOR_RED);
    shell_println("'", COLOR_RED);
  }
}

void shell_init(void) {
  fs.file_count = 0;
  fs.current_dir[0] = '/';
  fs.current_dir[1] = '\0';

  shell_clear_screen();
  shell_println("=== Basic OS Shell ===", COLOR_LIGHT_GREEN);
  shell_println("Type 'help' for commands", COLOR_LIGHT_GREEN);
  shell_putchar('\n', COLOR_BLACK);
}

void shell_run(void) {
  while (1) {
    char cmd_buffer[MAX_CMD_LEN];
    shell_read_line(cmd_buffer, MAX_CMD_LEN);
    if (cmd_buffer[0] != '\0') {
      shell_parse_command(cmd_buffer);
    }
    shell_putchar('\n', COLOR_BLACK);
  }
}

# OS

A bare-metal x86 OS built from scratch — custom bootloader, GDT, 32-bit protected mode, and a C kernel.

## Setup

### Prerequisites

| Tool | Purpose |
|---|---|
| `nasm` | Assembles `.asm` sources |
| `x86_64-elf-gcc` | Cross-compiles the C kernel (freestanding) |
| `x86_64-elf-ld` | Links kernel objects into a flat binary |
| `qemu-system-x86_64` | Emulates the OS |

On macOS, install via Homebrew:

```bash
brew install nasm qemu
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-gcc
```

### Build & Run

```bash
git clone https://github.com/adventureseeker981/OS.git
cd OS
mkdir -p Build/binaries
bash run.sh
```

`run.sh` compiles the kernel, assembles the bootloader, stitches the binaries together, and launches them in QEMU.

## Keyboard Input

The OS reads keystrokes directly from the PS/2 keyboard controller (I/O port `0x60`). Pressing a key sends a scancode to the kernel, which maps it to an ASCII character and echoes it to the VGA text buffer at `0xB8000`.

## Acknowledgements

Thanks to all the contributors and the open-source community for their support!

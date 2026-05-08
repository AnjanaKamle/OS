# My OS Learning Journey
### -Anjana

# The Beginning
I thought building an OS would take me months. Bootloaders? Assembly? Kernels? Impossible.
Then I realised I don't have to build it from scratch and that my teammate has already written the bootloader in assembly. Suddenly, I didn't need to understand x86 magic, I just needed to write C code on top of what he built.

## The YouTube Phase

I started watching keyboard input tutorials on YouTube. Copy paste and then pray it works. Most of it did, but I had no idea why.
My shell.c grew with copy-pasted functions. If something broke, I was lost. I'd spend hours searching "how to fix keyboard scancode mapping" instead of understanding what a scancode actually was.

## The Game Changing Moment

I was stuck on keyboard input again. This time, instead of searching, I read the code I'd copied and saw patterns. Scancode 0x10 = Q. Scancode 0x1E = A. It clicked: each scancode corresponded to a specific key. That's when i realised I could write this myself.
In the next couple hours, I wrote lines of if-statements, mapping scancodes to characters, and it worked!

## When Google Became Optional

After that, it just kept getting easier. I realied less on the internet and more on myself and started building a functioning shell with working commands.

**Thankful to my teammate @adventureseeker981 for introducing me to this project and for building the foundation OS code**
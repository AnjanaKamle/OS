[org 0x7c00]
; Jumping to the start
jmp start
;Setting kernel Now at address 1000
KERNEL_START equ 0x1000

;Booting From The Disk
BOOT_DISK: DB 0

start:

    mov [BOOT_DISK], dl
    xor ax, ax
    mov es, ax
    mov ds, ax
    mov bp, 0x8000
    mov sp, bp

    mov bx, KERNEL_START
    mov dh, 20

    mov ah, 0x02
    mov al, dh
    mov ch, 0
    mov dh, 0
    mov cl, 2
    mov dl, [BOOT_DISK]
    int 0x13
    jc Disk_error

    mov ah, 0x0
    mov al, 0x3
    int 0x10
    jmp No_Error

Disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    jmp $

No_Error:

    CODE_SEG equ code_descriptor - GDT_Start
    DATA_SEG equ data_descriptor - GDT_Start

    cli

    lgdt [GDT_descriptor]
    mov eax, cr0        ; last bit of cr0 = 1
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_PM
    jmp $

GDT_Start:
    null_descriptor:
        dd 0x0 ; four times 00000000
        dd 0x0 ; four times 00000000
    code_descriptor:
        dw 0xffff
        dw 0x0 ; 16 bit +
        db 0x0 ; 8 bits = 24 bits 
        db 0b10011010
        ;  pre
        ;   priv
        ;    type flags
        db 0b11001111
        db 0x0
    data_descriptor:
        dw 0xffff
        dw 0x0 ; 16 bit +
        db 0x0 ; 8 bits = 24 bits 
        db 0b10010010
        ;    pre
        ;     priv
        ;      type flags
        db 0b11001111
        db 0
        
GDT_end:

GDT_descriptor:
    dw GDT_end-GDT_Start-1
    dd GDT_Start

[bits 32]
start_PM:
    mov ax, DATA_SEG
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x9000
    mov esp, ebp
    
    jmp KERNEL_START

times 510-($-$$) db 0
dw 0xAA55


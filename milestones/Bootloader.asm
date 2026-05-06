mov bx, 0
mov ah, 0x0e
jmp loop0
char:
	times 256 db 0 
	jmp loop0

String:
	db 10,"Hello, Welcome to my bootloader! ",0

loop0:
	mov al, [String+bx]
	int 0x10
	inc bx
	cmp al, 0
	je end
	jmp loop0

end:
	jmp $
	times 510-($-$$) db 0
	db 0x55, 0xaa
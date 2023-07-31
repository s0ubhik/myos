section .multiboot
magic dd 0x1badb002
flags dd (1 << 0 | 1 << 1)
check dd 0xe4524ffb ; magic + flags

section .text
extern kernel_main
global loader
global kernel_stack
global pop_func
global start_user_mode
global tss_kernel_stack
tss_kernel_stack dd kernel_stack

loader:
    mov esp, kernel_stack
    push kernel_stack
    call kernel_main
    jmp _stop

_stop:
    cli
    hlt
    jmp _stop

start_user_mode:
	mov ax, (4 * 8) | 3 
	mov ds, ax 
	mov es, ax 
	mov fs, ax
	mov gs, ax

	mov esp, user_stack
	mov ebp, esp

	mov eax, esp
	push (4 * 8) | 3
	push eax
	pushf
	push (3 * 8) | 3 
	push user_code
	iret

user_code:
    int 0x80
    int 0x80
    int 0x80
    int 0x80
    int 0x80
    jmp $
pop_func:
	pop eax
	pop eax
	pop eax
	ret
jmp $

section .bss
resb 2*1024*1024 ; 2 MiB
kernel_stack:
resb 2*1024*1024 
user_stack:

section .multiboot
magic dd 0x1badb002
flags dd (1 << 0 | 1 << 1)
check dd 0xe4524ffb ; magic + flags

section .text
extern kernel_main
global loader

loader:
    mov esp, kernel_stack
    push kernel_stack
    call kernel_main

_stop:
    cli
    hlt
    jmp _stop

section .bss
resb 2*1024*1024 ; 2 MiB

kernel_stack:

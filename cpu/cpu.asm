section .text
global syscall_handle
extern check_syscall

syscall_handle:
    cli
    push ebp
    mov ebp, esp

    push edx
    push ecx
    push ebx
    push eax

    call check_syscall
    mov esi, eax ; Return value

    pop eax
    pop ebx
    pop ecx
    pop edx

    mov eax, esi

    mov esp, ebp
    pop ebp
    sti
    iret

jmp $
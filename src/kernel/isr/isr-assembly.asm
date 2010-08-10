extern isrHandler
bits 32
%macro ISR_COMMON 2
        [GLOBAL isr%1]
isr%1:
        cli
%if %2 = 0
        push byte 0
%endif
        push byte %1
        jmp isr_common_stub
%endmacro

%macro ISRS 3
%assign i %1
%rep %2
        ISR_COMMON i, %3
%assign i i+1
%endrep
%endmacro

        ISRS 0, 8, 0            ; 0..7 no error
        ISRS 8, 1, 1            ; 8 error
        ISRS 9, 1, 0            ; 9 no error
        ISRS 10, 5, 1           ; 10..14 error
        ISRS 15, 17, 0          ; 15..31 no error

isr_common_stub:
        pusha                   ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
        push ds
        push es
        push fs
        push gs
        mov ax, 0x10            ; Load the Kernel Data Segment descriptor!
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov eax, esp            ; Push us the stack
        push eax
        mov eax, isrHandler
        call eax            ; A special call, preserves the 'eip' register
        pop eax
        pop gs
        pop fs
        pop es
        pop ds
        popa
        add esp, 8 ; Cleans up the pushed error code and pushed ISR number
        iret       ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!

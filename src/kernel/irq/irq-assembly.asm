extern irqHandler
bits 32
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

%macro IRQS 3
%assign i %1
%assign j %2
%rep %3
        IRQ i, j
%assign i i+1
%assign j j+1
%endrep
%endmacro

IRQS 0, 32, 16

; This is our common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
irq_common_stub:
        pusha                    ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
        push ds
        push es
        push fs
        push gs
        mov ax, ds               ; Lower 16-bits of eax = ds.
        push eax                 ; save the data segment descriptor

        mov ax, 0x10  ; load the kernel data segment descriptor
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax

        call irqHandler

        pop ebx        ; reload the original data segment descriptor
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx

        pop gs
        pop fs
        pop es
        pop ds
        popa                     ; Pops edi,esi,ebp...
        add esp, 8     ; Cleans up the pushed error code and pushed ISR number
        sti
        iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP


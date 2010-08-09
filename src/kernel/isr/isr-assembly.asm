[EXTERN isr_common_stub]

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
        ISRS 15, 16, 0          ; 15..31 no error

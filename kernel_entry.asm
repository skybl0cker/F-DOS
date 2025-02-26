[bits 16]                   ; Still in real mode
[global start]
[extern kernel_main]

start:
    cli                     ; Disable interrupts
    call kernel_main        ; Call the C kernel
    jmp $                   ; Hang if kernel returns

section .text
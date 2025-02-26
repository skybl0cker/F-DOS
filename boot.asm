[org 0x7c00]
start:
    mov [boot_drive], dl
    xor ax, ax
    mov ds, ax
    mov es, ax

load_kernel:
    mov ah, 0x00
    mov dl, [boot_drive]
    int 0x13
    jc disk_reset_fail
    mov si, reset_ok_msg
    call print_string

    mov ah, 0x02
    mov al, 1               ; Try 1 sector
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov dl, [boot_drive]
    mov bx, 0x1000
    mov es, bx
    xor bx, bx
    int 0x13
    jc read_fail
    mov si, read_ok_msg
    call print_string
    jmp 0x1000:0x0000

disk_reset_fail:
    mov si, reset_fail_msg
    call print_string
    jmp hang

read_fail:
    mov si, read_fail_msg
    call print_string
    call print_hex          ; Print error code in ah
    jmp hang

hang:
    jmp hang

print_string:
    lodsb
    test al, al
    jz return
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    jmp print_string
return:
    ret

; Print ah as hex
print_hex:
    push ax
    mov al, ah
    shr al, 4               ; High nibble
    call print_nibble
    mov al, ah
    and al, 0x0F            ; Low nibble
    call print_nibble
    pop ax
    ret

print_nibble:
    cmp al, 9
    jbe .digit
    add al, 'A' - 10
    jmp .print
.digit:
    add al, '0'
.print:
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    ret

reset_ok_msg:    db "Disk reset OK", 0
read_ok_msg:     db "Read OK", 0
reset_fail_msg:  db "Disk reset failed", 0
read_fail_msg:   db "Read failed: 0x", 0
msg:             db "Boot...", 0
boot_drive:      db 0

times 510-($-$$) db 0
dw 0xAA55
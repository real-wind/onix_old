[org 0x7c00]

; 设置屏幕模式为文本模式，清除屏幕
mov ax, 3
int 0x10

; 初始化段寄存器
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

xchg bx, bx
; call interrupt #普通调用

; ; 构建一个中断向量 interrupt
; mov word [0x80 * 4], interrupt ; 该中断向量 ip
; mov word [0x80 * 4 + 2], 0 ; 该中断向量的 cs

; 构建一个中断向量 interrupt
mov word [0 * 4], interrupt ; 该中断向量 ip
mov word [0 * 4 + 2], 0 ; 该中断向量的 cs

; int 0x80 ; linux 系统调用号 invoke

mov dx, 0
mov ax, 1
mov bx, 0

xchg bx, bx

div bx ; dx : ax / bx

; 阻塞
jmp $

interrupt:
    mov si, string
    call print
    xchg bx, bx
    ; ret  ; 普通返回
    iret  ; 中断返回


print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

string:
    db ".", 0; 

; 填充 0
times 510 - ($ - $$) db 0

; 主引导扇区的最后两个字节必须是 0x55 0xaa
; dw 0xaa55
db 0x55, 0xaa



[org 0x7c00]

; ������ĻģʽΪ�ı�ģʽ�������Ļ
mov ax, 3
int 0x10

; ��ʼ���μĴ���
mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

xchg bx, bx
; call interrupt #��ͨ����

; ; ����һ���ж����� interrupt
; mov word [0x80 * 4], interrupt ; ���ж����� ip
; mov word [0x80 * 4 + 2], 0 ; ���ж������� cs

; ����һ���ж����� interrupt
mov word [0 * 4], interrupt ; ���ж����� ip
mov word [0 * 4 + 2], 0 ; ���ж������� cs

; int 0x80 ; linux ϵͳ���ú� invoke

mov dx, 0
mov ax, 1
mov bx, 0

xchg bx, bx

div bx ; dx : ax / bx

; ����
jmp $

interrupt:
    mov si, string
    call print
    xchg bx, bx
    ; ret  ; ��ͨ����
    iret  ; �жϷ���


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

; ��� 0
times 510 - ($ - $$) db 0

; ��������������������ֽڱ����� 0x55 0xaa
; dw 0xaa55
db 0x55, 0xaa



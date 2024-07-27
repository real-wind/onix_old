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

mov si, booting
call print

xchg bx, bx

mov edi, 0x1000; ��ȡ��Ŀ���ڴ�
mov ecx, 2; ��ʼ����
mov bl, 4; ��������

call read_disk

; xchg bx, bx

cmp word [0x1000], 0x55aa; ��Ϊloader��[0x1000] dd 0x55aa
jnz error

jmp 0:0x1002; ��ӡ�ַ���λ�ã���ȡ��loader�д�ӡ�ַ�����

; ����
jmp $

read_disk:

    ; ���ö�д����������
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx; 0x1f3
    mov al, cl; ��ʼ������ǰ��λ
    out dx, al

    inc dx; 0x1f4
    shr ecx, 8
    mov al, cl; ��ʼ�������а�λ
    out dx, al

    inc dx; 0x1f5
    shr ecx, 8
    mov al, cl; ��ʼ�����ĸ߰�λ
    out dx, al

    inc dx; 0x1f6
    shr ecx, 8
    and cl, 0b1111; ������λ��Ϊ 0

    mov al, 0b1110_0000;
    or al, cl
    out dx, al; ���� - LBA ģʽ

    inc dx; 0x1f7
    mov al, 0x20; ��Ӳ��
    out dx, al

    xor ecx, ecx; �� ecx ���
    mov cl, bl; �õ���д����������

    .read:
        push cx; ���� cx
        call .waits; �ȴ�����׼�����
        call .reads; ��ȡһ������
        pop cx; �ָ� cx
        loop .read; ÿ��loop��дһ������

    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx
            jmp $+2; nop ֱ����ת����һ��
            jmp $+2; һ����ӳ�
            jmp $+2
            and al, 0b1000_1000
            cmp al, 0b0000_1000
            jnz .check
        ret

    .reads:
        mov dx, 0x1f0
        mov cx, 256; һ������ 256 ��
        .readw:
            in ax, dx
            jmp $+2; һ����ӳ� 
            jmp $+2
            jmp $+2
            mov [edi], ax
            add edi, 2
            loop .readw; ÿ��loop��ȡһ����
        ret

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

booting:
    db "Booting Onix...", 10, 13, 0; \n\r

error:
    mov si, .msg
    call print
    hlt; �� CPU ֹͣ
    jmp $
    .msg db "Booting Error!!!", 10, 13, 0

; ��� 0
times 510 - ($ - $$) db 0

; ��������������������ֽڱ����� 0x55 0xaa
; dw 0xaa55
db 0x55, 0xaa



[bits 32]; NASM��һ��αָ����ڸ��߻��������32λ����

extern kernel_init

global _start
_start:
    ; mov byte [0xb8000], 'K'; ��ʾ�������ں�
    ; xchg bx, bx

    call kernel_init
    xchg bx, bx
    int 0x80 ; ���� 0x80 �жϺ��� ϵͳ����

    ; mov bx, 0
    ; div bx
    
    jmp $; ����
[bits 32]; NASM��һ��αָ����ڸ��߻��������32λ����

extern kernel_init

global _start
_start:
    ; mov byte [0xb8000], 'K'; ��ʾ�������ں�
    ; xchg bx, bx
    call kernel_init
    ; xchg bx, bx
    jmp $; ����
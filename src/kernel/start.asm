[bits 32]; NASM��һ��αָ����ڸ��߻��������32λ����

extern kernel_init

global _start
_start:


    call kernel_init

    
    jmp $; ����
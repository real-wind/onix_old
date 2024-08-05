[bits 32]

extern console_init
extern gdt_init
extern memory_init
extern kernel_init

global _start
_start:
    push ebx; ards_count
    push eax; magic

    call console_init   ; ����̨��ʼ��
    call gdt_init       ; ȫ����������ʼ��
    call memory_init    ; �ڴ��ʼ��
    call kernel_init    ; �ں˳�ʼ��

    xchg bx, bx

    mov eax, 0; 0 ��ϵͳ����
    int 0x80;

    jmp $; ����
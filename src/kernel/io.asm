[bits 32]

section .text; �����

global inb ; �� inb ����
inb:
    push ebp ; �����ָ��
    mov ebp, esp ; ���û�ָ��

    xor eax, eax ; ��� eax �Ĵ���
    mov edx, [ebp + 8] ; ���˿ںż��ص� edx
    in al, dx ; �Ӷ˿� dx ��ȡ 8 bit �� al

    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ

    leave ; �ָ�ջ֡
    ret  ; ����

global outb
outb:
    push ebp
    mov ebp, esp ; ���û�ָ��

    mov edx, [ebp + 8] ; port
    mov eax, [ebp + 12] ; value
    out dx, al ; �� al �е� 8bit ������˿� dx

    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ

    leave ; �ָ�ջ֡
    ret ; ����

global inw ; �� inw ����
inw:
    push ebp ; �����ָ��
    mov ebp, esp ; ���û�ָ��

    xor eax, eax ; ��� eax �Ĵ���
    mov edx, [ebp + 8] ; ���˿ںż��ص� edx
    in ax, dx ; �Ӷ˿� dx ��ȡ 8 bit �� al

    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ

    leave ; �ָ�ջ֡
    ret  ; ����

global outw
outw:
    push ebp
    mov ebp, esp ; ���û�ָ��

    mov edx, [ebp + 8] ; port
    mov eax, [ebp + 12] ; value
    out dx, ax ; �� ax �е� 8bit ������˿� dx

    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ
    jmp $+2 ; һ�����Ծ

    leave ; �ָ�ջ֡
    ret ; ����
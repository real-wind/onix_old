[bits 32]

section .text

global task_switch
task_switch:
    push ebp          ; ����ɵĻ�ָ��
    mov ebp, esp      ; �����µĻ�ָ��

    push ebx          ; ���� ebx �Ĵ�����ֵ
    push esi          ; ���� esi �Ĵ�����ֵ
    push edi          ; ���� edi �Ĵ�����ֵ

    mov eax, esp      ; ��ջָ���ֵ���Ƶ� eax
    and eax, 0xfffff000 ; �� eax ���뵽ҳ��߽磬ͨ�����ڶ�λ��ǰ����Ķ�ջ

    mov [eax], esp    ; ���浱ǰ�����ջָ�� �� esp��ջָ�룩�ĵ�ǰֵ�洢�� eax ��ָ����ڴ�λ��

    mov eax, [ebp + 8] ; ��ȡ��һ������Ķ�ջָ��
    mov esp, [eax]    ; �л�����һ�������ջ

    pop edi           ; �ָ���һ������� edi �Ĵ�����ֵ
    pop esi           ; �ָ���һ������� esi �Ĵ�����ֵ
    pop ebx           ; �ָ���һ������� ebx �Ĵ�����ֵ
    pop ebp           ; �ָ���һ������Ļ�ָ��

    ret               ; ���أ�����ִ��������

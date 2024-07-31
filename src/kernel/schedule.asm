[bits 32]

section .text

global task_switch
task_switch:
    push ebp          ; 保存旧的基指针
    mov ebp, esp      ; 设置新的基指针

    push ebx          ; 保存 ebx 寄存器的值
    push esi          ; 保存 esi 寄存器的值
    push edi          ; 保存 edi 寄存器的值

    mov eax, esp      ; 将栈指针的值复制到 eax
    and eax, 0xfffff000 ; 将 eax 对齐到页面边界，通常用于定位当前任务的堆栈

    mov [eax], esp    ; 保存当前任务的栈指针 将 esp（栈指针）的当前值存储到 eax 所指向的内存位置

    mov eax, [ebp + 8] ; 获取下一个任务的堆栈指针
    mov esp, [eax]    ; 切换到下一个任务的栈

    pop edi           ; 恢复下一个任务的 edi 寄存器的值
    pop esi           ; 恢复下一个任务的 esi 寄存器的值
    pop ebx           ; 恢复下一个任务的 ebx 寄存器的值
    pop ebp           ; 恢复下一个任务的基指针

    ret               ; 返回，继续执行新任务

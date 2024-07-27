[bits 32]

section .text; 代码段

global inb ; 将 inb 导出
inb:
    push ebp ; 保存基指针
    mov ebp, esp ; 设置基指针

    xor eax, eax ; 清空 eax 寄存器
    mov edx, [ebp + 8] ; 将端口号加载到 edx
    in al, dx ; 从端口 dx 读取 8 bit 到 al

    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃

    leave ; 恢复栈帧
    ret  ; 返回

global outb
outb:
    push ebp
    mov ebp, esp ; 设置基指针

    mov edx, [ebp + 8] ; port
    mov eax, [ebp + 12] ; value
    out dx, al ; 将 al 中的 8bit 输出到端口 dx

    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃

    leave ; 恢复栈帧
    ret ; 返回

global inw ; 将 inw 导出
inw:
    push ebp ; 保存基指针
    mov ebp, esp ; 设置基指针

    xor eax, eax ; 清空 eax 寄存器
    mov edx, [ebp + 8] ; 将端口号加载到 edx
    in ax, dx ; 从端口 dx 读取 8 bit 到 al

    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃

    leave ; 恢复栈帧
    ret  ; 返回

global outw
outw:
    push ebp
    mov ebp, esp ; 设置基指针

    mov edx, [ebp + 8] ; port
    mov eax, [ebp + 12] ; value
    out dx, ax ; 将 ax 中的 8bit 输出到端口 dx

    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃
    jmp $+2 ; 一点短跳跃

    leave ; 恢复栈帧
    ret ; 返回
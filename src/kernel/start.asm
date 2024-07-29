[bits 32]; NASM的一个伪指令，用于告诉汇编器生成32位代码

extern kernel_init

global _start
_start:
    ; mov byte [0xb8000], 'K'; 表示进入了内核
    ; xchg bx, bx

    call kernel_init
    xchg bx, bx
    int 0x80 ; 调用 0x80 中断函数 系统调用

    ; mov bx, 0
    ; div bx
    
    jmp $; 阻塞
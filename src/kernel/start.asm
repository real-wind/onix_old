[bits 32]; NASM的一个伪指令，用于告诉汇编器生成32位代码

extern kernel_init

global _start
_start:


    call kernel_init

    
    jmp $; 阻塞
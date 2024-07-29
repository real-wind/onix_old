#include <onix/onix.h>
#include <onix/types.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/printk.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/global.h>
#include <onix/stdlib.h>



// 初始化内核时调用的函数
void kernel_init()
{
    console_init();       // 初始化控制台

    gdt_init();
    interrupt_init();
    // task_init();

    asm volatile(  // asm 关键字是用来标示内联汇编语句开始 volatile 用于指示编译器不要优化这段汇编代码，确保每次程序执行到这里时都会实际执行这些汇编指令
        "sti\n"  // set interrupt flag"，用于启用中断
        "movl %eax, %eax\n");  // 为了演示\n

    u32 counter = 0;
    while (true)
    {
        DEBUGK("looping in kernel init %d...\n", counter++);
        delay(100000000);
    }

    return;
}


#include <onix/task.h>
#include <onix/printk.h>
#include <onix/debug.h>

#define PAGE_SIZE 0x1000  // 定义页面大小为 4096 字节

// 静态定义两个任务的起始地址
task_t *a = (task_t *)0x1000; // 和task_t *a = 0x1000;效果一样   将内存地址 0x1000 转换为 task_t 类型指针，并赋值给 a
task_t *b = (task_t *)0x2000;

// 声明外部的任务切换函数
extern void task_switch(task_t *next); // 汇编实现 schedual.asm

// 获取当前运行的任务
task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"         // 将栈指针 esp 的值移动到 eax
        "andl $0xfffff000, %eax\n"   // 对 eax 应用掩码，以获取当前任务的堆栈顶部地址
    );
}

// 调度函数
void schedule()
{
    task_t *current = running_task();  // 获取当前正在运行的任务
    task_t *next = current == a ? b : a;  // 决定下一个要运行的任务，如果当前是a则切换到b，反之亦然
    task_switch(next);  // 调用任务切换函数
}

u32 thread_a()
{
    while (true)      // 无限循环
    {
        printk("A");  // 打印字符 "A" 到控制台
        schedule();   // 调用调度函数，允许切换到其他线程
    }
}

u32 thread_b()
{
    while (true)      // 无限循环
    {
        printk("B");  // 打印字符 "B" 到控制台
        schedule();   // 调用调度函数，允许切换到其他线程
    }
}


// 创建一个新任务，设置其堆栈和初始寄存器状态
static void task_create(task_t *task, target_t target)
{
    // 计算新任务的堆栈顶部位置
    // u32 *stack = (u32 *)task + PAGE_SIZE;
    u32 stack = (u32)task + PAGE_SIZE;

    // 为任务帧留出空间，并调整堆栈指针
    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;

    // 初始化任务帧的寄存器值
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;  // 设置任务的入口点

    // 将堆栈指针更新到任务结构体
    task->stack = (u32 *)stack;
}


// 初始化系统任务
void task_init()
{
    // 创建两个任务
    task_create(a, thread_a);
    task_create(b, thread_b);

    // 调用调度器启动任务调度
    schedule();
}

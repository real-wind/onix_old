#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include <onix/types.h>

typedef u32 target_t(); // typedef 定义了一个新的类型 target_t，这是一个函数指针类型，指向返回 u32 类型并不接受任何参数的函数

typedef struct task_t
{
    u32 *stack; // 一个指向 u32 类型的指针，代表这个任务的内核栈顶地址
} task_t;

typedef struct task_frame_t
{
    u32 edi;
    u32 esi;
    u32 ebx;
    u32 ebp;
    void (*eip)(void);
} task_frame_t;

void task_init();

#endif
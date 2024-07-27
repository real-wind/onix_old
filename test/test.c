// #include <onix/stdio.h>
#include <stdio.h>
#include <onix/types.h>
#include <onix/printk.h>

#define PAGE_SIZE 0x1000

// typedef struct {
//     char data[128]; // 定义一个包含128字节数据的结构体
// } task_t;

typedef struct task_t
{
    u32 *stack; // 一个指向 u32 类型的指针，代表这个任务的内核栈顶地址
} task_t;

int main() {
    task_t *a = (task_t *)0x1000;
    u32 *stack = (u32 *)a + PAGE_SIZE;
    u32 *stack1 = (u32 *)(a + PAGE_SIZE);
    u32 *stack2 = (u32 *)a + 1000;
    printf("Address of a: %p\n", (void *)a);
    printf("Address of stack: %p\n", (void *)stack);
    printf("Address of stack1: %p\n", (void *)stack1);
    printf("Address of stack2: %p\n", (void *)stack2);
    // printk("Address of a: %p\n", (void *)a);
    // printk("Address of stack: %p\n", (void *)stack);

    return 0;
}

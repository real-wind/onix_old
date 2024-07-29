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



// ��ʼ���ں�ʱ���õĺ���
void kernel_init()
{
    console_init();       // ��ʼ������̨

    gdt_init();
    interrupt_init();
    // task_init();

    asm volatile(  // asm �ؼ�����������ʾ���������俪ʼ volatile ����ָʾ��������Ҫ�Ż���λ����룬ȷ��ÿ�γ���ִ�е�����ʱ����ʵ��ִ����Щ���ָ��
        "sti\n"  // set interrupt flag"�����������ж�
        "movl %eax, %eax\n");  // Ϊ����ʾ\n

    u32 counter = 0;
    while (true)
    {
        DEBUGK("looping in kernel init %d...\n", counter++);
        delay(100000000);
    }

    return;
}


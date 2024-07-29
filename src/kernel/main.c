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
    task_init();

    return;
}


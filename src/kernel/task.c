#include <onix/task.h>
#include <onix/printk.h>
#include <onix/debug.h>

#define PAGE_SIZE 0x1000  // ����ҳ���СΪ 4096 �ֽ�

// ��̬���������������ʼ��ַ
task_t *a = (task_t *)0x1000; // ��task_t *a = 0x1000;Ч��һ��   ���ڴ��ַ 0x1000 ת��Ϊ task_t ����ָ�룬����ֵ�� a
task_t *b = (task_t *)0x2000;

// �����ⲿ�������л�����
extern void task_switch(task_t *next); // ���ʵ�� schedual.asm

// ��ȡ��ǰ���е�����
task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"         // ��ջָ�� esp ��ֵ�ƶ��� eax
        "andl $0xfffff000, %eax\n"   // �� eax Ӧ�����룬�Ի�ȡ��ǰ����Ķ�ջ������ַ
    );
}

// ���Ⱥ���
void schedule()
{
    task_t *current = running_task();  // ��ȡ��ǰ�������е�����
    task_t *next = current == a ? b : a;  // ������һ��Ҫ���е����������ǰ��a���л���b����֮��Ȼ
    task_switch(next);  // ���������л�����
}

u32 thread_a()
{
    while (true)      // ����ѭ��
    {
        printk("A");  // ��ӡ�ַ� "A" ������̨
        schedule();   // ���õ��Ⱥ����������л��������߳�
    }
}

u32 thread_b()
{
    while (true)      // ����ѭ��
    {
        printk("B");  // ��ӡ�ַ� "B" ������̨
        schedule();   // ���õ��Ⱥ����������л��������߳�
    }
}


// ����һ���������������ջ�ͳ�ʼ�Ĵ���״̬
static void task_create(task_t *task, target_t target)
{
    // ����������Ķ�ջ����λ��
    // u32 *stack = (u32 *)task + PAGE_SIZE;
    u32 stack = (u32)task + PAGE_SIZE;

    // Ϊ����֡�����ռ䣬��������ջָ��
    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;

    // ��ʼ������֡�ļĴ���ֵ
    frame->ebx = 0x11111111;
    frame->esi = 0x22222222;
    frame->edi = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)target;  // �����������ڵ�

    // ����ջָ����µ�����ṹ��
    task->stack = (u32 *)stack;
}


// ��ʼ��ϵͳ����
void task_init()
{
    // ������������
    task_create(a, thread_a);
    task_create(b, thread_b);

    // ���õ����������������
    schedule();
}

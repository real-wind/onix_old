#ifndef ONIX_TASK_H
#define ONIX_TASK_H

#include <onix/types.h>

#define KERNEL_USER 0
#define NORMAL_USER 1

#define TASK_NAME_LEN 16

typedef u32 target_t(); // typedef ������һ���µ����� target_t������һ������ָ�����ͣ�ָ�򷵻� u32 ���Ͳ��������κβ����ĺ���

typedef enum task_state_t
{
    TASK_INIT,     // ��ʼ��
    TASK_RUNNING,  // ִ��
    TASK_READY,    // ����
    TASK_BLOCKED,  // ����
    TASK_SLEEPING, // ˯��
    TASK_WAITING,  // �ȴ�
    TASK_DIED,     // ����
} task_state_t;

typedef struct task_t
{
    u32 *stack;               // �ں�ջ
    task_state_t state;       // ����״̬
    u32 priority;             // �������ȼ�
    u32 ticks;                // ʣ��ʱ��Ƭ
    u32 jiffies;              // �ϴ�ִ��ʱȫ��ʱ��Ƭ
    char name[TASK_NAME_LEN]; // ������
    u32 uid;                  // �û� id
    u32 pde;                  // ҳĿ¼�����ַ
    struct bitmap_t *vmap;    // ���������ڴ�λͼ
    u32 magic;                // �ں�ħ�������ڼ��ջ���
} task_t;

typedef struct task_frame_t
{
    u32 edi;
    u32 esi;
    u32 ebx;
    u32 ebp;
    void (*eip)(void);
} task_frame_t;

task_t *running_task();
void schedule();

void task_yield();

#endif
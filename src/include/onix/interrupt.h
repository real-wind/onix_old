#ifndef ONIX_INTERRUPT_H
#define ONIX_INTERRUPT_H

#include <onix/types.h>

#define IDT_SIZE 256
#define IRQ_CLOCK 0      // ʱ��
#define IRQ_KEYBOARD 1   // ����
#define IRQ_CASCADE 2    // 8259 ��Ƭ������
#define IRQ_SERIAL_2 3   // ���� 2
#define IRQ_SERIAL_1 4   // ���� 1
#define IRQ_PARALLEL_2 5 // ���� 2
#define IRQ_FLOPPY 6     // ���̿�����
#define IRQ_PARALLEL_1 7 // ���� 1
#define IRQ_RTC 8        // ʵʱʱ��
#define IRQ_REDIRECT 9   // �ض��� IRQ2
#define IRQ_MOUSE 12     // ���
#define IRQ_MATH 13      // Э������ x87
#define IRQ_HARDDISK 14  // ATA Ӳ�̵�һͨ��
#define IRQ_HARDDISK2 15 // ATA Ӳ�̵ڶ�ͨ��

#define IRQ_MASTER_NR 0x20 // ��Ƭ��ʼ������
#define IRQ_SLAVE_NR 0x28  // ��Ƭ��ʼ������

typedef struct gate_t
{
    u16 offset0;    // ����ƫ�� 0 ~ 15 λ
    u16 selector;   // �����ѡ����
    u8 reserved;    // ��������
    u8 type : 4;    // ������/�ж���/������
    u8 segment : 1; // segment = 0 ��ʾϵͳ��
    u8 DPL : 2;     // ʹ�� int ָ����ʵ����Ȩ��
    u8 present : 1; // �Ƿ���Ч
    u16 offset1;    // ����ƫ�� 16 ~ 31 λ
} _packed gate_t;

typedef void *handler_t; // �жϴ�����

void send_eoi(int vector);

// �����жϴ�����
void set_interrupt_handler(u32 irq, handler_t handler);
void set_interrupt_mask(u32 irq, bool enable);


bool interrupt_disable();             // ��� IF λ����������֮ǰ��ֵ
bool get_interrupt_state();           // ��� IF λ
void set_interrupt_state(bool state); // ���� IF λ

#endif
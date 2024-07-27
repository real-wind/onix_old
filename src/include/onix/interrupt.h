#ifndef ONIX_INTERRUPT_H
#define ONIX_INTERRUPT_H

#include <onix/types.h>

#define IDT_SIZE 256

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

void interrupt_init();

#endif
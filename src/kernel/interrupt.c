#include <onix/global.h>
#include <onix/debug.h>
#include <onix/interrupt.h>

gate_t idt[IDT_SIZE];
pointer_t idt_ptr;

extern void interrupt_handler();

// ��ʼ���ж�������
void interrupt_init() 
{
    for (size_t i = 0; i < IDT_SIZE; i++) 
    {
        gate_t *gate = &idt[i];
        gate->offset0 = (u32)interrupt_handler & 0xffff;
        gate->offset1 = ((u32)interrupt_handler >> 16) & 0xffff;
        gate->selector = 1 << 3; // �����
        gate->reserved = 0; // ��������
        gate->type = 0b1110; // �ж���
        gate->segment = 0; // ��ѡ��
        gate->DPL = 0; // ��Ȩ��
        gate->present = 1; // ����
    }
    idt_ptr.base = (u32)idt;
    idt_ptr.limit = sizeof(idt) - 1;
    BMB;
    asm volatile("lidt idt_ptr\n");
}
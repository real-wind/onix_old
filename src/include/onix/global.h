#ifndef ONIX_GLOBAL_H
#define ONIX_GLOBAL_H

#include <onix/types.h>

#define GDT_SIZE 128

#define KERNEL_CODE_IDX 1
#define KERNEL_DATA_IDX 2
#define KERNEL_TSS_IDX 3

#define USER_CODE_IDX 4
#define USER_DATA_IDX 5

#define KERNEL_CODE_SELECTOR (KERNEL_CODE_IDX << 3)
#define KERNEL_DATA_SELECTOR (KERNEL_DATA_IDX << 3)
#define KERNEL_TSS_SELECTOR (KERNEL_TSS_IDX << 3)

#define USER_CODE_SELECTOR (USER_CODE_IDX << 3 | 0b11)
#define USER_DATA_SELECTOR (USER_DATA_IDX << 3 | 0b11)

// ȫ��������
typedef struct descriptor_t /* �� 8 ���ֽ� */
{
    unsigned short limit_low;      // �ν��� 0 ~ 15 λ
    unsigned int base_low : 24;    // ����ַ 0 ~ 23 λ 16M
    unsigned char type : 4;        // ������
    unsigned char segment : 1;     // 1 ��ʾ����λ����ݶΣ�0 ��ʾϵͳ��
    unsigned char DPL : 2;         // Descriptor Privilege Level ��������Ȩ�ȼ� 0 ~ 3
    unsigned char present : 1;     // ����λ��1 ���ڴ��У�0 �ڴ�����
    unsigned char limit_high : 4;  // �ν��� 16 ~ 19;
    unsigned char available : 1;   // �ð��ŵĶ������ˣ��͸�����ϵͳ��
    unsigned char long_mode : 1;   // 64 λ��չ��־
    unsigned char big : 1;         // 32 λ ���� 16 λ;
    unsigned char granularity : 1; // ���� 4KB �� 1B
    unsigned char base_high;       // ����ַ 24 ~ 31 λ
} _packed descriptor_t;

// ��ѡ����
typedef struct selector_t
{
    u8 RPL : 2; // Request Privilege Level
    u8 TI : 1;  // Table Indicator
    u16 index : 13;
} selector_t;

// ȫ����������ָ��
typedef struct pointer_t
{
    u16 limit;
    u32 base;
} _packed pointer_t;

typedef struct tss_t
{
    u32 backlink; // ǰһ����������ӣ�������ǰһ����״̬�εĶ�ѡ����
    u32 esp0;     // ring0 ��ջ����ַ
    u32 ss0;      // ring0 ��ջ��ѡ����
    u32 esp1;     // ring1 ��ջ����ַ
    u32 ss1;      // ring1 ��ջ��ѡ����
    u32 esp2;     // ring2 ��ջ����ַ
    u32 ss2;      // ring2 ��ջ��ѡ����
    u32 cr3;
    u32 eip;
    u32 flags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldtr;          // �ֲ�������ѡ����
    u16 trace : 1;     // �����λ�������л�ʱ������һ�������쳣
    u16 reversed : 15; // ��������
    u16 iobase;        // I/O λͼ����ַ��16 λ�� TSS �� IO Ȩ��λͼ��ƫ��
    u32 ssp;           // ����Ӱ��ջָ��
} _packed tss_t;

void gdt_init();

#endif
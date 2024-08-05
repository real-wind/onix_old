#ifndef ONIX_GLOBAL_H
#define ONIX_GLOBAL_H

#include <onix/types.h>

#define GDT_SIZE 128

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
    u8 RPL : 2;
    u8 TI : 1;
    u16 index : 13;
} selector_t;

// ȫ����������ָ��
typedef struct pointer_t
{
    u16 limit;
    u32 base;
} _packed pointer_t;

void gdt_init();

#endif
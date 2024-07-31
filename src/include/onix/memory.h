#ifndef ONIX_MEMORY_H
#define ONIX_MEMORY_H

#include <onix/types.h>

#define PAGE_SIZE 0x1000     // һҳ�Ĵ�С 4K
#define MEMORY_BASE 0x100000 // 1M�������ڴ濪ʼ��λ��

typedef struct page_entry_t
{
    u8 present : 1;  // ���ڴ���
    u8 write : 1;    // 0 ֻ�� 1 �ɶ���д
    u8 user : 1;     // 1 ������ 0 �����û� DPL < 3
    u8 pwt : 1;      // page write through 1 ֱдģʽ��0 ��дģʽ
    u8 pcd : 1;      // page cache disable ��ֹ��ҳ����
    u8 accessed : 1; // �����ʹ�������ͳ��ʹ��Ƶ��
    u8 dirty : 1;    // ��ҳ����ʾ��ҳ���屻д��
    u8 pat : 1;      // page attribute table ҳ��С 4K/4M
    u8 global : 1;   // ȫ�֣����н��̶��õ��ˣ���ҳ��ˢ�»���
    u8 ignored : 3;  // �ð��ŵĶ������ˣ��͸�����ϵͳ��
    u32 index : 20;  // ҳ����
} _packed page_entry_t;

u32 get_cr3();
void set_cr3(u32 pde);

#endif
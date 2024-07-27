#include <onix/global.h>
#include <onix/string.h>
#include <onix/debug.h>

descriptor_t gdt[GDT_SIZE]; // �ں�ȫ����������
pointer_t gdt_ptr;          // �ں�ȫ����������ָ��
tss_t tss;                  // ����״̬��

void descriptor_init(descriptor_t *desc, u32 base, u32 limit)
{
    desc->base_low = base & 0xffffff;
    desc->base_high = (base >> 24) & 0xff;
    desc->limit_low = limit & 0xffff;
    desc->limit_high = (limit >> 16) & 0xf;
}

// ��ʼ���ں�ȫ����������
void gdt_init()
{
    DEBUGK("init gdt!!!\n");

    memset(gdt, 0, sizeof(gdt));

    descriptor_t *desc;
    desc = gdt + KERNEL_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // �����
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 λ
    desc->long_mode = 0;   // ���� 64 λ
    desc->present = 1;     // ���ڴ���
    desc->DPL = 0;         // �ں���Ȩ��
    desc->type = 0b1010;   // ���� / ������ / �ɶ� / û�б����ʹ�

    desc = gdt + KERNEL_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // ���ݶ�
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 λ
    desc->long_mode = 0;   // ���� 64 λ
    desc->present = 1;     // ���ڴ���
    desc->DPL = 0;         // �ں���Ȩ��
    desc->type = 0b0010;   // ���� / �������� / ��д / û�б����ʹ�

    desc = gdt + USER_CODE_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // �����
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 λ
    desc->long_mode = 0;   // ���� 64 λ
    desc->present = 1;     // ���ڴ���
    desc->DPL = 3;         // �û���Ȩ��
    desc->type = 0b1010;   // ���� / ������ / �ɶ� / û�б����ʹ�

    desc = gdt + USER_DATA_IDX;
    descriptor_init(desc, 0, 0xFFFFF);
    desc->segment = 1;     // ���ݶ�
    desc->granularity = 1; // 4K
    desc->big = 1;         // 32 λ
    desc->long_mode = 0;   // ���� 64 λ
    desc->present = 1;     // ���ڴ���
    desc->DPL = 3;         // �û���Ȩ��
    desc->type = 0b0010;   // ���� / �������� / ��д / û�б����ʹ�

    gdt_ptr.base = (u32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;
}

void tss_init()
{
    memset(&tss, 0, sizeof(tss));

    tss.ss0 = KERNEL_DATA_SELECTOR;
    tss.iobase = sizeof(tss);

    descriptor_t *desc = gdt + KERNEL_TSS_IDX;
    descriptor_init(desc, (u32)&tss, sizeof(tss) - 1);
    desc->segment = 0;     // ϵͳ��
    desc->granularity = 0; // �ֽ�
    desc->big = 0;         // �̶�Ϊ 0
    desc->long_mode = 0;   // �̶�Ϊ 0
    desc->present = 1;     // ���ڴ���
    desc->DPL = 0;         // ���������Ż������
    desc->type = 0b1001;   // 32 λ���� tss

    // BMB;
    asm volatile(
        "ltr %%ax\n" ::"a"(KERNEL_TSS_SELECTOR));
}

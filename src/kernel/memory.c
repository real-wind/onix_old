#include <onix/memory.h>
#include <onix/types.h>
#include <onix/debug.h>
#include <onix/assert.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define ZONE_VALID 1    // ards �����ڴ�����
#define ZONE_RESERVED 2 // ards ����������

#define IDX(addr) ((u32)addr >> 12) // ��ȡ addr ��ҳ����

typedef struct ards_t
{
    u64 base; // �ڴ����ַ
    u64 size; // �ڴ泤��
    u32 type; // ����
} _packed ards_t;

u32 memory_base = 0; // �����ڴ����ַ��Ӧ�õ��� 1M
u32 memory_size = 0; // �����ڴ��С
u32 total_pages = 0; // �����ڴ�ҳ��
u32 free_pages = 0;  // �����ڴ�ҳ��

#define used_pages (total_pages - free_pages) // ����ҳ��

void memory_init(u32 magic, u32 addr)
{
    u32 count;
    ards_t *ptr;

    // ����� onix loader ������ں�
    if (magic == ONIX_MAGIC)
    {
        count = *(u32 *)addr;
        ptr = (ards_t *)(addr + 4);
        for (size_t i = 0; i < count; i++, ptr++)
        {
            LOGK("Memory base 0x%p size 0x%p type %d\n",
                 (u32)ptr->base, (u32)ptr->size, (u32)ptr->type);
            if (ptr->type == ZONE_VALID && ptr->size > memory_size)
            {
                memory_base = (u32)ptr->base;
                memory_size = (u32)ptr->size;
            }
        }
    }
    else
    {
        panic("Memory init magic unknown 0x%p\n", magic);
    }

    LOGK("ARDS count %d\n", count);
    LOGK("Memory base 0x%p\n", (u32)memory_base);
    LOGK("Memory size 0x%p\n", (u32)memory_size);

    assert(memory_base == MEMORY_BASE); // �ڴ濪ʼ��λ��Ϊ 1M
    assert((memory_size & 0xfff) == 0); // Ҫ��ҳ����

    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("Total pages %d\n", total_pages);
    LOGK("Free pages %d\n", free_pages);
}
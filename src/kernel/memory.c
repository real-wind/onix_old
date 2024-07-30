#include <onix/memory.h>
#include <onix/types.h>
#include <onix/debug.h>
#include <onix/assert.h>
#include <onix/stdlib.h>
#include <onix/string.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define ZONE_VALID 1    // ards �����ڴ�����
#define ZONE_RESERVED 2 // ards ����������

#define IDX(addr) ((u32)addr >> 12) // ��ȡ addr ��ҳ����
#define PAGE(idx) ((u32)idx << 12)  // ��ȡҳ���� idx ��Ӧ��ҳ��ʼ��λ��
#define ASSERT_PAGE(addr) assert((addr & 0xfff) == 0)

typedef struct ards_t
{
    u64 base; // �ڴ����ַ
    u64 size; // �ڴ泤��
    u32 type; // ����
} _packed ards_t;

static u32 memory_base = 0; // �����ڴ����ַ��Ӧ�õ��� 1M
static u32 memory_size = 0; // �����ڴ��С
static u32 total_pages = 0; // �����ڴ�ҳ��
static u32 free_pages = 0;  // �����ڴ�ҳ��

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

static u32 start_page = 0;   // �ɷ��������ڴ���ʼλ��
static u8 *memory_map;       // �����ڴ�����
static u32 memory_map_pages; // �����ڴ�����ռ�õ�ҳ��

void memory_map_init()
{
    // ��ʼ�������ڴ�����
    memory_map = (u8 *)memory_base;

    // ���������ڴ�����ռ�õ�ҳ��
    memory_map_pages = div_round_up(total_pages, PAGE_SIZE);
    LOGK("Memory map page count %d\n", memory_map_pages);

    free_pages -= memory_map_pages;

    // ��������ڴ�����
    memset((void *)memory_map, 0, memory_map_pages * PAGE_SIZE);

    // ǰ 1M ���ڴ�λ�� �Լ� �����ڴ�������ռ�õ�ҳ���ѱ�ռ��
    start_page = IDX(MEMORY_BASE) + memory_map_pages;
    for (size_t i = 0; i < start_page; i++)
    {
        memory_map[i] = 1;
    }

    LOGK("Total pages %d free pages %d\n", total_pages, free_pages);
}

// ����һҳ�����ڴ�
static u32 get_page()
{
    for (size_t i = start_page; i < total_pages; i++)
    {
        // ��������ڴ�û��ռ��
        if (!memory_map[i])
        {
            memory_map[i] = 1;
            free_pages--;
            assert(free_pages >= 0);
            u32 page = ((u32)i) << 12;
            LOGK("GET page 0x%p\n", page);
            return page;
        }
    }
    panic("Out of Memory!!!");
}

// �ͷ�һҳ�����ڴ�
static void put_page(u32 addr)
{
    ASSERT_PAGE(addr);

    u32 idx = IDX(addr);

    // idx ���� 1M ���� С�� ��ҳ����
    assert(idx >= start_page && idx < total_pages);

    // ��ֻ֤��һ������
    assert(memory_map[idx] >= 1);

    // �������ü�һ
    memory_map[idx]--;

    // ��Ϊ 0�������ҳ��һ
    if (!memory_map[idx])
    {
        free_pages++;
    }

    assert(free_pages > 0 && free_pages < total_pages);
    LOGK("PUT page 0x%p\n", addr);
}

void memory_test()
{
    u32 pages[10];
    for (size_t i = 0; i < 10; i++)
    {
        pages[i] = get_page();
    }

    for (size_t i = 0; i < 10; i++)
    {
        put_page(pages[i]);
    }
}
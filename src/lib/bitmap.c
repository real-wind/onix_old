#include <onix/bitmap.h>
#include <onix/string.h>
#include <onix/onix.h>
#include <onix/assert.h>

// ����λͼ
void bitmap_make(bitmap_t *map, char *bits, u32 length, u32 offset)
{
    map->bits = bits;
    map->length = length;
    map->offset = offset;
}

// λͼ��ʼ����ȫ����Ϊ 0
void bitmap_init(bitmap_t *map, char *bits, u32 length, u32 start)
{
    memset(bits, 0, length);
    bitmap_make(map, bits, length, start);
}

// ����ĳһλ�Ƿ�Ϊ 1
bool bitmap_test(bitmap_t *map, idx_t index)
{
    assert(index >= map->offset);

    // �õ�λͼ������
    idx_t idx = index - map->offset;

    // λͼ�����е��ֽ�
    u32 bytes = idx / 8;

    // ���ֽ��е���һλ
    u8 bits = idx % 8;

    assert(bytes < map->length);

    // ������һλ�Ƿ���� 1
    return (map->bits[bytes] & (1 << bits));
}

// ����λͼĳλ��ֵ
void bitmap_set(bitmap_t *map, idx_t index, bool value)
{
    // value �����Ƕ�ֵ��
    assert(value == 0 || value == 1);

    assert(index >= map->offset);

    // �õ�λͼ������
    idx_t idx = index - map->offset;

    // λͼ�����е��ֽ�
    u32 bytes = idx / 8;

    // ���ֽ��е���һλ
    u8 bits = idx % 8;
    if (value)
    {
        // ��Ϊ 1
        map->bits[bytes] |= (1 << bits);
    }
    else
    {
        // ��Ϊ 0
        map->bits[bytes] &= ~(1 << bits);
    }
}

// ��λͼ�еõ������� count λ
int bitmap_scan(bitmap_t *map, u32 count)
{
    int start = EOF;                 // ���Ŀ�꿪ʼ��λ��
    u32 bits_left = map->length * 8; // ʣ���λ��
    u32 next_bit = 0;                // ��һ��λ
    u32 counter = 0;                 // ������

    // ��ͷ��ʼ��
    while (bits_left-- > 0)
    {
        if (!bitmap_test(map, map->offset + next_bit))
        {
            // �����һ��λû��ռ�ã����������һ
            counter++;
        }
        else
        {
            // �����������Ϊ 0������Ѱ��
            counter = 0;
        }

        // ��һλ��λ�ü�һ
        next_bit++;

        // �ҵ�����һ�£������ÿ�ʼ��λ�ã�����
        if (counter == count)
        {
            start = next_bit - count;
            break;
        }
    }

    // ���û�ҵ����򷵻� EOF(END OF FILE)
    if (start == EOF)
        return EOF;

    // �����ҵ���λ��ȫ����Ϊ 1
    bits_left = count;
    next_bit = start;
    while (bits_left--)
    {
        bitmap_set(map, map->offset + next_bit, true);
        next_bit++;
    }

    // Ȼ�󷵻�����
    return start + map->offset;
}

#include <onix/debug.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define LEN 2
u8 buf[LEN];
bitmap_t map;

void bitmap_tests()
{
    bitmap_init(&map, buf, LEN, 0);
    for (size_t i = 0; i < 33; i++)
    {
        idx_t idx = bitmap_scan(&map, 1);
        if (idx == EOF)
        {
            LOGK("TEST FINISH\n");
            break;
        }
        LOGK("%d\n", idx);
    }
}
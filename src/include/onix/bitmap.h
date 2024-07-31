#ifndef ONIX_BITMAP_H
#define ONIX_BITMAP_H

#include <onix/types.h>

typedef struct bitmap_t
{
    u8 *bits;   // λͼ������
    u32 length; // λͼ����������
    u32 offset; // λͼ��ʼ��ƫ��
} bitmap_t;

// ��ʼ��λͼ
void bitmap_init(bitmap_t *map, char *bits, u32 length, u32 offset);

// ����λͼ
void bitmap_make(bitmap_t *map, char *bits, u32 length, u32 offset);

// ����λͼ��ĳһλ�Ƿ�Ϊ 1
bool bitmap_test(bitmap_t *map, u32 index);

// ����λͼĳλ��ֵ
void bitmap_set(bitmap_t *map, u32 index, bool value);

// ��λͼ�еõ������� count λ
int bitmap_scan(bitmap_t *map, u32 count);

#endif
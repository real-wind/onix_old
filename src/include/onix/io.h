#ifndef ONIX_IO_H
#define ONIX_IO_H

#include <onix/types.h>

extern u8 inb(u16 port); // ����һ���ֽ�
extern u16 inw(u16 port); // ����һ����

extern void outb(u16 port, u8 value); // ���һ���ֽ�
extern void outw(u16 port, u16 value); // ���һ����

#endif

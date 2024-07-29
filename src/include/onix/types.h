#ifndef ONIX_TYPES_H
#define ONIX_TYPES_H

#include <onix/onix.h>

#define EOF -1 // END OF FILE
#define NULL ((void *)0) // ��ָ��
#define EOS '\0' // �ַ�����β

#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#endif

// ���ڶ�������Ľṹ��
#define _packed __attribute__((packed))

// ����ʡ�Ժ�����ջ֡
#define _ofp __attribute__((optimize("omit-frame-pointer")))

typedef unsigned int size_t;
            
typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef u32 time_t;
#endif
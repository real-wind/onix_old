#ifndef ONIX_TIME_H
#define ONIX_TIME_H

#include <onix/types.h>

typedef struct tm
{
    int tm_sec;   // ���� [0��59]
    int tm_min;   // ������ [0��59]
    int tm_hour;  // Сʱ�� [0��59]
    int tm_mday;  // 1 ���µ����� [0��31]
    int tm_mon;   // 1 �����·� [0��11]
    int tm_year;  // �� 1900 �꿪ʼ������
    int tm_wday;  // 1 �����е�ĳ�� [0��6] (������ =0)
    int tm_yday;  // 1 ���е�ĳ�� [0��365]
    int tm_isdst; // ����ʱ��־
} tm;

void time_read_bcd(tm *time);
void time_read(tm *time);
time_t mktime(tm *time);

#endif
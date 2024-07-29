#include <onix/types.h>
#include <onix/debug.h>
#include <onix/interrupt.h>
#include <onix/io.h>
#include <onix/time.h>
#include <onix/assert.h>
#include <onix/stdlib.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define CMOS_ADDR 0x70 // CMOS ��ַ�Ĵ���
#define CMOS_DATA 0x71 // CMOS ���ݼĴ���

#define CMOS_SECOND 0x01
#define CMOS_MINUTE 0x03
#define CMOS_HOUR 0x05

#define CMOS_A 0x0a
#define CMOS_B 0x0b
#define CMOS_C 0x0c
#define CMOS_D 0x0d
#define CMOS_NMI 0x80

// �� cmos �Ĵ�����ֵ
u8 cmos_read(u8 addr)
{
    outb(CMOS_ADDR, CMOS_NMI | addr);
    return inb(CMOS_DATA);
};

// д cmos �Ĵ�����ֵ
void cmos_write(u8 addr, u8 value)
{
    outb(CMOS_ADDR, CMOS_NMI | addr);
    outb(CMOS_DATA, value);
}

static u32 volatile counter = 0;

// ʵʱʱ���жϴ�����
void rtc_handler(int vector)
{
    // ʵʱʱ���ж�������
    assert(vector == 0x28);

    // ���жϿ����������жϴ�����ɵ��ź�
    send_eoi(vector);

    // �� CMOS �Ĵ��� C������ CMOS ���������ж�
    cmos_read(CMOS_C);

    set_alarm(1);

    LOGK("rtc handler %d...\n", counter++);
}

// ���� secs �����ʵʱʱ���ж�
void set_alarm(u32 secs)
{
    tm time;
    time_read(&time);

    u8 sec = secs % 60;
    secs /= 60;
    u8 min = secs % 60;
    secs /= 60;
    u32 hour = secs;

    time.tm_sec += sec;
    if (time.tm_sec >= 60)
    {
        time.tm_sec %= 60;
        time.tm_min += 1;
    }

    time.tm_min += min;
    if (time.tm_min >= 60)
    {
        time.tm_min %= 60;
        time.tm_hour += 1;
    }

    time.tm_hour += hour;
    if (time.tm_hour >= 24)
    {
        time.tm_hour %= 24;
    }

    cmos_write(CMOS_HOUR, bin_to_bcd(time.tm_hour));
    cmos_write(CMOS_MINUTE, bin_to_bcd(time.tm_min));
    cmos_write(CMOS_SECOND, bin_to_bcd(time.tm_sec));
}

void rtc_init()
{
    u8 prev;

    // cmos_write(CMOS_B, 0b01000010); // �������ж�
    cmos_write(CMOS_B, 0b00100010); // �������ж�
    cmos_read(CMOS_C); // �� C �Ĵ����������� CMOS �ж�

    set_alarm(2);   // 2 ��

    // �����ж�Ƶ��
    outb(CMOS_A, (inb(CMOS_A) & 0xf) | 0b1110);

    set_interrupt_handler(IRQ_RTC, rtc_handler);
    set_interrupt_mask(IRQ_RTC, true);
    set_interrupt_mask(IRQ_CASCADE, true);
}
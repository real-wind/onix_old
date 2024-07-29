#include <onix/time.h>
#include <onix/debug.h>
#include <onix/stdlib.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define CMOS_ADDR 0x70 // CMOS ��ַ�Ĵ���
#define CMOS_DATA 0x71 // CMOS ���ݼĴ���

// ������ CMOS ��Ϣ�ļĴ�������
#define CMOS_SECOND 0x00  // (0 ~ 59)
#define CMOS_MINUTE 0x02  // (0 ~ 59)
#define CMOS_HOUR 0x04    // (0 ~ 23)
#define CMOS_WEEKDAY 0x06 // (1 ~ 7) ������ = 1�������� = 7
#define CMOS_DAY 0x07     // (1 ~ 31)
#define CMOS_MONTH 0x08   // (1 ~ 12)
#define CMOS_YEAR 0x09    // (0 ~ 99)
#define CMOS_CENTURY 0x32 // ���ܲ�����
#define CMOS_NMI 0x80

#define MINUTE 60          // ÿ���ӵ�����
#define HOUR (60 * MINUTE) // ÿСʱ������
#define DAY (24 * HOUR)    // ÿ�������
#define YEAR (365 * DAY)   // ÿ����������� 365 ����

// ÿ���¿�ʼʱ���Ѿ���ȥ����
static int month[13] = {
    0, // ����ռλ��û�� 0 �£��� 1 �¿�ʼ
    0,
    (31),
    (31 + 29),
    (31 + 29 + 31),
    (31 + 29 + 31 + 30),
    (31 + 29 + 31 + 30 + 31),
    (31 + 29 + 31 + 30 + 31 + 30),
    (31 + 29 + 31 + 30 + 31 + 30 + 31),
    (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31),
    (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30),
    (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31),
    (31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)};

time_t startup_time;
int century;

// �������ɵ�ʱ����ܺ� UTC ʱ���г���
// ��ϵͳ����ʱ����أ�����Ҳ��Ҫ����������Сʱ
time_t mktime(tm *time)
{
    time_t res;
    int year; // 1970 �꿪ʼ������
    // ����� 1900 �꿪ʼ����������
    if (time->tm_year >= 70)
        year = time->tm_year - 70;
    else
        year = time->tm_year - 70 + 100;

    // ��Щ�꾭��������ʱ��
    res = YEAR * year;

    // �Ѿ���ȥ�����꣬ÿ���� 1 ��
    res += DAY * ((year + 1) / 4);

    // �Ѿ�������·ݵ�ʱ��
    res += month[time->tm_mon] * DAY;

    // ��� 2 ���Ѿ����ˣ����ҵ�ǰ�������꣬��ô��ȥһ��
    if (time->tm_mon > 2 && ((year + 2) % 4))
        res -= DAY;

    // ������Ѿ���ȥ����
    res += DAY * (time->tm_mday - 1);

    // �����ȥ��Сʱ
    res += HOUR * time->tm_hour;

    // ���Сʱ��ȥ�ķ���
    res += MINUTE * time->tm_min;

    // ������ӹ�ȥ����
    res += time->tm_sec;

    return res;
}

int get_yday(tm *time)
{
    int res = month[time->tm_mon]; // �Ѿ���ȥ���µ�����
    res += time->tm_mday;          // ����¹�ȥ������

    int year;
    if (time->tm_year >= 70)
        year = time->tm_year - 70;
    else
        year = time->tm_year - 70 + 100;

    // ����������꣬���� 2 ���Ѿ���ȥ�ˣ����ȥһ��
    // ע��1972 �������꣬�����㲻̫��ȷ�������� 100 ���ƽ��
    if ((year + 2) % 4 && time->tm_mon > 2)
    {
        res -= 1;
    }

    return res;
}


void time_read_bcd(tm *time)
{
    // CMOS �ķ����ٶȺ�����Ϊ�˼�Сʱ�����ڶ�ȡ������ѭ����������ֵ��
    // ����ʱ CMOS ����ֵ�����˱仯����ô�����¶�ȡ����ֵ��
    // �����ں˾��ܰ��� CMOS ��ʱ���������� 1 ��֮�ڡ�
    do
    {
        time->tm_sec = cmos_read(CMOS_SECOND);
        time->tm_min = cmos_read(CMOS_MINUTE);
        time->tm_hour = cmos_read(CMOS_HOUR);
        time->tm_wday = cmos_read(CMOS_WEEKDAY);
        time->tm_mday = cmos_read(CMOS_DAY);
        time->tm_mon = cmos_read(CMOS_MONTH);
        time->tm_year = cmos_read(CMOS_YEAR);
        century = cmos_read(CMOS_CENTURY);
    } while (time->tm_sec != cmos_read(CMOS_SECOND));
}

void time_read(tm *time)
{
    time_read_bcd(time);
    time->tm_sec = bcd_to_bin(time->tm_sec);
    time->tm_min = bcd_to_bin(time->tm_min);
    time->tm_hour = bcd_to_bin(time->tm_hour);
    time->tm_wday = bcd_to_bin(time->tm_wday);
    time->tm_mday = bcd_to_bin(time->tm_mday);
    time->tm_mon = bcd_to_bin(time->tm_mon);
    time->tm_year = bcd_to_bin(time->tm_year);
    time->tm_yday = get_yday(time);
    time->tm_isdst = -1;
    century = bcd_to_bin(century);
}

void time_init()
{
    tm time;
    time_read(&time);
    startup_time = mktime(&time);
    LOGK("startup time: %d%d-%02d-%02d %02d:%02d:%02d\n",
         century,
         time.tm_year,
         time.tm_mon,
         time.tm_mday,
         time.tm_hour,
         time.tm_min,
         time.tm_sec);
}
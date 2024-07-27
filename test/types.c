#include <onix/types.h>
#include <stdio.h>

typedef struct descriptor /* �� 8 ���ֽ� */
{
    unsigned short limit_low;      // �ν��� 0 ~ 15 λ
    unsigned int base_low : 24;    // ����ַ 0 ~ 23 λ 16M
    unsigned char type : 4;        // ������
    unsigned char segment : 1;     // 1 ��ʾ����λ����ݶΣ�0 ��ʾϵͳ��
    unsigned char DPL : 2;         // Descriptor Privilege Level ��������Ȩ�ȼ� 0 ~ 3
    unsigned char present : 1;     // ����λ��1 ���ڴ��У�0 �ڴ�����
    unsigned char limit_high : 4;  // �ν��� 16 ~ 19;
    unsigned char available : 1;   // �ð��ŵĶ������ˣ��͸�����ϵͳ��
    unsigned char long_mode : 1;   // 64 λ��չ��־
    unsigned char big : 1;         // 32 λ ���� 16 λ;
    unsigned char granularity : 1; // ���� 4KB �� 1B
    unsigned char base_high;       // ����ַ 24 ~ 31 λ
} __attribute__((packed)) descriptor; //�ڽṹ��ĸ�����Ա֮�䲻���������ֽ��������Ա

int main()
{
    printf("size of u8 %d\n", sizeof(u8));
    printf("size of u16 %d\n", sizeof(u16));
    printf("size of u32 %d\n", sizeof(u32));
    printf("size of u64 %d\n", sizeof(u64));
    return 0;
}
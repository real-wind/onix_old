#include <onix/string.h>
#include <onix/types.h>

char *strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

char *strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr != EOS)
    {
        ptr++;
    }
    while (true)
    {
        *ptr++ = *src;
        if (*src++ == EOS)
            return dest;
    }
}

size_t strlen(const char *str)
{
    char *ptr = (char *)str;
    while (*ptr != EOS)
    {
        ptr++;
    }
    return ptr - str;
}

int strcmp(const char *lhs, const char *rhs)
{
    while (*lhs == *rhs && *lhs != EOS && *rhs != EOS)
    {
        lhs++;
        rhs++;
    }
    return *lhs < *rhs ? -1 : *lhs > *rhs;
}

char *strchr(const char *str, int ch)
{
    char *ptr = (char *)str; // �� const char* ת��Ϊ char*���Ա���в���
    while (true)
    {
        if (*ptr == ch) // �����ǰ�ַ����� ch
        {
            return ptr; // ���ص�ǰ�ַ��ĵ�ַ
        }
        if (*ptr++ == EOS) // ���� ptr ָ�룬����Ƿ�Ϊ EOS
        {
            return NULL; // ��������ַ���ĩβû���ҵ��ַ����򷵻� NULL
        }
    }
}

char *strrchr(const char *str, int ch)
{
    char *last = NULL;
    char *ptr = (char *)str;
    while (true)
    {
        if (*ptr == ch)
        {
            last = ptr;
        }
        if (*ptr++ == EOS)
        {
            return last;
        }
    }
}

int memcmp(const void *lhs, const void *rhs, size_t count)
{
    char *lptr = (char *)lhs;
    char *rptr = (char *)rhs;
    while (*lptr == *rptr && count-- > 0)
    {
        lptr++;
        rptr++;
    }
    return (*lptr == *rptr) ? 0 : (*lptr < *rptr ? -1 : 1);

}

void *memset(void *dest, int ch, size_t count)
{
    char *ptr = dest;
    while (count--)
    {
        *ptr++ = ch;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    char *ptr = dest;
    const char *sptr = src;
    while (count--)
    {
        *ptr++ = *sptr++;
    }
    return dest;
}

void *memchr(const void *str, int ch, size_t count)
{
    const char *ptr = (const char *)str;  // ʹ�� const char* ���� const һ����
    while (count--)
    {
        if (*ptr == ch)
        {
            return (void *)ptr;  // ǿ������ת���� void*
        }
        ptr++;
    }
    return NULL;  // ���δ�ҵ������� NULL
}



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
    char *ptr = (char *)str; // 将 const char* 转换为 char*，以便进行操作
    while (true)
    {
        if (*ptr == ch) // 如果当前字符等于 ch
        {
            return ptr; // 返回当前字符的地址
        }
        if (*ptr++ == EOS) // 增加 ptr 指针，检查是否为 EOS
        {
            return NULL; // 如果到达字符串末尾没有找到字符，则返回 NULL
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
    const char *ptr = (const char *)str;  // 使用 const char* 保持 const 一致性
    while (count--)
    {
        if (*ptr == ch)
        {
            return (void *)ptr;  // 强制类型转换回 void*
        }
        ptr++;
    }
    return NULL;  // 如果未找到，返回 NULL
}



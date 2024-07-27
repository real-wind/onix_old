/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <onix/stdarg.h>
#include <onix/string.h>
#include <onix/assert.h>

#define ZEROPAD 0x01 // �����
#define SIGN 0x02    // unsigned/signed long
#define PLUS 0x04    // ��ʾ��
#define SPACE 0x08   // ���Ǽӣ����ÿո�
#define LEFT 0x10    // �����
#define SPECIAL 0x20 // 0x
#define SMALL 0x40   // ʹ��Сд��ĸ
#define DOUBLE 0x80  // ������

#define is_digit(c) ((c) >= '0' && (c) <= '9')

// ���ַ����ִ�ת��������������ָ��ǰ��
static int skip_atoi(const char **s)
{
    int i = 0;
    while (is_digit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

// ������ת��Ϊָ�����Ƶ��ַ���
// str - ����ַ���ָ��
// num - ����
// base - ���ƻ���
// size - �ַ�������
// precision - ���ֳ���(����)
// flags - ѡ��
static char *number(char *str, u32 *num, int base, int size, int precision, int flags)
{
    char pad, sign, tmp[36];
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    int index;
    char *ptr = str;

    // ��� flags ָ����Сд��ĸ������Сд��ĸ��
    if (flags & SMALL)
        digits = "0123456789abcdefghijklmnopqrstuvwxyz";

    // ��� flags ָ��Ҫ����룬�����������е������־
    if (flags & LEFT)
        flags &= ~ZEROPAD;

    // ������ƻ���С�� 2 ����� 36�����˳�����
    // Ҳ��������ֻ�ܴ�������� 2-32 ֮�����
    if (base < 2 || base > 36)
        return 0;

    // ��� flags ָ��Ҫ���㣬�����ַ����� c='0'������ c ���ڿո��ַ�
    pad = (flags & ZEROPAD) ? '0' : ' ';

    // ��� flags ָ���Ǵ�������������ֵ num С�� 0�����÷��ű��� sign=���ţ���ʹ num ȡ����ֵ
    if (flags & DOUBLE && (*(double *)(num)) < 0)
    {
        sign = '-';
        *(double *)(num) = -(*(double *)(num));
    }
    else if (flags & SIGN && !(flags & DOUBLE) && ((int)(*num)) < 0)
    {
        sign = '-';
        (*num) = -(int)(*num);
    }
    else
        // ������� flags ָ���ǼӺţ����� sign=�Ӻţ����������ʹ��ո��־�� sign=�ո񣬷����� 0
        sign = (flags & PLUS) ? '+' : ((flags & SPACE) ? ' ' : 0);

    // �������ţ�����ֵ�� 1
    if (sign)
        size--;

    // �� flags ָ��������ת���������ʮ�����ƿ���ټ��� 2 λ(����0x)
    if (flags & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        // ���ڰ˽��ƿ�ȼ� 1�����ڰ˽���ת�����ǰ��һ���㣩
        else if (base == 8)
            size--;
    }

    i = 0;

    // �����ֵ num Ϊ 0������ʱ�ַ���='0'��������ݸ����Ļ�������ֵ num ת�����ַ���ʽ
    if (flags & DOUBLE)
    {
        u32 ival = (u32)(*(double *)num);
        u32 fval = (u32)(((*(double *)num) - ival) * 1000000);

        int mantissa = 6;
        while (mantissa --)
        {
            index = (fval) % base;
            (fval) /= base;
            tmp[i++] = digits[index];
        }

        tmp[i++] = '.';

        do
        {
            index = (ival) % base;
            (ival) /= base;
            tmp[i++] = digits[index];
        } while (ival);
    }
    else if ((*num) == 0)
    {
        tmp[i++] = '0';
    }
    else
    {
        while ((*num) != 0)
        {
            index = (*num) % base;
            (*num) /= base;
            tmp[i++] = digits[index];
        }
    }

    // ����ֵ�ַ��������ھ���ֵ���򾫶�ֵ��չΪ���ָ���ֵ
    if (i > precision)
        precision = i;

    // ���ֵ size ��ȥ���ڴ����ֵ�ַ��ĸ���
    size -= precision;

    // ������������ʼ�γ�����Ҫ��ת�����������ʱ�����ַ��� str ��

    // �� flags ��û������(ZEROPAD) ������루���������־
    // ���� str ���������ʣ����ֵָ���Ŀո���
    if (!(flags & (ZEROPAD + LEFT)))
        while (size-- > 0)
            *str++ = ' ';

    // ���������λ����������
    if (sign)
        *str++ = sign;

    // �� flags ָ��������ת��
    if (flags & SPECIAL)
    {
        // ����ڰ˽���ת�����ͷһλ����һ��'0'
        if (base == 8)
            *str++ = '0';
        // ����ʮ����������'0x'
        else if (base == 16)
        {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    // �� flags ��û�������������룩��־, ����ʣ�����д�� c �ַ���'0'��ո�
    if (!(flags & LEFT))
        while (size-- > 0)
            *str++ = pad;

    // ��ʱ i ������ֵ num �����ָ���

    // �����ָ���С�ھ���ֵ���� str �з��루����ֵ-i����'0'
    while (i < precision--)
        *str++ = '0';

    // ��ת��ֵ���õ������ַ����� str �У��� i ��
    while (i-- > 0)
        *str++ = tmp[i];

    // �����ֵ�Դ�����
    // ���ʾ flags ��־����������־��־
    // ����ʣ�����з���ո�
    while (size-- > 0)
        *str++ = ' ';
    return str;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    int len;
    int i;

    // ���ڴ��ת�������е��ַ���
    char *str;
    char *s;
    int *ip;

    // number() ����ʹ�õı�־
    int flags;

    int field_width; // ����ֶο��
    int precision;   // min �������ָ�����max �ַ������ַ�����
    int qualifier;   // 'h', 'l' �� 'L' ���������ֶ�
    u32 num;
    u8 *ptr;

    // ���Ƚ��ַ�ָ��ָ�� buf
    // Ȼ��ɨ���ʽ�ַ�����
    // �Ը�����ʽת��ָʾ������Ӧ�Ĵ���
    for (str = buf; *fmt; ++fmt)
    {
        // ��ʽת��ָʾ�ַ������� '%' ��ʼ
        // ����� fmt ��ʽ�ַ�����ɨ�� '%'��Ѱ�Ҹ�ʽת���ַ����Ŀ�ʼ
        // ���Ǹ�ʽָʾ��һ���ַ��������δ��� str
        if (*fmt != '%')
        {
            *str++ = *fmt;
            continue;
        }

        // ����ȡ�ø�ʽָʾ�ַ����еı�־�򣬲�����־�������� flags ������
        flags = 0;
    repeat:
        // ������һ�� %
        ++fmt;
        switch (*fmt)
        {
        // ��������
        case '-':
            flags |= LEFT;
            goto repeat;
        // �żӺ�
        case '+':
            flags |= PLUS;
            goto repeat;
        // �ſո�
        case ' ':
            flags |= SPACE;
            goto repeat;
        // ������ת��
        case '#':
            flags |= SPECIAL;
            goto repeat;
        // Ҫ����(��'0')�������ǿո�
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }

        // ȡ��ǰ�����ֶο����ֵ������ field_width ������
        field_width = -1;

        // ��������������ֵ��ֱ��ȡ��Ϊ���ֵ
        if (is_digit(*fmt))
            field_width = skip_atoi(&fmt);

        // �������������ַ� '*'����ʾ��һ������ָ�����
        else if (*fmt == '*')
        {
            ++fmt;
            // ��˵��� va_arg ȡ���ֵ
            field_width = va_arg(args, int);

            // ����ʱ���ֵС�� 0����ø�����ʾ����б�־�� '-' ��־������룩
            if (field_width < 0)
            {
                // ��˻����ڱ�־����������ñ�־�������ֶο��ֵȡΪ�����ֵ
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        // ȡ��ʽת�����ľ����򣬲����� precision ������
        precision = -1;

        // ������ʼ�ı�־��'.' �䴦��������������������
        if (*fmt == '.')
        {
            ++fmt;
            // ���������������ֵ��ֱ��ȡ��Ϊ����ֵ
            if (is_digit(*fmt))
                precision = skip_atoi(&fmt);

            // ��������������ַ�'*'����ʾ��һ������ָ������
            else if (*fmt == '*')
            {
                // ��˵��� va_arg ȡ����ֵ
                precision = va_arg(args, int);
            }
            // ����ʱ���ֵС�� 0�����ֶξ���ֵȡΪ�����ֵ
            if (precision < 0)
                precision = 0;
        }

        // ������δ�������������η������������ qualifer ����
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
        {
            qualifier = *fmt;
            ++fmt;
        }

        // �������ת��ָʾ��
        switch (*fmt)
        {

        // ���ת��ָʾ����'c'�����ʾ��Ӧ����Ӧ���ַ�
        case 'c':
            // ��ʱ�����־�������������룬
            if (!(flags & LEFT))
                // ����ֶ�ǰ����� (�����ֵ - 1) ���ո��ַ���Ȼ���ٷ�������ַ�
                while (--field_width > 0)
                    *str++ = ' ';
            *str++ = (unsigned char)va_arg(args, int);
            // �������򻹴��� 0�����ʾΪ�����
            // ���ڲ����ַ�������� (���ֵ-1) ���ո��ַ�
            while (--field_width > 0)
                *str++ = ' ';
            break;

        // ���ת��ָʾ���� 's'�����ʾ��Ӧ�������ַ���
        case 's':
            s = va_arg(args, char *);
            // ����ȡ�����ַ����ĳ���
            len = strlen(s);
            // ���䳬���˾�����ֵ, ����չ������=�ַ�������
            if (precision < 0)
                precision = len;
            else if (len > precision)
                len = precision;

            // ��ʱ�����־��������������
            if (!(flags & LEFT))
                // ����ֶ�ǰ���� (���ֵ-�ַ�������) ���ո��ַ�
                while (len < field_width--)
                    *str++ = ' ';
            // Ȼ���ٷ�������ַ���
            for (i = 0; i < len; ++i)
                *str++ = *s++;
            // �������򻹴��� 0�����ʾΪ�����
            // ���ڲ����ַ������棬���(���ֵ-�ַ�������)���ո��ַ�
            while (len < field_width--)
                *str++ = ' ';
            break;

        // �����ʽת������'o'����ʾ�轫��Ӧ�Ĳ���ת���ɰ˽��������ַ���
        case 'o':
            num = va_arg(args, unsigned long);
            str = number(str, &num, 8, field_width, precision, flags);
            break;

        // �����ʽת������'p'����ʾ��Ӧ������һ��ָ������
        case 'p':
            // ��ʱ���ò���û�����ÿ������Ĭ�Ͽ��Ϊ 8��������Ҫ����
            if (field_width == -1)
            {
                field_width = 8;
                flags |= ZEROPAD;
            }
            num = va_arg(args, unsigned long);
            str = number(str, &num, 16, field_width, precision, flags);
            break;

        // ����ʽת��ָʾ�� 'x' �� 'X'
        // ���ʾ��Ӧ������Ҫ��ӡ��ʮ�����������
        case 'x':
            // 'x'��ʾ��Сд��ĸ��ʾ
            flags |= SMALL;
        case 'X':
            num = va_arg(args, unsigned long);
            str = number(str, &num, 16, field_width, precision, flags);
            break;

        // �����ʽת���ַ���'d', 'i' �� 'u'�����ʾ��Ӧ����������
        case 'd':
        case 'i':
            // 'd', 'i'������������������Ҫ���ϴ����ű�־
            flags |= SIGN;
        // 'u'�����޷�������
        case 'u':
            num = va_arg(args, unsigned long);
            str = number(str, &num, 10, field_width, precision, flags);
            break;

        // ����ʽת��ָʾ���� 'n'
        // ��ʾҪ�ѵ�ĿǰΪֹת��������ַ������浽��Ӧ����ָ��ָ����λ����
        case 'n':
            // �������� va_arg() ȡ�øò���ָ��
            ip = va_arg(args, int *);
            // Ȼ���Ѿ�ת���õ��ַ��������ָ����ָ��λ��
            *ip = (str - buf);
            break;
        case 'f':
            flags |= SIGN;
            flags |= DOUBLE;
            double dnum = va_arg(args, double);
            str = number(str, (u32 *)&dnum, 10, field_width, precision, flags);
            break;
        case 'b': // binary
            num = va_arg(args, unsigned long);
            str = number(str, &num, 2, field_width, precision, flags);
            break;
        case 'm': // mac address
            flags |= SMALL | ZEROPAD;
            ptr = va_arg(args, char *);
            for (size_t t = 0; t < 6; t++, ptr++)
            {
                int num = *ptr;
                str = number(str, &num, 16, 2, precision, flags);
                *str = ':';
                str++;
            }
            str--;
            break;
        case 'r': // ip address
            flags |= SMALL;
            ptr = va_arg(args, u8 *);
            for (size_t t = 0; t < 4; t++, ptr++)
            {
                int num = *ptr;
                str = number(str, &num, 10, field_width, precision, flags);
                *str = '.';
                str++;
            }
            str--;
            break;
        default:
            // ����ʽת�������� '%'�����ʾ��ʽ�ַ����д�
            if (*fmt != '%')
                // ֱ�ӽ�һ�� '%' д���������
                *str++ = '%';
            // �����ʽת������λ�ô������ַ�����Ҳֱ�ӽ����ַ�д���������
            // Ȼ�����ѭ�������ʽ�ַ���
            if (*fmt)
                *str++ = *fmt;
            else
                // �����ʾ�Ѿ�������ʽ�ַ����Ľ�β�������˳�ѭ��
                --fmt;
            break;
        } 
    }
    // �����ת���õ��ַ�����β�������ַ���������־
    *str = '\0';

    // ����ת���õ��ַ�������ֵ
    i = str - buf;
    assert(i < 1024);
    return i;
}

// �������ʽ����ַ����� buf
int sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int i = vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}

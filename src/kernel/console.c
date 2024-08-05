#include <onix/console.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/interrupt.h>

#define CRT_ADDR_REG 0x3D4 // CRT(6845)�����Ĵ���
#define CRT_DATA_REG 0x3D5 // CRT(6845)���ݼĴ���
#define CRT_START_ADDR_H 0xC // ��ʾ�ڴ���ʼλ�� - ��λ
#define CRT_START_ADDR_L 0xD // ��ʾ�ڴ���ʼλ�� - ��λ
#define CRT_CURSOR_H 0xE     // ���λ�� - ��λ
#define CRT_CURSOR_L 0xF     // ���λ�� - ��λ
#define MEM_BASE 0xB8000              // �Կ��ڴ���ʼλ��
#define MEM_SIZE 0x4000               // �Կ��ڴ��С
#define MEM_END (MEM_BASE + MEM_SIZE) // �Կ��ڴ����λ��
#define WIDTH 80                      // ��Ļ�ı�����
#define HEIGHT 25                     // ��Ļ�ı�����
#define ROW_SIZE (WIDTH * 2)          // ÿ���ֽ���
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // ��Ļ�ֽ���
#define ASCII_NUL 0x00
#define ASCII_ENQ 0x05
#define ASCII_BEL 0x07 // \a
#define ASCII_BS 0x08  // \b
#define ASCII_HT 0x09  // \t
#define ASCII_LF 0x0A  // \n
#define ASCII_VT 0x0B  // \v
#define ASCII_FF 0x0C  // \f
#define ASCII_CR 0x0D  // \r
#define ASCII_DEL 0x7F
static u32 screen; // ��ǰ��ʾ����ʼ���ڴ�λ��
static u32 pos; // ��¼��ǰ�����ڴ�λ��
static u32 x, y; // ��ǰ��������
static u8 attr = 7;        // �ַ���ʽ
static u16 erase = 0x0720; // �ո�
// ��õ�ǰ��ʾ���Ŀ�ʼλ��
static void get_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H); // ��ʼλ�øߵ�ַ
    screen = inb(CRT_DATA_REG) << 8;      // ��ʼλ�ø߰�λ
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    screen |= inb(CRT_DATA_REG);
    screen <<= 1; // screen *= 2
    screen += MEM_BASE;
}
// ���õ�ǰ��ʾ����ʼ��λ��
static void set_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H); // ��ʼλ�øߵ�ַ
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 1) & 0xff);
}
// ��õ�ǰ���λ��
static void get_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // �ߵ�ַ
    pos = inb(CRT_DATA_REG) << 8;     // �߰�λ
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    pos |= inb(CRT_DATA_REG);
    get_screen();
    pos <<= 1; // pos *= 2
    pos += MEM_BASE;
    u32 delta = (pos - screen) >> 1;
    x = delta % WIDTH;
    y = delta / WIDTH;
}
static void set_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // ���ߵ�ַ
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 9) & 0xff);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 1) & 0xff);
}
void console_clear()
{
    screen = MEM_BASE;
    pos = MEM_BASE;
    x = y = 0;
    set_cursor();
    set_screen();
    u16 *ptr = (u16 *)MEM_BASE;
    while (ptr < (u16 *)MEM_END)
    {
        *ptr++ = erase;
    }
}
// ���Ϲ���
static void scroll_up()
{
    if (screen + SCR_SIZE + ROW_SIZE >= MEM_END)
    {
        memcpy((void *)MEM_BASE, (void *)screen, SCR_SIZE);
        pos -= (screen - MEM_BASE);
        screen = MEM_BASE;
    }

    u32 *ptr = (u32 *)(screen + SCR_SIZE);
    for (size_t i = 0; i < WIDTH; i++)
    {
        *ptr++ = erase;
    }
    screen += ROW_SIZE;
    pos += ROW_SIZE;
    set_screen();
}

  
  
static void command_lf()
{
    if (y + 1 < HEIGHT)
    {
        y++;
        pos += ROW_SIZE;
        return;
    }
    scroll_up();
}
static void command_cr()
{
    pos -= (x << 1);
    x = 0;
}
static void command_bs()
{
    if (x)
    {
        x--;
        pos -= 2;
        *(u16 *)pos = erase;
    }
}
static void command_del()
{
    *(u16 *)pos = erase;
}

extern void start_beep();

void console_write(char *buf, u32 count)
{
    bool intr = interrupt_disable(); // ��ֹ�ж�
    char ch;
    while (count--)
    {
        ch = *buf++;
        switch (ch)
        {
        case ASCII_NUL:
            break;
        case ASCII_BEL:
            start_beep();
            break;
        case ASCII_BS:
            command_bs();
            break;
        case ASCII_HT:
            break;
        case ASCII_LF:
            command_lf();
            command_cr();
            break;
        case ASCII_VT:
            break;
        case ASCII_FF:
            command_lf();
            break;
        case ASCII_CR:
            command_cr();
            break;
        case ASCII_DEL:
            command_del();
            break;
        default:
            if (x >= WIDTH)
            {
                x -= WIDTH;
                pos -= ROW_SIZE;
                command_lf();
            }
            *((char *)pos) = ch;
            pos++;
            *((char *)pos) = attr;
            pos++;
            x++;
            break;
        }
    }
    set_cursor();

    // �ָ��ж�
    set_interrupt_state(intr);
}
void console_init()
{
    console_clear();
}
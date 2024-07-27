#include <onix/console.h>
#include <onix/io.h>
#include <onix/string.h>

#define CRT_ADDR_REG 0x3D4  // CRT(6845)��ַ�Ĵ���
#define CRT_DATA_REG 0x3D5  // CRT(6845)���ݼĴ���

#define CRT_START_ADDR_H 0xC  // ��ʾ�ڴ���ʼ��ַ - ��λ
#define CRT_START_ADDR_L 0xD  // ��ʾ�ڴ���ʼ��ַ - ��λ
#define CRT_CURSOR_H 0xE      // ���λ�� - ��λ
#define CRT_CURSOR_L 0xF      // ���λ�� - ��λ

#define MEM_BASE 0xB8000  // ��ʾ�ڴ����ַ
#define MEM_SIZE 0x4000   // ��ʾ�ڴ��С
#define MEM_END (MEM_BASE + MEM_SIZE)  // ��ʾ�ڴ������ַ

#define WIDTH 80  // ��Ļ���
#define HEIGHT 25 // ��Ļ�߶�
#define ROW_SIZE (WIDTH * 2)  // ÿ���ֽڴ�С
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // ��Ļ�ֽڴ�С

#define ASCII_NUL 0x00  // Null
#define ASCII_ENQ 0x05  // Enquiry
#define ASCII_BEL 0x07  // \a
#define ASCII_BS  0x08  // \b
#define ASCII_HT  0x09  // \t   HT - Horizontal Tab
#define ASCII_LF  0x0A  // \n   LF - Line Feed
#define ASCII_VT  0x0B  // \v   VT - Vertical Tab
#define ASCII_FF  0x0C  // \f   FF - Form Feed
#define ASCII_CR  0x0D  // \r   CR - Carriage Return
#define ASCII_DEL 0x7F  //



static u32 screen;  // ��ǰ��ʾ����ʼ���ڴ�λ��
static u32 pos;     // ��¼��ǰ�����ڴ�λ��

static u32 x, y;    // ��ǰ��������

static u8 attr = 7;             // �ַ�����
static u16 erase = 0x0720;      // ǰ��һ���ֽ��ǿո�20���ո��ascii�룩������һ���ֽ�����ʽ07��cpuС�˴洢���洢ʱ2007

// ��ȡ��ǰ��ʾ���Ŀ�ʼλ��
static void get_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);  // ��ʼ��ַ���ֽ�
    screen = inb(CRT_DATA_REG) << 8;       // ��ʼ��ַ��λ
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);  // ��ʼ��ַ���ֽ�
    screen |= inb(CRT_DATA_REG);           // ��ʼ��ַ��λ

    screen <<= 1;  // screen *= 2 ��Ļ��ʾ���ַ�Ϊ��λ���ڴ���Ҫ�ַ�����ʽ��ע��screen���ڴ�λ�ö�������ʾ�����ַ�λ�ã���Ϊ��ʾ��һ���ַ�����Ӧ�ڴ������ֽ�
    screen += MEM_BASE;  // ����Ϊ��ȷ�������ڴ��ַ
}

// ���õ�ǰ��ʾ����ʼλ��
static void set_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);  // ��ʼ��ַ���ֽ�
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);  // ����һλ�Ӱ�λ
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);  
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 1) & 0xff);     
}

// ��ȡ���λ��
static void get_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // ��ȡ����λ
    pos = inb(CRT_DATA_REG) << 8;     // ����λ
    outb(CRT_ADDR_REG, CRT_CURSOR_L); // ��ȡ����λ
    pos |= inb(CRT_DATA_REG);         // ����λ

    get_screen();

    pos <<= 1;  // �� pos ��λ�����ƶ�1λ���൱�ڽ� pos ��ֵ����2����Ȼ�󽫽����ֵ�� pos��pos << 1 
    pos += MEM_BASE;

    u32 delta = (pos - screen) >> 1;
    x = delta % WIDTH;
    y = delta / WIDTH;
}

// ���ù��λ��Ϊ pos
static void set_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // ���ù���λ
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 9) & 0xff); // д�����λ

    outb(CRT_ADDR_REG, CRT_CURSOR_L); // ���ù���λ
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 1) & 0xff); // д�����λ
}

// ����̨���
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


// �������ܣ����Ϲ�����Ļ��ʾ����
static void scroll_up()
{
    // ����ڹ�������Ļ�����Ƿ�ᳬ����ʾ�ڴ��ĩβ
    if (screen + SCR_SIZE + ROW_SIZE < MEM_END)
    {
        // ��ʼ��ָ��ptrָ����Ļ�����������һ�еĿ�ʼλ��
        u32 *ptr = (u32 *)(screen + SCR_SIZE);
        // ѭ���������У�����ַ�λ������Ϊeraseֵ���������Ļ���һ��
        for (size_t i = 0; i < WIDTH; i++)
        {
            *ptr++ = erase;  // �����Ļ���һ�е�ÿ���ַ�λ��
        }

        // ����screen��pos��ʹ�����Ƕ������ƶ�һ�еľ���
        screen += ROW_SIZE;  // ����ʾ�������ʼλ�������ƶ�һ��
        pos += ROW_SIZE;     // �������λ����Ӧ�����ƶ�һ��
    }
    else
    {
        // ������������ʾ�ڴ�ĩβʱ����������Ļ���ݸ��Ƶ���ʾ�ڴ����ʼλ��
        memcpy((void *)MEM_BASE, screen, SCR_SIZE);
        // ����posλ�ã���֤���λ�ò���
        pos -= (screen - MEM_BASE);
        // ����screenָ�뵽��ʾ�ڴ����ʼλ��
        screen = MEM_BASE;
    }
    // ������Ļ���ã����ܰ������¼�����ʾ��ز������ػ���Ļ
    set_screen();
}


// ����
static void command_lf()
{
    if (y + 1 < HEIGHT)  // ����Ƿ�����һ��
    {
        y++;             // �к�����
        pos += ROW_SIZE; // ����pos����һ�еĿ�ʼλ��
        return;
    }
    // ���y�Ѿ������һ�У�������Ҫ����scroll_up()
}

// ����س����ʹ���ص���ǰ�еĿ�ͷ
static void command_cr()
{
    pos -= (x << 1); // ����ÿ���ַ�λ��ռ��2���ֽڣ����ｫx����2Ȼ���ٳ���2��������2λ��
    x = 0;           // ��x��������Ϊ0�����еĿ�ʼ
}

// �����˸����Backspace��������������ƶ�һ��λ�ò�������λ�õ��ַ�
static void command_bs()
{
    if (x) // ȷ��x�����еĿ�ʼλ�ã�������ǰԽ��
    {
        x--;        // ��������ƶ�һ��λ��
        pos -= 2;   // �����ڴ�λ�ã�ÿ���ַ�λ��ռ��2�ֽ�
        *(u16 *)pos = erase; // ����ǰλ�õ��ַ�����Ϊ����ֵ��ͨ���ǿո��Ĭ������
    }
}

// ����ɾ�����������ı�����̨��ɾ����ǰ���λ�õ��ַ�
static void command_del()
{
    *(u16 *)pos = erase;  // ����ǰ���λ�õ��ַ�����Ϊ����ֵ
}



// ����̨д���ı�
void console_write(char *buf, u32 count)
{
    char ch;
    // char *ptr = (char *)pos;
    while (count--)
    {
        ch = *buf++; // ch = *buf; buf++;
        switch (ch)
        {
            case ASCII_NUL:
                // ����ַ��� ASCII ���ַ��������κδ���
                break;

            case ASCII_BEL:
            // todo \a
                break; // ASCII 7������������ڿ����豸����������ʲôҲ����
            case ASCII_BS:
                command_bs();
                break; // ASCII 8���˸����ͨ������ɾ��ǰһ���ַ�������ʲôҲ����
            case ASCII_HT:
                break; // ASCII 9��ˮƽ�Ʊ�������ڿ���ˮƽ��������ʲôҲ����
            case ASCII_LF:
                command_lf();
                command_cr();
                break; // ASCII 10�����з������ڿ��ƹ���ƶ�����һ�У�����ʲôҲ����
            case ASCII_VT:
                break; // ASCII 11����ֱ�Ʊ�������ڿ��ƴ�ֱ��������ʲôҲ����
            case ASCII_FF:
                command_lf();
                break; // ASCII 12����ҳ�������ڿ��ƴ�ӡ����ҳ������������ʲôҲ����
            case ASCII_CR:
                command_cr();
                break; // ASCII 13���س�����ͨ��ʹ��귵�����ף�����ʲôҲ����
            case ASCII_DEL:
                command_del();
                break; // ASCII 127��ɾ����������ɾ����ǰλ�õ��ַ�������ʲôҲ����

            
            default:
                if (x >= WIDTH)
                {
                    x -= WIDTH;
                    pos -= ROW_SIZE;  // ���¹��λ�õ���һ��
                    command_lf();
                }

                *((char *)pos) = ch;  // д���ַ�
                pos++;
                *((char *)pos) = attr;  // д���ַ�����
                pos++;
                
                // pos += 2;  // �����ڴ�λ��
                x++;  // ���º�����
                break;
        }
    }
    set_cursor();  // ���¹��λ��
}


void console_write(char *buf, u32 count);

void console_init()
{
    console_clear();

}

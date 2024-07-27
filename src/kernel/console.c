#include <onix/console.h>
#include <onix/io.h>
#include <onix/string.h>

#define CRT_ADDR_REG 0x3D4  // CRT(6845)地址寄存器
#define CRT_DATA_REG 0x3D5  // CRT(6845)数据寄存器

#define CRT_START_ADDR_H 0xC  // 显示内存起始地址 - 高位
#define CRT_START_ADDR_L 0xD  // 显示内存起始地址 - 低位
#define CRT_CURSOR_H 0xE      // 光标位置 - 高位
#define CRT_CURSOR_L 0xF      // 光标位置 - 低位

#define MEM_BASE 0xB8000  // 显示内存基地址
#define MEM_SIZE 0x4000   // 显示内存大小
#define MEM_END (MEM_BASE + MEM_SIZE)  // 显示内存结束地址

#define WIDTH 80  // 屏幕宽度
#define HEIGHT 25 // 屏幕高度
#define ROW_SIZE (WIDTH * 2)  // 每行字节大小
#define SCR_SIZE (ROW_SIZE * HEIGHT)  // 屏幕字节大小

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



static u32 screen;  // 当前显示器开始的内存位置
static u32 pos;     // 记录当前光标的内存位置

static u32 x, y;    // 当前光标的坐标

static u8 attr = 7;             // 字符属性
static u16 erase = 0x0720;      // 前面一个字节是空格20（空格的ascii码），后面一个字节是样式07，cpu小端存储，存储时2007

// 获取当前显示器的开始位置
static void get_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);  // 开始地址高字节
    screen = inb(CRT_DATA_REG) << 8;       // 开始地址高位
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);  // 开始地址低字节
    screen |= inb(CRT_DATA_REG);           // 开始地址低位

    screen <<= 1;  // screen *= 2 屏幕显示是字符为单位，内存需要字符和样式，注意screen是内存位置而不是显示器上字符位置，因为显示器一个字符，对应内存两个字节
    screen += MEM_BASE;  // 调整为正确的物理内存地址
}

// 设置当前显示器开始位置
static void set_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);  // 开始地址高字节
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 9) & 0xff);  // 右移一位加八位
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);  
    outb(CRT_DATA_REG, ((screen - MEM_BASE) >> 1) & 0xff);     
}

// 获取光标位置
static void get_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // 读取光标高位
    pos = inb(CRT_DATA_REG) << 8;     // 光标高位
    outb(CRT_ADDR_REG, CRT_CURSOR_L); // 读取光标低位
    pos |= inb(CRT_DATA_REG);         // 光标低位

    get_screen();

    pos <<= 1;  // 将 pos 的位向左移动1位（相当于将 pos 的值乘以2），然后将结果赋值回 pos，pos << 1 
    pos += MEM_BASE;

    u32 delta = (pos - screen) >> 1;
    x = delta % WIDTH;
    y = delta / WIDTH;
}

// 设置光标位置为 pos
static void set_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H); // 设置光标高位
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 9) & 0xff); // 写入光标高位

    outb(CRT_ADDR_REG, CRT_CURSOR_L); // 设置光标低位
    outb(CRT_DATA_REG, ((pos - MEM_BASE) >> 1) & 0xff); // 写入光标低位
}

// 控制台清除
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


// 函数功能：向上滚动屏幕显示内容
static void scroll_up()
{
    // 检查在滚动后屏幕内容是否会超出显示内存的末尾
    if (screen + SCR_SIZE + ROW_SIZE < MEM_END)
    {
        // 初始化指针ptr指向屏幕缓冲区的最后一行的开始位置
        u32 *ptr = (u32 *)(screen + SCR_SIZE);
        // 循环遍历整行，逐个字符位置设置为erase值，即清空屏幕最后一行
        for (size_t i = 0; i < WIDTH; i++)
        {
            *ptr++ = erase;  // 清空屏幕最后一行的每个字符位置
        }

        // 更新screen和pos，使得它们都向上移动一行的距离
        screen += ROW_SIZE;  // 将显示区域的起始位置向上移动一行
        pos += ROW_SIZE;     // 调整光标位置相应向上移动一行
    }
    else
    {
        // 当即将超出显示内存末尾时，将整个屏幕内容复制到显示内存的起始位置
        memcpy((void *)MEM_BASE, screen, SCR_SIZE);
        // 调整pos位置，保证相对位置不变
        pos -= (screen - MEM_BASE);
        // 重置screen指针到显示内存的起始位置
        screen = MEM_BASE;
    }
    // 更新屏幕设置，可能包括重新计算显示相关参数或重绘屏幕
    set_screen();
}


// 换行
static void command_lf()
{
    if (y + 1 < HEIGHT)  // 检查是否还有下一行
    {
        y++;             // 行号增加
        pos += ROW_SIZE; // 更新pos到下一行的开始位置
        return;
    }
    // 如果y已经是最后一行，可能需要调用scroll_up()
}

// 处理回车命令，使光标回到当前行的开头
static void command_cr()
{
    pos -= (x << 1); // 由于每个字符位置占用2个字节，这里将x乘以2然后再乘以2（即左移2位）
    x = 0;           // 将x坐标重置为0，即行的开始
}

// 处理退格键（Backspace），将光标向左移动一个位置并擦除该位置的字符
static void command_bs()
{
    if (x) // 确保x不是行的开始位置，避免向前越界
    {
        x--;        // 光标向左移动一个位置
        pos -= 2;   // 更新内存位置，每个字符位置占用2字节
        *(u16 *)pos = erase; // 将当前位置的字符设置为擦除值，通常是空格和默认属性
    }
}

// 处理删除操作，在文本控制台中删除当前光标位置的字符
static void command_del()
{
    *(u16 *)pos = erase;  // 将当前光标位置的字符设置为擦除值
}



// 控制台写入文本
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
                // 如果字符是 ASCII 空字符，不做任何处理
                break;

            case ASCII_BEL:
            // todo \a
                break; // ASCII 7，响铃符，用于控制设备发声，这里什么也不做
            case ASCII_BS:
                command_bs();
                break; // ASCII 8，退格符，通常用于删除前一个字符，这里什么也不做
            case ASCII_HT:
                break; // ASCII 9，水平制表符，用于控制水平跳格，这里什么也不做
            case ASCII_LF:
                command_lf();
                command_cr();
                break; // ASCII 10，换行符，用于控制光标移动到下一行，这里什么也不做
            case ASCII_VT:
                break; // ASCII 11，垂直制表符，用于控制垂直跳格，这里什么也不做
            case ASCII_FF:
                command_lf();
                break; // ASCII 12，换页符，用于控制打印机换页或清屏，这里什么也不做
            case ASCII_CR:
                command_cr();
                break; // ASCII 13，回车符，通常使光标返回行首，这里什么也不做
            case ASCII_DEL:
                command_del();
                break; // ASCII 127，删除符，用于删除当前位置的字符，这里什么也不做

            
            default:
                if (x >= WIDTH)
                {
                    x -= WIDTH;
                    pos -= ROW_SIZE;  // 更新光标位置到下一行
                    command_lf();
                }

                *((char *)pos) = ch;  // 写入字符
                pos++;
                *((char *)pos) = attr;  // 写入字符属性
                pos++;
                
                // pos += 2;  // 更新内存位置
                x++;  // 更新横坐标
                break;
        }
    }
    set_cursor();  // 更新光标位置
}


void console_write(char *buf, u32 count);

void console_init()
{
    console_clear();

}

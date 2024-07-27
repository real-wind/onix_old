#include <stdio.h>  // /usr/include/stdio.h

char message[] = "hello world!!!\n";
char buf[1024];  // .bss

int main()
{
    printf("%s", message);
    printf("%s", message);
    printf("%s", message);
}

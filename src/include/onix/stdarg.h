#ifndef ONIX_STDARG_H
#define ONIX_STDARG_H

typedef char *va_list;

#define stack_size(t) (sizeof(t) <= sizeof(char *) ? sizeof(char *) : sizeof(t))
#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *))
#define va_arg(ap, t) (*(t *)((ap += stack_size(t)) - stack_size(t)))
#define va_end(ap) (ap = (va_list)0)

#endif

//  `va_list`：保存可变参数指针
//  `va_start`：启用可变参数
//  `va_arg`：获取下一个参数
//  `va_end`：结束可变参数
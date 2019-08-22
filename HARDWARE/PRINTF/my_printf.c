#include "my_printf.h"
#include "stdio.h"
#include "stdarg.h"


/*int my_printf(char *str,...)

{
    int n;    //记录返回值
    va_list list;
    va_start(list,str);
    n=vprintf(str,list);
    va_end(list);
    return n;
}*/

int my_sprintf (char * buf, const char *format, ...)
{ 
    int lenth; 
    va_list ap; 
    va_start (ap, format);
    lenth = vsprintf (buf, format, ap); 
    va_end (ap); 
    return lenth;
}


/*
*主要利用"putchar"函数，检测首先检测是什么类型的数据，然后按照想打印的数据类型转换为字符型数
*据，然后利用"putchar"函数进行打印
*va_start(ap,v):将ap指向第一个实际参数的地址
*va_arg(ap,t):t是指实际参数的类型，将ap向后移动sizeof(t)，指向下一个实际参数的地址同时返回起地址
*va_end(ap):将指针置位NULL
*/

void printch(const char ch)      //输出字符
{  
    putchar(ch);  
}  

void printint(const int dec)     //输出整型数
{  
    if(dec == 0)  
    {  
        return;  
    }  
    printint(dec / 10);  
    putchar((char)(dec % 10 + '0'));  //转换为ASCII
}  

void printstr(const char *ptr)        //输出字符串
{  
    while(*ptr)  
    {  
        putchar(*ptr);  
        ptr++;  
    }  
}  

void printfloat(const float fl)     //输出浮点数，小数点第5位四舍五入
{  
    int fl_int = (int)fl;  //取整数部分
    int fl_flt = (int)(10000000 * (fl- fl_int));    //先取7位小数

    if(fl_flt % 10 >= 5)    //第七位四舍五入
    {  
        fl_flt = fl_flt / 10 + 1;  
    }  
    else  
    {  
        fl_flt = fl_flt / 10;  
    } 
    if(fl_int==0)        //如果正数部分为零，则打印一个0
			putchar('0');
    else		
      printint(fl_int);  //打印整数部分
    putchar('.');        //打印小数点
    if(fl_flt==0)        //如果小数部分为零则打印"000000"
      puts("000000");	   	
		else
      printint(fl_flt);  //打印小数部分
}  

void my_printf(const char *format,...)  
{  
    va_list ap;  
    va_start(ap,format);     //让ap指向第一个实际参数的地址
    while(*format)  
    {  
        if(*format != '%')  
        {  
            putchar(*format);
            format++;
        }
        else  
        {
            format++;  
            switch(*format)  
            {
                case 'c':  
                {
                    char valch = va_arg(ap,int);      //将ap指向到下一参数地址，并记录参数的地址
                    printch(valch);
                    format++;
                    break;
                }
                case 'd':
                {
                    int valint = va_arg(ap,int);      //将ap指向到下一参数地址，并记录参数的地址
                    printint(valint);
                    format++;
                    break;
                }  
                case 's':  
                {  
                    char *valstr = va_arg(ap,char *);  //将ap指向到下一参数地址，并记录参数的地址
                    printstr(valstr);  
                    format++;  
                    break;  
                }  
                case 'f':  
                {  
                    float valflt = va_arg(ap,double); //将ap指向到下一参数地址，并记录参数的地址
                    printfloat(valflt);  
                    format++;  
                    break;  
                }  
                default:  
                {  
                    printch(*format);  
                    format++;  
                }  
            }    
        }  
    }
    va_end(ap);    //即ap = NULL    
}  

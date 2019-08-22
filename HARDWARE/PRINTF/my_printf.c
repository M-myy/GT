#include "my_printf.h"
#include "stdio.h"
#include "stdarg.h"


/*int my_printf(char *str,...)

{
    int n;    //��¼����ֵ
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
*��Ҫ����"putchar"������������ȼ����ʲô���͵����ݣ�Ȼ�������ӡ����������ת��Ϊ�ַ�����
*�ݣ�Ȼ������"putchar"�������д�ӡ
*va_start(ap,v):��apָ���һ��ʵ�ʲ����ĵ�ַ
*va_arg(ap,t):t��ָʵ�ʲ��������ͣ���ap����ƶ�sizeof(t)��ָ����һ��ʵ�ʲ����ĵ�ַͬʱ�������ַ
*va_end(ap):��ָ����λNULL
*/

void printch(const char ch)      //����ַ�
{  
    putchar(ch);  
}  

void printint(const int dec)     //���������
{  
    if(dec == 0)  
    {  
        return;  
    }  
    printint(dec / 10);  
    putchar((char)(dec % 10 + '0'));  //ת��ΪASCII
}  

void printstr(const char *ptr)        //����ַ���
{  
    while(*ptr)  
    {  
        putchar(*ptr);  
        ptr++;  
    }  
}  

void printfloat(const float fl)     //�����������С�����5λ��������
{  
    int fl_int = (int)fl;  //ȡ��������
    int fl_flt = (int)(10000000 * (fl- fl_int));    //��ȡ7λС��

    if(fl_flt % 10 >= 5)    //����λ��������
    {  
        fl_flt = fl_flt / 10 + 1;  
    }  
    else  
    {  
        fl_flt = fl_flt / 10;  
    } 
    if(fl_int==0)        //�����������Ϊ�㣬���ӡһ��0
			putchar('0');
    else		
      printint(fl_int);  //��ӡ��������
    putchar('.');        //��ӡС����
    if(fl_flt==0)        //���С������Ϊ�����ӡ"000000"
      puts("000000");	   	
		else
      printint(fl_flt);  //��ӡС������
}  

void my_printf(const char *format,...)  
{  
    va_list ap;  
    va_start(ap,format);     //��apָ���һ��ʵ�ʲ����ĵ�ַ
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
                    char valch = va_arg(ap,int);      //��apָ����һ������ַ������¼�����ĵ�ַ
                    printch(valch);
                    format++;
                    break;
                }
                case 'd':
                {
                    int valint = va_arg(ap,int);      //��apָ����һ������ַ������¼�����ĵ�ַ
                    printint(valint);
                    format++;
                    break;
                }  
                case 's':  
                {  
                    char *valstr = va_arg(ap,char *);  //��apָ����һ������ַ������¼�����ĵ�ַ
                    printstr(valstr);  
                    format++;  
                    break;  
                }  
                case 'f':  
                {  
                    float valflt = va_arg(ap,double); //��apָ����һ������ַ������¼�����ĵ�ַ
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
    va_end(ap);    //��ap = NULL    
}  

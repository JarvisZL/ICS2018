#include "klib.h"
#include <stdarg.h>
#include <stdlib.h>


//#ifndef __ISA_NATIVE__
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static int cnt;
static int cntvs;
static int cnts;

static void itos(char s[],int x)
{
    int a[20];
    int num=0;
    while(x/10!=0)
    {
        int y=x%10;
        a[num++]=y;
        x=x/10;
    }
    a[num]=x;
   
    for(int i=0;i<=num;i++)
    {
        s[i]=a[num-i]+'0';
    }
    s[num+1]='\0';
    return;
}

static void stoi(char s[],int n)
{
   int index=0;
   while(s[index]!='\0')
   {
       n=n*10+s[index]-'0';
       index++;
   }
   return;
}
/*
int printf(const char *fmt, ...) {
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    cnt=0;
    const char* str=fmt;
    char temp[50];
    char *tem;
    char s[50];
    int x;

    while((*str)!='\0')
    {
        if((*str)=='%')
        {
            str++;
            switch(*str)
            {
                case 's':{
                             tem=va_arg(ap,char*);
                             cnt+=strlen(tem);
                             while(*tem!='\0')
                             {
                                 _putc(*tem);
                                 tem++;
                             }
                             break;
                         }
                case 'c':{
                             tem=va_arg(ap,char*);
                             _putc(*tem);
                             cnt++;
                             break;
                         }
                case 'd':{
                             memset(s,0,sizeof(s));
                             memset(temp,0,sizeof(temp));
                             x=va_arg(ap,int);
                             if(x==-2147483648)
                             {
                                 strcpy(temp,"-2147483648");
                                 cnt+=strlen(temp);
                             }
                             else if(x<0)
                             {
                                 x=-x;
                                 temp[0]='-';
                                 itos(s,x);
                                 strcat(temp,s);
                                 cnt+=strlen(temp);
                             }
                             else
                             {
                                 itos(s,x);
                                 strcpy(temp,s);
                                 cnt+=strlen(temp);
                             }
                             int index=0;
                             while(temp[index]!='\0')
                             {
                                 _putc(temp[index]);
                                 index++;
                             }
                             break;
                         }
                default: {
                             memset(temp,0,sizeof(temp));
                             strcpy(temp,"not implement!\n");
                             for(int index=0;;index++)
                             {
                                 if(temp[index]=='\0')
                                     break;
                                 _putc(temp[index]);
                             }
                             break;
                         }
            }
            str++;
        }
        else
        {
           _putc(*str);
           str++;
           cnt++;
        }
    }
    va_end(ap);
    return cnt;
}
*/

int printf(const char *fmt, ...) {
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    char s[200];
    cnt=vsprintf(s,fmt,ap);
    for(int i=0;;i++)
    {
        if(s[i]=='\0')
            break;
        _putc(s[i]);
    }
    va_end(ap);
    return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
    assert(fmt);
    cntvs=0;
    const char *str=fmt;
    char *tem=NULL;
    char temp[50];
    char s[50];
    int x;
    int flag=0;
    memset(out,0,sizeof(out));

    char num[20];
    int k,n=0;

    while((*str)!='\0')
    {
        if(*str=='%')
        {
            str++;
            //control width
            while(*str==48)
            {
                flag=1;
                str++;
            }
            k=0;
            memset(num,0,sizeof(num));
            while(*str>48&&*str<=57)
            {
               num[k]=*str;
               k++;
               str++;
            }
            if(num[0]!='\0')
            {
                stoi(num,n);
            }

// begin to check what to print
            switch(*str)
            {
                case 's':{
                             tem=va_arg(ap,char*);
                             if(flag)
                             {
                                if(strlen(tem)>=n)
                                {
                                   strcat(out,tem);
                                   cntvs+=strlen(tem);
                                }
                                else
                                {
                                    char s1[10]="";
                                    for(int i=0;i<n-strlen(tem);i++)
                                        s1[i]='0';
                                    s1[n-strlen(tem)]='\0';
                                    strcat(out,s1);
                                    cntvs+=strlen(s1);
                                    strcat(out,tem);
                                    cntvs+=strlen(tem);
                                }
                             }
                             else
                             {
                                if(strlen(tem)>=n)
                                {
                                   strcat(out,tem);
                                   cntvs+=strlen(tem);
                                }
                                else
                                {
                                    char s1[10]="";
                                    for(int i=0;i<n-strlen(tem);i++)
                                        s1[i]=' ';
                                    s1[n-strlen(tem)]='\0';
                                    strcat(out,s1);
                                    cntvs+=strlen(s1);
                                    strcat(out,tem);
                                    cntvs+=strlen(tem);
                                }
                             }
                             break;
                         }
                case 'd':{
                             memset(s,0,sizeof(s));
                             memset(temp,0,sizeof(temp));
                             x=va_arg(ap,int);
                             if(x==-2147483648)
                             {
                                 strcpy(temp,"-2147483648");
                                 strcat(out,temp);
                                 cntvs+=strlen(temp);
                             }
                             else if(x<0)
                             {
                                 x=-x;
                                 temp[0]='-';
                                 strcat(out,temp);
                                 cntvs++;
                                 n--;
                                 itos(s,x);
                                 if(flag)
                                 {
                                     if(strlen(s)>=n)
                                     {
                                         strcat(out,s);
                                         cntvs+=strlen(s);
                                     }
                                     else
                                     {
                                        char s1[10]="";
                                        for(int i=0;i<n-strlen(s);i++)
                                            s1[i]='0';
                                        s1[n-strlen(s)]='\0';
                                        strcat(out,s1);
                                        cntvs+=strlen(s1);
                                        strcat(out,s);
                                        cntvs+=strlen(s);
                                     }
                                 }
                                 else
                                 {
                                     if(strlen(s)>=n)
                                     {
                                         strcat(out,s);
                                         cntvs+=strlen(s);
                                     }
                                     else
                                     {
                                        char s1[10]="";
                                        for(int i=0;i<n-strlen(s);i++)
                                            s1[i]=' ';
                                        s1[n-strlen(s)]='\0';
                                        strcat(out,s1);
                                        cntvs+=strlen(s1);
                                        strcat(out,s);
                                        cntvs+=strlen(s);
                                     }
                                 }
                             }
                             else
                             {
                                 itos(s,x);
                                 if(flag)
                                 {
                                     if(strlen(s)>=n)
                                     {
                                         strcat(out,s);
                                         cntvs+=strlen(s);
                                     }
                                     else
                                     {
                                        char s1[10]="";
                                        for(int i=0;i<n-strlen(s);i++)
                                            s1[i]='0';
                                        s1[n-strlen(s)]='\0';
                                        strcat(out,s1);
                                        cntvs+=strlen(s1);
                                        strcat(out,s);
                                        cntvs+=strlen(s);
                                     }
                                 }
                                 else
                                 {
                                     if(strlen(s)>=n)
                                     {
                                         strcat(out,s);
                                         cntvs+=strlen(s);
                                     }
                                     else
                                     {
                                        char s1[10]="";
                                        for(int i=0;i<n-strlen(s);i++)
                                            s1[i]=' ';
                                        s1[n-strlen(s)]='\0';
                                        strcat(out,s1);
                                        cntvs+=strlen(s1);
                                        strcat(out,s);
                                        cntvs+=strlen(s);
                                     }
                                 }
                             }    
                             break;
                         }
                case 'c':{
                            tem=va_arg(ap,char*);
                            assert(strlen(tem)==1);
                             if(flag)
                             {
                                if(strlen(tem)>=n)
                                {
                                   strcat(out,tem);
                                   cntvs+=strlen(tem);
                                }
                                else
                                {
                                    char s1[10]="";
                                    for(int i=0;i<n-strlen(tem);i++)
                                        s1[i]='0';
                                    s1[n-strlen(tem)]='\0';
                                    strcat(out,s1);
                                    cntvs+=strlen(s1);
                                    strcat(out,tem);
                                    cntvs+=strlen(tem);
                                }
                             }
                             else
                             {
                                if(strlen(tem)>=n)
                                {
                                   strcat(out,tem);
                                   cntvs+=strlen(tem);
                                }
                                else
                                {
                                    char s1[10]="";
                                    for(int i=0;i<n-strlen(tem);i++)
                                        s1[i]=' ';
                                    s1[n-strlen(tem)]='\0';
                                    strcat(out,s1);
                                    cntvs+=strlen(s1);
                                    strcat(out,tem);
                                    cntvs+=strlen(tem);
                                }
                            }
                            break;
                         }
            }
            str++;
        }
        else
        {
            char ss[2];
            memset(ss,0,sizeof(ss));
            ss[0]=*str;
            ss[1]='\0';
            strcat(out,ss);
            cntvs++;
            str++;
        }
    } 
    return cntvs;
}


int sprintf(char *out, const char *fmt,...){
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    cnts=0;
    const char *str=fmt;
    char *tem=NULL;
    char temp[50];
    char s[50];
    int x;
    memset(out,0,sizeof(out));

    while((*str)!='\0')
    {
        if(*str=='%')
        {
            str++;
            switch(*str)
            {
                case 's':{
                            tem=va_arg(ap,char*);
                            strcat(out,tem);
                            cnts+=strlen(tem);
                            break;
                         }
                        
                case 'd': {
                               memset(s,0,sizeof(s));
                               memset(temp,0,sizeof(temp));
                               x=va_arg(ap,int);
                               if(x==-2147483648)
                               {
                                    strcpy(temp,"-2147483648");
                                    strcat(out,temp);
                                    cnts+=strlen(temp);
                               }
                               else if(x<0)
                               {
                                  x=-x;
                                  temp[0]='-';
                                  itos(s,x);
                                  strcat(temp,s);
                                  strcat(out,temp);
                                  cnts+=strlen(temp);
                                 
                               }
                               else 
                               {
                                   itos(s,x);
                                   strcat(out,s);
                                   cnts+=strlen(s);
                               }
                               break;
                          }
            }
            str++;
        }
        else
        {
           memset(temp,0,sizeof(temp));
           temp[0]=*str;
           temp[1]='\0';
           strcat(out,temp);
           cnts++;
           str++;
        }
            
    }
    va_end(ap);
    return cnts;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif

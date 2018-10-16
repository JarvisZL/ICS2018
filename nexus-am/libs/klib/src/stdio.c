#include "klib.h"
#include <stdarg.h>

#ifndef __ISA_NATIVE__

static int cnt;
/*
static void itos(char* s,int x)
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
        *s=a[num-i]-'0';
        s++;
    }
    *s='\0';
    return;
}
*/
int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}


int sprintf(char *out, const char *fmt,...){
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    cnt=0;
    const char *str=fmt;
    char *tem=NULL;
//    char *s=NULL;
    int x;

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
                            cnt+=strlen(tem);
                            break;
                         }
                        
                case 'd': {
                               x=va_arg(ap,int);
                               if(x==-2147483648)
                               {
                                    tem="-2147483648";
                                    strcat(out,tem);
                                    cnt+=strlen(tem);
                               }
                               break;
                          }
            }
        }
        else
            str++;
    }
    return cnt;
}

/*
int sprintf(char *out, const char *fmt, ...) {
    assert(fmt);
    va_list ap;
    va_start(ap,fmt);
    cnt=0;
    const char *str=fmt;
    char *ptr=out;

    while(*str!='\0')
    {
        if(*str=='%')
        {
            str++;
            switch(*str)
            {
                case 's': {
                              char *tem=va_arg(ap, char*);
                              strcat(ptr,tem);
                              cnt+=strlen(tem);
                          }
                          break;
             
               
                case 'd': {
                              int x=va_arg(ap,int);
                              char *tem=NULL;
                              char *ss=NULL;
                              if(x==-2147483648)
                              {
                                  char temp[12]="-2147483648";
                                  strcat(ptr,temp);
                                  cnt+=strlen(temp);
                              }
                              else if(x<0)
                              {
                                  x=-x;
                                  *ss='-';
                                  tem=ss;
                                  ss++;
                                  itos(ss,x);
                                  strcat(ptr,tem);
                                  cnt+=strlen(tem);
                              }
                              else if(x>=0)
                              {
                                  tem=ss;
                                  itos(ss,x);
                                  strcat(ptr,tem);
                                  cnt+=strlen(tem);
                              }
                              break;
                          } 
            }
        }
      else
      {
          char *tem=NULL;
          *tem=*str;
          *(tem+1)='\0';
          strcat(ptr,tem);
          cnt++;
          str++;
      }
    }
    va_end(ap);
    return cnt;
}
*/
int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif

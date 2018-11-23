#include "common.h"
#include <amdev.h>
#include "unistd.h"
#include <math.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
    char *ptr=(char*) buf;
    for(size_t i=0;i<len;++i)
        _putc(ptr[i]);
    return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

static char eve[50];
#define KEYDOWN_MASK 0x8000
size_t events_read(void *buf, size_t offset, size_t len) {
    memset(eve,0,sizeof(eve));
    int code=0;
    uint32_t timer=0;
    code=read_key();
    if(code!=_KEY_NONE)
    {
      if(code&KEYDOWN_MASK)
      {
          sprintf(eve,"kd %s\n",keyname[code&(~KEYDOWN_MASK)]);
      }
      else
      {
          sprintf(eve,"ku %s\n",keyname[code]);
      }
    }
    else
    {
       timer=uptime();
       sprintf(eve,"t %u\n",timer);
    }
    if(strlen(eve)>len)
         memcpy(buf,eve,len);
     else
    {
         len=strlen(eve);
         memcpy(buf,eve,len);
     }
    return len;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    size_t slen=strlen(dispinfo);
    if(offset==slen||len==0)
        return 0;
    if(offset+len>slen)
        len=slen-offset;
    memcpy(buf,dispinfo+offset,len);
    return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int w=screen_width();
   // int h=screen_height();
    int x=0, y=0;
    x=(offset%(4*w))/4;//offset and len are both Byte
    y=offset/(4*w);
    int w0=0,h0=0;
    if(4*x+len<=4*w)
    {
        w0=len/4;
        h0=1;
    }
    else
    {
        w0=w-x;
        h0=((len/4)%w0==0)?(len/4)/w0:((len/4)/w0)+1;//associated with pa2 vga
    }

    draw_rect((uint32_t*)buf,x,y,w0,h0); 
    return len;
}

extern int fs_open(const char* pathname);
extern ssize_t fs_read(int fd,void * buf,size_t len);

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
   sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}

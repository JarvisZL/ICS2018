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

size_t events_read(void *buf, size_t offset, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
    size_t slen=strlen(dispinfo);
    if(offset==slen*4||len==0)
        return 0;
    if(offset+len>slen*4)
        len=slen*4-offset;
    memcpy(buf,dispinfo+offset,len);
    return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
    int w=screen_width();
   // int h=screen_height();
    int x=0, y=0;
    x=(offset%(4*w))/4;
    y=offset/(16*w);
    Log("x:%d,y:%d,offset:%d,len:%d",x,y,offset,len);
    draw_rect((uint32_t*)buf,x,y,w,len/(4*w)); 
    return len;
}

extern int fs_open(const char* pathname);
extern ssize_t fs_read(int fd,void * buf,size_t len);

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
   sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d",screen_width(),screen_height());
   Log("%s",dispinfo);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}

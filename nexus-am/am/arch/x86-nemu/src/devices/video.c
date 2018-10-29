#include <am.h>
#include <x86.h>
#include <amdev.h>
#include <klib.h>
#define SCREEN_PORT 0x100
#define SCREEN_W 400
#define SCREEN_H 300
static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

//static uint32_t info0;
static uint32_t info1;

static inline int min(int x,int y)
{
    return x<y ? x:y;
}

size_t video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      info1=inl(SCREEN_PORT);
      _VideoInfoReg *info = (_VideoInfoReg *)buf;
      info->width = ((info1>>16)&0xffff);//-((info0>>16)&0xffff);
      info->height =(info1&0xffff);//-(info0&0xffff);
      return sizeof(_VideoInfoReg);
    }
  }
  return 0;
}

size_t video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _FBCtlReg *ctl = (_FBCtlReg *)buf;
    /*
      int i;
      int size =screen_width()*screen_height();
      for(i=0; i<size; i++) fb[i]=i;
      */
      int x=ctl->x, y=ctl->y, w=ctl->w, h=ctl->h;
      uint32_t *pixels=ctl->pixels;
      int cp_bytes=sizeof(uint32_t) *min(w,SCREEN_W-x);
      for(int j=0;j<h&&(y+j)<SCREEN_H;j++){
          memcpy(&fb[(y+j)*SCREEN_W+x],pixels,cp_bytes);
          pixels+=w;
      } 
      
      if (ctl->sync) {
        // do nothing, hardware syncs.
      }
      return sizeof(_FBCtlReg);
    }
  }
  return 0;
}

void vga_init() {
    //info0=inl(SCREEN_PORT);
}

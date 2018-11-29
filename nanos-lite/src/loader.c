#include "proc.h"

#define DEFAULT_ENTRY 0x4000000


static void* buff;
extern size_t get_ramdisk_size();
extern size_t ramdisk_read(void* buf, size_t offset,size_t len);
extern size_t fs_read(int fd,void * buf,size_t len);
extern int fs_open(const char *pathname,int flags,int mode);
extern size_t fs_filesz(int fd);
extern int fs_close(int fd);


static uintptr_t loader(PCB *pcb, const char *filename) {
    int fd=fs_open(filename,0,0);
    size_t length=fs_filesz(fd);
    
 //   size_t length=get_ramdisk_size();
    buff=(void *)DEFAULT_ENTRY;
    fs_read(fd,buff,length);
    // ramdisk_read(buff,0,length);
    
    fs_close(fd); 
    // TODO();
  return DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->tf = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}

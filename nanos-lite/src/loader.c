#include "proc.h"

#define DEFAULT_ENTRY 0x4000000


static void* buff;
extern size_t get_ramdisk_size();
extern size_t ramdisk_read(void* buf, size_t offset,size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
    size_t length=get_ramdisk_size();
    buff=(void *)DEFAULT_ENTRY;
    ramdisk_read(buff,0,length);
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

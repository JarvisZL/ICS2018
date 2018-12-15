#include "proc.h"
#define DEFAULT_ENTRY 0x8048000
#define PG_SIZE 4096


//static void* buff;
extern size_t get_ramdisk_size();
extern size_t ramdisk_read(void* buf, size_t offset,size_t len);
extern size_t fs_read(int fd,void * buf,size_t len);
extern int fs_open(const char *pathname,int flags,int mode);
extern size_t fs_filesz(int fd);
extern int fs_close(int fd);


static uintptr_t loader(PCB *pcb, const char *filename) {
   
    int fd=fs_open(filename,0,0);
    size_t length=fs_filesz(fd);
    
    
    uint32_t times=(length-1)/PG_SIZE; 
    
    for(uint32_t i=0;i<=times;++i)
    {
        uintptr_t vaddr=DEFAULT_ENTRY+i*PG_SIZE;
        void * page=new_page(1);
    //    PDE pde=(PDE)paddr_read((uintptr_t)pcb->as.ptr,4);

        _map(&pcb->as,(void *)vaddr,page,1);

   //     fs_lseek(fd,i*PG_SIZE,SEEK_SET);
      if(i==times)
      {
          fs_read(fd,page,length&PGMASK);
      }
      else
      {
          fs_read(fd,page,PG_SIZE);
      }
    }

    pcb->max_brk=pcb->cur_brk=DEFAULT_ENTRY+(times+1)*PG_SIZE;
    
    //buff=(void *)DEFAULT_ENTRY;
    //fs_read(fd,buff,length);
    
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

  pcb->cp = _kcontext(stack, entry, NULL);
}


void context_uload(PCB *pcb, const char *filename) {
  _protect(&pcb->as);

  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}

#include "nemu.h"
#include "memory/mmu.h"
#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

extern int is_mmio(paddr_t addr);
extern uint32_t mmio_read(paddr_t addr,int len,int map_NO);
extern void mmio_write(paddr_t addr,int len,uint32_t data,int map_NO);

/*
uint32_t paddr_read(paddr_t addr, int len) {
  return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}
*/
uint32_t paddr_read(paddr_t addr, int len) {
     if(is_mmio(addr)!=-1)
        return mmio_read(addr,len,is_mmio(addr)); 
     else
        return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
}

/*
void paddr_write(paddr_t addr, uint32_t data, int len) {
  memcpy(guest_to_host(addr), &data, len);
}
*/
void paddr_write(paddr_t addr, uint32_t data, int len) {
    if(is_mmio(addr)!=-1)
        mmio_write(addr,len,data,is_mmio(addr));
    else
        memcpy(guest_to_host(addr), &data, len);
}


paddr_t page_translate(vaddr_t addr)
{
    if(cpu.cr0.paging==0)
    {
        return addr;
    }
    else
    {
        uintptr_t dir=(addr>>22)<<2;
        
        uintptr_t base=cpu.cr3.page_directory_base<<12;
        uintptr_t pde_p=base|dir;
        
        PDE pde=(PDE)paddr_read(pde_p,4);
        if(pde.present==0)
        {
            Log("addr:%x",addr);
            Log("dir:%lx",dir);
            Log("base:%lx",base);
            Log("pde_p:%lx",pde_p);
            assert(0);
        }
        //assert(pde.present);
        /*
         * pay attention that pde_p is a physical addr, which is about nemu instead of real machine, so DO NOT use pointer like pde_p->present which will cause sf
         *
         */
        
        uintptr_t sec_base=pde.page_frame<<12;
        uintptr_t page=((addr>>12)<<22)>>20;
        uintptr_t pte_p=sec_base|page;
        
        PTE pte=(PTE)paddr_read(pte_p,4);
        assert(pte.present);

        uintptr_t page_base=pte.page_frame<<12;
        uintptr_t offset=addr&0xfff;
        
        return (paddr_t) (page_base|offset);
       // assert(0);
    }
}

uint32_t vaddr_read(vaddr_t addr, int len) {
    if(((addr+len-1)>>12) > (addr >> 12))
    {
        vaddr_t new_addr=(addr&0xfffff000)+0x1000;
        int fir_len=new_addr-addr;
        int sec_len=len-fir_len;
        paddr_t pa0=page_translate(addr);
        uint32_t val0=paddr_read(pa0,fir_len);
        paddr_t pa1=page_translate(new_addr);
        uint32_t val1=paddr_read(pa1,sec_len);
        uint32_t ret=(val1<<(fir_len*8))+val0;
        return ret;
        //assert(0);
    }
    else 
    {
        paddr_t paddr=page_translate(addr);
        return paddr_read(paddr,len);
    }
  //  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if(((addr+len-1)>>12) > (addr >>12))
  {
      assert(0);
  }
  else
  {
      paddr_t paddr=page_translate(addr);
      paddr_write(paddr,data,len);
  }
   // paddr_write(addr, data, len);
}

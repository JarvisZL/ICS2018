#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
    rtl_lm(&t0,&id_dest->addr,2);
    cpu.IDTR.limit=t0;
    t1=id_dest->addr+0x2;
    if(id_dest->width==2)
    {
         rtl_lm(&t2,&t1,3);
         cpu.IDTR.base=t2;
    }
    else if(id_dest->width==4)
    {
        rtl_lm(&t2,&t1,4);
        cpu.IDTR.base=t2;
    }
    // TODO();
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

extern uint32_t pio_read_w(ioaddr_t addr);
extern uint32_t pio_read_l(ioaddr_t addr);
extern uint32_t pio_read_b(ioaddr_t addr);

make_EHelper(in) {
 switch(id_dest->width)
 {
     case 1: t2=pio_read_b(id_src->val); operand_write(id_dest,&t2); break;
     case 2: t2=pio_read_w(id_src->val); operand_write(id_dest,&t2); break;
     case 4: t2=pio_read_l(id_src->val); operand_write(id_dest,&t2); break;
 }
    // TODO();

  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif

}

extern uint32_t pio_write_l(ioaddr_t addr,uint32_t data);
extern uint32_t pio_write_w(ioaddr_t addr,uint32_t data);
extern uint32_t pio_write_b(ioaddr_t addr,uint32_t data);

make_EHelper(out) {
    switch(id_src->width)
    {
        case 1: pio_write_b(id_dest->val,id_src->val); break;
        case 2: pio_write_w(id_dest->val,id_src->val); break;
        case 4: pio_write_l(id_dest->val,id_src->val); break;
    }
    //TODO();

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
    rtl_and(&t2,&(id_dest->val),&(id_src->val));
    cpu.EFLAGS.OF=0;
    cpu.EFLAGS.CF=0;
    rtl_update_ZFSF(&t2,id_dest->width);
    //TODO();

  print_asm_template2(test);
}

make_EHelper(and) {
   rtl_and(&t1,&(id_dest->val),&(id_src->val));
   operand_write(id_dest,&t1);
   cpu.EFLAGS.OF=0;
   cpu.EFLAGS.CF=0;
   rtl_update_ZFSF(&t1,id_dest->width);
    // TODO();

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&t2,&(id_dest->val),&(id_src->val));
  operand_write(id_dest,&t2);
  rtl_update_ZFSF(&t2,id_dest->width);
  cpu.EFLAGS.OF=0;
  cpu.EFLAGS.CF=0;
//  TODO();

  print_asm_template2(xor);
}

make_EHelper(or) {
   rtl_or(&t2,&(id_dest->val),&(id_src->val));
   operand_write(id_dest,&t2);
   cpu.EFLAGS.OF=0;
   cpu.EFLAGS.CF=0;
   rtl_update_ZFSF(&t2,id_dest->width);
//TODO();

  print_asm_template2(or);
}

make_EHelper(sar) {
   rtl_sar(&t2,&(id_dest->val),&(id_src->val));
   operand_write(id_dest,&t2);
   rtl_update_ZFSF(&t2,id_dest->width);
   // TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
   rtl_shl(&t2,&(id_dest->val),&(id_src->val));
   operand_write(id_dest,&t2);
   rtl_update_ZFSF(&t2,id_dest->width);
    // TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  rtl_shr(&t2,&id_dest->val,&id_src->val);
  operand_write(id_dest,&t2);
  rtl_update_ZFSF(&t2,id_dest->width);
    // TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
    rtl_not(&t2,&(id_dest->val));
    operand_write(id_dest,&t2);
    // TODO();

  print_asm_template1(not);
}

make_EHelper(rol){
   for(int i=0;i<id_src->val;i++)
   {
       if(i==0) t0=id_dest->val;
       switch(id_dest->width){
           case 1: {
                      t2=(t0>>7)&0x1;
                      t1=1;
                      rtl_shl(&t0,&t0,&t1);
                      t0=t0|t2;
                      operand_write(id_dest,&t0);
                      break;
                   }
           case 2 :{
                       t2=(t0>>15)&0x1;
                       t1=1;
                       rtl_shl(&t0,&t0,&t1);
                       t0=t0|t2;
                       operand_write(id_dest,&t0);
                       break;
                   }
           case 4 :{
                       t2=(t0>>31)&0x1;
                       t1=1;
                       rtl_shl(&t0,&t0,&t1);
                       t0=t0|t2;
                       operand_write(id_dest,&t0);
                       break;
                   }
          default: assert(0);
       }
   }
   if(id_src->val==1)
   {
       if(t2==cpu.EFLAGS.CF)
           cpu.EFLAGS.OF=1;
       else
           cpu.EFLAGS.OF=0;
   }
    print_asm_template2(rol);
}

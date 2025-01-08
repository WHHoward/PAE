#include "cpu/exec.h"

//make_EHelper(lui);

//make_EHelper(ld);
//make_EHelper(st);

make_EHelper(inv);
make_EHelper(nemu_trap);

//U-type
make_EHelper(lui);
make_EHelper(auipc);

//J-type
make_EHelper(jal);

//B-type beq,bne,blt,bge,bltu,bgeu,br
make_EHelper(beq);
make_EHelper(bne);
make_EHelper(blt);
make_EHelper(bge);
make_EHelper(bltu);
make_EHelper(bgeu);
make_EHelper(br);

//load
make_EHelper(ld);
make_EHelper(lb);
make_EHelper(lh);
make_EHelper(st);

//I-type jalr,addi,slli,slti,sltiu,xori,srli_srai,ori,andi
make_EHelper(jalr);
make_EHelper(addi);
make_EHelper(slli);
make_EHelper(slti);
make_EHelper(sltiu);
make_EHelper(xori);
make_EHelper(srli_srai);
make_EHelper(ori);
make_EHelper(andi);

//R-type sub_add,sll,slt,sltu,xor,srl_sra,or,and
make_EHelper(sub_add);
make_EHelper(sll);
make_EHelper(slt);
make_EHelper(sltu);
make_EHelper(xor);
make_EHelper(srl_sra);
make_EHelper(or);
make_EHelper(and);

//system
make_EHelper(system);
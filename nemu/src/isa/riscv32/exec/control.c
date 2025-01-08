#include "cpu/exec.h"

//jal
make_EHelper(jal)
{
  s0 = cpu.pc + 4;
  //rtl_sr(id_dest->reg, &decinfo.seq_pc, 4);
  rtl_sr(id_dest->reg, &s0, 4);
  rtl_add(&decinfo.seq_pc, &cpu.pc, &id_src->val);
  interpret_rtl_j(decinfo.seq_pc);
  print_asm_template2(jal);
}

//jalr
make_EHelper(jalr)
{
  s0 = cpu.pc + 4;
  rtl_sr(id_dest->reg, &s0, 4);
  rtl_add(&decinfo.seq_pc, &id_src->val, &id_src2->val);
  interpret_rtl_j(decinfo.seq_pc);
  print_asm_template2(jalr);
}

//beq
make_EHelper(beq)
{
  rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(beq);
}

//bne
make_EHelper(bne)
{
  rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(bne);
}

//blt
make_EHelper(blt)
{
  rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(blt);
}

//bge
make_EHelper(bge)
{
  rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(bge);
}

//bltu
make_EHelper(bltu)
{
  rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(bltu);
}

//bgeu
make_EHelper(bgeu)
{
  rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, decinfo.jmp_pc);
  print_asm_template3(bgeu);
}


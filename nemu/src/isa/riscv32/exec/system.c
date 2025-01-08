#include "cpu/exec.h"
extern void raise_intr(uint32_t NO, vaddr_t epc);
int32_t read_csr(int32_t csr_num)
{
  switch (csr_num)
  {
  case 0x100:
    return decinfo.isa.sstatus;
  case 0x142:
    return decinfo.isa.scause;
  case 0x105:
    return decinfo.isa.stvec;
  case 0x141:
    return decinfo.isa.sepc;
  default:
    assert(0);
  }
}

void write_csr(int32_t csr_num, int32_t val)
{
  switch (csr_num)
  {
  case 0x100:
    decinfo.isa.sstatus = val;
    break;
  case 0x142:
    decinfo.isa.scause = val;
    break;
  case 0x105:
    decinfo.isa.stvec = val;
    break;
  case 0x141:
    decinfo.isa.sepc = val;
    break;
  default:
    assert(0);
  }
}

make_EHelper(system)
{
  Instr instr = decinfo.isa.instr;
  switch (instr.funct3)
  {
    case 0b000:
      if((instr.val & ~(0x7f)) == 0)//ecall
      {
        raise_intr(reg_l(17), cpu.pc);
      }
      else if(instr.val == 0x10200073)// sret
      {
        decinfo.jmp_pc = decinfo.isa.sepc + 4;
        rtl_j(decinfo.jmp_pc);
      }
      else
      {
        assert(0);
      }
      break;
    case 0b001: //csrrw
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, id_src->val);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(cssrw);
      break;
    case 0b010: //csrrs
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, s0 | id_src->val);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(csrrs);
      break;
    case 0b011: //csrrc
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, s0 & ~id_src->val);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(csrrc);
      break;
    case 0b101: //csrrwi
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, id_src->reg);
      rtl_li(&s1, instr.rs1);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(csrrwi);
      break;
    case 0b110: //csrrsi
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, s0 | id_src->reg);
      rtl_li(&s1, instr.rs1);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(csrrsi);
      break;
    case 0b111: //csrrci
      s0 = read_csr(instr.csr);
      write_csr(instr.csr, s0 & ~id_src->reg);
      rtl_li(&s1, instr.rs1);
      rtl_sr(id_dest->reg, &s0, 4);
      print_asm_template3(csrrci);
      break;
    default:
      assert(0);
      break;
  }
}

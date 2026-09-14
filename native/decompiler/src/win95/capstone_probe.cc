// Capstone integration probe: proves the bundled Capstone 4.0.2 static lib links
// into our MSVC x64 build and decodes both 16-bit (NE/MZ) and 32-bit (PE/LE) x86
// with full operand detail -- the accurate decode + typed operands our hand-written
// front-end lacks. If this prints correct disassembly, Capstone can back the lifter.
#define CAPSTONE_STATIC
#include "capstone.h"
#include <cstdio>

static void run(const char* label, cs_mode mode, const uint8_t* code, size_t n, uint64_t addr) {
  csh h;
  if (cs_open(CS_ARCH_X86, mode, &h) != CS_ERR_OK) { std::printf("%s: cs_open FAILED\n", label); return; }
  cs_option(h, CS_OPT_DETAIL, CS_OPT_ON);
  cs_insn* insn;
  size_t count = cs_disasm(h, code, n, addr, 0, &insn);
  std::printf("== %s == (%zu insns)\n", label, count);
  for (size_t i = 0; i < count; ++i) {
    std::printf("  %04llx  %-10s %-24s", (unsigned long long)insn[i].address,
                insn[i].mnemonic, insn[i].op_str);
    cs_x86& x = insn[i].detail->x86;
    for (int o = 0; o < x.op_count; ++o) {
      cs_x86_op& op = x.operands[o];
      const char* t = op.type == X86_OP_REG ? "reg" : op.type == X86_OP_IMM ? "imm"
                    : op.type == X86_OP_MEM ? "mem" : "?";
      std::printf(" [%s sz=%d]", t, op.size);
    }
    std::printf("\n");
  }
  cs_free(insn, count);
  cs_close(&h);
}

int main() {
  int major, minor;
  cs_version(&major, &minor);
  std::printf("Capstone %d.%d linked OK\n\n", major, minor);
  // 32-bit: push ebp; mov ebp,esp; push 0x10; call rel32; mov eax,[ebp+8]; ret
  const uint8_t c32[] = {0x55, 0x8B, 0xEC, 0x68, 0x10, 0x00, 0x00, 0x00,
                         0xE8, 0x05, 0x00, 0x00, 0x00, 0x8B, 0x45, 0x08, 0xC3};
  run("x86-32 (PE/LE)", CS_MODE_32, c32, sizeof c32, 0x401000);
  // 16-bit: push bp; mov bp,sp; mov ax,[bp+6]; add ax,4; retf
  const uint8_t c16[] = {0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x06, 0x05, 0x04, 0x00, 0xCB};
  run("x86-16 (NE/MZ)", CS_MODE_16, c16, sizeof c16, 0x1000);
  return 0;
}

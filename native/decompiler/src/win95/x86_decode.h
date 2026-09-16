// Win95 port: 16/32-bit x86 instruction decoder.
// Variable-length decode: prefixes -> opcode (+0F map) -> ModRM/SIB -> disp/imm.
// Default operand/address size is 16-bit (NE code); 0x66/0x67 toggle to 32-bit.
// Produces a structured X86Insn the x86->IL lifter consumes (and a text form for
// disassembly output). Replaces XEXMagic's ppc_disasm for the x86 port.
#pragma once

#include <cstdint>
#include <string>

namespace win95 {

// Register ids. Encoded as (class*16 + index). Index 0..7 = AX/CX/DX/BX/SP/BP/SI/DI
// order (the x86 encoding order). Segment regs 0..5 = ES/CS/SS/DS/FS/GS.
enum RegClass { RC_GPR = 0, RC_SEG = 1, RC_ST = 2 };
constexpr int RegId(RegClass c, int idx) { return c * 16 + idx; }
constexpr int kNoReg = -1;

enum class OpKind { None, Reg, Mem, Imm, Rel, FarPtr };

struct X86Op {
  OpKind kind = OpKind::None;
  int    size = 2;          // operand size in bytes: 1, 2, or 4
  // Reg
  int    reg = kNoReg;
  // Mem: seg:[base + index*scale + disp]
  int    base = kNoReg;
  int    index = kNoReg;
  int    scale = 1;
  int    seg = kNoReg;      // segment override reg, or kNoReg for default
  int32_t disp = 0;
  // Imm / Rel
  int64_t imm = 0;          // immediate, or absolute target for Rel
  // FarPtr (direct): seg:off
  uint16_t far_seg = 0;
  uint32_t far_off = 0;
};

struct X86Insn {
  uint32_t addr = 0;        // linear address of the instruction
  int      len = 0;         // total encoded length in bytes
  std::string mnem;         // "mov", "add", "jmp", "call", "retf", ...
  X86Op    ops[3];
  int      nops = 0;

  // prefixes / state
  bool opsize = false;      // 0x66 applied (operand size flipped from default)
  bool addrsize = false;    // 0x67 applied
  bool lock = false;
  bool rep = false;         // F3
  bool repne = false;       // F2
  int  seg_override = kNoReg;
  bool is_far = false;      // far call/jmp/ret

  bool valid = false;       // false => could not decode (treated as data byte)
};

// Decode one instruction at data[pos]. default_op16 = true for NE (16-bit) code.
// Returns an X86Insn; on failure valid==false and len==1 (skip one byte).
X86Insn DecodeX86(const uint8_t* data, size_t size, size_t pos, uint32_t addr,
                  bool default_op16 = true);

// Assembly syntax flavour for FormatX86. Nasm is the readable objdump/NASM display
// (`word [si]`, `0x21`); Masm is the MASM/Watcom form (`word ptr [si]`, `21h`) our
// assemblers (wasm/ML) accept -- required when the text is used as an __asm fallback.
enum class AsmSyntax { Nasm, Masm };

// Disassembly of a decoded instruction, in the requested assembler syntax (default NASM).
std::string FormatX86(const X86Insn& in, AsmSyntax syn = AsmSyntax::Nasm);

// Register name for a (reg id, size) pair, e.g. (RC_GPR+0, 2) -> "ax".
std::string RegName(int reg, int size);

}  // namespace win95

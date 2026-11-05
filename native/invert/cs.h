// cs.h -- Capstone, as the models see it: the capstone.dll of Python's `capstone` package (5.0.7),
// loaded by path (cbinv_init), so every mnemonic and operand text is the one Python's decoder gave.
// native/decompiler's Capstone is 4.0.2 and prints differently in places: it is not used here.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cbinv {

struct Op {             // one operand, with detail (whole_function's matcher)
  int type = 0;         // capstone x86_op_type: 1 REG, 2 IMM, 3 MEM
  long long imm = 0;
  int mem_base = 0;     // X86_REG_INVALID = 0
  long long mem_disp = 0;
};

struct CsIns {
  long long address = 0;
  int size = 0;
  std::vector<uint8_t> bytes;
  std::string mnemonic, op_str;
  std::vector<Op> operands;   // only with detail
};

// Python's md.disasm(code, address): every instruction up to the first that does not decode.
std::vector<CsIns> disasm16(const uint8_t *code, size_t n, long long address, bool detail = false);
inline std::vector<CsIns> disasm16(const std::vector<uint8_t> &code, long long address, bool detail = false) {
  return disasm16(code.data(), code.size(), address, detail);
}

// once per process: the DLL's path; false (and a message) if it does not load
bool cs_load(const wchar_t *dll, std::string &err);

}  // namespace cbinv

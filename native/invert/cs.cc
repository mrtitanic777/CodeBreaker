// cs.cc -- the capstone.dll Python's package carries, its entry points looked up by name (LoadLibraryW
// of a path Python already loaded hands back that same module).  Three handles: 16-bit x86 without
// and with detail, as platforms.decoder(16) and platforms.decoder(16, detail=True) make them, and
// 32-bit x86 without, as platforms.decoder(32) (the MSVC 2.0 models).
#include "cs.h"

#include <capstone/capstone.h>

#include <cstdio>
#include <mutex>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace cbinv {

namespace {
typedef cs_err (*open_t)(cs_arch, cs_mode, csh *);
typedef cs_err (*option_t)(csh, cs_opt_type, size_t);
typedef size_t (*disasm_t)(csh, const uint8_t *, size_t, uint64_t, size_t, cs_insn **);
typedef void (*free_t)(cs_insn *, size_t);
open_t p_open;
option_t p_option;
disasm_t p_disasm;
free_t p_free;
csh plain_h, detail_h, plain32_h;
bool loaded;
std::mutex mu;
}  // namespace

bool cs_load(const wchar_t *dll, std::string &err) {
  std::lock_guard<std::mutex> g(mu);
  if (loaded) return true;
  HMODULE m = LoadLibraryW(dll);
  if (!m) {
    err = "cannot load capstone.dll (error " + std::to_string(GetLastError()) + ")";
    return false;
  }
  p_open = (open_t)GetProcAddress(m, "cs_open");
  p_option = (option_t)GetProcAddress(m, "cs_option");
  p_disasm = (disasm_t)GetProcAddress(m, "cs_disasm");
  p_free = (free_t)GetProcAddress(m, "cs_free");
  if (!p_open || !p_option || !p_disasm || !p_free) {
    err = "capstone.dll lacks cs_open/cs_option/cs_disasm/cs_free";
    return false;
  }
  if (p_open(CS_ARCH_X86, CS_MODE_16, &plain_h) != CS_ERR_OK || p_open(CS_ARCH_X86, CS_MODE_16, &detail_h) != CS_ERR_OK ||
      p_open(CS_ARCH_X86, CS_MODE_32, &plain32_h) != CS_ERR_OK) {
    err = "cs_open failed";
    return false;
  }
  p_option(detail_h, CS_OPT_DETAIL, CS_OPT_ON);
  loaded = true;
  return true;
}

namespace {
std::vector<CsIns> run(csh h, const uint8_t *code, size_t n, long long address, bool detail) {
  std::vector<CsIns> out;
  if (!loaded || n == 0) return out;
  cs_insn *all = nullptr;
  size_t k = p_disasm(h, code, n, (uint64_t)address, 0, &all);
  out.resize(k);
  for (size_t i = 0; i < k; i++) {
    CsIns &o = out[i];
    o.address = (long long)all[i].address;
    o.size = all[i].size;
    o.bytes.assign(all[i].bytes, all[i].bytes + all[i].size);
    o.mnemonic = all[i].mnemonic;
    o.op_str = all[i].op_str;
    if (detail && all[i].detail) {
      const cs_x86 &x = all[i].detail->x86;
      for (int j = 0; j < x.op_count; j++) {
        Op op;
        op.type = x.operands[j].type;
        if (op.type == X86_OP_IMM) op.imm = x.operands[j].imm;
        if (op.type == X86_OP_MEM) {
          op.mem_base = x.operands[j].mem.base;
          op.mem_disp = x.operands[j].mem.disp;
        }
        o.operands.push_back(op);
      }
    }
  }
  if (all) p_free(all, k);
  return out;
}
}  // namespace

std::vector<CsIns> disasm16(const uint8_t *code, size_t n, long long address, bool detail) {
  return run(detail ? detail_h : plain_h, code, n, address, detail);
}

std::vector<CsIns> disasm32(const uint8_t *code, size_t n, long long address) {
  return run(plain32_h, code, n, address, false);
}

}  // namespace cbinv

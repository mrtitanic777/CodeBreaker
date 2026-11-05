// cs32.h -- Capstone, 32-bit x86, by the capstone.dll Python's `capstone` package carries (5.0.7):
// the very decoder platforms.decoder(32) hands out, so a mnemonic and an operand text here are
// the ones Python saw.  The DLL is found by path (the shim passes it) and loaded once.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cbf {

struct CsIns {
    int64_t address = 0;
    int size = 0;
    std::string mnemonic, op_str;
    bool imm0 = false;        // detail: operands[0] is an immediate
    int64_t imm = 0;          //   and its value
};

// md.disasm(code, address) (count 0: every instruction up to the first that does not decode) or
// the first `count`; `detail` only for the operand facts.  Raises ImportError when the DLL
// cannot be loaded.
std::vector<CsIns> cs32_disasm(const std::wstring &dll, const uint8_t *code, size_t n, int64_t address,
                               size_t count = 0, bool detail = false);

}  // namespace cbf

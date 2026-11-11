// codebreaker/symbols/x86.hpp -- the capstone x86 decoder of platforms.decoder(bits, "x86", detail), as the symbols
// modules use it (api_arguments, runtime_library, names), and the 32-bit PE function carve of formats.pe_functions.
//
// The decoder is the capstone.dll Python's `capstone` package carries, loaded by path: hand it over once with
// set_capstone_dll() before the first use (a use without it is an ImportError, as a missing `capstone` module is).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "codebreaker/formats/pe.hpp"
#include "py.hpp"

namespace codebreaker::symbols::x86 {

void set_capstone_dll(const py::str& path);

// One decoded instruction (capstone's CsInsn; disp/imm fields only for a decoder made with detail=true).
struct Insn {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
    int64_t disp = 0, disp_size = 0, disp_offset = 0;
    int64_t imm_size = 0, imm_offset = 0;
    bool first_is_imm = false;      // operands[0].type == X86_OP_IMM
    int64_t first_imm = 0;          // operands[0].imm
};

// md.disasm(code, address[, count]) (count 0: to the end); `bits` 16 or 32
std::vector<Insn> disasm(int bits, bool detail, const std::string& code, int64_t address, size_t count = 0);

// md.disasm_lite(code, address): (address, size, mnemonic, op_str) of the instructions
struct Lite {
    int64_t address = 0, size = 0;
    std::string mnemonic, op_str;
};
std::vector<Lite> disasm_lite(int bits, const std::string& code, int64_t address);
// the first instruction only: `next(md.disasm_lite(code, address), None)`
std::optional<Lite> first_lite(int bits, const std::string& code, int64_t address);

// formats.pe_functions.PeCarve: the function starts and extents of a PE module, and its decoded instructions
class PeCarve {
  public:
    struct Func {
        int64_t start = 0, end = 0;
        std::optional<py::str> name;
    };
    std::shared_ptr<formats::pe::PEFile> pe;
    py::list<std::tuple<uint64_t, py::bytes, py::str>> sections;
    py::dict<int64_t, Func> funcs;                  // start -> function (ascending start)
    std::vector<int64_t> addresses;                // ascending addresses of every decoded instruction

    explicit PeCarve(const py::str& path);
    // (base, bytes) of the section holding `va`; base -1 when none
    std::pair<int64_t, const py::bytes*> sec_of(int64_t va) const;
    bool has_insn(int64_t va) const;
    // the instruction decoded at `va` (with detail)
    Insn insn(int64_t va) const;

  private:
    std::unordered_set<int64_t> is_decoded_;
};

// formats.pe_functions.carve(module): one carve per module path, kept
std::shared_ptr<PeCarve> carve(const py::str& path);

}  // namespace codebreaker::symbols::x86

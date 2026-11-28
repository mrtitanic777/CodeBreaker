// recompile/pcode_emit.hpp -- one Xbox 360 instruction as C++ on the recompiled program's context, written from its p-code.
//
//     pcode_emit::Translator t(languages / "ppc_64_xenon_be.sla");
//     std::optional<pcode_emit::Statements> s = t.instruction(address, word, next_word, names);
//
// Step 2 of docs/RECOMPILER.md: the semantics come from the description the decompiler lifts and the emulator runs, not from a table
// of instructions. Each p-code operation becomes one statement: a register of the description is the context field the recompiler
// names it by (`names`: a field of ctx or a local, as the title's configuration says), a temporary is a local of the instruction's
// block, LOAD and STORE go through PPC_LOAD_* / PPC_STORE_*, and the operations' values are unsigned integers of their size (a float
// operation reads and writes the bits). What the output looks like around the instruction (the function, its labels and branches) is
// the recompiler's: an instruction is declined (nullopt, and `why` says why) when it holds
//   * a way out of it (a branch, call or return to an address), or a user operation (VMX128's vector arithmetic, sync, the traps):
//     the recompiler writes those;
//   * a register the context does not have (MSR, the time base, the reservation of lwarx/stwcx., XER as a whole, xer_count), or reads
//     one the context keeps no value for (the FPSCR's bits): registers the context has no place for and nothing reads are dropped
//     (every floating-point operation updates about 25 FPSCR bits; `bl` copies r2 to r2Save);
//   * a fused multiply-add (the description multiplies and adds with two roundings, the processor with one), floating point on a
//     vector register, a store an `eieio` follows (memory-mapped I/O), `db16cyc`, or an operation or size the writer has no form for.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "decompile/engine/sleigh_reader.hh"

namespace cb::recompile::pcode_emit {

// How the recompiler names the context's registers in the function being written ("ctx.r3" or a local "r3" ...).
struct Names {
    std::function<std::string(std::size_t)> r, f, v, cr;
    std::function<std::string()> ctr, xer, lr;   // lr: empty when the context keeps no LR
    std::function<std::string()> msr, reserved;  // msr: empty when the context keeps no MSR; reserved: lwarx's reservation
    // The ways out of an instruction, as the recompiler writes them (lines that start with a tab): a branch to an address, a call, a
    // call and a branch through a register's value. Unset: an instruction with a way out is declined.
    std::function<std::string(std::uint32_t)> jump, call;
    std::function<std::string(const std::string&)> call_indirect, jump_indirect;
};

struct Statements {
    std::string text;    // each line starts with a tab and ends with a newline
    bool fpu = false;    // it computes with the floating-point unit
    bool vmx = false;    // it computes with the vector unit's floating point (the VMX flush mode)
};

class Translator {
public:
    explicit Translator(const std::filesystem::path& sla);

    // The statements of the instruction `word` at `address` (`next_word` is the one after it), or nullopt when it is declined.
    std::optional<Statements> instruction(std::uint32_t address, std::uint32_t word, std::uint32_t next_word, const Names& names,
                                          std::string* why = nullptr) const;

    // The instruction's mnemonic as the description prints it ("addi", "stwu" ...; empty for no instruction).
    std::string mnemonic(std::uint32_t address, std::uint32_t word) const;

    // A place of the register space the context has (or knows to drop).
    struct Cell {
        enum class Kind : std::uint8_t { Gpr, Fpr, Vector, Cr, Ctr, Lr, XerBit, Msr, TimeBase, Dropped };
        Kind kind = Kind::Dropped;
        std::uint64_t offset = 0;
        unsigned size = 0;
        unsigned index = 0;         // Gpr, Fpr, Vector, Cr: the register's number
        std::string member;         // XerBit: "so", "ov", "ca"; Dropped: the description's name
    };

private:
    decompile::sleigh::Reader reader_;
    std::vector<Cell> cells_;        // by offset
    const Cell* cell_at(std::uint64_t offset) const;
};

}  // namespace cb::recompile::pcode_emit

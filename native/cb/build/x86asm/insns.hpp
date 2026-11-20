// cb/build/x86asm/insns.hpp -- the instruction tables of x86asm: the templates the encoder matches against.
//
// NASM's x86/insns.dat is turned by insns.pl into three tables: every instruction template (NASM's struct itemplate: the
// opcode, the operand flag words, the decorator flags, the byte-code program that says how to encode it, and an index
// into the table of instruction flag sets), the byte-code programs themselves (nasm_bytecodes, see x86/bytecode.txt) and
// the flag sets (insns_flags). They are generated into insns_table.cpp (the generator is docs/history/x86asm-generators/
// encoder/gen_insns_table.py, which reads NASM's own generated files, so the data is exactly NASM's). This header is the
// way to get at them. The ItemTemplate structure itself is in types.hpp.
#pragma once

#include <array>
#include <cstdint>

#include "cb/build/x86asm/types.hpp"

namespace cb::build::x86asm {

// The numbers of things in the generated tables.
inline constexpr std::size_t kNumTemplates = 12320;
inline constexpr std::size_t kNumBytecodes = 73487;
inline constexpr std::size_t kNumInsnsFlags = 655;
inline constexpr std::size_t kNumInstructions = static_cast<std::size_t>(I_HINT_NOP63) + 1;

// Where the templates of one instruction are in kTemplates (NASM's struct itemplate_list: a count and a pointer).
struct TemplateRange {
    std::uint32_t first;   // index of the first template in kTemplates
    std::uint32_t count;   // how many templates the instruction has (0: none, e.g. a macro-only name)
};

extern const std::uint8_t kBytecodes[kNumBytecodes];                    // nasm_bytecodes
extern const std::array<IFlags, kNumInsnsFlags> kInsnsFlags;            // insns_flags
extern const std::array<ItemTemplate, kNumTemplates> kTemplates;        // every template, grouped by opcode
extern const std::array<TemplateRange, kNumInstructions> kInstructions; // nasm_instructions

// A view over the templates of one instruction.
struct TemplateList {
    const ItemTemplate* temp = nullptr;
    unsigned ntemp = 0;
    const ItemTemplate* begin() const { return temp; }
    const ItemTemplate* end() const { return temp + ntemp; }
};

// The templates of an opcode (nasm_instructions[opcode]); empty for an out-of-range opcode.
inline TemplateList templates_of(Opcode opcode) {
    if (opcode < 0 || static_cast<std::size_t>(opcode) >= kNumInstructions) return {};
    const TemplateRange& r = kInstructions[static_cast<std::size_t>(opcode)];
    return {kTemplates.data() + r.first, r.count};
}

// The flag set of a template (insns_flags[itemp->iflag_idx]) and a test of one of its flags (itemp_has).
inline const IFlags& itemp_flags(const ItemTemplate& t) { return kInsnsFlags[t.iflag_idx]; }
inline bool itemp_has(const ItemTemplate& t, unsigned bit) { return itemp_flags(t).test(bit); }

// The size-flag, argument-requirement and size-match masks of a template (itemp_smask, itemp_arx, itemp_smx).
inline std::uint32_t itemp_smask(const ItemTemplate& t) { return itemp_flags(t).field[0] & IF_SMASK; }
inline std::uint32_t itemp_armask(const ItemTemplate& t) { return itemp_flags(t).field[0] & IF_ARMASK; }
inline std::uint32_t itemp_smmask(const ItemTemplate& t) { return itemp_flags(t).field[0] & IF_SMMASK; }
inline std::uint32_t itemp_arx(const ItemTemplate& t) { return itemp_armask(t) >> IF_AR0; }
inline std::uint32_t itemp_smx(const ItemTemplate& t) { return itemp_smmask(t) >> IF_SM0; }

// The bytes of a template's byte-code program, up to (not including) its terminating zero.
inline const std::uint8_t* itemp_code(const ItemTemplate& t) { return t.code; }

}  // namespace cb::build::x86asm

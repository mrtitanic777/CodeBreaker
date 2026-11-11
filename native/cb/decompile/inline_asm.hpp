// cb/decompile/inline_asm.hpp -- a 16-bit function as Microsoft C 8 source whose body is one _asm block.
//
//     auto text = inline_asm::whole_body(name, code, start, end, fixups, name_at, seg, "int2");
//     if (text.ok()) use(text.value()); else say(text.error().what());   // why it cannot be said this way
//
// A function analyze/origin finds holding an _asm block (INT n, IN/OUT, CLI/STI, the carry -- inside the BP frame
// C 8 gives every function with one) was written as C with assembly in it. Its first form is the whole body as the
// block: the instructions between the frame C 8 makes and the return it makes, written back in the inline
// assembler's words. C 8 then builds the frame round them -- `push bp / mov bp, sp` or `enter n, 0`, `leave`, the
// return its convention says -- so whether the text is the function is the compiler's to say, byte for byte
// (verify/authored/match16: the "asm" way of saying). Measured with C 8.00c (2026-10-28): SUWIN's 2:7E3C
// (`mov bx, h / mov ah, 68h / int 21h`) and 8:0538 (INT 2Fh, then C round a second block) come out as the
// original's bytes.
//
// What the block keeps as the original has it: places in the frame as [bp+k] / [bp-k] (the frame is declared as
// many bytes as the original's), the module's data as ds:[n]. What it names: jumps within the function as labels,
// calls (a far call's fix-up, a near call's target) by the name the module's header declares. A function this
// cannot say (a jump out of it, a return in its middle, a fix-up it cannot name) is left as the decompiler wrote
// it: whole_body answers with a NotAsmBody that says why.
//
// The instructions come from Capstone (the prebuilt capstone.dll, cb/core/capstone_api): decode16() is the 16-bit
// decoder with details; whole_body takes another decoder when the instructions are already at hand.
#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::decompile::inline_asm {

// Why a function cannot be written as one _asm block: an expected outcome (the decompiler's own text is used
// instead), so it travels in a Result. what() is the reason, to be shown as it is.
class NotAsmBody : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(NotAsmBody)
};

enum class OperandKind : std::uint8_t { Invalid, Reg, Imm, Mem };   // Capstone's X86_OP_*

// A memory operand: Capstone's register ids (0 = none) and the displacement.
struct MemOperand {
    std::uint16_t segment = 0;
    std::uint16_t base = 0;
    std::uint16_t index = 0;
    std::uint8_t scale = 0;
    std::int64_t disp = 0;
};

// One operand of a decoded instruction (Capstone keeps reg and imm in one union: both are given as the decoder
// read them).
struct Operand {
    OperandKind kind = OperandKind::Invalid;
    std::uint16_t reg = 0;
    std::int64_t imm = 0;
    MemOperand mem;
};

// One decoded instruction (the fields of Capstone's cs_insn this module reads).
struct Instruction {
    std::uint32_t address = 0;
    std::uint8_t size = 0;
    std::string mnemonic;               // as Capstone says it ("rep stosw" carries its prefix)
    std::string op_str;
    Bytes bytes;                        // the instruction's own bytes
    std::array<std::uint8_t, 4> prefix{};
    std::vector<Operand> operands;

    // The mnemonic without a prefix word ("stosw" of "rep stosw").
    std::string_view base_mnemonic() const;
};

// The instructions of `code`, as the code sits at `address`; decoding stops at the first byte that is not one.
using Decoder = std::function<std::vector<Instruction>(ByteView code, std::uint32_t address)>;

// The 16-bit x86 decoder (Capstone, Intel syntax, details on).
std::vector<Instruction> decode16(ByteView code, std::uint32_t address);

// What the module's fix-ups say, by offset in the segment: the target's name (nullopt: the fix-up names nothing),
// and for a selector-only fix-up (the old ("segment", offset) keys) the segment it gives. Lookup only.
struct Fixups {
    std::unordered_map<std::uint32_t, std::optional<std::string>> names;
    std::unordered_map<std::uint32_t, std::optional<std::uint32_t>> segments;
};

// A function's name by segment number and offset, or nullopt.
using NameAt = std::function<std::optional<std::string>(std::uint32_t segment, std::uint16_t offset)>;

// The string instructions (movsb, stosw, ...) and the jumps the inline assembler takes labels for (jmp, the
// conditional jumps, jcxz, loop*), by name.
std::span<const std::string_view> string_op_names();
std::span<const std::string_view> jump_names();
bool is_string_op(std::string_view mnemonic);
bool is_jump(std::string_view mnemonic);

// Is it an instruction the inline assembler does not take -- the 386's, which /G3 makes: a 32-bit register, an
// operand-size prefix, a near conditional jump?
bool is_386(const Instruction& i);

// The frame C 8 makes for the function: how many instructions it takes, how many bytes of frame it declares, whether
// it loads DS (`__loadds`). nullopt when there is none.
struct Prologue {
    std::size_t taken = 0;
    std::int64_t frame = 0;
    bool loads_ds = false;
};
std::optional<Prologue> find_prologue(std::span<const Instruction> ins);

// The return C 8 makes: how many instructions it takes, whether it is far, how many bytes of arguments it removes.
struct Epilogue {
    std::size_t taken = 0;
    bool far = false;
    std::int64_t pops = 0;
};
std::optional<Epilogue> find_epilogue(std::span<const Instruction> ins, bool loads_ds);

// The instruction's operands in the inline assembler's words (a string instruction has none; a direct memory operand
// is the module's data, `ds:[n]`; `les`/`lds` get the `dword ptr` the assembler wants).
std::string operands_text(const Instruction& i);

// The function at code[start:end] as a C function whose body is one _asm block, with its result type `result`.
// `code` is the whole segment, start/end offsets in it (end is clamped to the segment); `seg` is the segment's number,
// what name_at is asked about a near call. The error is a NotAsmBody.
Result<std::string> whole_body(std::string_view name, ByteView code, std::uint32_t start, std::uint32_t end,
                               const Fixups& fixups, const NameAt& name_at, std::uint32_t seg,
                               std::string_view result = "int2", const Decoder& decoder = {});

}  // namespace cb::decompile::inline_asm

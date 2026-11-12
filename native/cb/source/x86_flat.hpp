// cb/source/x86_flat.hpp -- what the x86 writers that write a whole file as one NASM source (source/ne_writer, source/le_writer; the
// 32-bit pe_writer is the same kind) share: the instruction as the writers read it, which instructions end a flow, how an instruction is
// spelled for NASM, and the lines that put an instruction's mnemonic beside the bytes it is written as.
//
// An instruction is written as its mnemonic only where NASM gives the original's bytes back; what NASM would write another way (a
// register-to-register `mov` with the direction bit set, a short immediate where the long form was linked) is spelled so it does not,
// and anything it cannot be made to give back is written as bytes by the rounds of source/flat_rounds.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::source::x86_flat {

// One decoded instruction, with what the writers read of its encoding.
struct Insn {
    std::uint32_t address = 0;
    std::uint8_t size = 0;
    std::string mnemonic, op_str;
    std::array<std::uint8_t, 16> bytes{};   // the first `size` bytes are the instruction's
    std::uint8_t imm_size = 0, imm_offset = 0, disp_size = 0, disp_offset = 0;
    std::array<std::uint8_t, 4> prefix{};   // Capstone's four prefix bytes

    ByteView raw() const { return ByteView(bytes.data(), size); }
};

// The first instruction of `code` as `decoder` (opened with detail) reads it, or nothing where the bytes are not one.
std::optional<Insn> decode(const platforms::Decoder& decoder, ByteView code, std::uint32_t address);

// A branch, call or loop: j<cc>, jmp, jcxz, jecxz, call, loop<cc>.
bool is_branch(std::string_view mnemonic);

// The target a relative branch names as its operand ("0x1234"), nothing for any other instruction.
std::optional<std::uint32_t> branch_target(const Insn& insn);

// A string instruction as Capstone names it ("movsb", "rep stosw"); `quad_words` also takes the 64-bit width (movsq).
bool is_string_op(std::string_view mnemonic, bool quad_words = false);

// The segment-override prefixes (es cs ss ds fs gs).
bool is_segment_prefix(std::uint8_t byte);

// The instructions whose immediate may be a sign-extended byte, so that NASM writes the short form where the original linked the long.
bool is_arith(std::string_view mnemonic);

// A 32-, 16- or 8-bit general register's name.
bool is_register(std::string_view name);

// The two-operand instructions that have a register-to-register form in both directions, and the opcode of the one with the direction
// bit clear (add 0x00, or 0x08 ... mov 0x88); in name order.
struct RegisterPairOp {
    std::string_view mnemonic;
    std::uint8_t opcode;
};
std::span<const RegisterPairOp> register_pair_ops();
bool has_register_pair_form(std::string_view mnemonic);

// `op_ a, b` where the original has the direction bit set (opcode r, r/m), `op a, b` where it is NASM's own form; nothing when the
// instruction is not a register-to-register one of register_pair_ops().
std::optional<std::string> register_pair_text(const Insn& insn);

// The operand text in NASM's spelling for what Capstone writes otherwise: `ptr` dropped, xmmword and xword as oword and tword,
// `st(1)` as `st1`, `ds:[bx]` as `[ds:bx]`.
std::string nasm_operands(std::string_view operands);

// A number at the very end of `text`: its first position and its value; `-0x10`, `0x7f`, `12` (the sign is part of it).
struct TrailingNumber {
    std::size_t start = 0;
    std::int64_t value = 0;
};
std::optional<TrailingNumber> trailing_number(std::string_view text);

// The bytes [lo, hi) of `bytes` as `0x12,0x34`.
std::string db_bytes(ByteView bytes, std::size_t lo, std::size_t hi);

// `line`, then spaces to column 40, then `; mnemonic operands`.
std::string with_original(std::string line, const Insn& insn);

}  // namespace cb::source::x86_flat

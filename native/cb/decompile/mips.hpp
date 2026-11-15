// cb/decompile/mips.hpp -- the MIPS R3000A's instructions (MIPS I, little-endian, as the PlayStation's CPU has them): a
// disassembler and an assembler.
//
//     decode(code, at, pc)         the instruction at code[at:] running at address pc, or nullopt
//     text(insn, labels)           "addiu $sp, $sp, -32" / "beq $a0, $zero, L_80010040" / "lw $t0, 4($sp)"
//     assemble(line, pc, labels)   the bytes (little-endian) of one instruction line
//
// Every instruction of MIPS I, the system control coprocessor's (MFC0, MTC0, RFE) and the Geometry Transformation Engine's
// (COP2: MFC2 CFC2 MTC2 CTC2 BC2F BC2T LWC2 SWC2, and the commands RTPS ... NCCT). A word with a field the instruction
// does not use set (a nonzero `rs` in SLL) is not an instruction, so data is not read as code and what is written as an
// instruction assembles back to the same word.
//
// Syntax is GNU as's, the assembler the PlayStation homebrew toolchains (PSn00bSDK, mipsel-none-elf-gcc) use, written the way
// its output is read with `.set noreorder`: every delay slot is a line of its own, nothing is moved or filled in:
//
//     lui   $a0, %hi(msg)           %hi(x) is (x + 0x8000) >> 16, so it pairs with an addiu or a load or store's offset
//     addiu $a0, $a0, %lo(msg)      %lo(x) is x & 0xFFFF
//     jal   sub_80010100            a jump or call to an address in the same 256 MB region
//     bne   $v0, $zero, L_80010040  a branch's offset is made from its target
//     lw    $t0, 4($sp)             registers: $zero $at $v0 $v1 $a0-$a3 $t0-$t9 $s0-$s7 $k0 $k1 $gp $sp $fp $ra, or $0-$31
//     mtc2  $t0, $0                 coprocessor registers are numbers
//     rtps  1, 0                    a GTE command, then its sf and lm bits (MVMVA: sf, mx, v, cv, lm; NCLIP: none)
//
// The pseudo-instructions are `nop`, `move rd, rs` (or rd, rs, $zero, as GNU as writes it), `b`, `beqz`, `bnez`, `li` and `la` (the assembler
// writes them; the disassembler writes `nop`, `move`, `b`, `beqz` and `bnez` only).
//
// Expressions are numbers (decimal, 0x hex), labels, `%hi()` and `%lo()`, `+ - * / & | ^ ~ << >>` and parentheses.
// Whether an instruction is the last of a flow is the decoder's: a jump, a branch and a call each have one delay slot, which
// is the instruction after them (`Insn::delay_slot`).
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/decompile/labels.hpp"
#include "cb/decompile/z80.hpp"

namespace cb::decompile::mips {

// How control leaves an instruction (the Z80 module's: None, Jump, Branch, Call, Return, Indirect, Halt).
using Flow = z80::Flow;

// What an instruction's operands are; each has a text shape and an encoding (mips.cpp's table).
enum class Form : std::uint8_t {
    None,        // nop-like: syscall (with its code), break, rfe
    RdRsRt,      // addu $d, $s, $t
    RdRtSa,      // sll $d, $t, sa
    RdRtRs,      // sllv $d, $t, $s
    Rs,          // jr $s, mthi $s, mtlo $s
    Rd,          // mfhi $d, mflo $d
    RdRs,        // jalr $d, $s
    RsRt,        // mult $s, $t
    RtRsImm,     // addiu $t, $s, imm
    RtImm,       // lui $t, imm
    RsRtBranch,  // beq $s, $t, target
    RsBranch,    // bgtz $s, target
    Jump,        // j target
    RtMem,       // lw $t, imm($s)
    RtCop,       // mfc0 $t, $d
    CopBranch,   // bc2f target
    Command,     // a GTE command with its fields
    Raw,         // cop2 0x...: a coprocessor 2 command word the GTE names no command for
};

// One decoded instruction.
struct Insn {
    std::uint32_t word = 0;
    std::uint32_t pc = 0;                  // the address it runs at
    std::string mnemonic;                  // "addu", "lw", "rtps", "cop2" ...
    Form form = Form::None;
    std::uint8_t rs = 0, rt = 0, rd = 0, shamt = 0;
    std::int32_t imm = 0;                  // RtRsImm and RtMem: the 16-bit field (sign-extended; zero-extended for ANDI ORI XORI and LUI);
                                           // None and Command: the code or the command's fields; Raw: the 25-bit command
    std::optional<std::uint32_t> target;   // where a branch, jump or call goes
    Flow flow = Flow::None;
    bool delay_slot = false;               // the instruction after it runs before the transfer takes effect

    std::array<std::uint8_t, 4> raw{};     // the word as the code holds it (little-endian)

    ByteView bytes() const { return {raw.data(), raw.size()}; }
};

// The instruction at code[at:], which runs at address `pc`, or nullopt when the bytes are not one (or are fewer than four).
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc);
// The same from the word.
std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc);

// The instruction as source. `labels` names branch and jump targets; an empty name counts as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});
// The instruction with its immediate written as `immediate` ("%hi(msg)", "%lo(msg)", "msg + 4"), for an instruction that
// has one (RtRsImm, RtImm, RtMem); any other is written as text() does.
std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate);

// The register's name ("$sp"), and the number of a name ("$sp", "$29", "sp": nullopt for neither).
std::string_view register_name(unsigned number);
std::optional<unsigned> register_number(std::string_view text);

// A numeric expression as the assembler reads it. ParseError for a malformed one, z80::UnknownLabel for a label not in
// `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The bytes of one instruction line (a pseudo-instruction may be two words). `labels` resolves names; `pc` is where the
// instruction will be. ParseError for text that is not an instruction or one whose operand is out of reach (a branch too far,
// an immediate that does not fit its field, a jump into another 256 MB region); z80::UnknownLabel for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {});

}  // namespace cb::decompile::mips

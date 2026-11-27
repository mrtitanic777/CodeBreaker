// decompile/mips.hpp -- the MIPS instructions of the consoles: the PlayStation's R3000A (MIPS I, little-endian) and the Nintendo 64's
// R4300i (MIPS III, big-endian), a disassembler and an assembler.
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
//
// The two processors are two `Variant`s, and every function takes one (the R3000A when it is left out, so the PlayStation's callers are
// as they were). The R4300i has no GTE (COP2 is not decoded) and no RFE, and adds what MIPS II and III have:
//
//     64-bit integers   dsll dsrl dsra (and the 32 and V forms) dmult dmultu ddiv ddivu dadd daddu dsub dsubu daddi daddiu
//                       ld sd ldl ldr sdl sdr lld scd lwu ll sc, and sync, cache
//     branch likely     beql bnel blezl bgtzl bltzl bgezl bltzall bgezall bc1fl bc1tl: the delay slot runs only when the branch is
//                       taken (`Insn::likely`); the flow is read as for any branch
//     traps             tge tgeu tlt tltu teq tne and tgei tgeiu tlti tltiu teqi tnei
//     system control    mfc0 dmfc0 mtc0 dmtc0 and tlbr tlbwi tlbwr tlbp eret
//     the FPU (COP1)    mfc1 dmfc1 cfc1 mtc1 dmtc1 ctc1, lwc1 ldc1 swc1 sdc1, bc1f bc1t, and add sub mul div sqrt abs mov neg
//                       round.l trunc.l ceil.l floor.l round.w trunc.w ceil.w floor.w cvt.s cvt.d cvt.w cvt.l and the sixteen compares
//                       c.f c.un c.eq c.ueq c.olt c.ult c.ole c.ule c.sf c.ngle c.seq c.ngl c.lt c.nge c.le c.ngt, each with its format
//                       (`add.s`, `cvt.d.w`, `c.lt.d`); FPU registers are written `$f0` ... `$f31`, the control registers `$31`
//
// An R4300i program's words are big-endian: `decode(code, at, pc, Variant::R4300)` reads them so, and `assemble` gives them so.
#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/bytes.hpp"
#include "core/error.hpp"
#include "decompile/labels.hpp"
#include "decompile/z80.hpp"

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
    // the R4300i's additions
    RsImm,       // tgei $s, imm
    FtMem,       // lwc1 $f0, imm($s)
    OpMem,       // cache op, imm($s)
    RtFs,        // mfc1 $t, $f2
    FdFsFt,      // add.s $f0, $f1, $f2
    FdFs,        // mov.s $f0, $f1
    FsFt,        // c.eq.s $f1, $f2
    RsRtCode,    // teq $s, $t, code (the code is optional, 0 to 1023)
    // the R5900's and the Allegrex's additions
    RdRt,        // seb $d, $t;  pabsw $d, $t
    FdFt,        // sqrt.s $f0, $f1 (the R5900's reads FT)
    Special,     // an instruction with its own operand layout (the VU0's macro mode, the Allegrex's VFPU, ext and ins): see mips_ext.hpp
};

// Which processor's instructions.
enum class Variant : std::uint8_t {
    R3000A,      // the PlayStation's: MIPS I, little-endian, with the GTE
    R4300,       // the Nintendo 64's: MIPS III, big-endian, with the FPU
    R5900,       // the PlayStation 2's Emotion Engine: MIPS III + MIPS IV's MOVZ/MOVN/PREF, little-endian, 128-bit registers (LQ, SQ and the multimedia
                 // instructions), a single-precision FPU, VU0 in COP2
    Allegrex,    // the PSP's: MIPS32 (little-endian) with CLZ CLO MADD MSUB MAX MIN, EXT INS SEB SEH WSBH BITREV ROTR, a single-precision FPU and the VFPU
    Rsp,         // the Nintendo 64's RSP: MIPS I's integer instructions without multiply, divide, LWL/LWR/SWL/SWR and SYSCALL (big-endian), its
                 // vector unit in COP2 and the vector loads and stores (mips_rsp.hpp)
};

// Is the processor's code held big-endian (the Nintendo 64's) or little-endian (all the others)?
constexpr bool big_endian(Variant v) { return v == Variant::R4300 || v == Variant::Rsp; }

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
    bool likely = false;                   // a branch likely (R4300): the delay slot runs only when the branch is taken
    bool rsp = false;                      // one of the RSP's own (mips_rsp): its vector unit, loads and stores

    std::array<std::uint8_t, 4> raw{};     // the word as the code holds it (big-endian for the R4300, little-endian for the others)

    ByteView bytes() const { return {raw.data(), raw.size()}; }
};

// The instruction at code[at:], which runs at address `pc`, or nullopt when the bytes are not one (or are fewer than four).
std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Variant variant = Variant::R3000A);
// The same from the word.
std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc, Variant variant = Variant::R3000A);

// The instruction as source. `labels` names branch and jump targets; an empty name counts as no label.
std::string text(const Insn& insn, const AddressLabels& labels = {});
// The instruction with its immediate written as `immediate` ("%hi(msg)", "%lo(msg)", "msg + 4"), for an instruction that
// has one (RtRsImm, RtImm, RtMem); any other is written as text() does.
std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate);

// The register's name ("$sp"), and the number of a name ("$sp", "$29", "sp": nullopt for neither).
std::string_view register_name(unsigned number);
std::optional<unsigned> register_number(std::string_view text);
// The number of an FPU register written "$f12" (nullopt for anything else).
std::optional<unsigned> fpu_register_number(std::string_view text);

// A numeric expression as the assembler reads it. ParseError for a malformed one, z80::UnknownLabel for a label not in
// `labels`.
std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc = 0);

// The bytes of one instruction line (a pseudo-instruction may be two words). `labels` resolves names; `pc` is where the
// instruction will be. ParseError for text that is not an instruction or one whose operand is out of reach (a branch too far,
// an immediate that does not fit its field, a jump into another 256 MB region); z80::UnknownLabel for a label that is not there.
Bytes assemble(std::string_view line, std::uint32_t pc = 0, const NameLabels& labels = {}, Variant variant = Variant::R3000A);

}  // namespace cb::decompile::mips

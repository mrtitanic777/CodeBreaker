// cb/decompile/mips.cpp -- the MIPS R3000A's and R4300i's instructions: a table of every encoding, read both ways (mips.hpp).

#include "cb/decompile/mips.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <deque>
#include <format>
#include <unordered_map>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/decompile/mips_ext.hpp"

namespace cb::decompile::mips {

namespace {

// ---- the table --------------------------------------------------------------------------------------------------

// Which processors have an instruction: a set of bits, one per Variant.
struct Isa {
    static constexpr std::uint8_t R3000A = 1, R4300 = 2, R5900 = 4, Allegrex = 8, Both = 15;
    static constexpr std::uint8_t Mips3 = R4300 | R5900;                 // MIPS III's 64-bit integer instructions
    static constexpr std::uint8_t Mips32 = R4300 | R5900 | Allegrex;     // what MIPS II added: branch likely, traps, SYNC, CACHE, the FPU's moves and branches
    static constexpr std::uint8_t Ee = R5900;
    static constexpr std::uint8_t Psp = Allegrex;
};

struct Def {
    std::string_view name;
    Form form;
    std::uint32_t match;   // the word's fixed bits ...
    std::uint32_t mask;    // ... and which bits those are (everything else is an operand)
    Flow flow = Flow::None;
    bool delay = false;
    std::uint8_t isa = Isa::Both;
    bool likely = false;   // a branch likely
};

constexpr bool has(std::uint8_t isa, Variant v) { return (isa & (1u << static_cast<unsigned>(v))) != 0; }

constexpr std::uint32_t op(std::uint32_t o) { return o << 26; }
constexpr std::uint32_t kSpecialMask = 0xFC00003F;   // opcode and function
constexpr std::uint32_t kNoSa = 0xFC0007FF;          // ... and shamt zero
constexpr std::uint32_t kRegimmMask = 0xFC1F0000;

constexpr auto kDefs = std::to_array<Def>({
    {"sll", Form::RdRtSa, 0x00, 0xFFE0003F},
    {"srl", Form::RdRtSa, 0x02, 0xFFE0003F},
    {"sra", Form::RdRtSa, 0x03, 0xFFE0003F},
    {"sllv", Form::RdRtRs, 0x04, kNoSa},
    {"srlv", Form::RdRtRs, 0x06, kNoSa},
    {"srav", Form::RdRtRs, 0x07, kNoSa},
    {"jr", Form::Rs, 0x08, 0xFC1FFFFF, Flow::Indirect, true},
    {"jalr", Form::RdRs, 0x09, 0xFC1F07FF, Flow::Call, true},
    {"syscall", Form::None, 0x0C, kSpecialMask},
    {"break", Form::None, 0x0D, kSpecialMask},
    {"mfhi", Form::Rd, 0x10, 0xFFFF07FF},
    {"mthi", Form::Rs, 0x11, 0xFC1FFFFF},
    {"mflo", Form::Rd, 0x12, 0xFFFF07FF},
    {"mtlo", Form::Rs, 0x13, 0xFC1FFFFF},
    {"mult", Form::RsRt, 0x18, 0xFC00FFFF},
    {"multu", Form::RsRt, 0x19, 0xFC00FFFF},
    {"div", Form::RsRt, 0x1A, 0xFC00FFFF},
    {"divu", Form::RsRt, 0x1B, 0xFC00FFFF},
    {"add", Form::RdRsRt, 0x20, kNoSa},
    {"addu", Form::RdRsRt, 0x21, kNoSa},
    {"sub", Form::RdRsRt, 0x22, kNoSa},
    {"subu", Form::RdRsRt, 0x23, kNoSa},
    {"and", Form::RdRsRt, 0x24, kNoSa},
    {"or", Form::RdRsRt, 0x25, kNoSa},
    {"xor", Form::RdRsRt, 0x26, kNoSa},
    {"nor", Form::RdRsRt, 0x27, kNoSa},
    {"slt", Form::RdRsRt, 0x2A, kNoSa},
    {"sltu", Form::RdRsRt, 0x2B, kNoSa},
    {"bltz", Form::RsBranch, op(1) | (0u << 16), kRegimmMask, Flow::Branch, true},
    {"bgez", Form::RsBranch, op(1) | (1u << 16), kRegimmMask, Flow::Branch, true},
    {"bltzal", Form::RsBranch, op(1) | (16u << 16), kRegimmMask, Flow::Call, true},
    {"bgezal", Form::RsBranch, op(1) | (17u << 16), kRegimmMask, Flow::Call, true},
    {"j", Form::Jump, op(2), 0xFC000000, Flow::Jump, true},
    {"jal", Form::Jump, op(3), 0xFC000000, Flow::Call, true},
    {"beq", Form::RsRtBranch, op(4), 0xFC000000, Flow::Branch, true},
    {"bne", Form::RsRtBranch, op(5), 0xFC000000, Flow::Branch, true},
    {"blez", Form::RsBranch, op(6), kRegimmMask, Flow::Branch, true},
    {"bgtz", Form::RsBranch, op(7), kRegimmMask, Flow::Branch, true},
    {"addi", Form::RtRsImm, op(8), 0xFC000000},
    {"addiu", Form::RtRsImm, op(9), 0xFC000000},
    {"slti", Form::RtRsImm, op(10), 0xFC000000},
    {"sltiu", Form::RtRsImm, op(11), 0xFC000000},
    {"andi", Form::RtRsImm, op(12), 0xFC000000},
    {"ori", Form::RtRsImm, op(13), 0xFC000000},
    {"xori", Form::RtRsImm, op(14), 0xFC000000},
    {"lui", Form::RtImm, op(15), 0xFFE00000},
    {"mfc0", Form::RtCop, op(0x10) | (0u << 21), 0xFFE007FF},
    {"mtc0", Form::RtCop, op(0x10) | (4u << 21), 0xFFE007FF},
    {"rfe", Form::None, 0x42000010, 0xFFFFFFFF, Flow::None, false, Isa::R3000A},
    {"mfc2", Form::RtCop, op(0x12) | (0u << 21), 0xFFE007FF, Flow::None, false, Isa::R3000A},
    {"cfc2", Form::RtCop, op(0x12) | (2u << 21), 0xFFE007FF, Flow::None, false, Isa::R3000A},
    {"mtc2", Form::RtCop, op(0x12) | (4u << 21), 0xFFE007FF, Flow::None, false, Isa::R3000A},
    {"ctc2", Form::RtCop, op(0x12) | (6u << 21), 0xFFE007FF, Flow::None, false, Isa::R3000A},
    {"bc2f", Form::CopBranch, op(0x12) | (8u << 21) | (0u << 16), 0xFFFF0000, Flow::Branch, true, Isa::R3000A},
    {"bc2t", Form::CopBranch, op(0x12) | (8u << 21) | (1u << 16), 0xFFFF0000, Flow::Branch, true, Isa::R3000A},
    {"lb", Form::RtMem, op(0x20), 0xFC000000},
    {"lh", Form::RtMem, op(0x21), 0xFC000000},
    {"lwl", Form::RtMem, op(0x22), 0xFC000000},
    {"lw", Form::RtMem, op(0x23), 0xFC000000},
    {"lbu", Form::RtMem, op(0x24), 0xFC000000},
    {"lhu", Form::RtMem, op(0x25), 0xFC000000},
    {"lwr", Form::RtMem, op(0x26), 0xFC000000},
    {"sb", Form::RtMem, op(0x28), 0xFC000000},
    {"sh", Form::RtMem, op(0x29), 0xFC000000},
    {"swl", Form::RtMem, op(0x2A), 0xFC000000},
    {"sw", Form::RtMem, op(0x2B), 0xFC000000},
    {"swr", Form::RtMem, op(0x2E), 0xFC000000},
    {"lwc2", Form::RtMem, op(0x32), 0xFC000000, Flow::None, false, Isa::R3000A},
    {"swc2", Form::RtMem, op(0x3A), 0xFC000000, Flow::None, false, Isa::R3000A},
});

// What the R4300i has beyond MIPS I (and without the GTE): MIPS II's branch likely and traps, MIPS III's 64-bit integers, its system
// control coprocessor's TLB and exception return, and the FPU's moves, loads, stores and branches; the FPU's arithmetic is made in `fpu_defs`.
constexpr std::uint32_t kTrapMask = 0xFC00003F;      // opcode and function (the code field, bits 15-6, is the program's own: the compilers put 7 in the divide-by-zero check)
constexpr auto kDefs4300 = std::to_array<Def>({
    {"sync", Form::None, 0x0F, 0xFFFFFFFF, Flow::None, false, Isa::Mips32},
    {"dsllv", Form::RdRtRs, 0x14, kNoSa, Flow::None, false, Isa::Mips3},
    {"dsrlv", Form::RdRtRs, 0x16, kNoSa, Flow::None, false, Isa::Mips3},
    {"dsrav", Form::RdRtRs, 0x17, kNoSa, Flow::None, false, Isa::Mips3},
    {"dmult", Form::RsRt, 0x1C, 0xFC00FFFF, Flow::None, false, Isa::R4300},
    {"dmultu", Form::RsRt, 0x1D, 0xFC00FFFF, Flow::None, false, Isa::R4300},
    {"ddiv", Form::RsRt, 0x1E, 0xFC00FFFF, Flow::None, false, Isa::R4300},
    {"ddivu", Form::RsRt, 0x1F, 0xFC00FFFF, Flow::None, false, Isa::R4300},
    {"dadd", Form::RdRsRt, 0x2C, kNoSa, Flow::None, false, Isa::Mips3},
    {"daddu", Form::RdRsRt, 0x2D, kNoSa, Flow::None, false, Isa::Mips3},
    {"dsub", Form::RdRsRt, 0x2E, kNoSa, Flow::None, false, Isa::Mips3},
    {"dsubu", Form::RdRsRt, 0x2F, kNoSa, Flow::None, false, Isa::Mips3},
    {"tge", Form::RsRtCode, 0x30, kTrapMask, Flow::None, false, Isa::Mips32},
    {"tgeu", Form::RsRtCode, 0x31, kTrapMask, Flow::None, false, Isa::Mips32},
    {"tlt", Form::RsRtCode, 0x32, kTrapMask, Flow::None, false, Isa::Mips32},
    {"tltu", Form::RsRtCode, 0x33, kTrapMask, Flow::None, false, Isa::Mips32},
    {"teq", Form::RsRtCode, 0x34, kTrapMask, Flow::None, false, Isa::Mips32},
    {"tne", Form::RsRtCode, 0x36, kTrapMask, Flow::None, false, Isa::Mips32},
    {"dsll", Form::RdRtSa, 0x38, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"dsrl", Form::RdRtSa, 0x3A, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"dsra", Form::RdRtSa, 0x3B, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"dsll32", Form::RdRtSa, 0x3C, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"dsrl32", Form::RdRtSa, 0x3E, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"dsra32", Form::RdRtSa, 0x3F, 0xFFE0003F, Flow::None, false, Isa::Mips3},
    {"bltzl", Form::RsBranch, op(1) | (2u << 16), kRegimmMask, Flow::Branch, true, Isa::Mips32, true},
    {"bgezl", Form::RsBranch, op(1) | (3u << 16), kRegimmMask, Flow::Branch, true, Isa::Mips32, true},
    {"tgei", Form::RsImm, op(1) | (8u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"tgeiu", Form::RsImm, op(1) | (9u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"tlti", Form::RsImm, op(1) | (10u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"tltiu", Form::RsImm, op(1) | (11u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"teqi", Form::RsImm, op(1) | (12u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"tnei", Form::RsImm, op(1) | (14u << 16), kRegimmMask, Flow::None, false, Isa::Mips32},
    {"bltzall", Form::RsBranch, op(1) | (18u << 16), kRegimmMask, Flow::Call, true, Isa::Mips32, true},
    {"bgezall", Form::RsBranch, op(1) | (19u << 16), kRegimmMask, Flow::Call, true, Isa::Mips32, true},
    {"beql", Form::RsRtBranch, op(0x14), 0xFC000000, Flow::Branch, true, Isa::Mips32, true},
    {"bnel", Form::RsRtBranch, op(0x15), 0xFC000000, Flow::Branch, true, Isa::Mips32, true},
    {"blezl", Form::RsBranch, op(0x16), kRegimmMask, Flow::Branch, true, Isa::Mips32, true},
    {"bgtzl", Form::RsBranch, op(0x17), kRegimmMask, Flow::Branch, true, Isa::Mips32, true},
    {"daddi", Form::RtRsImm, op(0x18), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"daddiu", Form::RtRsImm, op(0x19), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"ldl", Form::RtMem, op(0x1A), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"ldr", Form::RtMem, op(0x1B), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"dmfc0", Form::RtCop, op(0x10) | (1u << 21), 0xFFE007FF, Flow::None, false, Isa::R4300},
    {"dmtc0", Form::RtCop, op(0x10) | (5u << 21), 0xFFE007FF, Flow::None, false, Isa::R4300},
    {"tlbr", Form::None, 0x42000001, 0xFFFFFFFF, Flow::None, false, Isa::Mips3},
    {"tlbwi", Form::None, 0x42000002, 0xFFFFFFFF, Flow::None, false, Isa::Mips3},
    {"tlbwr", Form::None, 0x42000006, 0xFFFFFFFF, Flow::None, false, Isa::Mips3},
    {"tlbp", Form::None, 0x42000008, 0xFFFFFFFF, Flow::None, false, Isa::Mips3},
    {"eret", Form::None, 0x42000018, 0xFFFFFFFF, Flow::Indirect, false, Isa::Mips32},
    {"mfc1", Form::RtFs, op(0x11) | (0u << 21), 0xFFE007FF, Flow::None, false, Isa::Mips32},
    {"dmfc1", Form::RtFs, op(0x11) | (1u << 21), 0xFFE007FF, Flow::None, false, Isa::R4300},
    {"cfc1", Form::RtCop, op(0x11) | (2u << 21), 0xFFE007FF, Flow::None, false, Isa::Mips32},
    {"mtc1", Form::RtFs, op(0x11) | (4u << 21), 0xFFE007FF, Flow::None, false, Isa::Mips32},
    {"dmtc1", Form::RtFs, op(0x11) | (5u << 21), 0xFFE007FF, Flow::None, false, Isa::R4300},
    {"ctc1", Form::RtCop, op(0x11) | (6u << 21), 0xFFE007FF, Flow::None, false, Isa::Mips32},
    {"bc1f", Form::CopBranch, op(0x11) | (8u << 21) | (0u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Mips32},
    {"bc1t", Form::CopBranch, op(0x11) | (8u << 21) | (1u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Mips32},
    {"bc1fl", Form::CopBranch, op(0x11) | (8u << 21) | (2u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Mips32, true},
    {"bc1tl", Form::CopBranch, op(0x11) | (8u << 21) | (3u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Mips32, true},
    {"lwu", Form::RtMem, op(0x27), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"sdl", Form::RtMem, op(0x2C), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"sdr", Form::RtMem, op(0x2D), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"cache", Form::OpMem, op(0x2F), 0xFC000000, Flow::None, false, Isa::Mips32},
    {"ll", Form::RtMem, op(0x30), 0xFC000000, Flow::None, false, Isa::R4300 | Isa::Allegrex},
    {"lwc1", Form::FtMem, op(0x31), 0xFC000000, Flow::None, false, Isa::Mips32},
    {"lld", Form::RtMem, op(0x34), 0xFC000000, Flow::None, false, Isa::R4300},
    {"ldc1", Form::FtMem, op(0x35), 0xFC000000, Flow::None, false, Isa::R4300},
    {"ld", Form::RtMem, op(0x37), 0xFC000000, Flow::None, false, Isa::Mips3},
    {"sc", Form::RtMem, op(0x38), 0xFC000000, Flow::None, false, Isa::R4300 | Isa::Allegrex},
    {"swc1", Form::FtMem, op(0x39), 0xFC000000, Flow::None, false, Isa::Mips32},
    {"scd", Form::RtMem, op(0x3C), 0xFC000000, Flow::None, false, Isa::R4300},
    {"sdc1", Form::FtMem, op(0x3D), 0xFC000000, Flow::None, false, Isa::R4300},
    {"sd", Form::RtMem, op(0x3F), 0xFC000000, Flow::None, false, Isa::Mips3},
});

// What the PlayStation 2's R5900 and the PSP's Allegrex have beyond the above, that the table says in one entry each: MIPS IV's conditional
// moves, the Emotion Engine's shift amount register, LQ and SQ, the three-operand and second-pipeline multiplies, the multimedia
// instructions' non-uniform ones (the uniform ones are made from `kMmiOps`), COP0's EI and DI, VU0's moves and branches (its macro-mode
// instructions are in mips_ext); the Allegrex's counting, rotating and sign-extending instructions, MADD MSUB MAX MIN, HALT and the
// internal-cache moves.
constexpr std::uint32_t kMmiRsRt = 0xFC00FFFF, kMmiRdRsRt = 0xFC0007FF, kMmiRdRt = 0xFFE007FF, kMmiRd = 0xFFFF07FF, kMmiRs = 0xFC1FFFFF;
constexpr auto kDefsExtra = std::to_array<Def>({
    {"movz", Form::RdRsRt, 0x0A, kNoSa, Flow::None, false, Isa::Ee | Isa::Psp},
    {"movn", Form::RdRsRt, 0x0B, kNoSa, Flow::None, false, Isa::Ee | Isa::Psp},
    {"mfsa", Form::Rd, 0x28, 0xFFFF07FF, Flow::None, false, Isa::Ee},
    {"mtsa", Form::Rs, 0x29, 0xFC1FFFFF, Flow::None, false, Isa::Ee},
    {"mtsab", Form::RsImm, op(1) | (0x18u << 16), kRegimmMask, Flow::None, false, Isa::Ee},
    {"mtsah", Form::RsImm, op(1) | (0x19u << 16), kRegimmMask, Flow::None, false, Isa::Ee},
    {"lq", Form::RtMem, op(0x1E), 0xFC000000, Flow::None, false, Isa::Ee},
    {"sq", Form::RtMem, op(0x1F), 0xFC000000, Flow::None, false, Isa::Ee},
    {"pref", Form::OpMem, op(0x33), 0xFC000000, Flow::None, false, Isa::Ee},
    {"ei", Form::None, 0x42000038, 0xFFFFFFFF, Flow::None, false, Isa::Ee},
    {"di", Form::None, 0x42000039, 0xFFFFFFFF, Flow::None, false, Isa::Ee},
    {"bc0f", Form::CopBranch, op(0x10) | (8u << 21) | (0u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee},
    {"bc0t", Form::CopBranch, op(0x10) | (8u << 21) | (1u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee},
    {"bc0fl", Form::CopBranch, op(0x10) | (8u << 21) | (2u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee, true},
    {"bc0tl", Form::CopBranch, op(0x10) | (8u << 21) | (3u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee, true},
    {"qmfc2", Form::RtCop, op(0x12) | (1u << 21), 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"qmfc2.i", Form::RtCop, op(0x12) | (1u << 21) | 1, 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"cfc2", Form::RtCop, op(0x12) | (2u << 21), 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"cfc2.i", Form::RtCop, op(0x12) | (2u << 21) | 1, 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"qmtc2", Form::RtCop, op(0x12) | (5u << 21), 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"qmtc2.i", Form::RtCop, op(0x12) | (5u << 21) | 1, 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"ctc2", Form::RtCop, op(0x12) | (6u << 21), 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"ctc2.i", Form::RtCop, op(0x12) | (6u << 21) | 1, 0xFFE007FF, Flow::None, false, Isa::Ee},
    {"bc2f", Form::CopBranch, op(0x12) | (8u << 21) | (0u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee},
    {"bc2t", Form::CopBranch, op(0x12) | (8u << 21) | (1u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee},
    {"bc2fl", Form::CopBranch, op(0x12) | (8u << 21) | (2u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee, true},
    {"bc2tl", Form::CopBranch, op(0x12) | (8u << 21) | (3u << 16), 0xFFFF0000, Flow::Branch, true, Isa::Ee, true},
    // the multiplies: two operands (the result in HI and LO) or three (the low word also in RD), and the second pipeline's
    {"mult", Form::RdRsRt, 0x18, kNoSa, Flow::None, false, Isa::Ee},
    {"multu", Form::RdRsRt, 0x19, kNoSa, Flow::None, false, Isa::Ee},
    {"madd", Form::RsRt, op(0x1C) | 0x00, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"madd", Form::RdRsRt, op(0x1C) | 0x00, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"maddu", Form::RsRt, op(0x1C) | 0x01, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"maddu", Form::RdRsRt, op(0x1C) | 0x01, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"mult1", Form::RsRt, op(0x1C) | 0x18, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"mult1", Form::RdRsRt, op(0x1C) | 0x18, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"multu1", Form::RsRt, op(0x1C) | 0x19, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"multu1", Form::RdRsRt, op(0x1C) | 0x19, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"div1", Form::RsRt, op(0x1C) | 0x1A, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"divu1", Form::RsRt, op(0x1C) | 0x1B, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"madd1", Form::RsRt, op(0x1C) | 0x20, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"madd1", Form::RdRsRt, op(0x1C) | 0x20, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"maddu1", Form::RsRt, op(0x1C) | 0x21, kMmiRsRt, Flow::None, false, Isa::Ee},
    {"maddu1", Form::RdRsRt, op(0x1C) | 0x21, kMmiRdRsRt, Flow::None, false, Isa::Ee},
    {"plzcw", Form::RdRs, op(0x1C) | 0x04, 0xFC1F07FF, Flow::None, false, Isa::Ee},
    {"mfhi1", Form::Rd, op(0x1C) | 0x10, kMmiRd, Flow::None, false, Isa::Ee},
    {"mthi1", Form::Rs, op(0x1C) | 0x11, kMmiRs, Flow::None, false, Isa::Ee},
    {"mflo1", Form::Rd, op(0x1C) | 0x12, kMmiRd, Flow::None, false, Isa::Ee},
    {"mtlo1", Form::Rs, op(0x1C) | 0x13, kMmiRs, Flow::None, false, Isa::Ee},
    {"pmfhl.lw", Form::Rd, op(0x1C) | 0x30 | (0u << 6), kMmiRd, Flow::None, false, Isa::Ee},
    {"pmfhl.uw", Form::Rd, op(0x1C) | 0x30 | (1u << 6), kMmiRd, Flow::None, false, Isa::Ee},
    {"pmfhl.slw", Form::Rd, op(0x1C) | 0x30 | (2u << 6), kMmiRd, Flow::None, false, Isa::Ee},
    {"pmfhl.lh", Form::Rd, op(0x1C) | 0x30 | (3u << 6), kMmiRd, Flow::None, false, Isa::Ee},
    {"pmfhl.sh", Form::Rd, op(0x1C) | 0x30 | (4u << 6), kMmiRd, Flow::None, false, Isa::Ee},
    {"pmthl.lw", Form::Rs, op(0x1C) | 0x31, kMmiRs, Flow::None, false, Isa::Ee},
    {"psllh", Form::RdRtSa, op(0x1C) | 0x34, 0xFFE0003F, Flow::None, false, Isa::Ee},
    {"psrlh", Form::RdRtSa, op(0x1C) | 0x36, 0xFFE0003F, Flow::None, false, Isa::Ee},
    {"psrah", Form::RdRtSa, op(0x1C) | 0x37, 0xFFE0003F, Flow::None, false, Isa::Ee},
    {"psllw", Form::RdRtSa, op(0x1C) | 0x3C, 0xFFE0003F, Flow::None, false, Isa::Ee},
    {"psrlw", Form::RdRtSa, op(0x1C) | 0x3E, 0xFFE0003F, Flow::None, false, Isa::Ee},
    {"psraw", Form::RdRtSa, op(0x1C) | 0x3F, 0xFFE0003F, Flow::None, false, Isa::Ee},
    // the Allegrex
    {"clz", Form::RdRs, 0x16, 0xFC1F07FF, Flow::None, false, Isa::Psp},
    {"clo", Form::RdRs, 0x17, 0xFC1F07FF, Flow::None, false, Isa::Psp},
    {"madd", Form::RsRt, 0x1C, 0xFC00FFFF, Flow::None, false, Isa::Psp},
    {"maddu", Form::RsRt, 0x1D, 0xFC00FFFF, Flow::None, false, Isa::Psp},
    {"max", Form::RdRsRt, 0x2C, kNoSa, Flow::None, false, Isa::Psp},
    {"min", Form::RdRsRt, 0x2D, kNoSa, Flow::None, false, Isa::Psp},
    {"msub", Form::RsRt, 0x2E, 0xFC00FFFF, Flow::None, false, Isa::Psp},
    {"msubu", Form::RsRt, 0x2F, 0xFC00FFFF, Flow::None, false, Isa::Psp},
    {"rotr", Form::RdRtSa, 0x00200002, 0xFFE0003F, Flow::None, false, Isa::Psp},
    {"rotrv", Form::RdRtRs, 0x46, kNoSa, Flow::None, false, Isa::Psp},
    {"wsbh", Form::RdRt, 0x7C0000A0, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"wsbw", Form::RdRt, 0x7C0000E0, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"seb", Form::RdRt, 0x7C000420, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"bitrev", Form::RdRt, 0x7C000520, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"seh", Form::RdRt, 0x7C000620, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"halt", Form::None, 0x70000000, 0xFFFFFFFF, Flow::Halt, false, Isa::Psp},
    {"mfic", Form::RtCop, 0x70000024, 0xFFE007FF, Flow::None, false, Isa::Psp},
    {"mtic", Form::RtCop, 0x70000026, 0xFFE007FF, Flow::None, false, Isa::Psp},
});

// The multimedia instructions that follow one pattern: the opcode (0x1C), the group's function (MMI0 0x08, MMI1 0x28, MMI2 0x09, MMI3 0x29) and
// the sub-function in the shift amount field.
struct MmiOp {
    std::string_view name;
    std::uint32_t function;
    std::uint32_t sub;
    Form form;
};
constexpr std::uint32_t kMmi0 = 0x08, kMmi1 = 0x28, kMmi2 = 0x09, kMmi3 = 0x29;
constexpr auto kMmiOps = std::to_array<MmiOp>({
    {"paddw", kMmi0, 0x00, Form::RdRsRt},  {"psubw", kMmi0, 0x01, Form::RdRsRt},  {"pcgtw", kMmi0, 0x02, Form::RdRsRt},  {"pmaxw", kMmi0, 0x03, Form::RdRsRt},
    {"paddh", kMmi0, 0x04, Form::RdRsRt},  {"psubh", kMmi0, 0x05, Form::RdRsRt},  {"pcgth", kMmi0, 0x06, Form::RdRsRt},  {"pmaxh", kMmi0, 0x07, Form::RdRsRt},
    {"paddb", kMmi0, 0x08, Form::RdRsRt},  {"psubb", kMmi0, 0x09, Form::RdRsRt},  {"pcgtb", kMmi0, 0x0A, Form::RdRsRt},  {"paddsw", kMmi0, 0x10, Form::RdRsRt},
    {"psubsw", kMmi0, 0x11, Form::RdRsRt}, {"pextlw", kMmi0, 0x12, Form::RdRsRt}, {"ppacw", kMmi0, 0x13, Form::RdRsRt},  {"paddsh", kMmi0, 0x14, Form::RdRsRt},
    {"psubsh", kMmi0, 0x15, Form::RdRsRt}, {"pextlh", kMmi0, 0x16, Form::RdRsRt}, {"ppach", kMmi0, 0x17, Form::RdRsRt},  {"paddsb", kMmi0, 0x18, Form::RdRsRt},
    {"psubsb", kMmi0, 0x19, Form::RdRsRt}, {"pextlb", kMmi0, 0x1A, Form::RdRsRt}, {"ppacb", kMmi0, 0x1B, Form::RdRsRt},  {"pext5", kMmi0, 0x1E, Form::RdRt},
    {"ppac5", kMmi0, 0x1F, Form::RdRt},
    {"pabsw", kMmi1, 0x01, Form::RdRt},    {"pceqw", kMmi1, 0x02, Form::RdRsRt},  {"pminw", kMmi1, 0x03, Form::RdRsRt},  {"padsbh", kMmi1, 0x04, Form::RdRsRt},
    {"pabsh", kMmi1, 0x05, Form::RdRt},    {"pceqh", kMmi1, 0x06, Form::RdRsRt},  {"pminh", kMmi1, 0x07, Form::RdRsRt},  {"pceqb", kMmi1, 0x0A, Form::RdRsRt},
    {"padduw", kMmi1, 0x10, Form::RdRsRt}, {"psubuw", kMmi1, 0x11, Form::RdRsRt}, {"pextuw", kMmi1, 0x12, Form::RdRsRt}, {"padduh", kMmi1, 0x14, Form::RdRsRt},
    {"psubuh", kMmi1, 0x15, Form::RdRsRt}, {"pextuh", kMmi1, 0x16, Form::RdRsRt}, {"paddub", kMmi1, 0x18, Form::RdRsRt}, {"psubub", kMmi1, 0x19, Form::RdRsRt},
    {"pextub", kMmi1, 0x1A, Form::RdRsRt}, {"qfsrv", kMmi1, 0x1B, Form::RdRsRt},
    {"pmaddw", kMmi2, 0x00, Form::RdRsRt}, {"psllvw", kMmi2, 0x02, Form::RdRtRs}, {"psrlvw", kMmi2, 0x03, Form::RdRtRs}, {"pmsubw", kMmi2, 0x04, Form::RdRsRt},
    {"pmfhi", kMmi2, 0x08, Form::Rd},      {"pmflo", kMmi2, 0x09, Form::Rd},      {"pinth", kMmi2, 0x0A, Form::RdRsRt},  {"pmultw", kMmi2, 0x0C, Form::RdRsRt},
    {"pdivw", kMmi2, 0x0D, Form::RsRt},    {"pcpyld", kMmi2, 0x0E, Form::RdRsRt}, {"pmaddh", kMmi2, 0x10, Form::RdRsRt}, {"phmadh", kMmi2, 0x11, Form::RdRsRt},
    {"pand", kMmi2, 0x12, Form::RdRsRt},   {"pxor", kMmi2, 0x13, Form::RdRsRt},   {"pmsubh", kMmi2, 0x14, Form::RdRsRt}, {"phmsbh", kMmi2, 0x15, Form::RdRsRt},
    {"pexeh", kMmi2, 0x1A, Form::RdRt},    {"prevh", kMmi2, 0x1B, Form::RdRt},    {"pmulth", kMmi2, 0x1C, Form::RdRsRt}, {"pdivbw", kMmi2, 0x1D, Form::RsRt},
    {"pexew", kMmi2, 0x1E, Form::RdRt},    {"prot3w", kMmi2, 0x1F, Form::RdRt},
    {"pmadduw", kMmi3, 0x00, Form::RdRsRt}, {"psravw", kMmi3, 0x03, Form::RdRtRs}, {"pmthi", kMmi3, 0x08, Form::Rs},     {"pmtlo", kMmi3, 0x09, Form::Rs},
    {"pinteh", kMmi3, 0x0A, Form::RdRsRt}, {"pmultuw", kMmi3, 0x0C, Form::RdRsRt}, {"pdivuw", kMmi3, 0x0D, Form::RsRt}, {"pcpyud", kMmi3, 0x0E, Form::RdRsRt},
    {"por", kMmi3, 0x12, Form::RdRsRt},    {"pnor", kMmi3, 0x13, Form::RdRsRt},   {"pexch", kMmi3, 0x1A, Form::RdRt},    {"pcpyh", kMmi3, 0x1B, Form::RdRt},
    {"pexcw", kMmi3, 0x1E, Form::RdRt},
});

constexpr std::uint32_t mmi_mask(Form form) {
    switch (form) {
        case Form::RdRt: return kMmiRdRt;
        case Form::Rd: return kMmiRd;
        case Form::Rs: return kMmiRs;
        case Form::RsRt: return kMmiRsRt;
        default: return kMmiRdRsRt;
    }
}

// The FPU's arithmetic, conversions and compares, one entry per format: `add.s`, `cvt.d.w`, `c.lt.d` ... (the fmt field is bits 25-21).
struct FpuOp {
    std::string_view name;
    std::uint32_t function;
    Form form;
    unsigned formats;   // which formats take it: bit 0 S (16), 1 D (17), 2 W (20), 3 L (21)
};
constexpr unsigned kS = 1, kD = 2, kW = 4, kL = 8;
constexpr auto kFpuOps = std::to_array<FpuOp>({
    {"add", 0, Form::FdFsFt, kS | kD},         {"sub", 1, Form::FdFsFt, kS | kD},         {"mul", 2, Form::FdFsFt, kS | kD},
    {"div", 3, Form::FdFsFt, kS | kD},         {"sqrt", 4, Form::FdFs, kS | kD},          {"abs", 5, Form::FdFs, kS | kD},
    {"mov", 6, Form::FdFs, kS | kD},           {"neg", 7, Form::FdFs, kS | kD},           {"round.l", 8, Form::FdFs, kS | kD},
    {"trunc.l", 9, Form::FdFs, kS | kD},       {"ceil.l", 10, Form::FdFs, kS | kD},       {"floor.l", 11, Form::FdFs, kS | kD},
    {"round.w", 12, Form::FdFs, kS | kD},      {"trunc.w", 13, Form::FdFs, kS | kD},      {"ceil.w", 14, Form::FdFs, kS | kD},
    {"floor.w", 15, Form::FdFs, kS | kD},      {"cvt.s", 32, Form::FdFs, kD | kW | kL},   {"cvt.d", 33, Form::FdFs, kS | kW | kL},
    {"cvt.w", 36, Form::FdFs, kS | kD},        {"cvt.l", 37, Form::FdFs, kS | kD},
});
constexpr std::array<std::string_view, 16> kCompares = {"f", "un", "eq", "ueq", "olt", "ult", "ole", "ule", "sf", "ngle", "seq", "ngl", "lt", "nge", "le", "ngt"};
constexpr std::array<std::pair<unsigned, char>, 4> kFormats = {{{16, 's'}, {17, 'd'}, {20, 'w'}, {21, 'l'}}};

// One processor's instructions: the shared table and its own. The names the FPU's entries are made from live as long as the table does.
struct Table {
    std::vector<Def> defs;
    std::deque<std::string> names;
};

Table make_table(Variant variant) {
    Table t;
    for (const Def& d : kDefs)
        if (has(d.isa, variant)) t.defs.push_back(d);
    if (variant == Variant::R3000A) return t;
    for (const Def& d : kDefs4300)
        if (has(d.isa, variant)) t.defs.push_back(d);
    for (const Def& d : kDefsExtra)
        if (has(d.isa, variant)) t.defs.push_back(d);
    const std::uint8_t mine = static_cast<std::uint8_t>(1u << static_cast<unsigned>(variant));
    const auto add = [&](std::string name, Form form, std::uint32_t match, std::uint32_t mask) {
        t.names.push_back(std::move(name));
        t.defs.push_back({t.names.back(), form, match, mask, Flow::None, false, mine});
    };
    if (variant == Variant::R5900) {
        // the Emotion Engine's FPU: single precision only (and the integer word it converts from and to), with the accumulator and its own
        // operations; SQRT reads FT
        const auto s = [](std::uint32_t function) { return op(0x11) | (16u << 21) | function; };
        constexpr std::uint32_t kFdFsFt = 0xFFE0003F, kFdFs = 0xFFFF003F, kFsFt = 0xFFE007FF, kFdFt = 0xFFE0F83F;
        add("add.s", Form::FdFsFt, s(0), kFdFsFt);
        add("sub.s", Form::FdFsFt, s(1), kFdFsFt);
        add("mul.s", Form::FdFsFt, s(2), kFdFsFt);
        add("div.s", Form::FdFsFt, s(3), kFdFsFt);
        add("sqrt.s", Form::FdFt, s(4), kFdFt);
        add("abs.s", Form::FdFs, s(5), kFdFs);
        add("mov.s", Form::FdFs, s(6), kFdFs);
        add("neg.s", Form::FdFs, s(7), kFdFs);
        add("rsqrt.s", Form::FdFsFt, s(0x16), kFdFsFt);
        add("adda.s", Form::FsFt, s(0x18), kFsFt);
        add("suba.s", Form::FsFt, s(0x19), kFsFt);
        add("mula.s", Form::FsFt, s(0x1A), kFsFt);
        add("madd.s", Form::FdFsFt, s(0x1C), kFdFsFt);
        add("msub.s", Form::FdFsFt, s(0x1D), kFdFsFt);
        add("madda.s", Form::FsFt, s(0x1E), kFsFt);
        add("msuba.s", Form::FsFt, s(0x1F), kFsFt);
        add("cvt.w.s", Form::FdFs, s(0x24), kFdFs);
        add("max.s", Form::FdFsFt, s(0x28), kFdFsFt);
        add("min.s", Form::FdFsFt, s(0x29), kFdFsFt);
        add("cvt.s.w", Form::FdFs, op(0x11) | (20u << 21) | 0x20, kFdFs);
        add("c.f.s", Form::FsFt, s(0x30), kFsFt);
        add("c.eq.s", Form::FsFt, s(0x32), kFsFt);
        add("c.lt.s", Form::FsFt, s(0x34), kFsFt);
        add("c.le.s", Form::FsFt, s(0x36), kFsFt);
    }
    if (variant == Variant::R4300 || variant == Variant::Allegrex) {
        const bool single = variant == Variant::Allegrex;   // the Allegrex's FPU has no doubles and no 64-bit integers
        for (const FpuOp& f : kFpuOps) {
            if (single && std::string_view(f.name).ends_with(".l")) continue;
            for (std::size_t i = 0; i < kFormats.size(); ++i) {
                if ((f.formats & (1u << i)) == 0) continue;
                if (single && (i == 1 || i == 3)) continue;
                const auto [fmt, letter] = kFormats[i];
                const std::uint32_t match = op(0x11) | (fmt << 21) | f.function;
                add(std::format("{}.{}", f.name, letter), f.form, match, f.form == Form::FdFsFt ? 0xFFE0003Fu : 0xFFFF003Fu);
            }
        }
        for (unsigned c = 0; c < kCompares.size(); ++c)
            for (std::size_t i = 0; i < (single ? 1u : 2u); ++i)   // S and D
                add(std::format("c.{}.{}", kCompares[c], kFormats[i].second), Form::FsFt, op(0x11) | (kFormats[i].first << 21) | (0x30 + c), 0xFFE007FFu);
    }
    if (variant == Variant::R5900) {
        for (const MmiOp& m : kMmiOps) {
            const std::uint32_t match = op(0x1C) | m.function | (m.sub << 6);
            add(std::string(m.name), m.form, match, mmi_mask(m.form));
        }
    }
    return t;
}

const std::vector<Def>& defs_of(Variant variant) {
    static const Table r3000 = make_table(Variant::R3000A);
    static const Table r4300 = make_table(Variant::R4300);
    static const Table r5900 = make_table(Variant::R5900);
    static const Table allegrex = make_table(Variant::Allegrex);
    switch (variant) {
        case Variant::R3000A: return r3000.defs;
        case Variant::R4300: return r4300.defs;
        case Variant::R5900: return r5900.defs;
        case Variant::Allegrex: return allegrex.defs;
    }
    return r3000.defs;
}

// The GTE's commands: the function in the low six bits, and the whole 25-bit command word the PlayStation SDK's macros make
// of it (what the disassembler writes as the bare name).
struct GteDef {
    std::string_view name;
    std::uint32_t function;
    std::uint32_t usual;   // the command's 25 bits as the SDK writes them
};
constexpr auto kGte = std::to_array<GteDef>({
    {"rtps", 0x01, 0x0180001},  {"nclip", 0x06, 0x1400006}, {"op", 0x0C, 0x178000C},   {"dpcs", 0x10, 0x0780010},
    {"intpl", 0x11, 0x0980011}, {"mvmva", 0x12, 0x0400012}, {"ncds", 0x13, 0x0E80413}, {"cdp", 0x14, 0x1280414},
    {"ncdt", 0x16, 0x0F80416},  {"nccs", 0x1B, 0x108041B},  {"cc", 0x1C, 0x138041C},   {"ncs", 0x1E, 0x0C8041E},
    {"nct", 0x20, 0x0D80420},   {"sqr", 0x28, 0x0A80428},   {"dcpl", 0x29, 0x0680029}, {"dpct", 0x2A, 0x0F8002A},
    {"avsz3", 0x2D, 0x158002D}, {"avsz4", 0x2E, 0x168002E}, {"rtpt", 0x30, 0x0280030}, {"gpf", 0x3D, 0x190003D},
    {"gpl", 0x3E, 0x1A0003E},   {"ncct", 0x3F, 0x118043F},
});

// The bits of a command that are fields: sf (19) and lm (10), and for MVMVA also mx (18-17), v (16-15) and cv (14-13). The others the
// hardware ignores (for the commands but MVMVA that includes 18-13); the SDK fills 24-20 with a command number and leaves the rest zero.
constexpr std::uint32_t kSfLm = 0x80400;
constexpr std::uint32_t kFunctionBits = 0x3F;
constexpr std::uint32_t ignored_mask(bool mvmva) { return 0x1FFFFFF & ~(mvmva ? 0xFE400u : kSfLm) & ~kFunctionBits; }

const GteDef* gte_by_function(std::uint32_t function) {
    for (const GteDef& g : kGte)
        if (g.function == function) return &g;
    return nullptr;
}
const GteDef* gte_by_name(std::string_view name) {
    for (const GteDef& g : kGte)
        if (g.name == name) return &g;
    return nullptr;
}

constexpr std::array<std::string_view, 32> kRegisters = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
                                                         "$t3",   "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
                                                         "$s6",   "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

std::int32_t sign16(std::uint32_t v) { return static_cast<std::int16_t>(v & 0xFFFF); }

bool unsigned_immediate(std::string_view name) { return name == "andi" || name == "ori" || name == "xori" || name == "lui"; }
bool coprocessor_register(std::string_view name) { return name == "lwc2" || name == "swc2"; }
std::string fpu_register(unsigned n) { return "$f" + std::to_string(n); }

}  // namespace

std::string_view register_name(unsigned number) { return kRegisters.at(number & 31); }

std::optional<unsigned> register_number(std::string_view t) {
    t = text::trim(t);
    if (t.empty()) return std::nullopt;
    const std::string_view bare = t.front() == '$' ? t.substr(1) : t;
    if (bare.empty()) return std::nullopt;
    if (bare == "s8") return 30;
    if (std::isdigit(static_cast<unsigned char>(bare[0]))) {
        const auto n = text::parse_int<unsigned>(bare);
        if (n && *n < 32) return *n;
        return std::nullopt;
    }
    if (t.front() != '$') return std::nullopt;   // a name without the dollar is a label
    for (unsigned i = 0; i < kRegisters.size(); ++i)
        if (kRegisters[i].substr(1) == bare) return i;
    return std::nullopt;
}

std::optional<unsigned> fpu_register_number(std::string_view t) {
    t = text::trim(t);
    if (t.size() < 3 || t[0] != '$' || t[1] != 'f' || !std::isdigit(static_cast<unsigned char>(t[2]))) return std::nullopt;
    const auto n = text::parse_int<unsigned>(t.substr(2));
    if (n && *n < 32) return *n;
    return std::nullopt;
}

// ---- decoding ---------------------------------------------------------------------------------------------------

std::optional<Insn> decode_word(std::uint32_t word, std::uint32_t pc, Variant variant) {
    Insn insn;
    insn.word = word;
    insn.pc = pc;
    for (unsigned i = 0; i < 4; ++i) insn.raw[i] = static_cast<std::uint8_t>(word >> (big_endian(variant) ? 24 - 8 * i : 8 * i));
    insn.rs = static_cast<std::uint8_t>((word >> 21) & 31);
    insn.rt = static_cast<std::uint8_t>((word >> 16) & 31);
    insn.rd = static_cast<std::uint8_t>((word >> 11) & 31);
    insn.shamt = static_cast<std::uint8_t>((word >> 6) & 31);

    if (variant == Variant::R3000A && (word >> 25) == 0x25) {   // a GTE command: COP2 with bit 25 set
        const std::uint32_t command = word & 0x1FFFFFF;
        const GteDef* g = gte_by_function(command & kFunctionBits);
        insn.imm = static_cast<std::int32_t>(command);
        if (!g) {
            insn.mnemonic = "cop2";
            insn.form = Form::Raw;
            return insn;
        }
        // NCLIP has no fields: its sf and lm must be zero, whatever else is set is the ignored bits
        if (g->name == "nclip" && (command & kSfLm) != 0) {
            insn.mnemonic = "cop2";
            insn.form = Form::Raw;
            return insn;
        }
        insn.mnemonic = std::string(g->name);
        insn.form = Form::Command;
        return insn;
    }
    if (word == 0) {
        insn.mnemonic = "nop";
        insn.form = Form::None;
        return insn;
    }
    for (const Def& d : defs_of(variant)) {
        if ((word & d.mask) != d.match) continue;
        insn.mnemonic = std::string(d.name);
        insn.form = d.form;
        insn.flow = d.flow;
        insn.delay_slot = d.delay;
        insn.likely = d.likely;
        const std::int32_t offset = sign16(word);
        switch (d.form) {
            case Form::None:
                insn.imm = static_cast<std::int32_t>((word >> 6) & 0xFFFFF);   // syscall, break: the code
                if (d.mask == 0xFFFFFFFF) insn.imm = 0;   // (rfe, eret, tlbp ... : no code)
                break;
            case Form::RtRsImm:
            case Form::RtImm: insn.imm = unsigned_immediate(d.name) ? static_cast<std::int32_t>(word & 0xFFFF) : offset; break;
            case Form::RsImm:
            case Form::FtMem:
            case Form::OpMem:
            case Form::RtMem: insn.imm = offset; break;
            case Form::RsRtCode: insn.imm = static_cast<std::int32_t>((word >> 6) & 0x3FF); break;
            case Form::RsRtBranch:
            case Form::RsBranch:
            case Form::CopBranch: insn.target = pc + 4 + static_cast<std::uint32_t>(offset) * 4; break;
            case Form::Jump: insn.target = ((pc + 4) & 0xF0000000u) | ((word & 0x3FFFFFF) << 2); break;
            default: break;
        }
        if (d.name == "jr") insn.flow = insn.rs == 31 ? Flow::Return : Flow::Indirect;
        if ((d.name == "beq" || d.name == "beql") && insn.rs == 0 && insn.rt == 0) insn.flow = Flow::Jump;   // `b`
        if (d.name == "bgezal" && insn.rs == 0) insn.flow = Flow::Call;
        return insn;
    }
    // what the table does not say in one line: VU0's macro mode and the VFPU, EXT and INS (mips_ext)
    if ((variant == Variant::R5900 || variant == Variant::Allegrex) && ext::decode(word, pc, variant, insn)) return insn;
    return std::nullopt;
}

std::optional<Insn> decode(ByteView code, std::size_t at, std::uint32_t pc, Variant variant) {
    if (at + 4 > code.size()) return std::nullopt;
    const auto b = [&](std::size_t i) { return static_cast<std::uint32_t>(code[at + i]); };
    const std::uint32_t word = big_endian(variant) ? b(0) << 24 | b(1) << 16 | b(2) << 8 | b(3) : b(0) | b(1) << 8 | b(2) << 16 | b(3) << 24;
    return decode_word(word, pc, variant);
}

// ---- text -------------------------------------------------------------------------------------------------------

namespace {

std::string address_text(std::uint32_t address, const AddressLabels& labels) {
    if (const auto it = labels.find(address); it != labels.end() && !it->second.empty()) return it->second;
    return std::format("0x{:08X}", address);
}

std::string signed_text(std::int32_t v) { return std::to_string(v); }

std::string cop_register(unsigned n) { return "$" + std::to_string(n); }

// The command's fields as the assembler reads them: sf and lm (MVMVA also mx, v, cv), and the ignored bits when they are not the SDK's.
std::string command_text(const Insn& insn) {
    const std::uint32_t command = static_cast<std::uint32_t>(insn.imm);
    const GteDef* g = gte_by_function(command & kFunctionBits);
    const bool mvmva = insn.mnemonic == "mvmva";
    const std::uint32_t mask = ignored_mask(mvmva);
    const std::uint32_t ignored = command & mask;
    const std::uint32_t sf = (command >> 19) & 1, lm = (command >> 10) & 1;
    if (mvmva) {
        std::string out = std::format("mvmva {}, {}, {}, {}, {}", sf, (command >> 17) & 3, (command >> 15) & 3, (command >> 13) & 3, lm);
        if (ignored != (g->usual & mask)) out += std::format(", 0x{:X}", ignored);
        return out;
    }
    if (command == g->usual) return insn.mnemonic;
    if (insn.mnemonic == "nclip") return std::format("nclip 0, 0, 0x{:X}", ignored);   // (sf and lm are zero, so only the ignored bits differ)
    std::string out = std::format("{} {}, {}", insn.mnemonic, sf, lm);
    if (ignored != (g->usual & mask)) out += std::format(", 0x{:X}", ignored);
    return out;
}

}  // namespace

std::string text(const Insn& insn, const AddressLabels& labels, std::string_view immediate) {
    const std::string& m = insn.mnemonic;
    const auto reg = [](unsigned n) { return std::string(register_name(n)); };
    const auto imm_text = [&](std::string_view fallback) { return immediate.empty() ? std::string(fallback) : std::string(immediate); };
    switch (insn.form) {
        case Form::None:
            if (m == "nop" || m == "rfe") return m;
            return insn.imm == 0 ? m : std::format("{} 0x{:X}", m, insn.imm);
        case Form::RdRsRt:
            if (m == "or" && insn.rt == 0) return std::format("move {}, {}", reg(insn.rd), reg(insn.rs));
            return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rs), reg(insn.rt));
        case Form::RdRtSa: return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rt), insn.shamt);
        case Form::RdRtRs: return std::format("{} {}, {}, {}", m, reg(insn.rd), reg(insn.rt), reg(insn.rs));
        case Form::Rs: return std::format("{} {}", m, reg(insn.rs));
        case Form::Rd: return std::format("{} {}", m, reg(insn.rd));
        case Form::RdRs:
            if (insn.rd == 31 && m == "jalr") return std::format("{} {}", m, reg(insn.rs));
            return std::format("{} {}, {}", m, reg(insn.rd), reg(insn.rs));
        case Form::RsRt: return std::format("{} {}, {}", m, reg(insn.rs), reg(insn.rt));
        case Form::RtRsImm: {
            const std::string value = unsigned_immediate(m) ? std::format("0x{:X}", insn.imm) : signed_text(insn.imm);
            return std::format("{} {}, {}, {}", m, reg(insn.rt), reg(insn.rs), imm_text(value));
        }
        case Form::RtImm: return std::format("{} {}, {}", m, reg(insn.rt), imm_text(std::format("0x{:X}", insn.imm)));
        case Form::RsRtBranch: {
            const std::string to = address_text(*insn.target, labels);
            if (m == "beq" && insn.rs == 0 && insn.rt == 0) return "b " + to;
            if (insn.rt == 0 && (m == "beq" || m == "bne")) return std::format("{} {}, {}", m == "beq" ? "beqz" : "bnez", reg(insn.rs), to);
            return std::format("{} {}, {}, {}", m, reg(insn.rs), reg(insn.rt), to);
        }
        case Form::RsBranch: return std::format("{} {}, {}", m, reg(insn.rs), address_text(*insn.target, labels));
        case Form::Jump: return std::format("{} {}", m, address_text(*insn.target, labels));
        case Form::RtMem: {
            const std::string rt = coprocessor_register(m) ? cop_register(insn.rt) : reg(insn.rt);
            return std::format("{} {}, {}({})", m, rt, imm_text(signed_text(insn.imm)), reg(insn.rs));
        }
        case Form::RtCop: return std::format("{} {}, {}", m, reg(insn.rt), cop_register(insn.rd));
        case Form::CopBranch: return std::format("{} {}", m, address_text(*insn.target, labels));
        case Form::Command: return command_text(insn);
        case Form::Raw: return std::format("cop2 0x{:X}", insn.imm);
        case Form::RsImm: return std::format("{} {}, {}", m, reg(insn.rs), signed_text(insn.imm));
        case Form::RsRtCode:
            if (insn.imm == 0) return std::format("{} {}, {}", m, reg(insn.rs), reg(insn.rt));
            return std::format("{} {}, {}, {}", m, reg(insn.rs), reg(insn.rt), insn.imm);
        case Form::FtMem: return std::format("{} {}, {}({})", m, fpu_register(insn.rt), imm_text(signed_text(insn.imm)), reg(insn.rs));
        case Form::OpMem: return std::format("{} {}, {}({})", m, insn.rt, imm_text(signed_text(insn.imm)), reg(insn.rs));
        case Form::RtFs: return std::format("{} {}, {}", m, reg(insn.rt), fpu_register(insn.rd));
        case Form::FdFsFt: return std::format("{} {}, {}, {}", m, fpu_register(insn.shamt), fpu_register(insn.rd), fpu_register(insn.rt));
        case Form::FdFs: return std::format("{} {}, {}", m, fpu_register(insn.shamt), fpu_register(insn.rd));
        case Form::FsFt: return std::format("{} {}, {}", m, fpu_register(insn.rd), fpu_register(insn.rt));
        case Form::RdRt: return std::format("{} {}, {}", m, reg(insn.rd), reg(insn.rt));
        case Form::FdFt: return std::format("{} {}, {}", m, fpu_register(insn.shamt), fpu_register(insn.rt));
        case Form::Special: return ext::text(insn, labels);
    }
    return m;
}

std::string text(const Insn& insn, const AddressLabels& labels) { return text(insn, labels, std::string_view()); }

// ---- expressions ------------------------------------------------------------------------------------------------

namespace {

class Expr {
public:
    Expr(std::string_view text, const NameLabels& labels, std::uint32_t pc) : s_(text), labels_(labels), pc_(pc) {}

    std::int64_t run() {
        const std::int64_t v = bit_or();
        skip();
        if (i_ != s_.size()) fail("unexpected '" + std::string(s_.substr(i_)) + "'");
        return v;
    }

private:
    [[noreturn]] void fail(const std::string& why) const { throw ParseError("", 0, 0, std::string(s_) + " (" + why + ")"); }
    void skip() {
        while (i_ < s_.size() && text::is_space(s_[i_])) ++i_;
    }
    bool eat(std::string_view token) {
        skip();
        if (s_.substr(i_, token.size()) == token) {
            i_ += token.size();
            return true;
        }
        return false;
    }
    // an operator that is not the start of a longer one
    bool eat_op(char c, char not_followed) {
        skip();
        if (i_ < s_.size() && s_[i_] == c && !(i_ + 1 < s_.size() && s_[i_ + 1] == not_followed)) {
            ++i_;
            return true;
        }
        return false;
    }

    std::int64_t bit_or() {
        std::int64_t v = bit_xor();
        while (eat_op('|', '\0')) v |= bit_xor();
        return v;
    }
    std::int64_t bit_xor() {
        std::int64_t v = bit_and();
        while (eat_op('^', '\0')) v ^= bit_and();
        return v;
    }
    std::int64_t bit_and() {
        std::int64_t v = shift();
        while (eat_op('&', '\0')) v &= shift();
        return v;
    }
    std::int64_t shift() {
        std::int64_t v = sum();
        for (;;) {
            if (eat("<<")) {
                const std::int64_t n = sum();
                if (n < 0 || n > 62) fail("shift out of range");
                v <<= n;
            } else if (eat(">>")) {
                const std::int64_t n = sum();
                if (n < 0 || n > 62) fail("shift out of range");
                v >>= n;
            } else {
                return v;
            }
        }
    }
    std::int64_t sum() {
        std::int64_t v = product();
        for (;;) {
            if (eat_op('+', '\0')) v += product();
            else if (eat_op('-', '\0')) v -= product();
            else return v;
        }
    }
    std::int64_t product() {
        std::int64_t v = unary();
        for (;;) {
            if (eat_op('*', '\0')) {
                v *= unary();
            } else if (eat_op('/', '\0')) {
                const std::int64_t d = unary();
                if (d == 0) fail("division by zero");
                v /= d;
            } else {
                return v;
            }
        }
    }
    std::int64_t unary() {
        skip();
        if (eat("-")) return -unary();
        if (eat("+")) return unary();
        if (eat("~")) return ~unary();
        return primary();
    }
    std::int64_t primary() {
        skip();
        if (i_ >= s_.size()) fail("an operand is missing");
        if (eat("%hi(")) return ((close(bit_or()) + 0x8000) >> 16) & 0xFFFF;
        if (eat("%lo(")) return close(bit_or()) & 0xFFFF;
        if (eat("(")) return close(bit_or());
        const char c = s_[i_];
        if (std::isdigit(static_cast<unsigned char>(c))) return number();
        if (c == '.' && (i_ + 1 >= s_.size() || !(std::isalnum(static_cast<unsigned char>(s_[i_ + 1])) || s_[i_ + 1] == '_'))) {
            ++i_;
            return pc_;   // `.` is here
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.' || c == '@') {
            const std::size_t from = i_;
            while (i_ < s_.size() && (std::isalnum(static_cast<unsigned char>(s_[i_])) || s_[i_] == '_' || s_[i_] == '.' || s_[i_] == '@')) ++i_;
            const std::string_view name = s_.substr(from, i_ - from);
            const auto it = labels_.find(name);
            if (it == labels_.end()) throw z80::UnknownLabel(std::string(name));
            return it->second;
        }
        fail(std::string("unexpected '") + c + "'");
    }
    std::int64_t close(std::int64_t v) {
        if (!eat(")")) fail("a ')' is missing");
        return v;
    }
    std::int64_t number() {
        const std::size_t from = i_;
        int base = 10;
        if (s_.substr(i_, 2) == "0x" || s_.substr(i_, 2) == "0X") {
            base = 16;
            i_ += 2;
        }
        const std::size_t digits = i_;
        while (i_ < s_.size() && std::isxdigit(static_cast<unsigned char>(s_[i_])) && (base == 16 || std::isdigit(static_cast<unsigned char>(s_[i_])))) ++i_;
        const auto v = text::parse_int<std::uint64_t>(s_.substr(digits, i_ - digits), base);
        if (!v || i_ == digits) fail("'" + std::string(s_.substr(from, i_ - from)) + "' is not a number");
        return static_cast<std::int64_t>(*v);
    }

    std::string_view s_;
    const NameLabels& labels_;
    std::uint32_t pc_;
    std::size_t i_ = 0;
};

}  // namespace

std::int64_t value(std::string_view expr, const NameLabels& labels, std::uint32_t pc) { return Expr(expr, labels, pc).run(); }

// ---- assembling -------------------------------------------------------------------------------------------------

namespace {

[[noreturn]] void refuse(std::string_view line, const std::string& why) { throw ParseError("", 0, 0, std::string(line) + " (" + why + ")"); }

void put_word(Bytes& out, std::uint32_t word, bool big) {
    for (unsigned i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(word >> (big ? 24 - 8 * i : 8 * i)));
}

unsigned need_fpu(std::string_view line, std::string_view operand) {
    if (const auto r = fpu_register_number(operand)) return *r;
    refuse(line, "'" + std::string(operand) + "' is not an FPU register");
}

unsigned need_register(std::string_view line, std::string_view operand) {
    const auto r = register_number(operand);
    if (!r) refuse(line, "'" + std::string(operand) + "' is not a register");
    return *r;
}

// The field bits of a command word from operands.
struct Operands {
    std::string_view line;
    std::vector<std::string> list;
    const NameLabels& labels;
    std::uint32_t pc;

    void count(std::size_t n) const {
        if (list.size() != n) refuse(line, std::format("{} operands where {} are written", list.size(), n));
    }
    std::int64_t number(std::size_t i) const { return value(list[i], labels, pc); }
    unsigned reg(std::size_t i) const { return need_register(line, list[i]); }
};

// `imm($reg)` or `($reg)`: the offset expression and the base register.
bool split_memory(std::string_view operand, std::string_view& offset, std::string_view& base) {
    operand = text::trim(operand);
    if (operand.empty() || operand.back() != ')') return false;
    int depth = 0;
    for (std::size_t i = operand.size(); i-- > 0;) {
        if (operand[i] == ')') ++depth;
        if (operand[i] == '(' && --depth == 0) {
            base = text::trim(operand.substr(i + 1, operand.size() - i - 2));
            offset = text::trim(operand.substr(0, i));
            return register_number(base).has_value();
        }
    }
    return false;
}

std::uint32_t field(std::string_view line, std::int64_t v, std::int64_t lo, std::int64_t hi, std::string_view what) {
    if (v < lo || v > hi) refuse(line, std::format("{} {} does not fit", what, v));
    return static_cast<std::uint32_t>(v);
}

std::uint32_t branch_offset(std::string_view line, std::int64_t target, std::uint32_t pc) {
    const std::int64_t distance = target - (static_cast<std::int64_t>(pc) + 4);
    if (distance % 4 != 0) refuse(line, "a branch target is not a word");
    if (distance / 4 < -32768 || distance / 4 > 32767) refuse(line, "the branch is out of reach");
    return static_cast<std::uint32_t>(distance / 4) & 0xFFFF;
}

}  // namespace

Bytes assemble(std::string_view line, std::uint32_t pc, const NameLabels& labels, Variant variant) {
    line = text::trim(line);
    std::size_t split = 0;
    while (split < line.size() && !text::is_space(line[split])) ++split;
    const std::string name = text::lower(line.substr(0, split));
    Operands ops{line, z80::split_operands(line.substr(split)), labels, pc};
    Bytes out;
    const bool big = big_endian(variant);
    const auto put_word = [&](Bytes& to, std::uint32_t word) { mips::put_word(to, word, big); };

    // ---- pseudo-instructions
    if (name == "nop") {
        ops.count(0);
        put_word(out, 0);
        return out;
    }
    if (name == "move") {
        ops.count(2);
        put_word(out, (ops.reg(0) << 11) | (ops.reg(1) << 21) | 0x25);   // or rd, rs, $zero (as GNU as writes it)
        return out;
    }
    if (name == "b") {
        ops.count(1);
        put_word(out, op(4) | branch_offset(line, ops.number(0), pc));
        return out;
    }
    if (name == "beqz" || name == "bnez") {
        ops.count(2);
        put_word(out, op(name == "beqz" ? 4 : 5) | (ops.reg(0) << 21) | branch_offset(line, ops.number(1), pc));
        return out;
    }
    if (name == "li") {
        ops.count(2);
        const std::int64_t v = ops.number(1);
        const unsigned rt = ops.reg(0);
        if (v >= -32768 && v <= 32767) {
            put_word(out, op(9) | (rt << 16) | (static_cast<std::uint32_t>(v) & 0xFFFF));            // addiu rt, $zero, v
        } else if (v >= 0 && v <= 0xFFFF) {
            put_word(out, op(13) | (rt << 16) | static_cast<std::uint32_t>(v));                       // ori rt, $zero, v
        } else if (v >= -0x80000000LL && v <= 0xFFFFFFFFLL) {
            const std::uint32_t u = static_cast<std::uint32_t>(v);
            put_word(out, op(15) | (rt << 16) | (u >> 16));                                           // lui rt, hi
            if ((u & 0xFFFF) != 0) put_word(out, op(13) | (rt << 21) | (rt << 16) | (u & 0xFFFF));    // ori rt, rt, lo
        } else {
            refuse(line, "the value does not fit in 32 bits");
        }
        return out;
    }
    if (name == "la") {
        ops.count(2);
        const std::uint32_t u = static_cast<std::uint32_t>(ops.number(1));
        const unsigned rt = ops.reg(0);
        put_word(out, op(15) | (rt << 16) | (((u + 0x8000) >> 16) & 0xFFFF));                         // lui rt, %hi
        put_word(out, op(9) | (rt << 21) | (rt << 16) | (u & 0xFFFF));                                  // addiu rt, rt, %lo
        return out;
    }

    // ---- GTE commands
    if (const GteDef* g = variant == Variant::R3000A ? gte_by_name(name) : nullptr) {
        std::uint32_t command = g->usual;
        if (!ops.list.empty()) {
            const bool mvmva = name == "mvmva";
            const std::size_t fields = mvmva ? 5 : 2;
            if (ops.list.size() != fields && ops.list.size() != fields + 1)
                refuse(line, std::format("{} takes {} fields (and the ignored bits)", name, fields));
            command = g->function;
            const std::uint32_t sf = field(line, ops.number(0), 0, 1, "sf");
            std::uint32_t lm = 0;
            if (mvmva) {
                command |= field(line, ops.number(1), 0, 3, "mx") << 17;
                command |= field(line, ops.number(2), 0, 3, "v") << 15;
                command |= field(line, ops.number(3), 0, 3, "cv") << 13;
                lm = field(line, ops.number(4), 0, 1, "lm");
            } else {
                lm = field(line, ops.number(1), 0, 1, "lm");
            }
            command |= (sf << 19) | (lm << 10);
            if (ops.list.size() == fields + 1) {
                const std::uint32_t ignored = field(line, ops.number(fields), 0, 0x1FFFFFF, "the ignored bits");
                if ((ignored & ~ignored_mask(mvmva)) != 0) refuse(line, "those bits are fields");
                command |= ignored;
            } else {
                command |= g->usual & ignored_mask(mvmva);
            }
        }
        put_word(out, op(0x12) | (1u << 25) | command);
        return out;
    }
    if (name == "cop2" && variant == Variant::R3000A) {
        ops.count(1);
        put_word(out, op(0x12) | (1u << 25) | field(line, ops.number(0), 0, 0x1FFFFFF, "the command"));
        return out;
    }

    // ---- the table
    const Def* def = nullptr;
    for (const Def& d : defs_of(variant)) {
        if (d.name != name) continue;
        // an instruction with a two-operand and a three-operand form (the R5900's MULT and MADD): the one the operands fit
        if (def && (d.form == Form::RdRsRt) != (ops.list.size() == 3)) continue;
        def = &d;
    }
    if (!def) {
        if (variant == Variant::R5900 || variant == Variant::Allegrex) {
            if (const std::optional<std::uint32_t> word = ext::assemble(name, ops.list, pc, labels, variant)) {
                put_word(out, *word);
                return out;
            }
        }
        refuse(line, "not an instruction");
    }
    std::uint32_t word = def->match;
    switch (def->form) {
        case Form::None:
            if (def->mask == 0xFFFFFFFF) {   // (rfe, eret, tlbp ... take nothing)
                ops.count(0);
            } else {
                if (ops.list.size() > 1) refuse(line, "too many operands");
                if (!ops.list.empty()) word |= field(line, ops.number(0), 0, 0xFFFFF, "the code") << 6;
            }
            break;
        case Form::RdRsRt: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 21) | (ops.reg(2) << 16); break;
        case Form::RdRtSa: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 16) | (field(line, ops.number(2), 0, 31, "the shift") << 6); break;
        case Form::RdRtRs: ops.count(3); word |= (ops.reg(0) << 11) | (ops.reg(1) << 16) | (ops.reg(2) << 21); break;
        case Form::Rs: ops.count(1); word |= ops.reg(0) << 21; break;
        case Form::Rd: ops.count(1); word |= ops.reg(0) << 11; break;
        case Form::RdRs:
            if (ops.list.size() == 1 && def->name == "jalr") {
                word |= (31u << 11) | (ops.reg(0) << 21);
            } else {
                ops.count(2);
                word |= (ops.reg(0) << 11) | (ops.reg(1) << 21);
            }
            break;
        case Form::RsRt: ops.count(2); word |= (ops.reg(0) << 21) | (ops.reg(1) << 16); break;
        case Form::RtRsImm:
            ops.count(3);
            word |= (ops.reg(0) << 16) | (ops.reg(1) << 21) | (field(line, ops.number(2), -32768, 65535, "the immediate") & 0xFFFF);
            break;
        case Form::RtImm:
            ops.count(2);
            word |= (ops.reg(0) << 16) | (field(line, ops.number(1), -32768, 65535, "the immediate") & 0xFFFF);
            break;
        case Form::RsRtBranch: ops.count(3); word |= (ops.reg(0) << 21) | (ops.reg(1) << 16) | branch_offset(line, ops.number(2), pc); break;
        case Form::RsBranch: ops.count(2); word |= (ops.reg(0) << 21) | branch_offset(line, ops.number(1), pc); break;
        case Form::CopBranch: ops.count(1); word |= branch_offset(line, ops.number(0), pc); break;
        case Form::Jump: {
            ops.count(1);
            const std::int64_t to = ops.number(0);
            if (to % 4 != 0) refuse(line, "a jump target is not a word");
            if (((to >> 28) & 0xF) != ((static_cast<std::int64_t>(pc) + 4) >> 28 & 0xF)) refuse(line, "the jump is into another 256 MB region");
            word |= static_cast<std::uint32_t>(to >> 2) & 0x3FFFFFF;
            break;
        }
        case Form::FtMem:
        case Form::OpMem:
        case Form::RtMem: {
            ops.count(2);
            std::string_view offset, base;
            if (!split_memory(ops.list[1], offset, base)) refuse(line, "a memory operand is offset($register)");
            unsigned rt = 0;
            if (def->form == Form::FtMem) rt = need_fpu(line, ops.list[0]);
            else if (def->form == Form::OpMem) rt = field(line, ops.number(0), 0, 31, "the operation");
            else if (coprocessor_register(def->name)) rt = field(line, register_number(ops.list[0]).value_or(99), 0, 31, "the register");
            else rt = ops.reg(0);
            const std::int64_t off = offset.empty() ? 0 : value(offset, labels, pc);
            word |= (rt << 16) | (need_register(line, base) << 21) | (field(line, off, -32768, 65535, "the offset") & 0xFFFF);
            break;
        }
        case Form::RtCop: ops.count(2); word |= (ops.reg(0) << 16) | (ops.reg(1) << 11); break;
        case Form::RsRtCode:
            if (ops.list.size() != 2) ops.count(3);
            word |= (ops.reg(0) << 21) | (ops.reg(1) << 16);
            if (ops.list.size() == 3) word |= field(line, ops.number(2), 0, 1023, "the code") << 6;
            break;
        case Form::RsImm: ops.count(2); word |= (ops.reg(0) << 21) | (field(line, ops.number(1), -32768, 65535, "the immediate") & 0xFFFF); break;
        case Form::RtFs: ops.count(2); word |= (ops.reg(0) << 16) | (need_fpu(line, ops.list[1]) << 11); break;
        case Form::FdFsFt: ops.count(3); word |= (need_fpu(line, ops.list[0]) << 6) | (need_fpu(line, ops.list[1]) << 11) | (need_fpu(line, ops.list[2]) << 16); break;
        case Form::FdFs: ops.count(2); word |= (need_fpu(line, ops.list[0]) << 6) | (need_fpu(line, ops.list[1]) << 11); break;
        case Form::FsFt: ops.count(2); word |= (need_fpu(line, ops.list[0]) << 11) | (need_fpu(line, ops.list[1]) << 16); break;
        case Form::RdRt: ops.count(2); word |= (ops.reg(0) << 11) | (ops.reg(1) << 16); break;
        case Form::FdFt: ops.count(2); word |= (need_fpu(line, ops.list[0]) << 6) | (need_fpu(line, ops.list[1]) << 16); break;
        case Form::Command:
        case Form::Raw:
        case Form::Special: break;
    }
    put_word(out, word);
    return out;
}

}  // namespace cb::decompile::mips

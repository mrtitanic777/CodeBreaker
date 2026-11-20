// decompile/a64_internal.hpp -- what a64.cpp, a64_table.cpp and a64_simd.cpp share: the operand kinds, a row, and the builder that reads a row's bit pattern and operand
// list. Not part of the public interface (decompile/a64.hpp).
//
// A row is written as a 32-symbol bit pattern (bit 31 first, spaces ignored) in which `0` and `1` are fixed bits and any other symbol names a field, and a list of operand
// tokens:
//
//     add("add", Op::AddSubImm, "s 0 0 10001 0 h iiiiiiiiiiii nnnnn ddddd", "RSd,RSn,AIMM");
//
// An operand token is a kind in capitals, an optional decimal parameter, and the lowercase letter of the field it reads (the kinds that read several fields name them in
// their description below and take no letter). The letters mean the same in every row of a class, and a field is wherever its symbol stands in the pattern.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "decompile/a64.hpp"

namespace cb::decompile::a64::detail {

enum class K : std::uint8_t {
    // general registers: X 64-bit, W 32-bit, R by the sf bit (bit 31); the S variants read register 31 as sp instead of the zero register
    X, W, R, XS, WS, RS,
    // scalar and vector registers by field: FB FH FS FD FQ are b h s d q; V is a vector with an arrangement from the size field `z` and Q bit 30 (`V` 8b..2d, `VF` the
    // floating-point 2s 4s 2d, `VH` the half-precision 4h 8h)
    FB, FH, FS, FD, FQ,
    // immediates: U unsigned decimal/hex, I signed; UW unsigned with the field's width and a `#` always; the field's bits only
    U, I,
    // add / sub immediate: fields i (imm12) and h (the shift), `#imm{, lsl #12}` or `:lo12:name`
    AIMM,
    // shifted register Rm{, shift #amt} (fields m, h, i), extended register Rm{, ext {#amt}} (fields m, o, i)
    SREG, EREG,
    // a logical immediate (fields N, r, u), a move-wide operand `#imm16{, lsl #n}` (i, h), `mov` of a movz / movn value, a bitmask value for `mov`
    LI, MWI, MOVZV, MOVNV,
    // bitfield aliases (fields r, u, sf): lsl / lsr / asr shifts, ubfx-style `lsb, width`, ubfiz-style `lsb, width`
    LSLI, SHRI, BFX, BFIZ,
    // conditions: COND the field, CONDINV its inverse
    COND, CONDINV,
    // pc-relative targets: PR the field (any width) times four, PADR adr (fields l, h), PADRP adrp
    PR, PADR, PADRP,
    // memory: MU<n> unsigned offset scaled by 1<<n (n base, i imm12); MT unscaled signed 9-bit; MPRE pre-index; MPOST post-index; MR<n> register offset (n, m, o, e);
    // MPO<n> pair offset (n, i imm7 scaled by 1<<n), MPPRE<n>, MPPOST<n>; MX the base register alone; MLO the single-element base
    MU, MT, MPRE, MPOST, MR, MPO, MPPRE, MPPOST, MX,
    // the system register (field y: 15 bits), a barrier option (field c), a prefetch operation (field t), a fixed literal
    SYSREG, BAR, PF, LIT,
    // the bit number of tbz (fields s, k), an fp immediate (field i), fixed-point bits (field u: 64 - scale)
    TBIT, FPIMM, FBITS,
    // vector registers with an arrangement: V by the size field (bits 23:22) and Q (bit 30): 8b 16b 4h 8h 2s 4s 1d 2d; VF by the floating-point size bit 22: 2s 4s 2d; VB bytes;
    // VW the wide arrangement of a size (8h 4s 2d); VPL the pairwise-long arrangement; VT / VSH / VSHW by imm5 (copy) or immh (shift-immediate) and Q; FZ a scalar of the size
    // field (b h s d), FZW twice as wide; VEL / VEL2 a vector element (index from imm5, or the second index from imm4); RG a general register by imm5; BYEL / BYELF the
    // by-element operand; SHL / SHR the shift-immediate amounts; VMA / VMI the arrangement and the immediate of a modified-immediate instruction; SLIST / TLIST register lists
    // and SPOST the post-index of a structure load; VD1 the upper half `v<n>.d[1]`
    V, VF, VB, VW, VX, VXW, VPL, VT, VSH, VSHW, FZ, FZW, VEL, VEL2, RG, RGQ, BYEL, BYELF, SHL, SHR, SHLL, VMA, VMI, SLIST, TLIST, SPOST, SLANE, RLIST, VD1,
};

struct Operand {
    K kind = K::U;
    std::uint8_t pos = 0, width = 0;   // the field the token names
    std::uint8_t param = 0;            // the token's number
    std::string literal;               // LIT
};

// An optional extra condition on a word the mask matched (for aliases and for fields that must not take every value).
using When = bool (*)(std::uint32_t word);

struct Row {
    std::string name;
    std::uint32_t mask = 0, bits = 0;
    Op op = Op::None;
    Flow flow = Flow::None;
    std::vector<Operand> operands;
    When when = nullptr;
    std::uint32_t (*fix)(std::uint32_t word) = nullptr;   // after the operands are read: fields an alias implies (the second source of `cinc` is its first)
    bool cond_suffix = false;   // `b.cond`: the condition after a dot in the mnemonic
    bool simd = false;          // for the table's own ordering checks
};

class Builder {
public:
    // Add a row; `pattern` as described above, `spec` the comma-separated operand tokens.
    Row& add(std::string name, Op op, std::string_view pattern, std::string_view spec, Flow flow = Flow::None, When when = nullptr);
    // The `b.cond` row.
    void add_cond_suffix(std::string name, Op op, std::string_view pattern, std::string_view spec, Flow flow);
    std::vector<Row>& rows() { return rows_; }

private:
    std::vector<Row> rows_;
};

// The table, made once.
const std::vector<Row>& rows();
// Rows of a mnemonic, in table order.
const std::vector<std::uint16_t>& rows_named(std::string_view name);

void add_integer_rows(Builder& b);
void add_memory_rows(Builder& b);
void add_fp_rows(Builder& b);
void add_simd_rows(Builder& b);

// The vector operand kinds (a64_simd.cpp): the kind a token's name says (throws for an unknown name), and reading / writing one. Reading starts at tokens[at] and moves `at`;
// `error` is set (when empty) with why it failed.
K simd_kind_by_name(std::string_view name);
bool parse_simd_operand(const Row& row, const Operand& o, const std::vector<std::string_view>& tokens, std::size_t& at, std::uint32_t& word, bool& arr_set, std::string& error,
                        const NameLabels& labels, bool lenient);
std::string print_simd_operand(const Row& row, const Operand& o, std::uint32_t word);
// The rows that may match a word whose bits 28:25 are `bucket`.
const std::vector<std::uint16_t>& table_bucket(unsigned bucket);

// Helpers shared by predicates.
inline std::uint32_t bits(std::uint32_t w, unsigned pos, unsigned width) { return (w >> pos) & (width >= 32 ? 0xFFFFFFFFu : ((1u << width) - 1)); }

}  // namespace cb::decompile::a64::detail

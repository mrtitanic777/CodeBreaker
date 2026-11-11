// cb/recompile/jumptable.cpp -- see jumptable.hpp.

#include "cb/recompile/jumptable.hpp"

#include <utility>

namespace cb::recompile::jumptable {
namespace {

constexpr std::uint32_t kMaxScanBack = 64;   // instructions looked at before the bctr

using Reg = std::uint32_t;

// ---- PPC instruction fields (a big-endian word already decoded to a number) ------------------------------------
constexpr std::uint32_t op(std::uint32_t w) { return (w >> 26) & 0x3F; }
constexpr std::uint32_t xo(std::uint32_t w) { return (w >> 1) & 0x3FF; }   // X/XO extended opcode (bits 1-10)
// D form
constexpr Reg d_rt(std::uint32_t w) { return (w >> 21) & 0x1F; }
constexpr Reg d_ra(std::uint32_t w) { return (w >> 16) & 0x1F; }
constexpr std::uint32_t d_uimm(std::uint32_t w) { return w & 0xFFFF; }
// X and XO form
constexpr Reg x_rt(std::uint32_t w) { return (w >> 21) & 0x1F; }
constexpr Reg x_ra(std::uint32_t w) { return (w >> 16) & 0x1F; }
constexpr Reg x_rb(std::uint32_t w) { return (w >> 11) & 0x1F; }
// M form (rlwinm): RS = source (bits 21-25), RA = destination (16-20), SH (11-15), MB (6-10), ME (1-5)
constexpr Reg m_rs(std::uint32_t w) { return (w >> 21) & 0x1F; }
constexpr Reg m_ra(std::uint32_t w) { return (w >> 16) & 0x1F; }
constexpr std::uint32_t m_sh(std::uint32_t w) { return (w >> 11) & 0x1F; }
constexpr std::uint32_t m_mb(std::uint32_t w) { return (w >> 6) & 0x1F; }
constexpr std::uint32_t m_me(std::uint32_t w) { return (w >> 1) & 0x1F; }

constexpr std::uint32_t spr_of(std::uint32_t w) { return ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5); }

// ---- opcode predicates (the classification of the rexglue decoder) ---------------------------------------------
// mtctr: the source register.
std::optional<Reg> mtctr_source(std::uint32_t w) {
    if (op(w) != 31 || xo(w) != 467 || spr_of(w) != 9) return std::nullopt;
    return x_rt(w);
}
constexpr bool is_mflr(std::uint32_t w) { return op(w) == 31 && xo(w) == 339 && spr_of(w) == 8; }
constexpr bool is_add(std::uint32_t w) { return op(w) == 31 && xo(w) == 266; }
constexpr bool is_lwzx(std::uint32_t w) { return op(w) == 31 && xo(w) == 23; }
constexpr bool is_lbzx(std::uint32_t w) { return op(w) == 31 && xo(w) == 87; }
constexpr bool is_lhzx(std::uint32_t w) { return op(w) == 31 && xo(w) == 279; }
constexpr bool is_rlwinm(std::uint32_t w) { return op(w) == 21; }
constexpr bool is_lis(std::uint32_t w) { return op(w) == 15 && d_ra(w) == 0; }   // addis rX, r0, imm
constexpr bool is_addi(std::uint32_t w) { return op(w) == 14; }
constexpr bool is_ori(std::uint32_t w) { return op(w) == 24; }
constexpr bool is_bctr(std::uint32_t w) { return op(w) == 19 && xo(w) == 528 && (w & 1) == 0; }
constexpr bool is_bcctr_any(std::uint32_t w) { return op(w) == 19 && xo(w) == 528; }   // bctr or bctrl
constexpr bool is_b(std::uint32_t w) { return op(w) == 18; }                            // b / ba
// bclr with BO = 20 (unconditional) is blr
constexpr bool is_blr(std::uint32_t w) { return op(w) == 19 && xo(w) == 16 && ((w >> 21) & 0x1F) == 20; }
constexpr bool is_cmpli(std::uint32_t w) { return op(w) == 10; }   // cmplwi / cmpldi
constexpr bool is_cmpi(std::uint32_t w) { return op(w) == 11; }    // cmpwi / cmpdi
constexpr bool is_stwu(std::uint32_t w) { return op(w) == 37; }

// A word that ends the straight-line code the scan may look through: padding, a return, an indirect or direct
// branch, a link-register save, or a stack frame's `stwu r1, -N(r1)`.
bool is_function_boundary(std::uint32_t w) {
    return w == 0 || is_blr(w) || is_bcctr_any(w) || is_b(w) || is_mflr(w) || (is_stwu(w) && d_ra(w) == 1 && d_rt(w) == 1);
}

constexpr std::int32_t s16(std::uint32_t v) { return static_cast<std::int32_t>(v & 0x8000 ? v | 0xFFFF0000u : v); }

// An address reached by arithmetic that may leave the 32-bit space is unmapped, not wrapped.
std::optional<std::uint8_t> read_u8(const GuestMemory& m, std::uint64_t a) {
    return a > 0xFFFFFFFFull ? std::nullopt : m.u8(static_cast<std::uint32_t>(a));
}
std::optional<std::uint16_t> read_u16(const GuestMemory& m, std::uint64_t a) {
    return a > 0xFFFFFFFFull ? std::nullopt : m.u16(static_cast<std::uint32_t>(a));
}
std::optional<std::uint32_t> read_u32(const GuestMemory& m, std::uint64_t a) {
    return a > 0xFFFFFFFFull ? std::nullopt : m.u32(static_cast<std::uint32_t>(a));
}

// The address of the instruction `back` words before `from`; nullopt when it would fall below address 4 (the
// first words of the image are never code).
std::optional<std::uint32_t> before(std::uint32_t from, std::uint32_t back) {
    const std::uint64_t d = static_cast<std::uint64_t>(back) * 4;
    if (from < d + 4) return std::nullopt;
    return static_cast<std::uint32_t>(from - d);
}

// ---- the table's shape --------------------------------------------------------------------------------------------
// How the entries of the table turn into targets.
enum class TableType { None, Abs, Computed, ByteOff, ShortOff };

// The state of one backwards scan.
struct TableMatch {
    TableType type = TableType::None;
    std::optional<Reg> ctr_reg, table_reg, index_reg, base_reg, offset_reg;
    std::uint32_t shift = 0;
    std::uint32_t table_high = 0, table_low = 0, base_high = 0, base_low = 0;
    bool found_mtctr = false, found_add = false, found_load = false, found_shift = false;
    bool found_table_lis = false, found_table_addi = false, found_base_lis = false, found_base_addi = false;
};

// How many entries the bounds check says the table has: `cmplwi rI, N` or `clrlwi rI, rI, k` before the bctr.
// 0 when no check is found (the table then runs while its entries look like code).
std::uint32_t scan_bounds(const GuestMemory& memory, std::uint32_t bctr_addr, Reg expected_index_reg) {
    std::uint32_t max_entries = 0;
    constexpr std::uint32_t kNoField = 0xFF;
    std::uint32_t cr_field = kNoField;
    for (std::uint32_t i = 1; i <= kMaxScanBack; ++i) {
        const auto addr = before(bctr_addr, i);
        if (!addr) break;
        const std::optional<std::uint32_t> word = memory.u32(*addr);
        if (!word || *word == 0) break;
        const std::uint32_t w = *word;
        // a conditional branch testing the GT bit names the CR field the compare used
        if (cr_field == kNoField && op(w) == 16) {
            const std::uint32_t bi = (w >> 16) & 0x1F;
            if ((bi & 0x3) == 1) cr_field = (bi >> 2) & 0x7;   // bgt / ble
        }
        // the implicit mask of a clrlwi on the index register: 2^(32-mb) entries
        if (is_rlwinm(w) && m_ra(w) == expected_index_reg) {
            const std::uint32_t mb = m_mb(w);
            if (m_sh(w) == 0 && m_me(w) == 31 && mb > 0 && mb < 32) {
                const std::uint32_t count = 1u << (32 - mb);
                if (count >= 2 && count <= 256) return count;
            }
        }
        // cmpli / cmpi: the count is the immediate + 1
        if (is_cmpli(w) || is_cmpi(w)) {
            const std::uint32_t imm = d_uimm(w);
            if (imm <= 1) continue;
            const bool reg_ok = d_ra(w) == expected_index_reg;
            const bool cr_ok = cr_field != kNoField && (d_rt(w) >> 2) == cr_field;
            if (reg_ok || cr_ok) {
                max_entries = imm + 1;
                if (reg_ok) return max_entries;
            }
        }
    }
    return max_entries;
}

// The case targets of the table, `count` entries (0: until the entries stop looking like code).
std::vector<std::uint32_t> read_targets(const TableMatch& m, std::uint32_t count, const GuestMemory& memory) {
    const std::uint32_t table = m.table_high + static_cast<std::uint32_t>(s16(m.table_low));
    const std::uint32_t base = m.base_high + static_cast<std::uint32_t>(s16(m.base_low));
    std::vector<std::uint32_t> out;
    const std::size_t limit = count == 0 ? kMaxTableEntries : count;
    for (std::size_t i = 0; i < limit; ++i) {
        std::uint32_t target = 0;
        switch (m.type) {
            case TableType::Abs: {
                const auto t = read_u32(memory, static_cast<std::uint64_t>(table) + i * 4);
                if (!t) return out;
                target = *t;
                break;
            }
            case TableType::Computed: {
                const auto b = read_u8(memory, static_cast<std::uint64_t>(table) + i);
                if (!b) return out;
                target = base + (static_cast<std::uint32_t>(*b) << m.shift);
                break;
            }
            case TableType::ByteOff: {
                const auto b = read_u8(memory, static_cast<std::uint64_t>(table) + i);
                if (!b) return out;
                target = base + *b;
                break;
            }
            case TableType::ShortOff:
            case TableType::None: {
                const auto h = read_u16(memory, static_cast<std::uint64_t>(table) + i * 2);
                if (!h) return out;
                target = base + *h;
                break;
            }
        }
        if (target & 3) break;
        if (target == 0 && m.type == TableType::Abs) break;
        if (!memory.is_exec(target)) {
            if (m.type == TableType::Abs) break;
            continue;   // an offset table may hold padding between its entries
        }
        out.push_back(target);
    }
    return out;
}

// A `lis` naming the table register is the table's high half -- or, when the table register is also the base
// register, first the base's and then the table's. false: both were already found, the instruction is not used.
bool take_lis_for_table(TableMatch& m, std::uint32_t w) {
    const bool reuse = m.found_add && m.table_reg == m.base_reg;
    const std::uint32_t high = d_uimm(w) << 16;
    if (reuse && !m.found_base_lis) {
        m.base_high = high;
        m.found_base_lis = true;
        return true;
    }
    if (!m.found_table_lis) {
        m.table_high = high;
        m.found_table_lis = true;
        return true;
    }
    return false;
}

// The same for the `addi` / `ori` that gives the low half.
bool take_low_for_table(TableMatch& m, std::uint32_t w) {
    const bool reuse = m.found_add && m.table_reg == m.base_reg;
    if (reuse && !m.found_base_addi) {
        m.base_low = d_uimm(w);
        m.found_base_addi = true;
        return true;
    }
    if (!m.found_table_addi) {
        m.table_low = d_uimm(w);
        m.found_table_addi = true;
        return true;
    }
    return false;
}

}  // namespace

std::optional<JumpTable> detect_one(const GuestMemory& memory, std::uint32_t bctr_addr) {
    TableMatch m;

    for (std::uint32_t i = 1; i <= kMaxScanBack; ++i) {
        const auto addr = before(bctr_addr, i);
        if (!addr) break;
        const std::optional<std::uint32_t> word = memory.u32(*addr);
        if (!word) break;
        const std::uint32_t w = *word;

        if (is_function_boundary(w)) {
            // an adjacent bctr is stepped over while the table's lis is still being hunted
            if (is_bcctr_any(w) && m.found_load && !m.found_table_lis) continue;
            break;
        }

        // 1. mtctr rX
        if (!m.found_mtctr) {
            if (const auto reg = mtctr_source(w)) {
                m.found_mtctr = true;
                m.ctr_reg = reg;
            }
            continue;
        }

        // 2a. add rCTR, rBase, rOffset (the offset-based kinds)
        if (!m.found_add && !m.found_load && is_add(w) && x_rt(w) == m.ctr_reg) {
            m.base_reg = x_ra(w);
            m.offset_reg = x_rb(w);
            m.found_add = true;
            continue;
        }

        // 2b. lwzx rCTR -> an absolute table (RA = table, RB = index, tentatively)
        if (!m.found_add && !m.found_load && is_lwzx(w) && x_rt(w) == m.ctr_reg) {
            m.type = TableType::Abs;
            m.table_reg = x_ra(w);
            m.index_reg = x_rb(w);
            m.found_load = true;
            continue;
        }

        // the load of an offset table: lbzx (byte or computed) or lhzx (short)
        if (m.found_add && !m.found_load) {
            if (is_lbzx(w) && x_rt(w) == m.offset_reg) {
                m.table_reg = x_ra(w);
                m.index_reg = x_rb(w);
                m.found_load = true;
                if (!m.found_shift) m.type = TableType::ByteOff;
                continue;
            }
            if (is_lhzx(w) && x_rt(w) == m.offset_reg) {
                m.type = TableType::ShortOff;
                m.table_reg = x_ra(w);
                m.index_reg = x_rb(w);
                m.found_load = true;
                continue;
            }
        }

        // 3. rlwinm: the index scaling (walk the index register back) or the offset's shift (a computed table)
        if (!m.found_shift && is_rlwinm(w)) {
            if (m.found_load && m_ra(w) == m.index_reg) {
                m.index_reg = m_rs(w);
                m.found_shift = true;
                continue;
            }
            if (m.found_add && m_ra(w) == m.offset_reg && m.type != TableType::ShortOff) {
                m.shift = m_sh(w);
                m.type = TableType::Computed;
                m.found_shift = true;
                m.offset_reg = m_rs(w);
                continue;
            }
        }

        // 4. lis / addi for the TABLE address (also: the base when the table register is the base register, and the
        //    RA/RB swap when the load's operands were guessed the wrong way round)
        if (m.found_load) {
            if (is_lis(w) && d_rt(w) == m.table_reg) {
                if (take_lis_for_table(m, w)) continue;
            } else if (!m.found_table_lis && is_lis(w) && d_rt(w) == m.index_reg) {
                std::swap(m.table_reg, m.index_reg);
                m.table_high = d_uimm(w) << 16;
                m.found_table_lis = true;
                continue;
            }
            if ((is_addi(w) || is_ori(w)) && d_rt(w) == m.table_reg) {
                if (take_low_for_table(m, w)) continue;
            } else if (!m.found_table_addi && (is_addi(w) || is_ori(w)) && d_rt(w) == m.index_reg) {
                std::swap(m.table_reg, m.index_reg);
                m.table_low = d_uimm(w);
                m.found_table_addi = true;
                continue;
            }
        }

        // 5. lis / addi for the BASE address (offset-based kinds without register reuse), with the same swap
        if (m.found_add) {
            if (!m.found_base_lis && is_lis(w)) {
                if (d_rt(w) == m.base_reg) {
                    m.base_high = d_uimm(w) << 16;
                    m.found_base_lis = true;
                    continue;
                }
                if (d_rt(w) == m.offset_reg) {
                    std::swap(m.base_reg, m.offset_reg);
                    m.base_high = d_uimm(w) << 16;
                    m.found_base_lis = true;
                    continue;
                }
            }
            if (!m.found_base_addi && (is_addi(w) || is_ori(w))) {
                if (d_rt(w) == m.base_reg) {
                    m.base_low = d_uimm(w);
                    m.found_base_addi = true;
                    continue;
                }
                if (d_rt(w) == m.offset_reg) {
                    std::swap(m.base_reg, m.offset_reg);
                    m.base_low = d_uimm(w);
                    m.found_base_addi = true;
                    continue;
                }
            }
        }

        // complete?
        const bool table_complete = m.found_table_lis && m.found_table_addi;
        const bool base_complete = !m.found_add || (m.found_base_lis && m.found_base_addi);
        if (m.found_mtctr && m.found_load && table_complete && base_complete) break;
    }

    if (!(m.found_mtctr && m.found_load && m.found_table_lis && m.found_table_addi)) return std::nullopt;
    if (m.found_add && !(m.found_base_lis && m.found_base_addi)) return std::nullopt;

    const std::uint32_t table_addr = m.table_high + static_cast<std::uint32_t>(s16(m.table_low));
    if (!memory.u8(table_addr)) return std::nullopt;

    const std::uint32_t count = scan_bounds(memory, bctr_addr, *m.index_reg);
    std::vector<std::uint32_t> targets = read_targets(m, count, memory);
    if (targets.size() < 2) return std::nullopt;
    return JumpTable{*m.index_reg, std::move(targets)};
}

std::map<std::uint32_t, JumpTable> detect_jump_tables(std::span<const Instruction> insns, const GuestMemory& memory) {
    std::map<std::uint32_t, JumpTable> out;
    for (const Instruction& insn : insns) {
        if (!is_bctr(insn.word)) continue;
        if (auto table = detect_one(memory, insn.va)) out.insert_or_assign(insn.va, std::move(*table));
    }
    return out;
}

}  // namespace cb::recompile::jumptable

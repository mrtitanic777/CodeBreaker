// codebreaker/recompile/jumptable.cpp -- typed C++ port of codebreaker.recompile.jumptable (see jumptable.hpp).

#include "codebreaker/recompile/jumptable.hpp"

namespace codebreaker::recompile::jumptable {

const int64_t MAX_SCAN_BACK = 64;

// ---- PPC instruction field decode (big-endian word) -----------------------
int64_t _op(int64_t w) { return (w >> 26) & 0x3F; }
int64_t _xo(int64_t w) { return (w >> 1) & 0x3FF; }  // X/XO extended opcode (bits 1-10)
// D-form
int64_t _d_rt(int64_t w) { return (w >> 21) & 0x1F; }
int64_t _d_ra(int64_t w) { return (w >> 16) & 0x1F; }
int64_t _d_uimm(int64_t w) { return w & 0xFFFF; }
// X-form / XO-form
int64_t _x_rt(int64_t w) { return (w >> 21) & 0x1F; }
int64_t _x_ra(int64_t w) { return (w >> 16) & 0x1F; }
int64_t _x_rb(int64_t w) { return (w >> 11) & 0x1F; }
// M-form (rlwinm): RS=src(21-25), RA=dest(16-20), SH(11-15), MB(6-10), ME(1-5)
int64_t _m_rs(int64_t w) { return (w >> 21) & 0x1F; }
int64_t _m_ra(int64_t w) { return (w >> 16) & 0x1F; }
int64_t _m_sh(int64_t w) { return (w >> 11) & 0x1F; }
int64_t _m_mb(int64_t w) { return (w >> 6) & 0x1F; }
int64_t _m_me(int64_t w) { return (w >> 1) & 0x1F; }

// ---- opcode predicates (match decode_instruction's Opcode:: classification) --
std::optional<int64_t> _is_mtctr(int64_t w) {
    if (_op(w) != 31 || _xo(w) != 467) {
        return std::nullopt;
    }
    int64_t spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5);
    if (spr == 9) {
        return _x_rt(w);
    }
    return std::nullopt;
}

bool _is_mflr(int64_t w) {
    if (_op(w) != 31 || _xo(w) != 339) {
        return false;
    }
    int64_t spr = ((w >> 16) & 0x1F) | (((w >> 11) & 0x1F) << 5);
    return spr == 8;
}

bool _is_add(int64_t w) { return _op(w) == 31 && _xo(w) == 266; }
bool _is_lwzx(int64_t w) { return _op(w) == 31 && _xo(w) == 23; }
bool _is_lbzx(int64_t w) { return _op(w) == 31 && _xo(w) == 87; }
bool _is_lhzx(int64_t w) { return _op(w) == 31 && _xo(w) == 279; }
bool _is_rlwinm(int64_t w) { return _op(w) == 21; }
bool _is_lis(int64_t w) { return _op(w) == 15 && _d_ra(w) == 0; }  // addis rX,r0,imm
bool _is_addi(int64_t w) { return _op(w) == 14; }
bool _is_ori(int64_t w) { return _op(w) == 24; }
bool _is_bctr(int64_t w) { return _op(w) == 19 && _xo(w) == 528 && (w & 1) == 0; }
bool _is_bcctr_any(int64_t w) { return _op(w) == 19 && _xo(w) == 528; }  // bctr or bctrl
bool _is_b(int64_t w) { return _op(w) == 18; }                           // b / ba
bool _is_blr(int64_t w) {
    // bclr (op19,xo16) with BO=20 (unconditional) -> blr
    return _op(w) == 19 && _xo(w) == 16 && ((w >> 21) & 0x1F) == 20;
}
bool _is_cmpli(int64_t w) { return _op(w) == 10; }  // cmplwi/cmpldi
bool _is_cmpi(int64_t w) { return _op(w) == 11; }   // cmpwi/cmpdi
bool _is_stwu(int64_t w) { return _op(w) == 37; }

bool _is_function_boundary(int64_t w) {
    // Port of is_function_boundary().
    if (w == 0) {
        return true;
    }
    if (_is_blr(w)) {
        return true;
    }
    if (_is_bcctr_any(w)) {
        return true;
    }
    if (_is_b(w)) {
        return true;
    }
    if (_is_mflr(w)) {
        return true;
    }
    if (_is_stwu(w) && _d_ra(w) == 1 && _d_rt(w) == 1) {  // stwu r1,-N(r1)
        return true;
    }
    return false;
}

int64_t _s16(int64_t v) { return (v & 0x8000) ? v - 0x10000 : v; }

std::tuple<int64_t, int64_t> _scan_bounds(const Reader& rd_u32, int64_t bctr_addr, int64_t expected_index_reg) {
    // Port of scanForBounds -> (max_entries, default_target).
    int64_t max_entries = 0;
    int64_t default_target = 0;
    int64_t cr_field = 0xFF;
    for (int64_t i = 1; i <= MAX_SCAN_BACK; ++i) {
        int64_t addr = bctr_addr - i * 4;
        if (addr < 4) {
            break;
        }
        std::optional<int64_t> ow = rd_u32(addr);
        if (!ow || *ow == 0) {
            break;
        }
        int64_t w = *ow;
        // conditional branch testing GT bit -> capture CR field + default target
        if (cr_field == 0xFF && _op(w) == 16) {  // bc family
            int64_t bi = (w >> 16) & 0x1F;
            if ((bi & 0x3) == 1) {  // tests GT bit (bgt/ble)
                cr_field = (bi >> 2) & 0x7;
                int64_t bd = w & 0xFFFC;
                if (bd & 0x8000) {
                    bd -= 0x10000;
                }
                int64_t aa = (w >> 1) & 1;
                default_target = aa ? (bd & 0xFFFFFFFF) : ((addr + bd) & 0xFFFFFFFF);
            }
        }
        // clrlwi/rlwinm implicit mask on the index register -> 2^(32-mb) entries
        if (_is_rlwinm(w) && _m_ra(w) == expected_index_reg) {
            int64_t sh = _m_sh(w), mb = _m_mb(w), me = _m_me(w);
            if (sh == 0 && me == 31 && 0 < mb && mb < 32) {
                int64_t cnt = int64_t(1) << (32 - mb);
                if (2 <= cnt && cnt <= 256) {
                    return {cnt, default_target};
                }
            }
        }
        // cmpli/cmpi: count = imm + 1
        if (_is_cmpli(w) || _is_cmpi(w)) {
            int64_t cmp_cr = _d_rt(w) >> 2;
            int64_t cmp_ra = _d_ra(w);
            int64_t cmp_imm = _d_uimm(w);
            if (cmp_imm <= 1) {
                continue;
            }
            bool reg_ok = (cmp_ra == expected_index_reg);
            bool cr_ok = (cr_field != 0xFF && cmp_cr == cr_field);
            if (reg_ok || (cr_ok && cmp_imm > 1)) {
                max_entries = cmp_imm + 1;
                if (reg_ok) {
                    return {max_entries, default_target};
                }
            }
        }
    }
    return {max_entries, default_target};
}

py::list<int64_t> _read_targets(const TableMatch& m, int64_t count, const Reader& rd_u8, const Reader& rd_u16,
                                const Reader& rd_u32, const IsExec& is_exec) {
    // Port of read_table_entries.
    int64_t table = (m.table_high + _s16(m.table_low)) & 0xFFFFFFFF;
    int64_t basev = (m.base_high + _s16(m.base_low)) & 0xFFFFFFFF;
    TableType ttype = m.type;
    int64_t shift = m.shift;
    py::list<int64_t> out;
    int64_t i = 0;
    while (count == 0 || i < count) {
        int64_t target;
        if (ttype == TableType::Abs) {
            std::optional<int64_t> t = rd_u32(table + i * 4);
            if (!t) {
                break;
            }
            target = *t;
        } else if (ttype == TableType::Computed) {
            std::optional<int64_t> b = rd_u8(table + i);
            if (!b) {
                break;
            }
            target = (basev + (*b << shift)) & 0xFFFFFFFF;
        } else if (ttype == TableType::ByteOff) {
            std::optional<int64_t> b = rd_u8(table + i);
            if (!b) {
                break;
            }
            target = (basev + *b) & 0xFFFFFFFF;
        } else {  // shortoff
            std::optional<int64_t> h = rd_u16(table + i * 2);
            if (!h) {
                break;
            }
            target = (basev + *h) & 0xFFFFFFFF;
        }
        if (target & 3) {
            break;
        }
        if (target == 0 && ttype == TableType::Abs) {
            break;
        }
        if (!is_exec(target)) {
            if (ttype == TableType::Abs) {
                break;
            }
            i += 1;
            continue;
        }
        out.append(target);
        i += 1;
    }
    return out;
}

std::optional<JumpTable> _detect_one(const Reader& rd_u8, const Reader& rd_u16, const Reader& rd_u32,
                                     const IsExec& is_exec, int64_t bctr_addr) {
    // Port of FunctionScanner::detect_jump_table for a single bctr address.
    TableMatch m;

    for (int64_t i = 1; i <= MAX_SCAN_BACK; ++i) {
        int64_t addr = bctr_addr - i * 4;
        if (addr < 4) {
            break;
        }
        std::optional<int64_t> ow = rd_u32(addr);
        if (!ow) {
            break;
        }
        int64_t w = *ow;

        if (_is_function_boundary(w)) {
            // allow continuing past an adjacent bctr while still hunting the lis
            if (_is_bcctr_any(w) && m.found_load && !m.found_table_lis) {
                continue;
            }
            break;
        }

        // Step 1: mtctr rX
        if (!m.found_mtctr) {
            if (_is_mtctr(w).has_value()) {
                m.found_mtctr = true;
                m.ctr_reg = *_is_mtctr(w);
            }
            continue;
        }

        // Step 2a: add rT(=ctr), rBase, rOffset  (offset-based types)
        if (!m.found_add && !m.found_load && _is_add(w)) {
            if (_x_rt(w) == m.ctr_reg) {
                m.base_reg = _x_ra(w);
                m.offset_reg = _x_rb(w);
                m.found_add = true;
                continue;
            }
        }

        // Step 2b: lwzx rT(=ctr) -> ABSOLUTE (RA=table, RB=index, tentative)
        if (!m.found_add && !m.found_load && _is_lwzx(w)) {
            if (_x_rt(w) == m.ctr_reg) {
                m.type = TableType::Abs;
                m.table_reg = _x_ra(w);
                m.index_reg = _x_rb(w);
                m.found_load = true;
                continue;
            }
        }

        // offset-based load: lbzx (byteoff/computed) or lhzx (shortoff)
        if (m.found_add && !m.found_load) {
            if (_is_lbzx(w) && _x_rt(w) == m.offset_reg) {
                m.table_reg = _x_ra(w);
                m.index_reg = _x_rb(w);
                m.found_load = true;
                if (!m.found_shift) {
                    m.type = TableType::ByteOff;
                }
                continue;
            }
            if (_is_lhzx(w) && _x_rt(w) == m.offset_reg) {
                m.type = TableType::ShortOff;
                m.table_reg = _x_ra(w);
                m.index_reg = _x_rb(w);
                m.found_load = true;
                continue;
            }
        }

        // Step 3: rlwinm -- index scaling (back-track index_reg) or offset shift (COMPUTED)
        if (!m.found_shift && _is_rlwinm(w)) {
            if (m.found_load && _m_ra(w) == m.index_reg) {
                m.index_reg = _m_rs(w);
                m.found_shift = true;
                continue;
            }
            if (m.found_add && _m_ra(w) == m.offset_reg && m.type != TableType::ShortOff) {
                m.shift = _m_sh(w);
                m.type = TableType::Computed;
                m.found_shift = true;
                m.offset_reg = _m_rs(w);
                continue;
            }
        }

        // Step 4: lis/addi for the TABLE address (+ RA/RB swap, + register reuse)
        if (m.found_load) {
            bool register_reuse = m.found_add && (m.table_reg == m.base_reg);
            // lis matching table_reg
            if (_is_lis(w) && _d_rt(w) == m.table_reg) {
                if (register_reuse) {
                    if (!m.found_base_lis) {
                        m.base_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                        m.found_base_lis = true;
                        continue;
                    } else if (!m.found_table_lis) {
                        m.table_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                        m.found_table_lis = true;
                        continue;
                    }
                } else if (!m.found_table_lis) {
                    m.table_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                    m.found_table_lis = true;
                    continue;
                }
            }
            // lis matching index_reg -> guessed wrong, swap table/index
            else if (!m.found_table_lis && _is_lis(w) && _d_rt(w) == m.index_reg) {
                std::swap(m.table_reg, m.index_reg);
                m.table_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                m.found_table_lis = true;
                continue;
            }
            // addi/ori matching table_reg
            if ((_is_addi(w) || _is_ori(w)) && _d_rt(w) == m.table_reg) {
                if (register_reuse) {
                    if (!m.found_base_addi) {
                        m.base_low = _d_uimm(w);
                        m.found_base_addi = true;
                        continue;
                    } else if (!m.found_table_addi) {
                        m.table_low = _d_uimm(w);
                        m.found_table_addi = true;
                        continue;
                    }
                } else if (!m.found_table_addi) {
                    m.table_low = _d_uimm(w);
                    m.found_table_addi = true;
                    continue;
                }
            }
            // addi/ori matching index_reg -> guessed wrong, swap
            else if (!m.found_table_addi && (_is_addi(w) || _is_ori(w)) && _d_rt(w) == m.index_reg) {
                std::swap(m.table_reg, m.index_reg);
                m.table_low = _d_uimm(w);
                m.found_table_addi = true;
                continue;
            }
        }

        // Step 5: lis/addi for the BASE address (offset-based, non-reuse), + swap
        if (m.found_add) {
            if (!m.found_base_lis && _is_lis(w)) {
                if (_d_rt(w) == m.base_reg) {
                    m.base_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                    m.found_base_lis = true;
                    continue;
                } else if (_d_rt(w) == m.offset_reg) {
                    std::swap(m.base_reg, m.offset_reg);
                    m.base_high = (_d_uimm(w) << 16) & 0xFFFFFFFF;
                    m.found_base_lis = true;
                    continue;
                }
            }
            if (!m.found_base_addi && (_is_addi(w) || _is_ori(w))) {
                if (_d_rt(w) == m.base_reg) {
                    m.base_low = _d_uimm(w);
                    m.found_base_addi = true;
                    continue;
                } else if (_d_rt(w) == m.offset_reg) {
                    std::swap(m.base_reg, m.offset_reg);
                    m.base_low = _d_uimm(w);
                    m.found_base_addi = true;
                    continue;
                }
            }
        }

        // complete?
        bool table_complete = m.found_table_lis && m.found_table_addi;
        bool base_complete = (!m.found_add) || (m.found_base_lis && m.found_base_addi);
        if (m.found_mtctr && m.found_load && table_complete && base_complete) {
            break;
        }
    }

    if (!(m.found_mtctr && m.found_load && m.found_table_lis && m.found_table_addi)) {
        return std::nullopt;
    }
    if (m.found_add && !(m.found_base_lis && m.found_base_addi)) {
        return std::nullopt;
    }

    int64_t table_addr = (m.table_high + _s16(m.table_low)) & 0xFFFFFFFF;
    if (!rd_u8(table_addr)) {
        return std::nullopt;
    }

    int64_t cnt = std::get<0>(_scan_bounds(rd_u32, bctr_addr, m.index_reg));
    py::list<int64_t> targets = _read_targets(m, cnt, rd_u8, rd_u16, rd_u32, is_exec);
    if (py::len(targets) < 2) {
        return std::nullopt;
    }
    return JumpTable{m.index_reg, targets};
}

py::dict<int64_t, JumpTable> detect_jump_tables(const py::list<Insn>& insns, int64_t func_lo, int64_t func_hi,
                                                const Reader& rd_u8, const Reader& rd_u16, const Reader& rd_u32,
                                                const IsExec& is_exec) {
    (void)func_lo;
    (void)func_hi;
    py::dict<int64_t, JumpTable> out;
    for (const Insn& insn : insns) {
        int64_t va = std::get<0>(insn);
        const std::variant<int64_t, py::bytes>& w = std::get<1>(insn);
        int64_t wi;
        if (std::holds_alternative<int64_t>(w)) {
            wi = std::get<int64_t>(w);
        } else {
            // int.from_bytes(w, "big")
            const py::bytes& b = std::get<py::bytes>(w);
            uint64_t acc = 0;
            int64_t n = py::len(b);
            for (int64_t k = 0; k < n; ++k) {
                if ((acc >> 56) != 0) {
                    throw py::OverflowError("int too large for int64");
                }
                acc = (acc << 8) | static_cast<uint64_t>(b[k]);
            }
            if (acc >> 63) {
                throw py::OverflowError("int too large for int64");
            }
            wi = static_cast<int64_t>(acc);
        }
        if (!_is_bctr(wi)) {
            continue;
        }
        std::optional<JumpTable> res = _detect_one(rd_u8, rd_u16, rd_u32, is_exec, va);
        if (res) {
            out.set(va, *res);
        }
    }
    return out;
}

}  // namespace codebreaker::recompile::jumptable

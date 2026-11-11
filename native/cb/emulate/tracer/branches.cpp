// branches.cc -- the branch log (trace_function(branches=True), emulator._branch_hook): for each
// conditional jump of the function itself, which way it went and what the comparison before it
// compared -- the values, and the memory each came from (a register's: where it was last loaded
// from in this function, if it was not written since).
#include "cb/emulate/tracer/run.hpp"

namespace cb::emulate::tracer {

static bool mask_of(int size, uint64_t &mask) {
    if (size < 0 || size > 8)
        return false;                          // (no integer compare in 32-bit code is wider)
    mask = size == 8 ? ~0ull : ((1ull << (8 * size)) - 1);
    return true;
}

static uint64_t address_of(uc_engine *uc, const Operand &m) {
    int64_t a = m.disp;
    if (m.base >= 0)
        a += int64_t(reg(uc, m.base));
    if (m.index >= 0)
        a += int64_t(reg(uc, m.index)) * m.scale;
    return uint64_t(a) & 0xFFFFFFFFull;
}

// _operand_value: false where Python's raises
static bool value_of(Run *r, const Operand &op, uint64_t &value, bool &has_at, uint64_t &at) {
    uint64_t mask;
    if (!mask_of(op.size, mask))
        return false;
    has_at = false;
    if (op.kind == OP_IMM) {
        value = uint64_t(op.imm) & mask;
        return true;
    }
    if (op.kind == OP_REG) {
        auto it = r->loaded.find(op.parent);
        if (it != r->loaded.end() && it->second.second == op.size) {
            has_at = true;
            at = it->second.first;
        }
        value = reg(r->uc, op.uc_reg) & mask;
        return true;
    }
    uint64_t a = address_of(r->uc, op);
    uint8_t buf[8];
    if (ucl().mem_read(r->uc, a, buf, size_t(op.size)) != UC_ERR_OK)
        return false;
    value = little(buf, size_t(op.size));
    has_at = true;
    at = a;
    return true;
}

void hook_branch(uc_engine *uc, uint64_t address, uint32_t, void *user) {
    Run *r = static_cast<Run *>(user);
    if (r->pend) {
        r->pend = false;
        if (int64_t(r->branch_log.size() / CBT_BRANCH_WORDS) < BRANCH_CAP &&
            (address == r->pend_target || address == r->pend_next)) {
            const Compare &c = r->pend_cmp;
            uint64_t row[CBT_BRANCH_WORDS] = {r->pend_addr, address == r->pend_target, c.has, uint64_t(c.mn),
                                              uint64_t(c.size), c.a, c.b, c.has_at_a, c.at_a, c.has_at_b, c.at_b};
            r->branch_log.insert(r->branch_log.end(), row, row + CBT_BRANCH_WORDS);
        }
    }
    const Branchy &d = r->cache->branchy(uc, r->sha_id, address);
    if (d.kind == B_CMP) {
        Compare c;
        if (value_of(r, d.ops[0], c.a, c.has_at_a, c.at_a) && value_of(r, d.ops[1], c.b, c.has_at_b, c.at_b)) {
            c.has = true;
            c.mn = d.mn;
            c.size = d.cmp_size;
            r->cmp = c;
        } else {
            r->cmp = Compare();
        }
    } else if (d.kind == B_JCC) {
        if (r->func_lo <= d.target && d.target < r->func_hi && d.target != d.next) {
            r->pend = true;
            r->pend_addr = address;
            r->pend_target = d.target;
            r->pend_next = d.next;
            r->pend_cmp = r->cmp;
        }
    } else if (d.kind == B_OTHER) {
        r->cmp = Compare();
    }
    for (int w : d.written)
        r->loaded.erase(w);
    if (d.has_load && d.load_parent >= 0)
        r->loaded[d.load_parent] = {address_of(uc, d.load_src), d.load_size};
}

}  // namespace cb::emulate::tracer

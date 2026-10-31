// memory.cc -- the memory side of a run: _memory_hooks' hook_mem (reads and writes, a write
// with what was there before) and hook_unmapped (a page mapped on demand, the fault kept as
// evidence), trace_function's lean hook_write and hook_intr, and putting a kept machine's
// pages back after the run.
#include "run.h"

namespace cbt {

void Run::save_page(uint64_t base) {
    if (saved_keys.count(base))
        return;
    std::vector<uint8_t> data(PAGE);
    if (UC.mem_read(uc, base, data.data(), PAGE) != UC_ERR_OK)
        return;                                // not there yet: mapping it notes that
    saved_keys.insert(base);
    saved_base.push_back(base);
    saved_data.push_back(std::move(data));
    saved_none.push_back(false);
}

void hook_mem(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user) {
    Run *r = static_cast<Run *>(user);
    uint64_t span = uint64_t(size > 1 ? size : 1);
    if (r->saving && type == UC_MEM_WRITE) {   // what the page held, to put back after
        uint64_t a = address & ~(PAGE - 1), b = (address + span - 1) & ~(PAGE - 1);
        r->save_page(a);
        if (b != a)
            r->save_page(b);
    }
    // a `rep` flood over mapped memory: one instruction, millions of accesses -- bounded by count
    r->memops = (r->memops < 0 ? 0 : r->memops) + 1;
    if (r->memops > MAX_MEMOPS) {
        r->truncated = r->governed = true;
        r->stop();
        return;
    }
    bool is_write = type == UC_MEM_WRITE;
    if (!is_write && !r->reads)
        return;
    if (is_write) {
        for (Static *s : r->statics)           // code written over: decode it from memory
            if (s->lo <= address && address < s->hi)
                r->dirty = true;
        int64_t seen = r->writes_seen = (r->writes_seen < 0 ? 0 : r->writes_seen) + 1;
        if (seen > MAX_MEM || (r->quiet && r->quiet_lo <= address && address < r->quiet_hi))
            return;
    }
    std::vector<uint64_t> &bucket = is_write ? r->writes_rec : r->reads_rec;
    if (int64_t(bucket.size() / CBT_MEM_WORDS) < MAX_MEM) {
        uint64_t pc = reg(uc, UC_X86_REG_EIP);
        uint64_t v = 0, old = 0;
        bool none = false;
        size_t n = size < 8 ? size_t(size < 0 ? 0 : size) : 8;
        uint8_t buf[8];
        if (is_write) {
            v = uint64_t(value) & 0xFFFFFFFFull;
            old = UC.mem_read(uc, address, buf, n) == UC_ERR_OK ? little(buf, n) : 0;
        } else if (UC.mem_read(uc, address, buf, n) == UC_ERR_OK) {
            v = little(buf, n);                // the value read (Unicorn passes 0 on a read)
        } else {
            none = true;
        }
        uint64_t row[CBT_MEM_WORDS] = {pc, address, uint64_t(int64_t(size)), v, none, uint64_t(r->insn_count),
                                       is_write ? 1u : 0u, old};
        bucket.insert(bucket.end(), row, row + CBT_MEM_WORDS);
    }
}

// LEAN: writes as (address, size, value, pc), nothing asked of the emulator for each; what a
// byte held before the run is read once per page.
void hook_write(uc_engine *uc, uc_mem_type, uint64_t address, int size, int64_t value, void *user) {
    Run *r = static_cast<Run *>(user);
    uint64_t base = address & ~(PAGE - 1);
    uint64_t span = uint64_t(size > 1 ? size : 1);
    if (!r->pages0_keys.count(base) || !r->pages0_keys.count((address + uint64_t(int64_t(size)) - 1) & ~(PAGE - 1))) {
        uint64_t two[2] = {base, (address + span - 1) & ~(PAGE - 1)};
        for (uint64_t b : two) {
            if (r->pages0_keys.count(b))
                continue;
            size_t at = r->pages0_data.size();
            r->pages0_data.resize(at + PAGE);
            if (UC.mem_read(uc, b, r->pages0_data.data() + at, PAGE) == UC_ERR_OK) {
                if (r->saving && !r->saved_keys.count(b)) {
                    r->saved_keys.insert(b);
                    r->saved_base.push_back(b);
                    r->saved_data.emplace_back(r->pages0_data.begin() + at, r->pages0_data.begin() + at + PAGE);
                    r->saved_none.push_back(false);
                }
            } else {
                std::fill(r->pages0_data.begin() + at, r->pages0_data.end(), 0);   // reads as zeros
            }
            r->pages0_keys.insert(b);
            r->pages0_base.push_back(b);
        }
    }
    for (Static *s : r->statics)
        if (s->lo <= address && address < s->hi)
            r->dirty = true;
    r->lean_seen++;
    if (r->lean_seen <= MAX_MEM) {
        uint64_t pc = (r->switched || !r->fast) ? reg(uc, UC_X86_REG_EIP) : 0;
        uint64_t row[CBT_LEAN_WORDS] = {address, uint64_t(int64_t(size)), uint64_t(value) & 0xFFFFFFFFull, pc};
        r->writes_rec.insert(r->writes_rec.end(), row, row + CBT_LEAN_WORDS);
    }
}

// An unmapped access: map the page(s) it spans, zeroed, and retry; keep the fault.  Two bails,
// by count: too many pages mapped, and faults that map nothing new.
bool hook_unmapped(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t, void *user) {
    Run *r = static_cast<Run *>(user);
    uint64_t span = uint64_t(size > 1 ? size : 1);
    uint64_t lo = address & ~(PAGE - 1), hi = (address + span - 1) & ~(PAGE - 1);
    int newly = 0;
    for (uint64_t base = lo; base < hi + PAGE; base += PAGE) {
        if (r->mapped.count(base))
            continue;
        r->gov_n++;                            // maps count toward the per-function budget
        if (int64_t(r->mapped.size()) >= MAX_MAPS || (r->gov_ceil && r->gov_n > r->gov_ceil)) {
            r->truncated = r->governed = true;
            r->stop();
            return false;
        }
        if (UC.mem_map(uc, base, PAGE, UC_PROT_ALL) != UC_ERR_OK)
            return false;                      // truly can't map -> let it fault
        r->mapped.insert(base);
        newly++;
        if (r->saving && !r->saved_keys.count(base)) {
            r->saved_keys.insert(base);
            r->saved_base.push_back(base);
            r->saved_data.emplace_back();
            r->saved_none.push_back(true);
        }
    }
    uint64_t kind = type == UC_MEM_READ_UNMAPPED ? 0 : type == UC_MEM_WRITE_UNMAPPED ? 1
                  : type == UC_MEM_FETCH_UNMAPPED ? 2 : 3;
    if (int64_t(r->lazy.size() / CBT_LAZY_WORDS) < MAX_MEM) {
        uint64_t row[CBT_LAZY_WORDS] = {reg(uc, UC_X86_REG_EIP), address, lo, kind, uint64_t(int64_t(size))};
        r->lazy.insert(r->lazy.end(), row, row + CBT_LAZY_WORDS);
    }
    if (newly == 0) {                          // couldn't make progress -> guard a loop
        r->stuck = (r->stuck < 0 ? 0 : r->stuck) + 1;
        if (r->stuck > 64) {
            r->truncated = r->governed = true;
            r->stop();
            return false;
        }
    } else {
        r->stuck = 0;
    }
    return true;
}

// INT 20h ; dd service: a VxD service call, stepped over; any other interrupt stops the run.
void hook_intr(uc_engine *uc, uint32_t intno, void *user) {
    Run *r = static_cast<Run *>(user);
    if (intno == 0x20) {
        uint64_t eip = reg(uc, UC_X86_REG_EIP);
        r->vxd.push_back(eip);
        set_reg(uc, UC_X86_REG_EIP, eip + 4);
        return;
    }
    r->stop();
}

int restore(Run *r) {
    for (size_t i = 0; i < r->saved_base.size(); i++) {
        uc_err e = r->saved_none[i] ? UC.mem_unmap(r->uc, r->saved_base[i], PAGE)
                                    : UC.mem_write(r->uc, r->saved_base[i], r->saved_data[i].data(), PAGE);
        if (e != UC_ERR_OK)
            return int(e);
    }
    return 0;
}

}  // namespace cbt

// recorder.cc -- the instruction side of a run: trace_function's hook_code (one instruction at a
// time), hook_block (a whole straight run of the file's code at once, `fast`) and settle.
// Each step is the Python's, in the same order, so the record and the run are the same.
#include <algorithm>
#include <mutex>

#include "run.h"

namespace cbt {

void code_step(Run *r, uint64_t address, uint32_t csize) {
    uc_engine *uc = r->uc;
    if (r->skip_set) {
        uint64_t done = r->skip;
        r->skip_set = false;
        if (done == address)
            return;
    }
    // the per-function instruction governor (emulator._GOV)
    r->gov_n++;
    if (r->gov_ceil && r->gov_n > r->gov_ceil) {
        r->truncated = r->governed = true;
        r->stop();
        return;
    }
    int64_t c = r->insn_count;
    if (c >= r->insn_cap) {
        r->truncated = true;
        r->stop();
        return;
    }
    r->insn_count = c + 1;

    // A landing in 00-padding (`add [eax], al`): a call through a pointer to nothing.  Stand in
    // for a return from it; stop a crawl through zeros.
    {
        bool failed = false, zero2 = false;
        if (csize == 2 || !(0 < csize && csize <= 15)) {
            uint8_t two[2];
            bool have = false;
            if (!r->dirty) {
                for (Static *s : r->statics) {
                    if (s->lo <= address && address < s->hi - 1) {
                        if (!r->init_page(address >> 12) && !r->init_page((address + 1) >> 12)) {
                            two[0] = s->image[address - s->lo];
                            two[1] = s->image[address - s->lo + 1];
                            have = true;
                        }
                        break;
                    }
                }
            }
            if (!have) {
                if (UC.mem_read(uc, address, two, 2) != UC_ERR_OK)
                    failed = true;
            }
            if (!failed)
                zero2 = two[0] == 0 && two[1] == 0;
        }
        if (failed) {
            r->zeros = 0;
        } else if (zero2) {
            r->zeros++;
            if (r->zeros == 2 && r->stop_wild) {
                r->bogus = 1;
                r->truncated = true;
                r->stop();
                return;
            }
            bool resumed = false;
            if (r->zeros == 2 && (r->bogus < 0 ? 0 : r->bogus) < 64) {
                uint64_t esp_now = reg(uc, UC_X86_REG_ESP);
                uint32_t ret_addr;
                if (read32(uc, esp_now, ret_addr)) {
                    set_reg(uc, UC_X86_REG_ESP, (esp_now + 4) & 0xFFFFFFFFull);
                    set_reg(uc, UC_X86_REG_EAX, 0);
                    set_reg(uc, UC_X86_REG_EIP, ret_addr);
                    r->bogus = (r->bogus < 0 ? 0 : r->bogus) + 1;
                    r->zeros = 0;
                    if (r->shadow)
                        sh_after_call(r->shadow, uc, false, 0);
                    return;
                }
                r->zeros = 0;                  // (the read failed: as Python's except)
                resumed = true;
            }
            if (!resumed && r->zeros > 16) {
                r->truncated = true;
                r->ran_into_zeros = true;
                r->stop();
                return;
            }
        } else {
            r->zeros = 0;
        }
    }

    // An import entered (its sentinel), or a landing pad reached: answer it with a stand-in.
    bool hit = false;
    int64_t code = 0;
    if (r->stub) {
        auto it = r->sentinels.find(address);
        if (it != r->sentinels.end()) {
            hit = true;
            code = it->second;
        }
    }
    if (!hit && r->pads && r->pads_lo <= address && address < r->pads_hi) {
        hit = true;
        code = -int64_t((address - r->pads_lo) / 4) - 1;
    }
    if (hit) {
        int got = stand_in(r, address, code);
        if (got != 1)
            return;
    }

    // The first instruction after a CALL is the callee's entry.
    if (r->pending >= 0) {
        Call &cr = r->calls[size_t(r->pending)];
        cr.state = 1;
        cr.resolved = address;
        cr.external = !(r->sec_lo <= address && address < r->sec_hi);
        uint64_t esp_c = reg(uc, UC_X86_REG_ESP);
        uint32_t a[6];
        bool ok = true;
        for (int i = 0; i < 6 && ok; i++)
            ok = read32(uc, esp_c + 4 + 4 * i, a[i]);
        if (ok) {
            cr.has_args = true;
            for (int i = 0; i < 6; i++)
                cr.args[i] = a[i];
            cr.ecx = reg(uc, UC_X86_REG_ECX);
            cr.edx = reg(uc, UC_X86_REG_EDX);
        }
        r->pending = -1;
    }
    if (int64_t(r->path.size()) < MAX_PATH_)
        r->path.push_back(address);
    if (!r->has_mnemonic(address)) {
        const InsnInfo *info = nullptr;
        Static *known = nullptr;
        const uint8_t *raw = nullptr;
        size_t n = std::min<size_t>(csize ? csize : 15, 15), rawn = 0;
        if (!r->dirty) {
            for (Static *s : r->statics) {
                if (s->lo <= address && address < s->hi) {
                    if (!r->init_page(address >> 12) && !r->init_page((address + 14) >> 12)) {
                        known = s;
                        std::lock_guard<std::mutex> g(cache_lock);
                        auto it = s->insns.find(address);
                        if (it != s->insns.end())
                            info = &it->second;
                        raw = s->image.data() + (address - s->lo);
                        rawn = std::min<size_t>(n, size_t(s->hi - address));
                    }
                    break;
                }
            }
        }
        if (!info) {
            uint8_t buf[15];
            if (!raw) {
                uc_err e = UC.mem_read(uc, address, buf, n);
                if (e != UC_ERR_OK) {          // (Python: the hook raises; the run stops on it)
                    r->hook_error = 2;
                    r->uc_err = e;
                    r->stop();
                    return;
                }
                raw = buf;
                rawn = n;
            }
            InsnInfo got;
            if (!decode_one(raw, rawn, address, got)) {
                got = InsnInfo();
                got.text = "?";
            }
            if (known) {
                std::lock_guard<std::mutex> g(cache_lock);
                info = &known->insns.emplace(address, got).first->second;
            } else {
                r->owned.push_back(got);
                info = &r->owned.back();
            }
        }
        r->add_mnemonic(address, &info->text);
        if (info->is_call) {
            Call cr;
            cr.pc = address;
            cr.has_target = info->has_target;
            cr.target = info->target;
            cr.into_func = info->has_target && r->func_lo <= info->target && info->target < r->func_hi;
            r->calls.push_back(cr);
            r->pending = int64_t(r->calls.size()) - 1;
        }
    }
    if (r->shadow)
        sh_step(r->shadow, uc, address, 0);
}

void hook_code(uc_engine *, uint64_t address, uint32_t size, void *user) {
    code_step(static_cast<Run *>(user), address, size);
}

// FAST: told of a whole straight run of the file's code at once.  Anything else goes to
// code_step one instruction at a time from there on (`switched`).
void hook_block(uc_engine *uc, uint64_t address, uint32_t size, void *user) {
    Run *r = static_cast<Run *>(user);
    if (r->switched)
        return;
    bool sentinel = r->sentinels.count(address) != 0;
    if ((r->stub && sentinel) || (r->pads && r->pads_lo <= address && address < r->pads_hi)) {
        r->last = nullptr;
        code_step(r, address, sentinel ? 2 : 1);
        if (r->hook_error)
            return;
        if (!r->truncated && reg(uc, UC_X86_REG_EIP) == address) {
            r->skip = address;                 // not returned from: go on with care
            r->skip_set = true;
            r->switched = true;
            r->stop();
        }
        return;
    }
    const Straight *info = nullptr;
    if (!r->dirty) {
        for (Static *s : r->statics) {
            if (s->lo <= address && address + size <= s->hi) {
                if (!r->init_page(address >> 12) && !r->init_page((address + size - 1) >> 12)) {
                    uint64_t key = address << 32 | size;
                    std::lock_guard<std::mutex> g(cache_lock);
                    auto it = s->blocks.find(key);
                    if (it == s->blocks.end()) {
                        Straight st;
                        straight_run(s->image.data() + (address - s->lo), size, address, st);
                        it = s->blocks.emplace(key, std::move(st)).first;
                    }
                    info = &it->second;
                }
                break;
            }
        }
    }
    int64_t c = r->insn_count;
    if (!info || !info->ok || info->zero || c + int64_t(info->addrs.size()) > r->insn_cap) {
        r->last = nullptr;
        r->switched = true;
        r->stop();
        return;
    }
    r->zeros = 0;
    r->insn_count = c + int64_t(info->addrs.size());
    uint64_t key = address << 32 | size;
    r->last = info;
    r->last_new.clear();
    if (r->seen_blocks.insert(key).second) {
        for (size_t i = 0; i < info->addrs.size(); i++) {
            uint64_t a = info->addrs[i];
            if (!r->has_mnemonic(a)) {
                r->add_mnemonic(a, &info->texts[i]);
                r->last_new.push_back(a);
            }
        }
    }
}

// A fast run that stopped inside a straight run: the instructions after the one it stopped at
// were counted on entry and did not happen.
void settle(Run *r) {
    if (r->fast && !r->switched && r->last) {
        uint64_t eip = reg(r->uc, UC_X86_REG_EIP);
        const Straight *info = r->last;
        if (std::find(info->addrs.begin(), info->addrs.end(), eip) != info->addrs.end()) {
            for (uint64_t a : info->addrs)
                if (a > eip)
                    r->insn_count--;
            for (uint64_t a : r->last_new)
                if (a > eip)
                    r->drop_mnemonic(a);
        }
        r->last = nullptr;
    }
}

}  // namespace cbt

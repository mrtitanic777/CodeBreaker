// driver_rec.cc -- the flat ring-0 driver recorder (codebreaker/emulate/driver.py `run`): the
// hooks of a VxD entry point's run under Unicorn, in C++.  Python still lays the regions out,
// sets the registers, starts the run (uc_emu_start to _RET) and reads the result; this attaches
// driver.run's hooks to Python's own Unicorn handle, in driver.run's order, and keeps the record
// they made.  Each step is the Python's, so the record and the run are the same.
//
// What stays Python: which register a service answers in (driver_services + the knowledge base),
// asked once per distinct service and kept for the run (the stand-in's resolver, cbt_drv_answers),
// and -- a run kept beside a shadow (the path explorer's) stays on the Python hooks for now.
#include <cstring>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/emulate/tracer/cbtrace.h"
#include "cb/emulate/tracer/libs.hpp"
#include "cb/emulate/tracer/shadow.hpp"

namespace cb::emulate::tracer {

// driver.py / emulator.py caps, repeated here.
static constexpr uint64_t D_PAGE = 0x1000;
static constexpr int64_t D_MAX_MEM = 40000;           // emulator._MAX_MEM
static constexpr int64_t D_MAX_MAPS = 512;            // emulator._MAX_MAPS
static constexpr int64_t D_MAX_MEMOPS = 3000000;      // emulator._MAX_MEMOPS
static constexpr int64_t D_MAX_COVER = 6000;          // driver.MAX_COVER
static constexpr int64_t D_MAX_OUTSIDE = 64;          // driver.MAX_OUTSIDE
static constexpr int64_t D_MAX_CALLS = 64;            // driver.MAX_CALLS (args kept per call)
static constexpr int64_t D_MAX_CALLS_RUN = 400;       // driver.MAX_CALLS_RUN
static constexpr int64_t D_MAX_MEMORY_STEPS = 200000; // driver.MAX_MEMORY_STEPS

enum { R_EAX = 0, R_EBX, R_ECX, R_EDX, R_ESI, R_EDI, R_EBP, R_ESP, R_EIP, R_EFLAGS };

struct DrvRun {
    uc_engine *uc = nullptr;
    int32_t ids[10] = {0};
    uint64_t vm_addr = 0, ret = 0, scratch_lo = 0, scratch_hi = 0;
    int32_t ins_in = 0, ins_out = 0;
    int64_t cap = 0, service_eax = 0, service_carry = 0, gov_n = 0, gov_ceil = 0;
    std::vector<std::pair<uint64_t, uint64_t>> spans;
    cbt_drv_answers answers = nullptr;
    cbt_drv_answers_ctx answers_ctx = nullptr;   // the same question with a context pointer (the typed face, native_driver)
    void *answers_user = nullptr;
    Shadow *shadow = nullptr;
    std::unordered_map<uint64_t, int32_t> answer_cache;   // device<<32 | service -> place (-2 unknown)

    // the record
    int64_t exit_code = 0, exit_int = 0;
    int64_t insns = 0;
    bool truncated = false, governed = false;
    int64_t outside = 0, memops = -1, writes_seen = -1, stuck = -1;
    int64_t hook_error = 0, uc_err = 0;
    std::vector<uint64_t> cover;
    std::unordered_set<uint64_t> cover_set;
    std::vector<uint64_t> calls, ports, reads, writes, lazy;
    int64_t ncalls = 0, nports = 0;
    std::unordered_set<uint64_t> mapped;
    std::vector<uc_hook> hooks;
    cbt_drv_result result;

    uint64_t r(int which) const { return reg(uc, ids[which]); }
    void setr(int which, uint64_t v) { set_reg(uc, ids[which], v); }
    void stop() { ucl().emu_stop(uc); }
    bool holds(uint64_t a) const {
        for (auto &s : spans)
            if (s.first <= a && a < s.second)
                return true;
        return false;
    }
};

// REGISTERS[:6] for given(): eax, ebx, ecx, edx, esi, edi
static const int GIVEN_REGS[6] = {R_EAX, R_EBX, R_ECX, R_EDX, R_ESI, R_EDI};

// (regs, 4 stack words) at ESP, as driver.run's given().
static void given(DrvRun *r, uint64_t out_regs[6], int &nregs, uint64_t out_words[4], int &nwords) {
    for (int i = 0; i < 6; i++)
        out_regs[i] = r->r(GIVEN_REGS[i]) & 0xFFFFFFFFull;
    nregs = 6;
    uint8_t b[16];
    if (ucl().mem_read(r->uc, r->r(R_ESP), b, 16) == UC_ERR_OK) {
        for (int i = 0; i < 4; i++)
            out_words[i] = little(b + 4 * i, 4);
        nwords = 4;
    } else {
        nwords = 0;
    }
}

// answer(): EAX and carry as the input says; jump_back pops the return address.  false on a read
// that could not be made (driver.run then ends the run as a fault, the same either way).
static bool answer(DrvRun *r, bool jump_back) {
    r->setr(R_EAX, r->service_eax & 0xFFFFFFFFull);
    uint64_t fl = r->r(R_EFLAGS);
    r->setr(R_EFLAGS, (fl & ~1ull) | (uint64_t(r->service_carry) & 1));
    if (jump_back) {
        uint64_t sp = r->r(R_ESP);
        uint32_t back;
        if (!read32(r->uc, sp, back))
            return false;
        r->setr(R_EIP, back);
        r->setr(R_ESP, (sp + 4) & 0xFFFFFFFFull);
    }
    if (r->shadow)
        sh_after_call(r->shadow, r->uc, true, uint64_t(r->service_eax), false, 0, 0, nullptr, 2,
                      uint64_t(r->service_carry));
    return true;
}

static void push_call(DrvRun *r, bool none_first, uint64_t first, uint64_t second, bool with_given) {
    uint64_t regs[6] = {0}, words[4] = {0};
    int nregs = 0, nwords = 0;
    if (with_given)
        given(r, regs, nregs, words, nwords);
    uint64_t row[CBT_DRV_CALL_WORDS] = {none_first ? 1u : 0u, first, second, uint64_t(nregs),
                                        regs[0], regs[1], regs[2], regs[3], regs[4], regs[5],
                                        uint64_t(nwords), words[0], words[1], words[2], words[3]};
    r->calls.insert(r->calls.end(), row, row + CBT_DRV_CALL_WORDS);
    r->ncalls++;
}

static void on_code(uc_engine *, uint64_t address, uint32_t, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    if (r->insns >= r->cap) {
        r->truncated = true;
        r->stop();
        return;
    }
    r->insns++;
    if (r->holds(address)) {
        if (int64_t(r->cover_set.size()) < D_MAX_COVER && r->cover_set.insert(address).second)
            r->cover.push_back(address);
        if (r->shadow)
            sh_step(r->shadow, r->uc, address, 0);
    } else if (r->scratch_lo <= address && address < r->scratch_hi) {
        if (r->shadow)                     // (a pointer's landing pad: a plain `ret`)
            sh_step(r->shadow, r->uc, address, 0);
    } else {
        // through a pointer nothing fills in (a callback, a hooked vector): a call out
        r->outside++;
        if (r->outside > D_MAX_OUTSIDE) {
            r->exit_code = 1;              // "wild"
            r->stop();
            return;
        }
        push_call(r, true, 0, address, true);   // (None, address): the call-out's address is `second`
        if (!answer(r, true)) {
            r->exit_code = 1;
            r->stop();
        }
    }
}

static int32_t where_for(DrvRun *r, int32_t device, int32_t service) {
    uint64_t key = (uint64_t(uint32_t(device)) << 32) | uint32_t(service);
    auto it = r->answer_cache.find(key);
    if (it != r->answer_cache.end())
        return it->second;
    int32_t out = -1;
    if (r->answers_ctx || r->answers) {
        const int raised = r->answers_ctx ? r->answers_ctx(r->answers_user, device, service, &out) : r->answers(device, service, &out);
        if (raised) {
            r->hook_error = 1;            // the caller raised: stop, it raises after the run
            r->stop();
            return -2;
        }
    }
    r->answer_cache.emplace(key, out);
    return out;
}

static void on_intr(uc_engine *, uint32_t intno, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    uint64_t eip = r->r(R_EIP);                    // just past the two-byte `int`
    if (intno == 0x20) {
        uint8_t b[4];
        if (ucl().mem_read(r->uc, eip, b, 4) != UC_ERR_OK) {
            r->exit_code = 2;                      // "exception: service number unreadable"
            r->stop();
            return;
        }
        uint32_t service = uint32_t(little(b, 2)), device = uint32_t(little(b + 2, 2));
        if (r->ncalls >= D_MAX_CALLS_RUN) {
            r->truncated = true;
            r->stop();
            return;
        }
        push_call(r, false, device, service, r->ncalls <= D_MAX_CALLS);   // driver: len(calls) <= MAX_CALLS
        int32_t place = where_for(r, int32_t(device), int32_t(service));
        if (place == -2)
            return;                                // Python raised in the resolver
        if (service & 0x8000) {                    // jump: the service returns to our caller
            answer(r, true);
        } else {
            answer(r, false);
            r->setr(R_EIP, (eip + 4) & 0xFFFFFFFFull);
        }
        if (place >= 0 && place < 7) {
            r->setr(place, r->vm_addr & 0xFFFFFFFFull);
            static const char *const NAMES[7] = {"EAX", "EBX", "ECX", "EDX", "ESI", "EDI", "EBP"};
            if (r->shadow)
                sh_forget(r->shadow, NAMES[place]);
        }
    } else if (intno != 0x41) {                    // 41h talks to a debugger: nothing happens
        r->exit_code = 3;
        r->exit_int = intno;
        r->stop();
    }
}

// The shared memory side, with driver.run's own per-access ceiling (MAX_MEMORY_STEPS) in front
// and reads=True, no saved/static/quiet (driver.run passes `_memory_hooks(U, rec, set())`).
static void on_mem(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    if ((r->memops < 0 ? 0 : r->memops) >= D_MAX_MEMORY_STEPS) {   // driver.run's on_memory wrapper
        r->truncated = true;
        r->stop();
        return;
    }
    r->memops = (r->memops < 0 ? 0 : r->memops) + 1;
    if (r->memops > D_MAX_MEMOPS) {
        r->truncated = r->governed = true;
        r->stop();
        return;
    }
    bool is_write = type == UC_MEM_WRITE;
    if (is_write) {
        int64_t seen = r->writes_seen = (r->writes_seen < 0 ? 0 : r->writes_seen) + 1;
        if (seen > D_MAX_MEM)
            return;
    }
    std::vector<uint64_t> &bucket = is_write ? r->writes : r->reads;
    if (int64_t(bucket.size() / CBT_MEM_WORDS) < D_MAX_MEM) {
        uint64_t pc = r->r(R_EIP);
        uint64_t v = 0, old = 0;
        bool none = false;
        size_t n = size < 8 ? size_t(size < 0 ? 0 : size) : 8;
        uint8_t buf[8];
        if (is_write) {
            v = uint64_t(value) & 0xFFFFFFFFull;
            old = ucl().mem_read(uc, address, buf, n) == UC_ERR_OK ? little(buf, n) : 0;
        } else if (ucl().mem_read(uc, address, buf, n) == UC_ERR_OK) {
            v = little(buf, n);
        } else {
            none = true;
        }
        // driver.run's rec has no "insn_count" key, so _memory_hooks records n = 0 for every op
        uint64_t row[CBT_MEM_WORDS] = {pc, address, uint64_t(int64_t(size)), v, none, 0u,
                                       is_write ? 1u : 0u, old};
        bucket.insert(bucket.end(), row, row + CBT_MEM_WORDS);
    }
}

static bool on_unmapped(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    uint64_t span = uint64_t(size > 1 ? size : 1);
    uint64_t lo = address & ~(D_PAGE - 1), hi = (address + span - 1) & ~(D_PAGE - 1);
    int newly = 0;
    for (uint64_t base = lo; base < hi + D_PAGE; base += D_PAGE) {
        if (r->mapped.count(base))
            continue;
        r->gov_n++;                                // maps count toward the per-function budget
        if (int64_t(r->mapped.size()) >= D_MAX_MAPS || (r->gov_ceil && r->gov_n > r->gov_ceil)) {
            r->truncated = r->governed = true;
            r->stop();
            return false;
        }
        if (ucl().mem_map(uc, base, D_PAGE, UC_PROT_ALL) != UC_ERR_OK)
            return false;                          // truly can't map -> let it fault
        r->mapped.insert(base);
        newly++;                                   // driver.run: fill=False, so no pattern written
    }
    uint64_t kind = type == UC_MEM_READ_UNMAPPED ? 0 : type == UC_MEM_WRITE_UNMAPPED ? 1
                  : type == UC_MEM_FETCH_UNMAPPED ? 2 : 3;
    if (int64_t(r->lazy.size() / CBT_LAZY_WORDS) < D_MAX_MEM) {
        uint64_t row[CBT_LAZY_WORDS] = {r->r(R_EIP), address, lo, kind, uint64_t(int64_t(size))};
        r->lazy.insert(r->lazy.end(), row, row + CBT_LAZY_WORDS);
    }
    if (newly == 0) {
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

static uint32_t on_in(uc_engine *, uint32_t port, int size, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    if (r->nports >= D_MAX_CALLS_RUN) {
        r->truncated = true;
        r->stop();
    } else {
        uint64_t row[CBT_DRV_PORT_WORDS] = {0u, port, uint64_t(int64_t(size)), 0u};
        r->ports.insert(r->ports.end(), row, row + CBT_DRV_PORT_WORDS);
        r->nports++;
    }
    return 0;
}

static void on_out(uc_engine *, uint32_t port, int size, uint32_t value, void *user) {
    DrvRun *r = static_cast<DrvRun *>(user);
    if (r->nports >= D_MAX_CALLS_RUN) {
        r->truncated = true;
        r->stop();
    } else {
        uint64_t mask = size >= 4 ? 0xFFFFFFFFull : ((1ull << (8 * size)) - 1);
        uint64_t row[CBT_DRV_PORT_WORDS] = {1u, port, uint64_t(int64_t(size)), uint64_t(value) & mask};
        r->ports.insert(r->ports.end(), row, row + CBT_DRV_PORT_WORDS);
        r->nports++;
    }
}

}  // namespace cb::emulate::tracer

using namespace cb::emulate::tracer;

extern "C" {

CBT_API void *cbt_drv_begin(const cbt_drv_config *c) {
    DrvRun *r = new DrvRun();
    r->uc = static_cast<uc_engine *>(c->uch);
    memcpy(r->ids, c->reg_ids, sizeof r->ids);
    r->vm_addr = c->vm_addr;
    r->ret = c->ret;
    r->scratch_lo = c->scratch_lo;
    r->scratch_hi = c->scratch_hi;
    r->cap = c->cap;
    r->service_eax = c->service_eax;
    r->service_carry = c->service_carry;
    r->gov_n = c->gov_n;
    r->gov_ceil = c->gov_ceil;
    r->answers = c->answers;
    r->shadow = static_cast<Shadow *>(c->shadow);
    for (int64_t i = 0; i < c->nspans; i++)
        r->spans.emplace_back(c->spans[2 * i], c->spans[2 * i + 1]);
    r->ins_in = c->ins_in;
    r->ins_out = c->ins_out;
    return r;
}

static int d_add(DrvRun *r, int type, void *cb, uint64_t begin, uint64_t end) {
    uc_hook h = 0;
    uc_err e = ucl().hook_add(r->uc, &h, type, cb, r, begin, end);
    if (e != UC_ERR_OK)
        return int(e);
    r->hooks.push_back(h);
    return 0;
}

static int d_add_insn(DrvRun *r, void *cb, int ins) {
    uc_hook h = 0;
    uc_err e = ucl().hook_add(r->uc, &h, UC_HOOK_INSN, cb, r, 1, 0, ins);
    if (e != UC_ERR_OK)
        return int(e);
    r->hooks.push_back(h);
    return 0;
}

CBT_API void cbt_drv_set_answers(void *run, cbt_drv_answers_ctx fn, void *user) {
    DrvRun *r = static_cast<DrvRun *>(run);
    r->answers_ctx = fn;
    r->answers_user = user;
}

CBT_API int cbt_drv_attach(void *run) {
    DrvRun *r = static_cast<DrvRun *>(run);
    int e = d_add(r, UC_HOOK_CODE, (void *)on_code, 1, 0);
    if (!e)
        e = d_add(r, UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, (void *)on_mem, 1, 0);
    if (!e)
        e = d_add(r, UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
                  (void *)on_unmapped, 1, 0);
    if (!e)
        e = d_add(r, UC_HOOK_INTR, (void *)on_intr, 1, 0);
    if (!e)
        e = d_add_insn(r, (void *)on_in, r->ins_in);
    if (!e)
        e = d_add_insn(r, (void *)on_out, r->ins_out);
    return e;
}

CBT_API void cbt_drv_state(void *run, int64_t *out) {
    DrvRun *r = static_cast<DrvRun *>(run);
    out[0] = r->hook_error;
    out[1] = r->uc_err;
    out[2] = r->gov_n;
}

CBT_API const cbt_drv_result *cbt_drv_result_get(void *run) {
    DrvRun *r = static_cast<DrvRun *>(run);
    cbt_drv_result &o = r->result;
    memset(&o, 0, sizeof o);
    o.exit_code = r->exit_code;
    o.exit_int = r->exit_int;
    o.insns = r->insns;
    o.truncated = r->truncated;
    o.governed = r->governed;
    o.outside = r->outside;
    o.memops = r->memops;
    o.writes_seen = r->writes_seen;
    o.stuck = r->stuck;
    o.gov_n = r->gov_n;
    o.hook_error = r->hook_error;
    o.uc_err = r->uc_err;
    o.cover = r->cover.data();
    o.ncover = int64_t(r->cover.size());
    o.calls = r->calls.data();
    o.ncalls = r->ncalls;
    o.ports = r->ports.data();
    o.nports = r->nports;
    o.reads = r->reads.data();
    o.nreads = int64_t(r->reads.size() / CBT_MEM_WORDS);
    o.writes = r->writes.data();
    o.nwrites = int64_t(r->writes.size() / CBT_MEM_WORDS);
    o.lazy = r->lazy.data();
    o.nlazy = int64_t(r->lazy.size() / CBT_LAZY_WORDS);
    return &o;
}

CBT_API int cbt_drv_detach(void *run) {
    DrvRun *r = static_cast<DrvRun *>(run);
    int bad = 0;
    for (uc_hook h : r->hooks)
        if (ucl().hook_del(r->uc, h) != UC_ERR_OK)
            bad = 1;
    r->hooks.clear();
    return bad;
}

CBT_API void cbt_drv_free(void *run) { delete static_cast<DrvRun *>(run); }

}  // extern "C"

// w16_rec.cc -- the 16-bit recorder (codebreaker/emulate/windows16.py trace_function16): the
// per-instruction code hook's plain path and the memory hooks of one NE function's run, in C++.
// Python still lays the module out, starts the run (uc_emu_start to SENT_BASE) and reads the
// result; this attaches the hooks to Python's own Unicorn handle, in trace_function16's order,
// and keeps the record the plain path and the memory hooks made.  The heavy, rare events are
// Python callbacks: the import stand-in (import_stub), a call into another of the module's
// functions (answer), and interrupts (on_intr); the branch log and the ports' device model stay
// Python hooks, which trace_function16 attaches itself.
#include <cstring>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cb/emulate/tracer/cbtrace.h"
#include "cb/emulate/tracer/libs.hpp"
#include "cb/emulate/tracer/shadow.hpp"

namespace cb::emulate::tracer {

static constexpr uint64_t W_PAGE = 0x1000;
static constexpr int64_t W_MAX_MEM = 40000;           // emulator._MAX_MEM
static constexpr int64_t W_MAX_MAPS = 512;            // emulator._MAX_MAPS
static constexpr int64_t W_MAX_MEMOPS = 3000000;      // emulator._MAX_MEMOPS
static constexpr int64_t W_MAX_PATH = 20000;

// emulator._pattern: what a page nothing set holds when a run asks for one filled.
static const std::vector<uint8_t> &pattern(uint64_t base) {
    static std::mutex lk;
    static std::unordered_map<uint64_t, std::vector<uint8_t>> cache;
    std::lock_guard<std::mutex> g(lk);
    auto it = cache.find(base);
    if (it == cache.end()) {
        if (cache.size() > 4096)
            cache.clear();
        std::vector<uint8_t> p(W_PAGE);
        for (uint64_t i = 0; i < W_PAGE; i++)
            p[i] = uint8_t(((base + i) * 0x9E3779B1ull >> 13) & 0xFF);
        it = cache.emplace(base, std::move(p)).first;
    }
    return it->second;
}

struct W16Run {
    uc_engine *uc = nullptr;
    int32_t eip_id = 0;
    bool near_fn = false, fill = false;
    int64_t cap = 0, gov_n = 0, gov_ceil = 0;
    uint64_t cs_base = 0, near_ret = 0, far_ret = 0, sent_lo = 0, sent_hi = 0, snap_base = 0;
    int64_t *count = nullptr;
    std::unordered_set<uint64_t> intercept;
    cbt_w16_event on_sentinel = nullptr, on_intercept = nullptr, on_intr = nullptr, on_own = nullptr;
    // an in-process caller's handlers (cbt_w16_set_context): one function with its own state, in place of the four above
    void *user = nullptr;
    cbt_w16_event_ctx ctx_event = nullptr;
    bool ctx_own = false;
    Shadow *shadow = nullptr;
    uint64_t own_lo = 0, own_hi = 0;

    // the record
    int64_t insn_count = 0;
    bool truncated = false, returned = false, limit_fault = false;
    int64_t memops = -1, writes_seen = -1, stuck = -1;
    int64_t hook_error = 0, uc_err = 0;
    std::vector<uint64_t> path, reads, writes, lazy;
    std::unordered_set<uint64_t> mapped;
    std::vector<uc_hook> hooks;
    cbt_w16_result result;

    uint64_t eip() const { return reg(uc, eip_id); }
    // The event handlers: the in-process caller's, or the four plain functions the C interface was given.
    int fire_sentinel(int64_t a, int64_t n) { return ctx_event ? ctx_event(user, CBT_W16_SENTINEL, a, n) : on_sentinel(a, n); }
    int fire_intercept(int64_t a, int64_t n) { return ctx_event ? ctx_event(user, CBT_W16_INTERCEPT, a, n) : on_intercept(a, n); }
    int fire_intr(int64_t a, int64_t n) { return ctx_event ? ctx_event(user, CBT_W16_INTERRUPT, a, n) : on_intr(a, n); }
    bool has_own() const { return ctx_event ? ctx_own : on_own != nullptr; }
    int fire_own(int64_t a, int64_t n) { return ctx_event ? ctx_event(user, CBT_W16_OWN, a, n) : on_own(a, n); }
    void stop() { ucl().emu_stop(uc); }
    void bump() { insn_count++; if (count) *count = insn_count; }
};

static void on_code(uc_engine *, uint64_t address, uint32_t, void *user) {
    W16Run *r = static_cast<W16Run *>(user);
    if (r->own_lo <= address && address < r->own_hi && r->has_own()) {
        // (a recorded-system context: the module's own code reached through a recorded far
        //  pointer -- that function of this side's build run, by name, Python's to redirect)
        int got = r->fire_own(int64_t(address), r->insn_count);
        if (got == 2) {
            r->hook_error = 1;
            r->stop();
            return;
        }
        if (got == 1)
            return;
    }
    if (!r->intercept.empty() && r->intercept.count(address) && r->insn_count) {
        if (r->fire_intercept(int64_t(address), r->insn_count)) {
            r->hook_error = 1;
            r->stop();
        }
        return;
    }
    if (r->insn_count >= r->cap) {
        r->truncated = true;
        r->stop();
        return;
    }
    if (r->near_fn && address == r->near_ret) {
        r->returned = true;
        r->stop();
        return;
    }
    if (address == r->far_ret) {
        r->returned = true;
        r->stop();
        return;
    }
    r->bump();
    if (int64_t(r->path.size()) < W_MAX_PATH)
        r->path.push_back(address);
    if (r->sent_lo <= address && address < r->sent_hi) {        // an import sentinel landing
        if (r->fire_sentinel(int64_t(address), r->insn_count)) {
            r->hook_error = 1;
            r->stop();
        }
        return;
    }
    if (r->shadow)                     // (an ordinary instruction: the shadow steps)
        sh_step(r->shadow, r->uc, address, sh_segbase(r->shadow, r->uc, reg(r->uc, UC_X86_REG_CS)));
}

static void on_intr(uc_engine *, uint32_t intno, void *user) {
    W16Run *r = static_cast<W16Run *>(user);
    if (r->fire_intr(int64_t(intno), r->insn_count)) {
        r->hook_error = 1;
        r->stop();
    }
}

// hook_limited + the shared hook_mem (reads=True, fill, no saved/static/quiet; n = insn_count).
static void on_mem(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t value, void *user) {
    W16Run *r = static_cast<W16Run *>(user);
    if (r->limit_fault)
        return;
    uint64_t span = uint64_t(size > 1 ? size : 1);
    if ((address & 0xFFFF) + span > 0x10000 && address < r->snap_base) {   // past the end of a segment
        r->limit_fault = true;
        r->stop();
        return;
    }
    r->memops = (r->memops < 0 ? 0 : r->memops) + 1;
    if (r->memops > W_MAX_MEMOPS) {
        r->truncated = true;
        r->stop();
        return;
    }
    bool is_write = type == UC_MEM_WRITE;
    if (is_write) {
        int64_t seen = r->writes_seen = (r->writes_seen < 0 ? 0 : r->writes_seen) + 1;
        if (seen > W_MAX_MEM)
            return;
    }
    std::vector<uint64_t> &bucket = is_write ? r->writes : r->reads;
    if (int64_t(bucket.size() / CBT_MEM_WORDS) < W_MAX_MEM) {
        uint64_t pc = r->eip();
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
        uint64_t row[CBT_MEM_WORDS] = {pc, address, uint64_t(int64_t(size)), v, none,
                                       uint64_t(r->insn_count), is_write ? 1u : 0u, old};
        bucket.insert(bucket.end(), row, row + CBT_MEM_WORDS);
    }
}

static bool on_unmapped(uc_engine *uc, uc_mem_type type, uint64_t address, int size, int64_t, void *user) {
    W16Run *r = static_cast<W16Run *>(user);
    uint64_t span = uint64_t(size > 1 ? size : 1);
    uint64_t lo = address & ~(W_PAGE - 1), hi = (address + span - 1) & ~(W_PAGE - 1);
    int newly = 0;
    for (uint64_t base = lo; base < hi + W_PAGE; base += W_PAGE) {
        if (r->mapped.count(base))
            continue;
        r->gov_n++;
        if (int64_t(r->mapped.size()) >= W_MAX_MAPS || (r->gov_ceil && r->gov_n > r->gov_ceil)) {
            r->truncated = true;
            r->stop();
            return false;
        }
        if (ucl().mem_map(uc, base, W_PAGE, UC_PROT_ALL) != UC_ERR_OK)
            return false;
        r->mapped.insert(base);
        newly++;
        if (r->fill) {
            const std::vector<uint8_t> &p = pattern(base);
            ucl().mem_write(uc, base, p.data(), W_PAGE);
        }
    }
    uint64_t kind = type == UC_MEM_READ_UNMAPPED ? 0 : type == UC_MEM_WRITE_UNMAPPED ? 1
                  : type == UC_MEM_FETCH_UNMAPPED ? 2 : 3;
    if (int64_t(r->lazy.size() / CBT_LAZY_WORDS) < W_MAX_MEM) {
        uint64_t row[CBT_LAZY_WORDS] = {r->eip(), address, lo, kind, uint64_t(int64_t(size))};
        r->lazy.insert(r->lazy.end(), row, row + CBT_LAZY_WORDS);
    }
    if (newly == 0) {
        r->stuck = (r->stuck < 0 ? 0 : r->stuck) + 1;
        if (r->stuck > 64) {
            r->truncated = true;
            r->stop();
            return false;
        }
    } else {
        r->stuck = 0;
    }
    return true;
}

}  // namespace cb::emulate::tracer

using namespace cb::emulate::tracer;

extern "C" {

CBT_API void *cbt_w16_begin(const cbt_w16_config *c) {
    W16Run *r = new W16Run();
    r->uc = static_cast<uc_engine *>(c->uch);
    r->eip_id = c->eip_id;
    r->near_fn = c->near_fn != 0;
    r->fill = c->fill != 0;
    r->cap = c->cap;
    r->gov_n = c->gov_n;
    r->gov_ceil = c->gov_ceil;
    r->cs_base = c->cs_base;
    r->near_ret = c->near_ret;
    r->far_ret = c->far_ret;
    r->sent_lo = c->sent_lo;
    r->sent_hi = c->sent_hi;
    r->snap_base = c->snap_base;
    r->count = c->count;
    for (int64_t i = 0; i < c->nintercept; i++)
        r->intercept.insert(c->intercept[i]);
    r->on_sentinel = c->on_sentinel;
    r->on_intercept = c->on_intercept;
    r->on_intr = c->on_intr;
    r->shadow = static_cast<Shadow *>(c->shadow);
    r->own_lo = c->own_lo;
    r->own_hi = c->own_hi;
    r->on_own = c->on_own;
    return r;
}

static int w_add(W16Run *r, int type, void *cb, uint64_t begin, uint64_t end) {
    uc_hook h = 0;
    uc_err e = ucl().hook_add(r->uc, &h, type, cb, r, begin, end);
    if (e != UC_ERR_OK)
        return int(e);
    r->hooks.push_back(h);
    return 0;
}

CBT_API int cbt_w16_attach(void *run) {
    W16Run *r = static_cast<W16Run *>(run);
    int e = w_add(r, UC_HOOK_CODE, (void *)on_code, 1, 0);
    if (!e)
        e = w_add(r, UC_HOOK_INTR, (void *)on_intr, 1, 0);
    if (!e)
        e = w_add(r, UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, (void *)on_mem, 1, 0);
    if (!e)
        e = w_add(r, UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
                  (void *)on_unmapped, 1, 0);
    return e;
}

CBT_API void cbt_w16_set_context(void *run, void *user, cbt_w16_event_ctx event, int has_own) {
    W16Run *r = static_cast<W16Run *>(run);
    r->user = user;
    r->ctx_event = event;
    r->ctx_own = has_own != 0;
}

CBT_API void cbt_w16_state(void *run, int64_t *out) {
    W16Run *r = static_cast<W16Run *>(run);
    out[0] = r->hook_error;
    out[1] = r->uc_err;
    out[2] = r->gov_n;
}

CBT_API const cbt_w16_result *cbt_w16_result_get(void *run) {
    W16Run *r = static_cast<W16Run *>(run);
    cbt_w16_result &o = r->result;
    memset(&o, 0, sizeof o);
    o.insn_count = r->insn_count;
    o.truncated = r->truncated;
    o.returned = r->returned;
    o.limit_fault = r->limit_fault;
    o.memops = r->memops;
    o.writes_seen = r->writes_seen;
    o.stuck = r->stuck;
    o.gov_n = r->gov_n;
    o.hook_error = r->hook_error;
    o.uc_err = r->uc_err;
    o.path = r->path.data();
    o.npath = int64_t(r->path.size());
    o.reads = r->reads.data();
    o.nreads = int64_t(r->reads.size() / CBT_MEM_WORDS);
    o.writes = r->writes.data();
    o.nwrites = int64_t(r->writes.size() / CBT_MEM_WORDS);
    o.lazy = r->lazy.data();
    o.nlazy = int64_t(r->lazy.size() / CBT_LAZY_WORDS);
    return &o;
}

CBT_API int cbt_w16_detach(void *run) {
    W16Run *r = static_cast<W16Run *>(run);
    int bad = 0;
    for (uc_hook h : r->hooks)
        if (ucl().hook_del(r->uc, h) != UC_ERR_OK)
            bad = 1;
    r->hooks.clear();
    return bad;
}

CBT_API void cbt_w16_reset(void *run) {
    W16Run *r = static_cast<W16Run *>(run);
    r->insn_count = 0;
    if (r->count)
        *r->count = 0;
    r->path.clear();
    r->reads.clear();
    r->writes.clear();
}

CBT_API void cbt_w16_free(void *run) { delete static_cast<W16Run *>(run); }

}  // extern "C"

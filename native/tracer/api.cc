// api.cc -- the C interface (cbtrace.h): set up, attach a run's hooks to Python's Unicorn
// engine, and hand its record back.
#include <cstring>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "cbtrace.h"
#include "run.h"

namespace cbt {

std::mutex cache_lock;
static std::vector<std::unique_ptr<Static>> statics_by_id;    // (never freed: _INSN lives as long)
static std::unordered_map<uint64_t, Branchy> decoded;          // sha id << 32 | address -> _DECODED

Static *static_by_id(int32_t id) {
    std::lock_guard<std::mutex> g(cache_lock);
    return id >= 0 && size_t(id) < statics_by_id.size() ? statics_by_id[size_t(id)].get() : nullptr;
}

const Branchy &branchy(uc_engine *uc, int32_t sha_id, uint64_t address) {
    uint64_t key = uint64_t(uint32_t(sha_id)) << 32 | (address & 0xFFFFFFFFull);
    std::lock_guard<std::mutex> g(cache_lock);
    auto it = decoded.find(key);
    if (it == decoded.end()) {
        Branchy b;
        decode_branchy(uc, address, b);
        it = decoded.emplace(key, std::move(b)).first;
    }
    return it->second;
}

}  // namespace cbt

using namespace cbt;

extern "C" {

CBT_API int cbt_version(void) { return CBT_VERSION; }

CBT_API int cbt_init(const wchar_t *unicorn_dll, const wchar_t *capstone_dll, char *err, int32_t errlen) {
    static std::mutex once;
    static int state = -1;
    std::lock_guard<std::mutex> g(once);
    if (state == 0)
        return 0;
    char buf[256] = {0};
    state = load_libs(unicorn_dll, capstone_dll, buf, sizeof buf) ? 0 : 1;
    if (state && err && errlen > 0) {
        strncpy(err, buf, size_t(errlen) - 1);
        err[errlen - 1] = 0;
    }
    return state;
}

CBT_API void cbt_register(const char *name, int32_t uc_id) { register_reg(name, uc_id); }

CBT_API int32_t cbt_static(uint64_t lo, uint64_t hi, const uint8_t *image) {
    auto s = std::make_unique<Static>();
    s->lo = lo;
    s->hi = hi;
    s->image.assign(image, image + (hi - lo));
    std::lock_guard<std::mutex> g(cache_lock);
    statics_by_id.push_back(std::move(s));
    return int32_t(statics_by_id.size() - 1);
}

CBT_API int32_t cbt_intern(const char *s) { return intern(s ? s : ""); }

CBT_API void *cbt_begin(const cbt_config *c) {
    Run *r = new Run();
    r->uc = static_cast<uc_engine *>(c->uch);
    r->flags = c->flags;
    r->sha_id = c->sha_id;
    r->insn_cap = c->insn_cap;
    r->stub_ret = c->stub_ret;
    r->stub_cleanup = c->stub_cleanup;
    r->gov_n = c->gov_n;
    r->gov_ceil = c->gov_ceil;
    r->func_lo = c->func_lo;
    r->func_hi = c->func_hi;
    r->sec_lo = c->sec_lo;
    r->sec_hi = c->sec_hi;
    r->pads_lo = c->pads_lo;
    r->pads_hi = c->pads_hi;
    r->quiet_lo = c->quiet_lo;
    r->quiet_hi = c->quiet_hi;
    r->lean_low_end = c->lean_low_end;
    r->lean_high_begin = c->lean_high_begin;
    for (int32_t i = 0; i < c->nstatics; i++) {
        Static *s = static_by_id(c->statics[i]);
        if (!s) {
            delete r;
            return nullptr;
        }
        r->statics.push_back(s);
    }
    r->init_pages.insert(c->init_pages, c->init_pages + c->ninit);
    r->mapped.insert(c->mapped, c->mapped + c->nmapped);
    r->saved_keys.insert(c->presaved, c->presaved + c->npresaved);
    for (int64_t i = 0; i < c->nsentinels; i++)
        r->sentinels.emplace(c->sentinels[i], i);   // (a repeated address: the first place, as a dict
                                                    //  built in this order would keep the last -- Python
                                                    //  passes each address once)
    r->resolver = c->resolver;
    uint32_t f = c->flags;
    r->fast = f & CBT_FAST;
    r->lean = f & CBT_LEAN;
    r->reads = f & CBT_READS;
    r->quiet = f & CBT_QUIET;
    r->stop_wild = f & CBT_STOP_WILD;
    r->stub = f & CBT_STUB;
    r->unique = f & CBT_UNIQUE;
    r->branches = f & CBT_BRANCHES;
    r->pads = f & CBT_PADS;
    r->saving = f & CBT_SAVED;
    if (!(f & CBT_RESOLVER))
        r->resolver = nullptr;
    return r;
}

static int add(Run *r, int type, void *cb, uint64_t begin, uint64_t end) {
    uc_hook h = 0;
    uc_err e = UC.hook_add(r->uc, &h, type, cb, r, begin, end);
    if (e != UC_ERR_OK)
        return int(e);
    r->hooks.push_back(h);
    return 0;
}

// The hooks in trace_function's order: the instruction (or block) hook, the memory hook (or the
// lean write hook below the stack), unmapped accesses, interrupts, the lean write hook above the
// stack, the branch log.  Unicorn runs the same with the same hooks.
CBT_API int cbt_attach(void *run) {
    Run *r = static_cast<Run *>(run);
    int e = r->fast ? add(r, UC_HOOK_BLOCK, (void *)hook_block, 1, 0) : add(r, UC_HOOK_CODE, (void *)hook_code, 1, 0);
    if (!e)
        e = !r->lean ? add(r, r->reads ? (UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE) : UC_HOOK_MEM_WRITE, (void *)hook_mem, 1, 0)
                     : add(r, UC_HOOK_MEM_WRITE, (void *)hook_write, 0, r->lean_low_end);
    if (!e)
        e = add(r, UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
                (void *)hook_unmapped, 1, 0);
    if (!e)
        e = add(r, UC_HOOK_INTR, (void *)hook_intr, 1, 0);
    if (!e && r->lean)
        e = add(r, UC_HOOK_MEM_WRITE, (void *)hook_write, r->lean_high_begin, 0xFFFFFFFFull);
    if (!e && r->branches)
        e = add(r, UC_HOOK_CODE, (void *)hook_branch, r->func_lo, r->func_hi - 1);
    return e;
}

CBT_API int cbt_attach_code(void *run) {
    return add(static_cast<Run *>(run), UC_HOOK_CODE, (void *)hook_code, 1, 0);
}

CBT_API int cbt_switched(void *run) { return static_cast<Run *>(run)->switched ? 1 : 0; }

CBT_API void cbt_state(void *run, int64_t *out) {
    Run *r = static_cast<Run *>(run);
    out[0] = r->hook_error;
    out[1] = r->uc_err;
    out[2] = r->truncated;
    out[3] = r->gov_n;
    out[4] = r->switched;
}

CBT_API void cbt_settle(void *run) { settle(static_cast<Run *>(run)); }

CBT_API const cbt_result *cbt_result_get(void *run) {
    Run *r = static_cast<Run *>(run);
    cbt_result &o = r->result;
    memset(&o, 0, sizeof o);
    o.insn_count = r->insn_count;
    o.truncated = r->truncated;
    o.governed = r->governed;
    o.dirty = r->dirty;
    o.ran_into_zeros = r->ran_into_zeros;
    o.switched = r->switched;
    o.memops = r->memops;
    o.writes_seen = r->writes_seen;
    o.stuck = r->stuck;
    o.bogus_stubs = r->bogus;
    o.stubbed = r->stubbed;
    o.lean_seen = r->lean_seen;
    o.api_calls_set = r->api_calls_set;
    o.gov_n = r->gov_n;
    o.hook_error = r->hook_error;
    o.uc_err = r->uc_err;
    o.path = r->path.data();
    o.npath = int64_t(r->path.size());
    // the instruction texts, first seen first (what settle took back left out)
    r->out_text.clear();
    r->out_mn.clear();
    for (size_t i = 0; i < r->mn_addr.size(); i++) {
        if (r->mn_dead[i])
            continue;
        r->out_mn.push_back(r->mn_addr[i]);
        r->out_text += *r->mn_text[i];
        r->out_text.push_back('\0');
    }
    o.mn_addr = r->out_mn.data();
    o.nmn = int64_t(r->out_mn.size());
    o.mn_text = r->out_text.data();
    o.mn_text_len = int64_t(r->out_text.size());
    std::vector<uint64_t> &addrs = r->out_calls;
    addrs.clear();
    for (const Call &c : r->calls) {
        uint64_t row[CBT_CALL_WORDS] = {c.pc, c.has_target, c.target, c.into_func, uint64_t(c.state), c.resolved,
                                        c.external, uint64_t(c.api), c.has_ret_sentinel, uint64_t(c.ret_sentinel),
                                        c.has_args, c.args[0], c.args[1], c.args[2], c.args[3], c.args[4],
                                        c.args[5], c.ecx, c.edx, c.has_argc, uint64_t(c.argc)};
        addrs.insert(addrs.end(), row, row + CBT_CALL_WORDS);
    }
    o.calls = addrs.data();
    o.ncalls = int64_t(r->calls.size());
    o.reads = r->reads_rec.data();
    o.nreads = int64_t(r->reads_rec.size() / CBT_MEM_WORDS);
    o.writes = r->writes_rec.data();
    o.nwrites = int64_t(r->writes_rec.size() / (r->lean ? CBT_LEAN_WORDS : CBT_MEM_WORDS));
    o.pages0 = r->pages0_base.data();
    o.pages0_data = r->pages0_data.data();
    o.npages0 = int64_t(r->pages0_base.size());
    o.lazy = r->lazy.data();
    o.nlazy = int64_t(r->lazy.size() / CBT_LAZY_WORDS);
    o.deps = r->deps.data();
    o.ndeps = int64_t(r->deps.size());
    o.api_calls = r->api_calls.data();
    o.napi_words = int64_t(r->api_calls.size());
    o.vxd = r->vxd.data();
    o.nvxd = int64_t(r->vxd.size());
    o.branches = r->branch_log.data();
    o.nbranches = int64_t(r->branch_log.size() / CBT_BRANCH_WORDS);
    return &o;
}

CBT_API int cbt_detach(void *run) {
    Run *r = static_cast<Run *>(run);
    int bad = 0;
    for (uc_hook h : r->hooks)
        if (UC.hook_del(r->uc, h) != UC_ERR_OK)
            bad = 1;
    r->hooks.clear();
    return bad;
}

CBT_API int cbt_restore(void *run) { return restore(static_cast<Run *>(run)); }

CBT_API void cbt_free(void *run) { delete static_cast<Run *>(run); }

}  // extern "C"

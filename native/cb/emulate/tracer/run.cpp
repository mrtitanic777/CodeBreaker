// run.cpp -- what runs of one program share (TraceCache), and putting a run's hooks on Unicorn's engine and taking
// them off.
#include "cb/emulate/tracer/run.hpp"

namespace cb::emulate::tracer {

int32_t TraceCache::add_static(uint64_t lo, uint64_t hi, const uint8_t *image) {
    auto s = std::make_unique<Static>();
    s->lo = lo;
    s->hi = hi;
    s->image.assign(image, image + (hi - lo));
    std::lock_guard<std::mutex> g(lock);
    statics_.push_back(std::move(s));
    return int32_t(statics_.size() - 1);
}

Static *TraceCache::static_by_id(int32_t id) {
    std::lock_guard<std::mutex> g(lock);
    return id >= 0 && size_t(id) < statics_.size() ? statics_[size_t(id)].get() : nullptr;
}

const Branchy &TraceCache::branchy(uc_engine *uc, int32_t sha_id, uint64_t address) {
    const uint64_t key = uint64_t(uint32_t(sha_id)) << 32 | (address & 0xFFFFFFFFull);
    std::lock_guard<std::mutex> g(lock);
    auto it = decoded_.find(key);
    if (it == decoded_.end()) {
        Branchy b;
        decode_branchy(uc, address, b);
        it = decoded_.emplace(key, std::move(b)).first;
    }
    return it->second;
}

int32_t TraceCache::intern_sha(const std::string &sha) {
    std::lock_guard<std::mutex> g(lock);
    auto it = shas_.find(sha);
    if (it != shas_.end()) return it->second;
    const int32_t id = int32_t(shas_.size());
    shas_.emplace(sha, id);
    return id;
}

namespace {

int add(Run *r, int type, void *callback, uint64_t begin, uint64_t end) {
    uc_hook h = 0;
    const uc_err e = ucl().hook_add(r->uc, &h, type, callback, r, begin, end);
    if (e != UC_ERR_OK) return int(e);
    r->hooks.push_back(h);
    return 0;
}

}  // namespace

// The hooks in trace_function's order: the instruction (or block) hook, the memory hook (or the lean write hook below
// the stack), unmapped accesses, interrupts, the lean write hook above the stack, the branch log. Unicorn runs the same
// with the same hooks.
int attach(Run *r) {
    int e = r->fast ? add(r, UC_HOOK_BLOCK, reinterpret_cast<void *>(hook_block), 1, 0)
                    : add(r, UC_HOOK_CODE, reinterpret_cast<void *>(hook_code), 1, 0);
    if (!e)
        e = !r->lean ? add(r, r->reads ? (UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE) : UC_HOOK_MEM_WRITE,
                           reinterpret_cast<void *>(hook_mem), 1, 0)
                     : add(r, UC_HOOK_MEM_WRITE, reinterpret_cast<void *>(hook_write), 0, r->lean_low_end);
    if (!e)
        e = add(r, UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED | UC_HOOK_MEM_FETCH_UNMAPPED,
                reinterpret_cast<void *>(hook_unmapped), 1, 0);
    if (!e) e = add(r, UC_HOOK_INTR, reinterpret_cast<void *>(hook_intr), 1, 0);
    if (!e && r->lean)
        e = add(r, UC_HOOK_MEM_WRITE, reinterpret_cast<void *>(hook_write), r->lean_high_begin, 0xFFFFFFFFull);
    if (!e && r->branches)
        e = add(r, UC_HOOK_CODE, reinterpret_cast<void *>(hook_branch), r->func_lo, r->func_hi - 1);
    return e;
}

int attach_code(Run *r) { return add(r, UC_HOOK_CODE, reinterpret_cast<void *>(hook_code), 1, 0); }

int detach(Run *r) {
    int bad = 0;
    for (uc_hook h : r->hooks)
        if (ucl().hook_del(r->uc, h) != UC_ERR_OK) bad = 1;
    r->hooks.clear();
    return bad;
}

}  // namespace cb::emulate::tracer

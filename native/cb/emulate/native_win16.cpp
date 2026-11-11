// cb/emulate/native_win16.cpp -- see native_win16.hpp.
#include "cb/emulate/native_win16.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/emulate/tracer/cbtrace.h"

namespace cb::emulate::native_win16 {

using native_trace::Access;

struct Run::Impl {
    Options options;
    void* handle = nullptr;
    std::exception_ptr raised;
    std::int64_t count = 0;                        // the recorder updates it as the run goes
    std::vector<std::uint64_t> intercept;          // the recorder reads it for the life of the run
    std::int64_t state[3] = {0, 0, 0};

    // The recorder's one callback: kind says which handler. 0 or 1 (or 2 for the own handler: it raised); the first
    // exception is kept for the caller.
    int event(int kind, std::int64_t a, std::int64_t n) {
        try {
            const Handlers& h = options.handlers;
            switch (kind) {
                case CBT_W16_SENTINEL: if (h.on_sentinel) h.on_sentinel(a, n); return 0;
                case CBT_W16_INTERCEPT: if (h.on_intercept) h.on_intercept(a, n); return 0;
                case CBT_W16_INTERRUPT: if (h.on_interrupt) h.on_interrupt(a, n); return 0;
                default: return h.on_own && h.on_own(a, n) ? 1 : 0;
            }
        } catch (...) {   // an isolation boundary: carried out of the run, rethrown by the caller
            if (!raised) raised = std::current_exception();
            return kind == CBT_W16_OWN ? 2 : 1;
        }
    }

    static int trampoline(void* user, int kind, std::int64_t a, std::int64_t n) {
        return static_cast<Impl*>(user)->event(kind, a, n);
    }
};

Run::Run(uc_struct* engine, Options options) : impl_(std::make_unique<Impl>()) {
    CB_ASSERT(options.governor != nullptr, "a 16-bit run needs a governor");
    Impl& s = *impl_;
    s.options = std::move(options);
    const Options& o = s.options;
    s.intercept = o.intercept;
    cbt_w16_config c{};
    c.uch = engine;
    c.eip_id = o.eip_id;
    c.near_fn = o.near_entry ? 1 : 0;
    c.fill = o.fill ? 1 : 0;
    c.cap = o.cap;
    c.cs_base = o.cs_base;
    c.near_ret = o.near_ret;
    c.far_ret = o.far_ret;
    c.sent_lo = o.sent_lo;
    c.sent_hi = o.sent_hi;
    c.snap_base = o.snap_base;
    c.gov_n = o.governor->n;
    c.gov_ceil = o.governor->ceil;
    c.count = &s.count;
    c.intercept = s.intercept.data();
    c.nintercept = static_cast<std::int64_t>(s.intercept.size());
    c.shadow = o.shadow;
    if (o.own) {
        c.own_lo = o.own->lo;
        c.own_hi = o.own->hi;
    }
    s.handle = cbt_w16_begin(&c);
    if (s.handle == nullptr) throw EmulationError("cbtrace: a 16-bit run could not be set up");
    cbt_w16_set_context(s.handle, &s, &Impl::trampoline, o.own && o.handlers.on_own ? 1 : 0);
    if (const int e = cbt_w16_attach(s.handle)) {
        cbt_w16_free(s.handle);
        s.handle = nullptr;
        throw EmulationError(std::format("cbtrace: a 16-bit hook could not be attached (unicorn error {})", e));
    }
}

Run::~Run() {
    if (impl_ && impl_->handle != nullptr) cbt_w16_free(impl_->handle);
}

void Run::reset() { cbt_w16_reset(impl_->handle); }

std::int64_t Run::count() const { return impl_->count; }

std::exception_ptr Run::raised() {
    Impl& s = *impl_;
    cbt_w16_state(s.handle, s.state);
    s.options.governor->n = s.state[2];
    std::exception_ptr e = s.raised;
    s.raised = nullptr;
    return e;
}

Trace Run::collect() {
    Impl& s = *impl_;
    const cbt_w16_result& r = *cbt_w16_result_get(s.handle);
    s.options.governor->n = r.gov_n;
    Trace t;
    t.insn_count = r.insn_count;
    t.truncated = r.truncated != 0;
    t.returned = r.returned != 0;
    t.limit_fault = r.limit_fault != 0;
    if (r.npath > 0) t.path.assign(r.path, r.path + r.npath);
    for (std::int64_t i = 0; i < r.nreads; ++i) {
        const std::uint64_t* w = r.reads + i * CBT_MEM_WORDS;
        t.mem_reads.push_back({w[0], w[1], w[2], w[4] ? std::nullopt : std::optional<std::uint64_t>(w[3]), w[5]});
    }
    for (std::int64_t i = 0; i < r.nwrites; ++i) {
        const std::uint64_t* w = r.writes + i * CBT_MEM_WORDS;
        t.mem_writes.push_back({w[0], w[1], w[2], w[3], w[5], w[7]});
    }
    for (std::int64_t i = 0; i < r.nlazy; ++i) {
        const std::uint64_t* w = r.lazy + i * CBT_LAZY_WORDS;
        CB_ASSERT(w[3] < 4, "a lazy map's access kind");
        t.lazy_maps.push_back({w[0], w[1], w[2], static_cast<Access>(w[3]), w[4]});
    }
    if (r.memops >= 0) t.memops = r.memops;
    if (r.writes_seen >= 0) t.writes_seen = r.writes_seen;
    if (r.stuck >= 0) t.stuck = r.stuck;
    return t;
}

void Run::detach() {
    if (cbt_w16_detach(impl_->handle)) throw EmulationError("cbtrace: a 16-bit hook could not be taken off");
}

}  // namespace cb::emulate::native_win16

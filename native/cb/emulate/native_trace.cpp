// cb/emulate/native_trace.cpp -- see native_trace.hpp.
#include "cb/emulate/native_trace.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

#include "cb/emulate/tracer/cbtrace.h"
#include "cb/emulate/tracer/run.hpp"

namespace cb::emulate::native_trace {

namespace tr = cb::emulate::tracer;

std::string_view to_text(Access a) {
    switch (a) {
        case Access::Read: return "read";
        case Access::Write: return "write";
        case Access::Fetch: return "fetch";
        case Access::Other: return "?";
    }
    return "?";
}

std::string_view to_text(CompareOp op) {
    static constexpr std::string_view kNames[9] = {"cmp", "test", "sub", "and", "or", "dec", "inc", "add", "xor"};
    return kNames[static_cast<std::size_t>(op)];
}

// ---- the cache -----------------------------------------------------------------------------------------------

struct TraceCache::Impl {
    tr::TraceCache cache;
};

TraceCache::TraceCache() : impl_(std::make_unique<Impl>()) {}
TraceCache::~TraceCache() = default;

StaticId TraceCache::add_code(std::uint64_t lo, ByteView image) {
    return impl_->cache.add_static(lo, lo + image.size(), image.data());
}

// ---- one run -------------------------------------------------------------------------------------------------

struct TraceRun::Impl {
    uc_struct* engine = nullptr;
    Options options;
    std::unique_ptr<tr::Run> run;
    std::vector<ImportName> names;                       // by place in options.sentinels
    std::exception_ptr raised;                           // what the resolver threw, carried out of the run
    bool attached = false;

    // An import entered: its argument count and what it gives back, as cbtrace.h's resolver wants them.
    int resolve(std::int32_t index, std::int64_t* out, std::int32_t cap) {
        try {
            const ImportName& import = names.at(static_cast<std::size_t>(index));
            ImportAnswer answer;
            if (import.dll == "CALLBACK") {
                answer.argc = 0;
            } else if (options.resolver) {
                answer = options.resolver(import);
            }
            out[0] = answer.argc ? *answer.argc : -1;
            out[1] = 0;
            if (answer.argc && *answer.argc != 0 && import.dll != "CALLBACK") {
                const std::int64_t n = std::min<std::int64_t>(static_cast<std::int64_t>(answer.outs.size()), cap);
                out[1] = n;
                for (std::int64_t k = 0; k < n; ++k) {
                    out[2 + 2 * k] = answer.outs[static_cast<std::size_t>(k)].index;
                    out[3 + 2 * k] = answer.outs[static_cast<std::size_t>(k)].value.value_or(-1);
                }
            }
            return 0;
        } catch (...) {                                  // carried out of the run, thrown by start()
            if (!raised) raised = std::current_exception();
            return 1;
        }
    }

    std::string api_text(std::int64_t code) const {
        if (code < 0) return std::format("CALLBACK!{}", -code - 1);
        const ImportName& n = names.at(static_cast<std::size_t>(code));
        return n.dll + "!" + n.name;
    }

    void sync_governor() const {
        if (options.governor) options.governor->n = run->gov_n;
    }
};

TraceRun::TraceRun(uc_struct* engine, Options options) : impl_(std::make_unique<Impl>()) {
    CB_ASSERT(options.cache != nullptr && options.governor != nullptr, "a run needs a cache and a governor");
    Impl& s = *impl_;
    s.engine = engine;
    s.options = std::move(options);
    const Options& o = s.options;
    s.run = std::make_unique<tr::Run>();
    tr::Run& r = *s.run;
    r.cache = &o.cache->impl().cache;
    r.uc = engine;
    r.flags = 0;
    r.fast = o.fast;
    r.lean = o.lean;
    r.reads = o.reads;
    r.quiet = o.quiet.has_value();
    r.stop_wild = o.stop_wild;
    r.stub = o.stub;
    r.unique = o.unique;
    r.branches = o.branches;
    r.pads = o.pads.has_value();
    r.saving = o.saved.has_value();
    r.sha_id = r.cache->intern_sha(o.module_hash);
    r.insn_cap = o.insn_cap;
    r.stub_ret = o.stub_ret;
    r.stub_cleanup = o.stub_cleanup;
    r.gov_n = o.governor->n;
    r.gov_ceil = o.governor->ceil;
    r.func_lo = o.func.lo;
    r.func_hi = o.func.hi;
    r.sec_lo = o.image.lo;
    r.sec_hi = o.image.hi;
    if (o.pads) {
        r.pads_lo = o.pads->lo;
        r.pads_hi = o.pads->hi;
    }
    if (o.quiet) {
        r.quiet_lo = o.quiet->lo;
        r.quiet_hi = o.quiet->hi;
    }
    r.lean_low_end = o.lean_low_end;
    r.lean_high_begin = o.lean_high_begin;
    for (StaticId id : o.statics) {
        tr::Static* code = r.cache->static_by_id(id);
        if (code == nullptr) throw UsageError(std::format("native_trace: no code range {}", id));
        r.statics.push_back(code);
    }
    r.init_pages.insert(o.init_pages.begin(), o.init_pages.end());
    r.mapped.insert(o.mapped.begin(), o.mapped.end());
    if (o.saved) r.saved_keys.insert(o.saved->begin(), o.saved->end());
    for (std::size_t i = 0; i < o.sentinels.size(); ++i) {
        r.sentinels.emplace(o.sentinels[i].address, static_cast<std::int64_t>(i));   // (a repeated address keeps the first place)
        s.names.push_back(o.sentinels[i].import);
    }
    Impl* self = &s;
    r.resolver = [self](std::int32_t index, std::int64_t* out, std::int32_t cap) { return self->resolve(index, out, cap); };
    if (const int e = tr::attach(&r)) {
        tr::detach(&r);
        throw EmulationError(std::format("native_trace: a hook could not be attached (unicorn error {})", e));
    }
    s.attached = true;
}

TraceRun::~TraceRun() {
    if (impl_ && impl_->attached && impl_->run) tr::detach(impl_->run.get());
}

void TraceRun::before_host_start() { impl_->raised = nullptr; }

void TraceRun::after_host_start() {
    Impl& s = *impl_;
    s.sync_governor();
    if (s.raised) {
        std::exception_ptr e = s.raised;
        s.raised = nullptr;
        std::rethrow_exception(e);
    }
    if (s.run->hook_error == 2) {                        // a read a hook could not make
        throw EmulationError(std::format("native_trace: a hook could not read the guest (unicorn error {})", s.run->uc_err));
    }
}

int TraceRun::start(std::uint64_t begin, std::uint64_t until, std::size_t count) {
    before_host_start();
    const uc_err status = tracer::ucl().emu_start(impl_->engine, begin, until, 0, count);
    after_host_start();
    return static_cast<int>(status);
}

int TraceRun::run(std::uint64_t begin, std::uint64_t until) {
    Impl& s = *impl_;
    const std::size_t cap = static_cast<std::size_t>(s.options.insn_cap) + 1;
    int status = 0;
    if (s.options.fast) {
        status = start(begin, until);
        if (switched()) {                                // from here on, one instruction at a time
            attach_code();
            tracer::ucl().ctl(s.engine, UC_CTL_WRITE(UC_CTL_TB_FLUSH, 0));
            status = start(tracer::reg(s.engine, UC_X86_REG_EIP), until, cap);
        }
    } else {
        status = start(begin, until, cap);
    }
    settle();
    return status;
}

bool TraceRun::switched() const { return impl_->run->switched; }

void TraceRun::attach_code() {
    if (const int e = tr::attach_code(impl_->run.get())) {
        throw EmulationError(std::format("native_trace: a hook could not be attached (unicorn error {})", e));
    }
}

void TraceRun::settle() {
    tr::settle(impl_->run.get());
}

bool TraceRun::truncated() const { return impl_->run->truncated; }

std::int64_t TraceRun::lean_seen() const { return impl_->run->lean_seen; }

void TraceRun::detach() {
    Impl& s = *impl_;
    s.attached = false;
    if (tr::detach(s.run.get())) throw EmulationError("native_trace: a hook could not be taken off");
}

void TraceRun::restore() {
    if (const int e = tr::restore(impl_->run.get())) {
        throw EmulationError(std::format("native_trace: the saved pages could not be put back (unicorn error {})", e));
    }
}

namespace {

// Rows of `width` words in a flat array (the recorder keeps its fixed-width records that way).
template <std::size_t Width, class F>
void for_rows(const std::vector<std::uint64_t>& words, F&& each) {
    for (std::size_t i = 0; i + Width <= words.size(); i += Width) each(&words[i]);
}

}  // namespace

TraceRecord TraceRun::collect() {
    Impl& s = *impl_;
    const tr::Run& r = *s.run;
    s.sync_governor();
    TraceRecord rec;
    rec.insn_count = r.insn_count;
    rec.truncated = r.truncated;
    rec.governed = r.governed;
    rec.path = r.path;
    for (std::size_t i = 0; i < r.mn_addr.size(); ++i) {   // (what settle took back left out)
        if (!r.mn_dead[i]) rec.mnemonics[r.mn_addr[i]] = *r.mn_text[i];
    }
    for (const tr::Call& c : r.calls) {
        CallRecord cr;
        cr.pc = c.pc;
        if (c.has_target) cr.target = c.target;
        cr.into_func = c.into_func;
        if (c.state == 1) {
            cr.resolved_target = c.resolved;
            cr.external = c.external;
        } else if (c.state == 2) {
            cr.resolved_target = c.resolved;
            cr.external = true;
            StubbedCall stub;
            stub.api = s.api_text(c.api);
            if (c.has_ret_sentinel) stub.ret_sentinel = c.ret_sentinel;
            cr.stub = std::move(stub);
        }
        if (c.has_args) {
            CallArgs a;
            for (int k = 0; k < 6; ++k) a.stack[k] = c.args[k];
            a.ecx = c.ecx;
            a.edx = c.edx;
            cr.args = a;
        }
        if (c.has_argc) cr.argc = c.argc;
        rec.calls.push_back(std::move(cr));
    }
    for_rows<CBT_MEM_WORDS>(r.reads_rec, [&](const std::uint64_t* w) {
        rec.mem_reads.push_back({w[0], w[1], w[2], w[4] ? std::nullopt : std::optional<std::uint64_t>(w[3]), w[5]});
    });
    if (r.lean) {
        for_rows<CBT_LEAN_WORDS>(r.writes_rec, [&](const std::uint64_t* w) { rec.lean_writes.push_back({w[0], w[1], w[2], w[3]}); });
        for (std::size_t i = 0; i < r.pages0_base.size(); ++i) {
            const std::uint8_t* page = r.pages0_data.data() + i * kPage;
            rec.pages0[r.pages0_base[i]] = Bytes(page, page + kPage);
        }
    } else {
        for_rows<CBT_MEM_WORDS>(r.writes_rec, [&](const std::uint64_t* w) { rec.mem_writes.push_back({w[0], w[1], w[2], w[3], w[5], w[7]}); });
    }
    for_rows<CBT_LAZY_WORDS>(r.lazy, [&](const std::uint64_t* w) {
        CB_ASSERT(w[3] < 4, "a lazy map's access kind");
        rec.lazy_maps.push_back({w[0], w[1], w[2], static_cast<Access>(w[3]), w[4]});
    });
    if (r.memops >= 0) rec.memops = r.memops;
    if (r.writes_seen >= 0) rec.writes_seen = r.writes_seen;
    rec.dirty = r.dirty;
    if (r.stuck >= 0) rec.stuck = r.stuck;
    if (r.bogus >= 0) rec.bogus_stubs = r.bogus;
    rec.ran_into_zeros = r.ran_into_zeros;
    for (std::int64_t d : r.deps) rec.api_deps.push_back(s.api_text(d));
    if (r.api_calls_set) {
        std::vector<ApiCall> seq;
        const std::vector<std::int64_t>& words = r.api_calls;          // code, argc (-1 unknown), argc arguments ...
        for (std::size_t i = 0; i + 1 < words.size();) {
            ApiCall call;
            call.api = s.api_text(words[i]);
            const std::int64_t argc = words[i + 1];
            if (argc >= 0) {
                const std::size_t end = std::min(words.size(), i + 2 + static_cast<std::size_t>(argc));
                call.args = std::vector<std::int64_t>(words.begin() + static_cast<std::ptrdiff_t>(i + 2), words.begin() + static_cast<std::ptrdiff_t>(end));
            }
            seq.push_back(std::move(call));
            i += 2 + (argc >= 0 ? static_cast<std::size_t>(argc) : 0);
        }
        rec.api_calls = std::move(seq);
    }
    if (r.stubbed >= 0) rec.stubbed_externals = r.stubbed;
    rec.vxd_services = r.vxd;
    if (r.branches) {
        for_rows<CBT_BRANCH_WORDS>(r.branch_log, [&](const std::uint64_t* w) {
            BranchEntry entry{w[0], w[1] != 0, std::nullopt};
            if (w[2]) {
                CB_ASSERT(w[3] < 9, "a branch compare's mnemonic");
                BranchCompare c;
                c.op = static_cast<CompareOp>(w[3]);
                c.size = w[4];
                c.a = w[5];
                c.b = w[6];
                if (w[7]) c.at_a = w[8];
                if (w[9]) c.at_b = w[10];
                entry.compare = c;
            }
            rec.branches.push_back(std::move(entry));
        });
    }
    return rec;
}

}  // namespace cb::emulate::native_trace

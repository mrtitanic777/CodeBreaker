// cb/emulate/native_driver.cpp -- see native_driver.hpp.
#include "cb/emulate/native_driver.hpp"

#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

#include "cb/emulate/tracer/cbtrace.h"
#include "cb/emulate/tracer/libs.hpp"

namespace cb::emulate::native_driver {

std::string DriverExit::text() const {
    switch (kind) {
        case Kind::Wild: return "wild";
        case Kind::ServiceUnreadable: return "exception: service number unreadable";
        case Kind::Interrupt: return std::format("int:{:02x}", interrupt);
    }
    return "wild";
}

struct DriverRun::Impl {
    Options options;
    uc_struct* engine = nullptr;
    void* handle = nullptr;                  // the recorder's run
    std::vector<std::uint64_t> spans;        // the flattened (lo, hi) pairs the recorder reads
    std::exception_ptr raised;               // what the resolver threw, carried out of the run
    bool attached = false;

    // The recorder asks which register a service answers in (cbt_drv_answers_ctx): 0, or nonzero when the answer threw.
    static int answers(void* user, std::int32_t device, std::int32_t service, std::int32_t* out) {
        Impl& self = *static_cast<Impl*>(user);
        try {
            const std::optional<unsigned> place =
                self.options.resolver ? self.options.resolver(static_cast<std::uint32_t>(device), static_cast<std::uint32_t>(service)) : std::nullopt;
            out[0] = place ? static_cast<std::int32_t>(*place) : -1;
            return 0;
        } catch (...) {                      // carried out of the run, thrown by after_host_start()
            if (!self.raised) self.raised = std::current_exception();
            return 1;
        }
    }

    void sync_governor() const {
        std::int64_t state[3] = {0, 0, 0};   // hook_error, uc_err, gov_n
        cbt_drv_state(handle, state);
        options.governor->n = state[2];
    }
};

DriverRun::DriverRun(uc_struct* engine, Options options) : impl_(std::make_unique<Impl>()) {
    CB_ASSERT(options.governor != nullptr, "a driver run needs a governor");
    Impl& s = *impl_;
    s.engine = engine;
    s.options = std::move(options);
    const Options& o = s.options;
    for (const native_trace::AddressRange& span : o.spans) {
        s.spans.push_back(span.lo);
        s.spans.push_back(span.hi);
    }
    cbt_drv_config c = {};
    c.uch = engine;
    const std::int32_t ids[10] = {UC_X86_REG_EAX, UC_X86_REG_EBX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESI,
                                  UC_X86_REG_EDI, UC_X86_REG_EBP, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_EFLAGS};
    for (int i = 0; i < 10; ++i) c.reg_ids[i] = ids[i];
    c.ins_in = UC_X86_INS_IN;
    c.ins_out = UC_X86_INS_OUT;
    c.vm_addr = o.vm_address;
    c.ret = o.return_address;
    c.scratch_lo = o.scratch.lo;
    c.scratch_hi = o.scratch.hi;
    c.spans = s.spans.empty() ? nullptr : s.spans.data();
    c.nspans = static_cast<std::int64_t>(o.spans.size());
    c.cap = o.cap;
    c.service_eax = o.service_eax;
    c.service_carry = o.service_carry ? 1 : 0;
    c.gov_n = o.governor->n;
    c.gov_ceil = o.governor->ceil;
    c.answers = nullptr;
    c.shadow = o.shadow;
    s.handle = cbt_drv_begin(&c);
    if (s.handle == nullptr) throw EmulationError("native_driver: a driver run could not be set up");
    cbt_drv_set_answers(s.handle, &Impl::answers, &s);
    if (const int e = cbt_drv_attach(s.handle)) {
        cbt_drv_detach(s.handle);
        cbt_drv_free(s.handle);
        s.handle = nullptr;
        throw EmulationError(std::format("native_driver: a hook could not be attached (unicorn error {})", e));
    }
    s.attached = true;
}

DriverRun::~DriverRun() {
    if (impl_ && impl_->handle != nullptr) {
        if (impl_->attached) cbt_drv_detach(impl_->handle);
        cbt_drv_free(impl_->handle);
    }
}

void DriverRun::before_host_start() { impl_->raised = nullptr; }

void DriverRun::after_host_start() {
    Impl& s = *impl_;
    s.sync_governor();
    if (s.raised) {
        std::exception_ptr e = s.raised;
        s.raised = nullptr;
        std::rethrow_exception(e);
    }
}

std::exception_ptr DriverRun::take_raised() {
    Impl& s = *impl_;
    s.sync_governor();
    std::exception_ptr e = s.raised;
    s.raised = nullptr;
    return e;
}

int DriverRun::start(std::uint64_t begin, std::size_t count) {
    Impl& s = *impl_;
    before_host_start();
    const uc_err status = tracer::ucl().emu_start(s.engine, begin, s.options.return_address, 0, count);
    after_host_start();
    return static_cast<int>(status);
}

namespace {

// Rows of `width` words in a flat array (the recorder keeps its fixed-width records that way).
template <std::size_t Width, class F>
void for_rows(const std::uint64_t* words, std::int64_t rows, F&& each) {
    for (std::int64_t i = 0; i < rows; ++i) each(words + static_cast<std::size_t>(i) * Width);
}

}  // namespace

DriverRecord DriverRun::collect() {
    Impl& s = *impl_;
    const cbt_drv_result& r = *cbt_drv_result_get(s.handle);
    s.options.governor->n = r.gov_n;
    DriverRecord rec;
    rec.insns = r.insns;
    rec.truncated = r.truncated != 0;
    rec.governed = r.governed != 0;
    rec.outside = r.outside;
    if (r.exit_code == 1) {
        rec.exit = DriverExit{DriverExit::Kind::Wild, 0};
    } else if (r.exit_code == 2) {
        rec.exit = DriverExit{DriverExit::Kind::ServiceUnreadable, 0};
    } else if (r.exit_code == 3) {
        rec.exit = DriverExit{DriverExit::Kind::Interrupt, static_cast<std::uint8_t>(r.exit_int)};
    }
    if (r.ncover > 0) rec.cover.assign(r.cover, r.cover + r.ncover);
    for_rows<CBT_DRV_CALL_WORDS>(r.calls, r.ncalls, [&](const std::uint64_t* w) {
        // none_first, first, second, nregs, regs[6], nwords, words[4]: a call through a pointer has no device
        DriverCall call;
        call.target.device = w[0] ? std::nullopt : std::optional<std::uint32_t>(static_cast<std::uint32_t>(w[1]));
        call.target.number = static_cast<std::uint32_t>(w[2]);
        const std::uint64_t nregs = std::min<std::uint64_t>(w[3], 6), nwords = std::min<std::uint64_t>(w[10], 4);
        call.registers.assign(w + 4, w + 4 + nregs);
        call.stack_words.assign(w + 11, w + 11 + nwords);
        rec.calls.push_back(std::move(call));
    });
    for_rows<CBT_DRV_PORT_WORDS>(r.ports, r.nports, [&](const std::uint64_t* w) {
        CB_ASSERT(w[0] < 2, "a port access kind");
        rec.ports.push_back({w[0] ? PortAccess::Kind::Out : PortAccess::Kind::In, w[1], w[2], w[3]});
    });
    for_rows<CBT_MEM_WORDS>(r.reads, r.nreads, [&](const std::uint64_t* w) {
        rec.mem_reads.push_back({w[0], w[1], w[2], w[4] ? std::nullopt : std::optional<std::uint64_t>(w[3]), w[5]});
    });
    for_rows<CBT_MEM_WORDS>(r.writes, r.nwrites, [&](const std::uint64_t* w) {
        rec.mem_writes.push_back({w[0], w[1], w[2], w[3], w[5], w[7]});
    });
    for_rows<CBT_LAZY_WORDS>(r.lazy, r.nlazy, [&](const std::uint64_t* w) {
        CB_ASSERT(w[3] < 4, "a lazy map's access kind");
        rec.lazy_maps.push_back({w[0], w[1], w[2], static_cast<native_trace::Access>(w[3]), w[4]});
    });
    if (r.memops >= 0) rec.memops = r.memops;
    if (r.writes_seen >= 0) rec.writes_seen = r.writes_seen;
    if (r.stuck >= 0) rec.stuck = r.stuck;
    return rec;
}

void DriverRun::detach() {
    Impl& s = *impl_;
    s.attached = false;
    if (cbt_drv_detach(s.handle)) throw EmulationError("native_driver: a driver hook could not be taken off");
}

}  // namespace cb::emulate::native_driver

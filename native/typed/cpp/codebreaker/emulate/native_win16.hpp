// codebreaker/emulate/native_win16.hpp -- COMPAT SHIM over cb/emulate/native_win16.hpp (docs/CPP_DESIGN.md 6.3), for the
// old emulate/windows16 port. Deleted with its last py:: consumer.
//
// The 16-bit recorder (the hooks, their limits, the record) is native/cb/emulate/native_win16.hpp, called in-process.
// Here: the old Run over a host Engine (its Unicorn handle), RunArgs, and the record as the optional fields of
// windows16's rec; nothing else. The host's Unicorn and Capstone DLLs are the ones native_trace::enabled() points the
// recorder at.
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/native_win16.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "py.hpp"

namespace codebreaker::emulate::native_win16 {

namespace ns = ::cb::emulate::native_win16;
namespace nt = codebreaker::emulate::native_trace;

inline constexpr int64_t _MEM_WORDS = 8, _LAZY_WORDS = 5;

inline bool enabled() { return nt::enabled(); }
inline std::optional<py::str> why_not() { return nt::why_not(); }

// The dict trace_function16 fills; collect() sets the keys below, a key that is not set is an empty optional.
struct Win16Record {
    std::optional<int64_t> insn_count;
    std::optional<bool> truncated;
    std::optional<bool> returned;
    std::optional<bool> limit_fault;
    std::optional<py::list<uint64_t>> path;
    std::optional<py::list<nt::MemRead>> mem_reads;
    std::optional<py::list<nt::MemWrite>> mem_writes;
    std::optional<py::list<nt::LazyMap>> lazy_maps;
    std::optional<int64_t> memops;
    std::optional<int64_t> writes_seen;
    std::optional<int64_t> _stuck;
};

// The arguments of Run (its keyword arguments).
struct RunArgs {
    int64_t eip_id = 0;
    bool near_ = false, fill = false;
    int64_t cap = 0;
    int64_t cs_base = 0, near_ret = 0, far_ret = 0, sent_lo = 0, sent_hi = 0, snap_base = 0;
    nt::Gov* gov = nullptr;
    py::list<uint64_t> intercept;
    std::function<void(int64_t, int64_t)> on_sentinel, on_intercept, on_intr;
    void* shadow = nullptr;
    std::optional<std::tuple<int64_t, int64_t>> own;
    std::function<bool(int64_t, int64_t)> on_own;
};

// One NE function's native hooks on a host engine.
class Run {
  public:
    Run(const nt::Engine& uc, const RunArgs& a) : gov_(a.gov) {
        require_enabled();
        ns::Options o;
        o.eip_id = static_cast<int>(a.eip_id);
        o.near_entry = a.near_;
        o.fill = a.fill;
        o.cap = a.cap;
        o.cs_base = static_cast<uint64_t>(a.cs_base);
        o.near_ret = static_cast<uint64_t>(a.near_ret);
        o.far_ret = static_cast<uint64_t>(a.far_ret);
        o.sent_lo = static_cast<uint64_t>(a.sent_lo);
        o.sent_hi = static_cast<uint64_t>(a.sent_hi);
        o.snap_base = static_cast<uint64_t>(a.snap_base);
        o.governor = &governor_;
        governor_.n = a.gov->n;
        governor_.ceil = a.gov->ceil;
        o.intercept.assign(a.intercept.vec().begin(), a.intercept.vec().end());
        o.shadow = static_cast<::cb::emulate::tracer::Shadow*>(a.shadow);
        if (a.own) o.own = ::cb::emulate::native_trace::AddressRange{static_cast<uint64_t>(std::get<0>(*a.own)), static_cast<uint64_t>(std::get<1>(*a.own))};
        o.handlers.on_sentinel = a.on_sentinel;
        o.handlers.on_intercept = a.on_intercept;
        o.handlers.on_interrupt = a.on_intr;
        o.handlers.on_own = a.on_own;
        run_ = std::make_unique<ns::Run>(static_cast<uc_struct*>(uc.uch), std::move(o));
    }

    void reset() { run_->reset(); }
    int64_t count() const { return run_->count(); }
    std::exception_ptr raised() {
        std::exception_ptr e = run_->raised();
        gov_->n = governor_.n;
        return e;
    }
    void collect(Win16Record& rec) {
        const ns::Trace t = run_->collect();
        gov_->n = governor_.n;
        rec.insn_count = t.insn_count;
        rec.truncated = t.truncated;
        if (t.returned) rec.returned = true;
        if (t.limit_fault) rec.limit_fault = true;
        py::list<uint64_t> path;
        for (uint64_t a : t.path) path.append(a);
        rec.path = path;
        py::list<nt::MemRead> reads;
        for (const auto& r : t.mem_reads) reads.append(nt::MemRead{r.pc, r.addr, r.size, r.value, r.n});
        rec.mem_reads = reads;
        py::list<nt::MemWrite> writes;
        for (const auto& w : t.mem_writes) writes.append(nt::MemWrite{w.pc, w.addr, w.size, w.value, w.n, w.old});
        rec.mem_writes = writes;
        py::list<nt::LazyMap> lazy;
        for (const auto& l : t.lazy_maps) lazy.append(nt::LazyMap{l.pc, l.addr, l.page, py::str(std::string(::cb::emulate::native_trace::to_text(l.access))), l.size});
        rec.lazy_maps = lazy;
        rec.memops = t.memops;
        rec.writes_seen = t.writes_seen;
        rec._stuck = t.stuck;
    }
    void detach() { run_->detach(); }

  private:
    static void require_enabled() {
        if (!enabled()) {
            std::optional<py::str> why = why_not();
            throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer): ") + (why ? *why : py::str("None")));
        }
    }
    nt::Gov* gov_;
    ::cb::emulate::native_trace::Governor governor_;
    std::unique_ptr<ns::Run> run_;
};

// The recorder is C++ only: raise when it cannot be used here, saying why.
inline void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer): ") + (why ? *why : py::str("None")));
    }
}

}  // namespace codebreaker::emulate::native_win16

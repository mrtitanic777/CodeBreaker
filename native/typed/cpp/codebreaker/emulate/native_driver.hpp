// codebreaker/emulate/native_driver.hpp -- SHIM over cb/emulate/native_driver.hpp (docs/CPP_DESIGN.md 6.3): the old Run, RunArgs
// and DriverRecord (py:: containers, optionals for "key present") over the new DriverRun, for the port that still uses it
// (emulate/driver). Conversion and forwarding only; deleted with its last py:: consumer.
//
// What the old header had and this one does not (no consumer used them): the loader of cbtrace.dll's C interface (_lib, Lib,
// _Config, _Result, _READY, _u64s, _words) -- the hooks are the in-process recorder now, called through the new DriverRun --
// and the register ids and instruction ids of RunArgs, which the new code takes from Unicorn's own headers (RunArgs keeps the
// fields so the caller need not change; they are ignored).
// The recorder calls the host's Unicorn and Capstone DLLs (native_trace::shim::use_host_libraries), as the 32-bit one does.
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/native_driver.hpp"
#include "codebreaker/emulate/native_trace.hpp"

namespace codebreaker::emulate::native_driver {

namespace nt = codebreaker::emulate::native_trace;
namespace nd = cb::emulate::native_driver;

// ---- the record (the keys collect() sets) ---------------------------------------------------------
// A "calls" entry: (first, second, regs, words); first is None when the call went to a pointer.
using Call = std::tuple<std::optional<uint64_t>, uint64_t, py::list<uint64_t>, py::list<uint64_t>>;
// A "ports" entry: ("in" | "out", port, size, value).
using Port = std::tuple<py::str, uint64_t, uint64_t, uint64_t>;

struct DriverRecord {
    std::optional<int64_t> insns;
    std::optional<bool> truncated;
    std::optional<bool> governed;
    std::optional<int64_t> outside;
    std::optional<py::str> exit;
    std::optional<py::set<uint64_t>> cover;
    std::optional<py::list<Call>> calls;
    std::optional<py::list<Port>> ports;
    std::optional<py::list<nt::MemRead>> mem_reads;
    std::optional<py::list<nt::MemWrite>> mem_writes;
    std::optional<py::list<nt::LazyMap>> lazy_maps;
    std::optional<int64_t> memops;
    std::optional<int64_t> writes_seen;
    std::optional<int64_t> _stuck;
};

// The arguments of Run (its keyword arguments). `reg_ids`, `ins_in`, `ins_out` are ignored (Unicorn's own headers give them);
// `spans` the image's [(lo, hi)]; `scratch` the scratch region (lo, hi); `gov` emulator._GOV; `answers(device, service)` -> the
// place in REGISTERS the service answers in (or -1).
struct RunArgs {
    py::list<int64_t> reg_ids;
    int64_t ins_in = 0, ins_out = 0;
    int64_t vm_addr = 0, ret = 0;
    std::tuple<int64_t, int64_t> scratch;
    py::list<std::tuple<int64_t, int64_t>> spans;
    int64_t cap = 0;
    int64_t service_eax = 0, service_carry = 0;
    nt::Gov* gov = nullptr;
    std::function<int64_t(int32_t, int32_t)> answers;
    void* shadow = nullptr;
};

// The recorder is C++ only (the Python hooks it replaced are gone): raise when the host's Unicorn cannot be used here, saying
// why.
inline bool enabled() {
    try {
        nt::shim::use_host_libraries();
        return true;
    } catch (...) {
        return false;
    }
}
inline std::optional<py::str> why_not() {
    try {
        nt::shim::use_host_libraries();
        return std::nullopt;
    } catch (const std::exception& e) {
        return py::str(std::string(e.what()));
    }
}
inline void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer): ") + (why ? *why : py::str("None")));
    }
}

// One driver run's native hooks on `uc`. Raises nothing itself; `enabled()` gates its use.
class Run {
  public:
    nt::Engine uc;
    nt::Gov* gov = nullptr;

    Run(const nt::Engine& uc_, const RunArgs& a) : uc(uc_), gov(a.gov) {
        require();
        nd::Options o;
        o.vm_address = static_cast<uint64_t>(a.vm_addr);
        o.return_address = static_cast<uint64_t>(a.ret);
        o.scratch = {static_cast<uint64_t>(std::get<0>(a.scratch)), static_cast<uint64_t>(std::get<1>(a.scratch))};
        for (const auto& s : a.spans.vec()) o.spans.push_back({static_cast<uint64_t>(std::get<0>(s)), static_cast<uint64_t>(std::get<1>(s))});
        o.cap = a.cap;
        o.service_eax = static_cast<uint32_t>(static_cast<uint64_t>(a.service_eax) & 0xFFFFFFFFull);
        o.service_carry = a.service_carry != 0;
        governor_.n = gov->n;
        governor_.ceil = gov->ceil;
        o.governor = &governor_;
        if (a.answers) {
            std::function<int64_t(int32_t, int32_t)> answers = a.answers;
            o.resolver = [answers](uint32_t device, uint32_t service) -> std::optional<unsigned> {
                const int64_t place = answers(static_cast<int32_t>(device), static_cast<int32_t>(service));
                return place < 0 ? std::nullopt : std::optional<unsigned>(static_cast<unsigned>(place));
            };
        }
        o.shadow = static_cast<cb::emulate::tracer::Shadow*>(a.shadow);
        impl_ = std::make_unique<nd::DriverRun>(reinterpret_cast<uc_struct*>(uc.uch), std::move(o));
    }
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;

    // The exception the resolver raised, if any (driver.run has no try around the hook, so a resolver that raised comes out of
    // emu_start as a UcError; this is the original).
    std::exception_ptr raised() {
        std::exception_ptr e = impl_->take_raised();
        gov->n = governor_.n;
        return e;
    }

    // The rec fields the Python hooks would have left, as data.
    void collect(DriverRecord& rec) {
        const nd::DriverRecord r = impl_->collect();
        gov->n = governor_.n;
        rec.insns = r.insns;
        rec.truncated = r.truncated;
        if (r.governed) rec.governed = true;
        rec.outside = r.outside;
        if (r.exit) rec.exit = cb::compat::to_py(r.exit->text());
        py::set<uint64_t> cover;
        for (uint64_t a : r.cover) cover.add(a);
        rec.cover = cover;
        py::list<Call> calls;
        for (const nd::DriverCall& c : r.calls) {
            py::list<uint64_t> regs, words;
            for (uint64_t v : c.registers) regs.append(v);
            for (uint64_t v : c.stack_words) words.append(v);
            calls.append(Call{c.target.device ? std::optional<uint64_t>(*c.target.device) : std::nullopt, c.target.number, regs, words});
        }
        rec.calls = calls;
        py::list<Port> ports;
        for (const nd::PortAccess& p : r.ports) ports.append(Port{py::str(p.kind == nd::PortAccess::Kind::Out ? "out" : "in"), p.port, p.size, p.value});
        rec.ports = ports;
        py::list<nt::MemRead> reads;
        for (const auto& x : r.mem_reads) reads.append(nt::MemRead{x.pc, x.addr, x.size, x.value, x.n});
        rec.mem_reads = reads;
        py::list<nt::MemWrite> writes;
        for (const auto& x : r.mem_writes) writes.append(nt::MemWrite{x.pc, x.addr, x.size, x.value, x.n, x.old});
        rec.mem_writes = writes;
        py::list<nt::LazyMap> lazy;
        for (const auto& x : r.lazy_maps) {
            lazy.append(nt::LazyMap{x.pc, x.addr, x.page, nt::_ACCESS[static_cast<int64_t>(x.access)], x.size});
        }
        rec.lazy_maps = lazy;
        if (r.memops) rec.memops = *r.memops;
        if (r.writes_seen) rec.writes_seen = *r.writes_seen;
        if (r.stuck) rec._stuck = *r.stuck;
    }

    void detach() { cb::compat::guard([&] { impl_->detach(); }); }

  private:
    cb::emulate::native_trace::Governor governor_;
    std::unique_ptr<nd::DriverRun> impl_;
};

}  // namespace codebreaker::emulate::native_driver

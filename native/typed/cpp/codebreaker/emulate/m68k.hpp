// codebreaker/emulate/m68k.hpp -- SHIM over cb/emulate/m68k.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a Bus over a
// py::bytearray with py:: callbacks, RunOptions, run / record / recorded) for the ports that still call it (verify/lanes/
// genesis). Conversion and forwarding only; deleted with its last py:: consumer.
//
// What the old header had and this one does not: the CPU class, its handler table and the flag and arithmetic helpers
// (no consumer used them; cb::emulate::m68k::Cpu is the interpreter), and the Stop exception (a stopped run is a Fault).
// A device read's value is cut to the access's size by the new Bus (the old one passed a negative or wide number on).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/m68k.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::m68k {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FX = 0x10, FN = 0x08, FZ = 0x04, FV = 0x02, FC = 0x01;
// the return address run() pushes (even, outside a cartridge's code): reaching it ends the run
inline constexpr int64_t RETURN = 0x00FFFFF0;

// (kind "r1" "r2" "r4" "w1" "w2" "w4", address, value) of one device access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t, int64_t)>;            // (address, size in bytes); empty: None
using IoWrite = std::function<void(int64_t, int64_t, int64_t)>;     // (address, size in bytes, value); empty: None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// Memory, big-endian, 24-bit addresses (a bytearray the caller shares); ranges handed to callbacks (each access logged).
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;

    // memory: the bytearray to use (shared, not copied), else `size` bytes of zeros
    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt, int64_t size = 0x1000000)
        : mem(memory ? *memory : py::bytearray::zeros(size)),
          impl_(std::make_shared<cb::emulate::m68k::Bus>(cb::emulate::m68k::Bus::over(
              cb::compat::guard([&] { return cb::ByteSpan(reinterpret_cast<uint8_t*>(mem.raw_mut().data()), mem.raw_mut().size()); })))) {}

    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite()) {
        io.append(IoRange(lo, hi, read, write));
        cb::emulate::m68k::Bus::ReadFn r;
        cb::emulate::m68k::Bus::WriteFn w;
        if (read) r = [read](uint32_t a, unsigned n) { return static_cast<uint32_t>(read(a, n) & 0xFFFFFFFF); };
        if (write) w = [write](uint32_t a, unsigned n, uint32_t v) { write(a, n, v); };
        impl_->map(cb::compat::int_from_py<uint32_t>(lo), cb::compat::int_from_py<uint32_t>(hi), std::move(r), std::move(w));
    }
    int64_t read(int64_t a, int64_t n) {
        const int64_t v = impl_->read(static_cast<uint32_t>(a & 0xFFFFFF), static_cast<unsigned>(n));
        sync();
        return v;
    }
    void write(int64_t a, int64_t n, int64_t v) {
        impl_->write(static_cast<uint32_t>(a & 0xFFFFFF), static_cast<unsigned>(n), static_cast<uint32_t>(v & 0xFFFFFFFF));
        sync();
    }

    // The bus the new interpreter runs on (its log is brought into `log` by sync()).
    cb::emulate::m68k::Bus& impl() { return *impl_; }
    void sync() {
        const auto& entries = impl_->log();
        for (; synced_ < entries.size(); ++synced_) {
            const auto& e = entries[synced_];
            log.append(IoAccess(cb::compat::to_py(e.text()), e.address, e.value));
        }
    }

  private:
    std::shared_ptr<cb::emulate::m68k::Bus> impl_;
    std::size_t synced_ = 0;
};

// run's record: {end: "return" | "stopped: <why>" | "limit", d: [8], a: [8], sr, steps, io}
struct RunRecord {
    py::str end;
    py::list<int64_t> d, a;
    int64_t sr = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as JSR would and run it until it returns.
// d, a: the registers to start from (None, or an empty list: all zero).
inline RunRecord run(const py::ref<Bus>& bus, int64_t entry, int64_t steps = MAX_STEPS,
                     const std::optional<py::list<int64_t>>& d = std::nullopt, const std::optional<py::list<int64_t>>& a = std::nullopt,
                     int64_t sp = 0x00FFFF00, int64_t sr = 0x2700) {
    namespace nm = cb::emulate::m68k;
    nm::RunOptions o;
    o.max_steps = steps < 0 ? 0 : static_cast<uint64_t>(steps);
    o.sp = static_cast<uint32_t>(sp & 0xFFFFFFFF);
    o.sr = static_cast<uint16_t>(sr & 0xFFFF);
    auto regs = [](const std::optional<py::list<int64_t>>& list) -> std::optional<std::array<uint32_t, 8>> {
        if (!list || list->size() == 0) return std::nullopt;
        std::array<uint32_t, 8> out{};
        for (size_t i = 0; i < out.size() && i < static_cast<size_t>(list->size()); ++i) out[i] = static_cast<uint32_t>((*list)[static_cast<int64_t>(i)] & 0xFFFFFFFF);
        return out;
    };
    o.d = regs(d);
    o.a = regs(a);
    const nm::RunResult r = nm::run((*bus).impl(), static_cast<uint32_t>(entry & 0xFFFFFFFF), o);
    (*bus).sync();
    RunRecord out;
    out.end = cb::compat::to_py(r.exit());
    for (uint32_t v : r.d) out.d.append(v);
    for (uint32_t v : r.a) out.a.append(v);
    out.sr = r.sr;
    out.steps = static_cast<int64_t>(r.steps);
    out.io = py::list<IoAccess>((*bus).log.vec());
    return out;
}

// run's record as the one run record (emulate/run.py): the registers (D0-D7 A0-A7 SR) its results,
// the bus's log its device accesses. What it wrote is in the memory it was given, which its
// lane reads (verify/lanes/image_lane.py).
inline emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    for (int64_t i = 0; i < 8; i++) results.set(py::percent(py::str("D%d"), i), emu_run::Register(out.d[i]));
    for (int64_t i = 0; i < 8; i++) results.set(py::percent(py::str("A%d"), i), emu_run::Register(out.a[i]));
    results.set(py::str("SR"), emu_run::Register(out.sr));
    py::list<emu_run::IO> io;
    for (const IoAccess& x : out.io) {
        emu_run::IO y;
        y.kind = std::get<0>(x);
        y.port = std::get<1>(x);
        y.value = std::get<2>(x);
        io.append(y);
    }
    return emu_run::Run(out.end, results, py::list<emu_run::Write>(), false, py::list<emu_run::Call>(), io, std::nullopt, std::nullopt,
                        py::list<int64_t>(), out.steps, out.end == py::str("limit"), py::str("big"), std::make_shared<RunRecord>(out));
}

// run(...)'s keywords
struct RunOptions {
    int64_t steps = MAX_STEPS;
    std::optional<py::list<int64_t>> d, a;
    int64_t sp = 0x00FFFF00;
    int64_t sr = 0x2700;
};

// The platform's runner (codebreaker/platforms): run(...) as a Run.
inline emu_run::Run recorded(const py::ref<Bus>& bus, int64_t entry, const RunOptions& kw = RunOptions()) {
    return record(run(bus, entry, kw.steps, kw.d, kw.a, kw.sp, kw.sr));
}

}  // namespace codebreaker::emulate::m68k

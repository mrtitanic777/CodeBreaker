// codebreaker/emulate/m6502.hpp -- SHIM over cb/emulate/m6502.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a Bus over a
// py::bytearray with py:: callbacks, the Memory variant, run / record / recorded and the run keywords) for the ports that
// still call it (verify/lanes/nes). Conversion and forwarding only; deleted with its last py:: consumer.
//
// What the old header had and this one does not: the CPU class (no consumer used it; cb::emulate::m6502::Cpu is the
// interpreter) and Stop (it was the Python's control-flow exception inside CPU.step, caught by run).
// A Bus over a bytearray needs the bytearray to be 64 KB (cb::UsageError otherwise, as py ValueError); a plain
// bytearray `Memory` runs with no device log, as before.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/m6502.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::m6502 {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t _N = 0x80, _V = 0x40, _U = 0x20, _B = 0x10, _D = 0x08, _I = 0x04, _Z = 0x02, _C = 0x01;
inline constexpr int64_t RETURN = 0xFFF0;          // where a called function's RTS lands

// (kind "r" or "w", address, value) of one I/O access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t)>;
using IoWrite = std::function<void(int64_t, int64_t)>;
// (lo, hi, read, write): an empty std::function is None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// 64 KB of memory (a bytearray the caller shares), with ranges handed to callbacks.
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;                        // (kind, address, value) of every I/O access

    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt)
        : mem(memory ? *memory : py::bytearray::zeros(0x10000)),
          impl_(std::make_shared<cb::emulate::m6502::Bus>(cb::emulate::m6502::Bus::over(cb::compat::guard([&] {
              return cb::ByteSpan(reinterpret_cast<uint8_t*>(mem.raw_mut().data()), mem.raw_mut().size());
          })))) {}

    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite()) {
        io.append(IoRange(lo, hi, read, write));
        cb::emulate::m6502::Bus::ReadFn r;
        cb::emulate::m6502::Bus::WriteFn w;
        if (read) r = [read](uint16_t a) { return static_cast<uint8_t>(read(a) & 0xFF); };
        if (write) w = [write](uint16_t a, uint8_t v) { write(a, v); };
        impl_->map(cb::compat::int_from_py<uint16_t>(lo), cb::compat::int_from_py<uint16_t>(hi), std::move(r), std::move(w));
    }
    int64_t getitem(int64_t a) {                   // bus[a]
        const int64_t v = impl_->read(cb::compat::int_from_py<uint16_t>(a));
        sync();
        return v;
    }
    void setitem(int64_t a, int64_t v) {           // bus[a] = v
        impl_->write(cb::compat::int_from_py<uint16_t>(a), static_cast<uint8_t>(v & 0xFF));
        sync();
    }

    // The bus the new interpreter runs on (its log is brought into `log` by sync()).
    cb::emulate::m6502::Bus& impl() { return *impl_; }
    void sync() {
        const auto& entries = impl_->log();
        for (; synced_ < entries.size(); ++synced_) {
            const auto& e = entries[synced_];
            log.append(IoAccess(py::str(e.kind == cb::emulate::m6502::DeviceAccess::Kind::Read ? "r" : "w"), e.address, e.value));
        }
    }

  private:
    std::shared_ptr<cb::emulate::m6502::Bus> impl_;
    std::size_t synced_ = 0;
};

// A Bus or a 64 KB bytearray.
using Memory = std::variant<py::bytearray, py::ref<Bus>>;

// run's record: {end: "return" | "stopped: <why>" | "limit", a, x, y, p, s, steps, io: [(kind, address, value)]}
struct RunRecord {
    py::str end;
    int64_t a = 0, x = 0, y = 0, p = 0, s = 0, steps = 0;
    py::list<IoAccess> io;
};

// run(...)'s keywords
struct RunOptions {
    int64_t a = 0, x = 0, y = 0, p = _U | _I;
    int64_t steps = MAX_STEPS;
    bool decimal = false;
};

// Call the function at `entry` as JSR would and run it until it returns.
// `memory` is a Bus or a 64 KB bytearray (changed in place: what the function wrote is in it).
inline RunRecord run(const Memory& memory, int64_t entry, int64_t a = 0, int64_t x = 0, int64_t y = 0, int64_t p = _U | _I,
                     int64_t steps = MAX_STEPS, bool decimal = false) {
    namespace nm = cb::emulate::m6502;
    nm::RunOptions o;
    o.a = static_cast<uint8_t>(a & 0xFF);
    o.x = static_cast<uint8_t>(x & 0xFF);
    o.y = static_cast<uint8_t>(y & 0xFF);
    o.p = static_cast<uint8_t>(p & 0xFF);
    o.max_steps = steps < 0 ? 0 : static_cast<uint64_t>(steps);
    o.decimal = decimal;
    const uint16_t at = static_cast<uint16_t>(entry & 0xFFFF);
    RunRecord out;
    auto fill = [&](const nm::RunResult& r) {
        out.end = cb::compat::to_py(r.exit());
        out.a = r.regs.a;
        out.x = r.regs.x;
        out.y = r.regs.y;
        out.p = r.regs.p;
        out.s = r.regs.s;
        out.steps = static_cast<int64_t>(r.steps);
    };
    if (const auto* bus = std::get_if<py::ref<Bus>>(&memory)) {
        fill(nm::run((*bus)->impl(), at, o));
        (*bus)->sync();
        out.io = py::list<IoAccess>((*bus)->log.vec());
    } else {
        const py::bytearray& ram = std::get<py::bytearray>(memory);
        nm::Bus plain = nm::Bus::over(cb::compat::guard([&] {
            return cb::ByteSpan(reinterpret_cast<uint8_t*>(ram.raw_mut().data()), ram.raw_mut().size());
        }));
        fill(nm::run(plain, at, o));
    }
    return out;
}

// run's record as the one run record (emulate/run.py): the registers (A X Y P S) its results,
// the bus's log its device accesses.
inline emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("A"), emu_run::Register(out.a));
    results.set(py::str("X"), emu_run::Register(out.x));
    results.set(py::str("Y"), emu_run::Register(out.y));
    results.set(py::str("P"), emu_run::Register(out.p));
    results.set(py::str("S"), emu_run::Register(out.s));
    py::list<emu_run::IO> io;
    for (const IoAccess& a : out.io) {
        emu_run::IO x;
        x.kind = std::get<0>(a);
        x.port = std::get<1>(a);
        x.value = std::get<2>(a);
        io.append(x);
    }
    return emu_run::Run(out.end, results, py::list<emu_run::Write>(), false, py::list<emu_run::Call>(), io, std::nullopt, std::nullopt,
                        py::list<int64_t>(), out.steps, out.end == py::str("limit"), py::str("little"), std::make_shared<RunRecord>(out));
}

// The platform's runner (codebreaker/platforms): run(...) as a Run.
inline emu_run::Run recorded(const Memory& memory, int64_t entry, const RunOptions& kw = RunOptions()) {
    return record(run(memory, entry, kw.a, kw.x, kw.y, kw.p, kw.steps, kw.decimal));
}

}  // namespace codebreaker::emulate::m6502

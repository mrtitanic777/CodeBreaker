// codebreaker/emulate/z80.hpp -- SHIM over cb/emulate/z80.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a Bus over a
// py::bytearray with py:: callbacks, the Memory variant, Stop, run / record / recorded and the run keywords) for the ports
// that still call it (emulate/sm83, verify/lanes/sms and z80_flat). Conversion and forwarding only; deleted with its last
// py:: consumer.
//
// What the old header had and this one does not: the CPU class and its handlers, operands and flag helpers (no consumer
// used them; cb::emulate::z80::Cpu is the interpreter).
// A Bus over a bytearray needs the bytearray to be 64 KB (cb::UsageError otherwise, as py ValueError); a plain
// bytearray `Memory` runs on a bus with no ports (IN reads 0xFF, nothing is logged), as before.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/z80.hpp"
#include "codebreaker/emulate/run.hpp"

namespace codebreaker::emulate::z80 {

namespace emu_run = ::codebreaker::emulate::run;   // (this namespace has a function run(), which hides the name)

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FS = 0x80, FZ = 0x40, FY = 0x20, FH = 0x10, FX = 0x08, FP = 0x04, FN = 0x02, FC = 0x01;
inline constexpr int64_t RETURN = 0xFFFF;          // the return address run() pushes: reaching it ends the run

// The old control-flow exception of the Python CPU (sm83 shares it).
class Stop : public py::Exception {
  public:
    Stop() : py::Exception() {}
    explicit Stop(const py::str& why) : py::Exception(why) {}
    const char* type() const override { return "Stop"; }
};

// (kind "r", "w", "in" or "out", address or port, value) of one I/O access
using IoAccess = std::tuple<py::str, int64_t, int64_t>;
using IoRead = std::function<int64_t(int64_t)>;
using IoWrite = std::function<void(int64_t, int64_t)>;
using PortIn = std::function<int64_t(int64_t)>;
using PortOut = std::function<void(int64_t, int64_t)>;
// (lo, hi, read, write): an empty std::function is None
using IoRange = std::tuple<int64_t, int64_t, IoRead, IoWrite>;

// 64 KB of memory (a bytearray the caller shares) and the ports; io ranges of memory go to callbacks.
class Bus {
  public:
    py::bytearray mem;
    py::list<IoRange> io;
    py::list<IoAccess> log;                        // (kind, address or port, value)

    explicit Bus(const std::optional<py::bytearray>& memory = std::nullopt, const PortIn& inp = PortIn(), const PortOut& outp = PortOut())
        : mem(memory ? *memory : py::bytearray::zeros(0x10000)),
          impl_(std::make_shared<cb::emulate::z80::Bus>(cb::emulate::z80::Bus::over(
              cb::compat::guard([&] { return cb::ByteSpan(reinterpret_cast<uint8_t*>(mem.raw_mut().data()), mem.raw_mut().size()); }),
              inp ? cb::emulate::z80::Bus::PortIn([inp](uint16_t p) { return static_cast<uint8_t>(inp(p) & 0xFF); })
                  : cb::emulate::z80::Bus::PortIn(),
              outp ? cb::emulate::z80::Bus::PortOut([outp](uint16_t p, uint8_t v) { outp(p, v); }) : cb::emulate::z80::Bus::PortOut()))) {}

    void map(int64_t lo, int64_t hi, const IoRead& read = IoRead(), const IoWrite& write = IoWrite()) {
        io.append(IoRange(lo, hi, read, write));
        cb::emulate::z80::Bus::ReadFn r;
        cb::emulate::z80::Bus::WriteFn w;
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
    int64_t inp(int64_t port) {
        const int64_t v = impl_->in(static_cast<uint16_t>(port & 0xFFFF));
        sync();
        return v;
    }
    void outp(int64_t port, int64_t v) {
        impl_->out(static_cast<uint16_t>(port & 0xFFFF), static_cast<uint8_t>(v & 0xFF));
        sync();
    }

    // The bus the new interpreter runs on (its log is brought into `log` by sync()).
    cb::emulate::z80::Bus& impl() { return *impl_; }
    void sync() {
        const auto& entries = impl_->log();
        for (; synced_ < entries.size(); ++synced_) {
            const auto& e = entries[synced_];
            log.append(IoAccess(cb::compat::to_py(cb::emulate::z80::to_text(e.kind)), e.where, e.value));
        }
    }

  private:
    std::shared_ptr<cb::emulate::z80::Bus> impl_;
    std::size_t synced_ = 0;
};

// A Bus or a 64 KB bytearray.
using Memory = std::variant<py::bytearray, py::ref<Bus>>;

// the registers run() sets before it starts (the Python's **regs: a field only when given)
struct Regs {
    std::optional<int64_t> a, f, b, c, d, e, h, l, ix, iy;
};

// run's record: {end: "return" | "stopped: <why>" | "limit", a, f, bc, de, hl, ix, iy, sp, steps, io}
struct RunRecord {
    py::str end;
    int64_t a = 0, f = 0, bc = 0, de = 0, hl = 0, ix = 0, iy = 0, sp = 0, steps = 0;
    py::list<IoAccess> io;
};

// run(...)'s keywords
struct RunOptions {
    int64_t steps = MAX_STEPS;
    int64_t sp = 0xDFF0;
    Regs regs;
};

// Call the function at `entry` as CALL would and run it until it returns.
// `memory` is a Bus or a 64 KB bytearray (changed in place).
inline RunRecord run(const Memory& memory, int64_t entry, int64_t steps = MAX_STEPS, int64_t sp = 0xDFF0, const Regs& regs = Regs()) {
    namespace nz = cb::emulate::z80;
    nz::RunOptions o;
    o.max_steps = steps < 0 ? 0 : static_cast<uint64_t>(steps);
    o.sp = static_cast<uint16_t>(sp & 0xFFFF);
    auto byte = [](const std::optional<int64_t>& v) { return v ? std::optional<uint8_t>(static_cast<uint8_t>(*v & 0xFF)) : std::nullopt; };
    auto word = [](const std::optional<int64_t>& v) { return v ? std::optional<uint16_t>(static_cast<uint16_t>(*v & 0xFFFF)) : std::nullopt; };
    o.regs = {byte(regs.a), byte(regs.f), byte(regs.b), byte(regs.c), byte(regs.d), byte(regs.e), byte(regs.h), byte(regs.l), word(regs.ix), word(regs.iy)};
    const uint16_t at = static_cast<uint16_t>(entry & 0xFFFF);
    RunRecord out;
    auto fill = [&](const nz::RunResult& r) {
        out.end = cb::compat::to_py(r.exit());
        out.a = r.regs.a;
        out.f = r.regs.f;
        out.bc = r.regs.bc();
        out.de = r.regs.de();
        out.hl = r.regs.hl();
        out.ix = r.regs.ix;
        out.iy = r.regs.iy;
        out.sp = r.regs.sp;
        out.steps = static_cast<int64_t>(r.steps);
    };
    if (const auto* bus = std::get_if<py::ref<Bus>>(&memory)) {
        fill(nz::run((*bus)->impl(), at, o));
        (*bus)->sync();
        out.io = py::list<IoAccess>((*bus)->log.vec());
    } else {
        const py::bytearray& ram = std::get<py::bytearray>(memory);
        nz::Bus plain = nz::Bus::bare(cb::compat::guard([&] { return cb::ByteSpan(reinterpret_cast<uint8_t*>(ram.raw_mut().data()), ram.raw_mut().size()); }));
        fill(nz::run(plain, at, o));
    }
    return out;
}

// run's record as the one run record (emulate/run.py): the registers (A F BC DE HL IX IY SP) its results,
// the bus's log its device accesses.
inline emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("A"), emu_run::Register(out.a));
    results.set(py::str("F"), emu_run::Register(out.f));
    results.set(py::str("BC"), emu_run::Register(out.bc));
    results.set(py::str("DE"), emu_run::Register(out.de));
    results.set(py::str("HL"), emu_run::Register(out.hl));
    results.set(py::str("IX"), emu_run::Register(out.ix));
    results.set(py::str("IY"), emu_run::Register(out.iy));
    results.set(py::str("SP"), emu_run::Register(out.sp));
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
    return record(run(memory, entry, kw.steps, kw.sp, kw.regs));
}

}  // namespace codebreaker::emulate::z80

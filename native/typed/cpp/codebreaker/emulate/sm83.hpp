// codebreaker/emulate/sm83.hpp -- SHIM over cb/emulate/sm83.hpp (docs/CPP_DESIGN.md 6.3): the old surface (the z80 shim's Bus,
// Stop and Memory, the run keywords, run / record / recorded) for the ports that still call it (verify/lanes/gb).
// Conversion and forwarding only; deleted with its last py:: consumer.
//
// What the old header had and this one does not: the CPU class, its handler compiler and the operand helpers (no consumer
// used them; cb::emulate::sm83::Cpu is the interpreter). The registers run() starts from are bytes (F keeps its high nibble).
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/sm83.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/z80.hpp"

namespace codebreaker::emulate::sm83 {

namespace emu_run = ::codebreaker::emulate::run;    // (this namespace has a function run(), which hides the name)

using Bus = ::codebreaker::emulate::z80::Bus;       // sm83.Bus: the same 64 KB bus with mapped ranges
using Stop = ::codebreaker::emulate::z80::Stop;
using Memory = ::codebreaker::emulate::z80::Memory; // a Bus or a 64 KB bytearray

inline constexpr int64_t MAX_STEPS = 200000;
inline constexpr int64_t FZ = 0x80, FN = 0x40, FH = 0x20, FC = 0x10;
inline constexpr int64_t RETURN = 0xFFFE;   // pushed by run(): reaching it ends the run

// the registers run() sets before it starts (the Python's **regs: a field only when given)
struct Regs {
    std::optional<int64_t> a, f, b, c, d, e, h, l;
    std::optional<bool> ime, halted;
};

// io of one run: (kind "r" or "w", address, value) of each access to a mapped range
using IoAccess = ::codebreaker::emulate::z80::IoAccess;

// run's record: {end: "return" | "stopped: <why>" | "limit", a, f, bc, de, hl, sp, steps, io}
struct RunRecord {
    py::str end;
    int64_t a = 0, f = 0, bc = 0, de = 0, hl = 0, sp = 0, steps = 0;
    py::list<IoAccess> io;
};

// Call the function at `entry` as CALL would and run it until it returns.
inline RunRecord run(const Memory& memory, int64_t entry, int64_t steps = MAX_STEPS, int64_t sp = 0xDFF0, const Regs& regs = Regs()) {
    namespace ns = cb::emulate::sm83;
    ns::RunOptions o;
    o.max_steps = steps < 0 ? 0 : static_cast<uint64_t>(steps);
    o.sp = static_cast<uint16_t>(sp & 0xFFFF);
    auto byte = [](const std::optional<int64_t>& v) { return v ? std::optional<uint8_t>(static_cast<uint8_t>(*v & 0xFF)) : std::nullopt; };
    o.regs = {byte(regs.a), byte(regs.f), byte(regs.b), byte(regs.c), byte(regs.d), byte(regs.e), byte(regs.h), byte(regs.l), regs.ime, regs.halted};
    const uint16_t at = static_cast<uint16_t>(entry & 0xFFFF);
    RunRecord out;
    auto fill = [&](const ns::RunResult& r) {
        out.end = cb::compat::to_py(r.exit());
        out.a = r.regs.a;
        out.f = r.regs.f;
        out.bc = r.regs.bc();
        out.de = r.regs.de();
        out.hl = r.regs.hl();
        out.sp = r.regs.sp;
        out.steps = static_cast<int64_t>(r.steps);
    };
    if (const auto* bus = std::get_if<py::ref<::codebreaker::emulate::z80::Bus>>(&memory)) {
        fill(ns::run((*bus)->impl(), at, o));
        (*bus)->sync();
        out.io = py::list<IoAccess>((*bus)->log.vec());
    } else {
        const py::bytearray& ram = std::get<py::bytearray>(memory);
        auto plain = cb::emulate::z80::Bus::bare(cb::compat::guard([&] { return cb::ByteSpan(reinterpret_cast<uint8_t*>(ram.raw_mut().data()), ram.raw_mut().size()); }));
        fill(ns::run(plain, at, o));
    }
    return out;
}

// run's record as the one run record (emulate/run.py): the registers (A F BC DE HL SP) its results,
// the bus's log its device accesses. What it wrote is in the memory it was given, which its
// lane reads (verify/lanes/image_lane.py).
inline emu_run::Run record(const RunRecord& out) {
    py::dict<py::str, emu_run::Register> results;
    results.set(py::str("A"), emu_run::Register(out.a));
    results.set(py::str("F"), emu_run::Register(out.f));
    results.set(py::str("BC"), emu_run::Register(out.bc));
    results.set(py::str("DE"), emu_run::Register(out.de));
    results.set(py::str("HL"), emu_run::Register(out.hl));
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

// run(...)'s keywords
struct RunOptions {
    int64_t steps = MAX_STEPS;
    int64_t sp = 0xDFF0;
    Regs regs;
};

// The platform's runner (codebreaker/platforms): run(...) as a Run.
inline emu_run::Run recorded(const Memory& memory, int64_t entry, const RunOptions& kw = RunOptions()) {
    return record(run(memory, entry, kw.steps, kw.sp, kw.regs));
}

}  // namespace codebreaker::emulate::sm83

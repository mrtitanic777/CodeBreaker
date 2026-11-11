// The shims at codebreaker/emulate/{m6502,z80,powerpc,native_driver}.hpp answer what the old layer's callers expect, through
// the new interpreters.
#include "codebreaker/emulate/m6502.hpp"
#include "codebreaker/emulate/native_driver.hpp"
#include "codebreaker/emulate/powerpc.hpp"
#include "codebreaker/emulate/z80.hpp"
#include "cb/core/testing.hpp"

namespace om6502 = codebreaker::emulate::m6502;
namespace oz80 = codebreaker::emulate::z80;
namespace oppc = codebreaker::emulate::powerpc;

CB_TEST("compat.emulate_cpu_shims.m6502_bus_shares_the_callers_memory", [] {
    py::bytearray mem = py::bytearray::zeros(0x10000);
    const unsigned char code[] = {0xAD, 0x02, 0x20, 0x8D, 0x10, 0x00, 0x60};   // LDA $2002 ; STA $10 ; RTS
    for (size_t i = 0; i < sizeof code; ++i) mem[0x8000 + static_cast<int64_t>(i)] = code[i];
    auto bus = std::make_shared<om6502::Bus>(mem);
    bus->map(0x2000, 0x2007, [](int64_t a) -> int64_t { return a == 0x2002 ? 0x80 : 0; });
    om6502::RunOptions kw;
    kw.x = 3;
    const auto run = om6502::recorded(om6502::Memory(bus), 0x8000, kw);
    CB_CHECK(run.exit && *run.exit == py::str("return"));
    CB_CHECK_EQ(static_cast<int64_t>(mem[0x10]), 0x80);                       // what the function wrote is in the caller's bytearray
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("A"))), 0x80);
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("X"))), 3);
    CB_CHECK_EQ(py::len(run.io), 1);
    CB_CHECK(run.io[0].kind == py::str("r"));
    CB_CHECK_EQ(py::len(bus->log), 1);                                       // the old log member is kept up
    CB_CHECK_EQ(bus->getitem(0x2002), 0x80);
    CB_CHECK_EQ(py::len(bus->log), 2);
    // a plain bytearray as memory
    const auto r2 = om6502::run(om6502::Memory(mem), 0x8000);
    CB_CHECK(r2.end == py::str("return"));
});

CB_TEST("compat.emulate_cpu_shims.z80_bus_and_run", [] {
    py::bytearray mem = py::bytearray::zeros(0x10000);
    const unsigned char code[] = {0xDB, 0x10, 0x32, 0x00, 0x90, 0xC9};   // IN A,($10) ; LD ($9000),A ; RET
    for (size_t i = 0; i < sizeof code; ++i) mem[0x8000 + static_cast<int64_t>(i)] = code[i];
    auto bus = std::make_shared<oz80::Bus>(mem, [](int64_t port) -> int64_t { return (port & 0xFF) == 0x10 ? 0x42 : 0xFF; });
    const auto run = oz80::recorded(oz80::Memory(bus), 0x8000);
    CB_CHECK(run.exit && *run.exit == py::str("return"));
    CB_CHECK_EQ(static_cast<int64_t>(mem[0x9000]), 0x42);
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("A"))), 0x42);
    CB_CHECK_EQ(py::len(run.io), 1);
    CB_CHECK(run.io[0].kind == py::str("in"));
    CB_CHECK_EQ(run.io[0].port, 0x10);
    // the sm83 port shares this Bus: its getitem / setitem and log
    bus->setitem(0x20, 5);
    CB_CHECK_EQ(bus->getitem(0x20), 5);
    // plain memory: IN reads 0xFF and nothing is logged
    oz80::RunOptions kw;
    kw.regs.b = 1;
    const auto r2 = oz80::run(oz80::Memory(mem), 0x8000, kw.steps, kw.sp, kw.regs);
    CB_CHECK_EQ(py::len(r2.io), 0);
    CB_CHECK_EQ(r2.a, 0xFF);
});

CB_TEST("compat.emulate_cpu_shims.powerpc_run_pages_and_machine_view", [] {
    py::list<oppc::Region> regions;
    const std::string words("\x38\x60\x00\x07\x4E\x80\x00\x20", 8);   // li r3,7 ; blr
    regions.append(oppc::Region(0x10000000, py::bytes(words)));
    py::list<uint64_t> args;
    oppc::RunOptions kw;
    kw.span = std::make_tuple<int64_t, int64_t>(0x10000000, 0x10000100);
    kw.branches = true;
    const oppc::Out out = oppc::run(regions, 0x10000000, args, kw);
    CB_CHECK(out.exit == py::str("ret"));
    CB_CHECK_EQ(out.ret, 7u);
    CB_CHECK_EQ(out.steps, 2);
    CB_CHECK_EQ(py::len(out.path), 2);
    CB_CHECK(out.saved);
    CB_CHECK(out.branches.has_value());
    CB_CHECK_EQ(out.machine->read(0x10000000, 4), 0x38600007u);
    CB_CHECK_EQ(out.machine->r[3], 7u);                                      // the registers are copied in
    CB_CHECK(out.machine->pages.get(0x10000).has_value());                   // written (loaded) pages
    CB_CHECK(!out.machine->shared.get(0x10000).has_value());
    // shared pages
    const oppc::Pages shared = oppc::pages(regions);
    oppc::RunOptions s;
    s.shared = shared;
    const oppc::Out q = oppc::run(py::list<oppc::Region>(), 0x10000000, args, s);
    CB_CHECK_EQ(q.ret, 7u);
    CB_CHECK(q.machine->shared.get(0x10000).has_value());
    CB_CHECK(!q.machine->pages.get(0x10000).has_value());
    // the record
    const auto rec = oppc::record(out);
    CB_CHECK(rec.exit && *rec.exit == py::str("ret"));
    CB_CHECK(rec.endian == py::str("big"));
    CB_CHECK_EQ(std::get<int64_t>(rec.results.at(py::str("r3"))), 7);
    CB_CHECK(rec.kept());
    CB_CHECK_EQ(py::len(oppc::filled(py::str("counts"), 8)), 8);
    CB_CHECK_EQ(py::len(oppc::filled(py::str("bogus"), 8)), 0);
    // a Shadow sees the steps through the old Machine view
    struct Log : oppc::Shadow {
        int steps = 0;
        uint64_t last_pc = 0;
        void step(oppc::Machine& m, int64_t pc) override {
            ++steps;
            last_pc = static_cast<uint64_t>(m.pc);
            CB_CHECK_EQ(m.pc, pc);
        }
        void after_call(oppc::Machine&, bool, uint64_t) override {}
    } log;
    oppc::RunOptions w;
    w.shadow = &log;
    oppc::run(regions, 0x10000000, args, w);
    CB_CHECK_EQ(log.steps, 2);
});

CB_TEST("compat.emulate_cpu_shims.native_driver_types", [] {
    namespace nd = codebreaker::emulate::native_driver;
    nd::DriverRecord rec;
    CB_CHECK(!rec.insns.has_value());
    nd::RunArgs a;
    CB_CHECK(a.gov == nullptr);
});

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

#include "codebreaker/emulate/m68k.hpp"
#include "codebreaker/emulate/sm83.hpp"

CB_TEST("compat.emulate_cpu_shims.m68k_bus_and_run", [] {
    namespace om68k = codebreaker::emulate::m68k;
    py::bytearray mem = py::bytearray::zeros(0x10000);
    const unsigned char code[] = {0x30, 0x39, 0x00, 0xC0, 0x00, 0x04, 0x4E, 0x75};   // MOVE.W $C00004,D0 ; RTS
    for (size_t i = 0; i < sizeof code; ++i) mem[0x200 + static_cast<int64_t>(i)] = code[i];
    auto bus = std::make_shared<om68k::Bus>(mem);
    bus->map(0xC00000, 0xC0000F, [](int64_t a, int64_t) -> int64_t { return (a & ~int64_t(1)) == 0xC00004 ? 0x0008 : 0; });
    om68k::RunOptions kw;
    kw.sp = 0x8000;
    const auto run = om68k::recorded(bus, 0x200, kw);
    CB_CHECK(run.exit && *run.exit == py::str("return"));
    CB_CHECK(run.endian == py::str("big"));
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("D0"))), 8);
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("A7"))), 0x8000);
    CB_CHECK_EQ(py::len(run.io), 1);
    CB_CHECK(run.io[0].kind == py::str("r2"));
    CB_CHECK_EQ(run.io[0].port, 0xC00004);
    CB_CHECK_EQ(py::len(bus->log), 1);                                       // the old log member is kept up
    // the bus shares the caller's bytearray, big-endian
    bus->write(0x20, 2, 0x1234);
    CB_CHECK_EQ(static_cast<int64_t>(mem[0x20]), 0x12);
    CB_CHECK_EQ(bus->read(0x20, 2), 0x1234);
    // registers given as lists
    py::list<int64_t> d;
    for (int i = 0; i < 8; i++) d.append(i + 1);
    om68k::RunOptions k2;
    k2.sp = 0x8000;
    k2.d = d;
    const auto r2 = om68k::run(std::make_shared<om68k::Bus>(mem), 0x200, k2.steps, k2.d, k2.a, k2.sp, k2.sr);
    CB_CHECK(r2.end == py::str("return"));
    CB_CHECK_EQ(r2.d[1], 2);
});

CB_TEST("compat.emulate_cpu_shims.sm83_run_on_a_bus_and_on_plain_memory", [] {
    namespace osm = codebreaker::emulate::sm83;
    py::bytearray mem = py::bytearray::zeros(0x10000);
    const unsigned char code[] = {0xF0, 0x44, 0xEA, 0x00, 0xC0, 0xC9};   // LDH A,($FF44) ; LD ($C000),A ; RET
    for (size_t i = 0; i < sizeof code; ++i) mem[0x8000 + static_cast<int64_t>(i)] = code[i];
    auto bus = std::make_shared<osm::Bus>(mem);
    bus->map(0xFF00, 0xFF7F, [](int64_t a) -> int64_t { return a == 0xFF44 ? 0x90 : 0; });
    const auto run = osm::recorded(osm::Memory(bus), 0x8000);
    CB_CHECK(run.exit && *run.exit == py::str("return"));
    CB_CHECK_EQ(static_cast<int64_t>(mem[0xC000]), 0x90);                    // what the function wrote is in the caller's bytearray
    CB_CHECK_EQ(std::get<int64_t>(run.results.at(py::str("A"))), 0x90);
    CB_CHECK_EQ(py::len(run.io), 1);
    CB_CHECK(run.io[0].kind == py::str("r"));
    CB_CHECK_EQ(run.io[0].port, 0xFF44);
    // plain memory: no bus, no log
    const auto r2 = osm::run(osm::Memory(mem), 0x8000);
    CB_CHECK(r2.end == py::str("return"));
    CB_CHECK_EQ(py::len(r2.io), 0);
    // the registers it starts from
    osm::RunOptions kw;
    kw.regs.b = 7;
    const unsigned char add[] = {0x80, 0xC9};   // ADD A,B ; RET
    for (size_t i = 0; i < sizeof add; ++i) mem[0x9000 + static_cast<int64_t>(i)] = add[i];
    const auto r3 = osm::run(osm::Memory(mem), 0x9000, kw.steps, kw.sp, kw.regs);
    CB_CHECK_EQ(r3.a, 7);
});

// cb/emulate/native_driver_test.cpp -- the driver recorder on a real Unicorn engine: a small flat ring-0 function that
// writes a port, calls a service of a device through INT 20h (twice) and returns is run, and what was recorded is checked
// against what the code does.
//
// DIVERGES from the Python module: the result is typed (the exit is a DriverExit, the cover a vector in order of first
// arrival rather than a set, the calls carry a ServiceCall); the register and instruction ids come from Unicorn's headers
// instead of the caller.
#include "cb/emulate/native_driver.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

using namespace cb;
using namespace cb::emulate::native_driver;
namespace nt = cb::emulate::native_trace;

namespace {

constexpr std::uint64_t kCode = 0xC0001000;
constexpr std::uint64_t kStack = 0xBFFF0000;
constexpr std::uint64_t kReturn = 0xDEAD0000;
constexpr std::uint64_t kVm = 0x12345678;

// mov eax,5 ; out 80h,al ; int 20h (service $0010, device $0001) ; int 20h (the same) ; ret
const Bytes kFunction = {
    0xB8, 0x05, 0x00, 0x00, 0x00,   // C0001000 mov eax,5
    0xE6, 0x80,                     // C0001005 out 80h,al
    0xCD, 0x20, 0x10, 0x00, 0x01, 0x00,   // C0001007 int 20h ; dw service ; dw device
    0xCD, 0x20, 0x10, 0x00, 0x01, 0x00,   // C000100D the same
    0xC3,                           // C0001013 ret
};

struct Engine {
    uc_engine* engine = nullptr;
    Engine() {
        const auto& uc = emu::unicorn::api();
        emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_32, &engine), "uc_open");
        emu::unicorn::check(uc.uc_mem_map(engine, kCode, 0x1000, UC_PROT_ALL), "map code");
        emu::unicorn::check(uc.uc_mem_write(engine, kCode, kFunction.data(), kFunction.size()), "write code");
        emu::unicorn::check(uc.uc_mem_map(engine, kStack, 0x1000, UC_PROT_ALL), "map stack");
        std::uint32_t esp = static_cast<std::uint32_t>(kStack + 0x800);
        const std::uint32_t ret = static_cast<std::uint32_t>(kReturn);
        uc.uc_mem_write(engine, esp, &ret, 4);
        uc.uc_reg_write(engine, UC_X86_REG_ESP, &esp);
    }
    ~Engine() { emu::unicorn::api().uc_close(engine); }
    std::uint64_t reg(int id) {
        std::uint64_t v = 0;
        emu::unicorn::api().uc_reg_read(engine, id, &v);
        return v;
    }
};

Options base(nt::Governor& gov) {
    Options o;
    o.vm_address = kVm;
    o.return_address = kReturn;
    o.scratch = {0x20000000, 0x20010000};
    o.spans = {{kCode, kCode + 0x1000}};
    o.cap = 1000;
    o.service_eax = 0x77;
    o.service_carry = true;
    o.governor = &gov;
    return o;
}

}  // namespace

CB_TEST("emulate.native_driver.records_a_run", [] {
    Engine e;
    nt::Governor gov{0, 100000};
    Options o = base(gov);
    std::vector<std::pair<std::uint32_t, std::uint32_t>> asked;
    o.resolver = [&](std::uint32_t device, std::uint32_t service) -> std::optional<unsigned> {
        asked.emplace_back(device, service);
        return 3;   // the service answers in EDX
    };
    DriverRun run(e.engine, o);
    CB_CHECK_EQ(run.start(kCode, 1001), 0);
    const DriverRecord rec = run.collect();
    run.detach();
    CB_CHECK(!rec.exit.has_value());
    CB_CHECK(!rec.truncated);
    CB_CHECK_EQ(rec.insns, 5);                              // mov, out, int, int, ret
    CB_CHECK_EQ(gov.n, 0);                                   // the governor counts pages mapped on demand: none here
    CB_CHECK_EQ(asked.size(), 1u);                          // asked once per distinct service
    CB_CHECK_EQ(asked[0].first, 1u);
    CB_CHECK_EQ(asked[0].second, 0x10u);
    CB_CHECK_EQ(rec.calls.size(), 2u);
    CB_CHECK(rec.calls[0].target.device == 1u);
    CB_CHECK_EQ(rec.calls[0].target.number, 0x10u);
    CB_CHECK_EQ(rec.calls[0].registers.size(), 6u);
    CB_CHECK_EQ(rec.calls[0].registers[0], 5u);             // eax at the call
    CB_CHECK_EQ(rec.ports.size(), 1u);
    CB_CHECK(rec.ports[0].kind == PortAccess::Kind::Out);
    CB_CHECK_EQ(rec.ports[0].port, 0x80u);
    CB_CHECK_EQ(rec.ports[0].size, 1u);
    CB_CHECK_EQ(rec.ports[0].value, 5u);
    // the first service answered EAX = service_eax, carry set, EDX = the VM marker
    CB_CHECK_EQ(e.reg(UC_X86_REG_EAX), 0x77u);
    CB_CHECK_EQ(e.reg(UC_X86_REG_EDX), kVm);
    CB_CHECK(e.reg(UC_X86_REG_EFLAGS) & 1);
    CB_CHECK_EQ(rec.cover.size(), 5u);
    CB_CHECK_EQ(rec.cover.front(), kCode);
    CB_CHECK_EQ(rec.outside, 0);
});

CB_TEST("emulate.native_driver.a_resolver_that_throws_comes_out_of_the_run", [] {
    Engine e;
    nt::Governor gov{0, 100000};
    Options o = base(gov);
    o.resolver = [](std::uint32_t, std::uint32_t) -> std::optional<unsigned> { throw UsageError("no such service"); };
    DriverRun run(e.engine, o);
    CB_CHECK_THROWS_AS(UsageError, run.start(kCode, 1001));
    run.detach();
});

CB_TEST("emulate.native_driver.no_resolver_answers_in_eax", [] {
    Engine e;
    nt::Governor gov{0, 100000};
    DriverRun run(e.engine, base(gov));
    CB_CHECK_EQ(run.start(kCode, 1001), 0);
    run.collect();
    CB_CHECK_EQ(e.reg(UC_X86_REG_EDX), 0u);
    run.detach();
});

CB_TEST("emulate.native_driver.cap_truncates", [] {
    Engine e;
    nt::Governor gov{0, 100000};
    Options o = base(gov);
    o.cap = 2;
    DriverRun run(e.engine, o);
    run.start(kCode, 3);
    const DriverRecord rec = run.collect();
    run.detach();
    CB_CHECK(rec.insns <= 3);
    CB_CHECK(rec.calls.empty());   // it was cut off before the first INT 20h
});

CB_TEST("emulate.native_driver.exit_text", [] {
    DriverExit wild;
    wild.kind = DriverExit::Kind::Wild;
    CB_CHECK_EQ(wild.text(), "wild");
    DriverExit unreadable;
    unreadable.kind = DriverExit::Kind::ServiceUnreadable;
    CB_CHECK_EQ(unreadable.text(), "exception: service number unreadable");
    DriverExit interrupt;
    interrupt.kind = DriverExit::Kind::Interrupt;
    interrupt.interrupt = 0x21;
    CB_CHECK_EQ(interrupt.text(), "int:21");
    interrupt.interrupt = 5;
    CB_CHECK_EQ(interrupt.text(), "int:05");
});

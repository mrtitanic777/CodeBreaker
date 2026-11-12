// The shim at codebreaker/emulate/native_win16.hpp: the old Run over a host Engine and the old record, through the new
// recorder, on a real engine in real mode. Builds at /W3: the old layer is not warning-clean.
#include <windows.h>   // (before the shim: it declares the two kernel32 calls itself unless windows.h is in)

#include "codebreaker/emulate/native_win16.hpp"

#include "cb/core/dynlib.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"
#include "cb/core/capstone_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

// (codebreaker::emulate::unicorn_uc::x86_register_table is defined by native_trace_shim_test.cpp)

namespace nt = codebreaker::emulate::native_trace;
namespace nw = codebreaker::emulate::native_win16;

CB_TEST("compat.native_win16_shim.run_records_in_the_old_shapes", [] {
    cb::fs::ScopedEnv uc_dll("CB_UNICORN_DLL", cb::fs::to_text(cb::emu::unicorn::library().path()));
    const auto& uc = cb::emu::unicorn::api();
    uc_engine* engine = nullptr;
    cb::emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_16, &engine), "uc_open");
    for (uint64_t at : {0x1000ull, 0xF000ull, 0x3000ull, 0x7000ull}) cb::emu::unicorn::check(uc.uc_mem_map(engine, at, 0x1000, UC_PROT_ALL), "map");
    const uint8_t nops[1] = {0x90};
    uc.uc_mem_write(engine, 0xF000, nops, 1);
    // mov ax,5 ; mov [3000h],ax ; inc ax ; ret
    const uint8_t code[] = {0xB8, 0x05, 0x00, 0xA3, 0x00, 0x30, 0x40, 0xC3};
    uc.uc_mem_write(engine, 0x1000, code, sizeof code);
    uint16_t sp = 0x7FF0;
    const uint16_t ret = 0xF000;
    uc.uc_mem_write(engine, sp, &ret, sizeof ret);
    uc.uc_reg_write(engine, UC_X86_REG_SP, &sp);

    nt::set_engine(py::str(cb::emu::unicorn::library().path().generic_string()),
                   py::str(cb::disasm::capstone::library().path().generic_string()), py::dict<py::str, int64_t>());
    CB_CHECK(nw::enabled());
    nw::require();
    nt::Engine host;
    host.uch = engine;
    nt::Gov gov;
    gov.ceil = 100;
    nw::RunArgs a;
    a.eip_id = UC_X86_REG_EIP;
    a.near_ = true;
    a.cap = 100;
    a.near_ret = 0xF000;
    a.far_ret = 0xFFFFF0;
    a.sent_lo = 0xE000;
    a.sent_hi = 0xF000 - 1;
    a.snap_base = 0x100000;
    a.gov = &gov;
    {
        nw::Run run(host, a);
        uc.uc_emu_start(engine, 0x1000, 0xF800, 0, 0);
        CB_CHECK(!run.raised());
        CB_CHECK_EQ(run.count(), 4);
        nw::Win16Record rec;
        run.collect(rec);
        CB_CHECK(rec.insn_count && *rec.insn_count == 4);
        CB_CHECK(rec.truncated && !*rec.truncated);
        CB_CHECK(rec.returned && *rec.returned);
        CB_CHECK(!rec.limit_fault);
        CB_CHECK(rec.path && rec.path->size() == 4);
        CB_CHECK(rec.mem_writes && rec.mem_writes->size() == 1);
        CB_CHECK((*rec.mem_writes)[0].addr == 0x3000 && (*rec.mem_writes)[0].value == 5);
        CB_CHECK(rec.mem_reads && rec.mem_reads->size() == 1);   // (the return address popped by ret)
        CB_CHECK(rec.lazy_maps && rec.lazy_maps->size() == 0);
        run.detach();
    }
    uc.uc_close(engine);
});

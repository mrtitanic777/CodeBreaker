// The shim at codebreaker/emulate/native_trace.hpp: the old Run / RunArgs / TraceRecord over the new recorder, on a
// real engine. The old lookup the shim calls (where unicorn.dll is) is pointed at cb's own DLL (CB_UNICORN_DLL), so the
// engine the test makes and the recorder's table are one Unicorn.
#include <windows.h>   // (before the shim: it declares the two kernel32 calls itself unless windows.h is in)

#include "codebreaker/emulate/native_trace.hpp"

#include "cb/core/dynlib.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"
#include "cb/core/capstone_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

namespace codebreaker::emulate::unicorn_uc {
py::dict<py::str, int64_t> x86_register_table() { return py::dict<py::str, int64_t>(); }   // (the recorder has its own)
}  // namespace codebreaker::emulate::unicorn_uc

namespace nt = codebreaker::emulate::native_trace;

CB_TEST("compat.native_trace_shim.run_records_in_the_old_shapes", [] {
    // the shim's recorder gets the Unicorn base/capstone_dll finds: cb's own, the one this test's engine is made by
    // (looked for once per process; no other test asks for it)
    cb::fs::ScopedEnv uc_dll("CB_UNICORN_DLL", cb::fs::to_text(cb::emu::unicorn::library().path()));
    const auto& uc = cb::emu::unicorn::api();
    uc_engine* engine = nullptr;
    cb::emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_32, &engine), "uc_open");
    auto map = [&](uint64_t at) { cb::emu::unicorn::check(uc.uc_mem_map(engine, at, 0x1000, UC_PROT_ALL), "map"); };
    map(0x401000);
    map(0x500000);
    map(0x7F000000);
    // mov eax,[esp+4] ; mov [500000h],eax ; inc eax ; ret
    const uint8_t code[] = {0x8B, 0x44, 0x24, 0x04, 0xA3, 0x00, 0x00, 0x50, 0x00, 0x40, 0xC3};
    uc.uc_mem_write(engine, 0x401000, code, sizeof code);
    const uint32_t frame[2] = {0x600000, 41};
    uc.uc_mem_write(engine, 0x7F000100, frame, sizeof frame);
    uint32_t esp = 0x7F000100;
    uc.uc_reg_write(engine, UC_X86_REG_ESP, &esp);

    nt::set_engine(py::str(cb::emu::unicorn::library().path().generic_string()),
                   py::str(cb::disasm::capstone::library().path().generic_string()), py::dict<py::str, int64_t>());
    CB_CHECK(nt::enabled());
    nt::Engine host;
    host.uch = engine;
    host.emu_start = [&](int64_t begin, int64_t until, int64_t count) {
        cb::emu::unicorn::check(uc.uc_emu_start(engine, static_cast<uint64_t>(begin), static_cast<uint64_t>(until), 0, static_cast<size_t>(count)), "emu_start");
    };
    nt::Gov gov;
    gov.ceil = 1000000;
    nt::RunArgs a;
    a.gov = &gov;
    a.sha = py::str("shim-test");
    a.insn_cap = 100;
    a.func = {0x401000, 0x40100B};
    a.sec = {0x400000, 0x402000};
    a.lean_ranges = {0x7EFFFFFF, 0x7F000000};
    a.reads = true;
    {
        nt::Run run(host, a);
        run.start(host, 0x401000, 0x600000, 101);
        nt::TraceRecord rec;
        run.settle(rec);
        run.collect(rec);
        CB_CHECK(rec.insn_count && *rec.insn_count == 4);
        CB_CHECK(rec.truncated && !*rec.truncated);
        CB_CHECK(rec.path && rec.path->size() == 4);
        CB_CHECK(rec.mnemonics && rec.mnemonics->get(0x401000)->utf8() == "mov eax, dword ptr [esp + 4]");
        CB_CHECK(rec.mem_reads && rec.mem_reads->size() >= 1);
        CB_CHECK(rec.mem_writes && std::holds_alternative<py::list<nt::MemWrite>>(*rec.mem_writes));
        bool wrote = false;
        for (const nt::MemWrite& w : std::get<py::list<nt::MemWrite>>(*rec.mem_writes)) wrote = wrote || (w.addr == 0x500000 && w.value == 41);
        CB_CHECK(wrote);
        CB_CHECK(!rec.branches.has_value());                    // not asked for: the key is absent
        CB_CHECK(!rec.pages0.has_value());                      // lean only
        CB_CHECK(rec.calls && rec.calls->size() == 0);
        CB_CHECK(gov.n == 4);
        run.detach();
    }
    uint64_t eax = 0;
    uc.uc_reg_read(engine, UC_X86_REG_EAX, &eax);
    CB_CHECK_EQ(eax, 42u);
    uc.uc_close(engine);
});

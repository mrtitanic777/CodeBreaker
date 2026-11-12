// The 16-bit run recorder on a real Unicorn engine in real mode (CS = DS = SS = 0, so linear = offset): small
// functions are run and what was recorded is checked against what the code does.
#include "cb/emulate/native_win16.hpp"

#include <stdexcept>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

using namespace cb;
using namespace cb::emulate::native_win16;
namespace native_trace = cb::emulate::native_trace;

namespace {

constexpr std::uint64_t kCode = 0x1000;
constexpr std::uint64_t kReturn = 0xF000;      // a near return lands here: the run has returned
constexpr std::uint64_t kSentinel = 0xE000;
constexpr std::uint64_t kData = 0x3000;
constexpr std::uint64_t kStack = 0x8000;

struct Machine {
    uc_engine* engine = nullptr;
    Machine() {
        const auto& uc = emu::unicorn::api();
        emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_16, &engine), "uc_open");
        for (std::uint64_t at : {kCode, kReturn, kSentinel, kData, kStack - 0x1000}) {
            emu::unicorn::check(uc.uc_mem_map(engine, at, 0x1000, UC_PROT_ALL), "uc_mem_map");
        }
        const Bytes nops(0x1000, 0x90);
        uc.uc_mem_write(engine, kReturn, nops.data(), nops.size());
        uc.uc_mem_write(engine, kSentinel, nops.data(), nops.size());
        std::uint16_t sp = static_cast<std::uint16_t>(kStack - 0x10);
        const std::uint16_t ret = static_cast<std::uint16_t>(kReturn);
        uc.uc_mem_write(engine, sp, &ret, sizeof ret);       // [sp] = the near return address
        uc.uc_reg_write(engine, UC_X86_REG_SP, &sp);
    }
    ~Machine() { emu::unicorn::api().uc_close(engine); }
    void code(ByteView b) { emu::unicorn::api().uc_mem_write(engine, kCode, b.data(), b.size()); }
    void start(std::uint64_t until = kReturn + 0x800) {
        emu::unicorn::api().uc_emu_start(engine, kCode, until, 0, 0);
    }
    std::uint64_t reg(int id) {
        std::uint64_t v = 0;
        emu::unicorn::api().uc_reg_read(engine, id, &v);
        return v;
    }
};

Options base(native_trace::Governor& gov) {
    Options o;
    o.eip_id = UC_X86_REG_EIP;
    o.near_entry = true;
    o.cap = 1000;
    o.near_ret = kReturn;
    o.far_ret = 0xFFFFF0;
    o.sent_lo = kSentinel;
    o.sent_hi = kSentinel + 0x1000;
    o.snap_base = 0x100000;
    o.governor = &gov;
    return o;
}

}  // namespace

CB_TEST("emulate.native_win16.records_the_path_the_writes_and_the_return", [] {
    Machine m;
    // mov ax,5 ; mov [3000h],ax ; mov bx,[3000h] ; inc ax ; ret
    m.code(Bytes{0xB8, 0x05, 0x00, 0xA3, 0x00, 0x30, 0x8B, 0x1E, 0x00, 0x30, 0x40, 0xC3});
    native_trace::Governor gov;
    gov.ceil = 100;
    Run run(m.engine, base(gov));
    m.start();
    CB_CHECK(!run.raised());
    const Trace t = run.collect();
    CB_CHECK(t.returned);
    CB_CHECK(!t.truncated);
    CB_CHECK(!t.limit_fault);
    CB_CHECK_EQ(t.insn_count, 5);
    CB_CHECK_EQ(run.count(), 5);
    const std::vector<std::uint64_t> path = {0x1000, 0x1003, 0x1006, 0x100A, 0x100B};
    CB_CHECK(t.path == path);
    CB_CHECK_EQ(t.mem_writes.size(), std::size_t{1});
    CB_CHECK_EQ(t.mem_writes[0].addr, kData);
    CB_CHECK_EQ(t.mem_writes[0].size, std::uint64_t{2});
    CB_CHECK_EQ(t.mem_writes[0].value, std::uint64_t{5});
    CB_CHECK_EQ(t.mem_writes[0].pc, std::uint64_t{0x1003});
    CB_CHECK_EQ(t.mem_writes[0].old, std::uint64_t{0});
    // two reads: the data word, and the return address `ret` pops off the stack
    CB_CHECK_EQ(t.mem_reads.size(), std::size_t{2});
    CB_CHECK_EQ(t.mem_reads[0].addr, kData);
    CB_CHECK(t.mem_reads[0].value == std::optional<std::uint64_t>(5));
    CB_CHECK_EQ(t.mem_reads[1].addr, kStack - 0x10);
    CB_CHECK(t.mem_reads[1].value == std::optional<std::uint64_t>(kReturn));
    CB_CHECK(t.writes_seen == std::optional<std::int64_t>(1));
    CB_CHECK(t.memops.has_value());
    CB_CHECK(t.lazy_maps.empty());
    run.detach();
});

CB_TEST("emulate.native_win16.the_cap_truncates_the_run", [] {
    Machine m;
    m.code(Bytes{0xEB, 0xFE});    // jmp $
    native_trace::Governor gov;
    Options o = base(gov);
    o.cap = 10;
    Run run(m.engine, o);
    m.start();
    const Trace t = run.collect();
    CB_CHECK(t.truncated);
    CB_CHECK(!t.returned);
    CB_CHECK_EQ(t.insn_count, 10);
});

CB_TEST("emulate.native_win16.a_page_nothing_mapped_is_mapped_on_demand_and_filled", [] {
    Machine m;
    // mov bx,[5000h] ; ret
    m.code(Bytes{0x8B, 0x1E, 0x00, 0x50, 0xC3});
    native_trace::Governor gov;
    gov.ceil = 100;
    Options o = base(gov);
    o.fill = true;
    Run run(m.engine, o);
    m.start();
    const Trace t = run.collect();
    CB_CHECK(t.returned);
    CB_CHECK_EQ(t.lazy_maps.size(), std::size_t{1});
    CB_CHECK_EQ(t.lazy_maps[0].addr, std::uint64_t{0x5000});
    CB_CHECK_EQ(t.lazy_maps[0].page, std::uint64_t{0x5000});
    CB_CHECK(t.lazy_maps[0].access == native_trace::Access::Read);
    CB_CHECK_EQ(gov.n, 1);                        // one page counted against the governor
    // BX holds the two pattern bytes at 5000h: ((addr * 0x9E3779B1) >> 13) & 0xFF
    auto pattern = [](std::uint64_t a) { return static_cast<std::uint8_t>(((a * 0x9E3779B1ull) >> 13) & 0xFF); };
    CB_CHECK_EQ(m.reg(UC_X86_REG_BX) & 0xFFFF, static_cast<std::uint64_t>(pattern(0x5000) | (pattern(0x5001) << 8)));
});

CB_TEST("emulate.native_win16.the_governor_ceiling_stops_a_run_that_maps_too_much", [] {
    Machine m;
    m.code(Bytes{0x8B, 0x1E, 0x00, 0x50, 0xC3});
    native_trace::Governor gov;
    gov.n = 5;
    gov.ceil = 5;                                 // no room for another page
    Run run(m.engine, base(gov));
    m.start();
    const Trace t = run.collect();
    CB_CHECK(t.truncated);
    CB_CHECK(!t.returned);
});

CB_TEST("emulate.native_win16.an_access_past_the_end_of_a_segment_is_a_limit_fault", [] {
    Machine m;
    // mov bx,[0FFFFh] ; ret -- a word read at offset FFFF runs one byte past the segment
    m.code(Bytes{0x8B, 0x1E, 0xFF, 0xFF, 0xC3});
    native_trace::Governor gov;
    gov.ceil = 100;
    Options o = base(gov);
    o.snap_base = 0x1000000;                      // (segmented: addresses below it have limits)
    Run run(m.engine, o);
    m.start();
    const Trace t = run.collect();
    CB_CHECK(t.limit_fault);
    CB_CHECK(!t.returned);
});

CB_TEST("emulate.native_win16.sentinels_intercepts_and_interrupts_reach_the_handlers", [] {
    Machine m;
    // int 21h ; jmp 0E000h (a landing in the sentinel range) -- written as push + ret to stay in 16-bit near code
    // mov ax,0E000h ; jmp ax
    m.code(Bytes{0xCD, 0x21, 0xB8, 0x00, 0xE0, 0xFF, 0xE0});
    native_trace::Governor gov;
    Options o = base(gov);
    std::vector<std::pair<std::int64_t, std::int64_t>> interrupts, sentinels, intercepts;
    o.handlers.on_interrupt = [&](std::int64_t what, std::int64_t n) { interrupts.emplace_back(what, n); };
    o.handlers.on_sentinel = [&](std::int64_t what, std::int64_t n) {
        sentinels.emplace_back(what, n);
        std::uint16_t ip = static_cast<std::uint16_t>(kReturn);   // the stand-in returns to the caller
        emu::unicorn::api().uc_reg_write(m.engine, UC_X86_REG_IP, &ip);
    };
    o.intercept = {0x1002};
    o.handlers.on_intercept = [&](std::int64_t what, std::int64_t n) { intercepts.emplace_back(what, n); };
    Run run(m.engine, o);
    m.start();
    CB_CHECK(!run.raised());
    CB_CHECK_EQ(interrupts.size(), std::size_t{1});
    CB_CHECK_EQ(interrupts[0].first, 0x21);
    CB_CHECK_EQ(interrupts[0].second, 1);                // the int was the first instruction counted
    CB_CHECK_EQ(intercepts.size(), std::size_t{1});
    CB_CHECK_EQ(intercepts[0].first, 0x1002);
    CB_CHECK_EQ(sentinels.size(), std::size_t{1});
    CB_CHECK_EQ(sentinels[0].first, static_cast<std::int64_t>(kSentinel));
});

CB_TEST("emulate.native_win16.a_handler_that_throws_stops_the_run_and_the_exception_comes_back", [] {
    Machine m;
    m.code(Bytes{0xCD, 0x21, 0x40, 0x40, 0xC3});     // int 21h ; inc ax ; inc ax ; ret
    native_trace::Governor gov;
    Options o = base(gov);
    o.handlers.on_interrupt = [](std::int64_t, std::int64_t) { throw EmulationError("no such service"); };
    Run run(m.engine, o);
    m.start();
    std::exception_ptr e = run.raised();
    CB_CHECK(static_cast<bool>(e));
    CB_CHECK_THROWS_AS(EmulationError, std::rethrow_exception(e));
    CB_CHECK(!run.raised());                              // (taken: it is not handed back twice)
    const Trace t = run.collect();
    CB_CHECK(!t.returned);
    CB_CHECK(t.insn_count < 3);
});

CB_TEST("emulate.native_win16.a_prelude_is_dropped_by_reset", [] {
    Machine m;
    m.code(Bytes{0x40, 0x40, 0xC3});                      // inc ax ; inc ax ; ret
    native_trace::Governor gov;
    Run run(m.engine, base(gov));
    m.start();
    CB_CHECK_EQ(run.count(), 3);
    run.reset();
    CB_CHECK_EQ(run.count(), 0);
    const Trace t = run.collect();
    CB_CHECK_EQ(t.insn_count, 0);
    CB_CHECK(t.path.empty());
});

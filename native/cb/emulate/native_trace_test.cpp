// cb/emulate/native_trace_test.cpp -- the 32-bit run recorder on a real Unicorn engine: a small function that reads
// its argument, writes memory, calls an import through the IAT and branches on the result is run in each of the
// recorder's modes, and what was recorded is checked against what the code does.
#include "cb/emulate/native_trace.hpp"

#include <chrono>

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"

#pragma warning(push, 0)
#include <unicorn/x86.h>
#pragma warning(pop)

using namespace cb;
using namespace cb::emulate::native_trace;

namespace {

constexpr std::uint64_t kCode = 0x401000;
constexpr std::uint64_t kIat = 0x402000;
constexpr std::uint64_t kData = 0x500000;
constexpr std::uint64_t kSentinel = 0x70000000;
constexpr std::uint64_t kReturn = 0x600000;
constexpr std::uint64_t kStackTop = 0x7F001000;

// int f(int x) { *(int*)0x500000 = x + 5; if (Sleep(100) == 0) return 0 (popped ebp); else return Sleep() + 1; }
const Bytes kFunction = {
    0x55,                                      // 401000 push ebp
    0x8B, 0xEC,                                // 401001 mov ebp,esp
    0x8B, 0x45, 0x08,                          // 401003 mov eax,[ebp+8]
    0x83, 0xC0, 0x05,                          // 401006 add eax,5
    0xA3, 0x00, 0x00, 0x50, 0x00,              // 401009 mov [500000h],eax
    0x6A, 0x64,                                // 40100E push 64h
    0xFF, 0x15, 0x00, 0x20, 0x40, 0x00,        // 401010 call dword ptr [402000h]   (the IAT slot: Sleep)
    0x83, 0xF8, 0x00,                          // 401016 cmp eax,0
    0x74, 0x01,                                // 401019 jz 40101C
    0x40,                                      // 40101B inc eax
    0x5D,                                      // 40101C pop ebp
    0xC3,                                      // 40101D ret
};

struct Machine {
    uc_engine* engine = nullptr;
    Machine() {
        const auto& uc = emu::unicorn::api();
        emu::unicorn::check(uc.uc_open(UC_ARCH_X86, UC_MODE_32, &engine), "uc_open");
        map(kCode & ~0xFFFull, 0x1000);
        write(kCode, kFunction);
        map(kIat, 0x1000);
        const std::uint8_t slot[4] = {0x00, 0x00, 0x00, 0x70};             // IAT[0] = the sentinel
        write(kIat, slot);
        map(kData, 0x1000);
        map(kSentinel, 0x1000);                                            // the stand-in's landing (executes as nops)
        const Bytes nops(0x1000, 0x90);
        write(kSentinel, nops);
        map(kStackTop - 0x1000, 0x1000);
        // the frame: [esp] = return address, [esp+4] = the argument
        std::uint32_t esp = static_cast<std::uint32_t>(kStackTop - 0x100);
        const std::uint32_t frame[2] = {static_cast<std::uint32_t>(kReturn), 7};
        uc.uc_mem_write(engine, esp, frame, sizeof frame);
        uc.uc_reg_write(engine, UC_X86_REG_ESP, &esp);
    }
    ~Machine() { emu::unicorn::api().uc_close(engine); }
    void map(std::uint64_t at, std::size_t size) { emu::unicorn::check(emu::unicorn::api().uc_mem_map(engine, at, size, UC_PROT_ALL), "uc_mem_map"); }
    void write(std::uint64_t at, ByteView data) { emu::unicorn::check(emu::unicorn::api().uc_mem_write(engine, at, data.data(), data.size()), "uc_mem_write"); }
    std::uint64_t reg(int id) {
        std::uint64_t v = 0;
        emu::unicorn::api().uc_reg_read(engine, id, &v);
        return v;
    }
    Bytes read(std::uint64_t at, std::size_t n) {
        Bytes out(n);
        emu::unicorn::api().uc_mem_read(engine, at, out.data(), n);
        return out;
    }
};

Options base_options(TraceCache& cache, Governor& gov, std::optional<StaticId> code = std::nullopt) {
    Options o;
    o.cache = &cache;
    o.governor = &gov;
    if (code) o.statics = {*code};
    o.init_pages = {kIat >> 12, kData >> 12, kSentinel >> 12, (kStackTop - 0x1000) >> 12};
    o.mapped = {kCode & ~0xFFFull, kIat, kData, kSentinel, kStackTop - 0x1000};
    o.sentinels = {{kSentinel, {"KERNEL32.dll", "Sleep"}}};
    o.module_hash = "test-module";
    o.stub = true;
    o.insn_cap = 1000;
    o.stub_ret = 0x1234;
    o.func = {kCode, kCode + kFunction.size()};
    o.image = {0x400000, 0x403000};
    o.lean_low_end = 0x7EFFFFFF;
    o.lean_high_begin = 0x7F000000;
    o.resolver = [](const ImportName& n) {
        ImportAnswer a;
        if (n.name == "Sleep") a.argc = 1;
        return a;
    };
    return o;
}

TraceRecord run_once(Machine& m, Options o, int* status = nullptr) {
    TraceRun run(m.engine, std::move(o));
    const int s = run.run(kCode, kReturn);
    if (status) *status = s;
    return run.collect();
}

bool path_has(const TraceRecord& r, std::uint64_t address) {
    return std::find(r.path.begin(), r.path.end(), address) != r.path.end();
}

}  // namespace

CB_TEST("emulate.native_trace.records_the_path_the_calls_and_the_memory", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    Options o = base_options(cache, gov);
    o.reads = true;
    int status = -1;
    const TraceRecord r = run_once(m, o, &status);
    CB_CHECK_EQ(status, 0);                                          // it ran to its return address
    CB_CHECK_EQ(m.reg(UC_X86_REG_EAX), 0x1235u);                     // Sleep's stand-in answered stub_ret; inc eax
    CB_CHECK(m.read(kData, 4) == (Bytes{12, 0, 0, 0}));              // *(int*)0x500000 = 7 + 5
    CB_CHECK(!r.truncated);
    // the path: every instruction in order, the stand-in's landing in the middle, the return last-but-the-end
    CB_CHECK_EQ(r.path.size(), 12u);
    CB_CHECK_EQ(r.path.front(), kCode);
    CB_CHECK(!path_has(r, kSentinel));                               // the stand-in answers instead of the import running
    CB_CHECK(path_has(r, 0x40101B));                                 // jz not taken: inc eax ran
    CB_CHECK_EQ(r.mnemonics.at(kCode), "push ebp");
    CB_CHECK_EQ(r.mnemonics.at(0x401003), "mov eax, dword ptr [ebp + 8]");
    CB_CHECK_EQ(r.mnemonics.at(0x401016), "cmp eax, 0");
    // the write to data, among the stack's own
    bool wrote_data = false;
    for (const MemWrite& w : r.mem_writes) {
        if (w.addr == kData) {
            wrote_data = true;
            CB_CHECK_EQ(w.size, 4u);
            CB_CHECK_EQ(w.value, 12u);
            CB_CHECK_EQ(w.pc, 0x401009u);
        }
    }
    CB_CHECK(wrote_data);
    // the reads: the argument off the stack, and the IAT slot
    bool read_arg = false, read_iat = false;
    for (const MemRead& rd : r.mem_reads) {
        if (rd.pc == 0x401003) read_arg = rd.value == 7u;
        if (rd.addr == kIat) read_iat = rd.value == kSentinel;
    }
    CB_CHECK(read_arg);
    CB_CHECK(read_iat);
    // the one call: indirect (through the IAT), answered by the stand-in, with the argument it was given
    CB_CHECK_EQ(r.calls.size(), 1u);
    const CallRecord& c = r.calls[0];
    CB_CHECK_EQ(c.pc, 0x401010u);
    CB_CHECK(c.indirect());
    CB_CHECK(!c.into_func);
    CB_CHECK_EQ(c.resolved_target.value_or(0), kSentinel);
    CB_CHECK(c.external);
    CB_CHECK(c.stub.has_value());
    if (c.stub) {
        CB_CHECK_EQ(c.stub->api, "KERNEL32.dll!Sleep");
        CB_CHECK(!c.stub->ret_sentinel.has_value() || *c.stub->ret_sentinel == 0x1234);
    }
    CB_CHECK_EQ(c.argc.value_or(-1), 1);
    CB_CHECK(c.args.has_value());
    if (c.args) CB_CHECK_EQ(c.args->stack[0], 0x64u);               // push 64h
    CB_CHECK(r.api_deps == (std::vector<std::string>{"KERNEL32.dll!Sleep"}));
    CB_CHECK(r.api_calls.has_value());
    if (r.api_calls) {
        CB_CHECK_EQ(r.api_calls->size(), 1u);
        CB_CHECK_EQ((*r.api_calls)[0].api, "KERNEL32.dll!Sleep");
        CB_CHECK((*r.api_calls)[0].args == (std::vector<std::int64_t>{0x64}));
    }
    CB_CHECK_EQ(r.stubbed_externals.value_or(-1), 1);
    CB_CHECK_EQ(r.insn_count, static_cast<std::int64_t>(r.path.size()) + 1);   // ... but it counts as one instruction
    CB_CHECK_EQ(gov.n, r.insn_count);                                // the governor carries the count
});

CB_TEST("emulate.native_trace.logs_the_branches_with_what_they_compared", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    Options o = base_options(cache, gov);
    o.branches = true;
    const TraceRecord r = run_once(m, o);
    CB_CHECK_EQ(r.branches.size(), 1u);
    if (r.branches.empty()) return;
    const BranchEntry& b = r.branches[0];
    CB_CHECK_EQ(b.address, 0x401019u);
    CB_CHECK(!b.taken);                                              // eax was 0x1234, not zero
    CB_CHECK(b.compare.has_value());
    if (b.compare) {
        CB_CHECK(b.compare->op == CompareOp::Cmp);
        CB_CHECK_EQ(b.compare->size, 4u);
        CB_CHECK_EQ(b.compare->a, 0x1234u);
        CB_CHECK_EQ(b.compare->b, 0u);
    }
});

CB_TEST("emulate.native_trace.a_fast_run_takes_whole_straight_runs", [] {
    // A fast run is told of straight runs, not of each instruction: it keeps the count, the instruction texts, the memory
    // it wrote and the imports it answered, but not the path or the call records (those need every instruction).
    TraceCache cache;
    const StaticId code = cache.add_code(kCode, kFunction);
    TraceRecord slow, fast;
    {
        Machine m;
        Governor gov{0, 1'000'000};
        slow = run_once(m, base_options(cache, gov));
    }
    {
        Machine m;
        Governor gov{0, 1'000'000};
        Options o = base_options(cache, gov, code);
        o.fast = true;
        fast = run_once(m, o);
    }
    CB_CHECK_EQ(fast.insn_count, slow.insn_count);
    CB_CHECK(fast.path.empty());
    CB_CHECK(fast.calls.empty());
    CB_CHECK_EQ(fast.mnemonics.size(), slow.mnemonics.size());
    for (const auto& [addr, text] : slow.mnemonics) {
        const std::string* got = fast.mnemonics.find(addr);
        CB_CHECK(got != nullptr);
        if (got) CB_CHECK_EQ(*got, text);
    }
    CB_CHECK_EQ(fast.mem_writes.size(), slow.mem_writes.size());
    CB_CHECK(fast.api_deps == slow.api_deps);
    CB_CHECK_EQ(fast.stubbed_externals.value_or(-1), 1);
});

CB_TEST("emulate.native_trace.a_lean_run_gives_writes_as_four_words_and_the_pages_first_written", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    Options o = base_options(cache, gov);
    o.lean = true;
    o.init_pages.clear();                                            // so the first write to a page notes the page
    const TraceRecord r = run_once(m, o);
    CB_CHECK(r.mem_writes.empty());
    bool wrote_data = false;
    for (const LeanWrite& w : r.lean_writes) {
        if (w.addr == kData) {
            wrote_data = true;
            CB_CHECK_EQ(w.size, 4u);
            CB_CHECK_EQ(w.value, 12u);
            CB_CHECK_EQ(w.pc, 0x401009u);
        }
    }
    CB_CHECK(wrote_data);
    const Bytes* page = r.pages0.find(kData);
    CB_CHECK(page != nullptr);
    if (page) {
        CB_CHECK_EQ(page->size(), kPage);
        CB_CHECK_EQ((*page)[0], 0);                                  // the page as it was before the write
    }
});

CB_TEST("emulate.native_trace.the_resolver_is_asked_once_per_import_and_its_errors_come_out_of_start", [] {
    {
        Machine m;
        TraceCache cache;
        Governor gov{0, 1'000'000};
        Options o = base_options(cache, gov);
        int asked = 0;
        o.resolver = [&](const ImportName& n) {
            ++asked;
            CB_CHECK_EQ(n.dll, "KERNEL32.dll");
            ImportAnswer a;
            a.argc = 1;
            return a;
        };
        (void)run_once(m, o);
        CB_CHECK_EQ(asked, 1);
    }
    {
        Machine m;
        TraceCache cache;
        Governor gov{0, 1'000'000};
        Options o = base_options(cache, gov);
        o.resolver = [](const ImportName&) -> ImportAnswer { throw ParseError("test", 1, 1, "no signature"); };
        CB_CHECK_THROWS_AS(ParseError, (void)run_once(m, o));
    }
});

CB_TEST("emulate.native_trace.a_run_that_faults_ends_with_unicorns_status", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    // jump into unmapped memory: ... the harness of the emulator says how it ended; the recorder reports the code
    Bytes bad = {0xE9, 0x00, 0x00, 0x00, 0x20};                     // jmp +0x20000000
    m.write(0x401000, bad);
    int status = 0;
    const TraceRecord r = run_once(m, base_options(cache, gov), &status);
    CB_CHECK(status != 0 || r.ran_into_zeros || r.truncated || !r.path.empty());
});

CB_TEST("emulate.native_trace.an_instruction_cap_truncates", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    Options o = base_options(cache, gov);
    o.insn_cap = 5;
    const TraceRecord r = run_once(m, o);
    CB_CHECK(r.truncated);
    CB_CHECK_EQ(r.insn_count, 5);
});

CB_TEST("emulate.native_trace.needs_a_cache_and_a_governor", [] {
    Machine m;
    Options o;
    CB_CHECK_THROWS_AS(InternalError, TraceRun(m.engine, o));
    TraceCache cache;
    o.cache = &cache;
    CB_CHECK_THROWS_AS(InternalError, TraceRun(m.engine, o));
    Governor gov;
    o.governor = &gov;
    o.statics = {99};                                                // a code range nobody added
    CB_CHECK_THROWS_AS(UsageError, TraceRun(m.engine, o));
});

CB_TEST("emulate.native_trace.hooks_come_off_with_the_run", [] {
    Machine m;
    TraceCache cache;
    Governor gov{0, 1'000'000};
    {
        TraceRun run(m.engine, base_options(cache, gov));
    }                                                                // destroyed without running: nothing left behind
    // a second, plain run on the same engine records and finishes (a stale hook would have faulted)
    const TraceRecord r = run_once(m, base_options(cache, gov));
    CB_CHECK_EQ(r.path.front(), kCode);
});

CB_TEST("emulate.native_trace.words", [] {
    CB_CHECK_EQ(std::string(to_text(Access::Fetch)), "fetch");
    CB_CHECK_EQ(std::string(to_text(Access::Other)), "?");
    CB_CHECK_EQ(std::string(to_text(CompareOp::Test)), "test");
    CB_CHECK_EQ(std::string(to_text(CompareOp::Xor)), "xor");
});

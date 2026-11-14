// cb/emulate/emulator_test.cpp -- the 32-bit function tracer, on real Unicorn runs of small PE modules built here (hand-assembled x86, laid
// out as a PE32 with a code section and an import directory) and of the real lz32.dll of the Windows 95 corpus.
//
// The hand-made module has one function for each thing a run can do: add two arguments and return (stdcall), call an import and branch on
// an argument, loop for ever, hit an invalid instruction, clobber a register it must keep, read memory the caller supplied, and call into
// another module that is only there as an overlay. Every expectation is worked out from the assembly below, not recorded.
//
// Notes:
//  - overlay imports are merged into the traced module's (the overlay's slots get landing places too), whether or not the traced module has
//    imports of its own.
//  - the section a function's offsets are taken from is the section it is in, so a second section of the same name has its own functions.
//  - the trace is a typed record (cb/emulate/native_trace's TraceRecord inside Trace): mnemonics are keyed by address, not by "0x%x" text.
//  - a text selector that is no number is a UsageError.
#include "cb/emulate/emulator.hpp"

#include <algorithm>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/knowledge/store.hpp"

using namespace cb;
using namespace cb::emulate;
using namespace cb::emulate::emulator;
namespace sfs = std::filesystem;
namespace nt = cb::emulate::native_trace;

namespace {

// ---- a PE32 made of hand-assembled functions ---------------------------------------------------------------------------------------

void emu_put16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}
void emu_put32(Bytes& b, std::size_t at, std::uint32_t v) {
    emu_put16(b, at, v & 0xFFFF);
    emu_put16(b, at + 2, v >> 16);
}
void emu_place(Bytes& text, std::size_t offset, std::initializer_list<std::uint8_t> code) { std::copy(code.begin(), code.end(), text.begin() + static_cast<std::ptrdiff_t>(offset)); }

constexpr std::uint32_t kBase = 0x400000;
constexpr std::uint32_t kOverlayBase = 0x10000000;
constexpr std::uint32_t kText = 0x1000, kIdata = 0x2000;   // the sections' RVAs
constexpr std::uint32_t kSleepSlot = kBase + 0x2050;       // the import address table slot of kernel32.dll!Sleep

// The functions of the module, as offsets in its code section.
constexpr std::uint32_t kAdd = 0x00, kSleepy = 0x20, kLoop = 0x50, kInvalid = 0x70, kClobber = 0x90, kReadMem = 0xB0, kCaller = 0x100;

// The code section of the main module.
Bytes emu_main_text() {
    Bytes t(0x200, 0xCC);
    // int __stdcall add(int a, int b) { return a + b; }
    emu_place(t, kAdd, {0x55, 0x8B, 0xEC, 0x8B, 0x45, 0x08, 0x03, 0x45, 0x0C, 0x5D, 0xC2, 0x08, 0x00});
    // int __stdcall sleepy(int flag) { int r = Sleep(100); *(int*)0x500000 = r; if (flag) r += 3; return r; }
    emu_place(t, kSleepy,
              {0x55, 0x8B, 0xEC, 0x6A, 0x64, 0xFF, 0x15, 0x50, 0x20, 0x40, 0x00,   // push 100 ; call [Sleep slot]
               0xA3, 0x00, 0x00, 0x50, 0x00,                                         // mov [500000h], eax
               0x8B, 0x4D, 0x08, 0x85, 0xC9, 0x74, 0x03,                             // mov ecx,[ebp+8] ; test ecx,ecx ; jz +3
               0x40, 0x40, 0x40,                                                     // inc eax x3
               0x5D, 0xC2, 0x04, 0x00});                                             // pop ebp ; ret 4
    emu_place(t, kLoop, {0x55, 0x8B, 0xEC, 0xEB, 0xFE});                             // for (;;);
    emu_place(t, kInvalid, {0x55, 0x8B, 0xEC, 0x0F, 0x0B});                          // ud2
    emu_place(t, kClobber, {0x55, 0x8B, 0xEC, 0x31, 0xDB, 0x5D, 0xC3});              // xor ebx,ebx (a register it must keep)
    emu_place(t, kReadMem, {0x55, 0x8B, 0xEC, 0xA1, 0x00, 0x00, 0x60, 0x00, 0x5D, 0xC3});   // return *(int*)0x600000
    // int caller(void) { return f_in_the_overlay(); }   (call 10001000h: rel32 = 10001000h - 401108h = 0FBFFEF8h)
    emu_place(t, kCaller, {0x55, 0x8B, 0xEC, 0xE8, 0xF8, 0xFE, 0xBF, 0x0F, 0x5D, 0xC3});
    return t;
}

// A PE32 DLL: a code section at RVA 0x1000 and, when `imports`, an import directory (kernel32.dll!Sleep) at 0x2000. The entry point is
// `entry` (an RVA in the code).
Bytes emu_pe(std::uint32_t image_base, const Bytes& text, std::uint32_t entry, bool imports) {
    Bytes f(0x600, 0);
    f[0] = 'M';
    f[1] = 'Z';
    emu_put32(f, 0x3C, 0x40);   // e_lfanew
    const std::size_t pe = 0x40;
    f[pe] = 'P';
    f[pe + 1] = 'E';
    emu_put16(f, pe + 4, 0x14C);    // i386
    emu_put16(f, pe + 6, 2);        // two sections
    emu_put16(f, pe + 20, 0xE0);    // size of the optional header
    emu_put16(f, pe + 22, 0x2102);  // a DLL, 32-bit, executable
    const std::size_t opt = pe + 24;
    emu_put16(f, opt, 0x10B);
    emu_put32(f, opt + 4, 0x200);   // size of code
    emu_put32(f, opt + 16, entry);
    emu_put32(f, opt + 20, kText);  // base of code
    emu_put32(f, opt + 28, image_base);
    emu_put32(f, opt + 32, 0x1000);   // section alignment
    emu_put32(f, opt + 36, 0x200);    // file alignment
    emu_put16(f, opt + 40, 4);
    emu_put16(f, opt + 48, 4);
    emu_put32(f, opt + 56, 0x3000);   // size of image
    emu_put32(f, opt + 60, 0x200);    // size of headers
    emu_put16(f, opt + 68, 2);        // subsystem
    emu_put32(f, opt + 92, 16);       // data directories
    if (imports) {
        emu_put32(f, opt + 96 + 8, kIdata);    // the import directory ...
        emu_put32(f, opt + 96 + 12, 0x28);     // ... (one descriptor and the null one)
    }
    const std::size_t sections = opt + 0xE0;
    const char text_name[8] = {'.', 't', 'e', 'x', 't', 0, 0, 0}, idata_name[8] = {'.', 'i', 'd', 'a', 't', 'a', 0, 0};
    std::copy(text_name, text_name + 8, f.begin() + static_cast<std::ptrdiff_t>(sections));
    emu_put32(f, sections + 8, 0x200);
    emu_put32(f, sections + 12, kText);
    emu_put32(f, sections + 16, 0x200);
    emu_put32(f, sections + 20, 0x200);
    emu_put32(f, sections + 36, 0x60000020);   // code, executable, readable
    std::copy(idata_name, idata_name + 8, f.begin() + static_cast<std::ptrdiff_t>(sections + 40));
    emu_put32(f, sections + 48, 0x200);
    emu_put32(f, sections + 52, kIdata);
    emu_put32(f, sections + 56, 0x200);
    emu_put32(f, sections + 60, 0x400);
    emu_put32(f, sections + 76, 0xC0000040);   // initialised data, readable, writable
    std::copy(text.begin(), text.end(), f.begin() + 0x200);
    if (imports) {
        const std::size_t id = 0x400;   // file offset of RVA 0x2000
        emu_put32(f, id + 0, kIdata + 0x40);   // OriginalFirstThunk
        emu_put32(f, id + 12, kIdata + 0x60);  // Name
        emu_put32(f, id + 16, kIdata + 0x50);  // FirstThunk: the import address table
        emu_put32(f, id + 0x40, kIdata + 0x70);   // the lookup table: Sleep by name
        emu_put32(f, id + 0x50, kIdata + 0x70);   // the address table, as the file holds it
        const char dll[] = "kernel32.dll";
        std::copy(dll, dll + sizeof dll, f.begin() + static_cast<std::ptrdiff_t>(id + 0x60));
        const char name[] = "Sleep";
        std::copy(name, name + sizeof name, f.begin() + static_cast<std::ptrdiff_t>(id + 0x72));   // (a hint of 0, then the name)
    }
    return f;
}

// The modules of a test, on disk: "main.dll" and "overlay.dll".
struct Modules {
    fs::ScopedTempDir dir{"cb-emulator-test"};
    sfs::path main_path, overlay_path;
    Modules() {
        main_path = dir.path() / "main.dll";
        overlay_path = dir.path() / "overlay.dll";
        fs::write_file(main_path, emu_pe(kBase, emu_main_text(), kText + kAdd, true));
        Bytes overlay_text(0x200, 0xCC);
        emu_place(overlay_text, 0, {0xB8, 0x7B, 0x00, 0x00, 0x00, 0xC3});   // mov eax,123 ; ret
        fs::write_file(overlay_path, emu_pe(kOverlayBase, overlay_text, kText, false));
    }
    Environment environment() const {
        Environment env;
        env.resolve = [this](std::string_view name) -> sfs::path {
            if (name == "main.dll") return main_path;
            if (name == "overlay.dll") return overlay_path;
            throw NotFoundError(sfs::path(std::string(name)), 2);
        };
        return env;
    }
};

ImportArgc emu_argc_of_sleep() {
    return [](const nt::ImportName& n) -> std::optional<std::int64_t> { return n.name == "Sleep" ? std::optional<std::int64_t>(1) : std::nullopt; };
}

// Registers by name, through the engine's own names (for the checks that read them).
std::vector<std::uint64_t> emu_path_of(const Trace& t) { return t.record.path; }

}  // namespace

CB_TEST("emulate.emulator.exit_words_and_machine_constants", [] {
    Trace t;
    CB_CHECK_EQ(t.exit_text(), "ret");
    t.exit = Exit::Truncated;
    CB_CHECK_EQ(t.exit_text(), "truncated");
    t.exit = Exit::Faulted;
    t.fault = "stopped at 0x1234";
    CB_CHECK_EQ(t.exit_text(), "exception: stopped at 0x1234");
    CB_CHECK_EQ(kStackTop, 0x33F000u);
    CB_CHECK_EQ(kStackBase + kStackSize, 0x340000u);
    CB_CHECK_EQ(kSavedSentinels[0], 0xB0B0B0B0u);
});

CB_TEST("emulate.emulator.a_tracer_needs_a_resolver", [] {
    CB_CHECK_THROWS_AS(UsageError, Tracer(Environment{}));
});

CB_TEST("emulate.emulator.load_lays_the_code_out_and_finds_functions_and_imports", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    const auto m = tracer.load("main.dll");
    CB_CHECK_EQ(m->va0, kBase + kText);
    CB_CHECK_EQ(m->code.size(), std::size_t{0x200});
    CB_CHECK_EQ(m->section, ".text");
    CB_CHECK_EQ(m->ranges.size(), std::size_t{1});
    CB_CHECK_EQ(m->ranges[0].lo, kBase + kText);
    CB_CHECK_EQ(m->ranges[0].hi, kBase + kText + 0x200);
    CB_CHECK_EQ(m->code[kAdd], 0x55);   // the bytes as they are in the file
    CB_CHECK_EQ(m->code[kCaller + 3], 0xE8);
    for (const std::uint32_t f : {kAdd, kSleepy, kLoop, kInvalid, kClobber, kReadMem, kCaller}) CB_CHECK(m->functions.contains(f));
    CB_CHECK_EQ(m->functions.at(kAdd), kSleepy);        // a function runs to the next one's start
    CB_CHECK_EQ(m->functions.rbegin()->second, 0x200u);   // the last to the section's end
    CB_CHECK_EQ(m->imports.size(), std::size_t{1});
    CB_CHECK_EQ(m->imports.at(kSleepSlot).dll, "kernel32.dll");
    CB_CHECK_EQ(m->imports.at(kSleepSlot).name, "Sleep");
    CB_CHECK_EQ(m->sha256, crypto::to_hex(crypto::sha256(fs::read_file(mods.main_path))));
    CB_CHECK(tracer.load("main.dll") == m);   // carved once
    CB_CHECK_THROWS_AS(NotFoundError, tracer.load("nowhere.dll"));
    // a file that is not a PE
    fs::write_text(mods.dir.path() / "text.dll", "this is not a program");
    Environment env = mods.environment();
    env.resolve = [&](std::string_view) { return mods.dir.path() / "text.dll"; };
    Tracer other(env);
    CB_CHECK_THROWS_AS(FormatError, other.load("anything"));
});

CB_TEST("emulate.emulator.resolve_selects_a_function", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    const std::uint32_t va0 = kBase + kText;
    auto check = [&](const FuncSelector& sel, std::uint32_t off) {
        const ResolvedFunction rf = tracer.resolve("main.dll", sel);
        CB_CHECK_EQ(rf.off, off);
        CB_CHECK_EQ(rf.va(), va0 + off);
    };
    check(FuncSelector{}, kAdd);                                       // none: the first function
    check(FuncSelector{std::uint64_t{va0 + kSleepy}}, kSleepy);        // a virtual address
    check(FuncSelector{std::uint64_t{kLoop}}, kLoop);                  // an offset
    check(FuncSelector{std::string("0x") + std::format("{:x}", va0 + kInvalid)}, kInvalid);   // hex text
    check(FuncSelector{std::string(std::to_string(kClobber))}, kClobber);                     // decimal text (an offset)
    check(FuncSelector{std::uint64_t{va0 + kSleepy + 0x13}}, kSleepy);   // inside a function: its start
    check(FuncSelector{std::uint64_t{va0 + kAdd + 0x1F}}, kAdd);
    const ResolvedFunction sleepy = tracer.resolve("main.dll", FuncSelector{std::uint64_t{kSleepy}});
    CB_CHECK_EQ(sleepy.end, kLoop);
    CB_CHECK_EQ(sleepy.end_va(), va0 + kLoop);
    CB_CHECK_THROWS_AS(FormatError, tracer.resolve("main.dll", FuncSelector{std::uint64_t{0x500000}}));   // no function there
    CB_CHECK_THROWS_AS(FormatError, tracer.resolve("main.dll", FuncSelector{std::uint64_t{0x1000}}));     // an offset past the code
    CB_CHECK_THROWS_AS(UsageError, tracer.resolve("main.dll", FuncSelector{std::string("sleepy")}));
    CB_CHECK_THROWS_AS(UsageError, tracer.resolve("main.dll", FuncSelector{std::string("")}));
});

CB_TEST("emulate.emulator.a_stdcall_function_returns_its_result", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    TraceOptions o;
    o.args = std::vector<std::uint32_t>{40, 2};
    const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kAdd}}, o);
    CB_CHECK(t.exit == Exit::Returned);
    CB_CHECK_EQ(t.exit_text(), "ret");
    CB_CHECK_EQ(t.module, "main.dll");
    CB_CHECK(t.module_path == mods.main_path);
    CB_CHECK_EQ(t.section, ".text");
    CB_CHECK_EQ(t.func_va, kBase + kText + kAdd);
    CB_CHECK_EQ(t.func_end_va, kBase + kText + kSleepy);
    CB_CHECK_EQ(t.func_off, kAdd);
    CB_CHECK_EQ(t.target_sha256.size(), std::size_t{64});
    CB_CHECK_EQ(t.args.size(), std::size_t{2});
    CB_CHECK_EQ(t.entry_esp, kStackTop - 12);   // two arguments and the return address
    CB_CHECK_EQ(t.ret.value_or(0), 42u);
    CB_CHECK_EQ(t.esp_delta.value_or(0), 12u);   // stdcall: the return address and 8 bytes of arguments
    CB_CHECK(t.saved_regs.has_value());
    CB_CHECK(*t.saved_regs == kSavedSentinels);   // a function that keeps what it must hands the sentinels back
    CB_CHECK_EQ(t.record.insn_count, 6);          // push, mov, mov, add, pop, ret
    CB_CHECK(!t.record.truncated);
    CB_CHECK_EQ(t.record.path.size(), std::size_t{6});
    CB_CHECK_EQ(t.record.path.front(), std::uint64_t{kBase + kText});
    CB_CHECK_EQ(t.record.mnemonics.at(kBase + kText), "push ebp");
    CB_CHECK(t.record.calls.empty());
    CB_CHECK(t.record.mem_writes.size() >= 1);   // the frame's own pushes (push ebp) are writes the hooks saw
    // the one run record
    const Run run = record(std::make_shared<const Trace>(t));
    CB_CHECK_EQ(run.exit.value_or(""), "ret");
    CB_CHECK_EQ(run.ended(), "ret");
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("EAX")), 42u);
    CB_CHECK(std::holds_alternative<double>(run.results.at("ST0")));   // the x87 stack top: whatever the engine holds
    CB_CHECK_EQ(run.stack.value_or(0), 12);
    CB_CHECK(run.kept());
    CB_CHECK_EQ(run.steps.value_or(0), 6u);
    CB_CHECK_EQ(run.path.size(), std::size_t{6});
    CB_CHECK(!run.truncated);
    CB_CHECK(!run.capped);
    CB_CHECK(run.endian == Endian::Little);
    CB_CHECK(std::any_cast<std::shared_ptr<const Trace>>(&run.raw) != nullptr);
    // arity: no arguments given is four zero dwords, and `arity` says how many
    const Trace four = tracer.trace("main.dll", FuncSelector{std::uint64_t{kAdd}});
    CB_CHECK_EQ(four.args.size(), std::size_t{4});
    CB_CHECK_EQ(four.ret.value_or(1), 0u);
    TraceOptions two;
    two.arity = 2;
    CB_CHECK_EQ(tracer.trace("main.dll", FuncSelector{std::uint64_t{kAdd}}, two).args.size(), std::size_t{2});
});

CB_TEST("emulate.emulator.imports_are_stood_in_for_and_their_calls_recorded", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    for (const std::uint32_t flag : {0u, 1u}) {
        TraceOptions o;
        o.args = std::vector<std::uint32_t>{flag};
        o.stub_externals = true;
        o.stub_ret = 7;
        o.argc = emu_argc_of_sleep();
        const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, o);
        CB_CHECK(t.exit == Exit::Returned);
        CB_CHECK_EQ(t.ret.value_or(0), flag != 0 ? 10u : 7u);   // Sleep answered 7; with a flag three more
        CB_CHECK_EQ(t.esp_delta.value_or(0), 8u);               // the stand-in popped Sleep's argument, so the frame was balanced
        CB_CHECK(t.record.api_calls.has_value());
        CB_CHECK_EQ(t.record.api_calls->size(), std::size_t{1});
        CB_CHECK_EQ(t.record.api_calls->at(0).api, "kernel32.dll!Sleep");
        CB_CHECK_EQ(t.record.api_calls->at(0).args.value_or(std::vector<std::int64_t>()).at(0), 100);   // the argument that was pushed
        CB_CHECK_EQ(t.record.api_deps.size(), std::size_t{1});
        CB_CHECK_EQ(t.record.api_deps[0], "kernel32.dll!Sleep");
        CB_CHECK_EQ(t.record.stubbed_externals.value_or(0), 1);
        // the write at 500000h went to memory nobody mapped: a zeroed page was mapped for it and the access recorded
        CB_CHECK_EQ(t.record.lazy_maps.size(), std::size_t{1});
        CB_CHECK_EQ(t.record.lazy_maps[0].addr, 0x500000u);
        const auto w = std::find_if(t.record.mem_writes.begin(), t.record.mem_writes.end(), [](const nt::MemWrite& m) { return m.addr == 0x500000; });
        CB_CHECK(w != t.record.mem_writes.end());
        CB_CHECK_EQ(w->value, 7u);
        CB_CHECK_EQ(w->size, 4u);
        // the run record names the call
        const Run run = record(std::make_shared<const Trace>(t));
        CB_CHECK_EQ(run.calls.size(), std::size_t{1});
        CB_CHECK_EQ(run.calls[0].name.value_or(""), "kernel32.dll!Sleep");
        CB_CHECK(run.kept());
    }
    // without a count for the import the stand-in cannot pop its argument: the frame is off by four, the return goes astray
    TraceOptions blind;
    blind.args = std::vector<std::uint32_t>{0};
    blind.stub_externals = true;
    const Trace lost = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, blind);
    CB_CHECK(lost.esp_delta.value_or(0) != 8u || lost.exit != Exit::Returned);
    // the process default applies to a run that does not say
    tracer.set_default_stub(true, emu_argc_of_sleep());
    TraceOptions plain;
    plain.args = std::vector<std::uint32_t>{0};
    plain.stub_ret = 5;
    const Trace dflt = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, plain);
    CB_CHECK_EQ(dflt.ret.value_or(0), 5u);
    CB_CHECK_EQ(dflt.record.api_calls->size(), std::size_t{1});
    // and an explicit no overrides it: the call goes through the table's own address (the file's name entry, which is not code)
    TraceOptions off;
    off.args = std::vector<std::uint32_t>{0};
    off.stub_externals = false;
    const Trace unstubbed = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, off);
    CB_CHECK(!unstubbed.record.api_calls.has_value());
});

CB_TEST("emulate.emulator.a_run_that_does_not_end_is_truncated_at_the_cap", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    TraceOptions o;
    o.insn_cap = 50;
    const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kLoop}}, o);
    CB_CHECK(t.exit == Exit::Truncated);
    CB_CHECK(t.record.truncated);
    CB_CHECK(t.record.insn_count >= 50);
    CB_CHECK(t.record.insn_count <= 52);
    const Run run = record(std::make_shared<const Trace>(t));
    CB_CHECK_EQ(run.ended(), "truncated");
    CB_CHECK(run.truncated);
    // the governor stops a run the cap would have let go on: a ceiling of 20 instructions for the function
    Tracer governed(mods.environment());
    governed.governor().ceil = 20;
    governed.governor().n = 0;
    const Trace g = governed.trace("main.dll", FuncSelector{std::uint64_t{kLoop}});
    CB_CHECK(g.exit == Exit::Truncated);
    CB_CHECK(g.record.governed);
    CB_CHECK(g.record.insn_count < 100);
    CB_CHECK(governed.governor().n >= 20);   // what the run took is counted against the function
});

CB_TEST("emulate.emulator.a_fault_is_the_traces_exit_not_an_exception", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kInvalid}});
    CB_CHECK(t.exit == Exit::Faulted);
    CB_CHECK(t.fault.find("nstruction") != std::string::npos);   // Unicorn's "Invalid instruction"
    CB_CHECK(t.exit_text().starts_with("exception: "));
    CB_CHECK(!t.saved_regs.has_value());   // not read after a fault
    const Run run = record(std::make_shared<const Trace>(t));
    CB_CHECK_EQ(run.ended(), "fault");
    CB_CHECK(!run.kept());   // the registers were not read
});

CB_TEST("emulate.emulator.a_function_that_loses_a_register_it_must_keep_is_caught", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kClobber}});
    CB_CHECK(t.exit == Exit::Returned);
    CB_CHECK(t.saved_regs.has_value());
    CB_CHECK_EQ((*t.saved_regs)[0], 0u);                          // EBX was zeroed
    CB_CHECK_EQ((*t.saved_regs)[1], kSavedSentinels[1]);          // the others are as they went in
    CB_CHECK_EQ(t.esp_delta.value_or(0), 4u);                     // cdecl: only the return address
    const Run run = record(std::make_shared<const Trace>(t));
    CB_CHECK(!run.kept());
    CB_CHECK(run.preserved.has_value());
    CB_CHECK_EQ(run.preserved->at("EBX").on_entry.value_or(0), std::uint64_t{kSavedSentinels[0]});
    CB_CHECK_EQ(run.preserved->at("EBX").at_end.value_or(1), 0u);
    // with sentinels of the caller's choosing the entry values follow them
    TraceOptions o;
    o.sentinels = std::array<std::uint32_t, 4>{1, 2, 3, 4};
    const Trace u = tracer.trace("main.dll", FuncSelector{std::uint64_t{kClobber}}, o);
    CB_CHECK_EQ((*u.saved_regs)[1], 2u);
    const Run ru = record(std::make_shared<const Trace>(u), o.sentinels);
    CB_CHECK_EQ(ru.preserved->at("ESI").on_entry.value_or(0), 2u);
});

CB_TEST("emulate.emulator.memory_the_caller_supplies_is_there_and_a_kept_machine_is_put_back", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    auto run = [&](std::uint32_t fill, bool reuse) {
        TraceOptions o;
        o.mem_init = {MemoryInit{0x600000, {static_cast<std::uint8_t>(fill), 2, 3, 4}}};
        o.reuse = reuse;
        return tracer.trace("main.dll", FuncSelector{std::uint64_t{kReadMem}}, o);
    };
    const Trace plain = run(1, false);
    CB_CHECK_EQ(plain.ret.value_or(0), 0x04030201u);
    CB_CHECK(plain.record.lazy_maps.empty());   // the caller's page is mapped before the run: nothing is mapped lazily
    CB_CHECK_EQ(plain.record.mem_reads.size() >= 1, true);
    // kept machines: the same result run after run, and each run sees its own memory (the first run's page is gone)
    for (const std::uint32_t fill : {1u, 9u, 1u, 7u}) {
        const Trace t = run(fill, true);
        CB_CHECK_EQ(t.ret.value_or(0), 0x04030200u + fill);
        CB_CHECK(t.exit == Exit::Returned);
    }
    // a run with no memory of its own on a kept machine reads what the machine had, which is nothing: zeros, mapped lazily and then unmapped
    TraceOptions bare;
    bare.reuse = true;
    for (int i = 0; i < 2; ++i) {
        const Trace t = tracer.trace("main.dll", FuncSelector{std::uint64_t{kReadMem}}, bare);
        CB_CHECK_EQ(t.ret.value_or(1), 0u);
        CB_CHECK_EQ(t.record.lazy_maps.size(), std::size_t{1});
    }
    // everything a reused run recorded is what a fresh one records
    TraceOptions a, b;
    a.args = b.args = std::vector<std::uint32_t>{0};
    a.stub_externals = b.stub_externals = true;
    a.argc = b.argc = emu_argc_of_sleep();
    a.stub_ret = b.stub_ret = 3;
    b.reuse = true;
    for (int i = 0; i < 3; ++i) {
        const Trace fresh = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, a);
        const Trace kept = tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, b);
        CB_CHECK_EQ(fresh.ret.value_or(0), kept.ret.value_or(1));
        CB_CHECK(emu_path_of(fresh) == emu_path_of(kept));
        CB_CHECK_EQ(fresh.record.mem_writes.size(), kept.record.mem_writes.size());
        CB_CHECK_EQ(fresh.record.lazy_maps.size(), kept.record.lazy_maps.size());
        CB_CHECK_EQ(fresh.record.insn_count, kept.record.insn_count);
        CB_CHECK(fresh.esp_delta == kept.esp_delta);
    }
});

CB_TEST("emulate.emulator.an_overlay_puts_another_module_beside_the_traced_one", [] {
    Modules mods;
    Tracer tracer(mods.environment());
    // Without it the call to 10001000h lands in memory nobody laid out: zeros, which the run cannot make a result of
    const Trace alone = tracer.trace("main.dll", FuncSelector{std::uint64_t{kCaller}});
    CB_CHECK(alone.ret.value_or(0) != 123u || alone.exit != Exit::Returned);
    TraceOptions o;
    o.overlay = "overlay.dll";
    const Trace beside = tracer.trace("main.dll", FuncSelector{std::uint64_t{kCaller}}, o);
    CB_CHECK(beside.exit == Exit::Returned);
    CB_CHECK_EQ(beside.ret.value_or(0), 123u);
    CB_CHECK_EQ(beside.record.insn_count, 7);   // push, mov, call, then the overlay's mov and ret, then pop, ret
});

CB_TEST("emulate.emulator.out_params_follow_the_headers_declaration", [] {
    symbols::prototypes::Prototype p;
    p.parameters = {{"HKEY", "key"}, {"LPCSTR", "name"}, {"PHKEY", "result"}, {"LPDWORD", "size"}, {"const PUINT", ""}, {"DWORD", ""}, {"", ""}};
    const auto outs = out_params(p);
    CB_CHECK_EQ(outs.size(), std::size_t{3});
    CB_CHECK_EQ(outs[0].index, 2);
    CB_CHECK(!outs[0].value.has_value());   // a handle: the stand-in hands out one
    CB_CHECK_EQ(outs[1].index, 3);
    CB_CHECK_EQ(outs[1].value.value_or(-1), 0);   // a number: zero
    CB_CHECK_EQ(outs[2].index, 4);
    CB_CHECK(out_params(symbols::prototypes::Prototype{}).empty());
});

CB_TEST("emulate.emulator.the_stand_ins_answers_come_from_the_environment", [] {
    // the environment's prototype lookup is asked by the API's own name, once, and an API that gives something back through a pointer
    // gets its out parameters recorded by the recorder; asked only for an import whose count is known and not 0
    Modules mods;
    std::vector<std::string> asked;
    Environment env = mods.environment();
    env.prototype = [&](std::string_view name) -> std::optional<symbols::prototypes::Prototype> {
        asked.emplace_back(name);
        symbols::prototypes::Prototype p;
        p.parameters = {{"DWORD", "ms"}};
        return p;
    };
    Tracer tracer(env);
    TraceOptions o;
    o.args = std::vector<std::uint32_t>{0};
    o.stub_externals = true;
    o.argc = emu_argc_of_sleep();
    for (int i = 0; i < 3; ++i) (void)tracer.trace("main.dll", FuncSelector{std::uint64_t{kSleepy}}, o);
    CB_CHECK_EQ(asked.size(), std::size_t{1});   // (cached after the first)
    CB_CHECK_EQ(asked[0], "Sleep");
});

CB_TEST("emulate.emulator.builtin_argc_reads_the_win32_model", [] {
    fs::ScopedTempDir dir("cb-emulator-kb");
    knowledge::Store store(dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path()));
    knowledge::Knowledge kb(store);
    CB_CHECK_EQ(builtin_argc(kb, "GlobalAlloc").value_or(-1), 2);
    CB_CHECK_EQ(builtin_argc(kb, "SetLastError").value_or(-1), 1);
    CB_CHECK_EQ(builtin_argc(kb, "MessageBoxA").value_or(-1), 4);   // the ANSI variant has its base's count
    CB_CHECK_EQ(builtin_argc(kb, "MessageBoxW").value_or(-1), 4);
    CB_CHECK_EQ(builtin_argc(kb, "GetLastError").value_or(-1), 0);
    CB_CHECK(!builtin_argc(kb, "NoSuchApiA").has_value());
    CB_CHECK(!builtin_argc(kb, "").has_value());
});

CB_TEST("emulate.emulator.the_real_lz32_dll_traces_the_same_fresh_and_kept", [] {
    const sfs::path lz32 = testing::data_path("formats/testdata/lz32.dll");
    if (!sfs::exists(lz32)) return;
    Environment env;
    env.resolve = [&](std::string_view) { return lz32; };
    Tracer tracer(env);
    const auto m = tracer.load("lz32.dll");
    CB_CHECK(!m->functions.empty());
    CB_CHECK(!m->imports.empty());
    CB_CHECK(m->va0 >= 0x10000000u);   // (a DLL's code is not at the bottom of memory)
    std::size_t traced = 0;
    for (const auto& [off, end] : m->functions) {
        if (traced == 12) break;
        TraceOptions a;
        a.stub_externals = true;
        a.argc = [](const nt::ImportName&) { return std::optional<std::int64_t>(2); };
        a.insn_cap = 2000;
        TraceOptions b = a;
        b.reuse = true;
        const Trace fresh = tracer.trace("lz32.dll", FuncSelector{std::uint64_t{off}}, a);
        const Trace kept1 = tracer.trace("lz32.dll", FuncSelector{std::uint64_t{off}}, b);
        const Trace kept2 = tracer.trace("lz32.dll", FuncSelector{std::uint64_t{off}}, b);
        for (const Trace* k : {&kept1, &kept2}) {
            CB_CHECK(fresh.exit == k->exit);
            CB_CHECK(fresh.ret == k->ret);
            CB_CHECK(fresh.esp_delta == k->esp_delta);
            CB_CHECK(emu_path_of(fresh) == emu_path_of(*k));
            CB_CHECK_EQ(fresh.record.mem_writes.size(), k->record.mem_writes.size());
            CB_CHECK_EQ(fresh.record.insn_count, k->record.insn_count);
            CB_CHECK(fresh.record.api_deps == k->record.api_deps);
        }
        CB_CHECK_EQ(emu_path_of(fresh).front(), std::uint64_t{m->va0 + off});   // it began at the function's first instruction
        ++traced;
    }
    CB_CHECK_EQ(traced, std::size_t{12});
});

// ---- the recorder's agreed records ---------------------------------------------
//
// A small module of three hand-assembled functions is traced in each of eight configurations of the recorder; the golden holds, for each case,
// a digest of the agreed record (it cannot be recomputed here). What the golden pins is also which cases gave the SAME record -- a run that stubs its imports and one that does not, when
// the run meets none; a run and the same run under a cap it does not reach -- and that is what is checked here: the same module (the code bytes below,
// but for the import slot's address), the same eight configurations and arguments, and every set of cases the golden gives one
// digest must give one record here, a record being its end, result, stack, instructions that ran, the imports entered and the branches taken. Beside
// them the record of f is worked out from its code.
//
//   f(a, p):   if (*p != a) { *p = a; Sleep(a); }  read an unmapped page  ...  g()  ...  ret 8       g: a counted loop       h: call 0; int 20h
namespace {

constexpr std::uint32_t kTraceBase = 0x10000000, kTraceF = 0x00, kTraceG = 0x30, kTraceH = 0x40;
constexpr std::uint32_t kTraceP = kScratchBase + 0x100;

Bytes emu_trace_text() {
    Bytes t(0x100, 0x90);
    emu_place(t, kTraceF,
              {0x55, 0x8B, 0xEC, 0x53,                           // push ebp ; mov ebp,esp ; push ebx
               0x8B, 0x45, 0x08, 0x8B, 0x4D, 0x0C,               // mov eax,[ebp+8] ; mov ecx,[ebp+0Ch]
               0x8B, 0x19, 0x3B, 0xD8, 0x74, 0x09,               // mov ebx,[ecx] ; cmp ebx,eax ; je +9
               0x89, 0x01, 0x50, 0xFF, 0x15, 0x50, 0x20, 0x00, 0x10,   // mov [ecx],eax ; push eax ; call [Sleep slot at 10002050h]
               0x8B, 0x15, 0x00, 0x00, 0x40, 0x00,               // mov edx,[400000h]: an unmapped page
               0x49, 0x75, 0x02, 0x33, 0xC0,                     // dec ecx ; jne +2 ; xor eax,eax
               0xE8, 0x07, 0x00, 0x00, 0x00,                     // call g
               0x5B, 0x5D, 0xC2, 0x08, 0x00});                   // pop ebx ; pop ebp ; ret 8
    emu_place(t, kTraceG, {0xB9, 0x03, 0x00, 0x00, 0x00, 0x01, 0xC8, 0xE2, 0xFC, 0xC3});   // mov ecx,3 ; add eax,ecx ; loop -4 ; ret
    emu_place(t, kTraceH, {0x33, 0xC0, 0xFF, 0xD0, 0xCD, 0x20, 0x01, 0x00, 0x02, 0x00, 0xC3});   // xor eax,eax ; call eax ; int 20h ... ; ret
    return t;
}

struct TraceConfig {
    const char* name;
    std::function<void(TraceOptions&)> set;
};

// What a record is, for comparing two of them.
std::string emu_trace_seen(const Trace& t) {
    std::string out = std::format("{} ret={:x} esp={} saved={} insns={} |", t.exit_text(), t.ret.value_or(0), t.esp_delta.value_or(0), t.saved_regs ? 1 : 0, t.record.insn_count);
    for (const std::uint64_t a : t.record.path) out += std::format(" {:x}", a);
    out += " | calls";
    if (t.record.api_calls) {
        for (const nt::ApiCall& c : *t.record.api_calls) {
            out += " " + c.api + "(";
            if (c.args) for (const std::int64_t a : *c.args) out += std::format("{:x},", a);
            out += ")";
        }
    }
    out += " | branches";
    for (const nt::BranchEntry& b : t.record.branches) out += std::format(" {:x}{}", b.address, b.taken ? "T" : "F");
    return out;
}

}  // namespace

CB_TEST("emulate.emulator.cases_the_recorder_agreed_gave_one_record_give_one_record", [] {
    fs::ScopedTempDir dir("cb-emulator-trace");
    const sfs::path module = dir.path() / "native-trace-test.dll";
    fs::write_file(module, emu_pe(kTraceBase, emu_trace_text(), kText, true));
    Environment env;
    env.resolve = [&](std::string_view) { return module; };
    Tracer tracer(env);
    const ImportArgc sleep_argc = [](const nt::ImportName& n) -> std::optional<std::int64_t> { return n.name == "Sleep" ? std::optional<std::int64_t>(1) : std::nullopt; };
    const auto judge = [&](TraceOptions& o) {
        o.stub_externals = true;
        o.reads = false;
        o.stop_wild = true;
        o.quiet_stack = true;
        o.reuse = true;
        o.fast = true;
        o.lean = true;
        o.pads = nt::AddressRange{0xC0DE0000, 0xC0DE1000};
        o.argc = sleep_argc;
    };
    const std::vector<TraceConfig> configs = {
        {"plain", [](TraceOptions& o) { o.stub_externals = false; }},
        {"stub", [&](TraceOptions& o) { o.stub_externals = true; o.argc = sleep_argc; }},
        {"stub-unique-reads", [&](TraceOptions& o) { o.stub_externals = true; o.stub_ret_unique = true; o.reads = true; o.argc = sleep_argc; }},
        {"judge", judge},
        {"judge-branches", [&](TraceOptions& o) { judge(o); o.branches = true; }},
        {"judge-capped", [&](TraceOptions& o) { judge(o); o.insn_cap = 12; }},
        {"one-at-a-time", [&](TraceOptions& o) { judge(o); o.fast = false; o.lean = false; o.branches = true; }},
        {"no-resolver", [](TraceOptions& o) { o.stub_externals = true; o.reuse = true; o.fast = true; }},
    };
    // the golden: key -> digest, for the cases of this module (f with two argument lists, g, h)
    std::map<std::string, std::string> digest_of;
    for (const std::string_view line : text::split(fs::read_text(testing::data_path("emulate/testdata/native_trace_golden.txt")), '\n')) {
        const std::size_t cut = line.rfind(' ');
        if (cut == std::string_view::npos) continue;
        const std::string key(line.substr(0, cut));
        if (key.starts_with("0x30 ") || key.starts_with("0x40 ") || (key.starts_with("f ") && key.find("shadow") == std::string::npos && key.find("raises") == std::string::npos))
            digest_of[key] = std::string(line.substr(cut + 1));
    }
    CB_CHECK_EQ(digest_of.size(), std::size_t{32});   // 8 configurations x (f twice, g, h)

    std::map<std::string, std::string> seen;   // key -> the record here
    for (const TraceConfig& config : configs) {
        for (const std::uint32_t a : {5u, 7u}) {
            TraceOptions o;
            config.set(o);
            o.args = std::vector<std::uint32_t>{a, kTraceP};
            o.mem_init.push_back(MemoryInit{kTraceP, Bytes{7, 0, 0, 0}});
            seen[std::format("f {} [{}, {}]", config.name, a, kTraceP)] = emu_trace_seen(tracer.trace("native-trace-test.dll", FuncSelector{std::uint64_t{kTraceF}}, o));
        }
        for (const std::uint32_t off : {kTraceG, kTraceH}) {
            TraceOptions o;
            config.set(o);
            o.args = std::vector<std::uint32_t>{0, 0};
            seen[std::format("0x{:x} {}", off, config.name)] = emu_trace_seen(tracer.trace("native-trace-test.dll", FuncSelector{std::uint64_t{off}}, o));
        }
    }
    CB_CHECK_EQ(seen.size(), digest_of.size());
    std::size_t groups = 0;
    for (const auto& [key, digest] : digest_of) {
        for (const auto& [other, other_digest] : digest_of) {
            if (key >= other || digest != other_digest) continue;
            ++groups;
            CB_CHECK(seen.contains(key) && seen.contains(other));
            if (seen.at(key) != seen.at(other)) testing::record_failure(__FILE__, __LINE__, "the golden gives '" + key + "' and '" + other + "' one record, but here they differ:\n  " + seen.at(key) + "\n  " + seen.at(other));
        }
    }
    CB_CHECK(groups >= 8);   // the golden's sets: plain/stub/stub-unique-reads for f(7) g and h; judge/judge-capped for g and h

    // the record of f(5, p) with its imports stubbed, worked out from the code: *p (7) != 5, so it stores 5 and enters Sleep(5); it reads the unmapped page
    // 400000h (mapped on demand); ecx = P != 1 so the jne goes; g runs (3 passes); ret 8 -- stdcall with two arguments
    TraceOptions o;
    o.stub_externals = true;
    o.argc = sleep_argc;
    o.args = std::vector<std::uint32_t>{5, kTraceP};
    o.mem_init.push_back(MemoryInit{kTraceP, Bytes{7, 0, 0, 0}});
    const Trace t = tracer.trace("native-trace-test.dll", FuncSelector{std::uint64_t{kTraceF}}, o);
    CB_CHECK_EQ(t.exit_text(), "ret");
    CB_CHECK_EQ(t.esp_delta.value_or(0), 12u);
    CB_CHECK(t.record.api_calls.has_value() && t.record.api_calls->size() == 1);
    if (t.record.api_calls && !t.record.api_calls->empty()) {
        CB_CHECK_EQ((*t.record.api_calls)[0].api, "kernel32.dll!Sleep");   // (the import directory's spelling of the DLL's name)
        CB_CHECK(((*t.record.api_calls)[0].args == std::optional<std::vector<std::int64_t>>(std::vector<std::int64_t>{5})));
    }
    CB_CHECK_EQ(t.record.path.front(), std::uint64_t{kTraceBase + kText + kTraceF});
    const std::string with5 = seen.at(std::format("f stub [5, {}]", kTraceP)), with7 = seen.at(std::format("f stub [7, {}]", kTraceP));
    CB_CHECK(with5 != with7);   // the other argument: the je goes, Sleep is not entered
});

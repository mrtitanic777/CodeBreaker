// Tests of verify/lanes/win64: the inputs, an x64 image as the lane runs it, one run's fingerprint in Unicorn's x64, and verdicts on a
// rebuilt function placed beside the original. The original is formats/xboxone_test_image's PE32+ (its entry calls Sleep(100) through
// the import table and returns the 7 it keeps; `add` returns RCX + RDX); the "rebuilt" code is a few bytes put at the lane's base.
#include "verify/lanes/win64.hpp"

#include <algorithm>
#include <format>
#include <set>
#include <span>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/xboxone_test_image.hpp"

namespace cb::verify::lanes {
namespace {

using Lane = Win64Lane;
namespace xt = formats::xboxone_test;
constexpr std::uint64_t kRebuilt = 0x150000000;

// The test image written out and read as the lane reads it.
Lane::Image test_image() {
    const fs::ScopedTempDir dir("cbw64");
    const std::filesystem::path file = dir.path() / "test.exe";
    fs::write_file(file, xt::pe(false));
    return Lane::read_image(file);
}

// A context with the test image, and `code` as the rebuilt side at kRebuilt.
Lane::Context context_with(Bytes code) {
    Lane::Context c;
    c.original = test_image();
    Lane::Image rebuilt;
    rebuilt.base = kRebuilt;
    rebuilt.end = kRebuilt + 0x1000;
    rebuilt.regions.push_back({kRebuilt, std::move(code), false});
    c.rebuilt = std::move(rebuilt);
    c.argc["Sleep"] = 1;
    return c;
}

Lane::Vector input(std::vector<std::int64_t> args) {
    Lane::Vector v;
    v.args = std::move(args);
    v.args.resize(Lane::kArguments, 0);
    return v;
}

// A context whose original is `code` at 0x140001000, writable data at 0x140002000 (zeros), and one import slot (Sleep) at 0x140003000.
constexpr std::uint64_t kCode = 0x140001000, kData = 0x140002000, kSlot = 0x140003000;
Lane::Context made_of(Bytes code) {
    Lane::Context c;
    c.original.base = 0x140000000;
    c.original.end = 0x140004000;
    c.original.regions.push_back({kCode, std::move(code), false});
    c.original.regions.push_back({kData, Bytes(0x100, 0), true});
    c.original.regions.push_back({kSlot, Bytes(8, 0), false});
    c.original.slots[kSlot] = "Sleep";
    return c;
}
std::pair<std::uint64_t, std::uint64_t> span_of(const Lane::Context& c) { return {kCode, kCode + c.original.regions[0].bytes.size()}; }
// Whether some input takes the branch at `site` the way `taken` says.
bool takes(const Lane::Context& c, std::span<const Lane::Vector> inputs, std::uint64_t site, bool taken) {
    for (const Lane::Vector& v : inputs) {
        const auto t = Lane::trace(c, kCode, span_of(c), v);
        if (!t) continue;
        for (const auto& b : t->observation.branches)
            if (b.address == site && b.taken == taken) return true;
    }
    return false;
}

}  // namespace

CB_TEST("verify.lanes.win64.the_fixed_inputs_are_six_arguments_each", [] {
    const std::vector<Lane::Vector> v = Lane::vectors();
    CB_CHECK(v.size() >= 7);
    for (const Lane::Vector& x : v) CB_CHECK_EQ(x.args.size(), Lane::kArguments);
    // some point into the scratch memory, some of those at a memory of pointers
    CB_CHECK(std::ranges::any_of(v, [](const Lane::Vector& x) { return x.args[0] == static_cast<std::int64_t>(Lane::kScratchBase); }));
    CB_CHECK(std::ranges::any_of(v, [](const Lane::Vector& x) { return x.fill == Lane::Fill::Pointers; }));
    CB_CHECK(std::ranges::any_of(v, [](const Lane::Vector& x) { return x.fill == Lane::Fill::Zeros; }));
});

CB_TEST("verify.lanes.win64.directed_inputs_aim_at_the_constants_the_code_compares_with", [] {
    // cmp ecx, 0x2a ; jne +2 ; sub edx, 5 ; cmp r8d, 1 ; ret       (1 and 0 tell nothing: left out)
    const Bytes code = {0x83, 0xF9, 0x2A, 0x75, 0x02, 0x83, 0xEA, 0x05, 0x41, 0x83, 0xF8, 0x01, 0xC3};
    const std::vector<Lane::Vector> d = Lane::directed(code, 0x140001000);
    CB_CHECK_EQ(d.size(), 4u);   // two for each constant
    CB_CHECK_EQ(d[0].args[3], 0x2A);
    CB_CHECK_EQ(*d[0].stub_ret, 0x2A);
    CB_CHECK_EQ(d[1].args[0], static_cast<std::int64_t>(Lane::kScratchBase));
    CB_CHECK_EQ(d[1].args[1], 0x2A);
    CB_CHECK_EQ(d[2].args[0], 5);
});

CB_TEST("verify.lanes.win64.directed_inputs_take_the_constants_a_test_compares_with_but_not_the_frames", [] {
    // sub rsp, 0x28 ; test cl, 0x40 ; ret
    const Bytes code = {0x48, 0x83, 0xEC, 0x28, 0xF6, 0xC1, 0x40, 0xC3};
    const std::vector<Lane::Vector> d = Lane::directed(code, 0x140001000);
    CB_CHECK_EQ(d.size(), 2u);
    CB_CHECK_EQ(d[0].args[0], 0x40);
});

CB_TEST("verify.lanes.win64.operands_are_read_as_capstone_writes_them", [] {
    const auto r = Lane::operand("r8d");
    CB_CHECK(r && r->kind == Lane::Operand::Kind::Reg && r->reg == "R8D" && r->size == 4);
    CB_CHECK_EQ(Lane::operand("-0x10")->imm, ~std::uint64_t{0xF});
    const auto m = Lane::operand("dword ptr [rcx + rdx*4 - 0x10]");
    CB_CHECK(m && m->kind == Lane::Operand::Kind::Mem && m->size == 4);
    CB_CHECK_EQ(m->base, "RCX");
    CB_CHECK_EQ(m->index, "RDX");
    CB_CHECK_EQ(m->scale, 4u);
    CB_CHECK_EQ(m->imm, ~std::uint64_t{0xF});
    CB_CHECK_EQ(Lane::operand("byte ptr [rip + 0x1f3e]")->base, "RIP");
    CB_CHECK(!Lane::operand("qword ptr gs:[0x30]"));   // (a segment's address: not read)
    CB_CHECK(!Lane::operand("xmm0"));
    CB_CHECK_EQ(Lane::operands("qword ptr [rax], 0").size(), 2u);
});

CB_TEST("verify.lanes.win64.conditions_are_read_off_the_flags", [] {
    constexpr std::uint64_t cf = 1, zf = 0x40, sf = 0x80, of = 0x800;
    CB_CHECK(*Lane::condition("e", zf, 0) && !*Lane::condition("ne", zf, 0));
    CB_CHECK(*Lane::condition("b", cf, 0) && !*Lane::condition("a", cf, 0) && *Lane::condition("be", zf, 0));
    CB_CHECK(*Lane::condition("l", sf, 0) && !*Lane::condition("l", sf | of, 0) && *Lane::condition("g", 0, 0));
    CB_CHECK(*Lane::condition("rcxz", 0, 0) && !*Lane::condition("rcxz", 0, 5));
    CB_CHECK(!Lane::condition("mp", 0, 0));
});

CB_TEST("verify.lanes.win64.an_input_is_the_same_after_a_trip_through_the_guided_search", [] {
    Lane::Vector v = input({1, 2, 3});
    v.stub_ret = 7;
    v.fill = Lane::Fill::Zeros;
    v.mem.push_back({Lane::kScratchBase + 8, Bytes{1, 2}});
    const Lane::Vector w = Lane::from_guided(Lane::to_guided(v));
    CB_CHECK(w.args == v.args && w.stub_ret == v.stub_ret && w.fill == v.fill && w.mem == v.mem);
    CB_CHECK(!Lane::from_guided(Lane::to_guided(input({}))).stub_ret);   // (no result set stays none)
});

CB_TEST("verify.lanes.win64.a_trace_has_the_branches_and_what_they_compared", [] {
    // cmp ecx, 0x2a ; jne +6 ; mov eax, 1 ; ret ; xor eax, eax ; ret
    const Lane::Context c = made_of({0x83, 0xF9, 0x2A, 0x75, 0x06, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3, 0x31, 0xC0, 0xC3});
    const auto t = Lane::trace(c, kCode, span_of(c), input({5}));
    CB_CHECK(t && !t->faulted);
    CB_CHECK_EQ(t->observation.branches.size(), 1u);
    const auto& b = t->observation.branches[0];
    CB_CHECK_EQ(b.address, kCode + 3);
    CB_CHECK(b.taken);
    CB_CHECK(b.compare && b.compare->a == 5 && b.compare->b == 0x2A && b.compare->size == 4u);
});

CB_TEST("verify.lanes.win64.guided_inputs_turn_a_comparison_the_fixed_inputs_never_meet", [] {
    // cmp ecx, 0x1234567 ; jne +6 ; mov eax, 1 ; ret ; xor eax, eax ; ret
    const Lane::Context c = made_of({0x81, 0xF9, 0x67, 0x45, 0x23, 0x01, 0x75, 0x06, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3, 0x31, 0xC0, 0xC3});
    const std::vector<Lane::Vector> seeds = Lane::vectors();
    CB_CHECK(!takes(c, seeds, kCode + 6, false));
    const std::vector<Lane::Vector> got = Lane::guided_inputs(c, span_of(c), seeds);
    CB_CHECK(takes(c, got, kCode + 6, false));
});

CB_TEST("verify.lanes.win64.guided_inputs_set_a_field_read_through_a_pointer_argument", [] {
    // mov rax, [rcx + 0x10] ; cmp dword ptr [rax + 8], 0x77 ; jne +3 ; xor eax, eax ; ret ; mov eax, 1 ; ret
    const Lane::Context c = made_of({0x48, 0x8B, 0x41, 0x10, 0x83, 0x78, 0x08, 0x77, 0x75, 0x03, 0x31, 0xC0, 0xC3, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3});
    // only a pointer in the argument: the counting bytes there are no address, so the run faults on the second read
    const std::vector<Lane::Vector> seeds = {input({static_cast<std::int64_t>(Lane::kScratchBase)})};
    const auto first = Lane::trace(c, kCode, span_of(c), seeds[0]);
    CB_CHECK(first && first->faulted && first->fix);
    CB_CHECK(first->fix->kind == guided::PlaceKind::Addr);
    CB_CHECK_EQ(first->fix->index, Lane::kScratchBase + 0x10);   // the field the address was loaded from
    const std::vector<Lane::Vector> got = Lane::guided_inputs(c, span_of(c), seeds);
    CB_CHECK(takes(c, got, kCode + 8, true));
    CB_CHECK(takes(c, got, kCode + 8, false));                   // the field set to what it is compared with
});

CB_TEST("verify.lanes.win64.a_repair_puts_a_pointer_where_a_null_one_is_followed", [] {
    // mov rax, [rcx] ; mov eax, [rax] ; ret     -- run on scratch memory of zeros
    const Lane::Context c = made_of({0x48, 0x8B, 0x01, 0x8B, 0x00, 0xC3});
    Lane::Vector v = input({static_cast<std::int64_t>(Lane::kScratchBase)});
    v.fill = Lane::Fill::Zeros;
    const auto t = Lane::trace(c, kCode, span_of(c), v);
    CB_CHECK(t && t->faulted && t->fix && t->fix->index == Lane::kScratchBase);
    const Lane::Vector w = Lane::repaired(v, *t->fix);
    CB_CHECK_EQ(w.mem.size(), 1u);
    const auto again = Lane::trace(c, kCode, span_of(c), w);
    CB_CHECK(again && !again->faulted);
    // a code pointer leads to a `ret`
    const Lane::Vector code = Lane::repaired(v, {guided::PlaceKind::Arg, 1, 0}, true);
    CB_CHECK(static_cast<std::uint64_t>(code.args[1]) == Lane::kScratchBase + Lane::kScratchSize - 0x10);
    CB_CHECK(code.mem.back().data == Bytes{0xC3});
});

CB_TEST("verify.lanes.win64.guided_inputs_reach_every_case_of_a_switch", [] {
    // 0x00 cmp ecx, 3 ; 0x03 ja 0x3a ; 0x05 lea rax, [rip + 0xe] (-> 0x1a, the table) ; 0x0c jmp qword ptr [rax + rcx*8]
    // 0x1a the table: four addresses, case k at 0x40 + 6k ; 0x3a default: mov eax, 9 ; ret ; 0x40 each case: mov eax, k ; ret
    Bytes code = {0x83, 0xF9, 0x03, 0x77, 0x35, 0x48, 0x8D, 0x05, 0x0E, 0x00, 0x00, 0x00, 0xFF, 0x24, 0xC8};
    code.resize(0x1A, 0xCC);
    for (std::uint64_t k = 0; k < 4; ++k) {
        const std::uint64_t to = kCode + 0x40 + 6 * k;
        for (int b = 0; b < 8; ++b) code.push_back(static_cast<std::uint8_t>(to >> (8 * b)));
    }
    for (const std::uint8_t k : {std::uint8_t{9}, std::uint8_t{0}, std::uint8_t{1}, std::uint8_t{2}, std::uint8_t{3}}) {
        const Bytes one = {0xB8, k, 0x00, 0x00, 0x00, 0xC3};
        code.insert(code.end(), one.begin(), one.end());
    }
    const Lane::Context c = made_of(code);
    const std::vector<Lane::Vector> seeds = {input({0}), input({7})};
    const std::vector<Lane::Vector> got = Lane::guided_inputs(c, span_of(c), seeds);
    std::set<std::uint64_t> reached;
    for (const Lane::Vector& v : got) {
        const auto t = Lane::trace(c, kCode, span_of(c), v);
        if (t) reached.insert(t->observation.reach.begin(), t->observation.reach.end());
    }
    for (std::uint64_t k = 1; k < 4; ++k) CB_CHECK(reached.contains(kCode + 0x40 + 6 * k));
});

CB_TEST("verify.lanes.win64.an_input_sets_memory_in_the_scratch_and_the_globals_only", [] {
    // mov eax, [rcx] ; add eax, [rdx] ; ret
    const Lane::Context c = made_of({0x8B, 0x01, 0x03, 0x02, 0xC3});
    Lane::Vector v = input({static_cast<std::int64_t>(Lane::kScratchBase + 0x40), static_cast<std::int64_t>(kData)});
    v.mem.push_back({Lane::kScratchBase + 0x40, Bytes{5, 0, 0, 0}});
    v.mem.push_back({kData, Bytes{6, 0, 0, 0}});
    v.mem.push_back({0x50000000, Bytes{1}});   // (nowhere the machine has: left out)
    const compare::Fingerprint fp = Lane::fingerprint(c, kCode, v, false, false);
    CB_CHECK(!fp.err);
    CB_CHECK_EQ(*fp.ret, 11);
    // nor over a section that is not writable: the rebuilt DLL keeps its own copy of what is there (here: a `ret` over the code)
    v.mem.push_back({kCode, Bytes{0xC3}});
    CB_CHECK_EQ(*Lane::fingerprint(c, kCode, v, false, false).ret, 11);
});

CB_TEST("verify.lanes.win64.a_rebuilt_function_handed_to_an_import_is_the_original_function", [] {
    // the original: lea rcx, [rip + 0x7] (its own address + 0xe) ; jmp qword ptr [rip + 0x1fe9] (Sleep's slot) ; the rebuilt: the
    // same, with its own address
    const Bytes code = {0x48, 0x8D, 0x0D, 0x07, 0x00, 0x00, 0x00, 0xFF, 0x25, 0xF3, 0x1F, 0x00, 0x00, 0xCC, 0xC3};
    Lane::Context c = made_of(code);
    c.argc["Sleep"] = 1;
    constexpr std::uint64_t kAt = 0x150000000;
    Lane::Image rebuilt;
    rebuilt.base = kAt;
    rebuilt.end = kAt + 0x1000;
    rebuilt.regions.push_back({kAt, {0x48, 0x8D, 0x0D, 0x07, 0x00, 0x00, 0x00, 0xFF, 0x25, 0xF3, 0x2F, 0x00, 0xF0, 0xCC, 0xC3}, false});
    c.rebuilt = std::move(rebuilt);
    c.originals[kAt + 0xE] = kCode + 0xE;   // (the callback, rebuilt and original)
    const compare::Fingerprint a = Lane::fingerprint(c, kCode, input({}), false, true);
    const compare::Fingerprint b = Lane::fingerprint(c, kAt, input({}), true, true);
    CB_CHECK_EQ(a.deps.size(), 1u);
    CB_CHECK(a.deps == b.deps);
    CB_CHECK_EQ(a.deps[0], std::format("Sleep({:x})", kCode + 0xE));
});

CB_TEST("verify.lanes.win64.an_image_is_its_sections_and_import_slots", [] {
    const Lane::Image image = test_image();
    CB_CHECK_EQ(image.base, xt::kImageBase);
    CB_CHECK_EQ(image.end, xt::kImageBase + 0x4000);
    CB_CHECK_EQ(image.slots.size(), 1u);
    CB_CHECK_EQ(image.slots.at(xt::kImageBase + xt::kIat), "Sleep");
    CB_CHECK_EQ(image.regions.size(), 3u);   // the headers, .text, .rdata
});

CB_TEST("verify.lanes.win64.a_run_records_the_import_call_the_result_and_the_stack", [] {
    const Lane::Context c = context_with({0xC3});
    const compare::Fingerprint fp = Lane::fingerprint(c, xt::kImageBase + xt::kEntry, input({}), false, false,
                                                      std::pair{xt::kImageBase + xt::kEntry, xt::kImageBase + xt::kEntry + 0x1A});
    CB_CHECK(!fp.err);
    CB_CHECK_EQ(fp.exit, std::string(compare::kExitReturned));
    CB_CHECK_EQ(*fp.ret, 7);
    CB_CHECK_EQ(fp.deps.size(), 1u);
    CB_CHECK_EQ(fp.deps[0], "Sleep(64)");          // (as many arguments as the SDK declares: one)
    CB_CHECK_EQ(*fp.pop, 8);                       // the return address taken
    CB_CHECK(*fp.saved);
    CB_CHECK_EQ(fp.cov.size(), 6u);                // every instruction of the entry ran
    // what the import returns is the input's
    Lane::Vector v = input({});
    v.stub_ret = 5;
    CB_CHECK_EQ(*Lane::fingerprint(c, xt::kImageBase + xt::kEntry, v, false, false).ret, 7);
    CB_CHECK_EQ(*Lane::fingerprint(c, xt::kImageBase + xt::kAdd, input({30, 12}), false, false).ret, 42);
});

CB_TEST("verify.lanes.win64.a_fault_is_one_ending_and_a_runaway_is_truncated", [] {
    // mov rax, [0] ; ret          and          jmp $
    const Lane::Context c = context_with({0x48, 0x8B, 0x04, 0x25, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xEB, 0xFE});
    const compare::Fingerprint fault = Lane::fingerprint(c, kRebuilt, input({}), true, false);
    CB_CHECK_EQ(fault.exit, "exception: fault");
    CB_CHECK(!fault.ret);
    const compare::Fingerprint spin = Lane::fingerprint(c, kRebuilt + 9, input({}), true, false);
    CB_CHECK(spin.truncated);
});

CB_TEST("verify.lanes.win64.a_rebuilt_function_that_does_the_same_is_equivalent_and_one_that_does_not_is_caught", [] {
    Lane::Item item;
    item.key = "140001020";
    item.va = xt::kImageBase + xt::kAdd;
    item.end = item.va + 4;
    item.rebuilt_va = kRebuilt;
    // lea eax, [rdx + rcx] ; ret            -- the same sum, written the other way round
    CB_CHECK(Lane::judge_function(context_with({0x8D, 0x04, 0x0A, 0xC3}), item).verdict == Verdict::Equivalent);
    // lea eax, [rcx + rdx + 1] ; ret        -- one more
    const Judgement off = Lane::judge_function(context_with({0x8D, 0x44, 0x11, 0x01, 0xC3}), item);
    CB_CHECK(off.verdict == Verdict::Divergent);
    CB_CHECK_EQ(off.reason, "ret-differs");
    // mov ebx, ecx ; lea eax, [rbx + rdx] ; ret     -- the right sum, RBX not handed back
    const Judgement saved = Lane::judge_function(context_with({0x8B, 0xD9, 0x8D, 0x04, 0x13, 0xC3}), item);
    CB_CHECK(saved.verdict == Verdict::Divergent);
    CB_CHECK_EQ(saved.reason, "registers-differ");
});

CB_TEST("verify.lanes.win64.the_tools_are_found_under_a_visual_studio_folder", [] {
    const fs::ScopedTempDir dir("cbw64");
    CB_CHECK(!Lane::find_tools({dir.path()}).has_value());
    for (const char* version : {"14.29.30133", "14.51.36231"}) {
        const std::filesystem::path bin = dir.path() / "18" / "Community" / "VC" / "Tools" / "MSVC" / version / "bin" / "Hostx64" / "x64";
        fs::make_dirs(bin);
        fs::write_text(bin / "cl.exe", "");
        fs::write_text(bin / "link.exe", "");
    }
    const auto tools = Lane::find_tools({dir.path()});
    CB_CHECK(tools.has_value());
    CB_CHECK(fs::to_text(tools->cl).find("14.51.36231") != std::string::npos);   // the newest
    CB_CHECK_EQ(tools->link.filename().string(), "link.exe");
});

}  // namespace cb::verify::lanes

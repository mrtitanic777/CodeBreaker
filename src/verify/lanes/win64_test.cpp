// Tests of verify/lanes/win64: the inputs, an x64 image as the lane runs it, one run's fingerprint in Unicorn's x64, and verdicts on a
// rebuilt function placed beside the original. The original is formats/xboxone_test_image's PE32+ (its entry calls Sleep(100) through
// the import table and returns the 7 it keeps; `add` returns RCX + RDX); the "rebuilt" code is a few bytes put at the lane's base.
#include "verify/lanes/win64.hpp"

#include <algorithm>

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

}  // namespace

CB_TEST("verify.lanes.win64.the_fixed_inputs_are_six_arguments_each", [] {
    const std::vector<Lane::Vector> v = Lane::vectors();
    CB_CHECK(v.size() >= 7);
    for (const Lane::Vector& x : v) CB_CHECK_EQ(x.args.size(), Lane::kArguments);
    // some point into the scratch memory, some of those at a memory of pointers
    CB_CHECK(std::ranges::any_of(v, [](const Lane::Vector& x) { return x.args[0] == static_cast<std::int64_t>(Lane::kScratchBase); }));
    CB_CHECK(std::ranges::any_of(v, [](const Lane::Vector& x) { return x.pointers; }));
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

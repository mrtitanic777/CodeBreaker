#include "cb/source/ne_asm/segdiff.hpp"

#include <memory>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {
// (std::string::contains is C++23.)
bool has(std::string_view haystack, std::string_view needle) { return haystack.find(needle) != std::string_view::npos; }
}  // namespace


using namespace cb;
using namespace cb::source::ne_asm::segdiff;
namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

namespace {

stdfs::path lzexpand() { return testing::data_path("formats/testdata/lzexpand.dll"); }

Bytes bytes_of(std::initializer_list<int> values) {
    Bytes out;
    for (const int v : values) out.push_back(static_cast<std::uint8_t>(v));
    return out;
}

// A 16-bit object holding `code` in _TEXT with the given publics, and a near fix-up (two bytes) at each of `fixups`.
Bytes object(const Bytes& code, const OrderedMap<std::string, std::int64_t>& publics, const std::vector<int>& fixups) {
    Bytes obj = omf::raw_object("test.c", code, publics);
    if (fixups.empty()) return obj;
    // insert one FIXUPP before the MODEND (the last record): offset-type, segment-relative, to segment 1
    std::size_t last = 0;
    for (std::size_t at = 0; at + 3 <= obj.size(); at += 3 + (obj[at + 1] | (obj[at + 2] << 8))) last = at;
    Bytes payload;
    for (const int at : fixups) {
        payload.push_back(static_cast<std::uint8_t>(0xC4 | (at >> 8)));   // fix-up, segment-relative, location 1 (offset)
        payload.push_back(static_cast<std::uint8_t>(at & 0xFF));
        payload.push_back(0x00);                // frame method 0 and target method 0 (a segment), displacement given
        payload.push_back(1);                   // the frame: segment 1
        payload.push_back(1);                   // the target: segment 1
        payload.push_back(0);                   // displacement, low
        payload.push_back(0);                   // displacement, high
    }
    Bytes out(obj.begin(), obj.begin() + static_cast<std::ptrdiff_t>(last));
    const Bytes fixupp = omf::record(0x9C, payload);
    out.insert(out.end(), fixupp.begin(), fixupp.end());
    out.insert(out.end(), obj.begin() + static_cast<std::ptrdiff_t>(last), obj.end());
    return out;
}

// Microsoft C 8 stood in for by a table of what the selftest's sources compile to: the group has helper first and caller
// after it (the calls inside the group are concrete); with helper external, caller's two calls carry fix-ups; the
// mutated group differs in one byte.
Bytes fake_compile(std::string_view source, std::string_view) {
    OrderedMap<std::string, std::int64_t> publics;
    if (has(source, "int helper(int x)")) {
        Bytes code = bytes_of({0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x04, 0x40, 0x5D, 0xC3, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
                               0x55, 0x8B, 0xEC, 0xE8, 0x10, 0x00, 0xD1, 0xE0, 0xC3});
        if (has(source, "* 3 +")) code[23] = 0xD2;   // a different multiplier
        publics.insert_or_assign("_helper", 0);
        publics.insert_or_assign("_caller", 16);
        return object(code, publics, {});
    }
    if (has(source, "extern int helper")) {
        const Bytes code = bytes_of({0x55, 0x8B, 0xEC, 0xE8, 0x00, 0x00, 0xE8, 0x00, 0x00, 0xD1, 0xE0, 0xC3});
        publics.insert_or_assign("_caller", 0);
        return object(code, publics, {4, 7});
    }
    throw ToolError("fake-cl", 2, "cannot compile that");
}

}  // namespace

CB_TEST("source.ne_asm.segdiff.relocation_field_widths", [] {
    CB_CHECK_EQ(relocation_width(0), 1u);    // low byte
    CB_CHECK_EQ(relocation_width(2), 2u);    // selector
    CB_CHECK_EQ(relocation_width(3), 4u);    // far pointer
    CB_CHECK_EQ(relocation_width(5), 2u);    // offset
    CB_CHECK_EQ(relocation_width(11), 6u);   // 48-bit pointer
    CB_CHECK_EQ(relocation_width(1), 2u);    // anything else is taken as two
    CB_CHECK_EQ(relocation_width(15), 2u);
});

// The sizes, mask counts, first mask offsets and the digest of the segment's bytes are what the reference
// (codebreaker/source/ne_asm/segdiff.py shipped_ne_segment) gives for lzexpand.dll.
CB_TEST("source.ne_asm.segdiff.the_shipped_side_masks_every_relocated_field", [] {
    const formats::ne::Module module = formats::ne::Module::open_file(lzexpand());
    const Shipped s1 = shipped_segment(module, 1);
    CB_CHECK_EQ(s1.code.size(), 456u);
    CB_CHECK_EQ(s1.mask.size(), 52u);
    CB_CHECK_EQ(crypto::to_hex(crypto::sha256(s1.code)).substr(0, 16), "cb1a8b1d99260565");
    const std::vector<std::uint64_t> first1(s1.mask.begin(), std::next(s1.mask.begin(), 12));
    CB_CHECK((first1) == ((std::vector<std::uint64_t>{37, 38, 39, 40, 88, 89, 90, 91, 117, 118, 119, 120})));

    const Shipped s2 = shipped_segment(module, 2);
    CB_CHECK_EQ(s2.code.size(), 1882u);
    CB_CHECK_EQ(s2.mask.size(), 148u);
    CB_CHECK_EQ(crypto::to_hex(crypto::sha256(s2.code)).substr(0, 16), "a36107df2b0d1e7e");
    const std::vector<std::uint64_t> first2(s2.mask.begin(), std::next(s2.mask.begin(), 12));
    CB_CHECK((first2) == ((std::vector<std::uint64_t>{25, 26, 27, 28, 74, 75, 76, 77, 174, 175, 176, 177})));

    const Shipped s3 = shipped_segment(module, 3);   // a segment with nothing to relocate
    CB_CHECK_EQ(s3.code.size(), 24u);
    CB_CHECK(s3.mask.empty());

    CB_CHECK_THROWS_AS(UsageError, (void)shipped_segment(module, 0));
    CB_CHECK_THROWS_AS(UsageError, (void)shipped_segment(module, 99));
});

CB_TEST("source.ne_asm.segdiff.the_differ_ignores_masked_offsets_on_both_sides", [] {
    const Bytes ours = bytes_of({1, 2, 3, 4, 5, 6});
    // equal
    Verdict v = diff(ours, {}, ours, {});
    CB_CHECK(v.exact && v.length_match);
    CB_CHECK_EQ(v.n_diff, 0u);
    CB_CHECK(!v.first_diff.has_value());
    // a difference where ours is masked, one where theirs is: neither counts
    const Bytes other = bytes_of({1, 9, 3, 4, 9, 6});
    v = diff(ours, {1}, other, {4});
    CB_CHECK(v.exact);
    CB_CHECK_EQ(v.masked, 2u);
    // a difference no mask covers is found, counted and located
    v = diff(ours, {1}, other, {});
    CB_CHECK(!v.exact);
    CB_CHECK(v.length_match);
    CB_CHECK_EQ(v.n_diff, 1u);
    CB_CHECK_EQ(*v.first_diff, 4u);
    // another length is never exact, even with equal bytes, and the comparison is over the shorter
    v = diff(ours, {}, bytes_of({1, 2, 3}), {});
    CB_CHECK(!v.exact);
    CB_CHECK(!v.length_match);
    CB_CHECK_EQ(v.n_diff, 0u);
    CB_CHECK_EQ(v.our_len, 6u);
    CB_CHECK_EQ(v.shipped_len, 3u);
    // masks past the end count as masked but compare nothing
    v = diff(ours, {100, 200}, ours, {});
    CB_CHECK(v.exact);
    CB_CHECK_EQ(v.masked, 2u);
    // nothing against nothing
    CB_CHECK(diff({}, {}, {}, {}).exact);
});

CB_TEST("source.ne_asm.segdiff.the_verdict_as_text", [] {
    const Bytes ours = bytes_of({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    Bytes other = ours;
    other[6] = 0xEE;
    const Verdict v = diff(ours, {}, other, {});
    CB_CHECK_EQ(render(v), "verdict: length ok, content differs\n  our=14 shipped=14 bytes, masked=0, diffs=1, first@0x6");
    CB_CHECK_EQ(render(v, ByteView(ours), ByteView(other), {5}, {}),
                "verdict: length ok, content differs\n  our=14 shipped=14 bytes, masked=0, diffs=1, first@0x6\n"
                "  @0x2 ours    : 03 04 05 -- 07 08 09 0a 0b 0c 0d 0e\n"   // (the window is 4 before the difference to 8 after)
                "  @0x2 shipped : 03 04 05 -- ee 08 09 0a 0b 0c 0d 0e");
    CB_CHECK_EQ(render(diff(ours, {}, ours, {})), "verdict: BYTE-EXACT\n  our=14 shipped=14 bytes, masked=0, diffs=0");
    CB_CHECK(render(diff(ours, {}, bytes_of({1, 2}), {})).starts_with("verdict: length differs\n  our=14 shipped=2 bytes"));
});

CB_TEST("source.ne_asm.segdiff.a_group_is_compiled_and_cut_into_functions", [] {
    std::string seen_flags;
    const Compiler16 compile = [&](std::string_view source, std::string_view flags) {
        seen_flags = std::string(flags);
        return fake_compile(source, flags);
    };
    const auto group = compile_group(compile, kSelftestGroup);
    CB_CHECK_EQ(seen_flags, "/AM /Ox /c");
    CB_CHECK_EQ(group.size(), 2u);
    CB_CHECK_EQ(group.find("_helper")->code.size(), 16u);   // cut at the next public
    CB_CHECK_EQ(group.find("_caller")->code.size(), 9u);
    compile_group(compile, kSelftestGroup, "/AL /Od /c");
    CB_CHECK_EQ(seen_flags, "/AL /Od /c");
    const auto alone = compile_group(compile, "extern int helper(int);\nint caller(int y){return helper(y)*2+helper(y+1);}\n");
    CB_CHECK((alone.find("_caller")->fixup_mask) == ((std::set<std::uint64_t>{4, 5, 7, 8})));
    // a compiler that makes no object is an error that carries its words
    CB_CHECK_THROWS_AS(ToolError, (void)compile_group(compile, "int broken("));
});

CB_TEST("source.ne_asm.segdiff.the_selftest_proves_the_mechanism", [] {
    testing::CapturedStd captured;
    CB_CHECK(selftest(fake_compile));
    const std::string out = captured.out();
    CB_CHECK(out.starts_with("segdiff selftest\n\n"));
    CB_CHECK(has(out, "(a) round-trip caller vs itself: verdict: BYTE-EXACT\n"));
    CB_CHECK(has(out, "(b) mutated caller vs original: verdict: length ok, content differs\n"));
    CB_CHECK(has(out, "(c) caller masked bytes: grouped=0, callee-external=4  -> grouping resolves the calls\n"));
    CB_CHECK(out.ends_with("\nSELFTEST: PASS\n"));
});

CB_TEST("source.ne_asm.segdiff.the_selftest_fails_when_grouping_resolves_nothing", [] {
    // a compiler that gives the same code with the same fix-ups however the group is written: (c) cannot hold
    const Compiler16 stubborn = [](std::string_view source, std::string_view) {
        OrderedMap<std::string, std::int64_t> publics;
        publics.insert_or_assign("_caller", 0);
        const Bytes code = bytes_of({0x55, 0x8B, 0xEC, static_cast<int>(has(source, "* 3 +") ? 0xD2 : 0xD1), 0xC3});
        return object(code, publics, {1});
    };
    testing::CapturedStd captured;
    CB_CHECK(!selftest(stubborn));
    CB_CHECK(has(captured.out(), "grouped=2, callee-external=2  -> ??"));
    CB_CHECK(captured.out().ends_with("\nSELFTEST: FAIL\n"));
    // and one that has no _caller at all
    const Compiler16 empty = [](std::string_view, std::string_view) { return omf::raw_object("x.c", bytes_of({0xC3}), {{"_other", 0}}); };
    testing::CapturedStd second;
    CB_CHECK(!selftest(empty));
    CB_CHECK(has(second.out(), "the group has no _caller"));
});

namespace {

// A corpus (the module is named by path, so it holds nothing).
struct World {
    fs::ScopedTempDir dir{"cbsegdiff"};
    fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<fs::ScopedEnv> root_env;
    std::unique_ptr<base::Corpus> corpus;
    World() {
        fs::make_dirs(dir.path() / "corpus");
        root_env.emplace("CB_CORPUS_ROOT", fs::to_text(dir.path() / "corpus"));
        base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus";
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        corpus = std::make_unique<base::Corpus>(products);
    }
};

}  // namespace

CB_TEST("source.ne_asm.segdiff.the_command_compares_a_function_with_the_shipped_bytes", [] {
    World world;
    const formats::ne::Module module = formats::ne::Module::open_file(lzexpand());
    const Shipped seg = shipped_segment(module, 1);
    // a "compiled" function equal to the shipped bytes at 0x20..0x30 where its fix-ups are zero: the relocated field there
    // (shipped offsets 37..40, that is 5..8 in the function) is masked on the shipped side
    const std::size_t at = 0x20, size = 16;
    Bytes code(seg.code.begin() + at, seg.code.begin() + at + size);
    Bytes zeroed = code;
    for (std::size_t k = 5; k <= 8; ++k) zeroed[k] = 0;
    stdfs::path csrc = world.dir.path() / "recon.c";
    fs::write_text(csrc, "int Foo(void) { return 0; }\n");

    const auto compile = [&](const Bytes& body) {
        return [body](std::string_view, std::string_view) {
            OrderedMap<std::string, std::int64_t> publics;
            publics.insert_or_assign("_Foo", 0);
            return omf::raw_object("recon.c", body, publics);
        };
    };
    const std::vector<std::string> args = {fs::to_text(csrc), "--module", fs::to_text(lzexpand()), "--seg", "1", "--off", "0x20", "--func", "_Foo"};
    {
        testing::CapturedStd captured;
        CB_CHECK_EQ(main({*world.corpus, compile(zeroed)}, args), kExitOk);
        CB_CHECK_EQ(captured.out(), "verdict: BYTE-EXACT\n  our=16 shipped=16 bytes, masked=4, diffs=0\n");
    }
    {   // one byte wrong, outside the relocated field
        Bytes wrong = zeroed;
        wrong[12] ^= 0xFF;
        testing::CapturedStd captured;
        CB_CHECK_EQ(main({*world.corpus, compile(wrong)}, args), kExitFailed);
        const std::string out = captured.out();
        CB_CHECK(out.starts_with("verdict: length ok, content differs\n  our=16 shipped=16 bytes, masked=4, diffs=1, first@0xc\n"));
        CB_CHECK(has(out, "@0x8 ours    : -- "));   // the window starts in the masked field
    }
    {   // --size narrows the comparison to the first bytes: still equal, but the lengths differ
        std::vector<std::string> narrower = args;
        narrower.insert(narrower.end(), {"--size", "8"});
        testing::CapturedStd captured;
        CB_CHECK_EQ(main({*world.corpus, compile(zeroed)}, narrower), kExitFailed);
        CB_CHECK(captured.out().starts_with("verdict: length differs\n  our=16 shipped=8 bytes"));
    }
    {   // a function the group does not have
        std::vector<std::string> other = args;
        other.back() = "_Bar";
        testing::CapturedStd captured;
        CB_CHECK_EQ(main({*world.corpus, compile(zeroed)}, other), kExitFailed);
        CB_CHECK(has(captured.err(), "function _Bar not in the compiled group; have: _Foo"));
    }
    // misuse: nothing but a source; no such segment
    CB_CHECK_THROWS_AS(UsageError, (void)main({*world.corpus, compile(zeroed)}, std::vector<std::string>{fs::to_text(csrc)}));
    std::vector<std::string> far = args;
    far[4] = "99";
    CB_CHECK_THROWS_AS(UsageError, (void)main({*world.corpus, compile(zeroed)}, far));
    // --selftest needs nothing else
    testing::CapturedStd captured;
    CB_CHECK((main({*world.corpus, fake_compile}, std::vector<std::string>{"--selftest"})) == (kExitOk));
});

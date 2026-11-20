// cb-unity: alone (test helpers named for this module)
// Tests of cb/symbols/debug_build_estimate on the real 16-bit module LZEXPAND.DLL with the canned listings of its segments 1 and 2 that
// source/ne_asm's tests carry (scenario_a.rows, scenario_b.rows: objdump's lines, so objdump is not needed). The "debug build" is the same
// module with listings changed in the ways a debug build differs from the retail one: another constant (High), another instruction (the
// instruction names are alike, not the same: Aligned, Probable, Guess), a function with no counterpart. Expected values were worked out by hand
// from the listings and the tier rules.
//
// The .MAP is read by debug_build::parse_map (by lines); the tier is an enum and the estimates are
// keyed by (segment, offset); the modules are opened by the caller; the diff has no closing zero-size block (nothing reads it).
#include "symbols/debug_build_estimate.hpp"

#include <format>
#include <sstream>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace est = cb::symbols::debug_build_estimate;
namespace db = cb::symbols::debug_build;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;
namespace ne = cb::formats::ne;

namespace {

std::filesystem::path estimate_testdata(std::string_view name) { return cb::testing::data_path("source/ne_asm/testdata/" + std::string(name)); }

ne::Module lz_module() { return ne::Module::open(cb::fs::read_file(cb::testing::data_path("formats/testdata/lzexpand.dll"))); }

sg::SharedOrdinals shared_ordinals() { return std::make_shared<const sg::Ordinals>(sg::load_ordinals(estimate_testdata("ORDINALS.md"))); }

using Rows = std::vector<std::pair<std::uint32_t, std::string>>;

Rows rows_of(std::string_view name) {
    Rows out;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(estimate_testdata(name)), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        out.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return out;
}

void set_row(Rows& rows, std::uint32_t offset, std::string text) {
    for (auto& [at, existing] : rows)
        if (at == offset) existing = std::move(text);
}

std::string objdump_text(const Rows& rows) {
    std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
    for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
    return out;
}

// objdump as the listings give it: segment 1 (456 bytes) and segment 2 (1882) have the rows given, every other segment none.
fx::Objdump objdump_of(Rows first, Rows second) {
    return [first = std::move(first), second = std::move(second)](cb::ByteView code, std::uint32_t) {
        if (code.size() == 456) return objdump_text(first);
        if (code.size() == 1882) return objdump_text(second);
        return std::string();
    };
}

sg::Seg segment_with(const ne::Module& m, std::size_t index, const Rows& first, const Rows& second) {
    return sg::Seg(m, index, shared_ordinals(), {}, objdump_of(first, second));
}

// What each function of segment 1 is, for these listings: 0x0 (32 instructions), 0x90 (6), 0xa0 (4), 0xc0 (3); none is an export.
const est::SegmentNames kDebugNames = {{0x0, "DbgInit"}, {0x90, "DbgNinety"}, {0xA0, "DbgAye"}, {0xC0, "DbgSea"}};

}  // namespace

CB_TEST("symbols.debug_build_estimate.tiers_have_their_names", [] {
    CB_CHECK_EQ(est::tier_name(est::Tier::Exact), "EXACT");
    CB_CHECK_EQ(est::tier_name(est::Tier::High), "HIGH");
    CB_CHECK_EQ(est::tier_name(est::Tier::Aligned), "ALIGNED");
    CB_CHECK_EQ(est::tier_name(est::Tier::Probable), "PROBABLE");
    CB_CHECK_EQ(est::tier_name(est::Tier::Guess), "GUESS");
    for (const est::Tier t : {est::Tier::Exact, est::Tier::High, est::Tier::Aligned}) CB_CHECK(est::is_confident(t));
    for (const est::Tier t : {est::Tier::Probable, est::Tier::Guess}) CB_CHECK(!est::is_confident(t));
});

CB_TEST("symbols.debug_build_estimate.the_same_code_is_exact_in_every_gap", [] {
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    const sg::Seg retail = segment_with(m, 1, a, b), debug = segment_with(m, 1, a, b);
    const auto got = est::align_segment(1, retail, debug, kDebugNames, {});
    CB_CHECK_EQ(got.size(), 4U);
    CB_CHECK_EQ(got.at(0x0).name, "DbgInit");
    CB_CHECK_EQ(got.at(0x90).name, "DbgNinety");
    CB_CHECK_EQ(got.at(0xA0).name, "DbgAye");
    CB_CHECK_EQ(got.at(0xC0).name, "DbgSea");
    for (const auto& [offset, e] : got) {
        CB_CHECK(e.tier == est::Tier::Exact);
        CB_CHECK_EQ(e.score, 1.0);
    }
});

CB_TEST("symbols.debug_build_estimate.another_constant_is_high_not_exact", [] {
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    Rows other = a;
    set_row(other, 0x96, "mov    bx,WORD PTR [bp+0xc]");   // sub_0090 loads another argument: the same skeleton, not the same code
    const sg::Seg retail = segment_with(m, 1, a, b), debug = segment_with(m, 1, other, b);
    const auto got = est::align_segment(1, retail, debug, kDebugNames, {});
    CB_CHECK_EQ(got.size(), 4U);
    CB_CHECK(got.at(0x90).tier == est::Tier::High);
    CB_CHECK_EQ(got.at(0x90).name, "DbgNinety");
    CB_CHECK_EQ(got.at(0x90).score, 1.0);
    CB_CHECK(got.at(0x0).tier == est::Tier::Exact);
    CB_CHECK(got.at(0xA0).tier == est::Tier::Exact);
    CB_CHECK(got.at(0xC0).tier == est::Tier::Exact);
});

CB_TEST("symbols.debug_build_estimate.what_is_left_in_equal_numbers_is_paired_by_position", [] {
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    Rows other = a;
    set_row(other, 0xA4, "xor    bx,bx");   // sub_00a0: enter, mov, call, retf  against  enter, xor, call, retf: three of four names alike
    const sg::Seg retail = segment_with(m, 1, a, b), debug = segment_with(m, 1, other, b);
    const auto got = est::align_segment(1, retail, debug, kDebugNames, {});
    CB_CHECK_EQ(got.size(), 4U);
    CB_CHECK(got.at(0xA0).tier == est::Tier::Aligned);
    CB_CHECK_EQ(got.at(0xA0).name, "DbgAye");
    CB_CHECK_EQ(got.at(0xA0).score, 0.75);   // 2 * 3 / (4 + 4)
    CB_CHECK_EQ(est::tier_name(got.at(0x90).tier), "EXACT");
    CB_CHECK_EQ(got.at(0x90).name, "DbgNinety");
});

CB_TEST("symbols.debug_build_estimate.unequal_numbers_are_matched_by_likeness", [] {
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    const sg::Seg retail = segment_with(m, 1, a, b);
    {
        // the debug build has no function at 0xc0 and sub_00a0 differs: against the one left in the debug build there are two in the retail
        // one, so the likeliest pair is made, if it is alike enough
        Rows other = a;
        set_row(other, 0xA4, "xor    bx,bx");
        est::SegmentNames names = kDebugNames;
        names.erase(0xC0);
        const auto got = est::align_segment(1, retail, segment_with(m, 1, other, b), names, {});
        CB_CHECK_EQ(got.size(), 3U);   // the function nothing is left for stays unnamed
        CB_CHECK(!got.contains(0xC0));
        CB_CHECK(got.at(0xA0).tier == est::Tier::Guess);   // 0.75: between 0.55 and 0.80
        CB_CHECK_EQ(got.at(0xA0).score, 0.75);
    }
    {
        // one name of six differs (pop bp -> nop) and the debug build has nothing at 0xa0: 2 * 5 / 12 = 0.83, a probable match
        Rows close = a;
        set_row(close, 0x99, "nop");
        est::SegmentNames names = kDebugNames;
        names.erase(0xA0);
        const auto got = est::align_segment(1, retail, segment_with(m, 1, close, b), names, {});
        CB_CHECK_EQ(got.size(), 3U);
        CB_CHECK(!got.contains(0xA0));
        CB_CHECK(got.at(0x90).tier == est::Tier::Probable);
        CB_CHECK_EQ(got.at(0x90).name, "DbgNinety");
        CB_CHECK_EQ(got.at(0x90).score, 0.83);
        CB_CHECK(got.at(0x0).tier == est::Tier::Exact);
    }
});

CB_TEST("symbols.debug_build_estimate.anchors_fix_the_gaps_and_are_never_overridden", [] {
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    const sg::Seg retail = segment_with(m, 1, a, b), debug = segment_with(m, 1, a, b);
    // the exact matches say 0x90 is DbgNinety: only the functions on each side of it are estimated against what is on the same side
    const db::Names exact = {{{1, 0x90}, "DbgNinety"}};
    const auto got = est::align_segment(1, retail, debug, kDebugNames, exact);
    CB_CHECK_EQ(got.at(0x0).name, "DbgInit");
    CB_CHECK_EQ(got.at(0xA0).name, "DbgAye");
    CB_CHECK_EQ(got.at(0xC0).name, "DbgSea");
    // an anchor whose name the debug build does not have anchors nothing
    const auto unknown = est::align_segment(1, retail, debug, kDebugNames, {{{1, 0x90}, "NotInTheMap"}});
    CB_CHECK_EQ(unknown.size(), 4U);
    // with no names in the debug build there is nothing to say
    CB_CHECK(est::align_segment(1, retail, debug, {}, {}).empty());
});

CB_TEST("symbols.debug_build_estimate.the_exports_score_the_tiers", [] {
    // segment 2 of LZEXPAND exports LZINIT at its start; asked to predict the functions that have an export name, the estimate names it from
    // the debug build's map: the same name is a correct prediction
    const ne::Module m = lz_module();
    const Rows a = rows_of("scenario_a.rows"), b = rows_of("scenario_b.rows");
    const sg::Seg retail = segment_with(m, 2, a, b), debug = segment_with(m, 2, a, b);
    CB_CHECK_EQ(retail.name_at(0), "LZINIT");
    const auto got = est::align_segment(2, retail, debug, {{0x0, "lzinit"}, {0x40, "OtherThing"}}, {}, /*predict_exports=*/true);
    CB_CHECK_EQ(got.size(), 1U);
    CB_CHECK_EQ(got.at(0x0).name, "lzinit");
    // and the same call without predict_exports leaves the export alone (it is not anonymous)
    const auto internal = est::align_segment(2, retail, debug, {{0x0, "lzinit"}, {0x40, "OtherThing"}}, {});
    CB_CHECK(!internal.contains(0x0));
});

CB_TEST("symbols.debug_build_estimate.a_module_is_estimated_from_a_debug_build_of_itself", [] {
    const ne::Module m = lz_module();
    const fx::Objdump dump = objdump_of(rows_of("scenario_a.rows"), rows_of("scenario_b.rows"));
    const std::string map = " 0001:0000  DbgInit\r\n 0001:0090  DbgNinety\r\n 0001:00A0  DbgAye\r\n 0001:00C0  DbgSea\r\n 0002:0000  LZINIT\r\n";
    const est::Report r = est::estimate_module(m, m, map, shared_ordinals(), dump);
    // sub_0090 (six instructions) is matched exactly by the debug build's signature, so it is an anchor; the others, too short to trust to the
    // signature match, are estimated by their place between anchors
    CB_CHECK(r.tiers.contains({1, 0x90}));
    CB_CHECK(r.tiers.at({1, 0x90}).tier == est::Tier::Exact);
    CB_CHECK_EQ(r.tiers.at({1, 0x90}).name, "DbgNinety");
    CB_CHECK_EQ(r.tiers.at({1, 0xC0}).name, "DbgSea");
    CB_CHECK_EQ(r.tiers.at({1, 0xA0}).name, "DbgAye");
    // LZINIT's own signature matched exactly, so it is an anchor too : locked in as Exact, and an anchor is not scored
    CB_CHECK(r.tiers.at({2, 0}).tier == est::Tier::Exact);
    CB_CHECK_EQ(r.tiers.at({2, 0}).name, "LZINIT");
    CB_CHECK_EQ(r.count(est::Tier::Exact), 5U);
    CB_CHECK_EQ(r.of(est::Tier::Exact).total, 0U);
    // a map with nothing in it estimates nothing
    CB_CHECK(est::estimate_module(m, m, "", shared_ordinals(), dump).tiers.empty());
    // the exports score the tiers: a debug build that calls the code of LZINIT something else is predicted wrong, with Exact confidence
    const est::Report wrong = est::estimate_module(m, m, " 0002:0000  Other\r\n", shared_ordinals(), dump);
    CB_CHECK(wrong.tiers.empty());   // (the export is no internal function, and nothing else is named)
    CB_CHECK_EQ(wrong.of(est::Tier::Exact).total, 1U);
    CB_CHECK_EQ(wrong.of(est::Tier::Exact).correct, 0U);
    CB_CHECK_EQ(wrong.of(est::Tier::Probable).total, 0U);
});

CB_TEST("symbols.debug_build_estimate.confident_names_drop_what_would_clash", [] {
    const ne::Module m = lz_module();
    const fx::Objdump dump = objdump_of(rows_of("scenario_a.rows"), rows_of("scenario_b.rows"));
    est::Estimates tiers;
    tiers[{1, 0x90}] = {"Good", est::Tier::Exact, 1.0};
    tiers[{1, 0xA0}] = {"Lead", est::Tier::Probable, 0.9};        // not a confident tier
    tiers[{1, 0xC0}] = {"lzinit", est::Tier::High, 1.0};          // would shadow the export LZINIT (at 2:0000), in any case
    tiers[{2, 0x40}] = {"Twin", est::Tier::Aligned, 0.8};
    tiers[{2, 0x80}] = {"Twin", est::Tier::High, 1.0};            // two functions claim the name: neither has it
    tiers[{2, 0x0}] = {"LZINIT", est::Tier::Exact, 1.0};          // the export's own place: no clash
    const db::Names names = est::confident_names(tiers, m, shared_ordinals(), dump);
    CB_CHECK_EQ(names.size(), 2U);
    CB_CHECK_EQ(names.at({1, 0x90}), "Good");
    CB_CHECK_EQ(names.at({2, 0x0}), "LZINIT");
});

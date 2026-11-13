// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/reachability: the branch parser, the walk of control flow from the entry points, the branch targets, the safe data
// regions, and the command, over LZEXPAND.DLL's first code segment with the listing of scenario_a (a canned objdump: objdump is not needed). The
// reachable set of the scenario was worked out by hand from its rows.
#include "cb/source/ne_asm/reachability.hpp"

#include <format>
#include <set>
#include <sstream>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace re = cb::source::ne_asm::reachability;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;

namespace {

std::vector<std::pair<std::uint32_t, std::string>> reach_rows(std::string_view name) {
    std::vector<std::pair<std::uint32_t, std::string>> rows;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(cb::testing::data_path("source/ne_asm/testdata/" + std::string(name))), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        rows.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return rows;
}

// objdump's listing of the rows, as it writes them
fx::Objdump reach_objdump(std::vector<std::pair<std::uint32_t, std::string>> rows) {
    return [rows = std::move(rows)](cb::ByteView, std::uint32_t) {
        std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
        for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
        return out;
    };
}

sg::Seg reach_segment() {
    const cb::formats::ne::Module module = fx::open_module(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const auto ordinals = std::make_shared<const sg::Ordinals>(sg::load_ordinals(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md")));
    return sg::Seg(module, 1, ordinals, {}, reach_objdump(reach_rows("scenario_a.rows")));
}

}  // namespace

CB_TEST("source.ne_asm.reachability.a_direct_branch_is_a_jump_a_call_or_a_loop_to_an_offset", [] {
    const auto jne = re::direct_branch("jne    0x20");
    CB_CHECK(jne.has_value());
    CB_CHECK_EQ(jne->op, "jne");
    CB_CHECK_EQ(jne->target, std::uint32_t{0x20});
    CB_CHECK_EQ(re::direct_branch("  call   0x90 ")->target, std::uint32_t{0x90});   // blanks do not matter
    CB_CHECK_EQ(re::direct_branch("loopnz 0x1f2")->op, "loopnz");
    CB_CHECK_EQ(re::direct_branch("jcxz   0x5")->op, "jcxz");
    CB_CHECK(!re::direct_branch("call   0x0:0x0"));       // a far pointer is not an offset
    CB_CHECK(!re::direct_branch("jmp    WORD PTR [bx+0x4]"));
    CB_CHECK(!re::direct_branch("jmp    ax"));
    CB_CHECK(!re::direct_branch("mov    ax,0x5"));         // not a branch
    CB_CHECK(!re::direct_branch("mov    0x5"));
    CB_CHECK(!re::direct_branch("jmp    0x"));
    CB_CHECK(!re::direct_branch("jmp    0xZZ"));
    CB_CHECK(!re::direct_branch("JMP    0x20"));           // objdump writes lower case
    CB_CHECK(!re::direct_branch("ret"));
    CB_CHECK(!re::direct_branch(""));
});

CB_TEST("source.ne_asm.reachability.an_offset_inside_the_segment_is_a_target", [] {
    CB_CHECK(re::own_segment_target("seg3:0x0012", 3) == std::optional<std::uint32_t>(0x12));
    CB_CHECK(re::own_segment_target("seg12:0xabc", 12) == std::optional<std::uint32_t>(0xabc));
    CB_CHECK(!re::own_segment_target("seg3:0x0012", 4));   // another segment's
    CB_CHECK(!re::own_segment_target("KERNEL.91", 1));
    CB_CHECK(!re::own_segment_target("seg3:0xGG", 3));
    CB_CHECK(!re::own_segment_target("seg3:0x", 3));
    CB_CHECK(!re::own_segment_target("segx:0x1", 3));
    CB_CHECK(!re::own_segment_target("seg3:12", 3));
    CB_CHECK(!re::own_segment_target("", 3));
});

CB_TEST("source.ne_asm.reachability.control_flow_from_the_entries", [] {
    const sg::Seg seg = reach_segment();
    const std::set<std::uint32_t> reached = re::reachable(seg);
    // the function at 0: its fall-through, and each of its branches' targets
    for (const std::uint32_t at : {0x0u, 0x1u, 0x3u, 0x4u, 0x7u, 0xau, 0xcu, 0xfu, 0x11u, 0x20u, 0x25u, 0x27u, 0x29u, 0x40u, 0x43u, 0x44u, 0x49u, 0x4au, 0x50u, 0x53u, 0x56u,
                                   0x60u, 0x62u, 0x64u, 0x66u, 0x80u, 0x81u, 0x82u, 0x90u, 0x93u, 0xc0u, 0xc3u, 0xc4u})
        CB_CHECK(reached.contains(at));
    // after `jmp 0x50` (0x11) control does not go on to 0x14, and nothing branches to 0x14: it and what follows it are never reached
    for (const std::uint32_t at : {0x14u, 0x17u, 0x19u, 0x1au}) CB_CHECK(!reached.contains(at));
    // after `jmp 0x90` (0xc4) it does not fall through to 0xc7; an instruction past it is no more reached
    CB_CHECK(!reached.contains(0xc7u));
    CB_CHECK(!reached.contains(0xc8u));
});

CB_TEST("source.ne_asm.reachability.every_entry_and_branch_target_needs_a_label", [] {
    const sg::Seg seg = reach_segment();
    const std::set<std::uint32_t> targets = re::branch_targets(seg);
    for (const std::uint32_t at : {0x20u, 0x40u, 0x50u, 0x60u, 0x80u, 0x90u, 0xc0u}) CB_CHECK(targets.contains(at));   // the rows' own branches
    CB_CHECK(targets.contains(0x0u));                                                                                 // a function's start
    for (const auto& [start, function] : seg.functions()) CB_CHECK(targets.contains(start));
    CB_CHECK(!targets.contains(0x14u));   // nothing enters it
    const auto branches = re::branch_targets(seg);
    CB_CHECK(!branches.empty());
    const auto all = re::branches(seg);
    CB_CHECK(std::find(all.begin(), all.end(), std::pair<std::uint32_t, std::uint32_t>{0xf, 0x20}) != all.end());
    CB_CHECK(std::find(all.begin(), all.end(), std::pair<std::uint32_t, std::uint32_t>{0x11, 0x50}) != all.end());
    CB_CHECK(std::find(all.begin(), all.end(), std::pair<std::uint32_t, std::uint32_t>{0xc4, 0x90}) != all.end());
    for (const auto& [source, target] : all) {   // only the direct ones: the far calls and the near ones with no offset are not here
        CB_CHECK(source != 0x44u);
        CB_CHECK(source != 0xa8u);
    }
});

CB_TEST("source.ne_asm.reachability.the_command_counts_what_is_never_reached", [] {
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    cb::fs::ScopedTempDir dir("reachtest");
    cb::fs::ScopedEnv root{"CB_CORPUS_ROOT", cb::fs::to_text(dir.path())};
    cb::base::Products products;
    products.target = "win95";
    products.target_folder = dir.path();
    products.checkout_root = dir.path() / "CodeBreaker";
    products.state = dir.path() / "state";
    const cb::base::Corpus corpus(products);
    const std::string lz = cb::fs::to_text(cb::testing::data_path("formats/testdata/lzexpand.dll"));
    const std::string ords = cb::fs::to_text(cb::testing::data_path("source/ne_asm/testdata/ORDINALS.md"));
    const fx::Objdump objdump = reach_objdump(reach_rows("scenario_a.rows"));
    const sg::Seg seg = reach_segment();
    const std::size_t rows = seg.rows().size(), reached = re::reachable(seg).size();
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(re::main(corpus, std::vector<std::string>{lz, "--seg", "1", "--ords", ords}, objdump), cb::kExitOk);
    const double percent = 100.0 * static_cast<double>(rows - reached) / static_cast<double>(rows);
    CB_CHECK_EQ(cap.out(), std::format("{} seg1: {} rows, {} reachable code, {} unreachable ({:.1f}% -> data candidates)\n", lz, rows, reached, rows - reached, percent));
    CB_CHECK_THROWS_AS(cb::Error, re::main(corpus, std::vector<std::string>{lz, "--seg", "99", "--ords", ords}, objdump));   // no such segment
});

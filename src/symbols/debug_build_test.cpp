// cb-unity: alone (test helpers named for this module)
// Tests of cb/symbols/debug_build on the real 16-bit module LZEXPAND.DLL (formats/testdata) with the listings of its segments 1 and 2 that the
// segment module's tests carry (source/ne_asm/testdata/scenario_a.rows, scenario_b.rows: canned, as objdump would print them, so objdump is not
// needed). One module stands in for both the retail binary and its debug build, with a .MAP written here, which is exactly the case the method
// must get right: every signature matches itself. The guards are each taken on its own: the ambiguous signature, the export that owns a signature
// under another name, the name that lives at one address, the function that is too short. The expected signatures were worked out by hand from
// the listing and the masking rules.
//
// The .MAP is read by lines (a line is ` SSSS:OOOO  name`, so a name is never joined from the next line when the offset ends
// its line); names.json is written by the module, one entry a line with the keys sorted as text, and read back as a typed table
// (segment, offset, name), with ParseError for a file that is not one; the debug build and the retail module are opened by the caller.
//
// Known limitation, as it decides which functions match: a far call `call 0x1:0x20` is masked by the branch rule first, to `call X:0x20`, so
// the offset of a far call target stays in the signature and a function that makes one is matched only where the offset happens to agree.
#include "symbols/debug_build.hpp"

#include <format>
#include <sstream>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace db = cb::symbols::debug_build;
namespace sg = cb::source::ne_asm::segment;
namespace fx = cb::source::ne_asm::fixups;
namespace ne = cb::formats::ne;
using cb::Bytes;

namespace {

std::filesystem::path testdata(std::string_view name) { return cb::testing::data_path("source/ne_asm/testdata/" + std::string(name)); }

ne::Module lz() { return ne::Module::open(cb::fs::read_file(cb::testing::data_path("formats/testdata/lzexpand.dll"))); }

sg::SharedOrdinals ordinals() { return std::make_shared<const sg::Ordinals>(sg::load_ordinals(testdata("ORDINALS.md"))); }

using RowList = std::vector<std::pair<std::uint32_t, std::string>>;

RowList rows_file(std::string_view name) {
    RowList out;
    std::istringstream in(cb::text::replace_all(cb::fs::read_text(testdata(name)), "\r\n", "\n"));
    for (std::string line; std::getline(in, line);) {
        if (line.empty()) continue;
        const std::size_t tab = line.find('\t');
        out.emplace_back(*cb::text::parse_int<std::uint32_t>(line.substr(0, tab), 16), line.substr(tab + 1));
    }
    return out;
}

std::string listing(const RowList& rows) {
    std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
    for (const auto& [offset, text] : rows) out += std::format("{:4x}:\t00 00\t{}\r\n", offset, text);
    return out;
}

// objdump as the listings give it: the lines of segment 1 for a segment of its length (456 bytes), those of segment 2 for one of its (1882), and
// nothing for any other, so that only these two segments of the module have code to speak of.
fx::Objdump objdump_with(RowList first, RowList second) {
    return [first = std::move(first), second = std::move(second)](cb::ByteView code, std::uint32_t) {
        if (code.size() == 456) return listing(first);
        if (code.size() == 1882) return listing(second);
        return std::string();
    };
}

fx::Objdump objdump() { return objdump_with(rows_file("scenario_a.rows"), rows_file("scenario_b.rows")); }

db::Signature of(const ne::Module& m, std::uint32_t segment, std::uint32_t start, const fx::Objdump& dump) {
    const sg::Seg seg(m, segment, ordinals(), {}, dump);
    return db::signature(seg, start);
}

}  // namespace

CB_TEST("symbols.debug_build.a_signature_masks_what_depends_on_the_offset", [] {
    const ne::Module module = lz();
    const fx::Objdump dump = objdump();
    // sub_0090 as the listing has it: whitespace collapsed, nothing offset-dependent in it (a [bp+..] operand is not an absolute address)
    const db::Signature ninety = {"push bp", "mov bp,sp", "mov ax,WORD PTR [bp+0x6]", "mov bx,WORD PTR [bp+0xa]", "pop bp", "ret 0x4"};
    CB_CHECK(of(module, 1, 0x90, dump) == ninety);
    // sub_00a0 ends in a far call: the branch rule masks its first number and leaves the offset (see the header of this file)
    const db::Signature a0 = {"enter 0x2,0x0", "mov bx,WORD PTR [bp+0x8]", "call X:0x0", "retf"};
    CB_CHECK(of(module, 1, 0xA0, dump) == a0);

    // each rule on a listing of its own: an absolute memory operand, a near jump, a far pointer that is not a branch's operand, an immediate
    RowList rows = rows_file("scenario_a.rows");
    for (auto& [offset, text] : rows) {
        if (offset == 0x93) text = "mov    ax,WORD   PTR [0x1234]";
        if (offset == 0x96) text = "jne    0x00a0";
        if (offset == 0x99) text = "ljmp   0x1:0x2";
    }
    const db::Signature masked = {"push bp", "mov bp,sp", "mov ax,WORD PTR [X]", "jne X", "ljmp X:X", "ret 0x4"};
    CB_CHECK(of(module, 1, 0x90, objdump_with(rows, rows_file("scenario_b.rows"))) == masked);
    // a function that is not there has no signature
    CB_CHECK(of(module, 3, 0, dump).empty());
});

CB_TEST("symbols.debug_build.a_map_lists_the_names_at_their_places", [] {
    const auto names = db::parse_map(
        " Address         Publics by Value\r\n"
        "\r\n"
        " 0001:0000       _Foo\r\n"
        " 0001:0090       BAR_baz   extra words\r\n"
        " 0002:00FF       x\r\n"
        "junk line 0003:0000 nope\r\n"
        " 0001:0090       Later\r\n"
        " 0001:zzzz       notahex\r\n"
        " 0004:0010\r\n"
        " 0004:0020       \r\n"
        "  0005:0000      indented\r\n"
        " 000a:00ff       lower_hex\r\n");
    CB_CHECK_EQ(names.size(), 3U);
    CB_CHECK_EQ(names.at(1).at(0), "_Foo");
    CB_CHECK_EQ(names.at(1).at(0x90), "Later");   // a place named twice keeps the later name
    CB_CHECK_EQ(names.at(2).at(0xFF), "x");
    CB_CHECK_EQ(names.at(0xA).at(0xFF), "lower_hex");
    CB_CHECK(!names.contains(3) && !names.contains(4) && !names.contains(5));
    CB_CHECK(db::parse_map("").empty());
});

CB_TEST("symbols.debug_build.the_index_names_the_signatures_of_a_debug_build", [] {
    const ne::Module module = lz();
    const fx::Objdump dump = objdump();
    const std::string map = " 0001:0090  DebugNinety\r\n 0001:00C0  DebugC0\r\n 0001:00A0  Short\r\n"
                            " 0001:0092  Inside\r\n"   // inside `mov bp,sp`: not the start of an instruction
                            " 0003:0000  NoCodeThere\r\n 000C:0000  DataSegment\r\n 0001:0FFF  PastTheEnd\r\n";
    const db::SignatureIndex index = db::build_index(module, map, ordinals(), dump);
    CB_CHECK_EQ(index.size(), 3U);
    CB_CHECK_EQ(index.at(of(module, 1, 0x90, dump)), "DebugNinety");
    CB_CHECK_EQ(index.at(of(module, 1, 0xC0, dump)), "DebugC0");
    CB_CHECK_EQ(index.at(of(module, 1, 0xA0, dump)), "Short");
    CB_CHECK(db::build_index(module, "", ordinals(), dump).empty());
    CB_CHECK_THROWS_AS(cb::FormatError, (void)db::build_index(ne::Module::open(cb::fs::read_file(cb::testing::data_path("formats/testdata/splitter.vxd"))), map, ordinals(), dump));
});

CB_TEST("symbols.debug_build.a_module_is_named_from_a_debug_build_of_itself", [] {
    const ne::Module module = lz();
    const fx::Objdump dump = objdump();
    const std::string map = " 0001:0090  DebugNinety\r\n 0001:00C0  DebugC0\r\n 0001:00A0  Short\r\n";
    const db::Match found = db::recover(module, module, map, ordinals(), dump);
    // only sub_0090 has five instructions or more (six): sub_00a0 has four and sub_00c0 three, too skeleton-like to trust
    CB_CHECK_EQ(found.names.size(), 1U);
    CB_CHECK_EQ(found.names.at({1, 0x90}), "DebugNinety");
    CB_CHECK_EQ(found.agree, 0U);   // no known export is renamed
    CB_CHECK_EQ(found.disagree, 0U);
    // the length bar is the caller's
    const db::SignatureIndex index = db::build_index(module, map, ordinals(), dump);
    CB_CHECK_EQ(db::match_module(module, index, ordinals(), 3, dump).names.size(), 3U);   // 6, 4 and 3 instructions
    CB_CHECK_EQ(db::match_module(module, index, ordinals(), 4, dump).names.size(), 2U);
    CB_CHECK_EQ(db::match_module(module, index, ordinals(), 7, dump).names.size(), 0U);
});

CB_TEST("symbols.debug_build.a_name_that_lives_at_two_addresses_is_dropped", [] {
    const ne::Module module = lz();
    const fx::Objdump dump = objdump();
    // two functions (sub_0000 of 32 instructions, sub_0090 of six) with different signatures named the same: a symbol lives at one address, so
    // both go; named differently, both stay
    const std::string map = " 0001:0000  SameName\r\n 0001:0090  SameName\r\n";
    CB_CHECK(db::recover(module, module, map, ordinals(), dump).names.empty());
    CB_CHECK_EQ(db::recover(module, module, " 0001:0000  One\r\n 0001:0090  Two\r\n", ordinals(), dump).names.size(), 2U);
});

CB_TEST("symbols.debug_build.an_export_keeps_its_own_signature", [] {
    const ne::Module module = lz();
    const fx::Objdump dump = objdump();
    const db::Signature init = of(module, 2, 0, dump);   // LZINIT, the first export, at the start of segment 2
    CB_CHECK(init.size() >= db::kMinLength);
    // named as it is: it agrees with itself and is "renamed" to the same name
    db::Match same = db::match_module(module, {{init, "LZINIT"}}, ordinals(), db::kMinLength, dump);
    CB_CHECK_EQ(same.agree, 1U);
    CB_CHECK_EQ(same.disagree, 0U);
    CB_CHECK_EQ(same.names.at({2, 0}), "LZINIT");
    // a debug name for the signature an export owns, under another name: dropped as collision-prone, and counted nowhere
    const db::Match other = db::match_module(module, {{init, "OtherName"}}, ordinals(), db::kMinLength, dump);
    CB_CHECK(other.names.empty());
    CB_CHECK_EQ(other.agree + other.disagree, 0U);
    // never a real export's name at another offset: sub_0090 matched to "LZINIT" would shadow the export at segment 2
    const db::Match shadow = db::match_module(module, {{of(module, 1, 0x90, dump), "LZINIT"}}, ordinals(), db::kMinLength, dump);
    CB_CHECK(shadow.names.empty());
    // the same lookup is case-insensitive
    const db::Match lower = db::match_module(module, {{of(module, 1, 0x90, dump), "lzinit"}}, ordinals(), db::kMinLength, dump);
    CB_CHECK(lower.names.empty());
});

CB_TEST("symbols.debug_build.names_json_round_trips", [] {
    const db::Names names = {{{1, 0x90}, "A"}, {{10, 0x10}, "B"}, {{2, 0xFE}, "C\"q"}};
    const std::string text = db::to_json(names);
    CB_CHECK_EQ(text, "{\n\"10:0010\": \"B\",\n\"1:0090\": \"A\",\n\"2:00fe\": \"C\\\"q\"\n}");   // keys sorted as text ('0' sorts before ':')
    const auto overrides = db::parse_overrides(text);
    CB_CHECK_EQ(overrides.size(), 3U);
    CB_CHECK_EQ(overrides.at(1).at(0x90), "A");
    CB_CHECK_EQ(overrides.at(10).at(0x10), "B");
    CB_CHECK_EQ(overrides.at(2).at(0xFE), "C\"q");
    CB_CHECK_EQ(db::to_json({}), "{}");
    CB_CHECK(db::parse_overrides("{}").empty());
    CB_CHECK_EQ((db::FunctionKey{12, 0x2A}).text(), "12:002a");
    CB_CHECK_EQ((db::FunctionKey{1, 0x12345}).text(), "1:12345");
    // not a names file
    CB_CHECK_THROWS_AS(cb::ParseError, (void)db::parse_overrides("{"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)db::parse_overrides("[1, 2]"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)db::parse_overrides("{\"nocolon\": \"x\"}"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)db::parse_overrides("{\"1:zz\": \"x\"}"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)db::parse_overrides("{\"1:0010\": 7}"));
});

CB_TEST("symbols.debug_build.the_overrides_name_the_functions_of_a_segment", [] {
    // what the file feeds the disassembler: the segment's own names for its anonymous functions
    const ne::Module module = lz();
    const auto overrides = db::parse_overrides("{\n\"1:0090\": \"Renamed\"\n}");
    const sg::Seg seg(module, 1, ordinals(), overrides.at(1), objdump_with(rows_file("scenario_a.rows"), {}));
    CB_CHECK_EQ(seg.name_at(0x90), "Renamed");
    CB_CHECK_EQ(seg.name_at(0xC0), "sub_00c0");
});

namespace {

struct World {
    cb::fs::ScopedTempDir dir{"cbdebugbuild"};
    cb::fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    std::optional<cb::fs::ScopedEnv> root_env;
    std::unique_ptr<cb::base::Corpus> corpus;
    World() {
        cb::fs::make_dirs(dir.path() / "corpus");
        root_env.emplace("CB_CORPUS_ROOT", cb::fs::to_text(dir.path() / "corpus"));
        cb::base::Products products;
        products.target = "win95";
        products.target_folder = dir.path() / "corpus";
        products.checkout_root = dir.path() / "CodeBreaker";
        products.state = dir.path() / "state";
        corpus = std::make_unique<cb::base::Corpus>(products);
    }
};

}  // namespace

CB_TEST("symbols.debug_build.the_command_needs_the_corpus_and_the_sdk", [] {
    World world;
    const db::Environment env{*world.corpus, world.dir.path() / "state", objdump()};
    // an empty corpus has no retail KRNL386
    cb::testing::CapturedStd captured;
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)db::main(env, std::vector<std::string>{"--ords", cb::fs::to_text(testdata("ORDINALS.md"))}));
});

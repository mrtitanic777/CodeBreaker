// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/ne_asm/fixups on the real 16-bit module LZEXPAND.DLL (formats/testdata): the fix-up sites of its segments are
// what the Python module gave, read back by hand against the relocation tables (the chain of a record visits every site that
// holds the link to the next one); a few records are rewritten to reach the label kinds the module does not use; the listing is
// made with a stand-in for objdump (a canned text), and with the real one when it is installed.
//
// DIVERGES from the Python module, by design: a segment that does not exist is a UsageError (the Python indexed the list: a wrong
// index counted from the end, or an IndexError); a segment with no data in the file has no fix-up sites (the Python read the
// start of the file as its bytes); a negative start or count is a UsageError of the command line; the temporary file objdump
// reads is in a folder of its own that is removed with it; the sites come sorted by offset.
#include "cb/source/ne_asm/fixups.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"

namespace fx = cb::source::ne_asm::fixups;
namespace ne = cb::formats::ne;
using cb::Bytes;

namespace {

Bytes lz_bytes() { return cb::fs::read_file(cb::testing::data_path("formats/testdata/lzexpand.dll")); }

bool has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

// The sites of a segment as {offset: label}.
std::map<std::uint32_t, std::string> sites(const ne::Module& module, std::size_t segment) {
    const auto segments = module.segments();
    std::map<std::uint32_t, std::string> out;
    for (const auto& [offset, target] : fx::build_fixup_map(module, segments.at(segment - 1), module.module_refs())) out[offset] = target.label;
    return out;
}

// Segment 1's relocation table: where its first record lies in the file.
std::size_t first_record(const ne::Module& module) {
    const ne::Segment segment = module.segments().front();
    return segment.file_offset + segment.length + 2;
}

// objdump's text for a code blob, as the stand-in prints it: what the real one wrote for the first bytes of LZEXPAND's segment 1
// (the header lines, the CRLF line ends of its Windows build, one instruction wrapped on a second line).
std::string canned(cb::ByteView code, std::uint32_t base) {
    std::string out = "\r\nsegment.bin:     file format binary\r\n\r\n\r\nDisassembly of section .data:\r\n\r\n00000000 <.data>:\r\n";
    const auto line = [&](std::uint32_t at, std::string_view bytes, std::string_view text) { out += std::format("{:4x}:\t{}\t{}\r\n", at, bytes, text); };
    line(base + 0x00, "1e                  ", "push   ds");
    line(base + 0x01, "58                  ", "pop    ax");
    line(base + 0x24, "9a 58 00 00 00       ", "call   0x0:0x58");
    line(base + 0x29, "c7 06 44 00 00 00    ", "mov    WORD PTR ds:0x44,0x0");
    line(base + 0x2a, "00 00 ", "");   // (a wrapped continuation line has no mnemonic)
    out += std::format("; {} bytes at 0x{:x}\r\n", code.size(), base);
    return out;
}

}  // namespace

CB_TEST("source.ne_asm.fixups.open_module_forms", [] {
    const Bytes file = lz_bytes();
    CB_CHECK_EQ(fx::open_module(Bytes(file)).module_name(), "LZEXPAND");
    // a raw embedded module, starting at its "NE"
    const std::uint32_t ne_offset = file[0x3C] | (file[0x3D] << 8) | (file[0x3E] << 16) | (file[0x3F] << 24);
    const Bytes raw(file.begin() + ne_offset, file.end());
    CB_CHECK(raw[0] == 'N' && raw[1] == 'E');
    CB_CHECK_EQ(fx::open_module(Bytes(raw)).module_name(), "LZEXPAND");
    CB_CHECK_EQ(fx::open_module(cb::testing::data_path("formats/testdata/lzexpand.dll")).segments().size(), 12u);
    // anything else is not a module
    CB_CHECK_THROWS_AS(cb::FormatError, (void)fx::open_module(Bytes{}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)fx::open_module(Bytes{'M'}));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)fx::open_module(Bytes(400, 'X')));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)fx::open_module(Bytes{'M', 'Z', 0, 0}));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)fx::open_module(cb::testing::data_path("formats/testdata/no-such-module.dll")));
});

CB_TEST("source.ne_asm.fixups.sites_of_real_segments", [] {
    const ne::Module module = fx::open_module(lz_bytes());
    using Sites = std::map<std::uint32_t, std::string>;
    // segment 1 imports from KERNEL only (the Python module's answer; each chain starts at the record's offset and follows the links)
    CB_CHECK(sites(module, 1) == (Sites{{37, "KERNEL.15"}, {88, "KERNEL.15"}, {117, "KERNEL.15"}, {141, "KERNEL.17"}, {164, "KERNEL.17"}, {205, "KERNEL.17"},
                                         {231, "KERNEL.17"}, {257, "KERNEL.17"}, {331, "KERNEL.83"}, {363, "KERNEL.85"}, {391, "KERNEL.84"}, {419, "KERNEL.84"},
                                         {439, "KERNEL.81"}}));
    // segment 2 calls its own entries (a call into a moveable segment goes through the entry table: the module's own name, the ordinal)
    const Sites two = sites(module, 2);
    CB_CHECK_EQ(two.size(), 37u);
    CB_CHECK_EQ(two.at(25), "LZEXPAND.16");
    CB_CHECK_EQ(two.at(74), "LZEXPAND.15");
    CB_CHECK_EQ(two.at(174), "KERNEL.15");
    CB_CHECK_EQ(two.at(1865), "KERNEL.17");
    // the segment of 0x18 bytes has no relocations; the data segment has none either
    CB_CHECK(sites(module, 3).empty());
    CB_CHECK(sites(module, 12).empty());
    // segment 7 mixes KERNEL imports and entries of the module itself
    CB_CHECK(sites(module, 7) == (Sites{{25, "KERNEL.84"}, {56, "KERNEL.84"}, {154, "KERNEL.82"}, {182, "KERNEL.86"}, {269, "LZEXPAND.16"}, {308, "LZEXPAND.15"},
                                         {371, "LZEXPAND.20"}, {404, "LZEXPAND.26"}, {425, "LZEXPAND.25"}, {511, "LZEXPAND.24"}, {537, "LZEXPAND.16"},
                                         {567, "KERNEL.81"}, {591, "KERNEL.88"}, {612, "LZEXPAND.14"}, {625, "LZEXPAND.15"}, {666, "KERNEL.81"},
                                         {688, "LZEXPAND.24"}, {727, "KERNEL.81"}, {735, "KERNEL.81"}}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)fx::annotated_listing(module, "x.dll", {13, 0, std::nullopt}, canned));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)fx::annotated_listing(module, "x.dll", {0, 0, std::nullopt}, canned));
});

CB_TEST("source.ne_asm.fixups.every_label_kind", [] {
    // Rewrite the first record of segment 1 (the KERNEL.15 chain: sites 37, 88, 117) and read the labels it gets.
    const Bytes original = lz_bytes();
    const std::size_t record = first_record(ne::Module::open(Bytes(original)));
    const auto labelled = [&](std::uint8_t flags, std::uint16_t b4, std::uint16_t b6) {
        Bytes data = original;
        data[record + 1] = flags;
        data[record + 4] = static_cast<std::uint8_t>(b4), data[record + 5] = static_cast<std::uint8_t>(b4 >> 8);
        data[record + 6] = static_cast<std::uint8_t>(b6), data[record + 7] = static_cast<std::uint8_t>(b6 >> 8);
        const ne::Module module = ne::Module::open(std::move(data));
        return fx::build_fixup_map(module, module.segments().front(), module.module_refs()).at(37);
    };
    CB_CHECK_EQ(labelled(1, 2, 7).label, "USER.7");                  // an ordinal of the second module
    CB_CHECK_EQ(labelled(1, 9, 7).label, "mod9.7");                  // a module the table does not hold
    CB_CHECK_EQ(labelled(1, 0, 7).label, "mod0.7");
    CB_CHECK(!labelled(1, 2, 7).segment);
    CB_CHECK_EQ(labelled(0, 2, 0x120).label, "seg2:0x0120");        // a fixed segment of the module
    CB_CHECK(labelled(0, 2, 0x120).segment && labelled(0, 2, 0x120).segment->segment == 2 && labelled(0, 2, 0x120).segment->offset == 0x120);
    CB_CHECK_EQ(labelled(0, 0xFF, 273).label, "LZEXPAND.273");     // segment 0xFF: the module's own entry-table ordinal
    CB_CHECK(!labelled(0, 0xFF, 273).segment);
    CB_CHECK_EQ(labelled(3, 5, 0).label, "osfixup#5");               // an emulator fix-up
    CB_CHECK_EQ(labelled(2, 1, 0).label.substr(0, 7), "KERNEL.");   // an import by name: the name the imported-name table has at that offset
});

CB_TEST("source.ne_asm.fixups.chain_rules", [] {
    const Bytes original = lz_bytes();
    const ne::Module reference = ne::Module::open(Bytes(original));
    const ne::Segment segment = reference.segments().front();
    const std::size_t record = first_record(reference);
    const auto map_of = [&](const Bytes& data) {
        const ne::Module module = ne::Module::open(Bytes(data));
        return fx::build_fixup_map(module, module.segments().front(), module.module_refs());
    };
    CB_CHECK_EQ(map_of(original).size(), 13u);
    // an additive record holds an addend, not the link: only the site itself
    Bytes additive = original;
    additive[record + 1] |= 0x04;
    CB_CHECK_EQ(map_of(additive).size(), 11u);   // 13 sites less the two chain successors of site 37 (88 and 117)
    CB_CHECK(!map_of(additive).contains(88));
    // a link to itself ends the chain
    Bytes loop = original;
    loop[segment.file_offset + 37] = 37, loop[segment.file_offset + 38] = 0;
    CB_CHECK_EQ(map_of(loop).size(), 11u);
    // a link that leaves the segment ends it too; the site whose link it was is still a site
    Bytes outside = original;
    outside[segment.file_offset + 88] = 0xF0, outside[segment.file_offset + 89] = 0x7F;
    CB_CHECK(map_of(outside).contains(88) && !map_of(outside).contains(117));
});

CB_TEST("source.ne_asm.fixups.parse_objdump", [] {
    const auto lines = fx::parse_objdump(canned({}, 0));
    CB_CHECK_EQ(lines.size(), 13u);
    CB_CHECK(!lines[0].offset && lines[0].text.empty());                        // the blank line first
    CB_CHECK(lines[1].text.find("file format binary") != std::string::npos && !lines[1].offset);
    // the instruction lines: the address, the line as printed, the assembly (the last tab field, trimmed)
    CB_CHECK(lines[7].offset && *lines[7].offset == 0 && lines[7].instruction == "push   ds");
    CB_CHECK(lines[9].offset && *lines[9].offset == 0x24 && lines[9].instruction == "call   0x0:0x58");
    CB_CHECK(lines[10].instruction == "mov    WORD PTR ds:0x44,0x0");
    CB_CHECK(lines[11].offset && *lines[11].offset == 0x2a && lines[11].instruction.empty());   // a wrapped line: an address and no mnemonic
    CB_CHECK(!lines[12].offset);                                                  // text with no tab-colon is not an instruction
    CB_CHECK(lines[9].text.starts_with("  24:\t9a"));
    // a head that is not a number is not an instruction line
    const auto odd = fx::parse_objdump("hello:\tworld\n   zz:\tpush bp\n  1f:\tnop\n");
    CB_CHECK_EQ(odd.size(), 3u);
    CB_CHECK(!odd[0].offset && !odd[1].offset && odd[2].offset && *odd[2].offset == 0x1f && odd[2].instruction == "nop");
    CB_CHECK(fx::parse_objdump("").empty());
});

CB_TEST("source.ne_asm.fixups.annotated_listing_with_a_stand_in_for_objdump", [] {
    const ne::Module module = fx::open_module(lz_bytes());
    std::uint32_t asked_base = 99;
    std::size_t asked_size = 0;
    const fx::Objdump spy = [&](cb::ByteView code, std::uint32_t base) {
        asked_base = base, asked_size = code.size();
        return canned(code, base);
    };
    const std::string text = fx::annotated_listing(module, "C:/mini/LZEXPAND.DLL", {1, 0, std::nullopt}, spy);
    CB_CHECK_EQ(asked_base, 0u);
    CB_CHECK_EQ(asked_size, 0x1c8u);
    CB_CHECK(text.starts_with("# LZEXPAND.DLL  segment #1 (CODE, 0x1c8 bytes)  imports: KERNEL, USER\n# 13 fixup sites in this segment\n\n"));
    // the call at 0x24 has the site 0x25 (KERNEL.15) in its bytes; the mov at 0x29 has none
    CB_CHECK(has(text, "  24:\t9a 58 00 00 00       \tcall   0x0:0x58   ; -> KERNEL.15\n"));
    CB_CHECK(has(text, "  29:\tc7 06 44 00 00 00    \tmov    WORD PTR ds:0x44,0x0\n"));
    CB_CHECK(!has(text, "ds:0x44,0x0   ;"));
    CB_CHECK(!has(text, "push   ds   ;"));
    // a range: the bytes handed over start at `start`, and the addresses are theirs
    (void)fx::annotated_listing(module, "x", {1, 16, 100}, spy);
    CB_CHECK_EQ(asked_base, 16u);
    CB_CHECK_EQ(asked_size, 100u);
    (void)fx::annotated_listing(module, "x", {1, 0x1c0, 100}, spy);   // the count is cut at the end of the segment
    CB_CHECK_EQ(asked_size, 8u);
    (void)fx::annotated_listing(module, "x", {1, 100000, 40}, spy);   // a start past the end lists nothing, at the address asked
    CB_CHECK_EQ(asked_size, 0u);
    CB_CHECK_EQ(asked_base, 100000u);
    // a data segment says so first
    const std::string data = fx::annotated_listing(module, "x.dll", {12, 0, 16}, spy);
    CB_CHECK(data.starts_with("(segment 12 is DATA)\n# x.dll  segment #12 (DATA, 0x284 bytes)"));
    // the last instruction's span is eight bytes: a site 7 bytes past its start is its own, 8 is not
    const fx::Objdump last = [](cb::ByteView, std::uint32_t) { return std::string("  28:\tcc\tint3\n"); };
    CB_CHECK(!has(fx::annotated_listing(module, "x", {1, 0, std::nullopt}, last), "->"));   // no site in [0x28, 0x30)
    const fx::Objdump near_site = [](cb::ByteView, std::uint32_t) { return std::string("  1e:\tcc\tint3\n"); };   // 0x1e + 8 reaches 0x25
    CB_CHECK(has(fx::annotated_listing(module, "x", {1, 0, std::nullopt}, near_site), "int3   ; -> KERNEL.15"));
});

CB_TEST("source.ne_asm.fixups.entry_listing", [] {
    const ne::Module module = fx::open_module(lz_bytes());
    const fx::Objdump spy = [](cb::ByteView code, std::uint32_t base) { return std::format("  {:x}:\t90\tnop\r\n; {} bytes\r\n", base, code.size()); };
    // ordinal 3 is LZINIT, segment 2 offset 0
    const std::string text = fx::entry_listing(module, "LZEXPAND.DLL", 3, spy);
    CB_CHECK(text.starts_with("# entry ordinal 3 -> segment 2 offset 0x0\n# LZEXPAND.DLL  segment #2 (CODE, 0x75a bytes)"));
    CB_CHECK(has(text, "; 128 bytes"));
    // ordinal 2 is LZOPENFILE at 0x262: the 128 bytes are cut at the segment's end only if it ends first
    CB_CHECK(has(fx::entry_listing(module, "LZEXPAND.DLL", 2, spy), "# entry ordinal 2 -> segment 2 offset 0x262\n"));
    CB_CHECK_EQ(fx::entry_listing(module, "LZEXPAND.DLL", 31, spy), "ordinal 31 not exported\n");
    CB_CHECK_EQ(fx::entry_listing(module, "LZEXPAND.DLL", 0, spy), "ordinal 0 not exported\n");
});

CB_TEST("source.ne_asm.fixups.command", [] {
    const auto path = cb::testing::data_path("formats/testdata/lzexpand.dll").string();
    const auto run = [&](std::vector<std::string> args) { return fx::main(args, canned); };
    cb::testing::CapturedStd captured;
    CB_CHECK_EQ(run({path, "1", "--start", "0x10", "--count", "20"}), cb::kExitOk);
    CB_CHECK_EQ(run({path, "--entry", "3"}), cb::kExitOk);
    CB_CHECK_EQ(run({path, "--entry", "40"}), cb::kExitOk);
    const std::string out = captured.out();
    CB_CHECK(has(out, "# lzexpand.dll  segment #1 (CODE, 0x1c8 bytes)  imports: KERNEL, USER"));
    CB_CHECK(has(out, "# entry ordinal 3 -> segment 2 offset 0x0"));
    CB_CHECK(has(out, "ordinal 40 not exported"));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({path, "--start", "zz"}));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({path, "99"}));
});

CB_TEST("source.ne_asm.fixups.with_the_real_objdump", [] {
    // objdump of binutils is not part of the repository: without it on the PATH the stand-in tests above are what runs
    try {
        if (!cb::process::run({.argv = {"objdump", "--version"}}).ok()) return;
    } catch (const cb::ToolError&) {
        return;
    }
    const ne::Module module = fx::open_module(lz_bytes());
    const std::string text = fx::annotated_listing(module, "LZEXPAND.DLL", {1, 0, std::nullopt});
    CB_CHECK(has(text, "# 13 fixup sites in this segment"));
    CB_CHECK(has(text, "push   ds\n"));
    CB_CHECK(has(text, "call   0x0:0x58   ; -> KERNEL.15\n"));
    // every instruction that has a site among its bytes is annotated: 13 sites, 13 instructions (one site each)
    std::size_t annotated = 0;
    for (std::size_t at = text.find("   ; -> "); at != std::string::npos; at = text.find("   ; -> ", at + 1)) ++annotated;
    CB_CHECK_EQ(annotated, 13u);
});

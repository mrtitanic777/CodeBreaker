// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/link. The source and the .DEF, and the body of a loadable object, are checked on a recovery made by hand over the
// object table of splitter.vxd (the expected text was worked out from the rules in the header, not recorded). The tools are stand-ins: an
// assembler that hands back a marker object, and linkers that hand back the original driver (so the rebuild must compare as byte exact) or
// nothing (so each stage's failure is reported). The overlay is tried on the original's own bytes with a few of the recovered ones damaged,
// and on another driver's (which must be refused). The real linker's plumbing is checked with a batch file standing in for LINK.
//
// Behaviour pinned here: the .DEF is written with the line ends the text has;
// the percentage of an object match is kept as a number and rounded where it is shown; an assembled object that cannot be matched is a FormatError
// carrying the same diagnosis; the loadable build assembles with the native ML or the period assembler of the
// toolchain; the tools are given, not found from a root folder.
#include "source/vxd/link.hpp"

#include <algorithm>
#include <format>
#include <optional>
#include <ranges>
#include <type_traits>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/vxd/le_writer.hpp"

namespace vx = cb::source::vxd;
namespace lk = cb::source::vxd::link;
namespace mx = cb::source::vxd::masm;
namespace le = cb::formats::le;
using cb::Bytes;

namespace {

std::string rendered(std::string_view s) { return "\"" + std::string(s) + "\""; }
template <class R>
    requires(std::ranges::range<R> && !std::is_convertible_v<const R&, std::string_view>)
std::string rendered(const R& range) {
    std::string out = "[";
    for (const auto& item : range) out += (out.size() > 1 ? ", " : "") + rendered(item);
    return out + "]";
}
#define CHECK_SAME(actual, expected) CB_CHECK_EQ(rendered(actual), rendered(expected))

le::Module splitter() { return le::Module::open(cb::testing::data_path("formats/testdata/splitter.vxd")); }
Bytes splitter_bytes() { return cb::fs::read_file(cb::testing::data_path("formats/testdata/splitter.vxd")); }

vx::Instruction insn(std::uint32_t size, std::string mnemonic, std::string operands, Bytes raw) {
    vx::Instruction i;
    i.size = size;
    i.mnemonic = std::move(mnemonic);
    i.operands = std::move(operands);
    i.raw = std::move(raw);
    return i;
}

// A recovery over object 1: push ebp / mov ebp,esp / jmp 0xc / seven data bytes / loc_1_000C: pop ebp / ret.
struct Hand {
    le::Module driver = splitter();
    vx::DdkNames names;
    mx::EmissionParts parts;
    Hand() {
        parts.code = {{0x55, 0x8B, 0xEC, 0xEB, 0x07, 1, 2, 3, 4, 5, 6, 7, 0x5D, 0xC3}};
        auto& i = parts.instructions;
        i[{1, 0}] = insn(1, "push", "ebp", {0x55});
        i[{1, 1}] = insn(2, "mov", "ebp, esp", {0x8B, 0xEC});
        i[{1, 3}] = insn(2, "jmp", "0xc", {0xEB, 0x07});
        i[{1, 12}] = insn(1, "pop", "ebp", {0x5D});
        i[{1, 13}] = insn(1, "ret", "", {0xC3});
        parts.labels[{1, 12}] = "loc_1_000C";
    }
    mx::Emission emission() { return mx::Emission(driver, names, parts); }
};

// What the stand-in tools were given.
struct Seen {
    std::vector<std::string> sources;
    std::vector<std::string> defs;
    std::vector<Bytes> objects;
};

mx::Toolchain fake_assembler(Seen& seen, bool works = true) {
    mx::Toolchain tools;
    tools.period = [&seen, works](std::span<const std::string> texts) {
        std::vector<mx::Assembled> out;
        for (const std::string& t : texts) {
            seen.sources.push_back(t);
            out.push_back(works ? mx::Assembled{Bytes{1, 2, 3}, "assembled\n"} : mx::Assembled{std::nullopt, std::string(2000, 'x') + "A2005: symbol redefinition\n"});
        }
        return out;
    };
    return tools;
}

lk::LinkFn fake_link(Seen& seen, std::optional<Bytes> gives) {
    return [&seen, gives](cb::ByteView object, std::string_view def, const std::filesystem::path&) {
        seen.objects.emplace_back(object.begin(), object.end());
        seen.defs.emplace_back(def);
        return lk::Linked{gives, "LINK : warning\n"};
    };
}

}  // namespace

CB_TEST("source.vxd.link.def_attributes_come_from_the_object_flags", [] {
    CB_CHECK_EQ(lk::segment_attributes(0), "NONDISCARDABLE");
    CB_CHECK_EQ(lk::segment_attributes(0x40), "PRELOAD NONDISCARDABLE");
    CB_CHECK_EQ(lk::segment_attributes(0x10), "DISCARDABLE");
    CB_CHECK_EQ(lk::segment_attributes(0x40 | 0x20 | 0x8000 | 0x4000 | 0x200), "PRELOAD NONDISCARDABLE SHARED IOPL CONFORMING RESIDENT");
    CB_CHECK_EQ(lk::segment_attributes(0x10 | 0x20), "DISCARDABLE SHARED");
});

CB_TEST("source.vxd.link.the_def_segment_map_is_the_ddks", [] {
    const std::string_view segments = lk::kVxdDefSegments;
    CB_CHECK(segments.starts_with("    _LPTEXT     CLASS 'LCODE'   PRELOAD NONDISCARDABLE\r\n"));
    CB_CHECK(segments.ends_with("    _RCODE      CLASS 'RCODE'\r\n"));
    CB_CHECK_EQ(std::count(segments.begin(), segments.end(), '\n'), 16);
});

CB_TEST("source.vxd.link.the_source_and_def_of_a_driver", [] {
    Hand hand;
    hand.parts.ddb = le::Ddb{};
    hand.parts.ddb->object = 1;
    hand.parts.ddb->rva = 5;   // the DDB lies in the data, so VXD_DDB is defined there
    hand.parts.ddb->name = "TEST";
    mx::Emission code = hand.emission();
    const lk::SourceAndDef plain = lk::emit_source_and_def(code, "TEST");
    const std::vector<std::string> source = {".386p", "SEG01 segment dword public use32 'LCODE01'", "        push   ebp", "        mov    ebp, esp", "        jmp    SHORT loc_1_000C",
                                             "public VXD_DDB", "VXD_DDB label byte", "        db  001h, 002h, 003h, 004h, 005h, 006h, 007h", "loc_1_000C:",
                                             "        pop    ebp", "        ret", "SEG01 ends", "        end"};
    CHECK_SAME(cb::text::lines(plain.source), source);
    CB_CHECK(plain.source.ends_with("        end\r\n"));
    const le::Object& object = hand.driver.objects().front();
    const std::vector<std::string> def = {"VXD TEST", "DESCRIPTION 'TEST'", "", "SEGMENTS", "    SEG01    CLASS 'LCODE01' " + lk::segment_attributes(object.flags), "", "EXPORTS",
                                          "        VXD_DDB @1"};
    CHECK_SAME(cb::text::lines(plain.def), def);

    // an instruction forced raw is db; the rest is unchanged
    const lk::SourceAndDef forced = lk::emit_source_and_def(code, "TEST", {{1, {1}}});
    CB_CHECK(forced.source.find("\r\n        db  08Bh, 0ECh\r\n") != std::string::npos);
    CB_CHECK(forced.source.find("mov    ebp, esp") == std::string::npos);
    // with no DDB nothing is exported as a label
    Hand bare;
    mx::Emission bare_code = bare.emission();
    CB_CHECK(lk::emit_source_and_def(bare_code, "TEST").source.find("VXD_DDB") == std::string::npos);
});

namespace {

// A recovery for the loadable body: a call whose rel32 is a relocation, a load of an absolute address that is one, a ret, data, a labelled data
// pointer.
Hand body_hand() {
    Hand hand;
    hand.parts.instructions.clear();
    hand.parts.labels.clear();
    hand.parts.code = {Bytes(0x18, 0)};
    const auto place = [&](std::uint32_t at, vx::Instruction i) {
        std::copy(i.raw.begin(), i.raw.end(), hand.parts.code[0].begin() + at);
        hand.parts.instructions[{1, at}] = std::move(i);
    };
    place(0x00, insn(5, "call", "0x10", {0xE8, 0, 0, 0, 0}));
    place(0x05, insn(5, "mov", "eax, dword ptr [0x1000]", {0xA1, 0, 0x10, 0, 0}));
    place(0x0A, insn(1, "ret", "", {0xC3}));
    place(0x10, insn(1, "ret", "", {0xC3}));
    hand.parts.instructions[{1, 0}].fixup = le::Fixup(le::InternalFixup{1, 0x10, 8});
    hand.parts.instructions[{1, 5}].fixup = le::Fixup(le::InternalFixup{1, 0x14, 7});
    hand.parts.fixups.insert_or_assign(vx::Place{1, 1}, le::Fixup(le::InternalFixup{1, 0x10, 8}));
    hand.parts.fixups.insert_or_assign(vx::Place{1, 6}, le::Fixup(le::InternalFixup{1, 0x14, 7}));
    hand.parts.fixups.insert_or_assign(vx::Place{1, 0x14}, le::Fixup(le::InternalFixup{1, 0x10, 7}));
    hand.parts.labels[{1, 0x10}] = "sub_1_0010";
    hand.parts.labels[{1, 0x14}] = "loc_1_0014";
    return hand;
}

}  // namespace

CB_TEST("source.vxd.link.a_loadable_body_has_symbolic_relocations", [] {
    Hand hand = body_hand();
    const mx::Emission code = hand.emission();
    const std::vector<std::string> expected = {
        "        call   sub_1_0010",                // a self-relative rel32: a symbolic branch
        "        db  0A1h",                         // a load of an absolute address: the opcode, then the relocated field as `dd offset`
        "        dd  offset loc_1_0014",
        "        ret",
        "        db  000h, 000h, 000h, 000h, 000h",
        "sub_1_0010:",
        "        ret",
        "        db  000h, 000h, 000h",
        "loc_1_0014:",
        "        dd  offset sub_1_0010",            // a data pointer
    };
    CHECK_SAME(lk::link_body(code, 1), expected);
    // an extra label is defined where it falls, an unlabelled relocation stays raw
    const std::vector<std::string> with_extra = lk::link_body(code, 1, {{0x0A, "EXTRA"}});
    CB_CHECK(std::find(with_extra.begin(), with_extra.end(), "EXTRA label byte") != with_extra.end());
    hand.parts.labels.erase({1, 0x14});
    const mx::Emission lost = hand.emission();
    const std::vector<std::string> raw = lk::link_body(lost, 1);
    CB_CHECK_EQ(raw.at(1), "        db  0A1h, 000h, 010h, 000h, 000h");   // the target has no label: the whole instruction stays its bytes
});

CB_TEST("source.vxd.link.a_service_call_in_a_loadable_body_is_raw", [] {
    Hand hand;
    hand.parts.instructions.clear();
    hand.parts.labels.clear();
    vx::Instruction call = insn(6, "int", "20h", {0xCD, 0x20, 0x3F, 0, 1, 0});
    call.service = vx::ServiceCall{1, 0x3F, false};
    hand.parts.code = {Bytes{0xCD, 0x20, 0x3F, 0, 1, 0}};
    hand.parts.instructions[{1, 0}] = call;
    const mx::Emission code = hand.emission();
    CHECK_SAME(lk::link_body(code, 1), std::vector<std::string>{"        db  0CDh, 020h, 03Fh, 000h, 001h, 000h"});
});

CB_TEST("source.vxd.link.two_drivers_compared_object_by_object", [] {
    const le::Module original = splitter();
    const lk::CompareObjects same = lk::compare_objects(original, original);
    CB_CHECK_EQ(same.total, original.objects().size());
    CB_CHECK_EQ(same.exact_objects, same.total);
    CB_CHECK(!same.objects.empty());
    CB_CHECK_EQ(same.objects.front().equal, same.objects.front().length);
    CB_CHECK_EQ(same.objects.front().percent(), 100.0);
    // one byte of the first object's pages changed: that object is matched to its original and is not exact
    Bytes damaged = splitter_bytes();
    const std::uint64_t at = original.header().data_pages + 7;
    damaged[at] ^= 0xFF;
    const le::Module mine = le::Module::from_buffer(std::move(damaged));
    const lk::CompareObjects off = lk::compare_objects(original, mine);
    CB_CHECK_EQ(off.exact_objects, 0U);
    CB_CHECK_EQ(off.objects.front().oracle, 1U);
    CB_CHECK_EQ(off.objects.front().equal + 1, off.objects.front().length);
    CB_CHECK(off.objects.front().percent() < 100.0);
    CB_CHECK(off.objects.front().percent() > 99.0);
    // an object that no original object has the length of is not listed
    const lk::CompareObjects different = lk::compare_objects(le::Module::open(cb::testing::data_path("formats/testdata/srammtd.vxd")), original);
    CB_CHECK(different.objects.empty());
    CB_CHECK_EQ(different.total, original.objects().size());
});

CB_TEST("source.vxd.link.the_overlay_lays_the_assembled_code_over_the_recovered", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    mx::EmissionParts parts;
    {
        const vx::Disassembly found = vx::Disassembly::run(original, names);
        const mx::Emission whole(found, names);
        parts.fixups = whole.fixups();
        parts.instructions = whole.instructions();
        parts.labels = whole.labels();
        parts.jump_tables = whole.jump_tables();
        parts.code = {Bytes(whole.code(1).begin(), whole.code(1).end())};
        // no DDB: no macro-regenerated regions, so the assembled object is the whole object
    }
    const Bytes truth = parts.code[0];
    // ten bytes the assembly must put right, none of them in a fix-up field (the loader writes those, so they are not compared and not replaced)
    std::size_t damage = 200;
    for (bool clear = false; !clear; ++damage) {
        clear = true;
        for (const auto& [at, fixup] : parts.fixups) clear = clear && !(at.offset + 4 > static_cast<std::int64_t>(damage) && at.offset < static_cast<std::int64_t>(damage) + 10);
    }
    --damage;
    for (std::size_t k = 0; k < 10; ++k) parts.code[0][damage + k] ^= 0xAA;
    mx::Emission code(original, names, parts);
    const lk::Overlay result = lk::overlay_code(code, original, splitter_bytes());
    CB_CHECK_EQ(result.rows.size(), original.objects().size());
    const lk::OverlayRow& row = result.rows.front();
    CB_CHECK(row.ok);
    CB_CHECK_EQ(row.assembled_object.value_or(0), 1U);
    CB_CHECK_EQ(row.macro, 0U);
    CB_CHECK(row.score > 0.9);
    CB_CHECK(row.compared > 100);
    CB_CHECK(Bytes(code.code(1).begin(), code.code(1).end()) == truth);   // the damage is gone from the recovery
    // and the file written from it reads as the original
    const le::Module rebuilt = le::Module::from_buffer(Bytes(result.vxd));
    CB_CHECK(rebuilt.object_data(rebuilt.objects().front()) == original.object_data(original.objects().front()));
    CB_CHECK_EQ(rebuilt.resident_name(), original.resident_name());
});

CB_TEST("source.vxd.link.an_assembled_object_that_matches_nothing_is_refused", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    mx::Emission code(vx::Disassembly::run(original, names), names);
    const Bytes other = cb::fs::read_file(cb::testing::data_path("formats/testdata/srammtd.vxd"));
    try {
        (void)lk::overlay_code(code, original, other);
        CB_CHECK(false);   // it must not be accepted
    } catch (const cb::FormatError& e) {
        const std::string message = e.what();
        CB_CHECK(message.find("refusing to ship scrambled code") != std::string::npos);
        CB_CHECK(message.find("obj1(asm=1 score=") != std::string::npos);
        CB_CHECK(message.find("Assembled objects (index:length): 1:") != std::string::npos);
    }
});

CB_TEST("source.vxd.link.the_whole_driver_through_assembler_and_linker", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    Seen seen;
    const mx::Toolchain assembler = fake_assembler(seen);
    const lk::Environment env{names, assembler, fake_link(seen, splitter_bytes())};
    const lk::CompareBuild r = lk::build_and_compare(env, original, "SPLITTER");
    CB_CHECK(r.ok);
    CB_CHECK_EQ(r.original_length, splitter_bytes().size());
    CB_CHECK_EQ(r.vxd_length, splitter_bytes().size());
    CB_CHECK_EQ(r.report.exact_objects, r.report.total);   // the linker gave the original back
    CB_CHECK_EQ(r.link_log, "LINK : warning\n");
    // what the tools were given
    CB_CHECK_EQ(seen.sources.size(), 1U);
    CB_CHECK(seen.sources[0].starts_with(".386p\r\nSEG01 segment dword public use32 'LCODE01'\r\n"));
    CB_CHECK(seen.objects.front() == (Bytes{1, 2, 3}));
    CB_CHECK(seen.defs.front().starts_with("VXD SPLITTER\r\nDESCRIPTION 'SPLITTER'\r\n\r\nSEGMENTS\r\n"));
    CB_CHECK(seen.defs.front().ends_with("EXPORTS\r\n        VXD_DDB @1\r\n"));
    // the DDB is published where it is
    CB_CHECK(seen.sources[0].find("public VXD_DDB\r\nVXD_DDB label byte\r\n") != std::string::npos);
});

CB_TEST("source.vxd.link.each_stage_reports_where_it_stopped", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    {   // the assembler makes no object: the tail of its log is the report
        Seen seen;
        const mx::Toolchain assembler = fake_assembler(seen, false);
        const lk::Environment env{names, assembler, fake_link(seen, std::nullopt)};
        const lk::CompareBuild r = lk::build_and_compare(env, original, "SPLITTER");
        CB_CHECK(!r.ok);
        CB_CHECK_EQ(r.stage, "assemble");
        CB_CHECK_EQ(r.log.size(), 800U);
        CB_CHECK(r.log.ends_with("A2005: symbol redefinition\n"));
        CB_CHECK(seen.objects.empty());   // the linker was not asked
    }
    {   // the linker makes no file: the source and the .DEF are kept for the reader
        Seen seen;
        const mx::Toolchain assembler = fake_assembler(seen);
        const lk::Environment env{names, assembler, fake_link(seen, std::nullopt)};
        const lk::CompareBuild r = lk::build_and_compare(env, original, "SPLITTER");
        CB_CHECK(!r.ok);
        CB_CHECK_EQ(r.stage, "link");
        CB_CHECK_EQ(r.log, "LINK : warning\n");
        CB_CHECK(r.source.starts_with(".386p"));
        CB_CHECK(r.def.starts_with("VXD SPLITTER"));
    }
});

CB_TEST("source.vxd.link.a_functional_build_counts_the_relocations_that_survive", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    Seen seen;
    const mx::Toolchain assembler = fake_assembler(seen);
    const lk::Environment env{names, assembler, fake_link(seen, splitter_bytes())};
    const lk::FunctionalBuild r = lk::build_functional(env, original, "SPLITTER");
    CB_CHECK(r.ok);
    CB_CHECK(r.original_internal > 0);
    CB_CHECK_EQ(r.rebuilt_fixups, r.original_fixups);   // the linker's output is the original here
    CB_CHECK_EQ(r.rebuilt_internal, r.original_internal);
    CB_CHECK(seen.sources.front().find("dd  offset ") != std::string::npos);   // the source carries the relocations symbolically
});

CB_TEST("source.vxd.link.the_loadable_build_refuses_what_it_cannot_overlay", [] {
    const le::Module original = splitter();
    const vx::DdkNames names;
    Seen seen;
    const mx::Toolchain assembler = fake_assembler(seen);
    // a linker that gives another driver back: nothing it made lines up with the recovered code, and the overlay says so rather than ship scrambled code
    const Bytes other = cb::fs::read_file(cb::testing::data_path("formats/testdata/srammtd.vxd"));
    const lk::Environment env{names, assembler, fake_link(seen, other)};
    const lk::LoadableBuild r = lk::build_loadable_idiomatic(env, original);
    CB_CHECK(!r.ok);
    CB_CHECK_EQ(r.stage, "overlay");
    CB_CHECK(r.log.find("refusing to ship scrambled code") != std::string::npos);
    CB_CHECK(r.source.starts_with(".586p\r\n"));
    CB_CHECK(seen.defs.front().starts_with("VXD Splitter\r\nDESCRIPTION 'Splitter reconstructed from source'\r\n\r\nSEGMENTS\r\n"));
    CB_CHECK(seen.defs.front().find("    _LPTEXT     CLASS 'LCODE'") != std::string::npos);
    CB_CHECK(seen.defs.front().ends_with("EXPORTS\r\n        Splitter_DDB @1\r\n"));
    // the original given back, DDB and all: the macro-regenerated regions are in it, so its bytes line up with the recovered code only before the first
    // hole; how much the match scores decides the outcome, and whichever it is the report says so
    const lk::Environment same{names, assembler, fake_link(seen, splitter_bytes())};
    const lk::LoadableBuild again = lk::build_loadable_idiomatic(same, original);
    if (again.ok) {
        CB_CHECK_EQ(again.objects, original.objects().size());
        CB_CHECK_EQ(again.overlay.size(), original.objects().size());
        CB_CHECK(again.min_overlay_score >= 0.5);
        CB_CHECK_EQ(again.rebuilt_internal, again.original_internal);
    } else {
        CB_CHECK_EQ(again.stage, "overlay");
    }
    // a name given is the one used
    seen.defs.clear();
    (void)lk::build_loadable_idiomatic(env, original, "OTHER");
    CB_CHECK(seen.defs.front().starts_with("VXD OTHER\r\n"));
    // an assembler that fails, and a linker that does
    Seen failed;
    const mx::Toolchain broken = fake_assembler(failed, false);
    CB_CHECK_EQ(lk::build_loadable_idiomatic(lk::Environment{names, broken, fake_link(failed, std::nullopt)}, original).stage, "assemble");
    CB_CHECK_EQ(lk::build_loadable_idiomatic(lk::Environment{names, assembler, fake_link(failed, std::nullopt)}, original).stage, "link");
});

CB_TEST("source.vxd.link.the_linkers_run_where_they_are_told", [] {
    CB_WINDOWS_ONLY("its linker stand-ins are .cmd files");
    cb::fs::ScopedTempDir dir("cblink");
    const std::filesystem::path script = dir.path() / "linker.cmd";
    cb::fs::write_text(script, "@copy /y m.obj m.vxd >nul\r\n@echo linked with %*\r\n");
    const std::filesystem::path work = dir.path() / "work";
    cb::fs::make_dirs(work);
    const Bytes object = {9, 8, 7};
    const lk::Linked made = lk::standard_link(script)(object, "VXD X\r\n", work);
    CB_CHECK(made.vxd.has_value());
    CB_CHECK(made.vxd.value_or(Bytes()) == object);
    CB_CHECK(made.log.find("linked with /VXD /NOD /OUT:m.vxd /DEF:m.def m.obj") != std::string::npos);
    CB_CHECK_EQ(cb::fs::read_text(work / "m.def"), "VXD X\r\n");
    // no such program: a log, no file
    const lk::Linked missing = lk::standard_link(dir.path() / "absent.exe")(object, "", work / "..");
    CB_CHECK(!missing.vxd.has_value());
    CB_CHECK(missing.log.starts_with("link exec failed"));
    // LINK 2.60 and its DLLs are staged once in a folder without spaces; this stand-in folder has no real LINK, so the run itself fails
    const std::filesystem::path msvc20 = dir.path() / "msvc20";
    cb::fs::make_dirs(msvc20);
    for (const char* file : {"LINK.EXE", "DBI.DLL", "LINK.ERR", "IMAGEHLP.DLL"}) cb::fs::write_text(msvc20 / file, "x");
    const std::filesystem::path stage = dir.path() / "stage";
    const lk::Linked staged = lk::link_260(msvc20, stage, 0x200)(object, "", work);
    CB_CHECK(!staged.vxd.has_value());
    CB_CHECK(std::filesystem::exists(stage / "DBI.DLL") && std::filesystem::exists(stage / "IMAGEHLP.DLL") && std::filesystem::exists(stage / "LINK.ERR"));
});

CB_TEST("source.vxd.link.the_command_wants_a_file", [] {
    const vx::DdkNames names;
    Seen seen;
    const mx::Toolchain assembler = fake_assembler(seen);
    const lk::Environment env{names, assembler, fake_link(seen, std::nullopt)};
    cb::testing::CapturedStd captured;
    CB_CHECK_EQ(lk::main(env, std::vector<std::string>{}), cb::kExitUsage);
    // a driver whose tools make nothing: the failure is reported and the exit code says so
    const std::string path = cb::fs::to_text(cb::testing::data_path("formats/testdata/splitter.vxd"));
    const mx::Toolchain failing = fake_assembler(seen, false);
    const lk::Environment broken{names, failing, fake_link(seen, std::nullopt)};
    CB_CHECK_EQ(lk::main(broken, std::vector<std::string>{path, "SPLITTER"}), cb::kExitFailed);
    CB_CHECK(captured.out().find("FAILED at assemble:") != std::string::npos);
});

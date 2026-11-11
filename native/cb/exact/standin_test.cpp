#include "cb/exact/standin.hpp"

#include <unordered_set>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

using namespace cb;
using namespace cb::exact;

// (CB_CHECK_EQ prints its operands, and MSVC formats a vector as a range: for buffers and lists compare with ==)
#define CB_CHECK_SAME(a, b) CB_CHECK((a) == (b))

// The fixtures in testdata/standin were assembled by hand (the generator is described in each test) and the three
// rebuilt objects are what the Python module this one replaced made of them; each was checked by reading the result
// back: the code, the lengths of the functions, the fix-ups the linker is left with, and the external names.
//
//   base.obj   MODA: _f1 at 0 (push bp; mov bp,sp; call _ext; pop bp; ret) with a self-relative fix-up to _ext,
//              _f2 at 8 (mov ax, offset _DATA:_var; a call to _f1 the compiler filled in; ret), _f3 at 15 (ret)
//   alt.obj    the same file from another toolchain: _f2 is the program's (a far call to _far_fn, DGROUP's segment)
//   program    _f1 at 0x100, _f2 at 0x108 (9 bytes), _f3 at 0x111; load segment 0x10, DGROUP at image offset 0x1A0;
//              the loader adds the load segment at 0x10B and 0x10E

namespace {

struct Fixture {
    Bytes base = fs::read_file(testing::data_path("exact/testdata/standin/base.obj"));
    Bytes alt = fs::read_file(testing::data_path("exact/testdata/standin/alt.obj"));
    Bytes image = crypto::from_hex(text::trim(fs::read_text(testing::data_path("exact/testdata/standin/program.hex"))));
    SymbolTable symbols = {{"_ext", {SymKind::Code, 0x180}}, {"_far_fn", {SymKind::Code, 0x150}}, {"_f1", {SymKind::Code, 0x100}}};
    std::unordered_set<std::uint32_t> relocs = {0x10B, 0x10E};
    std::vector<std::uint32_t> relocations = {0x10B, 0x10E};
    std::vector<std::uint32_t> addrs = {0x100, 0x108, 0x111};

    standin::Program program() const {
        compare::Program c;
        c.dgroup = 0x1A0;
        c.loadseg = 0x10;
        c.code_base = [](std::uint32_t) { return 0u; };
        c.relocs = &relocs;
        return {image, symbols, c, relocations};
    }
};

Bytes golden(const char* name) { return fs::read_file(testing::data_path(std::string("exact/testdata/standin/") + name)); }

std::vector<std::string> names(const std::vector<formats::omf::FunctionSpan>& fns) {
    std::vector<std::string> out;
    for (const auto& f : fns) out.push_back(f.name);
    return out;
}

}  // namespace

CB_TEST("exact.standin.a_function_that_is_not_the_programs_becomes_its_bytes", [] {
    Fixture fx;
    const auto got = standin::rebuild(fx.program(), fx.base, fx.addrs, 0x112, "MODX", {});
    CB_CHECK_SAME(got.stand_ins, (std::vector<std::string>{"_f2"}));
    CB_CHECK_SAME(got.object, golden("standins.obj"));

    const formats::omf::Object o = formats::omf::read(got.object);
    // the code: _f1 as compiled (its word for the linker is still 0), _f2 the program's bytes with its two segment
    // words zeroed (the linker fills them), then _f3
    const Bytes* text = o.segments.find("_TEXT");
    CB_CHECK(text != nullptr);
    CB_CHECK_EQ(crypto::to_hex(*text), "558bece800005dc39a00000000b80000c3c3");
    const auto fns = formats::omf::functions(o, "_TEXT");
    CB_CHECK_SAME(names(fns), (std::vector<std::string>{"_f1", "_f2", "_f3"}));
    CB_CHECK_EQ(fns[1].offset, 8);
    CB_CHECK_EQ(fns[1].length, 9);
    CB_CHECK_EQ(fns[2].offset, 17);   // _f3 moved behind the longer _f2
    // the fix-ups the linker is left with: _f1's call, then the stand-in's far call's segment (an external it
    // learned from SYMBOLS by the place) and DGROUP's segment
    CB_CHECK_EQ(o.fixups.size(), 3u);
    CB_CHECK(o.fixups[0].kind == formats::omf::FixupKind::Relative && o.fixups[0].at == 4);
    CB_CHECK(o.fixups[1].kind == formats::omf::FixupKind::Base && o.fixups[1].at == 11);
    CB_CHECK(o.fixups[1].target.kind == formats::omf::Target::Kind::External && *o.fixups[1].target.name == "_far_fn");
    CB_CHECK(o.fixups[2].kind == formats::omf::FixupKind::Base && o.fixups[2].at == 14);
    CB_CHECK(o.fixups[2].target.kind == formats::omf::Target::Kind::Group);
    CB_CHECK_SAME(o.externals, (std::vector<std::string>{"_ext", "_far_fn"}));
});

CB_TEST("exact.standin.another_toolchains_function_is_taken_before_the_programs_bytes", [] {
    Fixture fx;
    const std::vector<Bytes> alts = {fx.alt};
    const auto got = standin::rebuild(fx.program(), fx.base, fx.addrs, 0x112, "MODX", alts);
    CB_CHECK(got.stand_ins.empty());
    CB_CHECK_SAME(got.object, golden("from_alt.obj"));
    const formats::omf::Object o = formats::omf::read(got.object);
    // _f2 came from alt.obj with its names renumbered: its far call's external is _far_fn, number 2 here (1 there)
    CB_CHECK_EQ(o.fixups.size(), 3u);
    CB_CHECK(o.fixups[1].kind == formats::omf::FixupKind::Pointer && o.fixups[1].at == 9);
    CB_CHECK(*o.fixups[1].target.name == "_far_fn");
    CB_CHECK_SAME(o.externals, (std::vector<std::string>{"_ext", "_far_fn"}));
});

CB_TEST("exact.standin.the_last_function_may_be_followed_by_zeros_the_module_does_not_own", [] {
    Fixture fx;
    // the code ends one byte later than the last function does, and that byte is a zero: not the module's
    const auto got = standin::rebuild(fx.program(), fx.base, fx.addrs, 0x113, "MODX", {});
    CB_CHECK_SAME(got.stand_ins, (std::vector<std::string>{"_f2"}));
    CB_CHECK_SAME(got.object, golden("tail.obj"));
});

CB_TEST("exact.standin.no_title_keeps_the_modules_names", [] {
    Fixture fx;
    const auto got = standin::rebuild(fx.program(), fx.base, fx.addrs, 0x112, std::nullopt, {});
    // the same module but for its THEADR, which is the compiler's ("MODA", not "MODX")
    const auto records = formats::omf::records(got.object);
    CB_CHECK_EQ(records.at(0).type, 0x80);
    CB_CHECK_EQ(std::string(as_text(records.at(0).payload.subspan(1))), "MODA");
    const auto renamed = formats::omf::records(golden("standins.obj"));
    CB_CHECK_EQ(std::string(as_text(renamed.at(0).payload.subspan(1))), "MODX");
});

CB_TEST("exact.standin.the_input_must_agree_with_itself", [] {
    Fixture fx;
    // three functions compiled, two places
    CB_CHECK_THROWS_AS(FormatError, (void)standin::rebuild(fx.program(), fx.base, std::vector<std::uint32_t>{0x100, 0x108}, 0x112, "MODX", {}));
    // a stand-in's far call to a place SYMBOLS has no name for
    Fixture nameless;
    nameless.symbols.erase("_far_fn");
    CB_CHECK_THROWS_AS(FormatError, (void)standin::rebuild(nameless.program(), nameless.base, nameless.addrs, 0x112, "MODX", {}));
    // a module that is not one
    CB_CHECK_THROWS_AS(Error, (void)standin::rebuild(fx.program(), Bytes{0x80, 0x01}, fx.addrs, 0x112, "MODX", {}));
});

CB_TEST("exact.standin.fixup_records_round_trip", [] {
    // a self-relative fix-up to external 1 (frame: the target's), a segment-relative offset to segment 2 with
    // displacement 6 (frame: group 1), and a pointer fix-up to external 300 (a two-byte index)
    std::vector<standin::Fixup> fixes(3);
    fixes[0] = {.segment_relative = false, .location = 1, .offset = 0, .frame_method = 5, .frame_datum = std::nullopt,
                .target_method = 2, .target_datum = 1, .no_displacement = true, .displacement = 0, .segment = 1, .at = 0x104};
    fixes[1] = {.segment_relative = true, .location = 1, .offset = 0, .frame_method = 1, .frame_datum = std::uint16_t{1},
                .target_method = 0, .target_datum = 2, .no_displacement = false, .displacement = 6, .segment = 1, .at = 0x109};
    fixes[2] = {.segment_relative = true, .location = 3, .offset = 0, .frame_method = 5, .frame_datum = std::nullopt,
                .target_method = 2, .target_datum = 300, .no_displacement = true, .displacement = 0, .segment = 1, .at = 0x200};
    const Bytes record = standin::write_fixup_record(fixes, 0x100);
    // type, length, payload, checksum
    CB_CHECK_EQ(record.at(0), 0x9C);
    const auto records = formats::omf::records(record);
    CB_CHECK_EQ(records.size(), 1u);
    standin::FixupThreads threads;
    // the offsets are stored relative to the record's base, so reading them back gives offsets from it
    const auto back = standin::read_fixups(records[0].payload, threads);
    CB_CHECK_EQ(back.size(), 3u);
    CB_CHECK_EQ(back[0].offset, 4);
    CB_CHECK_EQ(back[1].offset, 9);
    CB_CHECK_EQ(back[2].offset, 0x100);
    CB_CHECK(back[1].segment_relative && back[1].location == 1 && back[1].frame_method == 1 && *back[1].frame_datum == 1);
    CB_CHECK_EQ(back[1].target_datum, 2);
    CB_CHECK_EQ(back[1].displacement, 6);
    CB_CHECK(back[0].no_displacement && !back[0].segment_relative);
    CB_CHECK_EQ(back[2].target_datum, 300);   // the two-byte index
    // a fix-up farther than 1024 bytes from its record cannot be written
    CB_CHECK_THROWS_AS(FormatError, (void)standin::write_fixup_record(fixes, 0xF00));
    CB_CHECK_THROWS_AS(FormatError, (void)standin::write_fixup_record(fixes, 0x105));   // before the record's base
});

CB_TEST("exact.standin.threads_resolve_fixups", [] {
    // THREAD: target thread 0 = external (method 2) datum 3; frame thread 1 = group (method 1) datum 1. Then a fix-up
    // that uses both (F bit set, frame thread 1; T bit set, target thread 0), P=1.
    const Bytes record = {
        0x08, 0x03,                  // 0 D=0 0 method=2 thread=0 : target thread 0, external, index 3
        0x45, 0x01,                  // 0 D=1 0 method=1 thread=1 : frame thread 1, group, index 1
        0x84, 0x10,                  // M=0 loc=1 offset=0x010
        0x80 | (1 << 4) | 8 | 4 | 0, // F=1 frame thread 1; T=1 target thread 0; P=1
    };
    standin::FixupThreads threads;
    const auto fixes = standin::read_fixups(record, threads);
    CB_CHECK_EQ(fixes.size(), 1u);
    CB_CHECK_EQ(fixes[0].offset, 0x10);
    CB_CHECK_EQ(fixes[0].location, 1);
    CB_CHECK_EQ(fixes[0].frame_method, 1);
    CB_CHECK_EQ(*fixes[0].frame_datum, 1);
    CB_CHECK_EQ(fixes[0].target_method, 2);
    CB_CHECK_EQ(fixes[0].target_datum, 3);
    CB_CHECK(fixes[0].no_displacement);
    CB_CHECK(threads.target(0).has_value() && threads.frame(1).has_value() && !threads.frame(0).has_value());
    // a thread nothing defined, and a record cut short
    standin::FixupThreads empty;
    CB_CHECK_THROWS_AS(FormatError, (void)standin::read_fixups(Bytes{0x84, 0x10, 0x80 | 8 | 4}, empty));
    CB_CHECK_THROWS_AS(TruncatedError, (void)standin::read_fixups(Bytes{0x84}, empty));
    CB_CHECK(standin::read_fixups({}, empty).empty());
});

CB_TEST("exact.standin.indexes", [] {
    Bytes b;
    standin::append_index(b, 5);
    standin::append_index(b, 0x7F);
    standin::append_index(b, 0x80);
    standin::append_index(b, 0x1234);
    CB_CHECK_EQ(crypto::to_hex(b), "057f80809234");
    CB_CHECK(standin::read_index(b, 0) == std::make_pair(std::uint16_t{5}, std::size_t{1}));
    CB_CHECK(standin::read_index(b, 2) == std::make_pair(std::uint16_t{0x80}, std::size_t{4}));
    CB_CHECK(standin::read_index(b, 4) == std::make_pair(std::uint16_t{0x1234}, std::size_t{6}));
    CB_CHECK_THROWS_AS(TruncatedError, (void)standin::read_index(Bytes{0x81}, 0));
    Bytes too_big;
    CB_CHECK_THROWS_AS(FormatError, standin::append_index(too_big, 0x8000));
});

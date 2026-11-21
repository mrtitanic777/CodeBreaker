// Tests of formats/format_engine.hpp: the descriptions in config/formats (mz, pe, ne, ines) on small in-memory images,
// the language itself on an inline description, and the engine's tree against the hand-written readers on the real
// files that are here (src/formats/testdata, D:/test-binaries, a homebrew NES image) -- they must agree field by field and
// write back the same bytes.
#include "formats/format_engine.hpp"

#include <filesystem>
#include <format>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "formats/format_compare.hpp"

using namespace cb;
namespace eng = cb::formats::engine;

namespace {

struct W {
    Bytes b;
    W& u8(std::uint32_t v) { return b.push_back(static_cast<std::uint8_t>(v)), *this; }
    W& u16(std::uint32_t v) { return u8(v & 0xFF).u8((v >> 8) & 0xFF); }
    W& u32(std::uint32_t v) { return u16(v & 0xFFFF).u16(v >> 16); }
    W& u64(std::uint64_t v) { return u32(static_cast<std::uint32_t>(v)).u32(static_cast<std::uint32_t>(v >> 32)); }
    W& str(std::string_view s) { return append(b, as_bytes(s)), *this; }
    W& zeros(std::size_t n) { return b.insert(b.end(), n, 0), *this; }
    W& pad_to(std::size_t n) { return b.resize(std::max(b.size(), n), 0), *this; }
};

eng::Registry registry() {
    eng::Registry r;
    r.add_directory(testing::data_path("../config/formats"));
    return r;
}

// An MZ header (28 bytes), padded to 0x40 with e_lfanew at 0x3C.
W dos_stub(std::uint32_t lfanew, std::uint32_t reloc_count = 0) {
    W w;
    w.str("MZ").u16(0x90).u16(3).u16(reloc_count).u16(4).u16(0).u16(0xFFFF).u16(0).u16(0xB8).u16(0).u16(0).u16(0).u16(0x40).u16(0);
    w.pad_to(0x3C).u32(lfanew);
    return w;
}

Bytes pe_image(bool plus) {
    W w = dos_stub(0x40);
    w.pad_to(0x40).str(std::string_view("PE\0\0", 4));
    const std::uint32_t opt_size = plus ? 240 : 224;
    w.u16(plus ? 0x8664 : 0x14C).u16(1).u32(0x12345678).u32(0).u32(0).u16(opt_size).u16(plus ? 0x22 : 0x2102);
    w.u16(plus ? 0x20B : 0x10B).u8(5).u8(2).u32(0x200).u32(0x400).u32(0).u32(0x1000).u32(0x1000);
    if (plus) w.u64(0x140000000ull);
    else w.u32(0x2000).u32(0x400000);
    w.u32(0x1000).u32(0x200).u16(4).u16(0).u16(1).u16(2).u16(4).u16(0).u32(0).u32(0x3000).u32(0x200).u32(0x99).u16(3).u16(0x8140);
    if (plus) w.u64(0x100000).u64(0x1000).u64(0x100000).u64(0x1000);
    else w.u32(0x100000).u32(0x1000).u32(0x100000).u32(0x1000);
    w.u32(0).u32(16);
    for (std::uint32_t i = 0; i < 16; ++i) w.u32(i == 1 ? 0x2000 : 0).u32(i == 1 ? 0x28 : 0);
    w.str(".text").zeros(3).u32(0x200).u32(0x1000).u32(0x200).u32(0x200).u32(0).u32(0).u16(0).u16(0).u32(0x60000020);
    w.pad_to(0x200);
    for (int i = 0; i < 0x200; ++i) w.u8(0x90);
    return w.b;
}

Bytes ne_image() {
    W w = dos_stub(0x40);
    w.pad_to(0x40).str("NE").u8(5).u8(10);
    w.u16(0x50).u16(19);                  // entry table (from the NE header): offset, length
    w.u32(0xDEADBEEF).u16(0x8301).u16(2).u16(0x400).u16(0x1000);   // crc, flags, autodata, heap, stack
    w.u32(0x00010005).u32(0x00020000);   // cs:ip, ss:sp
    w.u16(2).u16(0).u16(0);              // segments, module refs, nonresident size
    w.u16(0x40).u16(0x50).u16(0x50).u16(0x50).u16(0x50);   // segment table, resource, resident names, module refs, imported names
    w.u32(0).u16(1).u16(4).u16(0).u8(2).u8(0);   // nonresident offset, movable entries, shift 4, resource segments, OS 2, flags
    w.u16(0).u16(0).u16(0).u16(0x030A);   // gangload, swap, expected Windows version
    // segment table at 0x40 + 0x40 = 0x80: (sector, length, flags, min_alloc)
    w.u16(0x10).u16(0x20).u16(0x1D50).u16(0x20);
    w.u16(0).u16(0).u16(1).u16(0);       // no data, length 0, data segment, min_alloc 0 (= 64 KB)
    // entry table at 0x40 + 0x50 = 0x90
    w.u8(2).u8(1).u8(1).u16(0x10).u8(1).u16(0x20);               // two fixed entries in segment 1
    w.u8(1).u8(0xFF).u8(1).u16(0x3FCD).u8(2).u16(0x30);          // one moveable entry in segment 2
    w.u8(3).u8(0);                                                // three unused ordinals
    w.u8(0);                                                      // the end
    w.zeros(0x40);
    return w.b;
}

Bytes ines_image(unsigned prg_units, unsigned chr_units, std::uint8_t f6, std::uint8_t f7, bool trainer = false, std::uint8_t b8 = 0, std::uint8_t b9 = 0) {
    Bytes out = {'N', 'E', 'S', 0x1A, static_cast<std::uint8_t>(prg_units), static_cast<std::uint8_t>(chr_units), f6, f7, b8, b9, 0, 0, 0, 0, 0, 0};
    if (trainer) out.insert(out.end(), 512, 0xEE);
    for (unsigned b = 0; b < prg_units; ++b) out.insert(out.end(), 0x4000, static_cast<std::uint8_t>(0x10 + b));
    out.insert(out.end(), chr_units * 0x2000u, 0xCC);
    return out;
}

std::string describe(const eng::Comparison& c) {
    if (c.agree()) return {};
    std::string out = std::format("{}: reader {} engine {} ({})", c.format, c.reader_accepts, c.engine_accepts, c.engine_error);
    for (const auto& m : c.mismatches) out += std::format("\n  {} engine={} reader={}", m.field, m.engine, m.reader);
    return out;
}

// The agreement of one image with its format's hand-written reader, and the round trips.
void expect_agreement(const eng::Registry& reg, std::string_view format, const Bytes& image) {
    const eng::Comparison c = eng::compare(reg, format, image);
    CB_CHECK_EQ(describe(c), "");
    CB_CHECK(c.reader_accepts);
    CB_CHECK(c.round_trip);
    CB_CHECK(c.covered_round_trip);
    CB_CHECK(c.compared > 0);
}

}  // namespace

CB_TEST("formats.format_engine.descriptions_load", [] {
    const eng::Registry reg = registry();
    for (const std::string& name : eng::compared_formats()) CB_CHECK(reg.has(name));
});

CB_TEST("formats.format_engine.language", [] {
    eng::Registry reg;
    reg.add_text(R"(
        format demo
        endian be
        struct File {
            magic  : str(4) == "DEMO" | "DEMX"
            count  : u16
            items  : Item[count]
            tail   : u8[until _ == 0]
            mode   : u8 as Mode
            if mode == 1 { wide : u32 } else { narrow : u16 }
            blob @ 0 : bytes(4)
            after  : u16le
            win    : Win size(6)
            signed_byte : i8
            let doubled = count * 2
            let packed  = (count << 4) | (mode ? 1 : 0)
            assert doubled < 100
        }
        struct Item { id : u8 as Kind  size : u16 }
        struct Win  { a : u16  rest : bytes($end - $pos) }
        enum Kind  { plain = 1, fancy = 2 }
        enum Mode  { one = 1, two = 2 }
    )");
    W w;
    w.str("DEMO").u8(0).u8(2);                           // count 2 (big-endian)
    w.u8(1).u8(0x12).u8(0x34).u8(2).u8(0).u8(7);         // items
    w.u8(5).u8(6).u8(0);                                 // tail until a zero
    w.u8(1).u8(0xAA).u8(0xBB).u8(0xCC).u8(0xDD);         // mode 1, then wide
    w.u8(0x01).u8(0x02);                                 // after: little-endian
    w.u8(0x00).u8(0x09).u8(1).u8(2).u8(3).u8(4);         // win: a, then 4 remaining bytes of its 6
    w.u8(0xFE).u8(0x77);                                 // signed byte, a byte after the structure
    const eng::Tree t = reg.read("demo", w.b);
    CB_CHECK_EQ(t.root.u("count"), 2u);
    CB_CHECK_EQ(t.root.u("items[0].size"), 0x1234u);
    CB_CHECK_EQ(t.root.get("items[1].id").label, "fancy");
    CB_CHECK_EQ(t.root.get("items").children.size(), std::size_t{2});
    CB_CHECK_EQ(t.root.get("tail").children.size(), std::size_t{3});   // 5, 6, 0
    CB_CHECK_EQ(t.root.get("mode").label, "one");
    CB_CHECK_EQ(t.root.u("wide"), 0xAABBCCDDu);
    CB_CHECK(t.root.find("narrow") == nullptr);
    CB_CHECK_EQ(t.root.u("after"), 0x0201u);
    CB_CHECK_EQ(t.root.get("blob").offset, 0u);   // a pointer: read from 0 without moving the cursor
    CB_CHECK_EQ(t.root.get("after").offset, t.root.get("wide").offset + 4);
    CB_CHECK_EQ(t.root.u("win.a"), 9u);
    CB_CHECK_EQ(t.root.get("win.rest").size, 4u);
    CB_CHECK_EQ(t.root.i("signed_byte"), -2);
    CB_CHECK_EQ(t.root.i("doubled"), 4);
    CB_CHECK_EQ(t.root.i("packed"), 0x21);
    CB_CHECK(eng::write(t) == w.b);

    // A modified tree writes the new value; the rest stays as read.
    eng::Tree edited = reg.read("demo", w.b);
    edited.root.get("count").value = 2;
    edited.root.get("wide").value = 0x01020304;
    const Bytes out = eng::write(edited);
    CB_CHECK_EQ(out.size(), w.b.size());
    CB_CHECK(reg.read("demo", out).root.u("wide") == 0x01020304u);
    eng::set_text(edited.root.get("magic"), "DEMX");
    CB_CHECK(reg.read("demo", eng::write(edited)).root.str("magic") == "DEMX");
    CB_CHECK_THROWS_AS(Error, eng::set_text(edited.root.get("magic"), "TOOLONG"));

    // The magic, the assertion and a cut-short file are errors; the dump names the fields.
    Bytes bad = w.b;
    bad[0] = 'X';
    CB_CHECK(!reg.try_read("demo", bad).has_value());
    CB_CHECK_THROWS_AS(Error, reg.read("demo", bad));
    Bytes big_count = w.b;
    big_count[5] = 0x7F;   // count 0x7F: doubled > 100 and the items run out first
    CB_CHECK_THROWS_AS(Error, reg.read("demo", big_count));
    CB_CHECK_THROWS_AS(Error, reg.read("demo", ByteView(w.b).first(10)));
    const std::string text = eng::dump(t);
    CB_CHECK(text.find("items") != std::string::npos);
    CB_CHECK(text.find("[fancy]") != std::string::npos);
    CB_CHECK_EQ(reg.identify(w.b).size(), std::size_t{1});
});

// The language of the tables behind pointers: functions, xlat (a section table), scan, cstr, `[while ...]`, `try`, `part`,
// variables, a root at an offset, and 3-byte integers.
CB_TEST("formats.format_engine.tables", [] {
    eng::Registry reg;
    reg.add_text(R"(
        format tab
        endian le
        struct File {
            n       : u8
            maps    : Map[n]
            at_rva  : u16
            names @ off(at_rva)       : Name[3] try part names
            late  @ off(at_rva + 100) : u16 try
            missing : u16 try
            pad     : u8 try
            var     : u8 if $mode == 7
            let where   = scan(1, "XY")
            let nowhere = scan(0, "QQ")
            list @ 20 : Elt[while $pos < 22 until _.v == 0] try part list
            let tail = $mode + 1
        }
        struct Map { va : u16  size : u16  raw : u16  let end = va + size }
        struct Name { text : cstr(4) }
        struct Elt { v : u8 }
        struct Page { number : u24be  kind : u8 }
        struct Root { first : Page  second : Page }
        fn off(rva) = xlat(maps, rva, va, size, raw)
    )");
    W w;
    w.u8(2).u16(0x1000).u16(0x100).u16(0x40).u16(0x2000).u16(0x80).u16(0x60);   // two sections: 0x1000+0x100 -> 0x40, 0x2000+0x80 -> 0x60
    w.u16(0x2004).u16(0xBEEF).u8(0x55);                                         // at_rva: 4 bytes into the second section; missing; pad
    w.pad_to(20).u8(5).u8(6).u8(7).u8(0);                                       // the list at 20
    w.pad_to(0x40).str("ab").u8(0).str("cdefg").u8(0).str("h");                 // the first section's start
    w.pad_to(0x64).str("Z").u8(0).str("hi").u8(0).u8(0).str("XY").pad_to(0x70);
    const eng::Tree whole = reg.read("tab", w.b);
    CB_CHECK_EQ(whole.root.get("maps").children.size(), std::size_t{2});
    CB_CHECK_EQ(whole.root.u("maps[1].end"), 0x2080u);
    CB_CHECK_EQ(whole.root.u("missing"), 0xBEEFu);
    CB_CHECK_EQ(whole.root.u("pad"), 0x55u);
    CB_CHECK(whole.root.find("names") == nullptr);                              // a part not asked for
    CB_CHECK(whole.root.find("list") == nullptr);
    CB_CHECK_EQ(whole.root.u("late"), 0u);                                      // outside every section: a number that cannot be read is 0 ...
    CB_CHECK(whole.root.get("late").kind == eng::Kind::Let);                    // ... with no bytes behind it
    CB_CHECK_EQ(whole.root.i("nowhere"), -1);
    CB_CHECK_EQ(whole.root.i("where"), 0x6A);
    CB_CHECK(whole.root.find("var") == nullptr);
    CB_CHECK_EQ(whole.root.i("tail"), 1);

    eng::ReadOptions with;
    with.parts.insert("names");
    with.parts.insert("list");
    const eng::Tree t = reg.read("tab", w.b, with);
    const eng::Node& names = t.root.get("names");
    CB_CHECK_EQ(names.offset, 0x64u);                                           // 0x2004 -> 0x60 + 4
    CB_CHECK_EQ(names.children.size(), std::size_t{3});
    CB_CHECK_EQ(t.root.str("names[0].text"), "Z");                              // a cstr: the text up to its terminator ...
    CB_CHECK_EQ(t.root.get("names[0].text").size, 2u);                          // ... and the node covers the terminator too
    CB_CHECK_EQ(t.root.str("names[1].text"), "hi");
    CB_CHECK_EQ(t.root.get("names[2].text").size, 1u);                          // an empty string: just the terminator
    CB_CHECK_EQ(t.root.get("list").children.size(), std::size_t{2});            // 5, 6: the cursor reached the limit before the 0
    CB_CHECK(eng::write(t) == w.b);
    CB_CHECK_EQ(reg.parts("tab").size(), std::size_t{2});                       // list, names

    // `try`: a number past the end of the data is 0, a field is not there, an array keeps what was read.
    const Bytes cut(w.b.begin(), w.b.begin() + 0x10);                           // `missing` has one of its two bytes
    CB_CHECK_EQ(reg.read("tab", cut).root.u("missing"), 0u);
    CB_CHECK(reg.read("tab", cut).root.get("missing").kind == eng::Kind::Let);
    eng::ReadOptions all;
    all.all_parts = true;
    const Bytes early(w.b.begin(), w.b.begin() + 0x42);
    CB_CHECK_EQ(reg.read("tab", early, all).root.get("names").children.size(), std::size_t{0});   // the pointer lies beyond the data

    // variables, 3-byte integers and a root struct at an offset.
    eng::ReadOptions vars;
    vars.vars["mode"] = 7;
    const eng::Tree v = reg.read("tab", w.b, vars);
    CB_CHECK_EQ(v.root.i("tail"), 8);
    CB_CHECK_EQ(v.root.u("var"), 0u);                                           // read: the byte after `pad`
    const Bytes pages{0xAA, 0x01, 0x02, 0x03, 0x07, 0x00, 0x00, 0x09, 0x02};
    eng::ReadOptions at;
    at.root = "Root";
    at.at = 1;
    const eng::Tree r = reg.read("tab", pages, at);
    CB_CHECK_EQ(r.root.u("first.number"), 0x010203u);                           // big-endian, 3 bytes
    CB_CHECK_EQ(r.root.u("first.kind"), 7u);
    CB_CHECK_EQ(r.root.u("second.number"), 9u);
    CB_CHECK_EQ(r.root.get("second.number").offset, 5u);
    at.at = 100;
    CB_CHECK_THROWS_AS(Error, reg.read("tab", pages, at));
    // a tree read with keep_source = false is for reading only
    eng::ReadOptions lean;
    lean.keep_source = false;
    CB_CHECK(reg.read("tab", w.b, lean).source.empty());
});

CB_TEST("formats.format_engine.description_errors", [] {
    eng::Registry reg;
    CB_CHECK_THROWS_AS(ParseError, reg.add_text("struct File { a : u8 }"));                  // no `format` line
    CB_CHECK_THROWS_AS(ParseError, reg.add_text("format x\nstruct File { a : u8 "));         // block not closed
    CB_CHECK_THROWS_AS(ParseError, reg.add_text("format x\nstruct File { a : u8 if }"));     // no condition
    CB_CHECK_THROWS_AS(ParseError, reg.add_text("format x\nstruct File { a : u8 == \"q }")); // string not closed
    reg.add_text("format y\nstruct File { a : Nowhere }");
    CB_CHECK_THROWS_AS(FormatError, reg.read("y", Bytes{1, 2, 3}));                           // unknown struct
    CB_CHECK_THROWS_AS(NotFoundError, reg.read("nothing", Bytes{1}));
});

CB_TEST("formats.format_engine.mz", [] {
    const eng::Registry reg = registry();
    W w = dos_stub(0, 2);   // two relocations at 0x40
    w.pad_to(0x40).u16(0x10).u16(0x0).u16(0x22).u16(0x1);
    w.pad_to(0x200);
    const eng::Tree t = reg.read("mz", w.b);
    CB_CHECK_EQ(t.root.u("header.reloc_count"), 2u);
    CB_CHECK_EQ(t.root.u("header.header_size"), 0x40u);
    CB_CHECK_EQ(t.root.u("header.relocs[1].segment"), 1u);
    CB_CHECK_EQ(t.root.u("header.relocs[1].offset"), 0x22u);
    CB_CHECK_EQ(t.root.i("header.image_size"), 3 * 512 - (512 - 0x90));
    expect_agreement(reg, "mz", w.b);
    CB_CHECK(!reg.try_read("mz", Bytes{'N', 'E', 'S'}).has_value());
});

CB_TEST("formats.format_engine.pe", [] {
    const eng::Registry reg = registry();
    for (bool plus : {false, true}) {
        const Bytes image = pe_image(plus);
        const eng::Tree t = reg.read("pe", image);
        CB_CHECK_EQ(t.root.u("nt.coff.machine"), plus ? 0x8664u : 0x14Cu);
        CB_CHECK_EQ(t.root.get("nt.coff.machine").label, plus ? "x64" : "i386");
        CB_CHECK_EQ(t.root.get("nt.coff.characteristics").label, plus ? "executable|large_address_aware" : "executable|machine_32bit|dll");
        CB_CHECK_EQ(t.root.u("nt.optional.image_base"), plus ? 0x140000000ull : 0x400000ull);
        CB_CHECK_EQ(t.root.u("nt.optional.n_directories"), 16u);
        CB_CHECK_EQ(t.root.u("nt.optional.directories[1].size"), 0x28u);
        CB_CHECK_EQ(t.root.get("nt.optional.subsystem").label, "console");
        CB_CHECK_EQ(t.root.str("nt.sections[0].name"), ".text");
        CB_CHECK_EQ(t.root.get("nt.sections[0].characteristics").label, "code|execute|read");
        expect_agreement(reg, "pe", image);

        // Patch a value through the tree: only those bytes change.
        eng::Tree patched = reg.read("pe", image);
        patched.root.get("nt.optional.entry_point").value = 0x1234;
        const Bytes out = eng::write(patched);
        CB_CHECK_EQ(out.size(), image.size());
        std::size_t differing = 0;
        for (std::size_t i = 0; i < out.size(); ++i) differing += out[i] != image[i];
        CB_CHECK_EQ(differing, std::size_t{2});   // 0x1000 -> 0x1234
        CB_CHECK_EQ(reg.read("pe", out).root.u("nt.optional.entry_point"), 0x1234u);
    }
    CB_CHECK(!reg.try_read("pe", ne_image()).has_value());
    CB_CHECK(!reg.try_read("ne", pe_image(false)).has_value());
});

CB_TEST("formats.format_engine.ne", [] {
    const eng::Registry reg = registry();
    const Bytes image = ne_image();
    const eng::Tree t = reg.read("ne", image);
    CB_CHECK_EQ(t.root.u("ne.segment_count"), 2u);
    CB_CHECK_EQ(t.root.u("ne.shift"), 4u);
    CB_CHECK_EQ(t.root.u("ne.segments[0].file_offset"), 0x100u);
    CB_CHECK_EQ(t.root.u("ne.segments[1].byte_length"), 0u);
    CB_CHECK_EQ(t.root.get("ne.target_os").label, "windows");
    CB_CHECK_EQ(t.root.get("ne.entry_table").children.size(), std::size_t{4});   // three bundles and the terminator
    CB_CHECK_EQ(t.root.u("ne.entry_table[0].fixed[1].offset"), 0x20u);
    CB_CHECK_EQ(t.root.u("ne.entry_table[1].moveable[0].segment"), 2u);
    expect_agreement(reg, "ne", image);
});

CB_TEST("formats.format_engine.ines", [] {
    const eng::Registry reg = registry();
    {
        const Bytes image = ines_image(2, 1, 0x43, 0x20);
        const eng::Tree t = reg.read("ines", image);
        CB_CHECK_EQ(t.root.u("header.mapper"), 0x24u);
        CB_CHECK_EQ(t.root.u("header.battery"), 1u);
        CB_CHECK_EQ(t.root.get("header.flags6").label, "vertical|battery|0x40");   // 0x40: the mapper's low nibble, no flag
        CB_CHECK_EQ(t.root.get("prg").size, 0x8000u);
        CB_CHECK_EQ(t.root.get("chr").size, 0x2000u);
        expect_agreement(reg, "ines", image);
    }
    {   // a trainer, four-screen, and a title after the CHR
        Bytes image = ines_image(1, 1, 0x0C, 0);
        image.insert(image.begin() + 16, 512, 0xEE);
        append(image, as_bytes("TITLE"));
        expect_agreement(reg, "ines", image);
        CB_CHECK_EQ(reg.read("ines", image).root.get("extra").size, 5u);
    }
    {   // NES 2.0: the mapper widens, the submapper appears
        const Bytes image = ines_image(0, 0, 0x10, 0x28, false, 0x35, 0);
        const eng::Tree t = reg.read("ines", image);
        CB_CHECK_EQ(t.root.u("header.mapper"), 0x521u);
        CB_CHECK_EQ(t.root.u("header.submapper"), 3u);
        expect_agreement(reg, "ines", image);
    }
    {   // a trimmed dump keeps the CHR it has; a cut-short PRG is an error in both readers
        Bytes image = ines_image(1, 1, 0, 0);
        image.resize(image.size() - 0x100);
        expect_agreement(reg, "ines", image);
        Bytes cut = ines_image(2, 0, 0, 0);
        cut.resize(cut.size() - 1);
        const eng::Comparison c = eng::compare(reg, "ines", cut);
        CB_CHECK(c.agree());
        CB_CHECK(!c.engine_accepts);
    }
    CB_CHECK(!reg.try_read("ines", Bytes{'M', 'Z', 0, 0}).has_value());
});

CB_TEST("formats.format_engine.identify", [] {
    const eng::Registry reg = registry();
    // (the console descriptions that look for a header at a fixed place and ask no magic -- gb, genesis, sms -- read any file
    // of enough size, so the PC and NES formats are checked among the ones that need a magic)
    const auto names = [&](const Bytes& b) {
        std::string s;
        for (const auto& n : reg.identify(b))
            if (n != "gb" && n != "genesis" && n != "sms") s += n + " ";
        return s;
    };
    CB_CHECK_EQ(names(pe_image(false)), "mz pe ");
    CB_CHECK_EQ(names(ne_image()), "mz ne ");
    CB_CHECK_EQ(names(ines_image(1, 0, 0, 0)), "ines ");
});

// The engine against the hand-written readers on real files: every file in the repository's test data, the test binaries
// and a homebrew NES image, through every format -- the two readers agree on each (both read it and every value is the
// same, or neither reads it) and the tree writes back the file's bytes.
CB_TEST("formats.format_engine.real_files_agree_with_the_readers", [] {
    const eng::Registry reg = registry();
    std::vector<std::filesystem::path> files;
    for (const std::filesystem::path dir : {testing::data_path("formats/testdata"), std::filesystem::path("D:/test-binaries")}) {
        std::error_code ec;
        for (std::filesystem::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec))
            if (it->is_regular_file(ec) && it->file_size(ec) < (64u << 20)) files.push_back(it->path());
    }
    const std::filesystem::path nes = testing::beside_checkout("mario-kernel/smb.nes");
    std::error_code ec;
    if (std::filesystem::is_regular_file(nes, ec)) files.push_back(nes);
    std::size_t both_read = 0;
    for (const auto& file : files) {
        const Bytes data = fs::read_file(file);
        for (const std::string& format : eng::compared_formats()) {
            const eng::Comparison c = eng::compare(reg, format, data);
            CB_CHECK_EQ(describe(c), "");
            if (c.reader_accepts && c.engine_accepts) {
                ++both_read;
                CB_CHECK(c.round_trip);
                CB_CHECK(c.covered_round_trip);
            }
        }
    }
    if (!files.empty()) CB_CHECK(both_read > 0);
});

// ---- the console cartridge headers: a small image of each, the values the description gives, the bytes written back, and the
// hand-written reader's agreement (a reader that runs on the description reads the same). ----

namespace {

// An image of zeros with values put at offsets.
struct Img {
    Bytes b;
    explicit Img(std::size_t n) : b(n, 0) {}
    Img& u8(std::size_t at, std::uint32_t v) { return b[at] = static_cast<std::uint8_t>(v), *this; }
    Img& u16(std::size_t at, std::uint32_t v) { return u8(at, v & 0xFF).u8(at + 1, v >> 8); }
    Img& u16be(std::size_t at, std::uint32_t v) { return u8(at, v >> 8).u8(at + 1, v & 0xFF); }
    Img& u32(std::size_t at, std::uint32_t v) { return u16(at, v & 0xFFFF).u16(at + 2, v >> 16); }
    Img& u32be(std::size_t at, std::uint32_t v) { return u16be(at, v >> 16).u16be(at + 2, v & 0xFFFF); }
    Img& str(std::size_t at, std::string_view s) { return std::copy(s.begin(), s.end(), b.begin() + static_cast<std::ptrdiff_t>(at)), *this; }
};

// Reads `image` as `format` and checks the tree writes back to the same bytes.
eng::Tree read_back(const eng::Registry& reg, std::string_view format, const Bytes& image) {
    eng::Tree t = reg.read(format, image);
    CB_CHECK(eng::write(t) == image);
    CB_CHECK(eng::write(t, true).size() == image.size());
    return t;
}

}  // namespace

CB_TEST("formats.format_engine.gb", [] {
    const eng::Registry reg = registry();
    for (const std::uint32_t cgb : {0x00u, 0x80u, 0xC0u}) {
        Img i(0x8000);
        i.str(0x134, "TETRIS").u8(0x143, cgb).u8(0x146, 3).u8(0x147, 0x13).u16be(0x14E, 0xBEEF);
        std::uint32_t x = 0;
        for (std::size_t k = 0x134; k < 0x14D; ++k) x = (x - i.b[k] - 1) & 0xFF;
        i.u8(0x14D, x);
        const eng::Tree t = read_back(reg, "gb", i.b);
        const eng::Node& h = t.root.get("header");
        CB_CHECK_EQ(h.u("color"), cgb == 0 ? 0u : cgb == 0x80 ? 1u : 2u);
        CB_CHECK_EQ(h.get("title").size, cgb == 0 ? 15u : 11u);   // a Color cartridge's title is 11 bytes
        CB_CHECK_EQ(h.str("title"), "TETRIS");
        CB_CHECK_EQ(h.u("sgb"), 1u);
        CB_CHECK_EQ(h.u("cartridge_type"), 0x13u);
        CB_CHECK_EQ(h.u("header_checksum"), h.u("computed_checksum"));
        CB_CHECK_EQ(h.u("global_checksum"), 0xBEEFu);
        expect_agreement(reg, "gb", i.b);
    }
    CB_CHECK_THROWS_AS(FormatError, reg.read("gb", Bytes(0x14F, 0)));
});

CB_TEST("formats.format_engine.gba", [] {
    const eng::Registry reg = registry();
    Img i(0x1000);
    i.u32(0, 0xEA00002E).str(0xA0, "HELLO").str(0xAC, "AHEE").str(0xB0, "01").u8(0xB2, 0x96).u8(0xBC, 2);
    std::uint32_t sum = 0x19;
    for (std::size_t k = 0xA0; k <= 0xBC; ++k) sum += i.b[k];
    i.u8(0xBD, (0 - sum) & 0xFF);
    const eng::Tree t = read_back(reg, "gba", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.u("is_branch"), 1u);
    CB_CHECK_EQ(h.u("entry"), 0x080000C0u);   // $08000008 + 4 * $2E
    CB_CHECK_EQ(h.str("title"), "HELLO");
    CB_CHECK_EQ(h.str("game_code"), "AHEE");
    CB_CHECK_EQ(h.u("complement"), h.u("computed_complement"));
    expect_agreement(reg, "gba", i.b);
    Img back(0x1000);   // a branch backwards: $FFFFFE is -2 words
    back.u32(0, 0xEAFFFFFE).u8(0xB2, 0x96);
    CB_CHECK_EQ(reg.read("gba", back.b).root.u("header.entry"), 0x08000000u);
    i.u8(0xB2, 0);
    CB_CHECK_THROWS_AS(FormatError, reg.read("gba", i.b));          // no $96
    CB_CHECK_THROWS_AS(FormatError, reg.read("gba", Bytes(0x20, 0)));
});

CB_TEST("formats.format_engine.ngp", [] {
    const eng::Registry reg = registry();
    Img i(0x400);
    i.str(0, " LICENSED BY SNK CORPORATION").u32(0x1C, 0x200040).u16(0x20, 0x1234).u8(0x22, 1).u8(0x23, 0x10).str(0x24, "ASTEROIDS");
    const eng::Tree t = read_back(reg, "ngp", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.u("licensed"), 1u);
    CB_CHECK_EQ(h.u("colour"), 1u);
    CB_CHECK_EQ(h.u("start"), 0x200040u);
    CB_CHECK_EQ(h.u("catalog"), 0x1234u);
    CB_CHECK_EQ(h.str("title"), "ASTEROIDS");
    expect_agreement(reg, "ngp", i.b);
    i.u32(0x1C, 0x300000);                                          // a start address outside the ROM
    CB_CHECK_THROWS_AS(FormatError, reg.read("ngp", i.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("ngp", Bytes(100, 0xFF)));
});

CB_TEST("formats.format_engine.nds", [] {
    const eng::Registry reg = registry();
    Img i(0x400);
    i.str(0, "DEMO").str(0x0C, "ABCD").str(0x10, "01").u8(0x1E, 3).u32(0x20, 0x4000).u32(0x24, 0x2000800).u32(0x28, 0x2000000).u32(0x2C, 0x1000);
    i.u32(0x30, 0x8000).u32(0x40, 0x200).u32(0x44, 0x40).u32(0x68, 0x300).u32(0x84, 0x4000).u16(0x15C, 0xCF56).u16(0x15E, 0x1234);
    const eng::Tree t = read_back(reg, "nds", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.str("title"), "DEMO");
    CB_CHECK_EQ(h.str("game_code"), "ABCD");
    CB_CHECK_EQ(h.u("version"), 3u);
    CB_CHECK_EQ(h.u("arm9.entry"), 0x2000800u);
    CB_CHECK_EQ(h.u("arm9.size_bytes"), 0x1000u);
    CB_CHECK_EQ(h.u("arm7.rom_offset"), 0x8000u);
    CB_CHECK_EQ(h.u("fnt_offset"), 0x200u);
    CB_CHECK_EQ(h.u("banner_offset"), 0x300u);
    CB_CHECK_EQ(h.u("header_crc"), 0x1234u);
    expect_agreement(reg, "nds", i.b);
    i.u16(0x15C, 0);                                                // no logo checksum
    CB_CHECK_THROWS_AS(FormatError, reg.read("nds", i.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("nds", Bytes(0x100, 0)));
});

CB_TEST("formats.format_engine.vb", [] {
    const eng::Registry reg = registry();
    Img i(0x4000);
    i.u16(0x4000 - 0x10, 0x1234).str(0x4000 - 0x220, "VB TEST").str(0x4000 - 0x220 + 25, "AB").str(0x4000 - 0x220 + 27, "VTST").u8(0x4000 - 0x220 + 31, 1);
    const eng::Tree t = read_back(reg, "vb", i.b);   // the header sits 0x220 bytes before the end
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.offset, 0x4000u - 0x220);
    CB_CHECK_EQ(h.str("title"), "VB TEST");
    CB_CHECK_EQ(h.str("maker"), "AB");
    CB_CHECK_EQ(h.str("game_code"), "VTST");
    CB_CHECK_EQ(h.u("version"), 1u);
    expect_agreement(reg, "vb", i.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("vb", Bytes(0x5000, 1)));       // not a power of two
    CB_CHECK_THROWS_AS(FormatError, reg.read("vb", Bytes(0x4000, 0xFF)));    // the reset slot is padding
    CB_CHECK_THROWS_AS(FormatError, reg.read("vb", Bytes(0x4000, 0)));
});

CB_TEST("formats.format_engine.pce", [] {
    const eng::Registry reg = registry();
    Img plain(0x4000);
    plain.u16(0x2000 - 10, 0xE100).u16(0x2000 - 4, 0xE200).u16(0x2000 - 2, 0xE000);
    const eng::Tree t = read_back(reg, "pce", plain.b);
    CB_CHECK_EQ(t.root.get("copier").size, 0u);
    CB_CHECK_EQ(t.root.u("vectors.irq2"), 0xE100u);
    CB_CHECK_EQ(t.root.u("vectors.nmi"), 0xE200u);
    CB_CHECK_EQ(t.root.u("vectors.reset"), 0xE000u);
    expect_agreement(reg, "pce", plain.b);
    Bytes copier(512, 0xCC);                                         // a 512-byte copier header in front
    copier.insert(copier.end(), plain.b.begin(), plain.b.end());
    const eng::Tree c = read_back(reg, "pce", copier);
    CB_CHECK_EQ(c.root.get("copier").size, 512u);
    CB_CHECK_EQ(c.root.get("banks").size, 0x4000u);
    CB_CHECK_EQ(c.root.u("vectors.reset"), 0xE000u);
    expect_agreement(reg, "pce", copier);
    plain.u16(0x2000 - 2, 0x1000);                                   // RESET outside $E000-$FFFF
    CB_CHECK_THROWS_AS(FormatError, reg.read("pce", plain.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("pce", Bytes(0x2001, 0)));
});

CB_TEST("formats.format_engine.atari7800", [] {
    const eng::Registry reg = registry();
    auto image = [](std::size_t rom, std::uint32_t cart_type) {
        Img i(128 + rom);
        i.u8(0, 1).str(1, "ATARI7800").str(17, "PONG").u32be(49, static_cast<std::uint32_t>(rom)).u16be(53, cart_type).u8(55, 1).u8(57, 1);
        return i;
    };
    const Img linear = image(0x4000, 0);
    const eng::Tree t = read_back(reg, "atari7800", linear.b);
    CB_CHECK_EQ(t.root.get("header").str("title"), "PONG");
    CB_CHECK_EQ(t.root.u("header.declared_size"), 0x4000u);
    CB_CHECK_EQ(t.root.u("supergame"), 0u);
    CB_CHECK_EQ(t.root.u("base"), 0xC000u);                          // a 16 KB ROM sits at the top of the address space
    CB_CHECK_EQ(t.root.get("rom").size, 0x4000u);
    expect_agreement(reg, "atari7800", linear.b);
    const Img super = image(0x20000, 2);
    const eng::Tree s = read_back(reg, "atari7800", super.b);
    CB_CHECK_EQ(s.root.u("supergame"), 1u);
    CB_CHECK_EQ(s.root.u("base"), 0u);
    expect_agreement(reg, "atari7800", super.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("atari7800", Bytes(200, 0)));      // no magic
    CB_CHECK_THROWS_AS(FormatError, reg.read("atari7800", Bytes(50, 0)));
    CB_CHECK_THROWS_AS(FormatError, reg.read("atari7800", image(0x4001, 0).b)); // not a multiple of 4 KB
    Img cut = image(0x4000, 0);
    cut.u32be(49, 0x8000);                                           // declares more than there is
    CB_CHECK_THROWS_AS(FormatError, reg.read("atari7800", cut.b));
});

CB_TEST("formats.format_engine.sms", [] {
    const eng::Registry reg = registry();
    Img i(0x8000);
    i.str(0x7FF0, "TMR SEGA").u16(0x7FFA, 0xBEEF).u8(0x7FFC, 0x34).u8(0x7FFD, 0x12).u8(0x7FFE, 0x25).u8(0x7FFF, 0x4C);   // product 2 1234, version 5, export, 32 KB
    const eng::Tree t = read_back(reg, "sms", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.offset, 0x7FF0u);
    CB_CHECK_EQ(h.u("checksum"), 0xBEEFu);
    CB_CHECK_EQ(h.u("version"), 5u);
    CB_CHECK_EQ(h.u("product2") >> 4, 2u);
    CB_CHECK_EQ(h.u("region"), 4u);
    CB_CHECK_EQ(h.u("size_code"), 0xCu);
    expect_agreement(reg, "sms", i.b);
    Img small(0x4000);                                               // the signature at $3FF0 instead
    small.str(0x3FF0, "TMR SEGA");
    CB_CHECK_EQ(read_back(reg, "sms", small.b).root.get("header").offset, 0x3FF0u);
    CB_CHECK(read_back(reg, "sms", Bytes(0x8000, 0)).root.find("header") == nullptr);   // none: an SG-1000 or homebrew ROM
    CB_CHECK(read_back(reg, "sms", Bytes(100, 0)).root.find("header") == nullptr);
    expect_agreement(reg, "sms", small.b);
});

CB_TEST("formats.format_engine.genesis", [] {
    const eng::Registry reg = registry();
    Img i(0x400);
    i.u32be(0, 0xFF0000).u32be(4, 0x200).u32be(0x78, 0x300);          // the stack, reset and a vector at index 30 (vblank)
    i.str(0x100, "SEGA MEGA DRIVE ").str(0x110, "(C)TEST 2024.JAN").str(0x120, "DOMESTIC").str(0x150, "OVERSEAS").str(0x180, "GM 00000000-00");
    i.u16be(0x18E, 0x1234).u32be(0x1A0, 0).u32be(0x1A4, 0x3FF).u32be(0x1A8, 0xFF0000).u32be(0x1AC, 0xFFFFFF).str(0x1F0, "JUE");
    const eng::Tree t = read_back(reg, "genesis", i.b);
    CB_CHECK_EQ(t.root.get("vectors").children.size(), std::size_t{64});
    CB_CHECK_EQ(t.root.get("vectors").children[1].value, 0x200u);
    CB_CHECK_EQ(t.root.get("vectors").children[30].value, 0x300u);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.str("system"), "SEGA MEGA DRIVE ");
    CB_CHECK_EQ(h.str("domestic_name"), "DOMESTIC");
    CB_CHECK_EQ(h.u("checksum"), 0x1234u);
    CB_CHECK_EQ(h.u("rom_end"), 0x3FFu);
    CB_CHECK_EQ(h.u("ram_start"), 0xFF0000u);
    CB_CHECK_EQ(h.str("region"), "JUE");
    expect_agreement(reg, "genesis", i.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("genesis", Bytes(0x1FF, 0)));
    CB_CHECK_NOTHROW((void)reg.read("genesis", Bytes(0x200, 0)));
});

CB_TEST("formats.format_engine.n64", [] {
    const eng::Registry reg = registry();
    Img i(0x1000);
    i.u32be(0, 0x80371240).u32be(4, 0xF).u32be(8, 0x80000400).u32be(0x10, 0x11111111).u32be(0x14, 0x22222222).str(0x20, "TESTROM").u8(0x3B, 'N').str(0x3C, "XY").u8(0x3E, 'E').u8(0x3F, 1);
    const eng::Tree t = read_back(reg, "n64", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.u("entry"), 0x80000400u);
    CB_CHECK_EQ(h.u("crc2"), 0x22222222u);
    CB_CHECK_EQ(h.str("name"), "TESTROM");
    CB_CHECK_EQ(h.get("game_code").text, "XY");
    CB_CHECK_EQ(h.u("country"), static_cast<std::uint32_t>('E'));
    expect_agreement(reg, "n64", i.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("n64", Bytes(0x2000, 0x41)));   // no PI configuration word
    CB_CHECK_THROWS_AS(FormatError, reg.read("n64", ByteView(i.b).first(0x800)));
});

CB_TEST("formats.format_engine.snes", [] {
    const eng::Registry reg = registry();
    auto cartridge = [](std::size_t size, std::size_t place, std::uint32_t map) {
        Img i(size);
        i.str(place, "SUPER TEST CART      ").u8(place + 0x15, map).u8(place + 0x16, 2).u8(place + 0x17, 8).u8(place + 0x1B, 1);
        i.u16(place + 0x1C, 0x1234).u16(place + 0x1E, 0xFFFF - 0x1234).u16(place + 0x24, 0x8010).u16(place + 0x3C, 0x8000).u16(place + 0x3A, 0x8020);
        return i;
    };
    const Img lo = cartridge(0x8000, 0x7FC0, 0x20);
    const eng::Tree t = read_back(reg, "snes", lo.b);
    CB_CHECK_EQ(t.root.u("is_hirom"), 0u);
    CB_CHECK_EQ(t.root.u("header_offset"), 0x7FC0u);
    CB_CHECK_EQ(t.root.u("best"), 12u);                              // 4 + 2 + 1 + 2 + 2 + 1
    const eng::Node& h = t.root.get("lo");
    CB_CHECK_EQ(h.str("title"), "SUPER TEST CART      ");   // 21 bytes: the title field, space padded
    CB_CHECK_EQ(h.u("map_mode"), 0x20u);
    CB_CHECK_EQ(h.u("native.cop"), 0x8010u);
    CB_CHECK_EQ(h.u("emulation.reset"), 0x8000u);
    CB_CHECK_EQ(h.u("emulation.nmi"), 0x8020u);
    expect_agreement(reg, "snes", lo.b);
    const Img hi = cartridge(0x10000, 0xFFC0, 0x21);                 // HiROM: bit 0 of the map byte
    const eng::Tree u = read_back(reg, "snes", hi.b);
    CB_CHECK_EQ(u.root.u("is_hirom"), 1u);
    CB_CHECK_EQ(u.root.u("header_offset"), 0xFFC0u);
    expect_agreement(reg, "snes", hi.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("snes", Bytes(100, 0)));
    CB_CHECK_THROWS_AS(FormatError, reg.read("snes", Bytes(0x8000, 0xFF)));     // a size, and no internal header
    CB_CHECK_THROWS_AS(FormatError, reg.read("snes", Bytes(0x10000, 0x00)));
});

CB_TEST("formats.format_engine.lynx", [] {
    const eng::Registry reg = registry();
    Img cart(128);
    cart.str(0, "LYNX").u16(4, 512).u16(6, 1024).u16(8, 1).str(10, "BOING").str(42, "ATARI").u8(58, 1).u8(64, 0xFE);
    const eng::Tree t = read_back(reg, "lynx", cart.b);
    CB_CHECK_EQ(t.root.u("is_cartridge"), 1u);
    CB_CHECK_EQ(t.root.u("cart.bank0_block"), 512u);
    CB_CHECK_EQ(t.root.u("cart.bank1_block"), 1024u);
    CB_CHECK_EQ(t.root.get("cart").str("name"), "BOING");
    CB_CHECK_EQ(t.root.get("cart").str("manufacturer"), "ATARI");
    CB_CHECK_EQ(t.root.u("cart.rotation"), 1u);
    CB_CHECK_EQ(t.root.u("encrypted_count"), 2u);                    // 0x100 - $FE blocks claimed
    expect_agreement(reg, "lynx", cart.b);
    Img obj(0x40);                                                   // a BS93 object: load address $0200, block length $0040
    obj.u16be(2, 0x200).u16be(4, 0x40).str(6, "BS93");
    const eng::Tree o = read_back(reg, "lynx", obj.b);
    CB_CHECK_EQ(o.root.u("is_bs93"), 1u);
    CB_CHECK_EQ(o.root.u("object.load_address"), 0x200u);
    CB_CHECK_EQ(o.root.u("object.block_length"), 0x40u);
    expect_agreement(reg, "lynx", obj.b);
    obj.u16be(4, 0x80);                                              // a block longer than the file
    CB_CHECK_THROWS_AS(FormatError, reg.read("lynx", obj.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("lynx", Bytes(100, 0x55)));
});

CB_TEST("formats.format_engine.intv", [] {
    const eng::Registry reg = registry();
    Img i(3 + 2 + 512 + 2 + 4);                                      // one segment of one page ($5000), four bytes after it
    i.u8(0, 0xA8).u8(1, 1).u8(2, 0xFE).u8(3, 0x50).u8(4, 0x50).u16be(5, 0x1234).u16be(5 + 512, 0xBEEF).str(3 + 2 + 512 + 2, "TAIL");
    const eng::Tree t = read_back(reg, "intv", i.b);
    CB_CHECK_EQ(t.root.get("segments").children.size(), std::size_t{1});
    const eng::Node& s = t.root.get("segments[0]");
    CB_CHECK_EQ(s.u("first_page"), 0x50u);
    CB_CHECK_EQ(s.get("words").size, 512u);                          // 256 words of two bytes
    CB_CHECK_EQ(s.u("crc"), 0xBEEFu);
    CB_CHECK_EQ(t.root.get("tail").size, 4u);
    expect_agreement(reg, "intv", i.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("intv", Bytes(2000, 0)));
    Img bad = i;
    bad.u8(2, 0x00);                                                 // N and ~N do not match
    CB_CHECK_THROWS_AS(FormatError, reg.read("intv", bad.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("intv", ByteView(i.b).first(300)));   // a segment cut short
});

CB_TEST("formats.format_engine.coleco", [] {
    const eng::Registry reg = registry();
    Img i(0x2000);
    i.u8(0, 0xAA).u8(1, 0x55).u16(2, 0x7000).u16(4, 0x7100).u16(6, 0x7200).u16(8, 0x7300).u16(10, 0x8100).str(0x24, "TEST GAME");
    const eng::Tree t = read_back(reg, "coleco", i.b);
    const eng::Node& h = t.root.get("header");
    CB_CHECK_EQ(h.u("title_screen"), 1u);
    CB_CHECK_EQ(h.u("sprite_names"), 0x7000u);
    CB_CHECK_EQ(h.u("controllers"), 0x7300u);
    CB_CHECK_EQ(h.u("start"), 0x8100u);
    expect_agreement(reg, "coleco", i.b);
    i.u8(0, 0x55).u8(1, 0xAA);                                       // $55 $AA: no title screen
    CB_CHECK_EQ(reg.read("coleco", i.b).root.u("header.title_screen"), 0u);
    i.u8(1, 0x00);
    CB_CHECK_THROWS_AS(FormatError, reg.read("coleco", i.b));
    CB_CHECK_THROWS_AS(FormatError, reg.read("coleco", Bytes(0x10, 0xAA)));   // shorter than the header
});

CB_TEST("formats.format_engine.msx", [] {
    const eng::Registry reg = registry();
    Img at0(0x4000);
    at0.str(0, "AB").u16(2, 0x4010).u16(4, 0x4020).u16(6, 0).u16(8, 0x5000);
    const eng::Tree t = read_back(reg, "msx", at0.b);
    CB_CHECK_EQ(t.root.get("header").offset, 0u);
    CB_CHECK_EQ(t.root.u("header.init"), 0x4010u);
    CB_CHECK_EQ(t.root.u("header.statement"), 0x4020u);
    CB_CHECK_EQ(t.root.u("header.text"), 0x5000u);
    expect_agreement(reg, "msx", at0.b);
    Img page1(0xC000);                                               // a 48 KB image that fills pages 0-2: "AB" at $4000
    page1.str(0x4000, "AB").u16(0x4002, 0x4010);
    const eng::Tree p = read_back(reg, "msx", page1.b);
    CB_CHECK_EQ(p.root.get("header").offset, 0x4000u);
    CB_CHECK_EQ(p.root.u("header.init"), 0x4010u);
    expect_agreement(reg, "msx", page1.b);
    CB_CHECK_THROWS_AS(FormatError, reg.read("msx", Bytes(0x4000, 0)));      // no "AB"
    CB_CHECK_THROWS_AS(FormatError, reg.read("msx", Bytes{}));
});

// The language additions the console headers needed: raw peeks, sum8, printable, and an assertion's own words.
CB_TEST("formats.format_engine.language_peeks", [] {
    eng::Registry reg;
    reg.add_text(R"(
        format peeks
        endian le
        struct File {
            let a = peek8(1)
            let b = peek16(1)
            let c = peek16be(1)
            let d = peek32(0)
            let e = peek32be(0)
            let outside = peek8(100)
            let total = sum8(0, 4)
            let text = printable(4, 4)
            first : u8
            assert first != 0 : "the first byte is zero"
        }
    )");
    const Bytes data = {0x01, 0x02, 0x03, 0x04, 'A', 'b', 0x00, '~'};
    const eng::Tree t = reg.read("peeks", data);
    CB_CHECK_EQ(t.root.u("a"), 2u);
    CB_CHECK_EQ(t.root.u("b"), 0x0302u);
    CB_CHECK_EQ(t.root.u("c"), 0x0203u);
    CB_CHECK_EQ(t.root.u("d"), 0x04030201u);
    CB_CHECK_EQ(t.root.u("e"), 0x01020304u);
    CB_CHECK_EQ(t.root.i("outside"), -1);
    CB_CHECK_EQ(t.root.u("total"), 10u);
    CB_CHECK_EQ(t.root.u("text"), 3u);                               // 'A' 'b' '~' are printable, the NUL is not
    Bytes zero = data;
    zero[0] = 0;
    try {
        (void)reg.read("peeks", zero);
        CB_CHECK(false);
    } catch (const FormatError& e) {
        CB_CHECK(std::string(e.what()).find("the first byte is zero") != std::string::npos);
    }
});

// The console readers on the homebrew programs that sit beside the checkout (a repository of them): the description and the reader
// agree on each file, and the tree writes the bytes back.
CB_TEST("formats.format_engine.console_homebrew_agrees", [] {
    const std::filesystem::path root = testing::beside_checkout("homebrew-samples");
    std::error_code ec;
    if (!std::filesystem::is_directory(root, ec)) return;
    const eng::Registry reg = registry();
    std::size_t read_by_both = 0;
    for (std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec)) {
        if (!it->is_regular_file(ec) || it->file_size(ec) > (32u << 20)) continue;
        const Bytes data = fs::read_file(it->path());
        for (const std::string& format : eng::compared_formats()) {
            const eng::Comparison c = eng::compare(reg, format, data);
            CB_CHECK_EQ(describe(c), "");
            if (c.reader_accepts && c.engine_accepts) {
                ++read_by_both;
                CB_CHECK(c.round_trip);
                CB_CHECK(c.covered_round_trip);
            }
        }
    }
    CB_CHECK(read_by_both > 0);
});

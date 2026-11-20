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
    for (const char* name : {"mz", "pe", "ne", "ines"}) CB_CHECK(reg.has(name));
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
    const auto names = [&](const Bytes& b) { std::string s; for (const auto& n : reg.identify(b)) s += n + " "; return s; };
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
        for (const char* format : {"mz", "pe", "ne", "ines"}) {
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

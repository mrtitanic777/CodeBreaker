// Tests of cb/formats/powerpc_image.hpp. natalsu.exe is a real Xbox 360 image (the dashboard's natalsu, unpacked from its .xex; it is
// held as it sits in memory); natalsu.golden.txt is the recorded description of it (gen_powerpc_image_golden.py).
// The run-time type information and the file layout are tested on a small image made here, whose every word is written out.
#include "formats/powerpc_image.hpp"

#include <algorithm>
#include <format>
#include <sstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::formats::powerpc_image {
namespace {

// ---- the real image --------------------------------------------------------------------------------------------------------------

Image natalsu() { return Image::read(testing::data_path("formats/testdata/natalsu.exe")); }

std::vector<std::string> golden_lines() {
    const std::string text = fs::read_text(testing::data_path("formats/testdata/natalsu.golden.txt"));
    std::vector<std::string> lines;
    for (const std::string_view line : text::lines(text)) lines.emplace_back(line);
    return lines;
}

std::uint64_t hex_of(const std::string& s) { return std::stoull(s, nullptr, 16); }

CB_TEST("formats.powerpc_image.a_real_image", [] {
    const Image image = natalsu();
    CB_CHECK_EQ(image.base(), 0x98000000u);
    CB_CHECK(image.memory_layout());
    CB_CHECK_EQ(image.entry(), 0x980026a8u);
    CB_CHECK_EQ(image.code_begin(), 0x98002000u);
    CB_CHECK_EQ(image.code_end(), 0x98006454u);
    std::vector<std::string> got;
    for (const Span& s : image.spans()) got.push_back(std::format("span {:08x} {:x} {:x}", s.va, s.file_offset, s.size));
    for (const Section& s : image.sections()) got.push_back(std::format("section {} {:08x} {:x} {:08x}", s.name, s.va, s.size, s.characteristics));
    for (const Address a : image.functions()) got.push_back(std::format("function {:08x}", a));
    for (const auto& [a, name] : image.imports()) got.push_back(std::format("import {:08x} {}", a, name));
    for (const auto& [a, name] : image.inlined()) got.push_back(std::format("inline {:08x} {}", a, name));
    for (const auto& [a, name] : image.run_names()) got.push_back(std::format("run {:08x} {}", a, name));
    for (const auto& [a, e] : image.extents()) got.push_back(std::format("extent {:08x} {:x} {:x} {}", a, e.size, e.prologue_size, e.has_handler ? 1 : 0));
    // the golden's lines, kept in the order they were written (spans, sections, functions, imports, inline, runs, extents)
    std::vector<std::string> want;
    for (const std::string& line : golden_lines())
        if (!line.starts_with("base ") && !line.starts_with("memory ") && !line.starts_with("code ") && !line.starts_with("entry ") && !line.starts_with("word "))
            want.push_back(line);
    CB_CHECK_EQ(got.size(), want.size());
    CB_CHECK(got == want);
    CB_CHECK_EQ(image.functions().size(), 87u);
    CB_CHECK_EQ(image.imports().size(), 28u);
    CB_CHECK_EQ(image.extents().size(), 70u);
    // the words: where the file has them, and where it has not
    for (const std::string& line : golden_lines()) {
        if (!line.starts_with("word ")) continue;
        const std::vector<std::string_view> f = text::split(line, ' ');
        const std::optional<std::uint32_t> w = image.word(static_cast<Address>(hex_of(std::string(f[1]))));
        if (f[2] == "none") CB_CHECK(!w.has_value());
        else CB_CHECK(w && *w == hex_of(std::string(f[2])));
    }
});

CB_TEST("formats.powerpc_image.addresses_bytes_and_regions", [] {
    const Image image = natalsu();
    CB_CHECK_EQ(image.offset(0x98000000u).value_or(~0ull), 0ull);
    CB_CHECK(!image.offset(0x10u).has_value());
    CB_CHECK(!image.offset(0x98009000u + 0x9000).has_value());   // past the span
    const ByteView first = image.bytes_at(image.code_begin(), 8);
    CB_CHECK_EQ(first.size(), 8u);
    CB_CHECK(image.bytes_at(0x10, 8).empty());
    CB_CHECK_EQ(image.bytes_at(0x98008ffcu, 100).size(), 4u);   // the file ends there
    const std::vector<Image::Region> regions = image.regions();
    CB_CHECK_EQ(regions.size(), 1u);
    CB_CHECK_EQ(regions[0].va, 0x98000000u);
    CB_CHECK_EQ(regions[0].bytes.size(), 0x9000u);
    // a function's first instruction as the word and as the bytes agree
    const Address f = image.functions().front();
    const ByteView b = image.bytes_at(f, 4);
    CB_CHECK_EQ(image.word(f).value_or(0), (std::uint32_t{b[0]} << 24) | (std::uint32_t{b[1]} << 16) | (std::uint32_t{b[2]} << 8) | b[3]);
});

CB_TEST("formats.powerpc_image.errors", [] {
    CB_CHECK_THROWS_AS(NotFoundError, (void)Image::read(testing::data_path("formats/testdata/no_such_image.exe")));
    // an i386 module is not a PowerPC image
    CB_CHECK_THROWS_AS(FormatError, (void)Image::read(testing::data_path("formats/testdata/lz32.dll")));
    CB_CHECK_THROWS_AS(FormatError, (void)Image::parse(Bytes(64, 0)));
    CB_CHECK_THROWS_AS(FormatError, (void)Image::parse(Bytes{'M', 'Z'}));
});

CB_TEST("formats.powerpc_image.a_base_that_is_given_wins", [] {
    const Image image = Image::parse(fs::read_file(testing::data_path("formats/testdata/natalsu.exe")), 0x82000000u);
    CB_CHECK_EQ(image.base(), 0x82000000u);
    CB_CHECK_EQ(image.entry(), 0x820026a8u);
    // (the exception table's own addresses are the file's, so with another base none of them lands in the file: it is read as a packed one,
    // its first section at the new base + .rdata's address)
    CB_CHECK(!image.memory_layout());
    CB_CHECK_EQ(image.spans()[0].va, 0x82000400u);
    CB_CHECK_EQ(image.sections()[0].va, 0x82000400u);
});

// ---- a small image made here -------------------------------------------------------------------------------------------------------

void put_le(Bytes& b, std::size_t at, std::uint32_t v, int n) {
    for (int i = 0; i < n; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (8 * i));
}
void put_be(Bytes& b, std::size_t at, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) b[at + i] = static_cast<std::uint8_t>(v >> (24 - 8 * i));
}
void put_text(Bytes& b, std::size_t at, std::string_view text) {
    for (std::size_t i = 0; i < text.size(); ++i) b[at + i] = static_cast<std::uint8_t>(text[i]);
}

constexpr std::uint32_t kBase = 0x82000000u;
constexpr std::uint32_t kRdata = kBase + 0x1000, kPdata = kBase + 0x2000, kText = kBase + 0x3000;
constexpr std::uint32_t kNop = 0x60000000, kBlr = 0x4E800020, kMflr = 0x7D8802A6;

// A PowerPC image of three sections in file layout (.rdata at file 0x200, .pdata at 0x400, .text at 0x600), with these functions:
//   0x82003000  A   mflr r12; bl C; blr          (in the exception table)
//   0x82003010  B   mflr r12; blr                (in the exception table, with a handler)
//   0x82003020  C   nop; blr                     (found by the bl)
// then a run of two `std` before `stw r12,-8(r1); blr` (__savegprlr_14 and _15), and an import stub (library 3, ordinal 0x123).
// .rdata holds the run-time type information of class Outer::Inner (a table listing A and B) and of a class of the standard library
// (a table listing C).
Bytes small_image() {
    Bytes f(0x700, 0);
    put_text(f, 0, "MZ");
    put_le(f, 0x3c, 0x40, 4);
    put_text(f, 0x40, "PE");
    put_le(f, 0x44, 0x01F2, 2);   // machine
    put_le(f, 0x46, 3, 2);        // sections
    put_le(f, 0x54, 0xE0, 2);     // size of the optional header
    put_le(f, 0x56, 0x0102, 2);
    const std::size_t opt = 0x58;
    put_le(f, opt, 0x10b, 2);
    put_le(f, opt + 16, 0x3000, 4);        // entry rva
    put_le(f, opt + 28, kBase, 4);         // image base
    put_le(f, opt + 32, 0x1000, 4);
    put_le(f, opt + 36, 0x200, 4);
    put_le(f, opt + 56, 0x4000, 4);
    put_le(f, opt + 60, 0x200, 4);
    put_le(f, opt + 92, 16, 4);
    put_le(f, opt + 96 + 3 * 8, 0x2000, 4);   // exception table
    put_le(f, opt + 96 + 3 * 8 + 4, 0x18, 4);
    struct Sec {
        const char* name;
        std::uint32_t rva, raw, size, chars;
    };
    const Sec secs[] = {{".rdata", 0x1000, 0x200, 0x200, 0x40000040}, {".pdata", 0x2000, 0x400, 0x18, 0x40000040}, {".text", 0x3000, 0x600, 0x80, 0x60000020}};
    for (std::size_t i = 0; i < 3; ++i) {
        const std::size_t h = 0x138 + 40 * i;
        put_text(f, h, secs[i].name);
        put_le(f, h + 8, secs[i].size, 4);
        put_le(f, h + 12, secs[i].rva, 4);
        put_le(f, h + 16, secs[i].size, 4);
        put_le(f, h + 20, secs[i].raw, 4);
        put_le(f, h + 36, secs[i].chars, 4);
    }
    // .text
    const std::size_t text = 0x600;
    put_be(f, text + 0x00, kMflr);
    put_be(f, text + 0x04, 0x48000000 | 0x1C | 1);   // bl +0x1C: C
    put_be(f, text + 0x08, kBlr);
    put_be(f, text + 0x10, kMflr);
    put_be(f, text + 0x14, kBlr);
    put_be(f, text + 0x20, kNop);
    put_be(f, text + 0x24, kBlr);
    put_be(f, text + 0x28, 0xF9C1FF70);   // std r14,-0x90(r1)
    put_be(f, text + 0x2c, 0xF9E1FF78);   // std r15,-0x88(r1)
    put_be(f, text + 0x30, 0x9181FFF8);   // stw r12,-8(r1)
    put_be(f, text + 0x34, kBlr);
    put_be(f, text + 0x38, 0x01030123);   // the import stub: library 3, ordinal 0x123
    put_be(f, text + 0x3c, 0x02000000);
    put_be(f, text + 0x40, 0x7D6903A6);   // mtctr r11
    put_be(f, text + 0x44, 0x4E800420);   // bctr
    // .pdata: A (4 words, prologue 2), B (2 words, prologue 1, a handler), and an empty record
    put_be(f, 0x400, kText + 0x00);
    put_be(f, 0x404, (4u << 8) | 2);
    put_be(f, 0x408, kText + 0x10);
    put_be(f, 0x40c, 0x80000000u | (2u << 8) | 1);
    // .rdata: Outer::Inner
    const std::size_t rdata = 0x200;
    put_be(f, rdata + 0x00, kRdata + 0x44);   // the descriptor's table pointer
    put_text(f, rdata + 0x08, ".?AVInner@Outer@@");
    put_be(f, rdata + 0x2c, kRdata + 0x00);   // the locator (signature 0 at +0x20) names the descriptor
    put_be(f, rdata + 0x40, kRdata + 0x20);   // the word before the table points at the locator
    put_be(f, rdata + 0x44, kText + 0x00);    // the table: A, B, then something that is not a function
    put_be(f, rdata + 0x48, kText + 0x10);
    put_be(f, rdata + 0x4c, kText + 0x08);
    // a class of the standard library, whose table lists C
    put_be(f, rdata + 0x60, kRdata + 0x84);
    put_text(f, rdata + 0x68, ".?AVfoo@std@@");
    put_be(f, rdata + 0x8c, kRdata + 0x60);   // the locator at +0x80
    put_be(f, rdata + 0xa0, kRdata + 0x80);
    put_be(f, rdata + 0xa4, kText + 0x20);
    return f;
}

CB_TEST("formats.powerpc_image.a_small_image_in_file_layout", [] {
    const Image image = Image::parse(small_image());
    CB_CHECK_EQ(image.base(), kBase);
    CB_CHECK(!image.memory_layout());
    CB_CHECK_EQ(image.entry(), kText);
    CB_CHECK_EQ(image.code_begin(), kText);
    CB_CHECK_EQ(image.code_end(), kText + 0x80);
    CB_CHECK_EQ(image.spans().size(), 3u);
    CB_CHECK((image.spans()[2] == Span{kText, 0x600, 0x80}));
    CB_CHECK_EQ(image.sections().size(), 3u);
    CB_CHECK_EQ(image.sections()[2].name, ".text");
    CB_CHECK(image.sections()[2].executable());
    CB_CHECK(!image.sections()[0].executable());
    // A and B are in the table, C is the target of A's bl; the run and the stub are not functions
    const std::vector<Address> functions(image.functions().begin(), image.functions().end());
    CB_CHECK((functions == std::vector<Address>{kText + 0x00, kText + 0x10, kText + 0x20}));
    CB_CHECK_EQ(image.imports().size(), 1u);
    CB_CHECK_EQ(image.imports().at(kText + 0x38), "import3_ord_291");
    CB_CHECK_EQ(image.inlined().size(), 2u);
    CB_CHECK_EQ(image.inlined().at(kText + 0x28), "__savegprlr_14");
    CB_CHECK_EQ(image.inlined().at(kText + 0x2c), "__savegprlr_15");
    CB_CHECK_EQ(image.extents().size(), 2u);
    CB_CHECK((image.extents().at(kText) == Extent{16, 8, false}));
    CB_CHECK((image.extents().at(kText + 0x10) == Extent{8, 4, true}));
    CB_CHECK_EQ(image.word(kText).value_or(0), kMflr);
    CB_CHECK_EQ(image.word(kText + 0x7c).value_or(1), 0u);
    CB_CHECK(!image.word(kText + 0x80).has_value());   // past the section
});

CB_TEST("formats.powerpc_image.virtual_methods_through_run_time_type_information", [] {
    const Image image = Image::parse(small_image());
    const std::vector<VirtualMethod> methods = virtual_methods(image);
    // Outer::Inner's table lists A and B (the entry after them is not a function); the standard library's class is left out, so C is not named
    CB_CHECK_EQ(methods.size(), 2u);
    CB_CHECK((methods[0] == VirtualMethod{kText + 0x00, "Outer::Inner", 0}));
    CB_CHECK((methods[1] == VirtualMethod{kText + 0x10, "Outer::Inner", 1}));
});

CB_TEST("formats.powerpc_image.no_table_to_tell_by", [] {
    // without an exception table the layout is told by length: an image as long as its sections reach is the image in memory
    Bytes f = small_image();
    put_le(f, 0x58 + 96 + 3 * 8, 0, 4);
    put_le(f, 0x58 + 96 + 3 * 8 + 4, 0, 4);
    const Image image = Image::parse(f);
    CB_CHECK(!image.memory_layout());   // 0x700 bytes: shorter than .text's end (0x3080) in memory
    // the bl's target and nothing else (the table named A and B)
    const std::vector<Address> functions(image.functions().begin(), image.functions().end());
    CB_CHECK((functions == std::vector<Address>{kText + 0x20}));
    CB_CHECK(image.extents().empty());
});

CB_TEST("formats.powerpc_image.an_image_without_code", [] {
    Bytes f = small_image();
    put_le(f, 0x138 + 40 * 2 + 36, 0x40000040, 4);   // .text is no longer executable...
    put_text(f, 0x138 + 40 * 2, ".dat");             // ...nor called .text
    const Image image = Image::parse(f);
    CB_CHECK(!image.has_code());
    CB_CHECK(image.functions().empty());
    CB_CHECK(image.imports().empty());
    CB_CHECK_EQ(image.code_begin(), 0u);
    CB_CHECK_EQ(image.code_end(), 0u);
    CB_CHECK_EQ(image.extents().size(), 2u);   // the table is still the image's
});

}  // namespace
}  // namespace cb::formats::powerpc_image

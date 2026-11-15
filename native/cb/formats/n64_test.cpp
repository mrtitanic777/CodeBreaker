// cb-unity: alone (file-local names that other files in a batch also use)
// Nintendo 64 ROMs: the three byte orders read as one, the header field by field, the boot code, where the boot code loads the program, function
// starts, the checksums (against real homebrew ROMs when they are on this machine) and how the file is recognised.
#include "cb/formats/n64.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/detect.hpp"
#include "cb/source/isa_r4300.hpp"
#include "cb/source/n64_test_rom.hpp"

namespace n = cb::formats::n64;
namespace nt = cb::source::n64_test;
using cb::Bytes;
using std::string;

CB_TEST("formats.n64.the_three_byte_orders_are_one_rom", [] {
    const Bytes z64 = nt::test_rom(n::ByteOrder::Z64);
    const Bytes v64 = nt::test_rom(n::ByteOrder::V64);
    const Bytes n64 = nt::test_rom(n::ByteOrder::N64);
    CB_CHECK_EQ(static_cast<int>(z64[0]), 0x80);
    CB_CHECK_EQ(static_cast<int>(z64[1]), 0x37);
    CB_CHECK_EQ(static_cast<int>(v64[0]), 0x37);   // every pair of bytes swapped
    CB_CHECK_EQ(static_cast<int>(v64[1]), 0x80);
    CB_CHECK_EQ(static_cast<int>(n64[0]), 0x40);   // every word reversed
    CB_CHECK_EQ(static_cast<int>(n64[3]), 0x80);
    CB_CHECK(*n::order_of(z64) == n::ByteOrder::Z64);
    CB_CHECK(*n::order_of(v64) == n::ByteOrder::V64);
    CB_CHECK(*n::order_of(n64) == n::ByteOrder::N64);
    const n::Rom a = n::parse(z64), b = n::parse(v64), c = n::parse(n64);
    CB_CHECK(a.data == b.data);
    CB_CHECK(a.data == c.data);
    CB_CHECK(a.order == n::ByteOrder::Z64 && b.order == n::ByteOrder::V64 && c.order == n::ByteOrder::N64);
    // the file is what was read, in the order it came in
    CB_CHECK(a.in_order() == z64);
    CB_CHECK(b.in_order() == v64);
    CB_CHECK(c.in_order() == n64);
    CB_CHECK(n::Rom::convert(v64, n::ByteOrder::V64, n::ByteOrder::N64) == n64);
    CB_CHECK(n::Rom::convert(n64, n::ByteOrder::N64, n::ByteOrder::V64) == v64);
    CB_CHECK(!n::order_of(cb::ByteView(z64).first(3)));
    CB_CHECK(!n::order_of(Bytes{0, 1, 2, 3}));
});

CB_TEST("formats.n64.the_header_is_read_field_by_field", [] {
    const n::Rom rom = n::parse(nt::test_rom());
    const n::Header& h = rom.header;
    CB_CHECK_EQ(h.pi_config, 0x80371240u);
    CB_CHECK_EQ(h.clock_rate, 0xFu);
    CB_CHECK_EQ(h.entry, 0x80001000u);
    CB_CHECK_EQ(h.release, 0x1444u);
    CB_CHECK_EQ(h.crc1, 0x11223344u);
    CB_CHECK_EQ(h.crc2, 0x55667788u);
    CB_CHECK_EQ(h.name, string("N64 LANE TEST"));
    CB_CHECK_EQ(h.category, 'N');
    CB_CHECK_EQ(h.game_code, string("NT"));
    CB_CHECK_EQ(h.country, 'E');
    CB_CHECK_EQ(static_cast<int>(h.version), 0);
    // the boot code is $40-$FFF (the stand-in is no chip's: unknown), and the program starts at $1000
    CB_CHECK_EQ(rom.boot_code().size(), std::size_t{0xFC0});
    CB_CHECK(rom.cic == n::Cic::Unknown);
    CB_CHECK_EQ(std::string(n::cic_text(rom.cic)), string("unknown boot code"));
    CB_CHECK_EQ(rom.load_address(), 0x80001000u);
    CB_CHECK(!rom.program().empty());
    CB_CHECK_EQ(static_cast<int>(rom.program()[0]), 0x67);   // daddiu $sp, $sp, -32
    // a ROM shorter than the megabyte the checksums cover has none: nothing is claimed
    CB_CHECK(!rom.checksums());
    CB_CHECK(!rom.checksums_match());
});

CB_TEST("formats.n64.the_boot_code_copies_the_first_megabyte_to_the_entry_point", [] {
    const n::Rom rom = n::parse(nt::test_rom(n::ByteOrder::Z64, 0x300000));
    CB_CHECK_EQ(rom.data.size(), std::size_t{0x101000 + 0x300000 + 4});
    CB_CHECK_EQ(rom.program().size(), n::kLoadSize);                   // what IPL3 copies: 1 MB
    const cb::Bytes ram = rom.memory();
    CB_CHECK_EQ(ram.size(), n::kRdramSize);
    CB_CHECK(std::equal(rom.program().begin(), rom.program().end(), ram.begin() + 0x1000));   // at the entry point's physical address
    CB_CHECK_EQ(static_cast<int>(ram[0x100FFF + 1]), 0);               // nothing past the megabyte
    CB_CHECK_EQ(static_cast<int>(ram[0xFFF]), 0);                      // nothing before the entry point (but the OS words the boot code leaves)
    CB_CHECK_EQ(static_cast<int>(ram[0x303]), 1);                      // osTvType: NTSC (the country is E)
    CB_CHECK_EQ(static_cast<int>(ram[0x319]), 0x80);                   // osMemSize: $800000
    // a ROM shorter than a megabyte copies what there is
    const n::Rom small = n::parse(nt::test_rom());
    CB_CHECK(small.program().size() < 0x1000);
});

CB_TEST("formats.n64.functions_are_what_the_entry_point_reaches", [] {
    const n::Rom rom = n::parse(nt::test_rom());
    const std::vector<std::uint32_t> found = rom.functions(cb::source::isa::r4300_decoder());
    // `entry` and what it calls (print); helper and handler are only in the table, which a scan of calls does not see
    CB_CHECK_EQ(found.size(), std::size_t{2});
    CB_CHECK_EQ(found[0], 0x80001000u);
    CB_CHECK(found[1] > found[0] && found[1] < 0x80001100u);
});

CB_TEST("formats.n64.a_file_is_recognised_by_its_first_word_and_size", [] {
    cb::fs::ScopedTempDir dir("cb_n64_probe");
    for (const char* name : {"a.z64", "b.v64", "c.n64", "no_extension"}) {
        const n::ByteOrder order = name[2] == 'v' ? n::ByteOrder::V64 : name[2] == 'n' ? n::ByteOrder::N64 : n::ByteOrder::Z64;
        cb::fs::write_file(dir.path() / name, nt::test_rom(order));
        CB_CHECK(n::probe(dir.path() / name));
    }
    cb::fs::write_file(dir.path() / "short.z64", Bytes{0x80, 0x37, 0x12, 0x40, 0, 0, 0, 0});   // the word, but not the boot code
    CB_CHECK(!n::probe(dir.path() / "short.z64"));
    cb::fs::write_file(dir.path() / "other.z64", Bytes(0x2000, 0x41));
    CB_CHECK(!n::probe(dir.path() / "other.z64"));
    CB_CHECK(!n::probe(dir.path() / "missing.z64"));
    CB_CHECK_THROWS_AS(cb::FormatError, n::parse(Bytes(0x2000, 0x41)));
    CB_CHECK_THROWS_AS(cb::FormatError, n::parse(Bytes{0x80, 0x37, 0x12, 0x40}));
    // `info` and `reach` take it too: the format is named, with the title and the byte order
    cb::fs::write_file(dir.path() / "game.v64", nt::test_rom(n::ByteOrder::V64));
    const std::vector<cb::formats::detect::Detected> found = cb::formats::detect::identify(dir.path() / "game.v64");
    CB_CHECK(!found.empty());
    CB_CHECK_EQ(found.front().format, string("Nintendo 64 ROM"));
    CB_CHECK_EQ(found.front().family, string("n64"));
    CB_CHECK_EQ(found.front().command, string("reach"));
    CB_CHECK(found.front().detail.starts_with("\"N64 LANE TEST\" (NTE), v64 (byte-swapped)"));
    CB_CHECK(found.front().detail.ends_with("entry 0x80001000"));
});

namespace {

// Real homebrew ROMs (docs/TESTED_SOFTWARE.md), kept outside the repository: skipped where they are not.
std::optional<std::filesystem::path> homebrew(std::string_view name) {
    std::filesystem::path root = cb::testing::beside_checkout("homebrew-samples");
    if (const auto env = cb::fs::get_env("CB_HOMEBREW_SAMPLES"); env && !env->empty()) root = cb::fs::from_text(*env);
    const std::filesystem::path file = root / "n64" / "peterlemon" / std::string(name);
    if (!std::filesystem::is_regular_file(file)) return std::nullopt;
    return file;
}

}  // namespace

CB_TEST("formats.n64.the_checksums_of_real_roms_are_the_ones_the_boot_code_computes", [] {
    for (const char* name : {"HelloWorld_16BPP_HelloWorldCPU320x240_HelloWorldCPU16BPP320X240.N64", "CPUTest_CP1_ADD_CP1ADD.N64", "Initialize_Initialize.N64"}) {
        const auto file = homebrew(name);
        if (!file) continue;   // (not on this machine)
        const n::Rom rom = n::read(*file);
        CB_CHECK(rom.cic == n::Cic::C6102);
        CB_CHECK(rom.checksums_match());
        CB_CHECK_EQ(rom.header.entry, 0x80001000u);
        CB_CHECK_EQ(rom.data.size(), std::size_t{0x101000});
    }
});

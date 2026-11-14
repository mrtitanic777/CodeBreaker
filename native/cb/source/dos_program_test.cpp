// Tests of cb/source/dos_program: reading a built MZ file as it loads, and the loop's first round with no assembler to hand (the
// rounds with OpenWatcom's wasm and wlink are what `codebreaker dos-source` runs; they are not run here).
//
// Design notes:
//   * a build that is not an MZ file, a header that does not describe the file and a relocation table past the end are errors
//     (FormatError / TruncatedError);
//   * the build description <stem>.build.json is written by nlohmann (indent 1, keys in a fixed order);
//   * the layout of the original file around the linked program (packing, header data, overlay) is not done here.

#include "cb/source/dos_program.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::source::dos_program;
namespace dos = cb::formats::dos;

namespace {

// An MZ file: a header of two paragraphs holding one relocation (offset 1 of segment 0), then a 16-byte load module.
Bytes mz_file() {
    Bytes f(0x30, 0);
    const auto put = [&](std::size_t at, std::uint32_t v) {
        f[at] = static_cast<std::uint8_t>(v & 0xFF);
        f[at + 1] = static_cast<std::uint8_t>(v >> 8);
    };
    f[0] = 'M';
    f[1] = 'Z';
    put(2, 0x30);    // bytes in the last page
    put(4, 1);       // pages
    put(6, 1);       // relocations
    put(8, 2);       // header paragraphs
    put(0xE, 1);     // ss
    put(0x10, 0x80); // sp
    put(0x14, 0);    // ip
    put(0x16, 0);    // cs
    put(0x18, 0x1C); // the table
    put(0x1C, 1);    // offset
    put(0x1E, 0);    // segment
    f[0x20] = 0xB8;  // mov ax, 0010h
    f[0x21] = 0x10;
    f[0x22] = 0x00;
    f[0x23] = 0xC3;
    return f;
}

}  // namespace

CB_TEST("source.dos_program.a_built_mz_file_as_it_loads", [] {
    fs::ScopedTempDir tmp("cb-dosprog");
    fs::write_file(tmp.path() / "a.exe", mz_file());
    const LoadModule m = read_load_module(tmp.path() / "a.exe", true);
    CB_CHECK_EQ(m.image.size(), std::size_t{0x10});
    CB_CHECK((m.relocations == std::set<std::uint32_t>{1}));
    CB_CHECK_EQ(m.image[1], std::uint8_t{0x10});   // 0010h and the load segment 1000h
    CB_CHECK_EQ(m.image[2], std::uint8_t{0x10});
    CB_CHECK(m.registers.has_value());
    CB_CHECK((*m.registers == std::array<std::uint32_t, 4>{0x1000, 0, 0x1001, 0x80}));
    // a file that is only bytes is its own bytes
    const LoadModule flat = read_load_module(tmp.path() / "a.exe", false);
    CB_CHECK_EQ(flat.image.size(), std::size_t{0x30});
    CB_CHECK(flat.relocations.empty() && !flat.registers.has_value());
});

CB_TEST("source.dos_program.a_file_that_is_not_what_was_built_is_an_error", [] {
    fs::ScopedTempDir tmp("cb-dosprog");
    fs::write_text(tmp.path() / "not.exe", "this is not an MZ file at all, not even close...");
    CB_CHECK_THROWS_AS(cb::FormatError, (void)read_load_module(tmp.path() / "not.exe", true));
    Bytes table_past_the_end = mz_file();
    table_past_the_end[0x18] = 0x2E;   // the relocation table starts two bytes before the end: its entry is cut
    fs::write_file(tmp.path() / "cut.exe", table_past_the_end);
    CB_CHECK_THROWS_AS(cb::TruncatedError, (void)read_load_module(tmp.path() / "cut.exe", true));
    Bytes header_bigger = mz_file();
    header_bigger[8] = 9;   // a header of 144 bytes in a file of 48
    fs::write_file(tmp.path() / "big.exe", header_bigger);
    CB_CHECK_THROWS_AS(cb::FormatError, (void)read_load_module(tmp.path() / "big.exe", true));
    CB_CHECK_THROWS_AS(cb::IoError, (void)read_load_module(tmp.path() / "absent.exe", true));
});

CB_TEST("source.dos_program.without_an_assembler_the_first_round_says_so", [] {
    fs::ScopedTempDir tmp("cb-dosprog");
    const auto services = symbols::dos_interrupts::ServiceTable::load(std::filesystem::path(CB_SOURCE_DIR).parent_path() / "data" / "symbols" / "dos_interrupts.json");
    const platforms::Decoder decoder = platforms::Decoder::open(platforms::Arch::X86, 16, false);
    Bytes image(0x20, 0);
    image[0] = 0xEB;   // jmp short 4
    image[1] = 0x02;
    image[4] = 0xC3;
    dos::Program p;
    p.kind = dos::Kind::Com;
    p.base = dos::kLoadSegment * 16;
    p.spans.push_back({p.base, 0x20, "image"});
    p.entries.push_back({p.base, "start", dos::Entry::Call::Near});
    p.image = std::move(image);
    p.image.insert(p.image.begin(), 0x100, 0);   // the program prefix
    p.spans[0].length = static_cast<std::uint32_t>(p.image.size());
    p.entries[0].address = p.base + 0x100;
    std::vector<std::string> said;
    DosProgramOptions options;   // no tools: wasm is an empty path
    options.log = [&](const std::string& s) { said.push_back(s); };
    const DosProgramResult r = write_dos_program(p, "tiny", "TINY.COM", tmp.path() / "out", options, services, decoder);
    CB_CHECK(!r.linked && !r.same);
    CB_CHECK_EQ(r.rounds, 1);
    CB_CHECK_EQ(r.kind, "com");
    CB_CHECK(r.notes.size() == 1 && r.notes[0].starts_with("assembler: "));
    CB_CHECK(r.flat);
    CB_CHECK(std::filesystem::is_regular_file(r.source));
    const std::string text = fs::read_text(r.source);
    CB_CHECK(text.find("\r\n") != std::string::npos);   // as the assembler wants its lines
    CB_CHECK(text.find("jmp    short") != std::string::npos);
    CB_CHECK(!r.functions.empty());
    CB_CHECK_EQ(r.functions.begin()->first, std::uint32_t{0x100});
});

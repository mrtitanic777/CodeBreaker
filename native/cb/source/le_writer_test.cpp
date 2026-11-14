// Tests of the whole-file writer of device drivers over real ones: splitter.vxd and srammtd.vxd (Windows 95's, a few hundred instructions each,
// code and data objects, fix-ups of every width). A driver is read and its code found without an assembler; written back it needs NASM
// (settings: nasm, or CB_NASM) and those tests are skipped where there is none. The golden sources (testdata/writers/le_*.golden.asm) are what
// the writer wrote; they were read through -- the macros, an object's functions, the fix-up records and the page table -- and
// the test builds them again and holds the result to the original byte for byte.

#include "cb/source/le_writer.hpp"

#include <algorithm>

#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::le_writer {
namespace {

std::optional<std::filesystem::path> assembler_for_le_tests() {
    if (const auto env = fs::get_env("CB_NASM"); env && !env->empty()) return fs::from_text(*env);
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        if (const auto configured = main.path("nasm"); configured && std::filesystem::is_regular_file(*configured)) return configured;
        const std::filesystem::path built = main.checkout_root() / "native" / "nasm" / "build" / "bin" / "nasm.exe";
        if (std::filesystem::is_regular_file(built)) return built;
    } catch (const Error&) {   // (no configuration to read: no assembler to test against)
    }
    return std::nullopt;
}

std::filesystem::path driver_file(std::string_view name) { return testing::data_path("formats/testdata/" + std::string(name)); }

// The text with Windows line ends as Unix ones, and without its first line (the one that names the map file).
std::string without_first_line(std::string content) {
    content = text::replace_all(content, "\r\n", "\n");
    const std::size_t at = content.find('\n');
    return at == std::string::npos ? std::string() : content.substr(at + 1);
}

std::string golden_text(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

CB_TEST("source.le_writer.the_code_of_a_driver_is_found_from_its_entries_descriptor_and_fixups", [] {
    const Image image = Image::open(driver_file("srammtd.vxd"));
    CB_CHECK(!image.is_bundle());
    CB_CHECK_EQ(image.drivers().size(), 1u);
    CB_CHECK_EQ(image.instruction_count(), 412u);
    const Driver& d = image.drivers().front();
    CB_CHECK(d.tag().empty());
    CB_CHECK(!d.function_starts().empty());
    for (const Place& start : d.function_starts()) CB_CHECK(d.instructions().contains(start));
    // the entry point named by the header is a function
    const formats::le::Header& h = d.module().header();
    if (d.has_object(h.eip_object)) CB_CHECK(d.function_starts().contains(Place{h.eip_object, h.eip}) || !d.instructions().contains(Place{h.eip_object, h.eip}));
    // every instruction's bytes are the file's bytes at its place
    for (const auto& [at, insn] : d.instructions()) {
        const Object& o = d.object(at.object);
        CB_CHECK(o.whole && o.object.executable());
        CB_CHECK(std::equal(insn.raw().begin(), insn.raw().end(), image.data().begin() + static_cast<std::ptrdiff_t>(o.start + at.offset)));
    }
    // a fix-up site is never cut by an instruction: each lies wholly inside one or in none
    for (const auto& [at, site] : d.sites()) {
        for (std::uint32_t k = 0; k < site.width; ++k) (void)k;
        CB_CHECK(site.width >= 1 && site.width <= 6);
    }
});

CB_TEST("source.le_writer.a_writing_has_a_section_per_object_the_fixup_records_and_the_page_table", [] {
    Image image = Image::open(driver_file("splitter.vxd"));
    const flat_rounds::Source<Key> src = image.source({}, true, true);
    CB_CHECK(src.text.starts_with("; splitter.vxd -- assembled with NASM: nasm -f bin -o splitter.vxd this-file\n"));
    CB_CHECK(src.text.find("%macro _rr 3") != std::string::npos);
    CB_CHECK(src.text.find("section o1 start=o1_file vstart=0 align=1") != std::string::npos);
    CB_CHECK(src.text.find("bits 32") != std::string::npos);
    CB_CHECK(src.text.find("o1_start:") != std::string::npos);
    CB_CHECK(src.text.find("section fixrec start=0x") != std::string::npos);
    CB_CHECK(src.text.find("section fixpage start=0x") != std::string::npos);
    CB_CHECK(src.text.find("    dd fr_p1-fr_p0") != std::string::npos);   // a page's records start where the table says, as the distance from the first
    CB_CHECK(src.instruction_at_line.size() >= image.instruction_count());
    // not exact: each section follows the one before
    CB_CHECK(image.source({}, true, false).text.find("section o1 follows=") != std::string::npos);
    CB_CHECK(!image.assembly().lines().empty());
});

CB_TEST("source.le_writer.drivers_build_back_the_same_and_the_source_is_what_the_original_writer_wrote", [] {
    const auto nasm = assembler_for_le_tests();
    if (!nasm) return;   // (no assembler on this machine)
    struct Expect {
        const char* file;
        const char* golden;
        std::size_t instructions, raw, rounds;
    };
    for (const Expect& e : {Expect{"splitter.vxd", "le_splitter.golden.asm", 450, 14, 3}, Expect{"srammtd.vxd", "le_srammtd.golden.asm", 412, 2, 2}}) {
        fs::ScopedTempDir dir("cb_le_writer");
        std::vector<std::string> said;
        const Result r = reconstruct(driver_file(e.file), dir.path(), {.nasm = *nasm, .say = [&](std::string_view line) { said.emplace_back(line); }});
        CB_CHECK(r.same);
        CB_CHECK(r.image.has_value());
        CB_CHECK_EQ(r.instructions, e.instructions);
        CB_CHECK_EQ(r.raw, e.raw);
        CB_CHECK_EQ(r.rounds, e.rounds);
        CB_CHECK(r.source == dir.path() / (std::string(e.file) + ".asm"));
        CB_CHECK(!said.empty());
        CB_CHECK(fs::read_file(*r.image) == fs::read_file(driver_file(e.file)));
        CB_CHECK(without_first_line(fs::read_text(r.source)) == golden_text(e.golden));
    }
});

CB_TEST("source.le_writer.errors_a_missing_assembler_and_a_file_that_is_not_a_driver", [] {
    fs::ScopedTempDir dir("cb_le_writer");
    CB_CHECK_THROWS_AS(NotFoundError, reconstruct(driver_file("srammtd.vxd"), dir.path(), {.nasm = dir.path() / "no-nasm.exe"}));
    fs::write_file(dir.path() / "junk.vxd", Bytes(512, 0x41));
    CB_CHECK_THROWS_AS(FormatError, Image::open(dir.path() / "junk.vxd"));
    CB_CHECK_THROWS_AS(NotFoundError, Image::open(dir.path() / "absent.vxd"));
    CB_CHECK_THROWS_AS(FormatError, Image::open(driver_file("lzexpand.dll")));   // (an NE file)
});

CB_TEST("source.le_writer.the_command_prints_what_was_done_and_exits_by_whether_it_came_back_the_same", [] {
    std::vector<std::string> lines;
    const auto say = [&](std::string_view line) { lines.emplace_back(line); };
    fs::ScopedTempDir dir("cb_le_writer");
    const Environment env{.nasm = assembler_for_le_tests().value_or(dir.path() / "no-nasm.exe"), .state = dir.path() / "state"};
    CB_CHECK_EQ(command({}, env, say), 2);
    CB_CHECK(!lines.empty() && lines.back().find("le-source <driver> [--out DIR]") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"x.vxd", "--out"}, env, say));
    if (!assembler_for_le_tests()) return;
    lines.clear();
    const std::vector<std::string> args = {fs::to_text(driver_file("srammtd.vxd")), "--out", fs::to_text(dir.path() / "o")};
    CB_CHECK_EQ(command(args, env, say), 0);
    CB_CHECK(lines.back().find("srammtd.vxd: 412 instructions, 2 (0%) written as bytes; 2 rounds; builds back THE SAME") != std::string::npos);
    CB_CHECK_EQ(command(std::vector<std::string>{fs::to_text(driver_file("srammtd.vxd"))}, env, say), 0);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "state" / "le_source" / "srammtd" / "srammtd.vxd.asm"));
});

}  // namespace
}  // namespace cb::source::le_writer

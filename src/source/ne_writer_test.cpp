// Tests of the whole-file writer of 16-bit programs over a real one: lzexpand.dll (Windows 95's LZ decompressor, six code segments, 2242
// instructions, relocation chains of every kind). The program is read and its code found without an assembler; written back it is assembled by x86asm,
// in this process. The golden source (testdata/writers/ne_lzexpand.golden.asm)
// is what the writer wrote when it was recorded; it was read through -- the head, a segment's functions, a
// relocation chain, the segment table, the relocation table -- and, more to the point, the test builds it again and holds it to the
// original byte for byte.

#include "source/ne_writer.hpp"

#include <algorithm>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::ne_writer {
namespace {

std::filesystem::path lzexpand() { return testing::data_path("formats/testdata/lzexpand.dll"); }

// The text with Windows line ends as Unix ones, and without its first line (the one that names the map file).
std::string without_first_line(std::string content) {
    content = text::replace_all(content, "\r\n", "\n");
    const std::size_t at = content.find('\n');
    return at == std::string::npos ? std::string() : content.substr(at + 1);
}

std::string golden_text(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

CB_TEST("source.ne_writer.the_code_of_a_program_is_found_from_its_entries_and_relocations", [] {
    const Image image = Image::open(lzexpand());
    CB_CHECK_EQ(image.instruction_count(), 2242u);
    CB_CHECK(!image.function_starts().empty());
    for (const Place& start : image.function_starts()) CB_CHECK(image.instructions().contains(start));   // a function starts where an instruction is
    // every entry point the entry table names that holds code is a function
    const formats::ne::Module& m = image.module();
    std::size_t entries = 0;
    for (const formats::ne::Entry& e : m.entries()) {
        if (e.kind == formats::ne::Entry::Kind::Unused || !image.instructions().contains(Place{e.segment, e.offset})) continue;
        CB_CHECK(image.function_starts().contains(Place{e.segment, e.offset}));
        ++entries;
    }
    CB_CHECK(entries > 0);
    // an instruction's bytes are the file's bytes at its place
    for (const formats::ne::Segment& s : m.segments()) {
        if (s.file_offset == 0) continue;
        for (const auto& [at, insn] : image.instructions()) {
            if (at.segment != s.index) continue;
            CB_CHECK(std::equal(insn.raw().begin(), insn.raw().end(), m.data().begin() + static_cast<std::ptrdiff_t>(s.file_offset + at.offset)));
        }
    }
});

CB_TEST("source.ne_writer.a_writing_labels_what_is_pointed_at_and_starts_each_function_as_a_fragment", [] {
    Image image = Image::open(lzexpand());
    const flat_rounds::Source<Place> src = image.source({}, true, false);
    CB_CHECK(src.text.starts_with("; lzexpand.dll -- assembled with NASM: nasm -f bin -o lzexpand.dll this-file\nbits 16\nSHIFT equ 4\n"));
    CB_CHECK(src.text.find("seg1_start_file equ 0x300") != std::string::npos);
    CB_CHECK(src.text.find("section seg1 follows=part0 vstart=0 align=1") != std::string::npos);   // (not exact: each section follows the one before)
    CB_CHECK(src.text.find("seg1_start:") != std::string::npos);
    CB_CHECK(src.text.find("seg6_end:") != std::string::npos);
    // every instruction is on a line of its own that the line map names (the rounds read the assembler's complaints by it)
    CB_CHECK(src.instruction_at_line.size() >= image.instruction_count());
    // exact: the sections stand where the file has them
    const flat_rounds::Source<Place> exact = image.source({}, false, true);
    CB_CHECK(exact.text.find("section seg1 start=seg1_start_file vstart=0 align=1") != std::string::npos);
    // the L2 tree of the last writing: the head, the part outside the segments, then a fragment per segment and per function
    CB_CHECK(!image.assembly().lines().empty());
});

CB_TEST("source.ne_writer.a_place_a_relocation_patches_inside_an_instruction_is_written_as_bytes", [] {
    Image image = Image::open(lzexpand());
    const std::string text = image.source({}, true, false).text;
    // `lcall` through a relocation: its offset and selector are the chain's links, its mnemonic only beside them
    CB_CHECK(text.find("; lcall") != std::string::npos);
    CB_CHECK(text.find("dw 0xffff") != std::string::npos);   // the end of a relocation chain
});

CB_TEST("source.ne_writer.the_program_builds_back_the_same_and_the_source_is_what_the_original_writer_wrote", [] {
    fs::ScopedTempDir dir("cb_ne_writer");
    std::vector<std::string> said;
    const Result r = reconstruct(lzexpand(), dir.path(), {.say = [&](std::string_view line) { said.emplace_back(line); }});
    CB_CHECK(r.same);
    CB_CHECK(r.image.has_value());
    CB_CHECK_EQ(r.instructions, 2242u);
    CB_CHECK_EQ(r.raw, 98u);
    CB_CHECK_EQ(r.rounds, 2u);
    CB_CHECK(r.source == dir.path() / "lzexpand.dll.asm");
    CB_CHECK(!said.empty());
    CB_CHECK(fs::read_file(*r.image) == fs::read_file(lzexpand()));
    CB_CHECK(without_first_line(fs::read_text(r.source)) == golden_text("ne_lzexpand.golden.asm"));
});

CB_TEST("source.ne_writer.a_stub_given_as_bytes_is_included_in_front_of_the_header", [] {
    fs::ScopedTempDir dir("cb_ne_writer");
    // the program's DOS stub is what lies before its new header
    const Bytes file = fs::read_file(lzexpand());
    const std::size_t stub_size = Image::open(lzexpand()).module().ne_offset();
    fs::write_file(dir.path() / "stub.bin", ByteView(file).first(stub_size));
    const Result r = reconstruct(lzexpand(), dir.path() / "out", {.stub = dir.path() / "stub.bin"});
    CB_CHECK(r.same);
    CB_CHECK(fs::read_text(r.source).find("incbin \"") != std::string::npos);
    // and KeepStub writes the stub as data
    const Result kept = reconstruct(lzexpand(), dir.path() / "kept", {.stub = KeepStub{}});
    CB_CHECK(kept.same);
    CB_CHECK(fs::read_text(kept.source).find("incbin") == std::string::npos);
});

CB_TEST("source.ne_writer.the_stub_finder_is_asked_with_the_file_and_where_its_header_is", [] {
    fs::ScopedTempDir dir("cb_ne_writer");
    std::size_t asked_size = 0;
    std::uint64_t asked_header = 0;
    const FindStub finder = [&](ByteView file, std::uint64_t header, const std::filesystem::path&, const std::function<void(std::string_view)>&) {
        asked_size = file.size();
        asked_header = header;
        return std::optional<std::filesystem::path>();   // (no source for the stub: its bytes are kept)
    };
    const Result r = reconstruct(lzexpand(), dir.path(), {.find_stub = finder});
    CB_CHECK(r.same);
    CB_CHECK_EQ(asked_size, fs::read_file(lzexpand()).size());
    CB_CHECK_EQ(asked_header, Image::open(lzexpand()).module().ne_offset());
});

CB_TEST("source.ne_writer.errors_a_file_that_is_not_an_ne_program", [] {
    fs::ScopedTempDir dir("cb_ne_writer");
    fs::write_file(dir.path() / "junk.dll", Bytes(512, 0x41));
    CB_CHECK_THROWS_AS(FormatError, Image::open(dir.path() / "junk.dll"));
    CB_CHECK_THROWS_AS(NotFoundError, Image::open(dir.path() / "absent.dll"));
    CB_CHECK_THROWS_AS(FormatError, Image::open(testing::data_path("formats/testdata/comcat.dll")));   // (a PE file)
});

CB_TEST("source.ne_writer.the_command_prints_what_was_done_and_exits_by_whether_it_came_back_the_same", [] {
    std::vector<std::string> lines;
    const auto say = [&](std::string_view line) { lines.emplace_back(line); };
    fs::ScopedTempDir dir("cb_ne_writer");
    Environment env{.state = dir.path() / "state", .find_stub = nullptr};
    // no program named: the usage, and exit 2
    CB_CHECK_EQ(command({}, env, say), 2);
    CB_CHECK(!lines.empty() && lines.back().find("ne-source <program> [--out DIR] [--keep-stub]") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"x.dll", "--out"}, env, say));
    lines.clear();
    const std::vector<std::string> args = {fs::to_text(lzexpand()), "--keep-stub", "--out", fs::to_text(dir.path() / "o")};
    CB_CHECK_EQ(command(args, env, say), 0);
    CB_CHECK(lines.back().find("lzexpand.dll: 2242 instructions, 98 (4%) written as bytes; 2 rounds; builds back THE SAME") != std::string::npos);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "o" / "lzexpand.dll.asm"));
    // with no --out the folder is <state>/ne_source/<name>
    CB_CHECK_EQ(command(std::vector<std::string>{fs::to_text(lzexpand())}, env, say), 0);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "state" / "ne_source" / "lzexpand" / "lzexpand.dll.asm"));
});

}  // namespace
}  // namespace cb::source::ne_writer

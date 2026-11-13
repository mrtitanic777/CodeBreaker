// Tests of the whole-file writer of 32-bit programs over two real ones: lz32.dll and version.dll (Windows 95's LZ decompressor and version
// API, 378 and 612 instructions of code, import tables, base relocations). The program is read and its code found without an assembler;
// written back it needs NASM (settings: nasm, or CB_NASM) and those tests are skipped where there is none. The golden sources
// (testdata/writers/pe_lz32.golden.asm, pe_version.golden.asm) are what the Python writer (`pe-source`) wrote for the same two files; the
// test builds the C++ writer's source again, holds it to the golden text, and -- more to the point -- holds the image it assembles to the
// original byte for byte.

#include "cb/source/pe_writer.hpp"

#include <algorithm>
#include <cstring>
#include <format>

#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace cb::source::pe_writer {
namespace {

std::optional<std::filesystem::path> assembler_for_pe_tests() {
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

std::filesystem::path pe_lz32() { return testing::data_path("formats/testdata/lz32.dll"); }
std::filesystem::path pe_version() { return testing::data_path("source/testdata/writers/version.dll"); }

// The text with Windows line ends as Unix ones, and without its first line (the one that names the map file).
std::string pe_without_first_line(std::string content) {
    content = text::replace_all(content, "\r\n", "\n");
    const std::size_t at = content.find('\n');
    return at == std::string::npos ? std::string() : content.substr(at + 1);
}

std::string pe_golden_text(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

CB_TEST("source.pe_writer.the_code_of_a_program_is_found_from_its_entry_functions_and_relocations", [] {
    const Image lz32 = Image::open(pe_lz32());
    CB_CHECK_EQ(lz32.instruction_count(), 378u);
    CB_CHECK_EQ(Image::open(pe_version()).instruction_count(), 612u);
    CB_CHECK(!lz32.function_starts().empty());
    for (const Va start : lz32.function_starts()) CB_CHECK(lz32.instructions().contains(start));   // a function starts where an instruction is
    // the entry point is a function; every instruction's bytes are the file's bytes at its place
    CB_CHECK(lz32.function_starts().contains(lz32.image_base() + lz32.pe().header().entry_rva));
    for (const auto& [va, insn] : lz32.instructions()) {
        const auto at = lz32.pe().file_offset(va - lz32.image_base());
        CB_CHECK(at.has_value());
        CB_CHECK(std::equal(insn.raw().begin(), insn.raw().end(), lz32.pe().data().begin() + static_cast<std::ptrdiff_t>(*at)));
    }
    // a relocated place is never cut by an instruction: it lies whole in the displacement or the immediate of the one holding it
    for (const auto& [site, value] : lz32.relocated()) {
        for (std::uint32_t back = 1; back < 4; ++back) {
            const auto start = lz32.instructions().find(site - back);
            if (start != lz32.instructions().end()) CB_CHECK(start->second.size <= back || start->first + start->second.size >= site + 4);
        }
    }
    CB_CHECK(!lz32.relocated().empty());
});

CB_TEST("source.pe_writer.a_writing_labels_what_is_pointed_at_and_names_each_import_and_the_entry", [] {
    Image image = Image::open(pe_lz32());
    const flat_rounds::Source<Va> marked = image.source({}, true);
    const std::string& text = marked.text;
    const Va base = image.image_base();
    CB_CHECK(text.starts_with("; lz32.dll -- assembled with NASM: nasm -f bin -o lz32.dll this file\nbits 32\nIMAGE equ 0x"));
    CB_CHECK(text.find("section hdr start=0 vstart=0x" + text::lower(std::format("{:x}", base)) + " align=1") != std::string::npos);
    CB_CHECK(text.find("; the entry point") != std::string::npos);
    CB_CHECK(text.find("__imp_") != std::string::npos);                      // the import address table's slots are labels
    CB_CHECK(text.find("-IMAGE") != std::string::npos);                       // the import directory's RVAs are expressions
    CB_CHECK(text.find("progbits start=0x") != std::string::npos);
    // every instruction is on a line of its own that the line map names (the rounds read the assembler's complaints by it)
    CB_CHECK(marked.instruction_at_line.size() >= image.instruction_count());
    // the L2 tree of the last writing: a fragment for the headers, one per section and per function
    CB_CHECK(!image.assembly().lines().empty());
    // with nothing marked, a label stands only where something points
    const std::string unmarked = image.source({}, false).text;
    CB_CHECK(unmarked.size() < text.size());
});

CB_TEST("source.pe_writer.an_instruction_that_is_written_as_bytes_keeps_its_mnemonic_beside_it", [] {
    Image image = Image::open(pe_lz32());
    std::set<Va> raw;
    const Va first = image.instructions().begin()->first;
    raw.insert(first);
    const std::string text = image.source(raw, true).text;
    const x86_flat::Insn& insn = image.instructions().begin()->second;
    const std::string expected_comment = "; " + std::string(text::trim(insn.mnemonic + " " + insn.op_str));
    CB_CHECK(text.find("db 0x") != std::string::npos);
    CB_CHECK(text.find(expected_comment) != std::string::npos);
});

CB_TEST("source.pe_writer.the_programs_build_back_the_same_and_the_source_is_what_the_python_writer_wrote", [] {
    const auto nasm = assembler_for_pe_tests();
    if (!nasm) return;   // (no assembler on this machine)
    struct Case {
        std::filesystem::path program;
        std::string golden;
        std::size_t instructions, raw;
    };
    for (const Case& c : {Case{pe_lz32(), "pe_lz32.golden.asm", 378, 2}, Case{pe_version(), "pe_version.golden.asm", 612, 4}}) {
        fs::ScopedTempDir dir("cb_pe_writer");
        std::vector<std::string> said;
        const Result r = reconstruct(c.program, dir.path(), {.nasm = *nasm, .say = [&](std::string_view line) { said.emplace_back(line); }});
        CB_CHECK(r.same);
        CB_CHECK(r.image.has_value());
        CB_CHECK_EQ(r.instructions, c.instructions);
        CB_CHECK_EQ(r.raw, c.raw);
        CB_CHECK_EQ(r.rounds, 2u);
        CB_CHECK(r.source == dir.path() / (text::lower(fs::to_text(c.program.filename())) + ".asm"));
        CB_CHECK(!said.empty());
        CB_CHECK(fs::read_file(*r.image) == fs::read_file(c.program));
        CB_CHECK(pe_without_first_line(fs::read_text(r.source)) == pe_golden_text(c.golden));
    }
});

CB_TEST("source.pe_writer.a_stub_given_as_bytes_is_included_in_front_of_the_header", [] {
    const auto nasm = assembler_for_pe_tests();
    if (!nasm) return;
    fs::ScopedTempDir dir("cb_pe_writer");
    // the program's DOS stub is what lies before its new header
    const Bytes file = fs::read_file(pe_lz32());
    const std::size_t stub_size = Image::open(pe_lz32()).pe().header().e_lfanew;
    fs::write_file(dir.path() / "stub.bin", ByteView(file).first(stub_size));
    const Result r = reconstruct(pe_lz32(), dir.path() / "out", {.nasm = *nasm, .stub = dir.path() / "stub.bin"});
    CB_CHECK(r.same);
    CB_CHECK(fs::read_text(r.source).find("incbin \"") != std::string::npos);
    // and KeepStub writes the stub as data
    const Result kept = reconstruct(pe_lz32(), dir.path() / "kept", {.nasm = *nasm, .stub = KeepStub{}});
    CB_CHECK(kept.same);
    CB_CHECK(fs::read_text(kept.source).find("incbin") == std::string::npos);
});

CB_TEST("source.pe_writer.the_stub_finder_is_asked_with_the_file_and_where_its_header_is", [] {
    const auto nasm = assembler_for_pe_tests();
    if (!nasm) return;
    fs::ScopedTempDir dir("cb_pe_writer");
    std::size_t asked_size = 0;
    std::uint64_t asked_header = 0;
    const FindStub finder = [&](ByteView file, std::uint64_t header, const std::filesystem::path&, const std::function<void(std::string_view)>&) {
        asked_size = file.size();
        asked_header = header;
        return std::optional<std::filesystem::path>();   // (no source for the stub: its bytes are kept)
    };
    const Result r = reconstruct(pe_lz32(), dir.path(), {.nasm = *nasm, .find_stub = finder});
    CB_CHECK(r.same);
    CB_CHECK_EQ(asked_size, fs::read_file(pe_lz32()).size());
    CB_CHECK_EQ(asked_header, Image::open(pe_lz32()).pe().header().e_lfanew);
});

CB_TEST("source.pe_writer.errors_a_missing_assembler_and_files_that_are_not_pe_programs", [] {
    fs::ScopedTempDir dir("cb_pe_writer");
    CB_CHECK_THROWS_AS(NotFoundError, reconstruct(pe_lz32(), dir.path(), {.nasm = dir.path() / "no-nasm.exe"}));
    fs::write_file(dir.path() / "junk.dll", Bytes(512, 0x41));
    CB_CHECK_THROWS_AS(FormatError, Image::open(dir.path() / "junk.dll"));
    CB_CHECK_THROWS_AS(NotFoundError, Image::open(dir.path() / "absent.dll"));
    CB_CHECK_THROWS_AS(FormatError, Image::open(testing::data_path("formats/testdata/lzexpand.dll")));   // (an NE file)
    // damaged: the headers cut short, and a table that lies about where it is
    const Bytes file = fs::read_file(pe_lz32());
    fs::write_file(dir.path() / "short.dll", ByteView(file).first(0x90));
    CB_CHECK_THROWS_AS(FormatError, Image::open(dir.path() / "short.dll"));
});

CB_TEST("source.pe_writer.the_command_prints_what_was_done_and_exits_by_whether_it_came_back_the_same", [] {
    std::vector<std::string> lines;
    const auto say = [&](std::string_view line) { lines.emplace_back(line); };
    fs::ScopedTempDir dir("cb_pe_writer");
    Environment env{.nasm = assembler_for_pe_tests().value_or(dir.path() / "no-nasm.exe"), .state = dir.path() / "state", .find_stub = nullptr};
    // no program named: the usage, and exit 2
    CB_CHECK_EQ(command({}, env, say), 2);
    CB_CHECK(!lines.empty() && lines.back().find("pe-source <program> [--out DIR] [--stub SOURCE.ASM] [--keep-stub]") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"x.dll", "--out"}, env, say));
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"x.dll", "--stub"}, env, say));
    if (!assembler_for_pe_tests()) return;
    lines.clear();
    const std::vector<std::string> args = {fs::to_text(pe_lz32()), "--keep-stub", "--out", fs::to_text(dir.path() / "o")};
    CB_CHECK_EQ(command(args, env, say), 0);
    CB_CHECK(lines.back().find("lz32.dll: 378 instructions, 2 (0%) written as bytes; 2 rounds; builds back THE SAME") != std::string::npos);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "o" / "lz32.dll.asm"));
    // with no --out the folder is <state>/pe_source/<name>
    CB_CHECK_EQ(command(std::vector<std::string>{fs::to_text(pe_lz32())}, env, say), 0);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "state" / "pe_source" / "lz32" / "lz32.dll.asm"));
    // --stub FILE is the assembled stub to include
    const Bytes file = fs::read_file(pe_lz32());
    fs::write_file(dir.path() / "stub.bin", ByteView(file).first(Image::open(pe_lz32()).pe().header().e_lfanew));
    const std::vector<std::string> with_stub = {fs::to_text(pe_lz32()), "--stub", fs::to_text(dir.path() / "stub.bin"), "--out", fs::to_text(dir.path() / "s")};
    CB_CHECK_EQ(command(with_stub, env, say), 0);
    CB_CHECK(fs::read_text(dir.path() / "s" / "lz32.dll.asm").find("incbin") != std::string::npos);
});


// A 32-bit program of one section (`code` at its entry, image base 0x400000) built by hand: only what the reader needs.
Bytes pe_with_code(ByteView code) {
    Bytes f(0x400, 0);
    const auto put = [&](std::size_t at, std::uint32_t v, int n) {
        for (int i = 0; i < n; ++i) f[at + static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(v >> (8 * i));
    };
    put(0, 0x5A4D, 2);
    put(0x3C, 0x40, 4);                       // e_lfanew
    put(0x40, 0x4550, 4);                     // the PE signature
    put(0x44, 0x14C, 2);                      // i386
    put(0x46, 1, 2);                          // one section
    put(0x54, 0xE0, 2);                       // optional header size
    put(0x56, 0x2102, 2);                     // a DLL, 32-bit
    put(0x58, 0x10B, 2);
    put(0x68, 0x1000, 4);                     // entry
    put(0x74, 0x400000, 4);                   // image base
    put(0x78, 0x1000, 4);                     // section alignment
    put(0x7C, 0x200, 4);                      // file alignment
    put(0x88, 4, 2);                          // subsystem version
    put(0x90, 0x2000, 4);                     // size of image
    put(0x94, 0x200, 4);                      // size of headers
    put(0x9C, 2, 2);                          // subsystem
    put(0xB4, 16, 4);                         // data directories
    const std::size_t sec = 0x58 + 0xE0;
    std::memcpy(&f[sec], ".text", 5);
    put(sec + 8, static_cast<std::uint32_t>(code.size()), 4);
    put(sec + 12, 0x1000, 4);
    put(sec + 16, 0x200, 4);
    put(sec + 20, 0x200, 4);
    put(sec + 36, 0x60000020, 4);
    std::copy(code.begin(), code.end(), f.begin() + 0x200);
    return f;
}

CB_TEST("source.pe_writer.a_scaled_index_is_written_nosplit_so_the_assembler_keeps_it_one_register", [] {
    // mov eax, [ecx*4 + 0x10]; mov edx, fs:[ebx*8]; ret
    const Bytes code = {0x8B, 0x04, 0x8D, 0x10, 0x00, 0x00, 0x00, 0x64, 0x8B, 0x14, 0xDD, 0x00, 0x00, 0x00, 0x00, 0xC3};
    Image image(formats::pe::Image::parse(pe_with_code(code), "idx.dll"), "idx.dll");
    CB_CHECK_EQ(image.instruction_count(), 3u);
    const std::string text = image.source({}, true).text;
    CB_CHECK(text.find("mov eax, dword [nosplit ecx*4 + 0x10]") != std::string::npos);
    CB_CHECK(text.find("[fs:nosplit ebx*8") != std::string::npos);
    for (const char c : text) CB_CHECK(c >= 0x20 || c == '\n' || c == '\r' || c == '\t');   // (the Python once wrote control characters in the replacement)
});

}  // namespace
}  // namespace cb::source::pe_writer
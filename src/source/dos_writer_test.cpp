// Tests of the DOS writer over real programs of Windows 95: choice.com (a .COM file), find.exe and xcopy.exe (MZ programs LINK /EXEPACK stored)
// and xcopy32.exe (a DOS program with a Windows module behind it). Writing a program back needs OpenWatcom's assembler and linker (settings:
// openwatcom, or CB_OPENWATCOM); those tests are skipped where there is none. The golden sources (testdata/writers/dos_choice.golden.asm,
// dos_xcopy.golden.asm) are the recorded `dos-source` output for the same files; the tests build the C++ writer's source again,
// hold it to the golden text and, more to the point, hold the file it builds to the original byte for byte.

#include "source/dos_writer.hpp"

#include <algorithm>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "source/pe_writer.hpp"

namespace cb::source::dos_writer {
namespace {

// The tools: OpenWatcom from the checkout's settings (or CB_OPENWATCOM), and a file for what is probed that lives as long as the tests.
std::optional<Options> dw_options() {
    static const fs::ScopedTempDir probes_dir("cb_dos_writer_probes");
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        const auto root = main.path("openwatcom");
        const build::watcom::OpenWatcom ow = build::watcom::OpenWatcom::from_settings(root ? fs::to_text(*root) : std::string());
        if (ow.tool("wasm").empty() || ow.tool("wlink").empty()) return std::nullopt;
        return options_for(ow, probes_dir.path() / "assembly_probes.json");
    } catch (const Error&) {   // (no configuration to read: no tools to test with)
        return std::nullopt;
    }
}

std::filesystem::path dw_program(std::string_view name) { return testing::data_path("formats/testdata/" + std::string(name)); }

std::string dw_golden(std::string_view name) {
    return text::replace_all(fs::read_text(testing::data_path("source/testdata/writers/" + std::string(name))), "\r\n", "\n");
}

// A file of the Windows 95 corpus (settings: corpus), or nothing where the corpus is not on this machine.
std::optional<std::filesystem::path> dw_corpus_file(std::string_view relative) {
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        if (const auto root = main.path("corpus")) {
            const std::filesystem::path file = *root / fs::from_text(relative);
            if (std::filesystem::is_regular_file(file)) return file;
        }
    } catch (const Error&) {
    }
    return std::nullopt;
}
std::string dw_lf(const std::filesystem::path& file) { return text::replace_all(fs::read_text(file), "\r\n", "\n"); }

CB_TEST("source.dos_writer.a_com_file_builds_back_the_same_and_its_source_is_the_recorded_source", [] {
    const auto options = dw_options();
    if (!options) return;   // (no OpenWatcom on this machine)
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(dw_program("choice.com"), dir.path(), *options);
    CB_CHECK(r.same);
    CB_CHECK(r.linked);
    CB_CHECK_EQ(r.kind, "com");
    CB_CHECK_EQ(r.instructions, 1062u);
    CB_CHECK_EQ(r.raw, 10u);
    CB_CHECK_EQ(r.data, 2809u);
    CB_CHECK_EQ(r.rounds, 1u);
    CB_CHECK(r.notes.empty());
    CB_CHECK(!r.file.has_value());   // (a .COM file is its own file: there is no header to lay out)
    CB_CHECK(r.image.has_value());
    CB_CHECK(fs::read_file(*r.image) == fs::read_file(dw_program("choice.com")));
    CB_CHECK_EQ(r.sources.size(), 1u);
    CB_CHECK(r.sources.front() == dir.path() / "choice.asm");
    CB_CHECK(dw_lf(r.sources.front()) == dw_golden("dos_choice.golden.asm"));
    CB_CHECK_EQ(r.functions.size(), 22u);
});

CB_TEST("source.dos_writer.an_exepacked_program_is_laid_out_as_the_original_and_made_again_from_its_folder", [] {
    const auto options = dw_options();
    if (!options) return;
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(dw_program("find.exe"), dir.path(), *options);
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.kind, "mz");
    CB_CHECK_EQ(r.instructions, 2098u);
    CB_CHECK_EQ(r.raw, 17u);
    CB_CHECK_EQ(r.data, 7077u);
    CB_CHECK_EQ(r.functions.size(), 61u);
    // the file as the original has it: EXEPACKed again, behind the header's own data
    CB_CHECK(r.file.has_value() && r.file_same.value_or(false));
    CB_CHECK(r.file == dir.path() / "file" / "FIND.EXE");
    CB_CHECK(fs::read_file(*r.file) == fs::read_file(dw_program("find.exe")));
    CB_CHECK(r.facts.has_value() && r.facts->packer == "EXEPACK" && r.facts->words == 8u && r.facts->file == "FIND.EXE");
    CB_CHECK(r.facts->pairs.empty());   // (FIND has no relocation to list)
    for (const char* kept : {"find.hdr", "find.pack", "find.build.json", "find.lnk", "find.asm", "exepack/exepack.bin", "exepack/exepack.asm"}) {
        CB_CHECK(std::filesystem::is_regular_file(dir.path() / kept));
    }
    // the unpacker is a program of its own, built from its source
    CB_CHECK(fs::read_text(dir.path() / "find.pack").starts_with("; the stretches of find.exe that LINK /EXEPACK stored as fills"));
    // the folder alone makes the file again (the original is not read)
    fs::remove_tree(dir.path() / "file");
    const std::filesystem::path again = rebuild(dir.path(), *options);
    CB_CHECK(again == dir.path() / "file" / "FIND.EXE");
    CB_CHECK(fs::read_file(again) == fs::read_file(dw_program("find.exe")));
});

CB_TEST("source.dos_writer.a_pklite_program_is_packed_again_behind_pklites_own_decompressor", [] {
    const auto options = dw_options();
    if (!options) return;
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(dw_program("extract.exe"), dir.path(), *options);
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.instructions, 22423u);
    CB_CHECK(r.file_same.value_or(false));
    CB_CHECK(fs::read_file(*r.file) == fs::read_file(dw_program("extract.exe")));
    CB_CHECK(r.facts.has_value() && r.facts->packer == "PKLITE" && !r.facts->pairs.empty() && r.facts->offsets.empty());
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "pklite" / "pklite.bin"));
    CB_CHECK(std::ranges::find(r.remarks, "the relocation table is written in the original's frames and order") != r.remarks.end());
    CB_CHECK(std::ranges::find(r.remarks, "the memory it asks for is the original's (2976 paragraphs; the rule gives 2981)") != r.remarks.end());
    // the description of a PKLITE program names the order before the memory
    const std::string description = fs::read_text(dir.path() / "extract.build.json");
    CB_CHECK(description.find("\"packer\": \"PKLITE\"") != std::string::npos);
    CB_CHECK(description.find("\"order\"") < description.find("\"min_alloc\""));
    fs::remove_tree(dir.path() / "file");
    CB_CHECK(fs::read_file(rebuild(dir.path(), *options)) == fs::read_file(dw_program("extract.exe")));
});

CB_TEST("source.dos_writer.io_sys_is_four_programs_and_without_tools_none_of_them_is_built", [] {
    const auto kernel = dw_corpus_file("setup/inf/winboot.sys");
    if (!kernel) return;   // (no corpus on this machine)
    fs::ScopedTempDir dir("cb_dos_writer");
    Options options;
    options.probes = dir.path() / "probes.json";
    const Result r = reconstruct(*kernel, dir.path() / "out", options);
    CB_CHECK_EQ(r.kind, "kernel");
    CB_CHECK(!r.linked && !r.same);
    std::vector<std::string> stems;
    for (const Part& part : r.parts) stems.push_back(part.name);
    CB_CHECK((stems == std::vector<std::string>{"msload", "msdos", "sysinit", "exepack"}));
    CB_CHECK_EQ(r.instructions, 47882u);
    CB_CHECK_EQ(r.data, 101517u);
    CB_CHECK_EQ(r.sources.size(), 4u);
    CB_CHECK_EQ(r.notes.size(), 4u);
    for (const std::string& note : r.notes) CB_CHECK(note.find(": assembler: ") != std::string::npos);
    // the fills SYSINIT was stored with are listed beside the sources
    CB_CHECK(fs::read_text(dir.path() / "out" / "sysinit.pack").starts_with("; the stretches of SYSINIT that LINK /EXEPACK stored as fills"));
});

CB_TEST("source.dos_writer.the_relocation_table_is_written_in_the_original_linkers_order_and_the_description_says_how_to_make_it_again", [] {
    const auto options = dw_options();
    if (!options) return;
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(dw_program("xcopy.exe"), dir.path(), *options);
    CB_CHECK(r.same);
    CB_CHECK(r.file_same.value_or(false));
    CB_CHECK_EQ(r.remarks.size(), 1u);
    CB_CHECK_EQ(r.remarks.front(), "the relocation table is written in the original linker's order");
    CB_CHECK(dw_lf(r.sources.front()) == dw_golden("dos_xcopy.golden.asm"));
    // <stem>.build.json, key for key
    CB_CHECK_EQ(fs::read_text(dir.path() / "xcopy.build.json"),
                "{\n \"stem\": \"xcopy\",\n \"product\": \"xcopy.exe\",\n \"flat\": false,\n \"regs\": null,\n \"facts\": {\n  \"file\": \"XCOPY.EXE\",\n  \"packer\": \"EXEPACK\",\n"
                "  \"overlay\": false,\n  \"min_alloc\": 68,\n  \"full_last\": false,\n  \"words\": 8,\n  \"order\": [\n   3566,\n   2396\n  ]\n }\n}");
    // the fills the original's linker stored, one per line (what the unpacker is told)
    CB_CHECK_EQ(dw_lf(dir.path() / "xcopy.pack"),
                "; the stretches of xcopy.exe that LINK /EXEPACK stored as fills: its iterated data\n; (`dup`s), as offset length byte, in hex\n"
                "00AF0 0041 24\n00B38 0008 00\n00B4E 021A 00\n00DDD 000F 00\n00DF3 0141 00\n00F5A 0006 59\n");
});

CB_TEST("source.dos_writer.without_an_assembler_the_program_is_not_built_and_the_notes_say_so", [] {
    fs::ScopedTempDir dir("cb_dos_writer");
    Options options;   // no tools
    options.probes = dir.path() / "probes.json";
    const Result r = reconstruct(dw_program("choice.com"), dir.path() / "out", options);
    CB_CHECK(!r.linked && !r.same);
    CB_CHECK_EQ(r.rounds, 1u);
    CB_CHECK(r.notes.size() == 1 && r.notes[0].starts_with("assembler: "));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "out" / "choice.asm"));
});

CB_TEST("source.dos_writer.files_that_are_not_real_mode_programs_and_combined_files_without_their_other_writer_are_errors", [] {
    fs::ScopedTempDir dir("cb_dos_writer");
    Options options;
    options.probes = dir.path() / "probes.json";
    CB_CHECK_THROWS_AS(FormatError, reconstruct(dw_program("comcat.dll"), dir.path() / "o1", options));   // (a PE file)
    CB_CHECK_THROWS_AS(NotFoundError, reconstruct(dir.path() / "absent.exe", dir.path() / "o2", options));
    // a DOS program with a Windows module behind it needs the module builder
    CB_CHECK_THROWS_AS(UsageError, reconstruct(dw_program("xcopy32.exe"), dir.path() / "o3", options));
    // and a folder that holds no description is not rebuilt
    fs::make_dirs(dir.path() / "empty");
    CB_CHECK_THROWS_AS(NotFoundError, rebuild(dir.path() / "empty", options));
});

CB_TEST("source.dos_writer.a_dos_program_with_a_windows_module_hands_the_module_to_the_builder_and_says_what_it_made", [] {
    const auto options_found = dw_options();
    if (!options_found) return;
    Options options = *options_found;
    std::filesystem::path asked;
    options.build_windows = [&](const std::filesystem::path& program, const std::filesystem::path& outdir) {
        asked = outdir;
        CB_CHECK_EQ(program.filename().string(), "xcopy32.exe");
        WindowsBuild b;
        b.error = "no module builder in this test";
        return b;
    };
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(dw_program("xcopy32.exe"), dir.path(), options);
    CB_CHECK_EQ(r.kind, "dos+windows");
    CB_CHECK(r.same);   // (the DOS program of the file came back)
    CB_CHECK(r.windows.has_value() && !r.windows->linked);
    CB_CHECK(asked == dir.path() / "windows");
    CB_CHECK(!r.notes.empty() && r.notes.back() == "the Windows module did not build: no module builder in this test");
    CB_CHECK(r.file.has_value());   // (the DOS program as a file laid out as the stub is)
});

CB_TEST("source.dos_writer.a_windows_program_with_a_real_dos_stub_has_it_written_from_its_own_source", [] {
    const auto options = dw_options();
    const auto regedit = dw_corpus_file("WINDOWS/regedit.exe");   // (a 32-bit program whose DOS stub is a 38 KB registry editor)
    if (!options || !regedit) return;
    fs::ScopedTempDir dir("cb_dos_writer");
    std::vector<std::string> said;
    source::pe_writer::Options pe;
    pe.find_stub = stub_finder(*options);
    pe.say = [&](std::string_view line) { said.emplace_back(line); };
    const source::pe_writer::Result r = source::pe_writer::reconstruct(*regedit, dir.path(), pe);
    CB_CHECK(r.same);
    CB_CHECK_EQ(r.instructions, 10499u);
    CB_CHECK(!said.empty() && said.front().starts_with("the stub: 38209 bytes, from its own source ("));
    CB_CHECK(fs::read_text(r.source).find("incbin \"") != std::string::npos);
    // the stub's own folder is what the writer of programs made of it: its file is the original's first 38209 bytes
    const Bytes built = fs::read_file(dir.path() / "stub" / "file" / "STUB.EXE");
    const Bytes whole = fs::read_file(*regedit);
    CB_CHECK(built == Bytes(whole.begin(), whole.begin() + 38209));
});
CB_TEST("source.dos_writer.a_driver_with_a_vxd_is_put_together_as_the_original_is", [] {
    const auto options_found = dw_options();
    if (!options_found) return;
    Options options = *options_found;
    // DRVSPACE.BIN: a real-mode driver and a VxD in one file. The VxD is the driver lane's (here the recorded one,
    // testdata/writers/dsvxd.vxd); the driver is written and built here
    std::filesystem::path asked;
    options.write_vxd = [&](const std::filesystem::path& program, const std::filesystem::path& outdir, const Log&) {
        asked = outdir;
        CB_CHECK_EQ(program.filename().string(), "drvspace.bin");
        fs::make_dirs(outdir);
        const std::filesystem::path made = outdir / "dsvxd.vxd";
        std::filesystem::copy_file(testing::data_path("source/testdata/writers/dsvxd.vxd"), made, std::filesystem::copy_options::overwrite_existing);
        VxdPart part;
        part.source = outdir / "dsvxd.asm";
        part.image = made;
        part.linked = true;
        part.same = true;
        part.rounds = 1;
        part.instructions = 241;
        part.raw = 2;
        part.data = 100;
        return part;
    };
    fs::ScopedTempDir dir("cb_dos_writer");
    const Result r = reconstruct(testing::data_path("source/testdata/writers/drvspace.bin"), dir.path(), options);
    CB_CHECK_EQ(r.kind, "driver+vxd");
    CB_CHECK(asked == dir.path() / "vxd");
    CB_CHECK_EQ(r.instructions, 7136u);
    CB_CHECK_EQ(r.raw, 132u);
    CB_CHECK_EQ(r.data, 38379u);
    CB_CHECK_EQ(r.parts.size(), 2u);
    CB_CHECK(r.parts[0].name == "drvspace" && r.parts[0].same && r.parts[1].name == "vxd");
    CB_CHECK_EQ(r.sources.size(), 2u);
    // the driver comes back byte for byte; the file is laid out as LINK lays a VxD out, which the original's tables differ from
    CB_CHECK(!r.same && r.equivalent);
    CB_CHECK_EQ(r.notes.size(), 2u);
    CB_CHECK_EQ(r.notes[0], "the file differs from the original at 0x4 (65783 bytes, original 65271)");
    CB_CHECK(r.notes[1].starts_with("the driver is the original's byte for byte and the VxD the same module"));
    CB_CHECK(r.file.has_value() && *r.file == dir.path() / "file" / "DRVSPACE.BIN");
    const Bytes made = fs::read_file(*r.file);
    CB_CHECK_EQ(made.size(), 65783u);
    // the header's new-header field says where the VxD is: behind the header and the driver
    const Bytes head = fs::read_file(dir.path() / "drvspace.hdr"), driver = fs::read_file(*r.parts[0].image);
    const std::uint32_t at = static_cast<std::uint32_t>(head.size() + driver.size());
    CB_CHECK_EQ(made[0x3C] | (made[0x3D] << 8) | (made[0x3E] << 16), static_cast<int>(at));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "drvspace.tail") && std::filesystem::is_regular_file(dir.path() / "vxd" / "dsvxd.ver"));
    CB_CHECK(dw_lf(r.sources.front()).starts_with("; the real-mode driver of drvspace.bin -- reconstructed from the program image by CodeBreaker.\n"));
    // a driver writer that was not given is a usage error, and one whose VxD did not build leaves the file unmade
    Options none = options;
    none.write_vxd = nullptr;
    CB_CHECK_THROWS_AS(UsageError, reconstruct(testing::data_path("source/testdata/writers/drvspace.bin"), dir.path() / "o2", none));
    options.write_vxd = [&](const std::filesystem::path&, const std::filesystem::path&, const Log&) {
        VxdPart part;
        part.notes = {"the driver kit is not here"};
        return part;
    };
    const Result failed = reconstruct(testing::data_path("source/testdata/writers/drvspace.bin"), dir.path() / "o3", options);
    CB_CHECK(!failed.linked && !failed.same && !failed.file.has_value());
    CB_CHECK_EQ(failed.notes.size(), 1u);
    CB_CHECK_EQ(failed.notes[0], "vxd: the driver kit is not here");
});
CB_TEST("source.dos_writer.the_stub_finder_answers_only_for_a_stub_that_is_a_program_of_its_own", [] {
    fs::ScopedTempDir dir("cb_dos_writer");
    Options options;
    options.probes = dir.path() / "probes.json";
    const FindStub finder = stub_finder(options);
    std::vector<std::string> said;
    const auto say = [&](std::string_view line) { said.emplace_back(line); };
    // the 128-byte "cannot be run in DOS mode" kind is not worth a source
    Bytes small(0x100, 0);
    small[0] = 'M';
    small[1] = 'Z';
    small[2] = 0x80;   // bytes in the last page
    small[4] = 1;      // pages
    CB_CHECK(!finder(small, 0x80, dir.path(), say).has_value());
    // one that is longer than the new header is not a stub (the file says it is longer than where it goes on)
    small[2] = 0;
    small[4] = 2;
    CB_CHECK(!finder(small, 0x100, dir.path(), say).has_value());
    // a program that does not read: its bytes are kept, and the finder says why
    Bytes junk(0x300, 0x90);
    junk[0] = 'M';
    junk[1] = 'Z';
    junk[2] = 0x40;   // 0x240 bytes: a program of its own
    junk[3] = 0;
    junk[4] = 2;
    junk[5] = 0;
    CB_CHECK(!finder(junk, 0x240, dir.path(), say).has_value());
    CB_CHECK(!said.empty());
    CB_CHECK(!finder(Bytes(4, 0), 0x80, dir.path(), say).has_value());   // (too short to say how long it is)
});

CB_TEST("source.dos_writer.the_command_prints_what_was_done_and_exits_by_whether_it_came_back_the_same", [] {
    std::vector<std::string> lines;
    const auto say = [&](std::string_view line) { lines.emplace_back(line); };
    fs::ScopedTempDir dir("cb_dos_writer");
    Environment env{.options = dw_options().value_or(Options{}), .state = dir.path() / "state", .retail = nullptr};
    if (env.options.probes.empty()) env.options.probes = dir.path() / "probes.json";
    // no program named: the usage, and exit 2
    CB_CHECK_EQ(command({}, env, say), 2);
    CB_CHECK(!lines.empty() && lines.back().find("dos-source <program> [--out DIR]") != std::string::npos);
    CB_CHECK_THROWS_AS(UsageError, command(std::vector<std::string>{"x.exe", "--out"}, env, say));
    CB_CHECK_THROWS_AS(NotFoundError, command(std::vector<std::string>{"nothing-like-it.exe"}, env, say));
    if (!dw_options()) return;
    // a corpus name goes through the retail lookup
    env.retail = [&](std::string_view name) -> std::optional<std::filesystem::path> {
        if (name == "choice") return dw_program("choice.com");
        return std::nullopt;
    };
    lines.clear();
    const std::vector<std::string> args = {"choice", "--out", fs::to_text(dir.path() / "o")};
    CB_CHECK_EQ(command(args, env, say), 0);
    CB_CHECK(std::ranges::any_of(lines, [](const std::string& l) {
        return l == "choice.com: 1062 instructions, 10 (0%) written as bytes; 2809 bytes of data; 1 round; builds back THE SAME";
    }));
    CB_CHECK(lines.back() == "  source: " + fs::to_text(dir.path() / "o" / "choice.asm"));
    // with no --out the folder is <state>/dos_source/<name>
    CB_CHECK_EQ(command(std::vector<std::string>{fs::to_text(dw_program("choice.com"))}, env, say), 0);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "state" / "dos_source" / "choice" / "choice.asm"));
});

}  // namespace
}  // namespace cb::source::dos_writer

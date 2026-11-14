// cb-unity: alone
// Tests of the driver writer over real Windows 95 drivers: BIOS, SPLITTER and SRAMMTD (formats/testdata), and DRVSPACE.BIN, a real-mode driver
// with a VxD (DSVXD) behind it (testdata/writers). The source the writer makes is held to the recorded `driver-source` output (run on the
// same files): byte for byte for the text with every instruction as a mnemonic (testdata/writers/driver_*.all.golden.asm, made by
// Writer.source(None)), and -- where OpenWatcom's assembler and linker are on the machine (settings: openwatcom, or
// CB_OPENWATCOM; those tests are skipped where there is none) -- for the source after the rounds (driver_*.final.golden.asm), the driver it
// builds (it must be the original's module, and the recorded image is kept as driver_srammtd.built.vxd) and the VxD file DRVSPACE.BIN
// is put together from.
//
// Design notes:
//   * a patched field that runs past the end of its object (its first byte within three of the end) is not read as a marker in the decoder's
//     window;
//   * `Result::same()` is the module coming back (all five checks), not a share of bytes equal outside the patched
//     places;
//   * the service names come from the caller (ServiceLabel), not from a table the module reads itself;
//   * errors are the types of cb/core/error.hpp; the files are written as Latin-1 with CRLF.

#include "cb/source/driver_writer.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>

#include <nlohmann/json.hpp>

#include "cb/base/config.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/knowledge/package.hpp"
#include "cb/symbols/driver_services.hpp"

namespace cb::source::driver_writer {
namespace {

using nlohmann::ordered_json;

std::filesystem::path drw_driver(std::string_view name) { return testing::data_path("formats/testdata/" + std::string(name)); }
std::filesystem::path drw_data(std::string_view name) { return testing::data_path("source/testdata/writers/" + std::string(name)); }

std::string drw_lf(const std::filesystem::path& file) { return text::replace_all(fs::read_text(file), "\r\n", "\n"); }

// The driver kit's names of the services.
const symbols::driver_services::Table& drw_table() {
    static const symbols::driver_services::Table table = [] {
        const knowledge::Value doc = knowledge::Value::from_json_text(fs::read_text(testing::data_path("symbols/testdata/driver_services.json")));
        const auto rows = [&](const char* kind) { return knowledge::Rows(*doc.get(kind)->map()); };
        return symbols::driver_services::Table::from_rows(rows("devices"), rows("services"), rows("drivers"), rows("messages"));
    }();
    return table;
}
ServiceLabel drw_label() {
    return [](std::uint32_t device, std::uint32_t service) { return drw_table().label(device, service); };
}

// The tools: OpenWatcom from the checkout's settings (or CB_OPENWATCOM), and a file for what is probed that lives as long as the tests.
std::optional<Options> drw_options() {
    static const fs::ScopedTempDir probes_dir("cb_driver_writer_probes");
    try {
        const config::Config main = config::Config::load(config::current().main_checkout());
        const auto root = main.path("openwatcom");
        const build::watcom::OpenWatcom ow = build::watcom::OpenWatcom::from_settings(root ? fs::to_text(*root) : std::string());
        if (ow.tool("wasm").empty() || ow.tool("wlink").empty()) return std::nullopt;
        return options_for(ow, probes_dir.path() / "assembly_probes.json", drw_label());
    } catch (const Error&) {   // (no configuration to read: no tools to test with)
        return std::nullopt;
    }
}

// The first driver of a file, read and decoded, with the writer over it.
struct DrwWritten {
    std::vector<std::pair<std::string, formats::le::Module>> members;
    platforms::DecoderSet decoders;
    std::unique_ptr<Writer> writer;
    explicit DrwWritten(const std::filesystem::path& file) : members(formats::le::open_modules(file)) {
        writer = std::make_unique<Writer>(members.front().second, decoders.get(platforms::Arch::X86, 32, true), drw_label());
    }
};

void drw_check_all_mnemonics(const std::filesystem::path& file, std::string_view golden, std::size_t instructions, std::size_t raw_free_data, std::size_t functions) {
    DrwWritten d(file);
    const Writer::Text written = d.writer->source(nullptr);
    CB_CHECK_EQ(written.counts.instructions, instructions);
    CB_CHECK_EQ(written.counts.data, raw_free_data);
    CB_CHECK_EQ(d.writer->function_starts().size(), functions);
    CB_CHECK_EQ(d.writer->instruction_count(), instructions);
    CB_CHECK(written.text == drw_lf(drw_data(golden)));
}

CB_TEST("source.driver_writer.the_source_with_every_instruction_a_mnemonic_is_the_recorded_source", [] {
    drw_check_all_mnemonics(drw_driver("splitter.vxd"), "driver_splitter.all.golden.asm", 499, 97, 12);
    drw_check_all_mnemonics(drw_driver("srammtd.vxd"), "driver_srammtd.all.golden.asm", 415, 341, 6);
    // the VxD behind DRVSPACE.BIN: the loader reads the LE past the real-mode driver
    drw_check_all_mnemonics(drw_data("drvspace.bin"), "driver_dsvxd.all.golden.asm", 241, 100, 4);
});

CB_TEST("source.driver_writer.a_larger_driver_has_the_instructions_and_functions", [] {
    DrwWritten d(drw_driver("bios.vxd"));
    const Writer::Text written = d.writer->source(nullptr);
    CB_CHECK_EQ(written.counts.instructions, 4943u);
    CB_CHECK_EQ(written.counts.data, 1278u);
    CB_CHECK_EQ(d.writer->function_starts().size(), 156u);
    CB_CHECK_EQ(written.line_of.size(), 4829u);   // (every instruction that has a text; the line says which one it is)
    for (const auto& [line, at] : written.line_of) {
        const auto& ob = d.writer->objects().at(at.object);
        CB_CHECK(ob.ins.contains(at.offset));
        CB_CHECK(line >= 12);   // (behind the head)
    }
});

CB_TEST("source.driver_writer.a_place_that_nothing_can_be_written_at_is_the_nearest_place_before_it_and_the_distance", [] {
    DrwWritten d(drw_driver("splitter.vxd"));   // one object of 1540 bytes
    Writer& w = *d.writer;
    CB_CHECK_EQ(w.label(1, 0), "L1_00000");
    CB_CHECK_EQ(w.label(1, 3), "L1_00003");
    CB_CHECK_EQ(w.label(1, -5), "L1_00000 - 5");
    CB_CHECK_EQ(w.label(1, 0x7FFFFFFF), "L1_00604 + 2147482107");
    CB_CHECK_EQ(w.label(1, 0x80000010), "L1_00000 - 2147483632");   // (a 32-bit address past the middle of the range is negative)
    CB_CHECK_EQ(w.label(1, 0xFFFFFFF0), "L1_00000 - 16");
    // what the image names keeps its name: the control procedure, and the services under the names the kit gives them
    CB_CHECK_EQ(w.label(1, 153), "Splitter_control");
    CB_CHECK_EQ(w.label(1, 315), "Splitter_Service_0000");
});

CB_TEST("source.driver_writer.the_whole_driver_is_one_l2_tree_each_function_a_fragment_at_its_place", [] {
    DrwWritten d(drw_driver("splitter.vxd"));
    const Writer::Text written = d.writer->source(nullptr);
    CB_CHECK(d.writer->assembly().has_value());
    Arena arena;
    const ordered_json doc = translate::l2::to_json(*d.writer->assembly()->document(arena));
    std::size_t functions = 0;
    bool control = false;
    for (const ordered_json& unit : doc.at("units")) {
        if (unit.value("reason", "") != "function") continue;
        ++functions;
        if (unit.at("name") == "Splitter_control") {
            control = true;
            CB_CHECK(unit.at("origin").at("segment").get<int>() >= 1 && unit.at("length").get<int>() > 0);
        }
    }
    CB_CHECK_EQ(functions, 12u);
    CB_CHECK(control);
    CB_CHECK(written.text.starts_with("; Splitter -- a virtual device driver, reconstructed from the image by CodeBreaker.\n"));
});

CB_TEST("source.driver_writer.a_service_call_is_the_int_and_its_two_words_with_the_service_named_beside", [] {
    DrwWritten d(drw_driver("bios.vxd"));
    const Writer::Text written = d.writer->source(nullptr);
    std::size_t calls = 0;
    for (const auto& [i, ob] : d.writer->objects()) calls += ob.services.size();
    CB_CHECK(calls > 20);
    CB_CHECK(written.text.find("        int  20h\n        dd   0") != std::string::npos);
});

CB_TEST("source.driver_writer.without_an_assembler_nothing_builds_and_the_note_says_so", [] {
    fs::ScopedTempDir dir("cb_driver_writer");
    Options none;   // no tools
    none.probes = dir.path() / "probes.json";
    const Result r = reconstruct(drw_driver("splitter.vxd"), dir.path() / "out", none);
    CB_CHECK(!r.linked && !r.same() && !r.image.has_value());
    CB_CHECK_EQ(r.notes.size(), 1u);
    CB_CHECK(r.notes.front().starts_with("assembler: "));
    CB_CHECK_EQ(r.instructions, 499u);
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "out" / "splitter.asm"));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "out" / "module.l2.json"));
    // nothing was probed, so nothing is known to come back from its mnemonic: every instruction but the service calls is written as bytes
    // (those are 135 of the 499; they are neither one nor the other)
    CB_CHECK_EQ(r.raw, 364u);
});

CB_TEST("source.driver_writer.a_file_that_is_not_a_driver_and_a_member_it_has_not_got_are_errors", [] {
    fs::ScopedTempDir dir("cb_driver_writer");
    Options none;
    CB_CHECK_THROWS_AS(FormatError, (void)reconstruct(drw_driver("choice.com"), dir.path() / "a", none));
    CB_CHECK_THROWS_AS(UsageError, (void)reconstruct(drw_driver("splitter.vxd"), dir.path() / "b", none, 1));
    CB_CHECK_THROWS_AS(IoError, (void)reconstruct(dir.path() / "absent.vxd", dir.path() / "c", none));
});

CB_TEST("source.driver_writer.the_command_says_how_to_use_it_and_names_a_driver_that_is_not_there", [] {
    std::vector<std::string> said;
    const auto say = [&](std::string_view line) { said.emplace_back(line); };
    Environment env;
    CB_CHECK_EQ(command({}, env, say), 2);
    CB_CHECK(said.size() == 1 && said.front().starts_with("driver_writer -- a virtual device driver"));
    const std::vector<std::string> absent = {"no-such-driver.vxd"};
    CB_CHECK_THROWS_AS(NotFoundError, (void)command(absent, env, say));
    const std::vector<std::string> no_folder = {"--out"};
    CB_CHECK_THROWS_AS(UsageError, (void)command(no_folder, env, say));
});

// ---- with OpenWatcom's assembler and linker ----------------------------------------------------------------------------

void drw_check_built(const Result& r, std::size_t instructions, std::size_t raw, std::size_t data) {
    CB_CHECK(r.linked);
    CB_CHECK(r.same());
    CB_CHECK(r.built.has_value() && r.built->same.has_value() && *r.built->same == 1.0);
    CB_CHECK_EQ(r.instructions, instructions);
    CB_CHECK_EQ(r.raw, raw);
    CB_CHECK_EQ(r.data, data);
    CB_CHECK_EQ(r.rounds, 1u);
    CB_CHECK(r.differing_objects.empty());
    CB_CHECK(r.definitions.has_value() && r.l2.has_value());
}

CB_TEST("source.driver_writer.a_driver_is_written_built_and_comes_back_the_original_module", [] {
    const auto options = drw_options();
    if (!options) return;   // (no OpenWatcom on this machine)
    fs::ScopedTempDir dir("cb_driver_writer");
    const Result r = reconstruct(drw_driver("srammtd.vxd"), dir.path(), *options);
    drw_check_built(r, 415, 4, 341);
    CB_CHECK(r.source == dir.path() / "srammtd.asm");
    // the source after the rounds is the recorded one, the four instructions it could not have back written as bytes
    CB_CHECK(drw_lf(r.source) == drw_lf(drw_data("driver_srammtd.final.golden.asm")));
    CB_CHECK(fs::read_file(*r.image) == fs::read_file(drw_data("driver_srammtd.built.vxd")));
    // the definitions the linker was given, and the L2 tree beside the source
    CB_CHECK(fs::read_text(*r.definitions).starts_with("; "));
    CB_CHECK(fs::read_text(dir.path() / "module.l2.json").find("\"reason\": \"function\"") != std::string::npos);
    // made again in the same folder: the .def is kept as it is
    const std::string def = fs::read_text(*r.definitions);
    const Result again = reconstruct(drw_driver("srammtd.vxd"), dir.path(), *options);
    CB_CHECK(again.same() && fs::read_text(*again.definitions) == def);
});

CB_TEST("source.driver_writer.the_vxd_behind_a_real_mode_driver_is_written_as_the_hook_of_the_dos_writer", [] {
    const auto options = drw_options();
    if (!options) return;
    fs::ScopedTempDir dir("cb_driver_writer");
    const Result direct = reconstruct(drw_data("drvspace.bin"), dir.path() / "direct", *options);
    drw_check_built(direct, 241, 2, 100);
    CB_CHECK(direct.source == dir.path() / "direct" / "dsvxd.asm");
    CB_CHECK(drw_lf(direct.source) == drw_lf(drw_data("driver_dsvxd.final.golden.asm")));
    CB_CHECK(fs::read_file(*direct.image) == fs::read_file(drw_data("dsvxd.vxd")));   // (the recorded image)

    // the hook: what dos_writer asks for, answered with the same
    std::vector<std::string> said;
    const dos_writer::WriteVxd hook = vxd_writer(*options);
    const dos_writer::VxdPart part = hook(drw_data("drvspace.bin"), dir.path() / "hook", [&](const std::string& line) { said.push_back(line); });
    CB_CHECK(part.linked && part.same);
    CB_CHECK_EQ(part.instructions, 241u);
    CB_CHECK_EQ(part.raw, 2u);
    CB_CHECK_EQ(part.data, 100u);
    CB_CHECK_EQ(part.rounds, 1u);
    CB_CHECK(part.source == dir.path() / "hook" / "dsvxd.asm");
    CB_CHECK(part.image.has_value() && fs::read_file(*part.image) == fs::read_file(drw_data("dsvxd.vxd")));
    CB_CHECK(said.empty());   // (nothing came back different: no round to report)

    // and wired into the DOS writer, DRVSPACE.BIN is laid out as the original is
    dos_writer::Options dos = dos_writer::options_for(build::watcom::OpenWatcom::from_settings(fs::to_text(*config::Config::load(config::current().main_checkout()).path("openwatcom"))),
                                                      dir.path() / "dos_probes.json");
    dos.write_vxd = hook;
    const dos_writer::Result file = dos_writer::reconstruct(drw_data("drvspace.bin"), dir.path() / "file", dos);
    CB_CHECK_EQ(file.kind, "driver+vxd");
    CB_CHECK_EQ(file.parts.size(), 2u);
    CB_CHECK(file.parts[0].same && file.parts[1].name == "vxd" && file.parts[1].same);
    CB_CHECK_EQ(file.instructions, 7136u);
    CB_CHECK_EQ(file.raw, 132u);
    CB_CHECK_EQ(file.data, 38379u);
    CB_CHECK(!file.same && file.equivalent);
    CB_CHECK(file.file.has_value());
});

// The proof over the corpus, outside the suite's own inputs: CB_DRIVER_WRITER_FILES names drivers (separated by `;`), CB_DRIVER_WRITER_OUT the
// folder their sources are written under (<out>/<stem>), CB_DRIVER_WRITER_PROBES the file what was probed is kept in (else a new one for the run);
// a line a driver says how long it took. Nothing happens where it is not set.
CB_TEST("source.driver_writer.the_drivers_named_in_the_environment_come_back", [] {
    const auto files = fs::get_env("CB_DRIVER_WRITER_FILES");
    const auto out = fs::get_env("CB_DRIVER_WRITER_OUT");
    auto options = drw_options();
    if (!files || !out || !options) return;
    if (const auto probes = fs::get_env("CB_DRIVER_WRITER_PROBES")) options->probes = fs::from_text(*probes);   // (a file that keeps what was probed: warm)
    for (const std::string_view file : text::split(*files, ';')) {
        const std::filesystem::path path = fs::from_text(file);
        const auto begun = std::chrono::steady_clock::now();
        const Result r = reconstruct(path, fs::from_text(*out) / path.stem(), *options);
        const auto took = std::chrono::duration<double>(std::chrono::steady_clock::now() - begun).count();
        std::cout << std::format("{}: linked {} same {} instructions {} raw {} data {} rounds {} in {:.2f} s\n", fs::to_text(path.filename()), r.linked, r.same(), r.instructions,
                                 r.raw, r.data, r.rounds, took);
        CB_CHECK(r.linked && r.same());
    }
});

CB_TEST("source.driver_writer.the_command_writes_and_reports_a_driver", [] {
    const auto options = drw_options();
    if (!options) return;
    fs::ScopedTempDir dir("cb_driver_writer");
    std::vector<std::string> said;
    const auto say = [&](std::string_view line) { said.emplace_back(line); };
    Environment env;
    env.options = *options;
    env.state = dir.path() / "state";
    const std::vector<std::string> args = {fs::to_text(drw_driver("splitter.vxd"))};
    CB_CHECK_EQ(command(args, env, say), 0);
    CB_CHECK(!said.empty());
    CB_CHECK_EQ(said.front(), "splitter.vxd: 499 instructions, 0 (0%) written as bytes; 97 bytes of data; 1 round");
    CB_CHECK_EQ(said[1], "  built: objects same, descriptor same, services same, exports same, module same");
    CB_CHECK_EQ(said.back(), "  source: " + fs::to_text(dir.path() / "state" / "driver_source" / "splitter" / "splitter.asm"));
    CB_CHECK(std::filesystem::is_regular_file(dir.path() / "state" / "driver_source" / "splitter" / "splitter.vxd"));
});

}  // namespace
}  // namespace cb::source::driver_writer

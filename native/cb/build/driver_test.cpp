// Tests of the driver build: finding the source and the original, the linker script, the structural check on real VxDs
// (the pairs' notes are what the Python build/driver.py printed for the same files), the report line, the command, and
// -- when OpenWatcom and the Windows 95 DDK's include folder are named in CB_OPENWATCOM and CB_DRIVER_KIT -- a real
// assemble, link and check of a small driver source.
//
// DIVERGES from the Python:
//   * compare(): `same` counts a patched place only when it lies inside the object (the Python subtracted every place
//   the
//     loader patches from an object's size, also those past its end, which made the share too high). The patched
//     places are found once per image, not once per service.
//   * build(): an original that is not a driver (it cannot be read) gives a note and no checks; the Python raised.
//   * a run of a tool that does not finish in 300 s gives "timed out after 300 s", as before; a tool that cannot be
//     started is a failed run with the system's message.
#include "cb/build/driver.hpp"

#include <algorithm>
#include <cmath>
#include <set>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

using namespace cb;
using namespace cb::build::driver;
namespace stdfs = std::filesystem;
namespace le = cb::formats::le;
namespace drv = cb::build::driver;
using cb::build::watcom::OpenWatcom;

le::Module vxd(const std::string& name) {
    return le::Module::open(testing::data_path("formats/testdata/" + name));
}

bool contains(const std::vector<std::string>& v, const std::string& s) {
    return std::find(v.begin(), v.end(), s) != v.end();
}

// The wlink script a link wrote: the tool fails on `@script`, but the script is there.
std::string link_script(const stdfs::path& dir, const LinkOptions& options, std::string_view device = "VKD") {
    // (a program that rejects any argument: `where @script` is an invalid pattern)
    Tools tools;
    tools.wlink = fs::from_text(fs::get_env("SystemRoot").value_or("C:/Windows")) / "System32" / "where.exe";
    const ToolResult r = link(tools, dir / "vkd.obj", device, dir / "out" / "vkd.vxd", options);
    CB_CHECK(!r.file.has_value()); // it made no image
    return fs::read_text(dir / "out" / "vkd.lnk");
}

std::string paths_in(std::string s, const stdfs::path& dir) {
    std::string d = fs::to_text(dir);
    std::replace(d.begin(), d.end(), '/', '\\');
    return text::replace_all(s, d, "<D>");
}

} // namespace

// ---- finding the source and the original -----------------------------------------------------------------------

CB_TEST("build.driver.source_of", [] {
    fs::ScopedTempDir dir("drv");
    fs::write_text(dir.path() / "vkd" / "vkd.asm", "x");
    fs::make_dirs(dir.path() / "empty");
    fs::write_text(dir.path() / "other" / "thing.asm", "x");
    CB_CHECK(source_of(dir.path() / "vkd") == dir.path() / "vkd" / "vkd.asm");
    CB_CHECK(source_of(dir.path() / "vkd" / "vkd.asm") == dir.path() / "vkd" / "vkd.asm");
    CB_CHECK(source_of(fs::from_text(fs::to_text(dir.path() / "vkd") + "/")) ==
             fs::from_text(fs::to_text(dir.path() / "vkd") + "/") / "vkd.asm");
    CB_CHECK(source_of(dir.path() / "nodir") == dir.path() / "nodir"); // not a folder: as given
    CB_CHECK_THROWS_AS(UsageError, (void)source_of(dir.path() / "empty"));
    CB_CHECK_THROWS_AS(UsageError, (void)source_of(dir.path() / "other")); // the file must be named after the folder
});

CB_TEST("build.driver.original_of", [] {
    const std::vector<stdfs::path> corpus{"C:\\w95\\SYSTEM\\vmm.vxd", "C:\\w95\\SYSTEM\\VKD.VXD",
                                          "C:\\w95\\other\\vkd.386", "C:\\w95\\vpicd.vxd"};
    CB_CHECK(original_of("vkd.asm", corpus) ==
             stdfs::path("C:\\w95\\SYSTEM\\VKD.VXD")); // the first of that name, case aside
    CB_CHECK(original_of("C:\\x\\VPICD.ASM", corpus) == stdfs::path("C:\\w95\\vpicd.vxd"));
    CB_CHECK(!original_of("nothing.asm", corpus).has_value());
    CB_CHECK(!original_of("vkd.asm", {}).has_value());
    CB_CHECK(!original_of("vk.asm", corpus).has_value()); // whole stems only
});

CB_TEST("build.driver.kit_folder", [] {
    fs::ScopedTempDir dir("drv");
    // a kit whose path has no spaces is used where it is
    fs::write_text(dir.path() / "kit" / "VMM.INC", "a");
    CB_CHECK(kit_folder(dir.path() / "kit", dir.path() / "copy") == dir.path() / "kit");
    CB_CHECK(kit_folder({}, dir.path() / "copy").empty()); // no kit
    // one with spaces is copied (its .INC files only) to a folder without
    fs::write_text(dir.path() / "kit with spaces" / "VMM.INC", "vmm");
    fs::write_text(dir.path() / "kit with spaces" / "vmm.inc.bak", "no");
    fs::write_text(dir.path() / "kit with spaces" / "Other.inc", "other");
    fs::write_text(dir.path() / "kit with spaces" / "notes.txt", "text");
    const stdfs::path copy = dir.path() / "copy";
    const bool plain = fs::to_text(copy).find(' ') == std::string::npos;
    if (plain) { // (the temp folder's own path may have a space on a machine with a spaced user name)
        CB_CHECK(kit_folder(dir.path() / "kit with spaces", copy) == copy);
        CB_CHECK_EQ(fs::read_text(copy / "VMM.INC"), "vmm");
        CB_CHECK_EQ(fs::read_text(copy / "Other.inc"), "other");
        CB_CHECK(!stdfs::exists(copy / "notes.txt"));
        CB_CHECK(!stdfs::exists(copy / "vmm.inc.bak"));
        // made once: a second call leaves what is there
        fs::write_text(copy / "VMM.INC", "changed");
        CB_CHECK(kit_folder(dir.path() / "kit with spaces", copy) == copy);
        CB_CHECK_EQ(fs::read_text(copy / "VMM.INC"), "changed");
    }
    // nowhere without a space to copy to
    CB_CHECK(kit_folder(dir.path() / "kit with spaces", dir.path() / "copy with space").empty());
});

CB_TEST("build.driver.tool_prefers_binnt", [] {
    fs::ScopedTempDir dir("drv");
    const OpenWatcom none(dir.path() / "nowhere");
    CB_CHECK(tool(none, "wasm").empty());
    const OpenWatcom empty{stdfs::path()};
    CB_CHECK(tool(empty, "wasm").empty());
    fs::write_text(dir.path() / "ow" / "binnt64" / "wlink.exe", "64");
    fs::write_text(dir.path() / "ow" / "binnt64" / "wasm.exe", "64");
    fs::write_text(dir.path() / "ow" / "binnt" / "wasm.exe", "32");
    const OpenWatcom ow(dir.path() / "ow");
    CB_CHECK(tool(ow, "wasm") == dir.path() / "ow" / "binnt" / "wasm.exe");     // the driver build's own flavour first
    CB_CHECK(tool(ow, "wlink") == dir.path() / "ow" / "binnt64" / "wlink.exe"); // else the general one
    CB_CHECK(tool(ow, "wrc").empty());
    const Tools tools = find_tools(ow, {}, dir.path() / "state");
    CB_CHECK(tools.wasm == dir.path() / "ow" / "binnt" / "wasm.exe");
    CB_CHECK(tools.wlink == dir.path() / "ow" / "binnt64" / "wlink.exe");
    CB_CHECK(tools.kit.empty());
    CB_CHECK(tools.env.contains("WATCOM"));
});

// ---- assembling and linking -----------------------------------------------------------------------------------------

CB_TEST("build.driver.assemble_and_link_say_what_is_missing", [] {
    fs::ScopedTempDir dir("drv");
    Tools none;
    CB_CHECK_EQ(assemble(none, "x.asm", dir.path()).log, "no assembler");
    Tools no_kit;
    no_kit.wasm = "wasm.exe";
    const ToolResult r = assemble(no_kit, "x.asm", dir.path());
    CB_CHECK(!r.file.has_value());
    CB_CHECK_EQ(r.log, "the driver kit's include files were not found");
    CB_CHECK_EQ(link(none, "x.obj", "VKD", dir.path() / "x.vxd").log, "no linker");
});

CB_TEST("build.driver.a_tool_that_cannot_start_is_a_failed_run", [] {
    fs::ScopedTempDir dir("drv");
    Tools tools;
    tools.wasm = dir.path() / "no_such_assembler.exe";
    tools.kit = dir.path();
    const ToolResult r = assemble(tools, dir.path() / "x.asm", dir.path());
    CB_CHECK(!r.file.has_value());
    CB_CHECK(!r.log.empty());
});

CB_TEST("build.driver.link_script_default", [] {
    fs::ScopedTempDir dir("drv");
    const std::string script = paths_in(link_script(dir.path(), {}), dir.path());
    CB_CHECK_EQ(script, "format windows vxd\r\noption quiet\r\noption map=<D>\\out\\vkd.map\r\nname "
                        "<D>\\out\\vkd.vxd\r\nexport VKD_DDB\r\n"
                        "file <D>\\vkd.obj\r\nsegment class 'LCODE' preload\r\nsegment class 'MCODE' preload iopl\r\n"
                        "segment class 'ICODE' discardable\r\nsegment class 'PCODE' loadoncall\r\nsegment class "
                        "'PDATA' loadoncall shared\r\n"
                        "segment class 'SCODE' resident\r\nsegment class 'DBOCODE' preload conforming\r\n"
                        "segment class '16ICODE' preload discardable\r\n");
});

CB_TEST("build.driver.link_script_dynamic_unnamed_and_own_classes", [] {
    fs::ScopedTempDir dir("drv");
    LinkOptions options;
    options.dynamic = true;
    options.named = false;
    options.classes = {{"LCODE", "preload"}, {"PDATA", "resident"}};
    const std::string script = paths_in(link_script(dir.path(), options), dir.path());
    CB_CHECK_EQ(
        script,
        "format windows vxd virtdevice\r\noption quiet\r\noption map=<D>\\out\\vkd.map\r\nname <D>\\out\\vkd.vxd\r\n"
        "export VKD_DDB.1\r\nfile <D>\\vkd.obj\r\nsegment class 'LCODE' preload\r\nsegment class 'PDATA' resident\r\n");
});

CB_TEST("build.driver.link_script_from_a_definition_file", [] {
    fs::ScopedTempDir dir("drv");
    LinkOptions options;
    options.definitions = "LIBRARY VKD\r\nDESCRIPTION 'VKD Virtual Device\t(Version 4.0)'\r\nEXPORTS\r\n    VKD_DDB  "
                          "@1\r\n    VKD_Other @7 RESIDENT\r\n"
                          "  ;comment @3\r\nnot_an_export @9\r\n";
    const std::string script = paths_in(link_script(dir.path(), options), dir.path());
    CB_CHECK_EQ(script.substr(0, script.find("segment")),
                "format windows vxd\r\noption quiet\r\noption description 'VKD Virtual Device\t(Version 4.0)'\r\n"
                "option map=<D>\\out\\vkd.map\r\nname <D>\\out\\vkd.vxd\r\nexport VKD_DDB.1\r\nexport "
                "VKD_Other.7\r\nfile <D>\\vkd.obj\r\n");
    // a doubled quote stays doubled (the linker reads it as one); no EXPORTS: the device's descriptor
    LinkOptions quotes;
    quotes.definitions = "DESCRIPTION 'It''s a driver'\n";
    CB_CHECK(link_script(dir.path(), quotes).find("option description 'It''s a driver'\r\n") != std::string::npos);
    CB_CHECK(link_script(dir.path(), quotes).find("export VKD_DDB\r\n") != std::string::npos);
});

// ---- the structural check -----------------------------------------------------------------------------------------

CB_TEST("build.driver.structure_of_real_drivers", [] {
    const Structure splitter = structure(vxd("splitter.vxd"));
    CB_CHECK_EQ(splitter.objects.size(), 1u);
    CB_CHECK(splitter.objects[0] == (Structure::Object{1540, 8261}));
    CB_CHECK(splitter.descriptor.has_value());
    CB_CHECK_EQ(splitter.descriptor->name, "Splitter");
    CB_CHECK_EQ(splitter.descriptor->service_count, 5u);
    CB_CHECK_EQ(splitter.services.size(), 5u);
    CB_CHECK(splitter.services[0] == std::optional<le::Place>(le::Place{1, 315})); // the table of the Python's harness
    CB_CHECK(splitter.services[4] == std::optional<le::Place>(le::Place{1, 671}));
    CB_CHECK_EQ(splitter.exports.size(), 1u);
    CB_CHECK(splitter.exports[0].name == std::optional<std::string>("Splitter_DDB") &&
             splitter.exports[0].ordinal == 1);
    CB_CHECK(splitter.export_is_descriptor);
    CB_CHECK_EQ(splitter.module_flags, 0x38000u);
    CB_CHECK_EQ(splitter.references, 30u);

    const Structure bios = structure(vxd("bios.vxd"));
    CB_CHECK_EQ(bios.objects.size(), 4u);
    CB_CHECK_EQ(bios.module_flags, 0x28000u);
    CB_CHECK_EQ(bios.services.size(), 4u);
    CB_CHECK_EQ(bios.references, 270u);
});

CB_TEST("build.driver.an_image_agrees_with_itself", [] {
    for (const char* name : {"splitter.vxd", "srammtd.vxd", "bios.vxd"}) {
        const le::Module m = vxd(name);
        const Comparison c = compare(m, m);
        CB_CHECK(c.checks.all());
        CB_CHECK(c.notes.empty());
        CB_CHECK(c.same.has_value() && *c.same == 1.0);
    }
});

CB_TEST("build.driver.compare_names_each_difference", [] {
    const le::Module splitter = vxd("splitter.vxd"), srammtd = vxd("srammtd.vxd"), bios = vxd("bios.vxd");
    // the notes the Python's compare() wrote for the same pairs
    const Comparison a = compare(splitter, srammtd);
    CB_CHECK(!a.checks.objects && !a.checks.descriptor && !a.checks.services && !a.checks.exports && a.checks.module);
    CB_CHECK_EQ(a.notes.size(), 5u);
    CB_CHECK_EQ(a.notes[0],
                "objects: 1 of 1 differ; object 1 is 0x604 bytes flags 0x2045, original 0x670 flags 0x2045");
    CB_CHECK_EQ(a.notes[1], "descriptor: name is 'Splitter', original 'SRAMMTD'");
    CB_CHECK_EQ(a.notes[2], "services: 5 services, the original has 0");
    CB_CHECK_EQ(a.notes[3], "exports: [('Splitter_DDB', 1)], original [('SRAMMTD_DDB', 1)]");
    CB_CHECK_EQ(a.notes[4], "places the loader patches: 30, original 47");
    CB_CHECK(!a.same.has_value()); // the objects' sizes differ: none pair up

    const Comparison b = compare(splitter, bios);
    CB_CHECK(!b.checks.module);
    CB_CHECK_EQ(b.notes[0], "objects: 1 objects, the original has 4");
    CB_CHECK_EQ(b.notes[3], "exports: [('Splitter_DDB', 1)], original [('BIOS_DDB', 1)]");
    CB_CHECK_EQ(b.notes[4], "module: module flags 0x38000, original 0x28000");
    CB_CHECK_EQ(b.notes[5], "places the loader patches: 30, original 270");
    const Comparison c = compare(bios, splitter);
    CB_CHECK_EQ(c.notes[2], "services: 4 services, the original has 5");
});

CB_TEST("build.driver.compare_counts_equal_bytes_outside_patched_places", [] {
    // the same driver with two code bytes changed: the share falls by just those, of the bytes outside the places the
    // loader patches (each a 32-bit address)
    const Bytes original = fs::read_file(testing::data_path("formats/testdata/splitter.vxd"));
    const le::Module a = le::Module::from_buffer(original);
    const std::uint64_t at = *a.file_offset(1, 100);
    Bytes changed = original;
    changed[at] ^= 0xFF;
    changed[at + 1] ^= 0xFF;
    const le::Module b = le::Module::from_buffer(changed);

    std::set<std::int64_t> patched;
    for (const le::Reference& r : a.references()) {
        for (std::int64_t i = 0; i < 4; ++i)
            patched.insert(r.at.offset + i);
    }
    CB_CHECK(!patched.contains(100) && !patched.contains(101)); // the changed bytes are code, not an address
    const double total = static_cast<double>(a.object_data(a.objects()[0]).size());
    const double outside = total - static_cast<double>(patched.size());

    const Comparison c = compare(b, a);
    CB_CHECK(c.checks.all()); // the structure is the same
    CB_CHECK(c.same.has_value());
    CB_CHECK(std::abs(*c.same - (outside - 2.0) / outside) < 1e-12);
});

// ---- the whole build and its report
// ---------------------------------------------------------------------------------------

CB_TEST("build.driver.build_without_a_device_declaration", [] {
    fs::ScopedTempDir dir("drv");
    fs::write_text(dir.path() / "src" / "nodecl" / "nodecl.asm", ".386p\r\nend\r\n");
    BuildRequest request{.source = dir.path() / "src" / "nodecl", .work = dir.path() / "work"};
    const BuildResult r = drv::build(Tools{}, request);
    CB_CHECK_EQ(r.name, "nodecl");
    CB_CHECK(r.source == dir.path() / "src" / "nodecl" / "nodecl.asm");
    CB_CHECK(!r.assembled && !r.linked && !r.path);
    CB_CHECK_EQ(r.log, "the source declares no device (no Declare_Virtual_Device)");
    CB_CHECK(stdfs::is_directory(dir.path() / "work")); // the work folder is made
    CB_CHECK_EQ(report(r), "nodecl       does not assemble: the source declares no device (no Declare_Virtual_Device)");
});

CB_TEST("build.driver.build_finds_the_declaration_in_any_case", [] {
    fs::ScopedTempDir dir("drv");
    fs::write_text(dir.path() / "a.asm", "; header\r\n    declare_virtual_device  MINI, 1, 0\r\n");
    // with no assembler the build stops at the assembler -- after the declaration was found
    const BuildResult r = drv::build(Tools{}, {.source = dir.path() / "a.asm", .work = dir.path() / "w"});
    CB_CHECK_EQ(r.log, "no assembler");
    CB_CHECK_THROWS_AS(UsageError, (void)drv::build(Tools{}, {.source = dir.path(),
                                                              .work = dir.path() / "w"})); // a folder without a.asm
});

CB_TEST("build.driver.report_lines", [] {
    BuildResult r;
    r.name = "vkd";
    CB_CHECK_EQ(report(r), "vkd          does not assemble: ?");
    r.log = "first line of text\r\nsecond";
    CB_CHECK_EQ(report(r), "vkd          does not assemble: first line of text");
    r.assembled = true;
    CB_CHECK_EQ(report(r), "vkd          assembles, does not link: second");
    r.linked = true;
    CB_CHECK_EQ(report(r), "vkd          built (no original to check against)");
    r.notes = {"the rebuilt file could not be read: x"};
    CB_CHECK_EQ(report(r), "vkd          built (the rebuilt file could not be read: x)");
    r.checks = Checks{};
    r.checks->descriptor = false;
    r.same = 0.9876;
    CB_CHECK_EQ(report(r), "vkd          built  objects same descriptor DIFFERS services same exports same module same "
                           " 98.8% of paired bytes equal");
    r.same.reset();
    CB_CHECK_EQ(report(r),
                "vkd          built  objects same descriptor DIFFERS services same exports same module same");
    BuildResult wide;
    wide.name = "a_long_driver_name";
    wide.log = std::string(300, 'x');
    CB_CHECK_EQ(report(wide),
                "a_long_driver_name does not assemble: " + std::string(120, 'x')); // 120 columns of the message
});

CB_TEST("build.driver.command_declares_the_arguments", [] {
    Environment env;
    const cli::Command c = command(env);
    CB_CHECK_EQ(c.name, "driver-build");
    CB_CHECK_EQ(c.params.size(), 3u);
    const cli::Cli app("codebreaker", {c});
    CB_CHECK_THROWS_AS(UsageError, (void)app.parse({"driver-build"})); // a source is required
    const auto parsed = app.parse({"driver-build", "a", "b", "--original", "vkd.vxd", "--out", "d"});
    CB_CHECK_EQ(parsed.args.values("source").size(), 2u);
    CB_CHECK_EQ(parsed.args.value("original").value_or(""), "vkd.vxd");
    CB_CHECK_EQ(parsed.args.value("out").value_or(""), "d");
});

CB_TEST("build.driver.command_prints_a_line_per_source", [] {
    fs::ScopedTempDir dir("drv");
    fs::write_text(dir.path() / "good" / "good.asm",
                   "Declare_Virtual_Device GOOD, 1, 0\r\n"); // no tools: stops at the assembler
    fs::make_dirs(dir.path() / "bare");
    Environment env;
    env.state = dir.path() / "state";
    const cli::Command c = command(env);
    const cli::Cli app("codebreaker", {c});
    testing::CapturedStd captured;
    const int code = app.run({"driver-build", fs::to_text(dir.path() / "good"), fs::to_text(dir.path() / "bare")});
    const std::string out = captured.out();
    CB_CHECK_EQ(code, 1);
    CB_CHECK(out.find("good         does not assemble: no assembler\n") != std::string::npos);
    CB_CHECK(out.find("no bare.asm in it\n") != std::string::npos); // the folder without its source
    CB_CHECK(stdfs::is_directory(dir.path() / "state" / "driver_build" /
                                 "good")); // builds go in <state>/driver_build/<name>
});

// ---- the real tools ------------------------------------------------------------------------------------------------

// With CB_OPENWATCOM (the OpenWatcom tree) and CB_DRIVER_KIT (the Windows 95 DDK's include folder) set, a small driver
// is assembled and linked, and the result checked against itself.
CB_TEST("build.driver.build_a_driver_with_openwatcom", [] {
    const auto ow_root = fs::get_env("CB_OPENWATCOM");
    const auto kit = fs::get_env("CB_DRIVER_KIT");
    if (!ow_root || !kit)
        return; // (not set up: nothing to run)
    fs::ScopedTempDir dir("drv");
    const Tools tools = find_tools(OpenWatcom(fs::from_text(*ow_root)), fs::from_text(*kit), dir.path() / "state");
    CB_CHECK(!tools.wasm.empty() && !tools.wlink.empty() && !tools.kit.empty());
    fs::write_file(dir.path() / "mini" / "mini.asm", fs::read_file(testing::data_path("build/testdata/mini.asm")));
    const BuildResult r = drv::build(tools, {.source = dir.path() / "mini", .work = dir.path() / "out"});
    CB_CHECK(r.assembled);
    CB_CHECK(r.linked);
    CB_CHECK(r.path.has_value() && stdfs::is_regular_file(*r.path));
    CB_CHECK(!r.checks.has_value()); // no original: nothing to check against
    CB_CHECK(stdfs::is_regular_file(dir.path() / "out" / "mini.obj"));
    // the image it made reads back as a driver of the name the source declared, exporting its descriptor
    const le::Module built = le::Module::open(*r.path);
    const Structure s = structure(built);
    CB_CHECK(s.descriptor.has_value() && s.descriptor->name == "MINI");
    CB_CHECK(s.export_is_descriptor);
    CB_CHECK_EQ(s.exports.at(0).name.value_or(""), "MINI_DDB");
    // checked against itself it agrees
    BuildRequest again{.source = dir.path() / "mini", .work = dir.path() / "out2", .original = *r.path};
    const BuildResult r2 = drv::build(tools, again);
    CB_CHECK(r2.checks.has_value() && r2.checks->all());
    CB_CHECK(r2.same.has_value() && *r2.same > 0.99);
    // a source that does not assemble reports the assembler's message
    fs::write_text(dir.path() / "bad" / "bad.asm",
                   "Declare_Virtual_Device BAD, 1, 0\r\n  this is not an instruction\r\n");
    const BuildResult bad = drv::build(tools, {.source = dir.path() / "bad", .work = dir.path() / "out3"});
    CB_CHECK(!bad.assembled);
    CB_CHECK(!bad.log.empty());
});

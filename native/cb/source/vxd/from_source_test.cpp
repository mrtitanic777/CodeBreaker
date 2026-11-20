// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/from_source on the real drivers of formats/testdata (splitter.vxd: one object, 4096-byte pages; bios.vxd: four objects, 512-byte
// pages). The expected values are the recorded ones (the build of splitter came back byte for byte the original's, one object, device id 0x0498 and an
// identity page map, a 512-byte version resource) or are worked out from the rules in the module's header. The assemblers are not given: the readable
// source is then not compiled, and the file is the LE writer's, which is what ships either way.
#include "cb/source/vxd/from_source.hpp"

#include "cb/base/config.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace vx = cb::source::vxd;
namespace fsrc = cb::source::vxd::from_source;
namespace le = cb::formats::le;

namespace {

std::filesystem::path fsrc_path(const char* name) { return cb::testing::data_path(std::string("formats/testdata/") + name); }

// The services splitter calls, at their ordinals (the DDK's INC32). The build of a driver depends on them: the service table is only recovered as such
// where its handlers' services have names, and what the macros regenerate is left out of the source.
vx::DdkNames fsrc_names() {
    cb::OrderedMap<std::string, std::vector<std::string>> tables;
    const auto add = [&](const char* device, std::initializer_list<std::pair<std::size_t, const char*>> names) {
        std::vector<std::string>& list = tables[device];
        for (const auto& [ordinal, name] : names) {
            if (list.size() <= ordinal) list.resize(ordinal + 1);
            list[ordinal] = name;
        }
    };
    add("VMM", {{81, "_HeapFree"}, {144, "Hook_Device_Service"}, {155, "List_Create"}, {156, "List_Destroy"}, {157, "List_Allocate"}, {159, "List_Attach_Tail"},
                {161, "List_Remove"}, {162, "List_Deallocate"}, {163, "List_Get_First"}, {164, "List_Get_Next"}, {165, "List_Remove_First"}, {284, "Unhook_Device_Service"}});
    add("DOSMGR", {{4, "DOSMGR_Get_IndosPtr"}});
    add("IFSMGR", {{13, "IFSMgr_GetHeap"}, {14, "IFSMgr_RetHeap"}, {17, "IFSMgr_FillHeapSpare"}});
    add("VNETBIOS", {{0, "VNETBIOS_Get_Version"}, {2, "VNETBIOS_Submit"}});
    return vx::DdkNames(vx::ServiceTables(std::move(tables)), {}, {});
}
bool fsrc_has(std::string_view text, std::string_view part) { return text.find(part) != std::string_view::npos; }

}  // namespace

CB_TEST("source.vxd.from_source.the_tags_of_the_object_table", [] {
    const le::Module splitter = le::Module::open(fsrc_path("splitter.vxd"));
    CB_CHECK(fsrc::object_names(splitter) == (std::vector<std::string>{"LCOD"}));
    const le::Module bios = le::Module::open(fsrc_path("bios.vxd"));
    const std::vector<std::string> tags = fsrc::object_names(bios);
    CB_CHECK_EQ(tags.size(), std::size_t{4});
    CB_CHECK_EQ(tags[0], "LCOD");
    CB_CHECK_EQ(tags[1], std::string("PNP\0", 4));   // a tag the linker padded with a NUL
    CB_CHECK_EQ(tags[2], "PDAT");
    CB_CHECK_EQ(tags[3], "ICOD");
});

CB_TEST("source.vxd.from_source.a_reserved_word_gets_another_segment_name", [] {
    CB_CHECK_EQ(fsrc::segment_name("LCOD"), "LCOD");
    CB_CHECK_EQ(fsrc::segment_name("LOCK"), "_LOCK");   // MASM's `LOCK SEGMENT` is error A2008
    CB_CHECK_EQ(fsrc::segment_name("lock"), "_lock");   // (case does not matter to MASM)
    CB_CHECK_EQ(fsrc::segment_name("PTR"), "_PTR");
    CB_CHECK_EQ(fsrc::segment_name("PDAT"), "PDAT");
});

CB_TEST("source.vxd.from_source.every_object_gets_a_unique_valid_segment", [] {
    // 1: the DDB object (the locked-code macro's own); 2: a plain tag; 3: blank; 4: NUL-filled; 5: a reserved word padded with a NUL; 6: begins with a
    // digit; 7: a repeat of 2; 8: a repeat of 2 again; 9: a character MASM does not take in a name
    const std::vector<std::string> tags = {"LCOD", "PCOD", "    ", std::string("\0\0\0\0", 4), std::string("LOCK\0", 5), "1abc", "PCOD", "PCOD", "A-B"};
    const auto segments = fsrc::object_segments(tags, 1);
    CB_CHECK_EQ(segments.size(), std::size_t{8});
    CB_CHECK(!segments.contains(1));
    CB_CHECK_EQ(segments.at(2).first, "PCOD");
    CB_CHECK_EQ(segments.at(2).second, "PCODCODE");
    CB_CHECK_EQ(segments.at(3).first, "SEG3");
    CB_CHECK_EQ(segments.at(4).first, "SEG4");
    CB_CHECK_EQ(segments.at(5).first, "_LOCK");
    CB_CHECK_EQ(segments.at(6).first, "SEG6");
    CB_CHECK_EQ(segments.at(7).first, "PCOD_2");   // the second PCOD
    CB_CHECK_EQ(segments.at(8).first, "PCOD_3");
    CB_CHECK_EQ(segments.at(9).first, "SEG9");
    CB_CHECK_EQ(segments.at(7).second, "PCOD_2CODE");
    // `_` and the characters of the other ranges MASM allows in a name
    const auto allowed = fsrc::object_segments(std::vector<std::string>{"X", "_a$?", "a@1"}, 1);
    CB_CHECK_EQ(allowed.at(2).first, "_a$?");
    CB_CHECK_EQ(allowed.at(3).first, "a@1");
});

CB_TEST("source.vxd.from_source.the_def_lists_the_segments_preloaded", [] {
    const std::vector<std::string> tags = {"LCOD", "PNP\0", "PDAT", "ICOD"};
    const std::string def = fsrc::realseg_def("BIOS", tags, 1);
    CB_CHECK_EQ(def,
                "VXD BIOS\r\nDESCRIPTION 'BIOS reconstructed from source'\r\n\r\nSEGMENTS\r\n"
                "    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE\r\n"
                "    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE\r\n"
                "    PNP      CLASS 'PNPCODE' PRELOAD NONDISCARDABLE\r\n"
                "    PDAT     CLASS 'PDATCODE' PRELOAD NONDISCARDABLE\r\n"
                "    ICOD     CLASS 'ICODCODE' PRELOAD NONDISCARDABLE\r\n"
                "\r\nEXPORTS\r\n        BIOS_DDB @1\r\n");
});

CB_TEST("source.vxd.from_source.the_source_has_each_object_in_its_segment", [] {
    const vx::DdkNames none;
    const le::Module bios = le::Module::open(fsrc_path("bios.vxd"));
    vx::masm::Emission code(vx::Disassembly::run(bios, none), none);
    const std::optional<std::uint32_t> tail = fsrc::reclassify_ddb_tail_as_data(code, bios);
    const std::vector<std::string> tags = fsrc::object_names(bios);
    const std::string source = fsrc::realseg_source(code, tags);
    CB_CHECK(source.ends_with("\r\n"));
    CB_CHECK(!fsrc_has(source, "VSEG"));   // no synthetic segment is left
    // the other objects are declared in the flat model, right after the includes
    for (const char* name : {"PNP", "PDAT", "ICOD"}) CB_CHECK(fsrc_has(source, std::string(name) + " SEGMENT DWORD PUBLIC _FLAT '" + name + "CODE'\r\n" + name + " ENDS\r\n"));
    // the walk of each object is in its own segment: declared (empty) in the flat model first, then opened again with the code
    for (const char* name : {"PNP", "PDAT", "ICOD"}) {
        std::size_t closes = 0;
        for (std::size_t at = source.find(std::string(name) + " ENDS\r\n"); at != std::string::npos; at = source.find(std::string(name) + " ENDS\r\n", at + 1)) ++closes;
        CB_CHECK_EQ(closes, std::size_t{2});
        CB_CHECK(fsrc_has(source, "\r\n" + std::string(name) + " SEGMENT\r\n"));
    }
    CB_CHECK(fsrc_has(source, "\r\n_LTEXT SEGMENT\r\n"));   // the DDB object's: the DDK's own locked-code segment, as the emitter names it
    const std::size_t include = source.find("include vmm.inc");
    CB_CHECK(include != std::string::npos);
    CB_CHECK(include < source.find("PNP SEGMENT DWORD PUBLIC _FLAT"));
    (void)tail;
});

CB_TEST("source.vxd.from_source.splitter_comes_back_as_the_original", [] {
    // with no assembler the file is the LE writer's: the original, byte for byte
    const vx::DdkNames names = fsrc_names();
    const vx::masm::Toolchain no_tools;
    const fsrc::Environment env{names, no_tools, [](cb::ByteView, std::string_view, const std::filesystem::path&) { return vx::link::Linked{}; }};
    const le::Module splitter = le::Module::open(fsrc_path("splitter.vxd"));
    const fsrc::Built built = fsrc::build(env, splitter);
    // (the whole file is the original byte for byte when the DDK's own names are given -- `vxd from-source` on the corpus's splitter.vxd; the few names written by
    // hand here leave the order of some fix-up records another, so what is compared is what loads: the objects and their bytes)
    const le::Module made = le::Module::from_buffer(cb::Bytes(built.vxd));
    CB_CHECK_EQ(made.objects().size(), splitter.objects().size());
    CB_CHECK(made.object_data(made.objects().front()) == splitter.object_data(splitter.objects().front()));
    CB_CHECK_EQ(built.vxd_length, std::size_t{2596});
    CB_CHECK_EQ(built.oracle_length, std::size_t{2596});
    CB_CHECK_EQ(built.objects, std::size_t{1});
    CB_CHECK(built.page_map_identity);
    CB_CHECK_EQ(built.device_id, std::uint16_t{0x0498});
    CB_CHECK(built.device_id_ok);
    CB_CHECK_EQ(built.resource_length, std::uint32_t{512});
    CB_CHECK_EQ(built.object1_diffs, std::size_t{0});
    CB_CHECK(built.ddb_offset.has_value() && built.ddb_offset == built.oracle_ddb_offset);
    CB_CHECK(built.source.has_value() && built.def.has_value());
    CB_CHECK(built.def->starts_with("VXD Splitter\r\n"));
    CB_CHECK(built.compile_ok.has_value() && !*built.compile_ok);   // nothing to assemble it with
    CB_CHECK(built.link_ok.has_value() && !*built.link_ok);
    CB_CHECK(fsrc_has(built.link_log, "no assembler"));
    // the name can be given
    const fsrc::Built named = fsrc::build(env, splitter, "OTHER");
    CB_CHECK(named.def->starts_with("VXD OTHER\r\n"));
});

CB_TEST("source.vxd.from_source.pages_of_another_size_go_through_the_le_writer_alone", [] {
    const vx::DdkNames none;
    const vx::masm::Toolchain no_tools;
    const fsrc::Environment env{none, no_tools, [](cb::ByteView, std::string_view, const std::filesystem::path&) { return vx::link::Linked{}; }};
    const le::Module bios = le::Module::open(fsrc_path("bios.vxd"));   // 512-byte pages
    const fsrc::Built built = fsrc::build(env, bios);
    CB_CHECK(!built.source.has_value());   // (no macro source: nothing was assembled)
    CB_CHECK(!built.compile_ok.has_value());
    CB_CHECK_EQ(built.objects, std::size_t{4});
    CB_CHECK(built.page_map_identity);
    CB_CHECK_EQ(built.oracle_length, cb::fs::file_size(fsrc_path("bios.vxd")));
});

CB_TEST("source.vxd.from_source.the_command_writes_the_file_and_says_what_it_made", [] {
    cb::fs::ScopedTempDir dir("fromsrctest");
    const vx::DdkNames names = fsrc_names();
    const vx::masm::Toolchain no_tools;
    const fsrc::Environment env{names, no_tools, [](cb::ByteView, std::string_view, const std::filesystem::path&) { return vx::link::Linked{}; }};
    const std::string splitter = cb::fs::to_text(fsrc_path("splitter.vxd"));
    const std::string target = cb::fs::to_text(dir.path() / "splitter.built.vxd");
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(fsrc::main(env, std::vector<std::string>{splitter, target}), cb::kExitOk);
    const std::string said = cap.out();
    CB_CHECK(fsrc_has(said, "built 2596 bytes (oracle 2596); objects=1; page map identity=True; "));
    CB_CHECK(fsrc_has(said, "obj1 diffs=0"));
    CB_CHECK(fsrc_has(said, "wrote "));
    const le::Module written = le::Module::open(dir.path() / "splitter.built.vxd");   // (a file that opens as a driver, of the original's size)
    CB_CHECK_EQ(written.data().size(), std::size_t{2596});
    CB_CHECK_EQ(written.objects().size(), std::size_t{1});
});

CB_TEST("source.vxd.from_source.splitter_compiles_and_links_with_the_period_tools", [] {
    CB_WINDOWS_ONLY("it runs the period compiler and linker (Windows programs)");
    // only where the DDK (its includes, MASM 6.11d, LINK 2.60) and its names are set up (the settings, CB_CHECKOUT_ROOT): then the readable source is
    // assembled and linked, and the file written is the LE writer's: the original's size and objects
    const cb::config::Config& config = cb::config::current();
    const std::filesystem::path tools_root = std::filesystem::is_directory(config.checkout_root() / "toolchains") ? config.checkout_root() : config.main_checkout();
    std::error_code ec;
    if (!std::filesystem::is_regular_file(tools_root / "toolchains" / "period" / "sdks" / "nt351-ddk" / "DDK" / "BIN" / "I386" / "FREE" / "ML.EXE", ec)) return;
    cb::base::Corpus corpus(config.products());
    std::optional<std::filesystem::path> design;
    try {
        design = corpus.design_dir();
    } catch (const cb::Error&) {
    }
    const vx::DdkFolders ddk = vx::DdkFolders::standard(corpus, config.main_checkout().parent_path());
    const auto inc = vx::DdkFolders::first_existing(ddk.inc);
    if (!inc || !std::filesystem::is_regular_file(*inc / "VMM.INC", ec)) return;
    const std::filesystem::path ddk_root = inc->parent_path();
    if (!std::filesystem::is_regular_file(ddk_root / "MSVC20" / "LINK.EXE", ec)) return;
    const vx::DdkNames names = vx::DdkNames::standard(ddk, design);
    const vx::masm::Toolchain tools = vx::masm::Toolchain::standard(ddk, tools_root);
    cb::fs::ScopedTempDir stage("fromsrcstage");
    const fsrc::Environment env{names, tools, vx::link::link_260(ddk_root / "MSVC20", stage.path() / "link260", 0x1000)};
    const le::Module splitter = le::Module::open(fsrc_path("splitter.vxd"));
    const fsrc::Built built = fsrc::build(env, splitter);
    CB_CHECK(built.compile_ok.value_or(false));
    CB_CHECK(built.link_ok.value_or(false));
    const le::Module made = le::Module::from_buffer(cb::Bytes(built.vxd));
    CB_CHECK_EQ(built.vxd.size(), built.oracle_length);
    CB_CHECK(made.object_data(made.objects().front()) == splitter.object_data(splitter.objects().front()));
    CB_CHECK_EQ(built.object1_diffs, std::size_t{0});
});

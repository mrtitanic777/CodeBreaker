// cb-unity: alone (test helpers named for this module)
// Tests of cb/source/vxd/tree on the real driver SPLITTER.VXD of formats/testdata. The golden tree (testdata/tree_splitter) is what the module
// wrote for it (compared with the lines' CR left out: a text-mode write turns each CRLF into CR CR LF), read
// through the DDK's names; the tests with no DDK use the same names written by hand.
#include "source/vxd/tree.hpp"

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace vx = cb::source::vxd;
namespace tr = cb::source::vxd::tree;
namespace le = cb::formats::le;

namespace {

le::Module tree_driver(const char* name) { return le::Module::open(cb::testing::data_path(std::string("formats/testdata/") + name)); }

std::string tree_golden(const char* name) {
    return cb::text::replace_all(cb::fs::read_text(cb::testing::data_path(std::string("source/vxd/testdata/tree_splitter/") + name)), "\r\n", "\n");
}

std::string tree_read(const std::filesystem::path& p) { return cb::text::replace_all(cb::fs::read_text(p), "\r\n", "\n"); }

// The services splitter calls, at their ordinals (the DDK's INC32).
vx::DdkNames tree_names() {
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

}  // namespace

CB_TEST("source.vxd.tree.segment_class_by_flags", [] {
    const auto check = [](std::uint32_t flags, std::string_view cls, std::string_view segment) {
        const tr::SegmentClass c = tr::segment_class(flags);
        CB_CHECK_EQ(std::string(c.class_name), std::string(cls));
        CB_CHECK_EQ(std::string(c.segment), std::string(segment));
    };
    check(0x01, "LDATA", "_LDATA");           // not executable: data
    check(0x13, "IDATA", "_IDATA");           // not executable and discardable: init data
    check(0x8005, "MCODE", "_LMSGTABLE");     // IOPL: the message-hook table
    check(0x15, "ICODE", "_ITEXT");           // executable and discardable: init code
    check(0x45, "LCODE", "_LTEXT");           // preload (and locked)
    check(0x05, "PCODE", "_PTEXT");           // pageable
    check(0x8015, "MCODE", "_LMSGTABLE");     // IOPL wins over discardable
});

CB_TEST("source.vxd.tree.error_lines_of_each_assembler", [] {
    const std::string nasm = "a.asm:12: error: parser: instruction expected\nA.ASM:7: Fatal: out of memory\nx:3: warning: nothing\nb.asm:99:  ERROR: shouted\n";
    CB_CHECK(tr::error_lines(nasm, tr::Probe::Kind::Nasm) == (std::vector<std::size_t>{12, 7, 99}));
    const std::string wasm = "a.asm(31): Error! E032: nope\na.asm(32): Warning! W001: fine\na.asm(5): error: lower\nfoo (8)\n";
    CB_CHECK(tr::error_lines(wasm, tr::Probe::Kind::Wasm) == (std::vector<std::size_t>{31, 5}));
    CB_CHECK(tr::error_lines("", tr::Probe::Kind::Nasm).empty());
    CB_CHECK(tr::error_lines("a.asm:12: error", tr::Probe::Kind::Wasm).empty());   // the other assembler's spelling is not this one's
});

CB_TEST("source.vxd.tree.the_entry_table_names_the_ddb", [] {
    const le::Module splitter = tree_driver("splitter.vxd");
    const auto ddb = tr::entry_table_ddb(splitter);
    CB_CHECK(ddb.has_value());
    const vx::DdkNames none;
    const cb::source::vxd::masm::Emission code(vx::Disassembly::run(splitter, none), none);
    CB_CHECK(code.ddb().has_value());
    CB_CHECK_EQ(ddb->first, code.ddb()->object);
    CB_CHECK_EQ(static_cast<std::int64_t>(ddb->second), code.ddb()->rva);
});

CB_TEST("source.vxd.tree.splitter_tree_matches_the_recorded_output", [] {
    const le::Module splitter = tree_driver("splitter.vxd");
    cb::fs::ScopedTempDir out("treetest");
    const vx::DdkNames names = tree_names();
    const std::vector<std::string> files = tr::emit_tree(splitter, "SPLITTER", out.path(), names);
    CB_CHECK(files == (std::vector<std::string>{"splitter_lcode_01.asm"}));   // one object, written whole
    CB_CHECK_EQ(tree_read(out.path() / "splitter" / "splitter_lcode_01.asm"), tree_golden("splitter_lcode_01.asm"));
    CB_CHECK_EQ(tree_read(out.path() / "splitter" / "splitter.txt"), tree_golden("splitter.txt"));
    // CRLF line ends, written as they are
    const std::string raw = cb::fs::read_text(out.path() / "splitter" / "splitter_lcode_01.asm");
    CB_CHECK(raw.find("\r\r") == std::string::npos);
    CB_CHECK(raw.find("\r\n") != std::string::npos);
    CB_CHECK(raw.find("\n") == raw.find("\r\n") + 1);
    CB_CHECK(raw.find("public VXD_DDB") != std::string::npos);   // the DDB's place is exported, as the linker wants it
});

CB_TEST("source.vxd.tree.an_object_is_split_at_functions_and_raw_instructions_are_db", [] {
    const le::Module splitter = tree_driver("splitter.vxd");
    cb::fs::ScopedTempDir out("treetest");
    const vx::DdkNames names = tree_names();
    tr::Options small;
    small.max_instructions = 40;   // splitter has about 500
    const std::vector<std::string> parts = tr::emit_tree(splitter, "SPLITTER", out.path(), names, small);
    CB_CHECK(parts.size() >= 2);
    CB_CHECK_EQ(parts.front(), "splitter_lcode_01_01.asm");
    const std::string index = tree_read(out.path() / "splitter" / "splitter.txt");
    CB_CHECK(index.find("(part 1/") == std::string::npos);   // (the part note is in each file's header, the index lists ranges)
    CB_CHECK(index.find(parts.back()) != std::string::npos);
    CB_CHECK(tree_read(out.path() / "splitter" / parts.front()).find("(part 1/" + std::to_string(parts.size()) + ")") != std::string::npos);

    // a byte-exact build: the instructions of the force-raw set are db
    cb::fs::ScopedTempDir raw_out("treetest");
    cb::source::vxd::link::ForceRaw force;
    const cb::source::vxd::masm::Emission code(vx::Disassembly::run(splitter, names), names);
    for (const auto& [place, instruction] : code.instructions()) force[place.object].insert(static_cast<std::uint32_t>(place.offset));
    tr::Options raw;
    raw.force_raw = &force;
    tr::emit_tree(splitter, "SPLITTER", raw_out.path(), names, raw);
    const std::string all_raw = tree_read(raw_out.path() / "splitter" / "splitter_lcode_01.asm");
    CB_CHECK(all_raw.find("        push   ebp") == std::string::npos);   // not one instruction is written as text
    CB_CHECK(all_raw.find("        db  0") != std::string::npos);
});

CB_TEST("source.vxd.tree.the_command_writes_a_tree_for_each_driver_of_the_file", [] {
    cb::fs::ScopedTempDir out("treetest");
    const vx::DdkNames names = tree_names();
    const tr::Environment env{names, out.path() / "default"};
    const std::string splitter = cb::fs::to_text(cb::testing::data_path("formats/testdata/splitter.vxd"));
    std::string said;
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> line = {splitter};
        CB_CHECK_EQ(tr::main(env, line), cb::kExitOk);   // to the default folder
        said = cap.out();
    }
    CB_CHECK(said.find("splitter.vxd") != std::string::npos);
    CB_CHECK(said.find("-> 1 files in ") != std::string::npos);
    CB_CHECK(std::filesystem::is_regular_file(out.path() / "default" / "splitter" / "splitter_lcode_01.asm"));
    {
        cb::testing::CapturedStd cap;
        const std::vector<std::string> line = {splitter, cb::fs::to_text(out.path() / "named")};
        CB_CHECK_EQ(tr::main(env, line), cb::kExitOk);
    }
    CB_CHECK(std::filesystem::is_regular_file(out.path() / "named" / "splitter" / "splitter.txt"));
    cb::testing::CapturedStd cap;   // (the usage goes to stderr)
    CB_CHECK_EQ(tr::main(env, std::vector<std::string>{}), cb::kExitUsage);
});

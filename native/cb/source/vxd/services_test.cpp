// Tests of cb/source/vxd/services: include files written for the Python module's harness (testdata/inc_a, inc_dup:
// tables in every shape -- a second table of a device, a table with no services, a table with no end marker, names in
// other cases, a file that is not an .INC) and DDK folder trees built in a temporary directory. The expected answers
// are what the Python module gave on the same input; the real DDK's tables (37 devices, VMM's 0..., "Get_System_Time"
// at 3Fh) gave the same names and counts.
//
// DIVERGES from the Python module, by design: the files of the include folder are read in the order of their names
// (the Python took the order the operating system listed them in, which was the same on NTFS); the process keeps no
// cache of the folders it has read (the caller holds the tables it loaded); devices are matched in capitals of ASCII
// letters only; a `vmm.inc` in lower case is a VMM.INC
// (the Python tested the file name with case but the folder name without, and so missed it).
#include "cb/source/vxd/services.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace vxd = cb::source::vxd;

namespace {

using Names = std::vector<std::string>;

std::filesystem::path inc(const char* name) { return cb::testing::data_path(std::string("source/vxd/testdata/") + name); }

Names of(const vxd::ServiceTables& tables, const char* device) {
    const Names* names = tables.names(device);
    return names != nullptr ? *names : Names{"(no table)"};
}

void touch(const std::filesystem::path& file) { cb::fs::write_text(file, "x"); }

}  // namespace

CB_TEST("source.vxd.services.tables_of_a_folder", [] {
    const vxd::ServiceTables tables = vxd::ServiceTables::load(inc("inc_a"));
    std::vector<std::string> devices;
    for (const auto& [device, names] : tables.tables()) devices.push_back(device);
    // A.INC, B.INC, C.INC, D.INC, E.inc, ... in the order of the names; F.INC.txt and the empty files give nothing
    CB_CHECK(devices == (Names{"VMM", "VKD", "VPICD", "SHELL", "CR", "X", "Z"}));
    CB_CHECK(of(tables, "VMM") == (Names{"Get_VMM_Version", "Get_Cur_VM_Handle", "lower_case_service", "Dup"}));   // the first table wins; VMM_ServiceX and VXD_Service are other lines
    CB_CHECK(of(tables, "VKD") == (Names{"VKD_Get_Version", "VKD_Define_Hot_Key", "VKD_Remove_Hot_Key"}));   // no End_Service_Table: to the end of the file; C.INC's lower-case VKD table comes second
    CB_CHECK(of(tables, "VPICD") == (Names{"VPICD_Get_Version", "VPICD_Virtualize_IRQ"}));                 // C.INC's empty table does not count
    CB_CHECK(of(tables, "SHELL") == (Names{"SHELL_Get_Version"}));                                        // E.inc: the extension in lower case
    CB_CHECK(of(tables, "CR") == (Names{"One", "Two", "Three"}));                                         // a lone \r ends a line
    CB_CHECK(tables.names("CAFÉ") == nullptr);   // DIVERGES: I.INC names a device `café` and its services `cafÉ_Service`; the Python matched the letters of other cases, here only ASCII letters are told apart by case, so the table has no services
    CB_CHECK(tables.names("café") == nullptr);
    CB_CHECK(tables.names("NOTHERE") == nullptr);                                                            // F.INC.txt is not an include
    CB_CHECK(tables.names("Y") == nullptr);
});

CB_TEST("source.vxd.services.devices_are_matched_in_capitals", [] {
    const vxd::ServiceTables tables = vxd::ServiceTables::load(inc("inc_a"));
    CB_CHECK(tables.names("vmm") == tables.names("VMM"));
    CB_CHECK(tables.names("Vmm") != nullptr);
    CB_CHECK(tables.names("") == nullptr);
    CB_CHECK(tables.names("M") == nullptr);
});

CB_TEST("source.vxd.services.name_by_ordinal", [] {
    const vxd::ServiceTables tables = vxd::ServiceTables::load(inc("inc_a"));
    CB_CHECK(tables.name("VMM", 0) == std::optional<std::string>("Get_VMM_Version"));
    CB_CHECK(tables.name("vmm", 1) == std::optional<std::string>("Get_Cur_VM_Handle"));
    CB_CHECK(tables.name("VMM", 3) == std::optional<std::string>("Dup"));
    CB_CHECK(!tables.name("VMM", 4).has_value());          // past the table
    CB_CHECK(!tables.name("NOPE", 0).has_value());         // no such device
    // the top bit is a call-convention flag on some references: the low 15 bits are tried when the number is past the table
    CB_CHECK(tables.name("VMM", 0x8001) == std::optional<std::string>("Get_Cur_VM_Handle"));
    CB_CHECK(tables.name("VMM", 0x10000) == std::optional<std::string>("Get_VMM_Version"));   // (the device id in the high word is no flag)
    CB_CHECK(!tables.name("VMM", 0x7FFF).has_value());
    CB_CHECK(!tables.name("VMM", 0xFFFFFFFFu).has_value());
});

CB_TEST("source.vxd.services.first_table_of_a_device_wins_across_files", [] {
    const vxd::ServiceTables tables = vxd::ServiceTables::load(inc("inc_dup"));
    CB_CHECK_EQ(tables.tables().size(), std::size_t{1});
    CB_CHECK(of(tables, "DUP") == (Names{"from_one"}));   // 1.INC before 2.INC
});

CB_TEST("source.vxd.services.empty_and_missing_folders", [] {
    cb::fs::ScopedTempDir tmp("cb-svc");
    CB_CHECK(vxd::ServiceTables::load(tmp.path()).tables().empty());
    CB_CHECK(vxd::ServiceTables::load(tmp.path() / "missing").tables().empty());
    cb::fs::write_text(tmp.path() / "EMPTY.INC", "");
    cb::fs::make_dirs(tmp.path() / "G.INC");   // a folder named like an include is not read
    CB_CHECK(vxd::ServiceTables::load(tmp.path()).tables().empty());
});

CB_TEST("source.vxd.services.parse_one_text", [] {
    vxd::ServiceTables tables;
    vxd::ServiceTables::parse_into(tables, "Begin_Service_Table A, A\nA_Service One\nA_Service Two\nEnd_Service_Table\n");
    vxd::ServiceTables::parse_into(tables, "Begin_Service_Table A\nA_Service Again\nEnd_Service_Table\nBegin_Service_Table B\nB_Service Only\n");
    CB_CHECK(of(tables, "A") == (Names{"One", "Two"}));
    CB_CHECK(of(tables, "B") == (Names{"Only"}));
    vxd::ServiceTables none;
    vxd::ServiceTables::parse_into(none, "Begin_Service_Table\nBegin_Service_TableX\nno tables here");
    CB_CHECK(none.tables().empty());
});

CB_TEST("source.vxd.services.the_inc_folder_among_the_candidates", [] {
    cb::fs::ScopedTempDir tmp("cb-svc");
    cb::fs::make_dirs(tmp.path() / "one");
    cb::fs::make_dirs(tmp.path() / "two");
    cb::fs::write_text(tmp.path() / "file.INC", "x");
    vxd::DdkFolders ddk;
    ddk.inc = {tmp.path() / "missing", tmp.path() / "file.INC", {}, tmp.path() / "one", tmp.path() / "two"};
    // a candidate that is not a folder (or is empty text) is passed over; the first folder is the answer
    CB_CHECK(vxd::ServiceTables::find_inc_folder(ddk, std::nullopt) == tmp.path() / "one");
    CB_CHECK(vxd::ServiceTables::standard(ddk, std::nullopt).tables().empty());
    ddk.inc = {tmp.path() / "missing"};
    CB_CHECK(!vxd::ServiceTables::find_inc_folder(ddk, std::nullopt).has_value());
    CB_CHECK(!vxd::ServiceTables::find_inc_folder(ddk, tmp.path() / "missing").has_value());
});

CB_TEST("source.vxd.services.the_bounded_walk", [] {
    const auto walk = [](const std::function<void(const std::filesystem::path&)>& build) -> std::optional<std::string> {
        cb::fs::ScopedTempDir tmp("cb-svc");
        const auto design = tmp.path() / "design";
        cb::fs::make_dirs(design);
        build(design);
        const auto found = vxd::ServiceTables::find_inc_folder(vxd::DdkFolders{}, design);
        if (!found) return std::nullopt;
        return cb::fs::to_text(std::filesystem::relative(*found, design));
    };
    using P = std::filesystem::path;
    CB_CHECK(walk([](const P& d) { touch(d / "INC32" / "VMM.INC"); }) == std::optional<std::string>("INC32"));
    CB_CHECK(walk([](const P& d) { touch(d / "a" / "b" / "INC32" / "VMM.INC"); }) == std::optional<std::string>("a/b/INC32"));   // three levels down
    CB_CHECK(!walk([](const P& d) { touch(d / "a" / "b" / "c" / "INC32" / "VMM.INC"); }).has_value());                         // four: too deep
    CB_CHECK(walk([](const P& d) { touch(d / "a" / "INC32" / "INC32" / "VMM.INC"); }) == std::optional<std::string>("a/INC32/INC32"));
    CB_CHECK(!walk([](const P& d) { touch(d / "a" / "INC32" / "sub" / "INC32" / "VMM.INC"); }).has_value());
    CB_CHECK(!walk([](const P& d) { touch(d / "INC32" / "OTHER.INC"); }).has_value());          // no VMM.INC in it
    CB_CHECK(!walk([](const P& d) { touch(d / "readme.txt"); }).has_value());
    CB_CHECK(!walk([](const P& d) { touch(d / "INC32"); }).has_value());                        // a file called INC32
    // the first INC32 with a VMM.INC in the order of the names
    CB_CHECK(walk([](const P& d) {
                 touch(d / "a" / "INC32" / "OTHER.INC");
                 touch(d / "m" / "INC32" / "VMM.INC");
                 touch(d / "z" / "INC32" / "VMM.INC");
             }) == std::optional<std::string>("m/INC32"));
    // version control, build, corpus and hidden folders are not searched
    CB_CHECK(!walk([](const P& d) {
                  for (const char* skipped : {".git", ".hidden", "binaries", "corpus", "node_modules", "venv", "open-watcom-v2"}) {
                      touch(d / skipped / "INC32" / "VMM.INC");
                  }
              }).has_value());
    // a lower-case vmm.inc in a lower-case inc32 folder counts: file names are not told apart by case on Windows
    CB_CHECK(walk([](const P& d) { touch(d / "inc32" / "vmm.inc"); }) == std::optional<std::string>("inc32"));
});

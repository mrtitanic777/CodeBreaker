// Tests of cb/source/vxd/symbols and ddk: the DDK folders written for the Python module's harness (testdata/ddk/<case>):
// a VMM.INC and .SYM files built to hit every edge of the two readers (equate lines in all their forms, names at the
// limits of the allowed length, records cut off by the end of the file, markers, duplicate offsets). The expected
// answers are what the Python module gave on the same folders, each read and checked by hand; the real DDK
// (Windows95 corpus: 37 control messages, SYS_CRITICAL_INIT to SYS_VM_TERMINATE2 and beyond) gave the same list.
//
// DIVERGES from the Python module, by design: a device named with wildcard characters (`V*`, `v?d`) is a name, not a
// glob pattern; a name with a non-ASCII letter is not looked up as the file `NAME.SYM` is not either way.
#include "cb/source/vxd/symbols.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace vxd = cb::source::vxd;

namespace {

vxd::DdkFolders ddk(const std::string& name) {
    const auto root = cb::testing::data_path("source/vxd/testdata/ddk/" + name) / "Microsoft Windows 95 DDK";
    vxd::DdkFolders folders;
    folders.inc = {root / "INC32"};
    folders.debug = {root / "DEBUG"};
    return folders;
}

using Messages = std::map<std::uint32_t, std::string>;
using Names = std::vector<std::pair<unsigned, std::string>>;

Names list(const cb::OrderedMap<std::uint16_t, std::string>& m) {
    Names out;
    for (const auto& [offset, name] : m) out.emplace_back(offset, name);
    return out;
}

}  // namespace

CB_TEST("source.vxd.symbols.control_messages_of_a_vmm_inc", [] {
    // Early_Const (before the start), flag/bit/id/DIOC names, a value of 40h or more, a value too large for 32 bits,
    // a name that is no identifier, lines that are no `NAME EQU nnH`, a duplicate value; zero-padded and mixed-case
    // numbers; the block ends at SYSTEM_EXIT2
    const Messages expected = {{0, "SYS_CRITICAL_INIT"}, {1, "Device_Init"},           {3, "Sys_VM_Init"},
                               {4, "Sys_VM_Terminate"},  {12, "Name_With_9_Digits_123"}, {18, "Zero_Padded"},
                               {19, "Trailing_Space"},   {20, "SYSTEM_EXIT2"}};
    CB_CHECK(vxd::control_messages(ddk("ref_a")) == expected);
});

CB_TEST("source.vxd.symbols.control_messages_with_old_line_endings", [] {
    // lone \r and \r\n end lines, \x0b and \x0c are blanks before a name, and \x85 inside a name ruins the line
    const Messages expected = {{0, "SYS_CRITICAL_INIT"}, {1, "Second_One"}, {3, "Fourth"}, {4, "Fifth"}, {5, "SYSTEM_EXIT2"}};
    CB_CHECK(vxd::control_messages(ddk("ref_cr")) == expected);
});

CB_TEST("source.vxd.symbols.control_messages_edges_of_the_block", [] {
    CB_CHECK(vxd::control_messages(ddk("ref_noexit")) == (Messages{{0, "SYS_CRITICAL_INIT"}, {1, "A_B"}, {63, "C_D"}}));   // up to 3Fh, no end marker
    CB_CHECK(vxd::control_messages(ddk("ref_early_exit")) == (Messages{{1, "SYS_CRITICAL_INIT"}, {2, "A"}}));
    CB_CHECK(vxd::control_messages(ddk("ref_exit_first_after_start")) ==
             (Messages{{0, "SYS_CRITICAL_INIT"}, {1, "SYSTEM_EXIT2"}}));
    CB_CHECK(vxd::control_messages(ddk("ref_nostart")).empty());    // never starts
    CB_CHECK(vxd::control_messages(ddk("ref_empty_inc")).empty());
    CB_CHECK(vxd::control_messages(ddk("ref_noinc")).empty());      // no VMM.INC at all
    CB_CHECK(vxd::control_messages(vxd::DdkFolders{}).empty());     // no folders
});

CB_TEST("source.vxd.symbols.parse_control_messages_from_text", [] {
    const std::string text =
        "SYS_CRITICAL_INIT EQU 0H\nDevice_Init EQU 1H\n  Pad   EQU   2H   \nlower equ 3H\nNo_H EQU 4\nDEVICE_FLAG EQU 5H\nSYSTEM_EXIT2 EQU 6H\nAfter EQU 7H\n";
    CB_CHECK(vxd::parse_control_messages(text) ==
             (Messages{{0, "SYS_CRITICAL_INIT"}, {1, "Device_Init"}, {2, "Pad"}, {6, "SYSTEM_EXIT2"}}));
    CB_CHECK(vxd::parse_control_messages("").empty());
});

CB_TEST("source.vxd.symbols.sym_names", [] {
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "VKD")) ==
             (Names{{0x1000, "VKD_Filter_Keyboard_Input"},
                    {0x1010, "VKD_Put_Byte"},
                    {0x30, "_Two_Underscores_"},   // two underscores: not a marker
                    {0x40, "ABC"},
                    {0x60, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"},   // 40 letters: the longest taken
                    {0xFFFF, "Last_Entry"}}));
    // the file is found by its name without regard to case, whatever case the device is written in
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "vkd")) == list(vxd::sym_names(ddk("ref_a"), "VKD")));
    CB_CHECK(list(vxd::sym_names(ddk("ref_noinc"), "VKD")) == (Names{{1, "Lowercase_File"}}));   // the file is vkd.sym
    CB_CHECK(list(vxd::sym_names(ddk("ref_cr"), "VKD")) == (Names{{1, "Only_One"}}));
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "multi")) == (Names{{1, "First_Name"}, {2, "Second_Name"}}));
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "dup")) == (Names{{7, "Same_Offset_A"}, {8, "Same_Offset_C"}}));   // the first name at an offset wins
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "markers")) == (Names{{2, "_a_b"}, {6, "___"}, {7, "_x_y_z"}}));
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "edge")) == (Names{{0, "Zero"}, {0xFFFF, "Max"}, {258, "Mid_Value"}}));
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "a b")) == (Names{{9, "Spaced_Device"}}));
});

CB_TEST("source.vxd.symbols.sym_names_of_damaged_files", [] {
    // `01 00 03 a b`: a record of three letters that the end of the file cuts at two
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "tiny")) == (Names{{1, "ab"}}));
    CB_CHECK(vxd::sym_names(ddk("ref_a"), "empty").empty());
    CB_CHECK(vxd::sym_names(ddk("ref_a"), "junk").empty());
    CB_CHECK(vxd::sym_names(ddk("ref_a"), "NOPE").empty());
    CB_CHECK(vxd::sym_names(ddk("ref_a"), "").empty());
    CB_CHECK(vxd::sym_names(ddk("ref_a"), "VKD.SYM").empty());
    CB_CHECK(vxd::sym_names(ddk("ref_nostart"), "VKD").empty());   // no DEBUG folder
    // `Ends_Short` whole, then `ABCDE` where the record says ten letters and the file ends: taken as far as it goes
    CB_CHECK(list(vxd::sym_names(ddk("ref_a"), "lone")) == (Names{{5, "Ends_Short"}, {9, "ABCDE"}}));
});

CB_TEST("source.vxd.symbols.parse_sym_names_from_bytes", [] {
    const cb::Bytes sym = {0x10, 0x00, 0x03, 'A', 'B', 'C',     // 0010 ABC
                           0x20, 0x00, 0x02, 'X', 'Y',           // too short: skipped a byte at a time
                           0x30, 0x00, 0x04, 'a', ' ', 'b', 'c', // a space: not a name
                           0x40, 0x00, 0x05, '_', 'S', 'E', 'G', 'X'};   // a marker
    CB_CHECK(list(vxd::parse_sym_names(sym)) == (Names{{0x10, "ABC"}}));
    CB_CHECK(vxd::parse_sym_names({}).empty());
    const cb::Bytes three = {1, 0, 3};
    CB_CHECK(vxd::parse_sym_names(three).empty());   // fewer than four bytes: nothing to read
});

CB_TEST("source.vxd.symbols.ddk_folders", [] {
    cb::fs::ScopedTempDir tmp("cb-ddk");
    cb::fs::make_dirs(tmp.path() / "ref" / "Microsoft Windows 95 DDK" / "INC32");
    cb::base::Products products;
    products.checkout_root = tmp.path() / "checkout";
    products.state = tmp.path() / "state";
    products.reference = tmp.path() / "ref";
    const cb::base::Corpus corpus(products);
    const vxd::DdkFolders folders = vxd::DdkFolders::standard(corpus, tmp.path());
    CB_CHECK_EQ(folders.inc.size(), std::size_t{4});
    CB_CHECK_EQ(folders.debug.size(), std::size_t{2});
    CB_CHECK(folders.inc[0] == tmp.path() / "ref" / "Microsoft Windows 95 DDK" / "INC32");
    CB_CHECK(vxd::DdkFolders::first_existing(folders.inc) == folders.inc[0]);   // the reference folder's is the one there
    CB_CHECK(!vxd::DdkFolders::first_existing(folders.debug).has_value());
    CB_CHECK(!vxd::DdkFolders::first_existing(folders.inc, "VMM.INC").has_value());
    cb::fs::write_text(folders.inc[0] / "VMM.INC", "SYS_CRITICAL_INIT EQU 0H\nSYSTEM_EXIT2 EQU 14H\n");
    CB_CHECK(vxd::DdkFolders::first_existing(folders.inc, "VMM.INC") == folders.inc[0] / "VMM.INC");
    CB_CHECK(vxd::control_messages(folders) == (Messages{{0, "SYS_CRITICAL_INIT"}, {20, "SYSTEM_EXIT2"}}));
});

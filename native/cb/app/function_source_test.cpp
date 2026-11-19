// Tests of function_source.hpp: the functions of an output folder, the counts and filters, a function's two bodies and the lines that correspond.
#include "cb/app/function_source.hpp"

#include <filesystem>
#include <fstream>

#include "cb/core/testing.hpp"

namespace cb::app::fnsrc {
namespace {
namespace stdfs = std::filesystem;

stdfs::path fs_test_folder(const std::string& name) {
    const stdfs::path p = stdfs::temp_directory_path() / ("cb_fnsrc_" + name);
    std::error_code ec;
    stdfs::remove_all(p, ec);
    stdfs::create_directories(p);
    return p;
}

void fs_test_write(const stdfs::path& p, const std::string& text) {
    std::ofstream out(p, std::ios::binary);
    out << text;
}

const char* kC =
    "/* lz32.dll -- decompiled by CodeBreaker */\n"
    "\n"
    "void __cdecl16far sub_00024(void)\n"
    "\n"
    "{\n"
    "  (*(code *)0x10)();\n"
    "  return;\n"
    "}\n"
    "\n"
    "INT WINAPI LZRead(HFILE h, LPSTR buf, INT count) {\n"
    "    LZSTATE *lz = lz_handle_of(h);\n"
    "    if (lz == NULL) return -1;\n"
    "    return lz_copy_out(lz, buf, count);\n"
    "}\n"
    "typedef int (*cb)(int);\n"
    "int table[3] = {1, 2, 3};\n";

const char* kAsm =
    "sub_00024:\n"
    "        shl    bx, 1\n"
    "        retf\n"
    "LZRead:\n"
    "        push   ebp\n"
    "        call   lz_handle_of\n"
    "        test   eax, eax\n"
    "        jz     L_00045\n"
    "L_00045:\n"
    "        call   near ptr lz_copy_out\n"
    "        ret\n"
    "other proc\n"
    "        nop\n"
    "other endp\n";

CB_TEST("app.function_source.names_and_addresses_are_normalised", [] {
    CB_CHECK_EQ(address_of_name("sub_00024"), "0x00024");
    CB_CHECK_EQ(address_of_name("LZRead"), "");
    CB_CHECK_EQ(address_of_name("sub_xyz"), "");
    CB_CHECK_EQ(normal_address("0x0011F0"), "11f0");
    CB_CHECK_EQ(normal_address("11F0"), "11f0");
    CB_CHECK_EQ(normal_address("0x0"), "0");
});

CB_TEST("app.function_source.the_folder_scan_lists_definitions_in_order_and_none_is_judged", [] {
    const stdfs::path dir = fs_test_folder("scan");
    fs_test_write(dir / "lz32.c", kC);
    const auto list = scan_folder(dir);
    CB_CHECK_EQ(list.size(), 2u);   // (not the typedef, not the table)
    CB_CHECK_EQ(list[0].name, "sub_00024");
    CB_CHECK_EQ(list[0].address, "0x00024");
    CB_CHECK_EQ(list[0].source_line, 3);
    CB_CHECK_EQ(list[1].name, "LZRead");
    CB_CHECK_EQ(list[1].source_line, 10);
    CB_CHECK_EQ(list[1].source_file, "lz32.c");
    for (const auto& f : list) CB_CHECK(f.verdict == convert::FunctionVerdict::NotChecked);
    stdfs::remove_all(dir);
});

CB_TEST("app.function_source.an_empty_or_missing_folder_has_no_functions", [] {
    CB_CHECK(scan_folder(stdfs::temp_directory_path() / "cb_fnsrc_missing_folder").empty());
    CB_CHECK(stored_verdicts({}, {}).empty());
    CB_CHECK(stored_verdicts(stdfs::temp_directory_path() / "cb_fnsrc_no_such_file.dll", stdfs::temp_directory_path() / "cb_fnsrc_no_state").empty());
});

CB_TEST("app.function_source.recorded_functions_win_and_store_verdicts_fill_only_what_is_unchecked", [] {
    const stdfs::path dir = fs_test_folder("collect");
    fs_test_write(dir / "lz32.c", kC);
    std::vector<convert::FunctionRecord> recorded(3);
    recorded[0].name = "LZRead";
    recorded[0].address = "0x11F0";
    recorded[1].name = "sub_00024";
    recorded[2].name = "other";
    recorded[2].address = "0x11F0";
    recorded[2].verdict = convert::FunctionVerdict::Matches;
    const std::map<std::string, convert::FunctionVerdict> verdicts = {{"11f0", convert::FunctionVerdict::Differs}, {"24", convert::FunctionVerdict::Matches}};
    const auto list = collect(dir, recorded, verdicts);
    CB_CHECK_EQ(list.size(), 3u);
    CB_CHECK(list[0].verdict == convert::FunctionVerdict::Differs);   // by its address
    CB_CHECK(list[1].verdict == convert::FunctionVerdict::Matches);   // by the address in its name
    CB_CHECK(list[2].verdict == convert::FunctionVerdict::Matches);   // already judged: untouched
    CB_CHECK_EQ(list[0].source_line, 10);                              // the place found in the source
    CB_CHECK_EQ(list[1].source_file, "lz32.c");
    CB_CHECK(list[2].source_file.empty());                             // not defined there
    // no record: the folder's own list, every function unchecked
    const auto scanned = collect(dir, {});
    CB_CHECK_EQ(scanned.size(), 2u);
    CB_CHECK(scanned[0].verdict == convert::FunctionVerdict::NotChecked);
    stdfs::remove_all(dir);
});

CB_TEST("app.function_source.counts_percent_and_filter", [] {
    std::vector<convert::FunctionRecord> list(5);
    list[0].verdict = list[1].verdict = list[2].verdict = convert::FunctionVerdict::Matches;
    list[3].verdict = convert::FunctionVerdict::Differs;
    const Counts c = count(list);
    CB_CHECK_EQ(c.total, 5);
    CB_CHECK_EQ(c.matches, 3);
    CB_CHECK_EQ(c.differs, 1);
    CB_CHECK_EQ(c.unchecked, 1);
    CB_CHECK_EQ(c.percent_alike(), 60);   // of all five: the one nobody ran counts against it
    CB_CHECK_EQ(filtered(list, 0).size(), 5u);
    CB_CHECK(filtered(list, 1) == std::vector<int>{3});
    CB_CHECK(filtered(list, 2) == std::vector<int>{4});
    CB_CHECK_EQ(count(std::vector<convert::FunctionRecord>(4)).percent_alike(), -1);   // nothing judged: no figure
    CB_CHECK_EQ(count({}).percent_alike(), -1);
});

CB_TEST("app.function_source.the_c_body_runs_from_the_signature_to_the_closing_brace", [] {
    const stdfs::path dir = fs_test_folder("cbody");
    fs_test_write(dir / "lz32.c", kC);
    std::vector<std::string> lines;
    {
        std::ifstream in(dir / "lz32.c");
        for (std::string l; std::getline(in, l);) lines.push_back(l);
    }
    int first = -1;
    const auto read = c_body(lines, "LZRead", &first);
    CB_CHECK_EQ(read.size(), 5u);
    CB_CHECK_EQ(first, 9);
    CB_CHECK(read.back() == "}");
    const auto start = c_body(lines, "sub_00024");
    CB_CHECK_EQ(start.size(), 5u);   // (the blank line before its brace is dropped)
    CB_CHECK(start[1] == "{");
    CB_CHECK(c_body(lines, "nothing").empty());
    stdfs::remove_all(dir);
});

CB_TEST("app.function_source.the_asm_body_runs_to_the_next_label_that_is_not_local", [] {
    std::vector<std::string> lines;
    {
        std::string text = kAsm, l;
        for (const char c : text) {
            if (c == '\n') {
                lines.push_back(l);
                l.clear();
            } else {
                l += c;
            }
        }
    }
    const auto body = asm_body(lines, "LZRead");
    CB_CHECK_EQ(body.size(), 8u);   // its local label L_00045 is inside it
    CB_CHECK(body.front() == "LZRead:" && body.back() == "        ret");
    CB_CHECK_EQ(asm_body(lines, "sub_00024").size(), 3u);
    CB_CHECK_EQ(asm_body(lines, "OTHER").size(), 3u);   // proc ... endp, any case
    CB_CHECK(asm_body(lines, "missing").empty());
});

CB_TEST("app.function_source.calls_by_name_link_the_c_line_to_the_assembly_line", [] {
    const std::vector<std::string> a = {"LZRead:", "  push ebp", "  call lz_handle_of", "  call near ptr lz_copy_out", "  call lz_handle_of", "  ret"};
    const std::vector<std::string> c = {"INT LZRead(void)", "{", "  lz = lz_handle_of(h);", "  x = lz_copy_out(lz);", "  y = lz_handle_of(h);", "  if (z) return;", "}"};
    const auto links = link_by_calls(a, c);
    CB_CHECK_EQ(links.size(), 3u);
    CB_CHECK(links[0] == (Link{2, 2, 2, 2}));
    CB_CHECK(links[1] == (Link{3, 3, 3, 3}));
    CB_CHECK(links[2] == (Link{4, 4, 4, 4}));   // the second call of the same function matches the second `call`
    CB_CHECK(link_at(links, Side::C, 3) == &links[1]);
    CB_CHECK(link_at(links, Side::Asm, 4) == &links[2]);
    CB_CHECK(link_at(links, Side::C, 5) == nullptr);
    CB_CHECK(link_by_calls({}, c).empty());
});

CB_TEST("app.function_source.a_pair_is_read_from_the_folder_and_says_what_is_missing", [] {
    const stdfs::path dir = fs_test_folder("pair");
    fs_test_write(dir / "lz32.c", kC);
    fs_test_write(dir / "lz32.asm", kAsm);
    convert::FunctionRecord r;
    r.name = "LZRead";
    r.source_file = "lz32.c";
    const Pair pair = load_pair(dir, r);
    CB_CHECK_EQ(pair.c_lines.size(), 5u);
    CB_CHECK_EQ(pair.asm_lines.size(), 8u);
    CB_CHECK(pair.c_note.empty() && pair.asm_note.empty());
    CB_CHECK_EQ(pair.links.size(), 2u);   // lz_handle_of and lz_copy_out
    convert::FunctionRecord gone;
    gone.name = "nothing";
    const Pair none = load_pair(dir, gone);
    CB_CHECK(none.c_lines.empty() && none.asm_lines.empty() && !none.c_note.empty() && !none.asm_note.empty());
    // a function known only by address finds its sub_ label
    convert::FunctionRecord by_address;
    by_address.name = "start";
    by_address.address = "0x00024";
    CB_CHECK_EQ(load_pair(dir, by_address).asm_lines.size(), 3u);
    stdfs::remove_all(dir);
});

}  // namespace
}  // namespace cb::app::fnsrc

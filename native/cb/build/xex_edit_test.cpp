// Tests of cb/build/xex_edit.hpp on real Xbox 360 modules (Minecraft builds under C:/Nexia360/Games: a test whose module or the xex program
// is not on this machine is skipped). The command's output and the files it writes are compared with what `codebreaker xex-edit ...`
// printed and wrote (testdata/xex_edit/edit.txt: each case's stdout and exit code; edit_files.txt: "name size sha1" of every file it wrote,
// both made by the translated Python).
//
// DIVERGES from the Python: the assembler's error text is the C++ assembler's (the lines are compared up to "xex-edit: ").
#include "cb/build/xex_edit.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/command_test_kit.hpp"

using namespace cb;
namespace xex_edit = cb::build::xex_edit;

namespace {

// One case of the golden's log: "== name\n<stdout>exit=N\n".
std::map<std::string, std::string> xex_edit_cases() {
    std::map<std::string, std::string> cases;
    std::string name;
    const std::string log = fs::read_text(testing::data_path("build/testdata/xex_edit/edit.txt"));
    for (const std::string_view line : text::lines(log)) {
        if (line.starts_with("== ")) {
            name = std::string(line.substr(3));
            continue;
        }
        if (!name.empty()) cases[name] += std::string(line) + "\n";
    }
    return cases;
}

struct XexEditWorld {
    fs::ScopedTempDir dir{"cb-xex-edit"};
    std::optional<std::filesystem::path> logan = testing::game_file("LoganMinecraftTest/LoganMinecraftTest.xex");
    std::optional<std::filesystem::path> retail = testing::game_file("Minecraft Xbox 360 Edition/default.xex");
    std::optional<formats::xex::Tools> tools = testing::real_xex_tools(dir.path() / "unpacked");
    std::map<std::string, std::string> golden = xex_edit_cases();

    bool ready() const { return logan && retail && tools; }
    std::string out(const std::string& name) const { return fs::to_text(dir.path()) + "/" + name; }

    // Runs the command; its output with the folder written as <out>.
    std::string run(std::vector<std::string> args) const {
        xex_edit::Environment env;
        env.tools = *tools;
        std::string result = testing::run_command(xex_edit::command(env), std::move(args));
        return text::replace_all(result, fs::to_text(dir.path()) + "/", "<out>/");
    }
    std::string text_of(const std::optional<std::filesystem::path>& p) const { return fs::to_text(*p); }

    // The case `name` of the golden against what the command printed for `args`.
    void check(const std::string& name, std::vector<std::string> args) const {
        CB_CHECK_EQ(run(std::move(args)), golden.at(name));
    }
};

}  // namespace

CB_TEST("build.xex_edit.privileges_are_read_set_and_cleared_where_they_stand", [] {
    XexEditWorld w;
    if (!w.ready()) return;
    w.check("retail_privileges", {"privileges", w.text_of(w.retail)});
    w.check("logan_privileges", {"privileges", w.text_of(w.logan)});
    w.check("retail_set", {"privileges", w.text_of(w.retail), "+3", "-pal50", "-9", w.out("retail_set.xex")});
    w.check("retail_set_hex_and_high", {"privileges", w.text_of(w.retail), "+0x1f", "-2", w.out("retail_set2.xex")});
    w.check("retail_no_change", {"privileges", w.text_of(w.retail), w.out("retail_same.xex")});
    w.check("logan_no_entry", {"privileges", w.text_of(w.logan), "+3", w.out("none.xex")});
    w.check("bit_range", {"privileges", w.text_of(w.retail), "+99", w.out("x.xex")});
    w.check("bit_sign", {"privileges", w.text_of(w.retail), "3", w.out("x.xex")});
});

CB_TEST("build.xex_edit.code_edits_write_the_same_images_as_the_python", [] {
    XexEditWorld w;
    if (!w.ready()) return;
    w.check("nop3", {"nop", w.text_of(w.logan), "0x82061868", "3", w.out("nop3.xex")});
    w.check("nop1", {"nop", w.text_of(w.logan), "0x82061870", w.out("nop1.xex")});
    w.check("nop_outside", {"nop", w.text_of(w.logan), "0x81000000", w.out("x.xex")});
    w.check("bytes", {"bytes", w.text_of(w.logan), "0x82061868", "38 60 00 05 4e 80 00 20", w.out("bytes.xex")});
    w.check("asm", {"asm", w.text_of(w.logan), "0x82061868", "li r3, 5; blr", w.out("asm.xex")});
    w.check("asm_branch", {"asm", w.text_of(w.logan), "0x82061868", "b 0x82061900", w.out("asmb.xex")});
    w.check("hook", {"hook", w.text_of(w.logan), "0x82061868", "0x82062000", "li r3, 1; li r4, 2", w.out("hook.xex")});
    w.check("hook_relative", {"hook", w.out("asmb.xex"), "0x82061868", "0x82062000", "li r3, 1", w.out("x.xex")});
    // every file written is the Python's: name, size, sha1
    std::string written = testing::tree_manifest(w.dir.path());
    std::string expected;
    std::vector<std::string> expected_lines;
    const std::string files_log = fs::read_text(testing::data_path("build/testdata/xex_edit/edit_files.txt"));
    for (const std::string_view line : text::lines(files_log)) {
        if (line.empty() || line.starts_with("retail_")) continue;   // (the privilege images are compared below)
        expected_lines.push_back(std::string(line));
    }
    std::sort(expected_lines.begin(), expected_lines.end());
    for (const std::string& line : expected_lines) expected += line + "\n";
    std::string code_files;
    for (const std::string_view line : text::lines(written))
        if (!line.empty() && !line.starts_with("retail_") && !line.starts_with("unpacked/")) code_files += std::string(line) + "\n";
    CB_CHECK_EQ(code_files, expected);
});

CB_TEST("build.xex_edit.the_privilege_images_are_the_pythons", [] {
    XexEditWorld w;
    if (!w.ready()) return;
    w.check("retail_set", {"privileges", w.text_of(w.retail), "+3", "-pal50", "-9", w.out("retail_set.xex")});
    w.check("retail_set_hex_and_high", {"privileges", w.text_of(w.retail), "+0x1f", "-2", w.out("retail_set2.xex")});
    w.check("retail_no_change", {"privileges", w.text_of(w.retail), w.out("retail_same.xex")});
    std::string expected;
    std::vector<std::string> expected_lines;
    const std::string files_log = fs::read_text(testing::data_path("build/testdata/xex_edit/edit_files.txt"));
    for (const std::string_view line : text::lines(files_log))
        if (line.starts_with("retail_")) expected_lines.push_back(std::string(line));
    std::sort(expected_lines.begin(), expected_lines.end());
    for (const std::string& line : expected_lines) expected += line + "\n";
    std::string written;
    const std::string manifest = testing::tree_manifest(w.dir.path());
    for (const std::string_view line : text::lines(manifest))
        if (line.starts_with("retail_")) written += std::string(line) + "\n";
    CB_CHECK_EQ(written, expected);
});

CB_TEST("build.xex_edit.the_assembler_alone_prints_bytes_and_its_errors_are_reported", [] {
    const xex_edit::Environment env;
    const auto run = [&](std::vector<std::string> args) { return testing::run_command(xex_edit::command(env), std::move(args)); };
    const auto cases = xex_edit_cases();
    CB_CHECK_EQ(run({"asm-only", "li r3, 5; blr; b 0x82000020", "0x82000000"}), cases.at("asm_only"));
    const std::string bad = run({"asm-only", "frobnicate r3"});
    CB_CHECK(bad.starts_with("xex-edit: "));
    CB_CHECK(bad.ends_with("exit=1\n"));
    const std::string far = run({"asm-only", "b 0x20", "0x82000000"});   // (the Python wrapped such a branch silently)
    CB_CHECK(far.starts_with("xex-edit: "));
    CB_CHECK(far.ends_with("exit=1\n"));
    CB_CHECK_EQ(run({"frobnicate", "x.xex"}), cases.at("unknown"));
    // an option-looking token after the operation is the operation's, not the command's
    CB_CHECK_EQ(run({"privileges"}), "xex-edit: privileges needs 1 argument(s)\nexit=1\n");
});

CB_TEST("build.xex_edit.privileges_and_images_have_a_typed_api", [] {
    // a header with one privilege entry (key 0x30000 holding 0b101) and none for the high half
    Bytes xex(0x40, 0);
    const auto put = [&](std::size_t at, std::uint32_t v) {
        for (int k = 0; k < 4; ++k) xex[at + k] = static_cast<std::uint8_t>(v >> (24 - 8 * k));
    };
    put(0, 0x58455832);   // "XEX2"
    put(0x14, 1);         // one optional header
    put(0x18, xex_edit::kPrivilegeKeyLow);
    put(0x1C, 0b101);
    const xex_edit::Privileges found = xex_edit::privileges(xex);
    CB_CHECK(found.bits == (std::vector<unsigned>{0, 2}));
    CB_CHECK(found.low_at == std::optional<std::size_t>{0x1C});
    CB_CHECK(!found.high_at);
    const std::vector<xex_edit::Change> changes = {{1, true}, {0, false}};
    const Bytes edited = xex_edit::set_privileges(xex, changes);
    CB_CHECK(xex_edit::privileges(edited).bits == (std::vector<unsigned>{1, 2}));
    CB_CHECK_THROWS_AS(UsageError, xex_edit::set_privileges(xex, std::vector<xex_edit::Change>{{40, true}}));
    CB_CHECK_THROWS_AS(FormatError, xex_edit::privileges(Bytes(0x40, 0)));

    // an image: bytes in, bytes out, and the range checked
    Bytes basic(0x100, 0);
    basic[11] = 0x40;   // pe_offset = 0x40
    xex_edit::Image image(basic, 0x82000000);
    image.write(0x82000010, Bytes{1, 2, 3, 4});
    CB_CHECK(image.read(0x82000010, 4) == (Bytes{1, 2, 3, 4}));
    CB_CHECK_EQ(image.file()[0x40 + 0x10], 1);
    CB_CHECK_THROWS_AS(UsageError, image.read(0x81FFFFFF, 1));
    CB_CHECK_THROWS_AS(UsageError, image.read(0x82000000 + 0xC0, 1));
    // hooking an instruction that branches relative to itself is refused; any other is moved to the cave
    xex_edit::Image hooked(basic, 0x82000000);
    hooked.write(0x82000020, Bytes{0x48, 0x00, 0x00, 0x10});   // b +0x10
    CB_CHECK_THROWS_AS(UsageError, xex_edit::hook(hooked, 0x82000020, 0x82000080, "nop"));
    hooked.write(0x82000020, Bytes{0x38, 0x60, 0x00, 0x07});   // li r3, 7
    xex_edit::hook(hooked, 0x82000020, 0x82000080, "nop");
    CB_CHECK(hooked.read(0x82000020, 4) == (Bytes{0x48, 0x00, 0x00, 0x60}));                                       // b cave
    CB_CHECK(hooked.read(0x82000080, 12) == (Bytes{0x60, 0, 0, 0, 0x38, 0x60, 0, 7, 0x4B, 0xFF, 0xFF, 0x9C}));     // nop, li, b back
});

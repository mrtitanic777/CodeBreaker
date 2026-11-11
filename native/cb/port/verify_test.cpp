#include "cb/port/verify.hpp"

#include <atomic>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace v = cb::port::verify;
namespace stdfs = std::filesystem;
using cb::Bytes;

// The port in these tests is cmd.exe: a program every machine this runs on has, whose behaviour with `/c ...` is the
// command's own. (`cmd` of a case is its command line.)
stdfs::path cmd_exe() {
    const std::optional<std::string> spec = cb::fs::get_env("ComSpec");
    return spec ? cb::fs::from_text(*spec) : stdfs::path("C:/Windows/System32/cmd.exe");
}

std::string cmd_exe_text() { return cb::fs::to_text(cmd_exe()); }

// (a vector of text has no formatter for the failure message)
#define CHECK_SAME(a, b) CB_CHECK((a) == (b))

Bytes bytes_of(std::string_view s) { return cb::to_bytes(s); }

std::string text_of(const Bytes& b) { return std::string(cb::as_text(b)); }

// An original that is a function: what it showed and left in the folder it is given.
class FakeOriginal final : public v::OriginalRunner {
public:
    using Fn = std::function<v::Run(const stdfs::path& exe, const stdfs::path& folder, std::string_view cmd, cb::ByteView in)>;
    explicit FakeOriginal(Fn fn, std::string identity = "fake-1") : fn_(std::move(fn)), identity_(std::move(identity)) {}
    std::string identity() const override { return identity_; }
    v::Run run(const stdfs::path& exe, const stdfs::path& folder, std::string_view cmd, cb::ByteView in) const override {
        ++calls;
        return fn_(exe, folder, cmd, in);
    }
    mutable std::atomic<int> calls{0};

private:
    Fn fn_;
    std::string identity_;
};

v::Run shown(std::string_view console, std::uint32_t exit = 0) { return {bytes_of(console), v::Exit::of(exit)}; }

// The folder the cases live in: DATA.BIN to give, and a cases file written by the test.
struct Fixture {
    cb::fs::ScopedTempDir dir{"cb-verify"};
    stdfs::path base() const { return dir.path(); }
    Fixture() {
        cb::fs::write_file(dir.path() / "inputs" / "DATA.BIN", Bytes{0, 1, 2, 3, 0xFF});
        cb::fs::write_text(dir.path() / "orig.exe", "MZ original");
        cb::fs::write_text(dir.path() / "orig2.exe", "MZ the other original");
    }
    // A cases file with the given cases (a JSON list) and optional extra members, written beside the inputs.
    v::Config config(const std::string& cases, const std::string& more = "") const {
        const std::string json = std::format(R"({{"original":"orig.exe","port":"{}","inputs":{{"DATA.BIN":"inputs/DATA.BIN"}},{}"cases":{}}})",
                                             cmd_exe_text(), more.empty() ? "" : more + ",", cases);
        return v::parse_config(json);
    }
};

v::Report compare(const Fixture& f, const v::Config& config, const FakeOriginal& original, v::Services services = {}) {
    services.original = &original;
    return v::compare(config, f.base(), f.base() / "orig.exe", cmd_exe(), services);
}

// ---- patterns, diffs, lines ---------------------------------------------------------------------------------------

CB_TEST("port.verify.glob_match", [] {
    CB_CHECK(v::glob_match("OUT.TXT", "*.TXT"));
    CB_CHECK(v::glob_match("OUT.TXT", "*.txt"));       // (a DOS name has no case)
    CB_CHECK(v::glob_match("out.txt", "OUT.*"));
    CB_CHECK(!v::glob_match("A.QQ", "*.Q"));
    CB_CHECK(v::glob_match("A.Q", "*.Q"));
    CB_CHECK(v::glob_match("ABC", "A?C"));
    CB_CHECK(!v::glob_match("AC", "A?C"));
    CB_CHECK(v::glob_match("B1", "[a-c]?"));
    CB_CHECK(!v::glob_match("D1", "[a-c]1"));
    CB_CHECK(v::glob_match("D1", "[!a-c]1"));
    CB_CHECK(!v::glob_match("B1", "[!a-c]1"));
    CB_CHECK(v::glob_match("X-", "x[-]"));              // (a '-' last is itself)
    CB_CHECK(v::glob_match("]", "[]]"));                // (a ']' first is a member)
    CB_CHECK(v::glob_match("[", "["));                  // (a '[' that never closes is itself)
    CB_CHECK(v::glob_match("a[b", "a[b"));
    CB_CHECK(v::glob_match("A\\B", "A/B"));             // (either separator)
    CB_CHECK(v::glob_match("", "*"));
    CB_CHECK(v::glob_match("", ""));
    CB_CHECK(!v::glob_match("x", ""));
    CB_CHECK(v::glob_match("abcabc", "*bc"));
    CB_CHECK(v::glob_match("abc", "a*c*"));
    CB_CHECK(v::glob_match("axbxc", "a*b*c"));
    CB_CHECK(!v::glob_match("ab", "a*b*c"));
    CB_CHECK(v::glob_match("aaa", "*a*a*a*"));
    CB_CHECK(!v::glob_match("aa", "*a*a*a*"));
});

CB_TEST("port.verify.console_lines", [] {
    const auto lines = [](std::string_view s) { return v::console_lines(cb::as_bytes(s)); };
    CB_CHECK(lines("").empty());
    CHECK_SAME(lines("x"), std::vector<std::string>{"x"});
    CHECK_SAME(lines("a\r\nb\rc\n"), (std::vector<std::string>{"a", "b", "c"}));
    CHECK_SAME(lines("\n\n"), (std::vector<std::string>{"", ""}));
    CHECK_SAME(lines("a\n\nb"), (std::vector<std::string>{"a", "", "b"}));
    CHECK_SAME(lines("caf\xE9\r\n"), (std::vector<std::string>{"caf\xC3\xA9"}));   // (one character a byte: latin-1)
});

CB_TEST("port.verify.unified_diff_forms", [] {
    using L = std::vector<std::string>;
    CB_CHECK(v::unified_diff(L{}, L{}, "a", "b").empty());
    CB_CHECK(v::unified_diff(L{"x", "y"}, L{"x", "y"}, "a", "b").empty());
    CHECK_SAME(v::unified_diff(L{"x"}, L{"y"}, "original", "port", 2), (L{"--- original", "+++ port", "@@ -1 +1 @@", "-x", "+y"}));
    CHECK_SAME(v::unified_diff(L{}, L{"x"}, "a", "b"), (L{"--- a", "+++ b", "@@ -0,0 +1 @@", "+x"}));
    CHECK_SAME(v::unified_diff(L{"x"}, L{}, "a", "b"), (L{"--- a", "+++ b", "@@ -1 +0,0 @@", "-x"}));
    // context: two lines either side
    CHECK_SAME(v::unified_diff(L{"1", "2", "3", "4", "5", "6", "7"}, L{"1", "2", "3", "X", "5", "6", "7"}, "a", "b", 2),
                (L{"--- a", "+++ b", "@@ -2,5 +2,5 @@", " 2", " 3", "-4", "+X", " 5", " 6"}));
    // two changes far apart are two hunks; near, one
    CHECK_SAME(v::unified_diff(L{"a", "1", "2", "3", "4", "5", "6", "b"}, L{"A", "1", "2", "3", "4", "5", "6", "B"}, "f", "t", 1),
                (L{"--- f", "+++ t", "@@ -1,2 +1,2 @@", "-a", "+A", " 1", "@@ -7,2 +7,2 @@", " 6", "-b", "+B"}));
    CHECK_SAME(v::unified_diff(L{"a", "1", "b"}, L{"A", "1", "B"}, "f", "t", 1),
                (L{"--- f", "+++ t", "@@ -1,3 +1,3 @@", "-a", "+A", " 1", "-b", "+B"}));
});

// A diff, applied to the first list, gives the second: whatever the lists are.
std::vector<std::string> patched(const std::vector<std::string>& a, const std::vector<std::string>& diff) {
    std::vector<std::string> out;
    std::size_t at = 0;   // next line of a
    for (std::size_t i = 2; i < diff.size(); ++i) {
        const std::string& d = diff[i];
        if (d.starts_with("@@")) {
            const std::size_t minus = d.find('-'), comma = d.find_first_of(", ", minus);
            std::size_t start = static_cast<std::size_t>(std::stoul(d.substr(minus + 1, comma - minus - 1)));
            const std::size_t len_at = d[comma] == ',' ? comma + 1 : std::string::npos;
            const std::size_t len = len_at == std::string::npos ? 1 : static_cast<std::size_t>(std::stoul(d.substr(len_at)));
            if (len == 0) ++start;   // (an empty range is the line before)
            while (at + 1 < start) out.push_back(a[at++]);
        } else if (d[0] == ' ') {
            out.push_back(a[at++]);
        } else if (d[0] == '-') {
            ++at;
        } else {
            out.push_back(d.substr(1));
        }
    }
    while (at < a.size()) out.push_back(a[at++]);
    return out;
}

CB_TEST("port.verify.unified_diff_property_applies_to_the_first_list_to_make_the_second", [] {
    std::uint32_t seed = 12345;
    const auto next = [&seed](std::uint32_t n) {
        seed = seed * 1664525u + 1013904223u;
        return (seed >> 8) % n;
    };
    for (int round = 0; round < 300; ++round) {
        std::vector<std::string> a, b;
        const std::uint32_t na = next(14), alphabet = 2 + next(6);
        for (std::uint32_t i = 0; i < na; ++i) a.push_back(std::to_string(next(alphabet)));
        b = a;
        for (std::uint32_t edits = next(6); edits > 0; --edits) {
            const std::uint32_t kind = next(3);
            if (kind == 0 && !b.empty()) b.erase(b.begin() + next(static_cast<std::uint32_t>(b.size())));
            else if (kind == 1) b.insert(b.begin() + next(static_cast<std::uint32_t>(b.size()) + 1), "n" + std::to_string(next(9)));
            else if (!b.empty()) b[next(static_cast<std::uint32_t>(b.size()))] = "m" + std::to_string(next(9));
        }
        const std::size_t context = next(4);
        const std::vector<std::string> diff = v::unified_diff(a, b, "a", "b", context);
        CB_CHECK_EQ(diff.empty(), a == b);
        if (!diff.empty()) CB_CHECK(patched(a, diff) == b);
    }
});

CB_TEST("port.verify.unified_diff_of_big_different_lists", [] {
    // lists too different to align in a reasonable time are still diffed: the middle is one change
    std::vector<std::string> a, b;
    for (int i = 0; i < 6000; ++i) {
        a.push_back("a" + std::to_string(i));
        b.push_back("b" + std::to_string(i));
    }
    const std::vector<std::string> diff = v::unified_diff(a, b, "x", "y", 1);
    CB_CHECK_EQ(diff.size(), std::size_t{2 + 1 + 12000});
    CB_CHECK(patched(a, diff) == b);
});

CB_TEST("port.verify.exit_values", [] {
    CB_CHECK(v::Exit::unsaid() == v::Exit::of(0));   // (a program that gave none was not asked to)
    CB_CHECK(!(v::Exit::unsaid() == v::Exit::of(1)));
    CB_CHECK(v::Exit::of(3) == v::Exit::of(3));
    CB_CHECK(!(v::Exit::of(3) == v::Exit::of(4)));
    CB_CHECK(v::Exit::did_not_end() == v::Exit::did_not_end());
    CB_CHECK(!(v::Exit::did_not_end() == v::Exit::of(0)));
    CB_CHECK(!(v::Exit::unsaid() == v::Exit::did_not_end()));
    CB_CHECK_EQ(v::Exit::of(7).text(), "7");
    CB_CHECK_EQ(v::Exit::unsaid().text(), "(none)");
    CB_CHECK_EQ(v::Exit::did_not_end().text(), "did not end");
});

// ---- the cases file -----------------------------------------------------------------------------------------------

CB_TEST("port.verify.config_reads_everything", [] {
    const v::Config c = v::parse_config(R"({
        "original": "LIB.EXE", "port": "../modern/program.exe", "system": "dos",
        "inputs": {"HELLO.OBJ": "hello.obj", "B.OBJ": "b/b.obj"},
        "env": {"DOSRT_CORE_PAGES": "1103"},
        "volatile": ["Free memory: \\d+"],
        "programs": {"mc2": {"original": "MC2.EXE", "port": "mc2.exe"}},
        "cases": [
            {"name": "make", "cmd": "NEW +HELLO;", "stdin": "y\r", "files": ["HELLO.OBJ"], "ignore": ["*.Q"],
             "then": [{"name": "again", "program": "mc2", "make": {"IN.TXT": "caf\u00e9"}}]},
            {"name": "bare"}]})");
    CB_CHECK_EQ(c.original.value_or(""), "LIB.EXE");
    CB_CHECK_EQ(c.port.value_or(""), "../modern/program.exe");
    CB_CHECK_EQ(c.inputs.size(), std::size_t{2});
    CB_CHECK_EQ(c.inputs.at("B.OBJ"), "b/b.obj");
    CB_CHECK_EQ(c.env.at("DOSRT_CORE_PAGES"), "1103");
    CB_CHECK_EQ(c.volatile_patterns.size(), std::size_t{1});
    CB_CHECK_EQ(c.programs.at("mc2").port, "mc2.exe");
    CB_CHECK_EQ(c.cases.size(), std::size_t{2});
    const v::Case& first = c.cases[0];
    CB_CHECK_EQ(first.name, "make");
    CB_CHECK_EQ(first.cmd, "NEW +HELLO;");
    CB_CHECK_EQ(first.stdin_text, "y\r");
    CHECK_SAME(first.files, std::vector<std::string>{"HELLO.OBJ"});
    CHECK_SAME(first.ignore, std::vector<std::string>{"*.Q"});
    CB_CHECK(!first.program.has_value());
    CB_CHECK_EQ(first.then.size(), std::size_t{1});
    CB_CHECK_EQ(first.then[0].program.value_or(""), "mc2");
    CB_CHECK_EQ(first.then[0].make.at("IN.TXT"), "caf\xC3\xA9");
    CB_CHECK(c.cases[1].files.empty());
    CB_CHECK(c.cases[1].cmd.empty());
});

CB_TEST("port.verify.config_errors", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)v::parse_config("not json"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config("[]"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"original":"a"})"));                        // no cases
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"cases":{}})"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"original":3,"cases":[]})"));               // not text
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"cases":[{"name":"a","files":"X"}]})"));    // not a list
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"cases":[{"name":"a","files":["X"]}]})"));  // no such input
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"cases":[{"name":"a","program":"p"}]})"));  // no such program
    CB_CHECK_THROWS_AS(cb::FormatError,                                                                         // (a `then` too)
                       (void)v::parse_config(R"({"cases":[{"name":"a","then":[{"name":"b","program":"p"}]}]})"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"programs":{"p":{"original":"a"}},"cases":[]})"));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)v::parse_config(R"({"make":1,"cases":[{"make":{"A":1}}]})"));
    CB_CHECK_NOTHROW((void)v::parse_config(R"({"cases":[]})"));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)v::load_config("C:/no/such/cases.json"));
});

// ---- running the port ---------------------------------------------------------------------------------------------

CB_TEST("port.verify.run_port", [] {
    cb::fs::ScopedTempDir dir("cb-verify-port");
    const auto run = [&](std::string_view cmd, std::string_view in = "", const std::map<std::string, std::string>& env = {},
                         std::chrono::milliseconds timeout = std::chrono::seconds(20)) {
        return v::run_port(cmd_exe(), dir.path(), cmd, cb::as_bytes(in), env, timeout);
    };
    v::Run r = run("/c echo hi");
    CB_CHECK_EQ(text_of(r.console), "hi\r\n");
    CB_CHECK(r.exit == v::Exit::of(0));
    r = run("/c exit 3");
    CB_CHECK(r.exit == v::Exit::of(3));
    r = run("/c echo out& echo err 1>&2");   // (stderr is in the console, in order)
    CB_CHECK_EQ(text_of(r.console), "out\r\nerr \r\n");
    r = run("/c echo %CB_VERIFY_PROBE%", "", {{"CB_VERIFY_PROBE", "set by the case"}});
    CB_CHECK_EQ(text_of(r.console), "set by the case\r\n");
    r = run("/c findstr x", "ax\r\nb\r\nxc\r\n");   // (what is typed reaches it)
    CB_CHECK_EQ(text_of(r.console), "ax\r\nxc\r\n");
    r = run("/c echo made> OUT.TXT");                // (it runs in the folder)
    CB_CHECK(stdfs::exists(dir.path() / "OUT.TXT"));
    r = run("   /c   echo   a   ");                  // (the command is split at blanks)
    CB_CHECK_EQ(text_of(r.console), "a\r\n");
    r = run("/c ping -n 20 127.0.0.1", "", {}, std::chrono::milliseconds(300));   // a port that does not end
    CB_CHECK(r.exit == v::Exit::did_not_end());
    CB_CHECK_THROWS_AS(cb::ToolError, (void)v::run_port("C:/no/such/program.exe", dir.path(), "", {}, {}));
});

// ---- comparing ------------------------------------------------------------------------------------------------------

CB_TEST("port.verify.compare_same_console_and_files", [] {
    const Fixture f;
    const v::Config config = f.config(R"([
        {"name":"hello","cmd":"/c echo hi"},
        {"name":"write","cmd":"/c echo hi> OUT.TXT"},
        {"name":"typed","cmd":"/c findstr x","stdin":"ax\r\nb\r\n"}])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path& folder, std::string_view cmd, cb::ByteView in) {
        if (cmd == "/c echo hi") return shown("hi\r\n");
        if (cmd == "/c findstr x") return shown(cb::as_text(in).starts_with("ax") ? "ax\r\n" : "");   // (what is typed reaches both)
        cb::fs::write_text(folder / "OUT.TXT", "hi\r\n");
        return shown("");
    });
    const v::Report report = compare(f, config, original);
    CB_CHECK_EQ(report.rows.size(), std::size_t{3});
    CB_CHECK(report.rows[0].verdict == cb::verify::Verdict::Equivalent);
    CB_CHECK_EQ(report.rows[0].what, "same: console");
    CB_CHECK(report.rows[1].verdict == cb::verify::Verdict::Equivalent);
    CB_CHECK_EQ(report.rows[1].what, "same: console, OUT.TXT");   // (what the original made is named)
    CB_CHECK(report.details.size() <= 1);
    CB_CHECK_EQ(report.original_name, "orig.exe");
});

CB_TEST("port.verify.compare_names_every_kind_of_difference", [] {
    const Fixture f;
    const v::Config config = f.config(R"([
        {"name":"console","cmd":"/c echo hi"},
        {"name":"exit","cmd":"/c exit 3"},
        {"name":"differs","cmd":"/c echo hi> OUT.TXT"},
        {"name":"only-original","cmd":"/c echo"},
        {"name":"only-port","cmd":"/c echo x> MORE.TXT"},
        {"name":"ignored","cmd":"/c echo x> Q.TMP","ignore":["*.tmp"]},
        {"name":"short","cmd":"/c echo hi> SHORT.TXT"}])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path& folder, std::string_view cmd, cb::ByteView) {
        if (cmd == "/c echo hi") return shown("ho\r\n");
        if (cmd == "/c exit 3") return shown("", 0);
        if (cmd == "/c echo hi> OUT.TXT") {
            cb::fs::write_text(folder / "OUT.TXT", "ho\r\n");
            return shown("");
        }
        if (cmd == "/c echo") {
            cb::fs::write_text(folder / "EXTRA.TXT", "x");
            return shown("ECHO is on.\r\n");
        }
        if (cmd == "/c echo hi> SHORT.TXT") {
            cb::fs::write_text(folder / "SHORT.TXT", "hi");
            return shown("");
        }
        if (cmd == "/c echo x> Q.TMP") {
            cb::fs::write_text(folder / "Q.TMP", "other");
            return shown("");
        }
        return shown("");
    });
    const v::Report report = compare(f, config, original);
    CB_CHECK_EQ(report.rows.size(), std::size_t{7});
    for (const v::CaseRow& r : report.rows) CB_CHECK(r.name == "ignored" ? r.verdict == cb::verify::Verdict::Equivalent : r.verdict == cb::verify::Verdict::Divergent);
    CB_CHECK_EQ(report.rows[0].what, "console");
    CB_CHECK_EQ(report.rows[1].what, "exit 0 / 3");
    CB_CHECK_EQ(report.rows[2].what, "OUT.TXT differs (4 / 4 bytes, first at 1)");
    CB_CHECK(report.rows[3].what.find("EXTRA.TXT only the original's") != std::string::npos);
    CB_CHECK_EQ(report.rows[4].what, "MORE.TXT only the port's");
    CB_CHECK_EQ(report.rows[5].what, "same: console, Q.TMP");   // (not compared, but it is what the original made)
    CB_CHECK_EQ(report.rows[6].what, "SHORT.TXT differs (2 / 4 bytes, first at 2)");
    // the console difference is shown
    CB_CHECK(report.details[0].starts_with("### console: console\n\n```diff\n--- original\n+++ port\n@@ -1 +1 @@\n-ho\n+hi\n```\n"));
});

CB_TEST("port.verify.compare_runs_a_case_and_what_follows_in_the_same_folders", [] {
    const Fixture f;
    const v::Config config = f.config(R"([
        {"name":"write","cmd":"/c echo kept> OUT.TXT","then":[
            {"name":"read","cmd":"/c type OUT.TXT"},
            {"name":"read-given","cmd":"/c type IN.TXT","make":{"IN.TXT":"caf\u00e9"}},
            {"name":"data","cmd":"/c rem x","files":["DATA.BIN"]}]},
        {"name":"alone","cmd":"/c type OUT.TXT"}])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path& folder, std::string_view cmd, cb::ByteView) {
        if (cmd == "/c echo kept> OUT.TXT") {
            cb::fs::write_text(folder / "OUT.TXT", "kept\r\n");
            return shown("");
        }
        if (cmd == "/c type OUT.TXT") {
            if (!stdfs::exists(folder / "OUT.TXT")) return shown("The system cannot find the file specified.\r\n", 1);
            return shown(cb::fs::read_text(folder / "OUT.TXT"));
        }
        if (cmd == "/c type IN.TXT") return shown(cb::fs::read_text(folder / "IN.TXT"));
        return shown("");
    });
    const v::Report report = compare(f, config, original);
    CB_CHECK_EQ(report.rows.size(), std::size_t{5});
    CB_CHECK_EQ(report.rows[0].name, "write");
    CB_CHECK_EQ(report.rows[1].name, "read");
    CB_CHECK_EQ(report.rows[2].name, "read-given");
    CB_CHECK_EQ(report.rows[3].name, "data");
    CB_CHECK_EQ(report.rows[4].name, "alone");   // (a case's own `then` follows it, before the next case)
    CB_CHECK_EQ(report.rows[0].what, "same: console, OUT.TXT");
    CB_CHECK_EQ(report.rows[1].what, "same: console, OUT.TXT");   // (the file is still there: the same folders)
    CB_CHECK_EQ(report.rows[2].what, "same: console, OUT.TXT");   // IN.TXT is given by the case, not made
    CB_CHECK_EQ(report.rows[3].what, "same: console, IN.TXT, OUT.TXT");   // (DATA.BIN is given; IN.TXT was made by the case before: the original's folder has it)
    // the port of `alone` says what cmd.exe says of a missing file; the original's fake says it the same way
    CB_CHECK(report.rows[4].verdict == cb::verify::Verdict::Divergent || report.rows[4].verdict == cb::verify::Verdict::Equivalent);
});

CB_TEST("port.verify.compare_masks_what_depends_on_the_machine", [] {
    const Fixture f;
    const v::Config config = f.config(R"([{"name":"t","cmd":"/c echo time 34> T.TXT & echo time 56"},
                                           {"name":"u","cmd":"/c echo time 34> U.DAT"}])",
                                       R"("volatile":["time \\d+"])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path& folder, std::string_view cmd, cb::ByteView) {
        if (cmd == "/c echo time 34> U.DAT") {
            cb::fs::write_text(folder / "U.DAT", "time 12\r\n");
            return shown("");
        }
        cb::fs::write_text(folder / "T.TXT", "time 12 \r\n");
        return shown("time 78\r\n");
    });
    const v::Report report = compare(f, config, original);
    CB_CHECK_EQ(report.rows[0].what, "same: console, T.TXT");   // (the console, and a file that ends .TXT)
    CB_CHECK(report.rows[1].verdict == cb::verify::Verdict::Divergent);   // (not a file that ends .DAT)
    CB_CHECK_EQ(report.rows[1].what, "U.DAT differs (9 / 9 bytes, first at 5)");
});

CB_TEST("port.verify.compare_a_port_that_does_not_end", [] {
    const Fixture f;
    const v::Config config = f.config(R"([{"name":"hang","cmd":"/c ping -n 30 127.0.0.1"}])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path&, std::string_view, cb::ByteView) { return shown("", 0); });
    v::Services services;
    services.port_timeout = std::chrono::milliseconds(1500);
    const v::Report report = compare(f, config, original, services);
    CB_CHECK(report.rows[0].verdict == cb::verify::Verdict::Divergent);
    CB_CHECK_EQ(report.rows[0].what, "console, exit 0 / did not end");
    CB_CHECK(report.details[0].find("+(did not end)") != std::string::npos);
});

CB_TEST("port.verify.compare_another_program_of_the_set", [] {
    const Fixture f;
    const v::Config config = f.config(R"([{"name":"one","cmd":"/c echo one"},{"name":"two","program":"second","cmd":"/c echo two"}])",
                                       std::format(R"("programs":{{"second":{{"original":"orig2.exe","port":"{}"}}}})", cmd_exe_text()));
    const FakeOriginal original([](const stdfs::path& exe, const stdfs::path&, std::string_view cmd, cb::ByteView) {
        return shown(exe.filename() == "orig2.exe" ? "two\r\n" : (cmd == "/c echo one" ? "one\r\n" : "wrong\r\n"));
    });
    const v::Report report = compare(f, config, original);
    CB_CHECK(report.all_same());
    CB_CHECK_EQ(report.same(), std::size_t{2});
});

CB_TEST("port.verify.report_text", [] {
    const Fixture f;
    const v::Config config = f.config(R"([{"name":"hello","cmd":"/c echo hi"},{"name":"bare","cmd":""},{"name":"bad","cmd":"/c echo hi> OUT.TXT"}])");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path& folder, std::string_view cmd, cb::ByteView) {
        if (cmd == "/c echo hi> OUT.TXT") cb::fs::write_text(folder / "OUT.TXT", "ho");
        return shown(cmd == "/c echo hi" ? "hi\r\n" : "");
    });
    const v::Report report = compare(f, config, original);
    const std::string md = report.markdown();
    CB_CHECK(md.starts_with("# Behaviour: the original against the port\n\n`orig.exe` (under CodeBreaker's emulator) against `"));
    CB_CHECK(md.find("| case | command | | what |\n|---|---|---|---|\n| hello | `/c echo hi` | same | same: console |\n") != std::string::npos);
    CB_CHECK(md.find("| bare | `(none)` | ") != std::string::npos);
    CB_CHECK(md.find("| bad | `/c echo hi> OUT.TXT` | **DIFFERS** | OUT.TXT differs (2 / 4 bytes, first at 1) |\n") != std::string::npos);
    CB_CHECK(md.ends_with("\n"));
});

CB_TEST("port.verify.compare_needs_an_emulator", [] {
    const Fixture f;
    CB_CHECK_THROWS_AS(cb::UsageError, (void)v::compare(f.config("[]"), f.base(), f.base() / "orig.exe", cmd_exe(), {}));
});

// ---- the port alone -----------------------------------------------------------------------------------------------------

CB_TEST("port.verify.run_cases", [] {
    const Fixture f;
    const v::Config config = f.config(R"([
        {"name":"hello","cmd":"/c echo hi","then":[{"name":"after","cmd":"/c type OUT.TXT"}]},
        {"name":"ghost","program":"ghost","cmd":"/c echo no"},
        {"name":"env","cmd":"/c echo %CB_VERIFY_A%%CB_VERIFY_B%"},
        {"name":"hang","cmd":"/c ping -n 30 127.0.0.1"}])",
                                       std::format(R"("env":{{"CB_VERIFY_A":"from the file"}},"programs":{{"ghost":{{"original":"x.exe","port":"{}"}}}})",
                                                   cb::fs::to_text(f.base() / "not-built.exe")));
    v::Services services;
    services.port_timeout = std::chrono::milliseconds(3000);
    const auto consoles = v::run_cases(config, f.base(), cmd_exe(), {{"CB_VERIFY_B", " and more"}}, services);
    CB_CHECK_EQ(consoles.size(), std::size_t{4});   // (the ghost's case is left out)
    CB_CHECK_EQ(consoles[0].name, "hello");
    CB_CHECK_EQ(consoles[0].console, "hi\r\n");
    CB_CHECK_EQ(consoles[1].name, "after");
    CB_CHECK_EQ(consoles[2].name, "env");
    CB_CHECK_EQ(consoles[2].console, "from the file and more\r\n");
    CB_CHECK_EQ(consoles[3].name, "hang");
    CB_CHECK(consoles[3].console.ends_with("\n(did not end)"));
});

// ---- the original's run, kept -------------------------------------------------------------------------------------------

CB_TEST("port.verify.original_runs_are_kept_by_what_they_depend_on", [] {
    cb::fs::ScopedTempDir dir("cb-verify-cache");
    cb::base::cache::Config cc;
    cc.state_dir = dir.path() / "state";
    cc.shared_dir = dir.path() / "shared";
    cb::base::cache::Cache cache(cc);
    v::declare_cache(cache);
    cb::fs::write_text(dir.path() / "prog.exe", "MZ one");
    const auto fresh = [&](std::string_view name) {   // a folder holding the input of the case
        const stdfs::path d = dir.path() / name;
        cb::fs::write_text(d / "IN.TXT", "input");
        return d;
    };
    const auto make = [](std::string console, std::uint32_t exit) {
        return [=](const stdfs::path&, const stdfs::path& folder, std::string_view, cb::ByteView) {
            cb::fs::write_text(folder / "OUT.TXT", "made by the run");
            return shown(console, exit);
        };
    };
    const FakeOriginal runner(make("shown", 7));
    const stdfs::path exe = dir.path() / "prog.exe";
    v::Run r = v::run_original(runner, &cache, nullptr, exe, fresh("f1"), "a b", cb::as_bytes("typed"));
    CB_CHECK_EQ(text_of(r.console), "shown");
    CB_CHECK(r.exit == v::Exit::of(7));
    CB_CHECK_EQ(runner.calls.load(), 1);
    // the same again, in another folder with the same input: from the cache; the files the run left are put back
    const stdfs::path again = fresh("f2");
    cb::fs::write_text(again / "STRAY.TXT", "not part of the key's folder? it is: so this is another case");
    cb::fs::remove_tree(again / "STRAY.TXT");
    r = v::run_original(runner, &cache, nullptr, exe, again, "a b", cb::as_bytes("typed"));
    CB_CHECK_EQ(runner.calls.load(), 1);
    CB_CHECK_EQ(text_of(r.console), "shown");
    CB_CHECK(r.exit == v::Exit::of(7));
    CB_CHECK_EQ(cb::fs::read_text(again / "OUT.TXT"), "made by the run");
    CB_CHECK_EQ(cb::fs::read_text(again / "IN.TXT"), "input");
    // anything the run depends on, changed: it is run again
    (void)v::run_original(runner, &cache, nullptr, exe, fresh("f3"), "a c", cb::as_bytes("typed"));         // the command
    CB_CHECK_EQ(runner.calls.load(), 2);
    (void)v::run_original(runner, &cache, nullptr, exe, fresh("f4"), "a b", cb::as_bytes("typed2"));        // what is typed
    CB_CHECK_EQ(runner.calls.load(), 3);
    const stdfs::path changed = fresh("f5");
    cb::fs::write_text(changed / "IN.TXT", "other input");
    (void)v::run_original(runner, &cache, nullptr, exe, changed, "a b", cb::as_bytes("typed"));            // a file it is given
    CB_CHECK_EQ(runner.calls.load(), 4);
    cb::fs::write_text(dir.path() / "prog.exe", "MZ two");
    (void)v::run_original(runner, &cache, nullptr, exe, fresh("f6"), "a b", cb::as_bytes("typed"));        // the program
    CB_CHECK_EQ(runner.calls.load(), 5);
    const FakeOriginal other_emulator(make("shown", 7), "fake-2");
    (void)v::run_original(other_emulator, &cache, nullptr, exe, fresh("f7"), "a b", cb::as_bytes("typed"));   // the emulator
    CB_CHECK_EQ(other_emulator.calls.load(), 1);
    // no cache: every time; a run that did not end is kept as such
    (void)v::run_original(runner, nullptr, nullptr, exe, fresh("f8"), "a b", cb::as_bytes("typed"));
    CB_CHECK_EQ(runner.calls.load(), 6);
    const FakeOriginal hangs([](const stdfs::path&, const stdfs::path&, std::string_view, cb::ByteView) {
        return v::Run{bytes_of("too long"), v::Exit::did_not_end()};
    });
    (void)v::run_original(hangs, &cache, nullptr, exe, fresh("f9"), "", {});
    r = v::run_original(hangs, &cache, nullptr, exe, fresh("f10"), "", {});
    CB_CHECK_EQ(hangs.calls.load(), 1);
    CB_CHECK(r.exit == v::Exit::did_not_end());
    CB_CHECK_EQ(text_of(r.console), "too long");
});

CB_TEST("port.verify.files_of_names_files_in_upper_case", [] {
    cb::fs::ScopedTempDir dir("cb-verify-files");
    cb::fs::write_text(dir.path() / "a.txt", "one");
    cb::fs::write_text(dir.path() / "B.Bin", "two");
    stdfs::create_directory(dir.path() / "sub");
    cb::fs::write_text(dir.path() / "sub" / "c.txt", "three");   // (a folder's files are not the program's)
    const auto files = v::files_of(dir.path());
    CB_CHECK_EQ(files.size(), std::size_t{2});
    CB_CHECK_EQ(text_of(files.at("A.TXT")), "one");
    CB_CHECK_EQ(text_of(files.at("B.BIN")), "two");
});

// ---- the command --------------------------------------------------------------------------------------------------------

CB_TEST("port.verify.command", [] {
    const Fixture f;
    const std::string cases = R"([{"name":"hello","cmd":"/c echo hi"}])";
    cb::fs::write_text(f.base() / "good.json", std::format(R"({{"original":"orig.exe","port":"{}","cases":{}}})", cmd_exe_text(), cases));
    cb::fs::write_text(f.base() / "bad.json", std::format(R"({{"original":"orig.exe","port":"{}","cases":{}}})", cmd_exe_text(),
                                                          R"([{"name":"hello","cmd":"/c echo ho"}])"));
    cb::fs::write_text(f.base() / "unbuilt.json", R"({"original":"orig.exe","port":"nowhere.exe","cases":[]})");
    cb::fs::write_text(f.base() / "noport.json", R"({"original":"orig.exe","cases":[]})");
    const FakeOriginal original([](const stdfs::path&, const stdfs::path&, std::string_view, cb::ByteView) { return shown("hi\r\n"); });
    v::Services services;
    services.original = &original;
    const cb::cli::Cli cli("codebreaker", {v::command(services)});
    const auto run = [&](std::vector<std::string> args, int& code) {
        cb::testing::CapturedStd captured;
        args.insert(args.begin(), "verify");
        code = cli.run(args);
        return std::pair{captured.out(), captured.err()};
    };
    int code = -1;
    auto [out, err] = run({cb::fs::to_text(f.base() / "good.json")}, code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(out.find("**1 of 1 cases the same**") != std::string::npos);
    std::tie(out, err) = run({cb::fs::to_text(f.base() / "bad.json")}, code);
    CB_CHECK_EQ(code, 1);
    CB_CHECK(out.find("**DIFFERS**") != std::string::npos);
    std::tie(out, err) = run({cb::fs::to_text(f.base() / "good.json"), "--report", cb::fs::to_text(f.base() / "r.md")}, code);
    CB_CHECK_EQ(code, 0);
    CB_CHECK(out.starts_with("1 of 1 cases the same ("));
    CB_CHECK(cb::fs::read_text(f.base() / "r.md").starts_with("# Behaviour"));
    std::tie(out, err) = run({cb::fs::to_text(f.base() / "unbuilt.json")}, code);
    CB_CHECK_EQ(code, cb::kExitNotFound);
    CB_CHECK(err.find("the port is not built") != std::string::npos);
    std::tie(out, err) = run({cb::fs::to_text(f.base() / "noport.json"), "--port", "C:/no/such/port.exe"}, code);
    CB_CHECK_EQ(code, cb::kExitNotFound);
    std::tie(out, err) = run({cb::fs::to_text(f.base() / "good.json"), "--port", cmd_exe_text()}, code);   // --port overrides the file's
    CB_CHECK_EQ(code, 0);
    CB_CHECK_THROWS_AS(cb::UsageError, (void)run({cb::fs::to_text(f.base() / "noport.json")}, code));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)run({cb::fs::to_text(f.base() / "missing.json")}, code));
});

// ---- the real emulator --------------------------------------------------------------------------------------------------

// Set CB_CBDOS_DLL (and CB_UNICORN_DLL; the prebuilt one beside the sources is the default) to run these.
std::unique_ptr<v::CbdosRunner> real_emulator() {
    const std::optional<std::string> dll = cb::fs::get_env("CB_CBDOS_DLL");
    if (!dll) return nullptr;
    v::CbdosRunner::Settings s;
    s.dll = cb::fs::from_text(*dll);
    const std::optional<std::string> unicorn = cb::fs::get_env("CB_UNICORN_DLL");
    s.unicorn = unicorn ? cb::fs::from_text(*unicorn) : cb::testing::data_path("../third_party/bin/unicorn.dll");
    return v::CbdosRunner::load(s);
}

stdfs::path original_program(std::string_view name) { return cb::testing::data_path("port/testdata/verify/" + std::string(name)); }

CB_TEST("port.verify.cbdos_runs_dos_programs", [] {
    const std::unique_ptr<v::CbdosRunner> dos = real_emulator();
    if (!dos) return;
    cb::fs::ScopedTempDir dir("cb-verify-dos");
    v::Run r = dos->run(original_program("hello.exe"), dir.path(), "", {});
    CB_CHECK_EQ(text_of(r.console), "Hello from DOS\r\n");
    r = dos->run(original_program("tail.exe"), dir.path(), "a b c", {});
    CB_CHECK_EQ(text_of(r.console), "  a b c");   // (the command tail keeps its leading blank)
    r = dos->run(original_program("echoin.exe"), dir.path(), "", cb::as_bytes("abc\rdef"));
    CB_CHECK(r.exit == v::Exit::of(0));   // (it reads what is typed and ends)
    CB_CHECK(!dos->identity().empty());
});

CB_TEST("port.verify.cbdos_original_against_a_port_in_cases", [] {
    const std::unique_ptr<v::CbdosRunner> dos = real_emulator();
    if (!dos) return;
    cb::fs::ScopedTempDir dir("cb-verify-dos2");
    cb::fs::copy_tree(cb::testing::data_path("port/testdata/verify"), dir.path());
    // the original prints "Hello from DOS"; the port is cmd.exe printing the same, and something else
    const v::Config config = v::parse_config(std::format(
        R"({{"original":"hello.exe","port":"{}","cases":[{{"name":"same","cmd":"/c echo Hello from DOS"}},{{"name":"not","cmd":"/c echo Hello from Windows"}}]}})",
        cmd_exe_text()));
    v::Services services;
    services.original = dos.get();
    const v::Report report = v::compare(config, dir.path(), dir.path() / "hello.exe", cmd_exe(), services);
    CB_CHECK(report.rows[0].verdict == cb::verify::Verdict::Equivalent);
    CB_CHECK(report.rows[1].verdict == cb::verify::Verdict::Divergent);
});

CB_TEST("port.verify.cbdos_missing_files", [] {
    v::CbdosRunner::Settings s;
    s.dll = "C:/no/such/cbdos.dll";
    s.unicorn = "C:/no/such/unicorn.dll";
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)v::CbdosRunner::load(s));
});

}  // namespace

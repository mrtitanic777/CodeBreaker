#include "cb/core/process.hpp"

#include <atomic>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

using namespace cb;
using namespace cb::process;
using std::chrono::milliseconds;

namespace {

std::string out_text(const ProcessResult& r) { return std::string(text::trim(as_text(r.out))); }

// The command interpreter and the scripts of this machine: the tests say the same thing in each one's words.
#ifdef _WIN32
constexpr bool kWindows = true;
constexpr const char* kEol = "\r\n";
std::vector<std::string> shell(const std::string& script) { return {"cmd", "/c", script}; }
const std::string kOutThenErr = "echo out& echo err 1>&2";
std::string echo_variable(const std::string& name) { return "echo [%" + name + "%]"; }
const std::string kCwd = "cd";
const std::string kSleepLong = "ping -n 30 127.0.0.1 >nul";   // (cmd starts ping as a grandchild)
const std::string kReasonThenFail = "echo reason& exit 3";
constexpr char kPathSeparator = ';';
#else
constexpr bool kWindows = false;
constexpr const char* kEol = "\n";
std::vector<std::string> shell(const std::string& script) { return {"sh", "-c", script}; }
const std::string kOutThenErr = "echo out; echo err 1>&2";
std::string echo_variable(const std::string& name) { return "echo \"[$" + name + "]\""; }
const std::string kCwd = "pwd";
const std::string kSleepLong = "sleep 30; true";   // (sh starts sleep as a child)
const std::string kReasonThenFail = "echo reason; exit 3";
constexpr char kPathSeparator = ':';
#endif

}  // namespace

CB_TEST("core.process.quote_arg", [] {
    CB_CHECK_EQ(quote_arg("plain"), "plain");
    CB_CHECK_EQ(quote_arg("with space"), "\"with space\"");
    CB_CHECK_EQ(quote_arg(""), "\"\"");
    CB_CHECK_EQ(quote_arg("say \"hi\""), "\"say \\\"hi\\\"\"");
    CB_CHECK_EQ(quote_arg("C:\\dir with\\"), "\"C:\\dir with\\\\\"");   // trailing backslashes doubled before the closing quote
    CB_CHECK_EQ(quote_arg("a\\b"), "a\\b");                              // backslashes alone are literal
    CB_CHECK_EQ(quote_arg("a\\\"b"), "\"a\\\\\\\"b\"");
    CB_CHECK_EQ(command_line({"cmd", "/c", "echo hi"}), "cmd /c \"echo hi\"");
});

CB_TEST("core.process.run_collects_output_and_exit_code", [] {
    ProcessResult r = run({.argv = shell("echo hello")});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(out_text(r), "hello");
    CB_CHECK(r.err.empty());
    CB_CHECK(!r.timed_out);
    ProcessResult failed = run({.argv = shell("exit 7")});
    CB_CHECK_EQ(failed.exit_code, 7u);
    CB_CHECK(!failed.ok());
});

CB_TEST("core.process.stderr_separate_discard_merge", [] {
    const std::vector<std::string> argv = shell(kOutThenErr);
    ProcessResult sep = run({.argv = argv});
    CB_CHECK_EQ(out_text(sep), "out");
    CB_CHECK_EQ(std::string(text::trim(as_text(sep.err))), "err");
    ProcessResult merged = run({.argv = argv, .err = Capture::Merge});
    CB_CHECK(merged.err.empty());
    CB_CHECK(out_text(merged).find("out") != std::string::npos);
    CB_CHECK(out_text(merged).find("err") != std::string::npos);
    ProcessResult discarded = run({.argv = argv, .out = Capture::Discard, .err = Capture::Discard});
    CB_CHECK(discarded.out.empty() && discarded.err.empty());
    CB_CHECK(discarded.ok());
});

CB_TEST("core.process.output_to_file", [] {
    fs::ScopedTempDir dir("cbproc");
    ProcessResult r = run({.argv = shell("echo to-file"), .out = Capture::ToFile(dir.path() / "o" / "out.txt")});
    CB_CHECK(r.ok());
    CB_CHECK(r.out.empty());
    CB_CHECK_EQ(std::string(text::trim(fs::read_text(dir.path() / "o" / "out.txt"))), "to-file");
});

CB_TEST("core.process.environment", [] {
    ProcessResult r = run({.argv = shell(echo_variable("CB_PROC_TEST")), .env = {{"CB_PROC_TEST", "caf\xC3\xA9"}}});
    CB_CHECK_EQ(out_text(r).substr(0, 4), "[caf");
    // removal: a variable that is set in our environment is gone in the child
    fs::ScopedEnv mine("CB_PROC_REMOVED", "present");
    ProcessResult kept = run({.argv = shell(echo_variable("CB_PROC_REMOVED"))});
    CB_CHECK_EQ(out_text(kept), "[present]");
    // (Windows names are case-insensitive: the removal may spell the name differently)
    ProcessResult gone = run({.argv = shell(echo_variable("CB_PROC_REMOVED")), .env_remove = {kWindows ? "cb_proc_removed" : "CB_PROC_REMOVED"}});
    CB_CHECK_EQ(out_text(gone), kWindows ? "[%CB_PROC_REMOVED%]" : "[]");   // (cmd leaves an undefined variable as written)
    // an override of PATH finds the tool through the *child's* PATH
    ProcessResult found = run({.argv = {kWindows ? "where" : "ls"}, .env = {{"PATH", kWindows ? "C:\\Windows\\System32" : "/usr/bin:/bin"}}});
    CB_CHECK(found.exit_code != 0xFFFFFFFFu);
});

CB_TEST("core.process.working_directory", [] {
    fs::ScopedTempDir dir("cbproc");
    ProcessResult r = run({.argv = shell(kCwd), .cwd = dir.path()});
    CB_CHECK(fs::same_path(fs::from_text(out_text(r)), dir.path()));
});

CB_TEST("core.process.stdin_data", [] {
    const std::string eol = kEol;
    const std::string input = "pear" + eol + "apple" + eol + "fig" + eol;
    ProcessResult r = run({.argv = {"sort"}, .stdin_data = to_bytes(input)});
    CB_CHECK(r.ok());
    CB_CHECK_EQ(out_text(r), "apple" + eol + "fig" + eol + "pear");
    // a large input must not deadlock against a full pipe
    std::string big;
    for (int i = 0; i < 20000; ++i) big += "line number " + std::to_string(i) + eol;
    if (kWindows) {
        ProcessResult counted = run({.argv = {"findstr", "/c:line", "/n", "NUL"}, .stdin_data = to_bytes(big), .out = Capture::Discard});
        CB_CHECK(counted.exit_code != 0xFFFFFFFFu);
    } else {
        ProcessResult counted = run({.argv = {"wc", "-l"}, .stdin_data = to_bytes(big)});
        CB_CHECK_EQ(out_text(counted), "20000");
    }
});

CB_TEST("core.process.timeout_kills_the_whole_job", [] {
    // the shell starts a grandchild; the job (on POSIX: the session) must end both
    const auto t0 = std::chrono::steady_clock::now();
    ProcessResult r = run({.argv = shell(kSleepLong), .timeout = milliseconds(700)});
    const auto took = std::chrono::steady_clock::now() - t0;
    CB_CHECK(r.timed_out);
    CB_CHECK(!r.ok());
    CB_CHECK(took < std::chrono::seconds(10));
    CB_CHECK_THROWS_AS(TimeoutError, run_checked({.argv = shell(kSleepLong), .timeout = milliseconds(300)}));
});

CB_TEST("core.process.cannot_start_is_tool_error", [] {
    CB_CHECK_THROWS_AS(ToolError, run({.argv = {"definitely-not-a-program-cb"}}));
    CB_CHECK_THROWS_AS(ToolError, run({.argv = {kWindows ? "C:\\no\\such\\dir\\tool.exe" : "/no/such/dir/tool"}}));
    CB_CHECK_THROWS_AS(UsageError, run({.argv = {}}));
    CB_CHECK_THROWS_AS(ToolError, run_checked({.argv = shell("exit 3")}));
    try {
        (void)run_checked({.argv = shell(kReasonThenFail)});
    } catch (const ToolError& e) {
        CB_CHECK_EQ(e.tool_exit_code(), 3u);
        CB_CHECK(e.output().find("reason") != std::string::npos);
    }
});

CB_TEST("core.process.batch_files_and_paths_with_spaces", [] {
    fs::ScopedTempDir dir("cbproc");
    const auto script = dir.path() / "with space" / (kWindows ? "tool.cmd" : "tool");
    if (kWindows) fs::write_text(script, "@echo off\r\necho args=%~1;%~2\r\nexit /b 4\r\n");
    else fs::write_text(script, "#!/bin/sh\necho \"args=$1;$2\"\nexit 4\n");
    std::filesystem::permissions(script, std::filesystem::perms::owner_exec, std::filesystem::perm_options::add);   // (no effect on Windows)
    ProcessResult r = run({.argv = {fs::to_text(script), "one", "two words"}});
    CB_CHECK_EQ(r.exit_code, 4u);
    CB_CHECK_EQ(out_text(r), "args=one;two words");
    // found by bare name through the PATH override
    const std::string path = fs::to_text(script.parent_path()) + kPathSeparator + (kWindows ? "C:\\Windows\\System32" : "/usr/bin:/bin");
    ProcessResult byname = run({.argv = {"tool"}, .env = {{"PATH", path}}});
    CB_CHECK_EQ(byname.exit_code, 4u);
});

CB_TEST("core.process.streaming_start", [] {
    const std::string eol = kEol;
    Process p = start({.argv = {"sort"}});
    p.write(as_bytes("b" + eol + "a" + eol));
    p.close_stdin();
    Bytes out = p.read_all(Process::Stream::Out);
    CB_CHECK_EQ(std::string(text::trim(as_text(out))), "a" + eol + "b");
    auto code = p.wait(milliseconds(5000));
    CB_CHECK(code.has_value());
    CB_CHECK_EQ(*code, 0u);
    CB_CHECK(!p.running());
});

CB_TEST("core.process.streaming_kill_and_destructor", [] {
    Process p = start({.argv = shell(kSleepLong)});
    CB_CHECK(p.running());
    CB_CHECK(!p.wait(milliseconds(100)).has_value());   // still running after 100 ms
    p.kill();
    CB_CHECK(p.wait(milliseconds(10000)).has_value());
    {
        Process q = start({.argv = shell(kSleepLong)});
        CB_CHECK(q.running());
    }   // the destructor ends the job; the test must not hang or leave a process behind
});

CB_TEST("core.process.priority_and_memory_limit_are_accepted", [] {
    ProcessResult r = run({.argv = shell("echo ok"), .priority = Priority::BelowNormal});
    CB_CHECK(r.ok());
    ProcessResult m = run({.argv = shell("echo ok"), .memory_limit = 4096ull << 20});
    CB_CHECK(m.ok());
});

CB_TEST("core.process.parallel_runs_do_not_leak_pipes", [] {
    // Several launches at once: a handle that leaked into another child would keep a reader waiting forever.
    std::vector<std::jthread> threads;
    std::atomic<int> good{0};
    for (int i = 0; i < 6; ++i) {
        threads.emplace_back([&good, i] {
            ProcessResult r = run({.argv = shell("echo " + std::to_string(i))});
            if (r.ok() && std::string(text::trim(as_text(r.out))) == std::to_string(i)) ++good;
        });
    }
    threads.clear();
    CB_CHECK_EQ(good.load(), 6);
});

#include "cb/jobs/workers.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

#include <windows.h>

// DIVERGES from codebreaker/jobs/workers.cpp (the port):
//   * the kinds a worker can do are registered by the host (WorkerKinds), not hard-wired to gate/judge/build; a kind it
//     does not know is exit code 2, a share that throws is exit code 3 (as before);
//   * unhex is strict: an odd or non-hex token is a FormatError (the port decoded it to garbage); running out of tokens
//     is a FormatError (the port threw std::out_of_range);
//   * a hand-over folder is a Handover (deleted on scope exit, also on an exception); scratch_folder() left it to
//     the caller;
//   * the child is started in a Job Object with an optional timeout; a crashed worker is an exit code, never an
//     exception; count() is cb::parallel::workers() (no separate function).

using namespace cb;
using namespace cb::jobs;

namespace {

// The probe program (jobs/worker_probe_main.cpp) is built beside cb_tests.
WorkerLauncher probe() {
    const WorkerLauncher self = WorkerLauncher::for_this_program();
    return WorkerLauncher(self.exe().parent_path() / "cb_worker_probe.exe");
}

}  // namespace

CB_TEST("jobs.workers.tokens_round_trip", [] {
    TokenWriter w;
    w.integer(-12).text("hi there").text("").bytes(Bytes{0, 255, 16}).integer(0);
    CB_CHECK_EQ(w.str(), "-12 6869207468657265 - 00ff10 0");
    TokenReader r(w.str());
    CB_CHECK_EQ(r.integer(), -12);
    CB_CHECK_EQ(r.text(), "hi there");
    CB_CHECK_EQ(r.text(), "");
    CB_CHECK(r.bytes() == (Bytes{0, 255, 16}));
    CB_CHECK_EQ(r.integer(), 0);
    CB_CHECK_EQ(r.remaining(), 0u);
});

CB_TEST("jobs.workers.tokens_text_that_is_not_plain_survives", [] {
    // spaces, newlines, NULs and non-ASCII are why text travels as hex
    const std::string nasty = std::string("a b\nc\0d", 7) + "\xC3\xA9 \xFF";
    TokenWriter w;
    w.text(nasty).integer(std::numeric_limits<std::int64_t>::min());
    TokenReader r(w.str() + "\r\n");           // a trailing newline from a file is ignored
    CB_CHECK_EQ(r.text(), nasty);
    CB_CHECK_EQ(r.integer(), std::numeric_limits<std::int64_t>::min());
});

CB_TEST("jobs.workers.tokens_damage", [] {
    CB_CHECK_THROWS_AS(FormatError, TokenReader("").integer());
    CB_CHECK_THROWS_AS(FormatError, TokenReader("12abc").integer());
    CB_CHECK_THROWS_AS(FormatError, TokenReader("99999999999999999999").integer());
    CB_CHECK_THROWS_AS(FormatError, TokenReader("zz").text());           // not hex
    CB_CHECK_THROWS_AS(FormatError, TokenReader("abc").bytes());         // odd length
    TokenReader r("1");
    CB_CHECK_EQ(r.integer(), 1);
    CB_CHECK_THROWS_AS(FormatError, r.integer());                        // ran out
    CB_CHECK_EQ(TokenReader("  1   2 ").remaining(), 2u);
});

CB_TEST("jobs.workers.kinds", [] {
    WorkerKinds kinds;
    kinds.add("a", [](const Args&) { return 1; });
    CB_CHECK(kinds.find("a") != nullptr);
    CB_CHECK(kinds.find("b") == nullptr);
    CB_CHECK_THROWS_AS(UsageError, kinds.add("a", [](const Args&) { return 2; }));
});

CB_TEST("jobs.workers.dispatch_in_process", [] {
    WorkerKinds kinds;
    Args seen;
    kinds.add("take", [&seen](const Args& a) {
        seen = a;
        return 7;
    });
    kinds.add("boom", [](const Args&) -> int { throw ToolError("wcc", 1, "", "nope"); });
    CB_CHECK(!run_as_worker(kinds, Args{"prog"}));                              // not a worker
    CB_CHECK(!run_as_worker(kinds, Args{"prog", "build"}));
    CB_CHECK(!run_as_worker(kinds, Args{"prog", "--cb-worker"}));               // no kind: not a worker invocation
    CB_CHECK_EQ(run_as_worker(kinds, Args{"prog", "--cb-worker", "take", "x", "y"}).value(), 7);
    CB_CHECK((seen == Args{"x", "y"}));
    testing::CapturedStd captured;
    CB_CHECK_EQ(run_as_worker(kinds, Args{"prog", "--cb-worker", "nosuch"}).value(), kWorkerUnknownKind);
    CB_CHECK_EQ(run_as_worker(kinds, Args{"prog", "--cb-worker", "boom"}).value(), kWorkerFailed);
    CB_CHECK(captured.err().find("worker boom") != std::string::npos);
    CB_CHECK(captured.err().find("nosuch") != std::string::npos);
});

CB_TEST("jobs.workers.launcher_command_and_disabled", [] {
    const WorkerLauncher l("C:/x/codebreaker.exe");
    CB_CHECK(l.enabled());
    const std::vector<std::string> expected = {"C:/x/codebreaker.exe", "--cb-worker", "gate", "a b", "c"};
    CB_CHECK(l.command("gate", {"a b", "c"}) == expected);
    const WorkerLauncher none{std::filesystem::path()};
    CB_CHECK(!none.enabled());
    CB_CHECK_THROWS_AS(UsageError, (void)none.run("gate", {}));
    const fs::ScopedEnv off("CB_NO_WORKERS", "1");
    CB_CHECK(!WorkerLauncher("C:/x/codebreaker.exe").enabled());               // the switch of the old program
    CB_CHECK(WorkerLauncher::for_this_program().exe().empty());
});

CB_TEST("jobs.workers.this_program_knows_its_path", [] {
    const WorkerLauncher self = WorkerLauncher::for_this_program();
    CB_CHECK(self.enabled());
    CB_CHECK(self.exe().is_absolute());
    CB_CHECK(std::filesystem::exists(self.exe()));
});

CB_TEST("jobs.workers.real_process_exit_codes", [] {
    const WorkerLauncher l = probe();
    CB_CHECK_EQ(l.run("exit", {"0"}).exit_code, 0u);
    CB_CHECK(l.run("exit", {"0"}).ok());
    CB_CHECK_EQ(l.run("exit", {"5"}).exit_code, 5u);
    CB_CHECK(!l.run("exit", {"5"}).ok());
    testing::CapturedStd captured;
    CB_CHECK_EQ(l.run("nosuch", {}).exit_code, static_cast<std::uint32_t>(kWorkerUnknownKind));
    CB_CHECK_EQ(l.run("throw", {}).exit_code, static_cast<std::uint32_t>(kWorkerFailed));
});

CB_TEST("jobs.workers.real_process_handover", [] {
    const WorkerLauncher l = probe();
    const Handover h("test");
    TokenWriter in;
    in.integer(21).text("ab \xC3\xA9");
    h.write("in.txt", in.str());
    const WorkerResult r = l.run("double", {fs::to_text(h.file("in.txt")), fs::to_text(h.file("out.txt"))});
    CB_CHECK(r.ok());
    TokenReader out(h.read("out.txt"));
    CB_CHECK_EQ(out.integer(), 42);
    CB_CHECK_EQ(out.text(), "ab \xC3\xA9" "ab \xC3\xA9");
});

CB_TEST("jobs.workers.a_crashing_worker_is_an_exit_code", [] {
    // The guest crashed the worker: the parent sees the code and goes on (a sibling is unaffected).
    const WorkerLauncher l = probe();
    const WorkerResult crashed = l.run("crash", {});
    CB_CHECK_EQ(crashed.exit_code, 0xC0000005u);
    CB_CHECK(!crashed.ok());
    CB_CHECK(!crashed.timed_out);
    CB_CHECK(l.run("exit", {"0"}).ok());
});

CB_TEST("jobs.workers.timeout_ends_the_worker", [] {
    const WorkerLauncher l = probe();
    LaunchOptions o;
    o.timeout = std::chrono::milliseconds(700);
    const auto t0 = std::chrono::steady_clock::now();
    const WorkerResult r = l.run("sleep", {}, o);
    CB_CHECK(r.timed_out);
    CB_CHECK(!r.ok());
    CB_CHECK(std::chrono::steady_clock::now() - t0 < std::chrono::seconds(30));
});

CB_TEST("jobs.workers.handover_is_removed", [] {
    std::filesystem::path where;
    {
        const Handover h("cleanup");
        where = h.path();
        h.write("x.txt", "1");
        CB_CHECK(std::filesystem::exists(where / "x.txt"));
    }
    CB_CHECK(!std::filesystem::exists(where));
    // and on an exception
    try {
        const Handover h("cleanup");
        where = h.path();
        throw std::runtime_error("x");
    } catch (const std::runtime_error&) {
    }
    CB_CHECK(!std::filesystem::exists(where));
    CB_CHECK_THROWS_AS(NotFoundError, (void)Handover().read("absent.txt"));
});

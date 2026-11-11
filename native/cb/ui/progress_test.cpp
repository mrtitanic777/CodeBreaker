#include "cb/ui/progress.hpp"

#include <fstream>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}
namespace pr = cb::ui::progress;
using std::chrono::seconds;

pr::KindReading pe_kind() {
    pr::KindReading k{.kind = "pe", .name = "Win32 DLLs", .modules = 10, .measured = 4, .functions = 40};
    k.classes = {{"verified", "Verified", 25}, {"weak", "Weak", 5}, {"unverified", "Unverified", 0}, {"no-compile", "No compile", 10}};
    return k;
}

// a store that holds `fresh` results of the last ten minutes, the newest `age` seconds old
pr::ReadFn store_with(std::uint64_t fresh, seconds age) {
    return [=](pr::Clock::time_point now) {
        pr::StoreReading r;
        r.store_path = "state/functions.sqlite";
        r.kinds = {pe_kind()};
        r.opened = true;
        r.fresh = fresh;
        if (fresh != 0) r.recent = {{"comctl32.dll", "Win32 DLL", 12, now - age}};
        return r;
    };
}

void write(const std::filesystem::path& p, const std::string& text) {
    std::filesystem::create_directories(p.parent_path());
    std::ofstream(p, std::ios::binary) << text;
}
}  // namespace

CB_TEST("ui.progress.clock_text", [] {
    CB_CHECK_EQ(pr::clock_text(std::nullopt), std::string("-"));
    CB_CHECK_EQ(pr::clock_text(seconds(0)), std::string("0m 00s"));
    CB_CHECK_EQ(pr::clock_text(seconds(187)), std::string("3m 07s"));
    CB_CHECK_EQ(pr::clock_text(seconds(3600)), std::string("1h 00m"));
    CB_CHECK_EQ(pr::clock_text(seconds(3900 + 59)), std::string("1h 05m"));
});

CB_TEST("ui.progress.snapshot_rate_and_eta", [] {
    // 30 results in the last ten minutes: 3 a minute. 6 of 10 modules left at 40/4 = 10 functions a module:
    // 60 functions at 0.05 a second is 1200 s.
    const auto now = pr::Clock::now();
    const pr::Snapshot s = pr::snapshot(store_with(30, seconds(45)), std::nullopt, now);
    CB_CHECK_EQ(s.rate, 3.0);
    CB_CHECK_EQ(s.idle.value().count(), std::int64_t{45});
    CB_CHECK(s.active);
    CB_CHECK_EQ(s.eta.value().count(), std::int64_t{1200});
    CB_CHECK_EQ(s.recent.size(), std::size_t{1});
    CB_CHECK_EQ(s.recent[0].module, std::string("comctl32.dll"));
    CB_CHECK_EQ(s.store, std::string("state/functions.sqlite"));
    CB_CHECK_EQ(s.time.size(), std::size_t{8});
});

CB_TEST("ui.progress.snapshot_idle_and_empty", [] {
    // nothing for ten minutes: no rate, no estimate, idle
    const pr::Snapshot idle = pr::snapshot(store_with(0, seconds(0)), std::nullopt);
    CB_CHECK_EQ(idle.rate, 0.0);
    CB_CHECK(!idle.idle);
    CB_CHECK(!idle.eta);
    CB_CHECK(!idle.active || idle.workers != 0);   // (another run's workers may be about on the machine)
    // an old last result is not "active" either
    const pr::Snapshot old = pr::snapshot(store_with(1, seconds(500)), std::nullopt);
    CB_CHECK_EQ(old.idle.value().count(), std::int64_t{500});
    CB_CHECK_EQ(old.rate, 0.1);
    // no store: only the workers and the log
    const pr::Snapshot none = pr::snapshot([](pr::Clock::time_point) { return pr::StoreReading{.store_path = "s"}; }, std::nullopt);
    CB_CHECK(none.kinds.empty());
    CB_CHECK(none.recent.empty());
    CB_CHECK_EQ(none.store, std::string("s"));
});

CB_TEST("ui.progress.line", [] {
    pr::Snapshot s;
    s.time = "13:05:09";
    CB_CHECK_EQ(pr::line(s), std::string("[13:05:09] idle"));   // a reading with nothing in it
    s.kinds = {pe_kind()};
    s.recent = {{"comctl32.dll", "Win32 DLL", 12, seconds(45)}};
    s.rate = 3.0;
    s.idle = seconds(45);
    s.active = true;
    s.workers = 2;
    s.eta = seconds(1200);
    s.log = "pass 3 of 7";
    CB_CHECK_EQ(pr::line(s),
                std::string("[13:05:09] Win32 DLLs 4/10 modules, 40 functions (Verified 25, Weak 5, No compile 10) | "
                            "last: comctl32.dll 0m 45s ago | 3.0 results/min | 2 workers busy | about 20m 00s left | "
                            "pass 3 of 7"));
    // a kind with no functions yet; working with no rate
    pr::Snapshot w;
    w.time = "00:00:00";
    w.kinds = {{.kind = "ne", .name = "Win16", .modules = 3}};
    w.kinds[0].classes = {{"verified", "Verified", 0}};
    w.active = true;
    CB_CHECK_EQ(pr::line(w), std::string("[00:00:00] Win16 0/3 modules, 0 functions (nothing yet) | working"));
    // a long log line is cut at 80 columns
    pr::Snapshot l;
    l.time = "x";
    l.log = std::string(100, 'a');
    CB_CHECK_EQ(pr::line(l), std::string("[x] idle | ") + std::string(80, 'a'));
});

CB_TEST("ui.progress.last_line", [] {
    cb::fs::ScopedTempDir dir("cb-progress");
    const auto log = dir.path() / "run.log";
    write(log, "first\nsecond  \n\n   \n");
    CB_CHECK_EQ(pr::last_line(log), std::string("second"));
    write(log, "10%\r20%\r30%");   // a meter rewriting its line
    CB_CHECK_EQ(pr::last_line(log), std::string("30%"));
    write(log, "");
    CB_CHECK_EQ(pr::last_line(log), std::string());
    // only the end of a big file is read; the line that is cut at the start of the window is not the last
    write(log, std::string(10000, 'x') + "\nend of log\n");
    CB_CHECK_EQ(pr::last_line(log), std::string("end of log"));
    // a missing file is no line
    CB_CHECK_EQ(pr::last_line(dir.path() / "nothing.log"), std::string());
});

CB_TEST("ui.progress.busy_workers", [] {
    cb::fs::ScopedTempDir dir("cb-progress");
    const auto now = pr::Clock::now();
    write(dir.path() / "cbjobs1" / "w0" / "a.cur", "item 4");       // in the middle of an item
    write(dir.path() / "CBJOBS2" / "w1" / "b.CUR", "item 9");       // (names compare without case on Windows)
    write(dir.path() / "cbjobs1" / "w1" / "c.cur", "");             // finished: an empty note
    write(dir.path() / "cbjobs1" / "w2" / "d.txt", "item");         // not a note
    write(dir.path() / "other" / "w0" / "e.cur", "item");           // not the runner's folder
    write(dir.path() / ".cbjobs3" / "w0" / "f.cur", "item");        // hidden
    write(dir.path() / "cbjobs4" / "w0" / "old.cur", "item 1");     // a worker that is gone: the note is a day old
    std::filesystem::last_write_time(dir.path() / "cbjobs4" / "w0" / "old.cur",
                                     std::filesystem::file_time_type::clock::now() - std::chrono::hours(24));
    CB_CHECK_EQ(pr::busy_workers(dir.path(), now), std::uint64_t{2});
    CB_CHECK_EQ(pr::busy_workers(dir.path() / "missing", now), std::uint64_t{0});
});

CB_TEST("ui.progress.watch_once_and_command", [] {
    const auto read = store_with(30, seconds(45));
    cb::testing::CapturedStd captured;
    pr::Options options;
    options.once = true;
    CB_CHECK_EQ(pr::watch(options, read), 0);
    const std::string text = captured.out();
    CB_CHECK(text.starts_with("["));
    CB_CHECK(contains(text, "Win32 DLLs 4/10 modules, 40 functions (Verified 25, Weak 5, No compile 10)"));
    CB_CHECK(contains(text, "3.0 results/min"));
    CB_CHECK(text.ends_with("\n"));

    // the declared command: --once, with a log file
    cb::fs::ScopedTempDir dir("cb-progress");
    write(dir.path() / "run.log", "building lz32\n");
    cb::cli::Cli cli("codebreaker", {pr::command(read)});
    CB_CHECK_EQ(cli.run({"progress", "--once", "--log", cb::fs::to_text(dir.path() / "run.log")}), 0);
    CB_CHECK(contains(captured.out(), "building lz32"));
    CB_CHECK_THROWS_AS(cb::UsageError, (void)cli.run({"progress", "--once", "--interval", "soon"}));
});

CB_TEST("ui.progress.watch_stops_on_request", [] {
    cb::testing::CapturedStd captured;
    std::stop_source source;
    source.request_stop();   // asked to stop before the first reading
    CB_CHECK_EQ(pr::watch({}, store_with(0, seconds(0)), source.get_token()), 0);
    CB_CHECK(captured.out().empty());
});

#include "cb/ui/progress.hpp"

#include <io.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <format>
#include <system_error>
#include <thread>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::ui::progress {

namespace stdfs = std::filesystem;

namespace {

constexpr std::chrono::hours kNoteFresh{1};        // a worker's note older than this is a worker that is gone
constexpr std::size_t kLogTail = 4096;             // how much of the end of the log is looked at
constexpr std::size_t kLogShown = 80;              // columns of the log's last line a line of text shows
constexpr std::size_t kLineWidth = 200;            // columns of a rewritten line
constexpr std::chrono::seconds kIdleAfter{120};    // a run with no result for this long is idle
constexpr std::size_t kRecentRows = 8;

// A name a shell wildcard would not match (a leading dot).
bool hidden(const stdfs::path& p) { return p.filename().generic_string().starts_with('.'); }

// The directories and files of `dir` that are not hidden; empty when it cannot be read.
std::vector<stdfs::directory_entry> entries(const stdfs::path& dir) {
    std::vector<stdfs::directory_entry> out;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        if (!hidden(it->path())) out.push_back(*it);
    }
    return out;
}

std::string local_time_text(Clock::time_point t) {
    const std::time_t seconds = Clock::to_time_t(t);
    std::tm parts{};
    localtime_s(&parts, &seconds);
    return std::format("{:02}:{:02}:{:02}", parts.tm_hour, parts.tm_min, parts.tm_sec);
}

double round_tenth(double x) { return std::round(x * 10.0) / 10.0; }

}  // namespace

std::uint64_t busy_workers(const stdfs::path& temp, Clock::time_point now) {
    std::uint64_t busy = 0;
    for (const auto& jobs : entries(temp)) {   // <temp>/cbjobs*/*/*.cur
        if (!jobs.is_directory() || !text::istarts_with(jobs.path().filename().generic_string(), "cbjobs")) continue;
        for (const auto& worker : entries(jobs.path())) {
            if (!worker.is_directory()) continue;
            for (const auto& note : entries(worker.path())) {
                if (!text::iends_with(note.path().filename().generic_string(), ".cur")) continue;
                std::error_code ec;
                const auto size = note.file_size(ec);
                if (ec || size == 0) continue;
                const auto written = note.last_write_time(ec);
                if (ec) continue;
                if (now - std::chrono::clock_cast<Clock>(written) < kNoteFresh) ++busy;
            }
        }
    }
    return busy;
}

std::string last_line(const stdfs::path& file) {
    try {
        const fs::MappedFile mapped(file);
        ByteView view = mapped.view();
        if (view.size() > kLogTail) view = view.subspan(view.size() - kLogTail);
        // a lone \r is a line end here too: a progress meter rewrites its line with it
        std::string tail = text::sanitize_utf8(as_text(view));
        std::ranges::replace(tail, '\r', '\n');
        std::string_view last;
        for (const std::string_view l : text::lines(tail)) {
            if (!text::trim(l).empty()) last = text::trim(l);
        }
        return std::string(last);
    } catch (const IoError&) {
        return {};
    }
}

Snapshot snapshot(const ReadFn& read, const std::optional<stdfs::path>& log, Clock::time_point now) {
    Snapshot out;
    out.time = local_time_text(now);
    out.workers = busy_workers(stdfs::temp_directory_path(), now);
    if (log && !log->empty()) out.log = last_line(*log);
    const StoreReading store = read(now);
    out.store = store.store_path;
    out.kinds = store.kinds;
    if (!store.opened) return out;
    for (const RecentResult& r : store.recent) {
        const auto age = std::chrono::duration_cast<std::chrono::seconds>(now - r.last_written);
        out.recent.push_back({.module = r.module, .kind = r.kind, .results = r.results,
                              .age = std::max(age, std::chrono::seconds{0})});
    }
    const double window = static_cast<double>(kWindow.count());
    out.rate = round_tenth(static_cast<double>(store.fresh) * 60.0 / window);
    if (!out.recent.empty()) out.idle = out.recent.front().age;
    out.active = out.workers != 0 || (out.idle && *out.idle < kIdleAfter);
    std::uint64_t done = 0, left = 0, functions = 0;
    for (const KindReading& k : out.kinds) {
        done += k.measured;
        left += k.modules - k.measured;
        functions += k.functions;
    }
    if (store.fresh != 0 && done != 0 && left != 0) {   // as many functions a module as so far
        const double per_second = static_cast<double>(store.fresh) / window;
        out.eta = std::chrono::seconds(static_cast<std::int64_t>(
            static_cast<double>(left) * (static_cast<double>(functions) / static_cast<double>(done)) / per_second));
    }
    return out;
}

std::string clock_text(std::optional<std::chrono::seconds> span) {
    if (!span) return "-";
    const auto total = span->count();
    const auto hours = total / 3600, rest = total % 3600;
    return hours != 0 ? std::format("{}h {:02}m", hours, rest / 60) : std::format("{}m {:02}s", rest / 60, rest % 60);
}

std::string line(const Snapshot& s) {
    std::vector<std::string> parts;
    for (const KindReading& k : s.kinds) {
        std::vector<std::string> classes;
        for (const ClassCount& c : k.classes) {
            if (c.count != 0) classes.push_back(std::format("{} {}", c.title, c.count));
        }
        parts.push_back(std::format("{} {}/{} modules, {} functions ({})", k.name, k.measured, k.modules, k.functions,
                                    classes.empty() ? "nothing yet" : text::join(classes, ", ")));
    }
    if (!s.recent.empty()) parts.push_back(std::format("last: {} {} ago", s.recent.front().module, clock_text(s.idle)));
    parts.push_back(s.rate != 0.0 ? std::format("{:.1f} results/min", s.rate) : !s.active ? "idle" : "working");
    if (s.workers != 0) parts.push_back(std::format("{} workers busy", s.workers));
    if (s.eta) parts.push_back(std::format("about {} left", clock_text(s.eta)));
    if (!s.log.empty()) parts.push_back(std::string(text::utf8_prefix(s.log, kLogShown)));
    return std::format("[{}] {}", s.time, parts.empty() ? "no results yet" : text::join(parts, " | "));
}

int watch(const Options& options, const ReadFn& read, std::stop_token stop) {
    const bool console = _isatty(1) != 0;
    std::size_t width = 0;
    while (!stop.stop_requested()) {
        const std::string text = line(snapshot(read, options.log));
        if (options.once || !console) {
            out("{}\n", text);
            flush_out();
        } else {
            const std::string shown(text::utf8_prefix(text, kLineWidth));
            const std::size_t columns = text::display_width(shown);
            out("\r{}{}", shown, std::string(width > columns ? width - columns : 0, ' '));
            flush_out();
            width = columns;
        }
        if (options.once) return kExitOk;
        // sleep in slices so a stop request is answered within a tenth of a second
        const auto wake = std::chrono::steady_clock::now() + options.interval;
        while (!stop.stop_requested() && std::chrono::steady_clock::now() < wake) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    if (console && !options.once) out("\n");
    return kExitOk;
}

cli::Command command(ReadFn read, std::stop_token stop) {
    return cli::Command{
        .name = "progress",
        .summary = "show how a running sweep or module run is getting on, read from the function store",
        .params = {cli::Option{"--once", "", "print the status once and stop", cli::Arg::Flag},
                   cli::Option{"--interval", "", "seconds between readings", cli::Arg::Value, "SECONDS", "3"},
                   cli::Option{"--log", "", "also show the last line of a run's output file", cli::Arg::Value,
                               "FILE"}},
        .handler =
            [read = std::move(read), stop](const cli::Args& args) {
                Options options;
                options.once = args.flag("--once");
                // (an interval below a second would only hammer the store)
                const auto seconds = args.value_or("--interval", "3");
                double value = 3.0;
                const auto [end, ec] = std::from_chars(seconds.data(), seconds.data() + seconds.size(), value);
                if (ec != std::errc() || end != seconds.data() + seconds.size())
                    throw UsageError("--interval expects a number of seconds, got '" + seconds + "'");
                options.interval = std::chrono::duration<double>(std::max(1.0, value));
                if (const auto log = args.value("--log")) options.log = fs::from_text(*log);
                return watch(options, read, stop);
            },
        .area = "Run",
    };
}

}  // namespace cb::ui::progress

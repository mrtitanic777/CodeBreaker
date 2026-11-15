#include "cb/core/testing.hpp"


#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/sys.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <exception>
#include <fstream>
#include <mutex>

#ifndef CB_SOURCE_DIR
#define CB_SOURCE_DIR "."
#endif

namespace cb::testing {

namespace {

struct Entry {
    std::string name;
    TestFn fn;
};

// The registry is built during static initialisation of the test translation units, so it must itself
// be safe to touch before main: a function-local static.
std::vector<Entry>& registry() {
    static std::vector<Entry> entries;
    return entries;
}

struct Run {
    std::vector<Failure> failures;
    std::size_t checks = 0;
};
// The test being run. Tests run one at a time, but a test may check from worker threads, so this is process-wide
// (not thread_local) and guarded by a mutex.
Run* g_current = nullptr;
std::mutex g_mutex;

std::string xml_escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '&': out += "&amp;"; break;
            case '"': out += "&quot;"; break;
            default: out += c;
        }
    }
    return out;
}

}  // namespace

Registrar::Registrar(const char* name, TestFn fn) { registry().push_back({name, std::move(fn)}); }

void record_failure(const char* file, int line, std::string message) {
    std::lock_guard lock(g_mutex);
    if (g_current != nullptr) g_current->failures.push_back({file, line, std::move(message)});
}

void count_check() {
    std::lock_guard lock(g_mutex);
    if (g_current != nullptr) ++g_current->checks;
}

CapturedStd::CapturedStd() {
    dir_ = fs::full_names(fs::stdfs::temp_directory_path()) / fs::from_text("cbtest-capture-" + fs::unique_suffix());
    fs::make_dirs(dir_);
    flush_out();
    out_ = std::make_unique<sys::StreamRedirect>(sys::Stream::Out, dir_ / "out.txt");
    err_ = std::make_unique<sys::StreamRedirect>(sys::Stream::Err, dir_ / "err.txt");
}

CapturedStd::~CapturedStd() {
    flush_out();
    err_.reset();
    out_.reset();
    std::error_code ec;
    std::filesystem::remove_all(dir_, ec);
}

std::string CapturedStd::out() {
    flush_out();
    return fs::read_text(dir_ / "out.txt");
}

std::string CapturedStd::err() { return fs::read_text(dir_ / "err.txt"); }

std::filesystem::path data_path(std::string_view relative) {
    return std::filesystem::path(CB_SOURCE_DIR) / "cb" / fs::from_text(relative);
}

std::filesystem::path local_input(std::string_view relative) {
    const std::filesystem::path checkout = std::filesystem::path(CB_SOURCE_DIR).parent_path();
    return (checkout / "binaries" / "test-inputs" / fs::from_text(relative)).lexically_normal();
}

std::filesystem::path beside_checkout(std::string_view relative) {
    const std::filesystem::path parent = std::filesystem::path(CB_SOURCE_DIR).parent_path().parent_path();
    return (parent / fs::from_text(relative)).lexically_normal();
}

int run_all(int argc, char** argv) {
    std::string filter;
    std::string xml_path;
    bool list_only = false;
    std::size_t shard = 0, shards = 1;   // --shard K/N: every Nth test of the sorted list, from K (the gate runs N processes)
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--filter" && i + 1 < argc) filter = argv[++i];
        else if (arg == "--xml" && i + 1 < argc) xml_path = argv[++i];
        else if (arg == "--list") list_only = true;
        else if (arg == "--shard" && i + 1 < argc) {
            const std::string spec = argv[++i];
            const std::size_t slash = spec.find('/');
            if (slash == std::string::npos) { std::fprintf(stderr, "--shard K/N\n"); return 2; }
            shard = std::stoul(spec.substr(0, slash));
            shards = std::stoul(spec.substr(slash + 1));
            if (shards == 0 || shard >= shards) { std::fprintf(stderr, "--shard K/N: K below N\n"); return 2; }
        }
        else {
            std::fprintf(stderr, "usage: cb_tests [--filter <substring>] [--shard K/N] [--list] [--xml <file>]\n");
            return 2;
        }
    }

    auto& tests = registry();
    std::stable_sort(tests.begin(), tests.end(), [](const Entry& a, const Entry& b) { return a.name < b.name; });

    std::size_t passed = 0, failed = 0, total_checks = 0;
    std::string xml = "<testsuite name=\"cb_tests\">\n";
    std::size_t index = 0;
    for (const Entry& t : tests) {
        if (!filter.empty() && t.name.find(filter) == std::string::npos) continue;
        if (index++ % shards != shard) continue;
        if (list_only) {
            std::printf("%s\n", t.name.c_str());
            continue;
        }
        Run run;
        g_current = &run;
        const auto t0 = std::chrono::steady_clock::now();
        try {
            t.fn();
        } catch (const std::exception& e) {
            run.failures.push_back({"", 0, std::string("uncaught exception: ") + e.what()});
        } catch (...) {
            run.failures.push_back({"", 0, "uncaught non-standard exception"});
        }
        g_current = nullptr;
        const double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        total_checks += run.checks;
        xml += "  <testcase name=\"" + xml_escape(t.name) + "\" time=\"" + std::format("{:.3f}", secs) + "\"";
        if (run.failures.empty()) {
            ++passed;
            std::printf("PASS  %s\n", t.name.c_str());
            xml += "/>\n";
        } else {
            ++failed;
            std::printf("FAIL  %s\n", t.name.c_str());
            xml += ">\n";
            for (const Failure& f : run.failures) {
                std::printf("    %s:%d: %s\n", f.file.c_str(), f.line, f.message.c_str());
                xml += "    <failure message=\"" + xml_escape(f.file + ":" + std::to_string(f.line) + ": " + f.message) + "\"/>\n";
            }
            xml += "  </testcase>\n";
        }
    }
    xml += "</testsuite>\n";
    if (list_only) return 0;

    std::printf("\n%zu passed, %zu failed (%zu checks)\n", passed, failed, total_checks);
    if (!xml_path.empty()) {
        std::ofstream out(xml_path, std::ios::binary);
        out << xml;
    }
    return failed == 0 ? 0 : 1;
}

}  // namespace cb::testing

#include "cb/core/testing.hpp"

#include <windows.h>

#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/win.hpp"

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
    dir_ = fs::stdfs::temp_directory_path() /
           fs::from_text("cbtest-capture-" + std::to_string(GetCurrentProcessId()) + "-" +
                         std::to_string(GetTickCount64()) + "-" + std::to_string(reinterpret_cast<std::uintptr_t>(this)));
    fs::make_dirs(dir_);
    SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
    auto open = [&](const char* name) {
        return CreateFileW((dir_ / name).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    };
    out_ = open("out.txt");
    err_ = open("err.txt");
    flush_out();
    saved_out_ = GetStdHandle(STD_OUTPUT_HANDLE);
    saved_err_ = GetStdHandle(STD_ERROR_HANDLE);
    SetStdHandle(STD_OUTPUT_HANDLE, out_);
    SetStdHandle(STD_ERROR_HANDLE, err_);
}

CapturedStd::~CapturedStd() {
    flush_out();
    SetStdHandle(STD_OUTPUT_HANDLE, saved_out_);
    SetStdHandle(STD_ERROR_HANDLE, saved_err_);
    CloseHandle(out_);
    CloseHandle(err_);
    std::error_code ec;
    std::filesystem::remove_all(dir_, ec);
}

std::string CapturedStd::out() {
    flush_out();
    return fs::read_text(dir_ / "out.txt");
}

std::string CapturedStd::err() { return fs::read_text(dir_ / "err.txt"); }

std::filesystem::path data_path(std::string_view relative) {
    return std::filesystem::path(CB_SOURCE_DIR) / "cb" / std::filesystem::path(win::to_wide(relative));
}

int run_all(int argc, char** argv) {
    std::string filter;
    std::string xml_path;
    bool list_only = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--filter" && i + 1 < argc) filter = argv[++i];
        else if (arg == "--xml" && i + 1 < argc) xml_path = argv[++i];
        else if (arg == "--list") list_only = true;
        else {
            std::fprintf(stderr, "usage: cb_tests [--filter <substring>] [--list] [--xml <file>]\n");
            return 2;
        }
    }

    auto& tests = registry();
    std::stable_sort(tests.begin(), tests.end(), [](const Entry& a, const Entry& b) { return a.name < b.name; });

    std::size_t passed = 0, failed = 0, total_checks = 0;
    std::string xml = "<testsuite name=\"cb_tests\">\n";
    for (const Entry& t : tests) {
        if (!filter.empty() && t.name.find(filter) == std::string::npos) continue;
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

#include "core/testing.hpp"


#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/sys.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <exception>
#include <fstream>
#include <cstdlib>
#include <map>
#include <mutex>
#include <set>

#include <nlohmann/json.hpp>

#ifndef CB_SOURCE_DIR
#define CB_SOURCE_DIR "."
#endif

namespace cb::testing {

namespace {

struct Entry {
    std::string name;
    TestFn fn;
    std::string file;   // the source file of the test ("" when it was registered by hand): `--area` reads its folder
};

// The area of a test file: the folder under src ("/src/engine/show_test.cpp" -> "engine"), "" when there is none.
std::string area_of(const std::string& file) {
    std::string f = file;
    for (char& c : f)
        if (c == '\\') c = '/';
    const std::size_t at = f.find("src/");
    if (at == std::string::npos) return {};
    const std::size_t from = at + 10, slash = f.find('/', from);
    return slash == std::string::npos ? std::string() : f.substr(from, slash - from);
}

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

Registrar::Registrar(const char* name, TestFn fn) { registry().push_back({name, std::move(fn), {}}); }
Registrar::Registrar(const char* name, const char* file, TestFn fn) { registry().push_back({name, std::move(fn), file}); }

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
    return std::filesystem::path(CB_SOURCE_DIR) / fs::from_text(relative);
}

std::filesystem::path local_input(std::string_view relative) {
    const std::filesystem::path checkout = std::filesystem::path(CB_SOURCE_DIR).parent_path();
    return (checkout / "test-inputs" / fs::from_text(relative)).lexically_normal();
}

std::filesystem::path beside_checkout(std::string_view relative) {
    // A corpus kept in this checkout's test-inputs comes first (homebrew-samples lives there); otherwise a checkout next to this one.
    if (std::filesystem::path own = local_input(relative); std::filesystem::exists(own)) return own;
    const std::filesystem::path parent = std::filesystem::path(CB_SOURCE_DIR).parent_path().parent_path();
    return (parent / fs::from_text(relative)).lexically_normal();
}

std::filesystem::path work_corpus(std::string_view relative) {
    namespace stdfs = std::filesystem;
    auto read = [](const stdfs::path& file) {
        nlohmann::json doc;
        try {
            std::error_code ec;
            if (stdfs::is_regular_file(file, ec)) doc = nlohmann::json::parse(fs::read_text(file));
        } catch (...) {
        }
        return doc.is_object() ? doc : nlohmann::json::object();
    };
    auto text_of = [](const nlohmann::json& doc, const char* key) {
        return doc.contains(key) && doc[key].is_string() ? doc[key].get<std::string>() : std::string();
    };
    auto against = [](const stdfs::path& base, const std::string& text) {
        const stdfs::path p = fs::from_text(text);
        return (p.is_relative() ? base / p : p).lexically_normal();
    };
    stdfs::path corpus;
    if (const auto env = fs::get_env("CB_CORPUS"); env && !env->empty()) {
        corpus = fs::from_text(*env);
    } else {
        const stdfs::path checkout = stdfs::path(CB_SOURCE_DIR).parent_path();
        const nlohmann::json settings = read(checkout / "settings.json");
        if (const std::string c = text_of(settings, "corpus"); !c.empty()) {
            corpus = against(checkout, c);
        } else {
            std::string folder = fs::get_env("CB_WORK_FOLDER").value_or("");
            if (folder.empty()) folder = text_of(settings, "work_folder");
            if (folder.empty()) return {};
            const stdfs::path work = against(checkout, folder);
            const std::string own = text_of(read(work / "codebreaker.json"), "corpus");
            if (own.empty()) return {};
            corpus = against(work, own);
        }
    }
    return (corpus / fs::from_text(relative)).lexically_normal();
}

std::string rooted(std::string_view relative) {
#ifdef _WIN32
    return "C:/" + std::string(relative);
#else
    return "/" + std::string(relative);
#endif
}

bool windows_only(std::string_view reason) {
#ifdef _WIN32
    (void)reason;
    return false;
#else
    std::fprintf(stderr, "  (not run off Windows: %.*s)\n", static_cast<int>(reason.size()), reason.data());
    return true;
#endif
}

int run_all(int argc, char** argv) {
    std::string filter;
    std::string xml_path;
    bool list_only = false, times = false;
    std::string weights_path, times_file;
    std::set<std::string> areas;         // --area a,b: only the tests of these areas (a test whose file is not known is always run)
    std::size_t shard = 0, shards = 1;   // --shard K/N: every Nth test of the sorted list, from K (the gate runs N processes)
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--filter" && i + 1 < argc) filter = argv[++i];
        else if (arg == "--xml" && i + 1 < argc) xml_path = argv[++i];
        else if (arg == "--list") list_only = true;
        else if (arg == "--times") times = true;
        else if (arg == "--weights" && i + 1 < argc) weights_path = argv[++i];   // "seconds<TAB>name" lines: how long each test took last time
        else if (arg == "--times-file" && i + 1 < argc) times_file = argv[++i];   // writes the same lines for the tests this run ran
        else if (arg == "--area" && i + 1 < argc) {
            const std::string list = argv[++i];
            for (std::size_t from = 0; from <= list.size();) {
                const std::size_t comma = std::min(list.find(',', from), list.size());
                if (comma > from) areas.insert(list.substr(from, comma - from));
                from = comma + 1;
            }
        }
        else if (arg == "--shard" && i + 1 < argc) {
            const std::string spec = argv[++i];
            const std::size_t slash = spec.find('/');
            if (slash == std::string::npos) { std::fprintf(stderr, "--shard K/N\n"); return 2; }
            shard = std::stoul(spec.substr(0, slash));
            shards = std::stoul(spec.substr(slash + 1));
            if (shards == 0 || shard >= shards) { std::fprintf(stderr, "--shard K/N: K below N\n"); return 2; }
        }
        else {
            std::fprintf(stderr, "usage: cb_tests [--filter <substring>] [--area a,b] [--shard K/N] [--list] [--times] [--xml <file>]\n");
            return 2;
        }
    }

    auto& tests = registry();
    std::stable_sort(tests.begin(), tests.end(), [](const Entry& a, const Entry& b) { return a.name < b.name; });

    std::size_t passed = 0, failed = 0, total_checks = 0;
    std::string xml = "<testsuite name=\"cb_tests\">\n";
    std::vector<std::pair<double, std::string>> timed;
    std::vector<const Entry*> selected;
    for (const Entry& t : tests) {
        if (!filter.empty() && t.name.find(filter) == std::string::npos) continue;
        if (!areas.empty() && !t.file.empty() && !areas.contains(area_of(t.file))) continue;
        selected.push_back(&t);
    }
    // This process's share. Without weights: every Nth of the sorted list. With them (seconds a test took last time): longest first onto the lightest
    // shard, so no shard is left with the slow tests together; every process computes the same split from the same file. A test not in the file weighs
    // what the median one does.
    std::vector<bool> mine(selected.size(), false);
    if (weights_path.empty()) {
        for (std::size_t i = 0; i < selected.size(); ++i) mine[i] = i % shards == shard;
    } else {
        std::map<std::string, double> weight;
        std::ifstream in(weights_path);
        for (std::string line; std::getline(in, line);) {
            const std::size_t tab = line.find('\t');
            if (tab == std::string::npos) continue;
            weight[line.substr(tab + 1)] = std::atof(line.substr(0, tab).c_str());
        }
        std::vector<double> known;
        for (const auto& [name, secs] : weight) known.push_back(secs);
        std::sort(known.begin(), known.end());
        const double unknown = known.empty() ? 0.05 : known[known.size() / 2];
        std::vector<double> w(selected.size());
        std::vector<std::size_t> order(selected.size());
        for (std::size_t i = 0; i < selected.size(); ++i) {
            const auto it = weight.find(selected[i]->name);
            w[i] = it != weight.end() ? it->second : unknown;
            order[i] = i;
        }
        std::stable_sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) { return w[a] > w[b]; });
        std::vector<double> load(shards, 0.0);
        for (const std::size_t i : order) {
            const std::size_t k = static_cast<std::size_t>(std::min_element(load.begin(), load.end()) - load.begin());
            load[k] += w[i];
            if (k == shard) mine[i] = true;
        }
    }
    for (std::size_t at = 0; at < selected.size(); ++at) {
        if (!mine[at]) continue;
        const Entry& t = *selected[at];
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
        if (times || !times_file.empty()) timed.emplace_back(secs, t.name);
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

    if (!times_file.empty()) {
        std::ofstream out(times_file, std::ios::binary);
        for (const auto& [secs, name] : timed) out << std::format("{:.3f}", secs) << '\t' << name << '\n';
    }
    if (times) {   // the slowest five of this run, for the gate's --times
        std::sort(timed.begin(), timed.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
        for (std::size_t k = 0; k < timed.size() && k < 5; ++k) std::printf("SLOW  %.2f  %s\n", timed[k].first, timed[k].second.c_str());
    }
    std::printf("\n%zu passed, %zu failed (%zu checks)\n", passed, failed, total_checks);
    if (!xml_path.empty()) {
        std::ofstream out(xml_path, std::ios::binary);
        out << xml;
    }
    return failed == 0 ? 0 : 1;
}

}  // namespace cb::testing

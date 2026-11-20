// cb_exe_gate -- runs the real codebreaker.exe on a list of command cases and compares what it prints and the files it
// writes with goldens. It is the command-level half of the gate (tools\gate.cmd): cb_tests checks the modules, this
// checks that every command of the program, as a user types it, still does what it did.
//
//     cb_exe_gate [--exe EXE] [--repo DIR] [--cases FILE] [--golden DIR] [--corpus DIR]
//                 [--jobs N] [--filter TEXT] [--update] [--keep] [--list] [--unit CB_TESTS_EXE [--shards N]]
//
//   --exe-root the checkout the program belongs to, when --exe is a wrapper (its paths are printed as <root>)
//   --exe      the program (default: <repo>/state/cxx/codebreaker/build/codebreaker.exe)
//   --repo     where inputs and cases live (default: the folder above native/, found from the current directory)
//   --cases    default <repo>/tests/exe_gate/cases.txt        --golden  default <repo>/tests/exe_gate/golden
//   --corpus   the corpus files (default: the CB_CORPUS environment variable, else the "corpus" of the work folder that
//              <repo>/settings.json names: its codebreaker.json, relative to it);
//              cases that need it are skipped, and counted as skipped, when it is not there
//   --jobs     cases run side by side (default: min(cores - 2, 12) when the machine is otherwise idle, else 4, the cap for a shared machine;
//              the CB_GATE_JOBS environment variable also sets it)
//   --shared   the old cap of 4 (other sessions are using the machine)
//   --times    after the run, list the slowest cases, test shards and tests (seconds)
//   --changed  only what a change can affect since the last passing gate: the cases and unit-test areas that depend on the files that differ from
//              the record (--record FILE, default <repo>/state/work/gate-last.txt) -- gate_changed.hpp has the rules. Nothing recorded: everything runs.
//   --no-record  do not write the record after a passing run (a run on an exe not built from this tree)
//   A passing whole run (no --filter or --update, with --unit) writes the record: the tree it started on.
//   --update   record the goldens of the selected cases from the program's current behaviour (review the diff!)
//   --unit     also run the module tests (cb_tests) as N shards (default 8) in the same pool, so the whole gate is one run
//   --keep     leave each case's scratch folder (state/work/exe_gate/run-PID) for inspection
//
// A case file line is `name: word word "a quoted word" @option ...`. Words may use {repo} (the inputs), {corpus},
// {work} (the case's own scratch folder; CB_STATE is {work}/state, so no case touches the real state), and {out}
// ({work}/out: every file the case leaves there is compared). Options:
//     @needs=corpus|games|toolchains|translator-cs|translator-py  skip when the corpus / the Xbox 360 games folder (CB_GAMES, C:/Nexia360/Games) /
//              this machine's toolchains (<checkout>/toolchains: period compilers, the driver kit; not in the repository) are absent /
//              the translator cannot write C++17 from C# or from Python here (the program is asked: convert FILE --languages)
//              @timeout=SECONDS   (default 120)
//     @stdin=FILE        feed this file (a {repo} path) to stdin  @mask=REGEX       replace matches by <~> before comparing
//     @sizeonly=.obj,.map  output files with these extensions are compared by name only (a build time and the source path are inside them)
//     @sort              compare stdout as sorted lines     @head=N   compare only its first N lines
//     @stderr            compare stderr too (otherwise it is kept in the golden for reading, not compared)
//     @cwd=work          run in {work} (default: the repo)
// What is compared: exit code, stdout, the files under {out} (text with paths normalised, else bytes), and stderr if
// asked. Absolute paths (the repo, the program's checkout, the corpus, the scratch folder, the home folder) and
// ISO timestamps are replaced by fixed names so a golden is the same on any checkout.
#include <windows.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <filesystem>
#include <format>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cb/app/gate_changed.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

#include <nlohmann/json.hpp>

namespace {

namespace stdfs = std::filesystem;
using namespace cb;

std::string env_var(const char* name);

// The corpus the checkout's configuration names: settings.json's "corpus", else its work folder's codebreaker.json's
// (relative to that folder). Empty when neither names one.
stdfs::path configured_corpus(const stdfs::path& repo) {
    auto read = [](const stdfs::path& file) {
        nlohmann::json doc;
        try {
            if (stdfs::is_regular_file(file)) doc = nlohmann::json::parse(fs::read_text(file));
        } catch (...) {
        }
        return doc.is_object() ? doc : nlohmann::json::object();
    };
    auto relative_to = [](const stdfs::path& base, const std::string& text) {
        const stdfs::path p = fs::from_text(text);
        return (p.is_relative() ? base / p : p).lexically_normal();
    };
    const nlohmann::json settings = read(repo / "settings.json");
    if (settings.contains("corpus") && settings["corpus"].is_string()) return relative_to(repo, settings["corpus"].get<std::string>());
    std::string folder = env_var("CB_WORK_FOLDER");
    if (folder.empty() && settings.contains("work_folder") && settings["work_folder"].is_string()) folder = settings["work_folder"].get<std::string>();
    if (folder.empty()) return {};
    const stdfs::path work = relative_to(repo, folder);
    const nlohmann::json own = read(work / "codebreaker.json");
    if (own.contains("corpus") && own["corpus"].is_string()) return relative_to(work, own["corpus"].get<std::string>());
    return {};
}

using Vars = std::vector<std::pair<std::string, std::string>>;   // `name = value` lines of the cases file: {name} in words

struct Case {
    std::string name;
    std::vector<std::string> argv;
    bool needs_corpus = false;
    bool needs_games = false;
    bool needs_toolchains = false;
    bool needs_translator_cs = false;   // the translator can write C++17 from C# on this machine (the .NET SDK and a C++ compiler)
    bool needs_translator_py = false;   // ... from Python (python, a C++ compiler and the L2 library)
    std::string line;                   // the case's line as written (--changed notices a case whose line changed)
    std::vector<std::string> areas;     // @areas=a,b: areas of native/cb the case depends on besides its command's (tests/exe_gate/areas.txt)
    int timeout_s = 120;
    std::string stdin_file;
    std::vector<std::string> masks;
    bool compare_stderr = false;
    bool cwd_work = false;
    bool sort_lines = false;   // stdout compared as a sorted set of lines (output a pool of workers orders as it likes)
    std::vector<std::string> size_only;   // extensions of output files whose bytes carry a build time (objects, maps, linked images): name and size only
    int head = 0;              // compare only the first N stdout lines (output that grows with the corpus)
};

struct Env {
    stdfs::path exe, repo, cases, golden, corpus, games, scratch;
    stdfs::path exe_root;   // the checkout the exe lives in (its settings, its corpus)
    bool have_corpus = false, have_games = false, have_toolchains = false, have_translator_cs = false, have_translator_py = false;
    int jobs = 4;
    bool update = false, keep = false, times = false;
};

// How many cases to run side by side. The project's cap (4) holds while other sessions use the machine; on an idle one the gate takes
// most of it: the busy share is measured over a third of a second (system times: idle, kernel which includes idle, user), and
// below a quarter busy the pool is min(cores - 2, 12).
int default_jobs() {
    constexpr int kShared = 4;
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    const int cores = static_cast<int>(si.dwNumberOfProcessors);
    const int wide = std::clamp(cores - 2, kShared, 12);
    auto ticks = [](FILETIME f) { return (static_cast<std::uint64_t>(f.dwHighDateTime) << 32) | f.dwLowDateTime; };
    FILETIME i0, k0, u0, i1, k1, u1;
    if (!GetSystemTimes(&i0, &k0, &u0)) return kShared;
    Sleep(330);
    if (!GetSystemTimes(&i1, &k1, &u1)) return kShared;
    const std::uint64_t total = (ticks(k1) - ticks(k0)) + (ticks(u1) - ticks(u0));
    const std::uint64_t idle = ticks(i1) - ticks(i0);
    if (total == 0) return kShared;
    const double busy = 1.0 - static_cast<double>(idle) / static_cast<double>(total);
    return busy < 0.25 ? wide : kShared;
}

std::string env_var(const char* name) {
    const DWORD need = GetEnvironmentVariableA(name, nullptr, 0);   // (the Win32 call: every Windows compiler's CRT has it)
    if (need == 0) return {};
    std::string out(need, '\0');
    out.resize(GetEnvironmentVariableA(name, out.data(), need));
    return out;
}

std::string slashes(std::string s) {
    std::replace(s.begin(), s.end(), '\\', '/');
    return s;
}

// A word list from a case line: whitespace separates, double quotes group (and may sit inside a word).
std::vector<std::string> words_of(std::string_view line) {
    std::vector<std::string> out;
    std::string cur;
    bool in_q = false, any = false;
    for (char c : line) {
        if (c == '"') {
            in_q = !in_q;
            any = true;
        } else if (!in_q && (c == ' ' || c == '\t')) {
            if (any) out.push_back(std::move(cur));
            cur.clear();
            any = false;
        } else {
            cur += c;
            any = true;
        }
    }
    if (any) out.push_back(std::move(cur));
    return out;
}

std::vector<Case> load_cases(const stdfs::path& file, Vars& vars) {
    std::vector<Case> cases;
    std::set<std::string> seen;
    int lineno = 0;
    const std::string text = fs::read_text(file);
    for (std::string_view raw : text::lines(text)) {
        ++lineno;
        const std::string_view line = text::trim(raw);
        if (line.empty() || line[0] == '#') continue;
        const std::size_t eq = line.find(" = ");
        const std::size_t colon = line.find(':');
        if (eq != std::string_view::npos && (colon == std::string_view::npos || eq < colon)) {
            vars.emplace_back(std::string(text::trim(line.substr(0, eq))), std::string(text::trim(line.substr(eq + 3))));
            continue;
        }
        if (colon == std::string_view::npos) throw ParseError(fs::to_text(file), lineno, 1, "a case line is `name: words`");
        Case c;
        c.name = std::string(text::trim(line.substr(0, colon)));
        if (!seen.insert(c.name).second) throw ParseError(fs::to_text(file), lineno, 1, "case name used twice: " + c.name);
        c.line = std::string(line);
        for (std::string& w : words_of(line.substr(colon + 1))) {
            if (w.rfind("@needs=", 0) == 0) {
                if (w.substr(7) == "corpus") c.needs_corpus = true;
                else if (w.substr(7) == "games") c.needs_games = true;
                else if (w.substr(7) == "toolchains") c.needs_toolchains = true;
                else if (w.substr(7) == "translator-cs") c.needs_translator_cs = true;
                else if (w.substr(7) == "translator-py") c.needs_translator_py = true;
                else throw ParseError(fs::to_text(file), lineno, 1, "unknown @needs " + w);
            }
            else if (w.rfind("@timeout=", 0) == 0) c.timeout_s = std::stoi(w.substr(9));
            else if (w.rfind("@stdin=", 0) == 0) c.stdin_file = w.substr(7);
            else if (w.rfind("@mask=", 0) == 0) c.masks.push_back(w.substr(6));
            else if (w == "@stderr") c.compare_stderr = true;
            else if (w == "@cwd=work") c.cwd_work = true;
            else if (w == "@sort") c.sort_lines = true;
            else if (w.rfind("@sizeonly=", 0) == 0) {
                for (std::string_view e : text::split(std::string_view(w).substr(10), ',')) c.size_only.emplace_back(text::lower(e));
            }
            else if (w.rfind("@head=", 0) == 0) c.head = std::stoi(w.substr(6));
            else if (w.rfind("@areas=", 0) == 0) {
                for (std::string_view a : text::split(std::string_view(w).substr(7), ',')) c.areas.emplace_back(a);
            }
            else c.argv.push_back(std::move(w));
        }
        cases.push_back(std::move(c));
    }
    return cases;
}

// Everything that varies between checkouts and runs, replaced by a fixed name. Longer paths first.
struct Normaliser {
    std::vector<std::pair<std::string, std::string>> paths;   // lower-cased forward-slash path -> name
    std::vector<Regex> masks;
    static const Regex& timestamp() {
        static const Regex re(R"(\d{4}-\d\d-\d\d[T ]\d\d?:\d\d(:\d\d(\.\d+)?)?Z?( ?[AP]M)?)");
        return re;
    }
    // A bare date (a generated file's header: "generated 2026-11-14 by CodeBreaker"): the day a case runs is not content.
    static const Regex& date() {
        static const Regex re(R"(\b\d{4}-\d\d-\d\d\b)");
        return re;
    }

    void add_path(const stdfs::path& p, const std::string& name) {
        std::string s = slashes(fs::to_text(p));
        while (s.size() > 1 && s.back() == '/') s.pop_back();
        if (s.empty()) return;
        paths.emplace_back(text::lower(s), name);
        std::stable_sort(paths.begin(), paths.end(), [](const auto& a, const auto& b) { return a.first.size() > b.first.size(); });
    }

    std::string apply(std::string_view in) const {
        std::string s = slashes(std::string(in));
        std::string low = text::lower(s);
        std::string out;
        out.reserve(s.size());
        std::size_t i = 0;
        while (i < s.size()) {
            bool hit = false;
            for (const auto& [p, name] : paths) {
                if (low.compare(i, p.size(), p) == 0) {
                    out += name;
                    i += p.size();
                    hit = true;
                    break;
                }
            }
            if (!hit) out += s[i++];
        }
        out = timestamp().replace_all(out, "<time>");
        out = date().replace_all(out, "<date>");
        for (const Regex& m : masks) out = m.replace_all(out, "<~>");
        // Line ends are not content: a command's output is compared with \n.
        return text::replace_all(out, "\r\n", "\n");
    }
};

bool looks_text(const Bytes& b) {
    const std::size_t n = std::min<std::size_t>(b.size(), 8192);
    for (std::size_t i = 0; i < n; ++i)
        if (b[i] == 0) return false;
    return true;
}

std::string as_string(const Bytes& b) { return std::string(reinterpret_cast<const char*>(b.data()), b.size()); }

// The text a case's result is recorded as (and compared by).
struct Outcome {
    std::string compared;   // exit, stdout, files (and stderr if asked): what a mismatch means
    std::string recorded;   // compared + stderr for reading
    bool timed_out = false;
    double seconds = 0;
};

Outcome run_case(const Env& env, const Vars& vars, const Case& c, const stdfs::path& work) {
    stdfs::path work_abs = stdfs::absolute(work);
    fs::remove_tree(work_abs);
    fs::make_dirs(work_abs / "state");
    fs::make_dirs(work_abs / "out");

    const std::string repo_s = slashes(fs::to_text(env.repo));
    const std::string work_s = slashes(fs::to_text(work_abs));
    const std::string corpus_s = slashes(fs::to_text(env.corpus));
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(env.exe));
    for (const std::string& a : c.argv) {
        std::string w = a;
        for (const auto& [name, value] : vars) w = text::replace_all(w, "{" + name + "}", value);
        w = text::replace_all(w, "{repo}", repo_s);
        w = text::replace_all(w, "{corpus}", corpus_s);
        w = text::replace_all(w, "{games}", slashes(fs::to_text(env.games)));
        w = text::replace_all(w, "{work}", work_s);
        w = text::replace_all(w, "{out}", work_s + "/out");
        spec.argv.push_back(w);
    }
    spec.cwd = c.cwd_work ? work_abs : env.repo;
    spec.env["CB_STATE"] = work_s + "/state";
    spec.env["CB_JOBS"] = "2";                  // each case is itself parallel inside; the gate holds the total down
    spec.env["PYTHONIOENCODING"] = "utf-8";
    fs::make_dirs(work_abs / "tmp");
    spec.env["TEMP"] = work_s + "/tmp";           // scratch files of the program (and the tools it starts) stay with the case:
    spec.env["TMP"] = work_s + "/tmp";            // cases side by side must not share a folder
    spec.timeout = std::chrono::seconds(c.timeout_s);
    if (!c.stdin_file.empty()) {
        std::string f = c.stdin_file;
        for (const auto& [name, value] : vars) f = text::replace_all(f, "{" + name + "}", value);
        f = text::replace_all(f, "{repo}", repo_s);
        spec.stdin_data = fs::read_file(fs::from_text(f));
    }
    const process::ProcessResult r = process::run(spec);
    // A command may have started the background service of the case's state folder (the MCP server does: it outlives the program that started it and
    // keeps the knowledge database open). It is the case's own, so it ends with the case: asked to stop, and waited for, before the folder is removed.
    {
        process::ProcessSpec stop;
        stop.argv = {fs::to_text(env.exe), "serve", "--stop"};
        stop.cwd = spec.cwd;
        stop.env = spec.env;
        stop.timeout = std::chrono::seconds(30);
        try {
            (void)process::run(stop);
        } catch (const Error&) {   // isolation boundary: nothing to stop is not a failure of the case
        }
    }

    Normaliser n;
    n.add_path(work_abs, "<work>");
    n.add_path(env.repo, "<repo>");
    n.add_path(env.exe_root, "<root>");
    n.add_path(env.corpus, "<corpus>");
    n.add_path(env.games, "<games>");
    if (const std::string home = env_var("USERPROFILE"); !home.empty()) n.add_path(home, "<home>");
    for (const std::string& m : c.masks) n.masks.emplace_back(m);

    Outcome o;
    o.timed_out = r.timed_out;
    o.seconds = std::chrono::duration<double>(r.elapsed).count();
    std::string s = "exit: " + (r.timed_out ? std::string("TIMEOUT") : std::to_string(static_cast<std::int32_t>(r.exit_code))) + "\n";
    std::string shown = n.apply(as_string(r.out));
    if (c.sort_lines || c.head > 0) {
        std::vector<std::string> ls;
        for (std::string_view l : text::lines(shown)) ls.emplace_back(l);
        if (c.sort_lines) std::sort(ls.begin(), ls.end());
        if (c.head > 0 && ls.size() > static_cast<std::size_t>(c.head)) ls.resize(static_cast<std::size_t>(c.head));
        shown.clear();
        for (const std::string& l : ls) shown += l + "\n";
    }
    s += "--- stdout\n" + shown;
    if (!s.empty() && s.back() != '\n') s += "\n";
    s += "--- files\n";
    std::vector<stdfs::path> files;
    std::error_code ec;
    for (stdfs::recursive_directory_iterator it(work_abs / "out", ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file()) files.push_back(it->path());
    std::sort(files.begin(), files.end());
    for (const stdfs::path& f : files) {
        Bytes data = fs::read_file(f);
        const std::string rel = slashes(fs::to_text(stdfs::relative(f, work_abs / "out")));
        if (looks_text(data)) {
            const std::string norm = n.apply(as_string(data));
            data.assign(norm.begin(), norm.end());
        }
        const std::string ext = text::lower(fs::to_text(f.extension()));
        const bool by_size = std::find(c.size_only.begin(), c.size_only.end(), ext) != c.size_only.end();
        s += rel + " " + (by_size ? std::string() : std::to_string(data.size())) + " " + (by_size ? std::string("(content not compared)") : crypto::to_hex(crypto::sha256(data))) + "\n";
    }
    o.compared = s;
    if (c.compare_stderr) {
        o.compared += "--- stderr\n" + n.apply(as_string(r.err));
        o.recorded = o.compared;
    } else {
        o.recorded = o.compared + "--- stderr (not compared)\n" + n.apply(as_string(r.err));
    }
    if (!o.recorded.empty() && o.recorded.back() != '\n') o.recorded += "\n";
    if (!o.compared.empty() && o.compared.back() != '\n') o.compared += "\n";
    return o;
}

// Whether the program can translate a source file of this extension to C++17 here: it is asked (`convert FILE --languages`) and the line of cpp17 must not say
// "not available" (a missing .NET SDK, compiler, python ... is the reason it gives).
bool translator_ready(const Env& env, const char* file_name) {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(env.exe), "convert", file_name, "--languages"};
    spec.cwd = env.repo;
    spec.timeout = std::chrono::seconds(60);
    try {
        const process::ProcessResult r = process::run(spec);
        for (std::string_view line : text::lines(as_string(r.out)))
            if (line.rfind("  cpp17", 0) == 0) return line.find("not available") == std::string_view::npos;
    } catch (const Error&) {   // isolation boundary: a probe that cannot run says no
    }
    return false;
}

// What the golden file holds that is compared: everything before the "not compared" stderr section.
std::string compared_part(const std::string& golden) {
    const std::size_t k = golden.find("--- stderr (not compared)");
    return k == std::string::npos ? golden : golden.substr(0, k);
}

std::string first_differences(const std::string& want, const std::string& got) {
    const auto a = text::lines(want);
    const auto b = text::lines(got);
    std::string out;
    int shown = 0;
    for (std::size_t i = 0; i < std::max(a.size(), b.size()) && shown < 6; ++i) {
        const std::string_view x = i < a.size() ? a[i] : std::string_view("<end>");
        const std::string_view y = i < b.size() ? b[i] : std::string_view("<end>");
        if (x == y) continue;
        out += "      line " + std::to_string(i + 1) + "\n        golden: " + std::string(x.substr(0, 200)) +
               "\n        got:    " + std::string(y.substr(0, 200)) + "\n";
        ++shown;
    }
    return out;
}

stdfs::path find_repo(stdfs::path from) {
    for (from = stdfs::absolute(from); !from.empty(); from = from.parent_path()) {
        if (stdfs::exists(from / "native" / "cb" / "CMakeLists.txt")) return from;
        if (from == from.root_path()) break;
    }
    return {};
}

// <root>/state/cxx/codebreaker/build/codebreaker.exe -> <root>
stdfs::path root_of_exe(const stdfs::path& exe) {
    stdfs::path p = stdfs::absolute(exe).parent_path();
    for (int i = 0; i < 4 && !p.empty(); ++i) p = p.parent_path();   // build, codebreaker, cxx, state
    return p;
}

// HEAD of the repository, read from .git (informational: the record says which commit the tree was near). Empty when it cannot be read.
std::string head_commit(const stdfs::path& repo) {
    try {
        const stdfs::path git = repo / ".git";
        if (!stdfs::is_directory(git)) return {};
        const std::string head(text::trim(fs::read_text(git / "HEAD")));
        if (!head.starts_with("ref: ")) return head.substr(0, 9);
        const std::string ref = head.substr(5);
        if (stdfs::exists(git / ref)) return std::string(text::trim(fs::read_text(git / ref))).substr(0, 9);
        for (std::string_view line : text::lines(fs::read_text(git / "packed-refs")))
            if (line.size() > 41 && line.substr(41) == ref) return std::string(line.substr(0, 9));
    } catch (const std::exception&) {   // isolation boundary: the commit is a label only
    }
    return {};
}

int usage(const char* why) {
    std::fprintf(stderr, "cb_exe_gate: %s\nusage: cb_exe_gate [--exe EXE] [--repo DIR] [--cases FILE] [--golden DIR] [--corpus DIR] [--jobs N] [--filter TEXT] [--update] [--keep] [--list]\n", why);
    return 2;
}

}  // namespace

int main(int argc, char** argv) {
    win::use_utf8_console();
    try {
        Env env;
        std::optional<stdfs::path> exe_root, exe, repo, cases, golden, corpus;
        std::string filter;
        bool list_only = false;
        std::optional<stdfs::path> unit;
        int unit_shards = 0;   // 0: twice the pool, so the shards even out
        bool jobs_given = false, changed_only = false, no_record = false;
        std::optional<stdfs::path> record_file_arg;
        for (int i = 1; i < argc; ++i) {
            const std::string a = argv[i];
            auto value = [&]() -> std::string { return i + 1 < argc ? std::string(argv[++i]) : std::string(); };
            if (a == "--exe") exe = fs::from_text(value());
            else if (a == "--exe-root") exe_root = fs::from_text(value());
            else if (a == "--repo") repo = fs::from_text(value());
            else if (a == "--cases") cases = fs::from_text(value());
            else if (a == "--golden") golden = fs::from_text(value());
            else if (a == "--corpus") corpus = fs::from_text(value());
            else if (a == "--jobs") env.jobs = std::max(1, std::stoi(value())), jobs_given = true;
            else if (a == "--shared") env.jobs = 4, jobs_given = true;
            else if (a == "--times") env.times = true;
            else if (a == "--changed") changed_only = true;
            else if (a == "--no-record") no_record = true;
            else if (a == "--record") record_file_arg = fs::from_text(value());
            else if (a == "--filter") filter = value();
            else if (a == "--update") env.update = true;
            else if (a == "--keep") env.keep = true;
            else if (a == "--list") list_only = true;
            else if (a == "--unit") unit = fs::from_text(value());
            else if (a == "--shards") unit_shards = std::max(1, std::stoi(value()));
            else return usage(("unknown option " + a).c_str());
        }
        if (!jobs_given) {
            const std::string e = env_var("CB_GATE_JOBS");
            env.jobs = !e.empty() ? std::max(1, std::stoi(e)) : default_jobs();
        }
        if (unit_shards == 0) unit_shards = std::max(8, env.jobs * 2);
        env.repo = repo ? *repo : find_repo(stdfs::current_path());
        if (env.repo.empty()) return usage("no repository found (run from inside it, or pass --repo)");
        env.repo = stdfs::absolute(env.repo).lexically_normal();
        if (!env.repo.has_filename()) env.repo = env.repo.parent_path();
        env.cases = cases ? *cases : env.repo / "tests" / "exe_gate" / "cases.txt";
        env.golden = golden ? *golden : env.repo / "tests" / "exe_gate" / "golden";
        env.scratch = env.repo / "state" / "work" / "exe_gate" / ("run-" + std::to_string(_getpid()));   // one gate at a time per process
        if (exe) env.exe = *exe;
        else {
            const stdfs::path rel = stdfs::path("state") / "cxx" / "codebreaker" / "build" / "codebreaker.exe";
            env.exe = stdfs::exists(env.repo / rel) ? env.repo / rel : stdfs::path();
            // A worktree is <main>/.claude/worktrees/<name>: the program is built in the main checkout.
            if (env.exe.empty() && stdfs::exists(env.repo.parent_path().parent_path().parent_path() / rel))
                env.exe = env.repo.parent_path().parent_path().parent_path() / rel;
        }
        if (env.exe.empty() || !stdfs::exists(env.exe)) return usage("codebreaker.exe not found (pass --exe)");
        env.exe = stdfs::absolute(env.exe);
        env.exe_root = exe_root ? stdfs::absolute(*exe_root).lexically_normal() : root_of_exe(env.exe);
        if (corpus) env.corpus = *corpus;
        else if (const std::string c = env_var("CB_CORPUS"); !c.empty()) env.corpus = c;
        else env.corpus = configured_corpus(env.repo);
        env.have_corpus = stdfs::exists(env.corpus);
        env.games = env_var("CB_GAMES").empty() ? stdfs::path("C:/Nexia360/Games") : stdfs::path(env_var("CB_GAMES"));
        env.have_games = stdfs::exists(env.games);
        env.have_toolchains = stdfs::exists(env.exe_root / "toolchains") || stdfs::exists(env.repo / "toolchains");
        env.have_translator_cs = translator_ready(env, "probe.cs");
        env.have_translator_py = translator_ready(env, "probe.py");

        Vars vars;
        std::vector<Case> all = load_cases(env.cases, vars);
        std::vector<const Case*> picked;
        for (const Case& c : all)
            if (filter.empty() || c.name.find(filter) != std::string::npos) picked.push_back(&c);
        if (list_only) {
            for (const Case* c : picked) std::printf("%s\n", c->name.c_str());
            return 0;
        }
        fs::make_dirs(env.golden);

        // The tree this run starts on, and (--changed) what changed since the last passing gate. A tree edited during the run is recorded as it was at the
        // start, so the edit counts as changed next time. See gate_changed.hpp.
        namespace gc = cb::app::gate_changed;
        const stdfs::path record_file = record_file_arg ? *record_file_arg : env.repo / "state" / "work" / "gate-last.txt";
        const bool whole_run = !env.update && filter.empty() && !list_only;
        std::optional<gc::Record> tree;
        std::vector<std::string> unit_areas_to_run;   // empty with units_all false: none
        bool units_all = true, units_none = false;
        if (whole_run && (changed_only || !no_record)) {
            std::map<std::string, std::string> extra;
            std::string vars_text;
            for (const auto& [name, value] : vars) vars_text += name + " = " + value + "\n";
            extra["case-vars"] = crypto::to_hex(crypto::sha1(Bytes(vars_text.begin(), vars_text.end())));
            for (const Case& c : all) extra["case:" + c.name] = crypto::to_hex(crypto::sha1(Bytes(c.line.begin(), c.line.end())));
            gc::Record was;
            if (stdfs::exists(record_file)) was = gc::read_record(fs::read_text(record_file));
            tree = gc::scan_tree(env.repo, &was, extra);
            if (changed_only) {
                gc::Selection sel;
                std::size_t n_changed = 0;
                if (was.files.empty()) {
                    sel.everything = true;
                    sel.why = "no record of a passing gate yet";
                } else {
                    const std::vector<std::string> paths = gc::changed_paths(was, *tree);
                    n_changed = paths.size();
                    sel = gc::select(paths);
                }
                const gc::Graph graph = gc::read_graph(env.repo);
                gc::CommandAreas map;
                if (const stdfs::path af = env.repo / "tests" / "exe_gate" / "areas.txt"; stdfs::exists(af)) map = gc::parse_areas(fs::read_text(af));
                std::vector<const Case*> keep;
                for (const Case* c : picked)
                    if (gc::case_selected(sel, graph, map, c->name, c->argv.empty() ? std::string() : c->argv[0], c->areas)) keep.push_back(c);
                std::string shown;
                if (sel.everything) {
                    std::printf("cb_exe_gate --changed: everything runs (%s)\n", sel.why.c_str());
                } else {
                    const std::set<std::string> ua = gc::unit_areas(sel, graph);
                    unit_areas_to_run.assign(ua.begin(), ua.end());
                    units_all = false;
                    units_none = ua.empty();
                    for (const std::string& a : sel.areas) shown += (shown.empty() ? "" : " ") + a;
                    std::string cases_named;
                    for (const std::string& n : sel.cases) cases_named += (cases_named.empty() ? "" : " ") + n;
                    std::printf("cb_exe_gate --changed: %zu changed paths since the last passing gate (%s); areas [%s]%s%s; unit tests of [%s]; %zu of %zu cases\n",
                                n_changed, was.commit.c_str(), shown.c_str(), cases_named.empty() ? "" : "; cases changed: ", cases_named.c_str(),
                                text::join(unit_areas_to_run, " ").c_str(), keep.size(), picked.size());
                }
                picked = std::move(keep);
            }
        }

        auto update_or_filtered = [](const Env& e, const std::string& f) { return e.update || !f.empty(); };
        enum class Verdict { Pass, Fail, Skip, Recorded, Missing };
        struct Result {
            Verdict verdict = Verdict::Skip;
            std::string detail;
            double seconds = 0;
        };
        std::vector<Result> results(picked.size());
        std::mutex print_mutex;
        const auto t0 = std::chrono::steady_clock::now();
        std::printf("cb_exe_gate: %zu cases, %d at a time, %s\n", picked.size(), env.jobs, fs::to_text(env.exe).c_str());
        std::fflush(stdout);
        parallel::ThreadPool pool(static_cast<std::size_t>(env.jobs));
        // Module-test shards go first (they are the long tasks), then the cases.
        // (--changed: the areas' tests alone, in fewer shards; none when no area's tests are affected)
        std::size_t nunit = (unit && !update_or_filtered(env, filter) && !units_none) ? static_cast<std::size_t>(unit_shards) : 0;
        if (nunit && !units_all) nunit = std::max<std::size_t>(1, nunit / 2);
        const std::string area_arg = text::join(unit_areas_to_run, ",");
        const stdfs::path test_times_file = env.repo / "state" / "work" / "gate-test-times.txt";   // seconds per test, last run: "secs<TAB>name"
        const stdfs::path case_times_file = env.repo / "state" / "work" / "gate-case-times.txt";   // seconds per case, last run: "secs<TAB>name"
        {   // the longest cases first (the pool takes them in order), so the last one started is a short one
            std::map<std::string, double> last;
            if (stdfs::exists(case_times_file))
                for (std::string_view line : text::lines(fs::read_text(case_times_file))) {
                    const std::size_t tab = line.find('\t');
                    if (tab != std::string_view::npos) last[std::string(line.substr(tab + 1))] = std::atof(std::string(line.substr(0, tab)).c_str());
                }
            auto weight = [&](const Case* c) { const auto it = last.find(c->name); return it != last.end() ? it->second : 30.0; };
            std::stable_sort(picked.begin(), picked.end(), [&](const Case* a, const Case* b) { return weight(a) > weight(b); });
        }
        struct UnitResult {
            bool ok = true;
            std::string summary, failures;
            double seconds = 0;
            std::vector<std::pair<double, std::string>> slow;   // --times: the slowest tests of the shard
        };
        std::vector<UnitResult> unit_results(nunit);
        parallel::parallel_for(pool, nunit + picked.size(), [&](std::size_t t) {
            if (t < nunit) {
                UnitResult& ur = unit_results[t];
                try {
                    process::ProcessSpec spec;
                    spec.argv = {fs::to_text(*unit), "--shard", std::to_string(t) + "/" + std::to_string(nunit)};
                    if (!units_all) spec.argv.insert(spec.argv.end(), {"--area", area_arg});
                    if (env.times) spec.argv.push_back("--times");
                    if (stdfs::exists(test_times_file)) spec.argv.insert(spec.argv.end(), {"--weights", fs::to_text(test_times_file)});   // balance the shards by last time's seconds
                    spec.argv.insert(spec.argv.end(), {"--times-file", fs::to_text(env.scratch / ("unit-" + std::to_string(t) + ".times"))});
                    spec.cwd = env.repo;
                    spec.env["CB_JOBS"] = "2";
                    const stdfs::path shard_tmp = env.scratch / ("unit-" + std::to_string(t));   // shards side by side must not share a temp folder
                    fs::make_dirs(shard_tmp);
                    spec.env["TEMP"] = fs::to_text(shard_tmp);
                    spec.env["TMP"] = fs::to_text(shard_tmp);
                    spec.timeout = std::chrono::minutes(30);
                    const process::ProcessResult r = process::run(spec);
                    ur.seconds = std::chrono::duration<double>(r.elapsed).count();
                    const std::string out = as_string(r.out);
                    bool in_fail = false;
                    for (std::string_view line : text::lines(out)) {
                        if (line.rfind("FAIL", 0) == 0) in_fail = true;
                        else if (line.rfind("PASS", 0) == 0) in_fail = false;
                        if (in_fail) ur.failures += std::string(line) + '\n';
                        if (line.find(" passed, ") != std::string_view::npos) ur.summary = std::string(line);
                        if (line.rfind("SLOW  ", 0) == 0) {   // "SLOW  1.23  name"
                            const std::vector<std::string_view> w = text::split_ws(line);
                            if (w.size() >= 3) ur.slow.emplace_back(std::atof(std::string(w[1]).c_str()), "test " + std::string(w[2]));
                        }
                    }
                    ur.ok = r.ok();
                    if (!ur.ok && ur.failures.empty()) ur.failures = r.timed_out ? std::string("timed out\n") : "exit " + std::to_string(r.exit_code) + "\n" + out.substr(out.size() > 2000 ? out.size() - 2000 : 0);
                } catch (const std::exception& e) {
                    ur.ok = false;
                    ur.failures = std::string("could not run cb_tests: ") + e.what() + '\n';
                }
                std::lock_guard lock(print_mutex);
                if (!ur.ok) std::printf("FAIL  cb_tests shard %zu/%zu\n%s", t, nunit, ur.failures.c_str());
                std::fflush(stdout);
                return;
            }
            const std::size_t i = t - nunit;
            const Case& c = *picked[i];
            Result& res = results[i];
            if ((c.needs_corpus && !env.have_corpus) || (c.needs_games && !env.have_games) || (c.needs_toolchains && !env.have_toolchains) ||
                (c.needs_translator_cs && !env.have_translator_cs) || (c.needs_translator_py && !env.have_translator_py)) {
                res.verdict = Verdict::Skip;
                res.detail = (c.needs_translator_cs && !env.have_translator_cs) || (c.needs_translator_py && !env.have_translator_py)
                                 ? std::string("needs the translator's tools (the .NET SDK or python, and a C++ compiler): `codebreaker convert FILE --languages` says which is missing")
                             : c.needs_games && !env.have_games       ? "needs the games folder (" + fs::to_text(env.games) + ")"
                             : c.needs_toolchains && !env.have_toolchains ? std::string("needs this machine's toolchains (<checkout>/toolchains)")
                                                                          : "needs the corpus (" + fs::to_text(env.corpus) + ")";
            } else {
                try {
                    const stdfs::path work = env.scratch / c.name;
                    Outcome o = run_case(env, vars, c, work);
                    res.seconds = o.seconds;
                    const stdfs::path gfile = env.golden / (c.name + ".txt");
                    if (env.update) {
                        fs::write_text(gfile, o.recorded);
                        res.verdict = Verdict::Recorded;
                    } else if (!stdfs::exists(gfile)) {
                        res.verdict = Verdict::Missing;
                        res.detail = "no golden (run with --update)";
                    } else {
                        const std::string want = compared_part(text::replace_all(fs::read_text(gfile), "\r\n", "\n"));
                        if (want == o.compared) res.verdict = Verdict::Pass;
                        else {
                            res.verdict = Verdict::Fail;
                            res.detail = o.timed_out ? "timed out\n" : first_differences(want, o.compared);
                        }
                    }
                    if (!env.keep) fs::remove_tree(work);
                } catch (const std::exception& e) {
                    res.verdict = Verdict::Fail;
                    res.detail = std::string("could not run: ") + e.what() + "\n";
                }
            }
            std::lock_guard lock(print_mutex);
            const char* tag = res.verdict == Verdict::Pass ? "PASS" : res.verdict == Verdict::Fail ? "FAIL" : res.verdict == Verdict::Skip ? "SKIP" : res.verdict == Verdict::Recorded ? "REC " : "MISS";
            if (res.verdict != Verdict::Pass) {
                std::printf("%s  %s (%.1fs)", tag, c.name.c_str(), res.seconds);
                std::printf("\n%s", res.detail.c_str());
                if (!res.detail.empty() && res.detail.back() != '\n') std::printf("\n");
            }
            std::fflush(stdout);
        });
        std::size_t pass = 0, fail = 0, skip = 0, rec = 0;
        for (const Result& r : results) {
            if (r.verdict == Verdict::Pass) ++pass;
            else if (r.verdict == Verdict::Skip) ++skip;
            else if (r.verdict == Verdict::Recorded) ++rec;
            else ++fail;
        }
        const double total = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
        std::size_t unit_bad = 0, unit_pass = 0, unit_fail = 0;
        for (const UnitResult& u : unit_results) {
            if (!u.ok) ++unit_bad;
            // "123 passed, 0 failed (456 checks)"
            const std::vector<std::string_view> w = text::split_ws(u.summary);
            if (w.size() >= 4) {
                unit_pass += text::parse_int<std::size_t>(w[0]).value_or(0);
                unit_fail += text::parse_int<std::size_t>(w[2]).value_or(0);
            }
        }
        if (nunit) std::printf("\ncb_tests: %zu passed, %zu failed in %zu shards\n", unit_pass, unit_fail, nunit);
        if (env.times) {
            std::vector<std::pair<double, std::string>> rows;
            double case_sum = 0, shard_sum = 0;
            for (std::size_t i = 0; i < picked.size(); ++i) {
                if (results[i].verdict == Verdict::Skip) continue;
                rows.emplace_back(results[i].seconds, picked[i]->name);
                case_sum += results[i].seconds;
            }
            for (std::size_t t = 0; t < nunit; ++t) {
                rows.emplace_back(unit_results[t].seconds, "cb_tests shard " + std::to_string(t) + "/" + std::to_string(nunit));
                shard_sum += unit_results[t].seconds;
                rows.insert(rows.end(), unit_results[t].slow.begin(), unit_results[t].slow.end());
            }
            std::sort(rows.begin(), rows.end(), [](const auto& a, const auto& b) { return a.first > b.first; });
            std::printf("\nslowest (seconds; %d at a time; cases %.0f s and shards %.0f s of work in %.0f s of wall):\n", env.jobs, case_sum, shard_sum, total);
            for (std::size_t k = 0; k < rows.size() && k < 25; ++k) std::printf("  %7.1f  %s\n", rows[k].first, rows[k].second.c_str());
        }
        if (!env.keep) fs::remove_tree(env.scratch);
        if (env.update) std::printf("\n%zu goldens recorded (%.1f s)\n", rec, total);
        else std::printf("%zu passed, %zu failed, %zu skipped of %zu cases (%.1f s)\n", pass, fail, skip, picked.size(), total);
        if (!env.update) {   // what this run learned of how long things take, for the next run's ordering and shards
            try {
                auto merge = [&](const stdfs::path& file, const std::vector<std::pair<double, std::string>>& now) {
                    std::map<std::string, double> all_times;
                    if (stdfs::exists(file))
                        for (std::string_view line : text::lines(fs::read_text(file))) {
                            const std::size_t tab = line.find('\t');
                            if (tab != std::string_view::npos) all_times[std::string(line.substr(tab + 1))] = std::atof(std::string(line.substr(0, tab)).c_str());
                        }
                    for (const auto& [secs, name] : now) all_times[name] = secs;
                    std::string out;
                    for (const auto& [name, secs] : all_times) out += std::format("{:.3f}\t{}\n", secs, name);
                    fs::write_text(file, out);
                };
                std::vector<std::pair<double, std::string>> case_now, test_now;
                for (std::size_t i = 0; i < picked.size(); ++i)
                    if (results[i].verdict == Verdict::Pass) case_now.emplace_back(results[i].seconds, picked[i]->name);
                for (std::size_t t = 0; t < nunit; ++t) {
                    const stdfs::path f = env.scratch / ("unit-" + std::to_string(t) + ".times");
                    if (!stdfs::exists(f)) continue;
                    for (std::string_view line : text::lines(fs::read_text(f))) {
                        const std::size_t tab = line.find('\t');
                        if (tab != std::string_view::npos) test_now.emplace_back(std::atof(std::string(line.substr(0, tab)).c_str()), std::string(line.substr(tab + 1)));
                    }
                }
                if (!case_now.empty()) merge(case_times_file, case_now);
                if (!test_now.empty()) merge(test_times_file, test_now);
            } catch (const std::exception&) {   // isolation boundary: the timings are an optimisation
            }
        }
        const bool passed_all = fail == 0 && unit_bad == 0;
        // A passing whole run (every case it selected, and the unit tests) is the new base of --changed. Not when the run was told not to record (a
        // gate on an exe that was not just built from this tree), not a filtered or recording run, not one without its unit tests.
        if (passed_all && tree && !no_record && unit) {
            try {
                fs::make_dirs(record_file.parent_path());
                tree->commit = head_commit(env.repo);
                fs::write_text(record_file, gc::record_text(*tree));
            } catch (const std::exception& e) {
                std::fprintf(stderr, "cb_exe_gate: the record of this gate could not be written (%s)\n", e.what());
            }
        }
        return passed_all ? 0 : 1;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_exe_gate: %s\n", e.what());
        return 2;
    }
}

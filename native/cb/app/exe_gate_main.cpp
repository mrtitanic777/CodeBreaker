// cb_exe_gate -- runs the real codebreaker.exe on a list of command cases and compares what it prints and the files it
// writes with goldens. It is the command-level half of the gate (tools\gate.cmd): cb_tests checks the modules, this
// checks that every command of the program, as a user types it, still does what it did.
//
//     cb_exe_gate [--exe EXE] [--repo DIR] [--cases FILE] [--golden DIR] [--corpus DIR]
//                 [--jobs N] [--filter TEXT] [--update] [--keep] [--list] [--unit CB_TESTS_EXE [--shards N]]
//
//   --exe-root the checkout the program belongs to, when --exe is a wrapper (its paths are printed as <root>)
//   --exe      the program (default: <repo>/state/cxx/codebreaker/build/codebreaker.exe, else the main checkout's)
//   --repo     where inputs and cases live (default: the folder above native/, found from the current directory)
//   --cases    default <repo>/testdata/exe_gate/cases.txt        --golden  default <repo>/testdata/exe_gate/golden
//   --corpus   the Win95 corpus files (default: the CB_CORPUS environment variable, else ../Windows95/design/corpus/files);
//              cases that need it are skipped, and counted as skipped, when it is not there
//   --jobs     cases run side by side (default 4, the project's cap)
//   --update   record the goldens of the selected cases from the program's current behaviour (review the diff!)
//   --unit     also run the module tests (cb_tests) as N shards (default 8) in the same pool, so the whole gate is one run
//   --keep     leave each case's scratch folder (state/work/exe_gate/run-PID) for inspection
//
// A case file line is `name: word word "a quoted word" @option ...`. Words may use {repo} (the inputs), {corpus},
// {work} (the case's own scratch folder; CB_STATE is {work}/state, so no case touches the real state), and {out}
// ({work}/out: every file the case leaves there is compared). Options:
//     @needs=corpus|games  skip when the corpus / the Xbox 360 games folder (CB_GAMES, C:/Nexia360/Games) is absent
//              @timeout=SECONDS   (default 120)
//     @stdin=FILE        feed this file (a {repo} path) to stdin  @mask=REGEX       replace matches by <~> before comparing
//     @sizeonly=.obj,.map  output files with these extensions are compared by name only (a build time and the source path are inside them)
//     @sort              compare stdout as sorted lines     @head=N   compare only its first N lines
//     @stderr            compare stderr too (otherwise it is kept in the golden for reading, not compared)
//     @cwd=work          run in {work} (default: the repo)
// What is compared: exit code, stdout, the files under {out} (text with paths normalised, else bytes), and stderr if
// asked. Absolute paths (the repo, the program's checkout, the corpus, the scratch folder, the home folder) and
// ISO timestamps are replaced by fixed names so a golden is the same on any checkout.
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <process.h>
#include <filesystem>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/parallel.hpp"
#include "cb/core/process.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/core/win.hpp"

namespace {

namespace stdfs = std::filesystem;
using namespace cb;

using Vars = std::vector<std::pair<std::string, std::string>>;   // `name = value` lines of the cases file: {name} in words

struct Case {
    std::string name;
    std::vector<std::string> argv;
    bool needs_corpus = false;
    bool needs_games = false;
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
    bool have_corpus = false, have_games = false;
    int jobs = 4;
    bool update = false, keep = false;
};

std::string env_var(const char* name) {
    char* value = nullptr;
    std::size_t n = 0;
    std::string out;
    if (_dupenv_s(&value, &n, name) == 0 && value) out = value;
    std::free(value);
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
        for (std::string& w : words_of(line.substr(colon + 1))) {
            if (w.rfind("@needs=", 0) == 0) {
                if (w.substr(7) == "corpus") c.needs_corpus = true;
                else if (w.substr(7) == "games") c.needs_games = true;
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
        static const Regex re(R"(\d{4}-\d\d-\d\d[T ]\d\d:\d\d(:\d\d(\.\d+)?)?Z?)");
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
        int unit_shards = 8;
        for (int i = 1; i < argc; ++i) {
            const std::string a = argv[i];
            auto value = [&]() -> std::string { return i + 1 < argc ? std::string(argv[++i]) : std::string(); };
            if (a == "--exe") exe = fs::from_text(value());
            else if (a == "--exe-root") exe_root = fs::from_text(value());
            else if (a == "--repo") repo = fs::from_text(value());
            else if (a == "--cases") cases = fs::from_text(value());
            else if (a == "--golden") golden = fs::from_text(value());
            else if (a == "--corpus") corpus = fs::from_text(value());
            else if (a == "--jobs") env.jobs = std::max(1, std::stoi(value()));
            else if (a == "--filter") filter = value();
            else if (a == "--update") env.update = true;
            else if (a == "--keep") env.keep = true;
            else if (a == "--list") list_only = true;
            else if (a == "--unit") unit = fs::from_text(value());
            else if (a == "--shards") unit_shards = std::max(1, std::stoi(value()));
            else return usage(("unknown option " + a).c_str());
        }
        env.repo = repo ? *repo : find_repo(stdfs::current_path());
        if (env.repo.empty()) return usage("no repository found (run from inside it, or pass --repo)");
        env.repo = stdfs::absolute(env.repo).lexically_normal();
        if (!env.repo.has_filename()) env.repo = env.repo.parent_path();
        env.cases = cases ? *cases : env.repo / "testdata" / "exe_gate" / "cases.txt";
        env.golden = golden ? *golden : env.repo / "testdata" / "exe_gate" / "golden";
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
        else env.corpus = env.repo.parent_path() / "Windows95" / "design" / "corpus" / "files";
        if (!corpus && env_var("CB_CORPUS").empty() && !stdfs::exists(env.corpus))   // from a worktree: beside the main checkout
            env.corpus = env.repo.parent_path().parent_path().parent_path().parent_path() / "Windows95" / "design" / "corpus" / "files";
        env.have_corpus = stdfs::exists(env.corpus);
        env.games = env_var("CB_GAMES").empty() ? stdfs::path("C:/Nexia360/Games") : stdfs::path(env_var("CB_GAMES"));
        env.have_games = stdfs::exists(env.games);

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
        const std::size_t nunit = (unit && !update_or_filtered(env, filter)) ? static_cast<std::size_t>(unit_shards) : 0;
        struct UnitResult {
            bool ok = true;
            std::string summary, failures;
            double seconds = 0;
        };
        std::vector<UnitResult> unit_results(nunit);
        parallel::parallel_for(pool, nunit + picked.size(), [&](std::size_t t) {
            if (t < nunit) {
                UnitResult& ur = unit_results[t];
                try {
                    process::ProcessSpec spec;
                    spec.argv = {fs::to_text(*unit), "--shard", std::to_string(t) + "/" + std::to_string(nunit)};
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
            if ((c.needs_corpus && !env.have_corpus) || (c.needs_games && !env.have_games)) {
                res.verdict = Verdict::Skip;
                res.detail = c.needs_games && !env.have_games ? "needs the games folder (" + fs::to_text(env.games) + ")" : "needs the corpus (" + fs::to_text(env.corpus) + ")";
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
        if (!env.keep) fs::remove_tree(env.scratch);
        if (env.update) std::printf("\n%zu goldens recorded (%.1f s)\n", rec, total);
        else std::printf("%zu passed, %zu failed, %zu skipped of %zu cases (%.1f s)\n", pass, fail, skip, picked.size(), total);
        return (fail == 0 && unit_bad == 0) ? 0 : 1;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "cb_exe_gate: %s\n", e.what());
        return 2;
    }
}

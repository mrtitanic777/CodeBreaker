// cb/translate/javajudge.cpp -- the Java corpus judged on each back end, beside the JVM (see javajudge.hpp).
#include "cb/translate/javajudge.hpp"

#include <algorithm>
#include <atomic>
#include <format>
#include <map>
#include <mutex>
#include <set>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::javajudge {
namespace {

namespace stdfs = std::filesystem;

constexpr std::size_t kDetailBytes = 400;        // how much of a lane's detail a judgement keeps
constexpr std::size_t kJudgeErrorBytes = 300;    // ... of a judge's own failure
constexpr std::size_t kCompileErrorBytes = 300;  // ... of javac's or the translation's
constexpr std::size_t kMainTailBytes = 1500;     // ... of a program's output when its main differs
constexpr std::size_t kMainErrorBytes = 500;
constexpr std::chrono::seconds kRunTimeout{120};
constexpr std::chrono::seconds kJavacTimeout{300};

std::string head(std::string_view s, std::size_t n) { return std::string(s.substr(0, n)); }
std::string tail(std::string_view s, std::size_t n) { return std::string(s.size() > n ? s.substr(s.size() - n) : s); }

// A finished child: its exit code (-999 for one that timed out, as the caller's reports have always said) and what it wrote,
// as text.
struct Ran {
    std::int64_t code = 0;
    std::string out, err;
};

constexpr std::int64_t kTimedOut = -999;

Ran run_tool(std::vector<std::string> argv, std::chrono::seconds timeout, std::map<std::string, std::string> env = {}) {
    process::ProcessSpec spec;
    spec.argv = std::move(argv);
    spec.env = std::move(env);
    spec.timeout = timeout;
    const process::ProcessResult r = process::run(spec);
    if (r.timed_out) return {kTimedOut, "", "timed out"};
    return {static_cast<std::int64_t>(r.exit_code), text::sanitize_utf8(as_text(r.out)), text::sanitize_utf8(as_text(r.err))};
}

// Harness formats, by the Java type of a result.
std::string_view format_of(std::string_view java_type) {
    if (java_type == "int" || java_type == "long" || java_type == "short" || java_type == "byte") return "fi";
    if (java_type == "char") return "fc";
    if (java_type == "boolean") return "fb";
    if (java_type == "float") return "ff";
    if (java_type == "double") return "fd";
    return "fs";   // String
}

// The helpers the harness is written with: each result as text that compares alike across the two runtimes.
constexpr std::string_view kHelpers = R"HLP(
  static void out(String k, String v) { System.out.println(k + "|" + v); }
  static String fi(long v) { return Long.toString(v); }
  static String fc(char v) { return Integer.toString((int) v); }
  static String fb(boolean v) { return v ? "1" : "0"; }
  static String ff(float v) { return Integer.toHexString(Float.floatToRawIntBits(v)); }
  static String fd(double v) { return Long.toHexString(Double.doubleToRawLongBits(v)); }
  static String fs(String v) {
    if (v == null) return "null";
    StringBuilder b = new StringBuilder();
    for (int i = 0; i < v.length(); ++i) { int c = v.charAt(i); b.append(c < 256 ? String.format("%02x", c) : "u" + Integer.toHexString(c)); }
    return b.toString();
  }
)HLP";

bool has_mod(const std::vector<std::string>& mods, std::string_view mod) { return std::ranges::find(mods, mod) != mods.end(); }

std::string name_or_empty(const std::optional<std::string>& name) { return name.value_or(""); }

// The text of a Unit's package.
std::string package_text(const ir::Node* unit) {
    const auto* u = ir::get<ir::Unit>(unit);
    return u != nullptr ? u->package.value_or("") : std::string();
}

}  // namespace

std::string_view word(Verdict verdict) {
    switch (verdict) {
        case Verdict::Equivalent: return "equivalent";
        case Verdict::Divergent: return "divergent";
        case Verdict::Refused: return "refused";
        case Verdict::NotRun: return "not-run";
    }
    return "not-run";
}

Verdict verdict_of(LaneVerdict lane) {
    switch (lane) {
        case LaneVerdict::NoCompile: return Verdict::Refused;
        case LaneVerdict::Divergent: return Verdict::Divergent;
        case LaneVerdict::Good: return Verdict::Equivalent;
        case LaneVerdict::Other: break;
    }
    return Verdict::NotRun;
}

std::string_view backend_name(Backend backend) { return backend == Backend::Cpp03 ? "cpp03" : "cpp17"; }

// ---- through the transpiler -----------------------------------------------------------------------------------------

OrderedMap<std::string, ClassJudgement> judge_tree(TreeLane& lane, const stdfs::path& src, const stdfs::path& work, Backend backend,
                                                   const TreeOptions& options) {
    const std::string tag = std::string(backend_name(backend));
    const auto log = [&](const std::string& line) {
        if (options.log) options.log(line);
    };
    const std::vector<std::string> made = lane.prepare(src, work, backend);
    log(std::format("[{}] converted: {} classes", tag, made.size()));

    std::vector<std::string> names;
    for (const std::string& n : made) {
        if (!options.only || std::ranges::find(*options.only, n) != options.only->end()) names.push_back(n);
    }

    std::vector<ClassJudgement> judged(names.size());
    std::atomic<std::size_t> next = 0;
    std::mutex lock;   // the log's, and the lane's own bookkeeping that is not thread safe
    const auto worker = [&] {
        for (;;) {
            const std::size_t i = next++;
            if (i >= names.size()) return;
            const std::string& n = names[i];
            LaneResult result;
            // An isolation boundary: a judge that fails says so, and is not taken for alike.
            try {
                result = lane.judge(n);
            } catch (const std::exception& e) {
                result.verdict = LaneVerdict::Other;
                result.reason = "judge-error";
                result.detail = head(e.what(), kJudgeErrorBytes);
            }
            ClassJudgement& out = judged[i];
            out.gaps = lane.gaps(n);
            out.verdict = verdict_of(result.verdict);
            out.detail = result.reason + ": " + head(result.detail, kDetailBytes);
            std::lock_guard hold(lock);
            log(std::format("[{}] {}: {} ({} gaps)", tag, n, word(out.verdict), out.gaps));
        }
    };
    {
        std::vector<std::jthread> threads;
        for (unsigned i = 0; i < std::max(1u, options.workers); ++i) threads.emplace_back(worker);
    }   // (joined here)

    OrderedMap<std::string, ClassJudgement> out;
    for (std::size_t i = 0; i < names.size(); ++i) out.insert_or_assign(names[i], std::move(judged[i]));
    return out;
}

// ---- whole programs from L2 -----------------------------------------------------------------------------------------

std::optional<std::string_view> java_name(const ir::TypeP& type) {
    if (!type) return std::nullopt;
    if (const auto* p = std::get_if<ir::PrimType>(&type->body)) {
        switch (p->prim) {
            case ir::Prim::I32: return "int";
            case ir::Prim::I64: return "long";
            case ir::Prim::I16: return "short";
            case ir::Prim::I8: return "byte";
            case ir::Prim::U16: return "char";
            case ir::Prim::Bool: return "boolean";
            case ir::Prim::F32: return "float";
            case ir::Prim::F64: return "double";
            default: return std::nullopt;
        }
    }
    if (const auto* c = std::get_if<ir::ClassType>(&type->body); c != nullptr && c->name == "java.lang.String") return "String";
    return std::nullopt;
}

std::string package_of(std::span<ir::Node* const> units) {
    std::set<std::string> packages;
    for (const ir::Node* u : units) packages.insert(package_text(u));
    return packages.size() == 1 ? *packages.begin() : std::string();
}

namespace {

void visit_class(const ir::Node* class_node, const std::string& ref, bool ok, std::vector<ProbeMethod>& out) {
    const auto* c = ir::get<ir::ClassDecl>(class_node);
    if (c == nullptr) return;
    if (!c->typeparams.empty() || c->kind == "interface" || c->kind == "annotation") ok = false;
    for (const ir::Node* m : c->members) {
        if (const auto* nested = ir::get<ir::ClassDecl>(m)) {
            visit_class(m, ref + "." + name_or_empty(nested->name), ok && !has_mod(nested->mods, "private"), out);
            continue;
        }
        const auto* method = ir::get<ir::MethodDecl>(m);
        if (!ok || method == nullptr || method->kind != "method" || !method->sym || !method->sym->is_static ||
            has_mod(method->mods, "private") || method->name == "main" || !method->typeparams.empty())
            continue;
        ProbeMethod probe;
        probe.owner = ref;
        probe.name = name_or_empty(method->name);
        bool all = true;
        for (const ir::Node* p : method->params) {
            const auto* param = ir::get<ir::Param>(p);
            const auto jname = param != nullptr ? java_name(param->type) : std::nullopt;
            if (!jname) all = false;
            else probe.params.emplace_back(*jname);
        }
        const auto result = java_name(method->ret);
        if (result && all) {
            probe.result = std::string(*result);
            out.push_back(std::move(probe));
        }
    }
}

}  // namespace

std::vector<ProbeMethod> probe_methods(std::span<ir::Node* const> units) {
    std::vector<ProbeMethod> out;
    const std::string package = package_of(units);
    for (const ir::Node* u : units) {
        if (package_text(u) != package) continue;
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        for (const ir::Node* t : unit->types) {
            const auto* c = ir::get<ir::ClassDecl>(t);
            if (c != nullptr && c->name != kHarness) visit_class(t, name_or_empty(c->name), true, out);
        }
    }
    return out;
}

std::vector<std::string> main_classes(std::span<ir::Node* const> units) {
    std::vector<std::string> out;
    for (const ir::Node* u : units) {
        const auto* unit = ir::get<ir::Unit>(u);
        if (unit == nullptr) continue;
        for (const ir::Node* t : unit->types) {
            const auto* c = ir::get<ir::ClassDecl>(t);
            if (c == nullptr) continue;
            const bool has_main = std::ranges::any_of(c->members, [](const ir::Node* m) {
                const auto* method = ir::get<ir::MethodDecl>(m);
                return method != nullptr && method->name == "main" && method->sym && method->sym->is_static;
            });
            if (has_main) out.push_back(name_or_empty(c->name));
        }
    }
    return out;
}

Harness harness(std::span<const ProbeMethod> methods, std::string_view package, const ProbeInputs& probe) {
    std::string source;
    if (!package.empty()) source += std::format("package {};\n", package);
    source += std::format("public class {} {{\n", kHarness);
    source += kHelpers;
    source += "\n  public static void main(String[] a) {\n";
    Harness out;
    for (std::size_t mi = 0; mi < methods.size(); ++mi) {
        const ProbeMethod& m = methods[mi];
        const std::vector<std::vector<std::string>> cases = probe.cases(m.params);
        for (std::size_t ci = 0; ci < cases.size(); ++ci) {
            const std::vector<std::string>& values = cases[ci];
            const std::string key = std::format("{}.{}", mi, ci);
            std::vector<std::string> literals;
            for (std::size_t k = 0; k < std::min(m.params.size(), values.size()); ++k) literals.push_back(probe.literal(m.params[k], values[k]));
            source += std::format("    try {{ out(\"{0}\", {1}({2}.{3}({4}))); }} catch (Throwable t) {{ out(\"{0}\", \"throw\"); }}\n", key,
                                  format_of(m.result), m.owner, m.name, text::join(literals, ", "));
            out.cases.insert_or_assign(key, ProbeCase{m.owner + "." + m.name, values, m.result});
        }
    }
    source += "  }\n}\n";
    out.source = std::move(source);
    return out;
}

void stage_sources(const stdfs::path& src, const stdfs::path& dest) {
    std::error_code ec;
    stdfs::remove_all(dest, ec);
    fs::make_dirs(dest);
    std::vector<stdfs::path> found;
    for (stdfs::recursive_directory_iterator it(src, ec), end; !ec && it != end; it.increment(ec)) {
        if (it->is_regular_file() && it->path().extension() == ".java") found.push_back(it->path());
    }
    std::ranges::sort(found);   // (two files of one name: the later path wins, the same every time)
    for (const stdfs::path& f : found) stdfs::copy_file(f, dest / f.filename(), stdfs::copy_options::overwrite_existing);
}

namespace {

std::string without_cr(std::string s) { return text::replace_all(s, "\r\n", "\n"); }

// A program's path as Windows wants it on a command line (a batch file does not take forward slashes).
std::string windows_path(const stdfs::path& p) { return text::replace_all(fs::to_text(p), "/", "\\"); }

}  // namespace

ProgramVerdict judge_program(const ProgramHost& host, const stdfs::path& src, const stdfs::path& work, std::string_view backend) {
    ProgramVerdict res;
    res.backend = std::string(backend);
    if (!host.jdk) {
        res.why = "no JDK (settings.json: java)";
        return res;
    }
    const std::string javac = windows_path(host.jdk->javac), java = windows_path(host.jdk->jvm);

    const stdfs::path stage = work / "src";
    stage_sources(src, stage);
    Arena arena;
    const std::vector<ir::Node*> lowered = host.lower(arena, stage, work / "probe-l2");
    const std::vector<ProbeMethod> methods = probe_methods(lowered);
    const std::string package = package_of(lowered);
    const Harness probe = harness(methods, package, host.probe);
    fs::write_text(stage / (std::string(kHarness) + ".java"), probe.source);
    const std::vector<std::string> mains = main_classes(lowered);
    const std::string qual = package.empty() ? std::string() : package + ".";

    // javac, on the program and the harness
    const stdfs::path classes = work / "classes";
    fs::make_dirs(classes);
    {
        std::vector<stdfs::path> files;
        for (const auto& entry : stdfs::directory_iterator(stage)) {
            if (entry.path().extension() == ".java") files.push_back(entry.path());
        }
        std::ranges::sort(files);
        std::vector<std::string> argv = {javac, "-nowarn", "-encoding", "UTF-8", "-d", fs::to_text(classes)};
        for (const stdfs::path& f : files) argv.push_back(fs::to_text(f));
        const Ran compiled = run_tool(std::move(argv), kJavacTimeout);
        if (compiled.code != 0) {
            res.why = "javac: " + head(compiled.err, kCompileErrorBytes);
            return res;
        }
    }

    // the original, twice: a result that changes between two runs of Java itself says nothing
    const auto jvm = [&](const std::string& cls) { return run_tool({java, "-cp", fs::to_text(classes), qual + cls}, kRunTimeout); };
    const OrderedMap<std::string, std::string> first = host.probe.results(jvm(std::string(kHarness)).out);
    const OrderedMap<std::string, std::string> second = host.probe.results(jvm(std::string(kHarness)).out);
    OrderedMap<std::string, std::string> jres;
    for (const auto& [k, v] : first) {
        const std::string* again = second.find(k);
        if (again != nullptr && *again == v && v != "throw") jres.insert_or_assign(k, v);
    }
    OrderedMap<std::string, Ran> jmain;   // (a class named twice: its last run)
    for (const std::string& m : mains) jmain.insert_or_assign(m, jvm(m));

    // the translation
    std::optional<stdfs::path> exe;
    std::string why;
    Arena translated;
    try {
        const std::vector<ir::Node*> units = host.lower(translated, stage, work / "l2");
        Built built = host.build(units, work, backend);
        exe = std::move(built.exe);
        why = std::move(built.why);
    } catch (const Error& e) {   // a decline, or a front end failure: the program is refused, not the judge broken
        exe = std::nullopt;
        why = head(e.what(), kCompileErrorBytes);
    }
    if (!exe) {
        res.verdict = Verdict::Refused;
        res.why = why;
        return res;
    }

    const auto native = [&](const std::string& main_class) {
        return run_tool({windows_path(*exe)}, kRunTimeout, {{"JRT_MAIN", main_class}});
    };
    const OrderedMap<std::string, std::string> cres = host.probe.results(native(std::string(kHarness)).out);
    std::set<std::string> judged;
    for (const auto& [k, v] : jres) {
        const ProbeCase* c = probe.cases.find(k);
        if (c == nullptr) continue;
        ++res.cases;
        judged.insert(c->method);
        const std::string* got = cres.find(k);
        if (got == nullptr) res.differs.push_back({c->method, c->values, v, "no result (crashed?)"});
        else if (!host.probe.same(c->result_type, v, *got)) res.differs.push_back({c->method, c->values, v, *got});
    }
    res.methods = judged.size();

    bool main_ok = true;
    res.main.emplace();
    for (const auto& [m, jran] : jmain) {
        const Ran ran = native(m);
        const bool alike = jran.code == ran.code && without_cr(jran.out) == without_cr(ran.out);
        res.main->insert_or_assign(m, alike ? std::string("alike")
                                            : std::format("differs: exit {}/{}\n--- java\n{}\n--- native\n{}\n{}", jran.code, ran.code,
                                                          tail(jran.out, kMainTailBytes), tail(ran.out, kMainTailBytes), tail(ran.err, kMainErrorBytes)));
        main_ok = main_ok && alike;
    }
    if (!res.differs.empty() || !main_ok) res.verdict = Verdict::Divergent;
    else if (res.cases > 0 || !res.main->empty()) res.verdict = Verdict::Equivalent;
    else res.why = "nothing it could run and compare";
    return res;
}

}  // namespace cb::translate::javajudge

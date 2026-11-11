// codebreaker/translate/javajudge.cpp -- typed C++ from codebreaker.translate.javajudge. See javajudge.hpp.

#include "codebreaker/translate/javajudge.hpp"

#include <cstdio>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>

#include "codebreaker/java/probe.hpp"
#include "codebreaker/translate/backends/c_java.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::javajudge {

namespace probe = codebreaker::java::probe;
namespace lane = codebreaker::verify::lanes::java;
namespace V = codebreaker::verify::verdicts;
namespace cpp17 = codebreaker::translate::backends::cpp17;
namespace c_java = codebreaker::translate::backends::c_java;
namespace jlower = codebreaker::translate::frontends::java::lower;
using py::str;

namespace {

str S(const char* s) { return str(s); }

JavaFrontend& fe_() {
    static JavaFrontend f;
    return f;
}
const JavaFrontend& fe() {
    const JavaFrontend& f = fe_();
    if (!f.dump || !f.diagnostics) throw py::ImportError("codebreaker.translate.frontends.java: no front end given (set_java_frontend)");
    return f;
}

// the environment this process has, with `extra` over it, as a CreateProcess block (UCS-2, sorted, double-NUL ended)
std::wstring env_block(const std::map<std::string, std::string>& extra) {
    std::map<std::wstring, std::pair<std::wstring, std::wstring>> env;   // UPPER key -> (key, value)
    auto upper = [](std::wstring s) {
        for (wchar_t& c : s) c = static_cast<wchar_t>(towupper(c));
        return s;
    };
    LPWCH cur = GetEnvironmentStringsW();
    for (LPWCH p = cur; p && *p;) {
        std::wstring item(p);
        p += item.size() + 1;
        size_t eq = item.find(L'=', 1);   // (a name may start with '=': drive-current-directory entries)
        if (eq == std::wstring::npos) continue;
        env[upper(item.substr(0, eq))] = {item.substr(0, eq), item.substr(eq + 1)};
    }
    if (cur) FreeEnvironmentStringsW(cur);
    for (const auto& kv : extra) {
        std::wstring k = py::subprocess::to_wide(str(kv.first)), v = py::subprocess::to_wide(str(kv.second));
        env[upper(k)] = {k, v};
    }
    std::wstring out;
    for (const auto& kv : env) {
        out += kv.second.first + L"=" + kv.second.second;
        out.push_back(L'\0');
    }
    out.push_back(L'\0');
    return out;
}

struct Ran {
    int64_t code = 0;
    std::string out, err;
    bool timed_out = false;
};

// subprocess.run(cmd, stdout=PIPE, stderr=PIPE, cwd=cwd, timeout=timeout, env=env): the child has this process's stdin
Ran spawn(const py::list<str>& cmd, const std::optional<str>& cwd, int64_t timeout_s, const std::map<std::string, std::string>& env) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr, in_r = nullptr;
    std::vector<HANDLE> child_ends;
    auto fail = [&](DWORD code) {
        for (HANDLE h : {out_r, err_r})
            if (h) CloseHandle(h);
        for (HANDLE h : child_ends) CloseHandle(h);
        py::raise_winerror(code);
    };
    HANDLE cur = GetStdHandle(STD_INPUT_HANDLE);
    if (cur && cur != INVALID_HANDLE_VALUE && DuplicateHandle(GetCurrentProcess(), cur, GetCurrentProcess(), &in_r, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        child_ends.push_back(in_r);
    } else {
        in_r = nullptr;
    }
    if (!CreatePipe(&out_r, &out_w, &sa, 0)) fail(GetLastError());
    child_ends.push_back(out_w);
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    if (!CreatePipe(&err_r, &err_w, &sa, 0)) fail(GetLastError());
    child_ends.push_back(err_w);
    SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = err_w;
    PROCESS_INFORMATION pi{};
    std::wstring line = py::subprocess::to_wide(py::subprocess::list2cmdline(cmd));
    std::wstring wcwd = cwd ? py::subprocess::to_wide(*cwd) : std::wstring();
    std::wstring block = env_block(env);
    BOOL ok = CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, block.data(), cwd ? wcwd.c_str() : nullptr, &si, &pi);
    DWORD last = GetLastError();
    if (!ok) fail(last);
    CloseHandle(pi.hThread);
    for (HANDLE h : child_ends) CloseHandle(h);
    child_ends.clear();
    Ran r;
    auto drain = [](HANDLE h, std::string& dst) {
        char buf[8192];
        DWORD got = 0;
        while (h && ReadFile(h, buf, sizeof buf, &got, nullptr) && got > 0) dst.append(buf, got);
    };
    std::thread t_out([&] { drain(out_r, r.out); });
    std::thread t_err([&] { drain(err_r, r.err); });
    DWORD waited = WaitForSingleObject(pi.hProcess, timeout_s > 0 ? static_cast<DWORD>(timeout_s * 1000) : INFINITE);
    if (waited == WAIT_TIMEOUT) {
        r.timed_out = true;
        TerminateProcess(pi.hProcess, 1);   // (the child this call started)
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    t_out.join();
    t_err.join();
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    r.code = static_cast<int64_t>(code);
    CloseHandle(pi.hProcess);
    if (out_r) CloseHandle(out_r);
    if (err_r) CloseHandle(err_r);
    return r;
}

std::string read_all_(const str& path) {
    std::ifstream f(path.utf8(), std::ios::binary);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// open(path, 'w', encoding='utf-8').write(text): on Windows "\n" is written as "\r\n"
void write_text_(const str& path, const str& text) {
    std::ofstream f(path.utf8(), std::ios::binary | std::ios::trunc);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
    std::string u = text.utf8(), out;
    for (char c : u) {
        if (c == '\n') out.push_back('\r');
        out.push_back(c);
    }
    f.write(out.data(), static_cast<std::streamsize>(out.size()));
}

// s[:n]
str head(const str& s, int64_t n) { return s.size() > n ? s.slice(0, n) : s; }
// s[-n:]
str tail(const str& s, int64_t n) { return s.size() > n ? s.slice(s.size() - n, s.size()) : s; }

py::list<str> cat(const py::list<str>& a, const py::list<str>& b) {
    py::list<str> out;
    out.extend(a);
    out.extend(b);
    return out;
}

void print_line(const str& s) {
    std::string u = s.utf8();
    std::fwrite(u.data(), 1, u.size(), stdout);
    std::fputc('\n', stdout);
}

}  // namespace

const str EQUIVALENT = str("equivalent"), DIVERGENT = str("divergent"), REFUSED = str("refused"), NOT_RUN = str("not-run");

void set_java_frontend(const JavaFrontend& f) { fe_() = f; }

namespace {
// _STD = {'cpp03': ('C++03', 'c++03'), 'cpp17': ('C++17', 'c++17')}
std::tuple<str, str> std_of(const str& backend) {
    if (backend == S("cpp03")) return {S("C++03"), S("c++03")};
    if (backend == S("cpp17")) return {S("C++17"), S("c++17")};
    throw py::KeyError(py::repr(backend).utf8());
}
}  // namespace

// ------------------------------------------------------------------------------ through the transpiler
TreeVerdicts judge_tree(const str& src, const str& work, const str& backend, const std::optional<py::set<str>>& only, int64_t workers, const Log& log_in) {
    Log log = log_in ? log_in : Log(print_line);
    auto [label, std_] = std_of(backend);
    // (lane.CPP_STD, lane.STD, lane.FLAGS, probe.CXX_STD are module variables the Python reassigns and puts back)
    str& lane_std = const_cast<str&>(lane::STD);
    str& lane_cpp_std = const_cast<str&>(lane::CPP_STD);
    str saved_cpp_std = lane_cpp_std, saved_std = lane_std, saved_cxx = probe::CXX_STD;
    py::list<str> saved_flags = lane::FLAGS.copy();
    struct Restore {
        std::function<void()> f;
        ~Restore() { f(); }
    } restore{[&] {
        lane_cpp_std = saved_cpp_std;
        lane_std = saved_std;
        probe::CXX_STD = saved_cxx;
        lane::FLAGS.clear();
        lane::FLAGS.extend(saved_flags);
    }};
    lane_cpp_std = label;
    lane_std = std_;
    probe::CXX_STD = std_;
    {
        py::list<str> nf{S("-std=") + std_};
        for (const str& f : saved_flags) {
            if (!f.startswith(S("-std="))) nf.append(f);
        }
        lane::FLAGS.clear();
        lane::FLAGS.extend(nf);
    }
    lane::Prepared m = lane::prepare(src, work);
    log(S("[") + backend + S("] converted: ") + py::to_str(py::len(m.order)) + S(" classes"));
    lane::Built built = lane::build(m, src, work);
    py::list<str> names;
    for (const str& n : m.order) {
        if (built.items.contains(n) && (!only || only->contains(n))) names.append(n);
    }
    TreeVerdicts out;
    std::mutex lock;
    std::vector<str> todo(names.vec().begin(), names.vec().end());
    auto worker = [&]() {
        for (;;) {
            str n;
            {
                std::lock_guard<std::mutex> hold(lock);
                if (todo.empty()) return;
                n = todo.front();
                todo.erase(todo.begin());
            }
            str verdict, reason, detail;
            try {
                lane::Verdict v = lane::judge(built.context, built.items.at(n));
                verdict = std::get<0>(v);
                reason = std::get<1>(v);
                detail = std::get<3>(v);
            } catch (const py::Exception& e) {   // a judge that fails says so, it is not alike
                verdict = V::INCONCLUSIVE;
                reason = S("judge-error");
                detail = head(str(std::string(e.what())), 300);
            }
            int64_t gaps = m.texts.contains(n) ? m.texts.at(n).count(S("J2X-GAP:")) : 0;
            std::lock_guard<std::mutex> hold(lock);
            str word = _word(verdict, gaps);
            out.set(n, std::make_tuple(word, gaps, reason + S(": ") + head(detail, 400)));
            log(S("[") + backend + S("] ") + n + S(": ") + word + S(" (") + py::to_str(gaps) + S(" gaps)"));
        }
    };
    std::vector<std::thread> threads;
    for (int64_t i = 0; i < std::max<int64_t>(1, workers); ++i) threads.emplace_back(worker);
    for (std::thread& t : threads) t.join();
    return out;
}

str _word(const str& verdict, int64_t gaps) {
    (void)gaps;
    if (verdict == V::NO_COMPILE) return REFUSED;
    if (verdict == V::DIVERGENT) return DIVERGENT;
    if (V::GOOD().contains(verdict)) return EQUIVALENT;
    return NOT_RUN;
}

// ------------------------------------------------------------------------------ whole programs from L2
std::tuple<str, str> java_tools() {
    auto [javac, java] = lane::jdk();
    if (javac.empty()) throw py::RuntimeError("no JDK (settings.json: java)");
    return {javac, java};
}

py::list<ir::NodeP> lower_program(const str& src, const str& work, const std::optional<str>& release) {
    str tree = py::os::path::join(work, S("tree"));
    py::dict<str, ir::Dict> units = fe().dump(src, tree, release);
    for (const ir::Dict& d : fe().diagnostics(tree)) {
        const ir::Val& kind = d.at(S("kind"));
        const str* k = std::get_if<str>(&kind.v);
        if (k && *k == S("ERROR")) {
            const ir::Val& msg = d.at(S("message"));
            const str* m = std::get_if<str>(&msg.v);
            throw py::RuntimeError((S("javac: ") + head(m ? *m : S(""), 300)).utf8());
        }
    }
    py::list<str> keys;
    for (const auto& kv : units.items()) keys.append(std::get<0>(kv));
    keys.sort();
    py::list<ir::NodeP> out;
    for (const str& p : keys) out.append(jlower::lower(units.at(p), std::optional<str>(p)));
    return out;
}

std::tuple<int64_t, str, str> _run(const py::list<str>& cmd, const std::optional<str>& cwd, int64_t timeout, const std::map<std::string, std::string>& env) {
    Ran r = spawn(cmd, cwd, timeout, env);
    if (r.timed_out) return {-999, S(""), S("timed out")};
    return {r.code, py::bytes(r.out).decode("utf-8", "replace"), py::bytes(r.err).decode("utf-8", "replace")};
}

// the probe harness
const str HARNESS = str("J2XProbe");
const py::dict<str, str> _FORMAT = {{str("int"), str("fi")},     {str("long"), str("fi")},  {str("short"), str("fi")}, {str("byte"), str("fi")},
                                    {str("char"), str("fc")},    {str("boolean"), str("fb")}, {str("float"), str("ff")}, {str("double"), str("fd")},
                                    {str("String"), str("fs")}};
const py::dict<str, str> _JAVA_PRIM = {{str("i32"), str("int")},   {str("i64"), str("long")},     {str("i16"), str("short")},
                                       {str("i8"), str("byte")},   {str("u16"), str("char")},     {str("bool"), str("boolean")},
                                       {str("f32"), str("float")}, {str("f64"), str("double")}};
const str _HELPERS = str(R"HLP(
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
)HLP");

std::optional<str> _jname(const ir::TypeP& t) {
    if (!t) return std::nullopt;
    if (_JAVA_PRIM.contains(t->kind)) return _JAVA_PRIM.at(t->kind);
    if (t->kind == S("class") && t->name && *t->name == S("java.lang.String")) return S("String");
    return std::nullopt;
}

py::str _package(const py::list<ir::NodeP>& units) {
    std::set<str> pkgs;
    for (const ir::NodeP& n : units) {
        auto u = std::dynamic_pointer_cast<ir::Unit>(n);
        pkgs.insert(u && u->package ? *u->package : S(""));
    }
    return pkgs.size() == 1 ? *pkgs.begin() : S("");
}

py::list<ProbeMethod> probe_methods(const py::list<ir::NodeP>& units) {
    py::list<ProbeMethod> out;
    auto has = [](const std::optional<py::list<str>>& mods, const char* what) { return mods && mods->contains(S(what)); };
    std::function<void(const py::ref<ir::ClassDecl>&, const str&, bool)> visit = [&](const py::ref<ir::ClassDecl>& c, const str& ref, bool ok) {
        bool tp = c->typeparams && !c->typeparams->empty();
        if (tp || (c->kind && (*c->kind == S("interface") || *c->kind == S("annotation")))) ok = false;
        if (!c->members) return;
        for (const ir::NodeP& m : *c->members) {
            if (auto cd = std::dynamic_pointer_cast<ir::ClassDecl>(m)) {
                visit(cd, ref + S(".") + (cd->name ? *cd->name : S("None")), ok && !has(cd->mods, "private"));
            } else if (auto md = std::dynamic_pointer_cast<ir::MethodDecl>(m)) {
                if (ok && md->kind && *md->kind == S("method") && md->sym && md->sym->static_ && !has(md->mods, "private") &&
                    !(md->name && *md->name == S("main")) && !(md->typeparams && !md->typeparams->empty())) {
                    py::list<str> ps;
                    bool all = true;
                    if (md->params) {
                        for (const ir::NodeP& p : *md->params) {
                            auto pp = std::dynamic_pointer_cast<ir::Param>(p);
                            std::optional<str> n = _jname(pp ? pp->type : nullptr);
                            if (!n) all = false;
                            else ps.append(*n);
                        }
                    }
                    std::optional<str> r = _jname(md->ret);
                    if (r && all) out.append(ProbeMethod(ref, md->name ? *md->name : S("None"), ps, *r));
                }
            }
        }
    };
    str package = _package(units);
    for (const ir::NodeP& n : units) {
        auto u = std::dynamic_pointer_cast<ir::Unit>(n);
        if ((u && u->package ? *u->package : S("")) != package) continue;
        if (!u || !u->types) continue;
        for (const ir::NodeP& t : *u->types) {
            auto c = std::dynamic_pointer_cast<ir::ClassDecl>(t);
            if (c && !(c->name && *c->name == HARNESS)) visit(c, c->name ? *c->name : S("None"), true);
        }
    }
    return out;
}

std::tuple<str, ProbeKeys> harness(const py::list<ProbeMethod>& methods, const str& package) {
    py::list<str> lines;
    if (!package.empty()) lines.append(S("package ") + package + S(";"));
    lines.append(S("public class ") + HARNESS + S(" {"));
    lines.append(_HELPERS);
    lines.append(S("  public static void main(String[] a) {"));
    ProbeKeys keys;
    for (int64_t mi = 0; mi < methods.size(); ++mi) {
        const auto& [ref, name, ps, r] = methods[mi];
        py::list<py::list<str>> cs = probe::cases(ps);
        for (int64_t ci = 0; ci < cs.size(); ++ci) {
            const py::list<str>& values = cs[ci];
            str key = py::to_str(mi) + S(".") + py::to_str(ci);
            py::list<str> lits;
            for (int64_t k = 0; k < std::min(ps.size(), values.size()); ++k) lits.append(probe::_java_literal(ps[k], values[k]));
            str args = py::join(S(", "), lits);
            lines.append(S("    try { out(\"") + key + S("\", ") + _FORMAT.at(r) + S("(") + ref + S(".") + name + S("(") + args + S("))); } catch (Throwable t) { out(\"") +
                         key + S("\", \"throw\"); }"));
            keys.set(key, std::make_tuple(ref + S(".") + name, values, r));
        }
    }
    lines.append(S("  }"));
    lines.append(S("}"));
    lines.append(S(""));
    return {py::join(S("\n"), lines), keys};
}

str _stage(const str& src, const str& dest) {
    if (py::os::path::isdir(dest)) py::shutil::rmtree(dest);
    py::os::makedirs(dest);
    for (auto t : py::os::walk(src)) {
        for (const str& f : std::get<2>(t)) {
            if (f.endswith(S(".java"))) py::shutil::copy(py::os::path::join(std::get<0>(t), f), py::os::path::join(dest, f));
        }
    }
    return dest;
}

ProgramVerdict judge_program(const str& src, const str& work, const str& backend, const Log& log) {
    (void)log;
    auto [javac, java] = java_tools();
    ProgramVerdict res;
    res.backend = backend;
    str stage = _stage(src, py::os::path::join(work, S("src")));
    py::list<ir::NodeP> units = lower_program(stage, py::os::path::join(work, S("probe-l2")));
    py::list<ProbeMethod> methods = probe_methods(units);
    str package = _package(units);
    auto [text, keys] = harness(methods, package);
    write_text_(py::os::path::join(stage, HARNESS + S(".java")), text);
    py::list<str> mains;
    for (const ir::NodeP& n : units) {
        auto u = std::dynamic_pointer_cast<ir::Unit>(n);
        if (!u || !u->types) continue;
        for (const ir::NodeP& t : *u->types) {
            auto c = std::dynamic_pointer_cast<ir::ClassDecl>(t);
            if (!c || !c->members) continue;
            bool any = false;
            for (const ir::NodeP& m : *c->members) {
                auto md = std::dynamic_pointer_cast<ir::MethodDecl>(m);
                if (md && md->name && *md->name == S("main") && md->sym && md->sym->static_) any = true;
            }
            if (any) mains.append(c->name ? *c->name : S("None"));
        }
    }
    str qual = !package.empty() ? package + S(".") : S("");
    str classes = py::os::path::join(work, S("classes"));
    py::os::makedirs(classes, 0777, true);
    {
        py::list<str> files = py::os::listdir(stage);
        files.sort();
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("-d"), classes};
        for (const str& f : files) {
            if (f.endswith(S(".java"))) cmd.append(py::os::path::join(stage, f));
        }
        auto [code, o, err] = _run(cmd, std::nullopt, 300);
        (void)o;
        if (code != 0) {
            res.why = S("javac: ") + head(err, 300);
            return res;
        }
    }
    // the original, twice: a result that changes between two runs of Java itself says nothing
    py::list<py::dict<str, str>> runs;
    for (int i = 0; i < 2; ++i) runs.append(probe::_results(std::get<1>(_run(py::list<str>{java, S("-cp"), classes, qual + HARNESS}))));
    py::dict<str, str> jres;
    for (const auto& kv : runs[0].items()) {
        const str& k = std::get<0>(kv);
        const str& v = std::get<1>(kv);
        if (runs[1].contains(k) && runs[1].at(k) == v && v != S("throw")) jres.set(k, v);
    }
    py::list<std::tuple<str, std::tuple<int64_t, str, str>>> jmain;
    {
        std::set<str> done;
        for (const str& m : mains) {
            // dict((m, _run(...)) ...): one entry per name, the last run's value
            auto r = _run(py::list<str>{java, S("-cp"), classes, qual + m});
            bool replaced = false;
            for (int64_t i = 0; i < jmain.size(); ++i) {
                if (std::get<0>(jmain[i]) == m) {
                    jmain[i] = {m, r};
                    replaced = true;
                }
            }
            if (!replaced) jmain.append({m, r});
        }
    }
    // the translation
    std::optional<str> exe;
    str why;
    try {
        py::list<ir::NodeP> units2 = lower_program(stage, py::os::path::join(work, S("l2")));
        auto [e, w] = build(units2, work, backend);
        exe = e;
        why = w;
    } catch (const py::Exception& e) {   // Decline, or a front end failure
        exe = std::nullopt;
        why = str(e.type()) + S(": ") + head(str(std::string(e.what())), 300);
    }
    if (!exe) {
        res.verdict = REFUSED;
        res.why = why;
        return res;
    }
    auto [c0, out, e0] = _run(py::list<str>{*exe}, std::nullopt, 120, {{"JRT_MAIN", HARNESS.utf8()}});
    (void)c0;
    (void)e0;
    py::dict<str, str> cres = probe::_results(out);
    std::set<str> judged;
    for (const auto& kv : jres.items()) {
        const str& k = std::get<0>(kv);
        const str& v = std::get<1>(kv);
        if (!keys.contains(k)) continue;
        const auto& [name, values, rt] = keys.at(k);
        res.cases += 1;
        judged.insert(name);
        if (!cres.contains(k)) {
            res.differs.append({name, values, v, S("no result (crashed?)")});
        } else if (!probe::same(rt, v, cres.at(k))) {
            res.differs.append({name, values, v, cres.at(k)});
        }
    }
    res.methods = static_cast<int64_t>(judged.size());
    bool main_ok = true;
    res.main = py::dict<str, str>();
    for (int64_t i = 0; i < jmain.size(); ++i) {
        const str& m = std::get<0>(jmain[i]);
        const auto& [jc, jout, jerr] = std::get<1>(jmain[i]);
        (void)jerr;
        auto [cc, cout_, cerr] = _run(py::list<str>{*exe}, std::nullopt, 120, {{"JRT_MAIN", m.utf8()}});
        bool alike = jc == cc && jout.replace(S("\r\n"), S("\n")) == cout_.replace(S("\r\n"), S("\n"));
        res.main->set(m, alike ? S("alike")
                               : S("differs: exit ") + py::to_str(jc) + S("/") + py::to_str(cc) + S("\n--- java\n") + tail(jout, 1500) + S("\n--- native\n") +
                                     tail(cout_, 1500) + S("\n") + tail(cerr, 500));
        main_ok = main_ok && alike;
    }
    if (!res.differs.empty() || !main_ok) {
        res.verdict = DIVERGENT;
    } else if (res.cases || !res.main->empty()) {
        res.verdict = EQUIVALENT;
    } else {
        res.why = S("nothing it could run and compare");
    }
    return res;
}

std::tuple<std::optional<str>, str> build(const py::list<ir::NodeP>& units, const str& work, const str& backend) {
    str out = py::os::path::join(work, backend);
    if (py::os::path::isdir(out)) py::shutil::rmtree(out);
    py::os::makedirs(out);
    if (backend == S("cpp17")) {
        str text = cpp17::program(units);
        str src = py::os::path::join(out, S("program.cpp"));
        {
            // open(src, 'w', encoding='utf-8').write(text)
            write_text_(src, text);
        }
        str cxx = _host_cxx();
        str exe = py::os::path::join(out, S("program.exe"));
        py::list<str> cmd{cxx};
        cmd.extend(cpp17::FLAGS);
        cmd.extend(py::list<str>{S("-I"), cpp17::RUNTIME, src, S("-o"), exe});
        auto [code, o, err] = _run(cmd, std::nullopt, 600);
        (void)o;
        if (code != 0) {
            str first = head(err, 300);
            for (const str& l : py::splitlines(err)) {
                if (l.contains(S("error"))) {
                    first = l;
                    break;
                }
            }
            return {std::nullopt, S("does not compile: ") + head(first, 400)};
        }
        return {std::optional<str>(exe), S("")};
    }
    if (backend == S("c")) return c_java::build(units, out);
    throw py::ValueError(("no whole-program back end " + py::repr(backend).utf8()));
}

str _host_cxx() {
    str cxx = lane::compiler();
    if (cxx.empty()) throw py::RuntimeError("no C++ compiler (settings.json: host_cxx)");
    return cxx;
}

}  // namespace codebreaker::translate::javajudge

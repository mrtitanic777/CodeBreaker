// codebreaker/translate/judge_native_jvm.cpp -- typed C++ from codebreaker.translate.judge_native_jvm.
// See judge_native_jvm.hpp.

#include "codebreaker/translate/judge_native_jvm.hpp"

#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>

#include "codebreaker/java/transpile.hpp"
#include "codebreaker/translate/frontends/clang/j2x.hpp"
#include "codebreaker/translate/frontends/clang/package.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::judge_native_jvm {

namespace lane = codebreaker::verify::lanes::java;
namespace clang = codebreaker::translate::frontends::clang;
namespace j2x = codebreaker::translate::frontends::clang::j2x;
namespace transpile = codebreaker::java::transpile;
namespace l2 = codebreaker::translate::l2;
using py::str;

namespace {

str S(const char* s) { return str(s); }

// ---- subprocess.run(...) for this module: the runtime's run has no timeout, no stdin bytes, no environment ----
struct Ran {
    int64_t code = 0;
    std::string out, err;   // raw bytes
    bool timed_out = false;
};

// Runs `cmd` (the child this call starts), feeding it `input` on stdin when given (else it has the stdin of this
// process), waiting at most `timeout_s` seconds (0: forever; on a timeout the child is ended).
Ran spawn(const py::list<str>& cmd, const std::optional<str>& cwd, const std::optional<std::string>& input, int64_t timeout_s) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr, in_r = nullptr, in_w = nullptr;
    std::vector<HANDLE> child_ends;
    auto fail = [&](DWORD code) {
        for (HANDLE h : {out_r, err_r, in_w})
            if (h) CloseHandle(h);
        for (HANDLE h : child_ends) CloseHandle(h);
        py::raise_winerror(code);
    };
    if (input) {
        if (!CreatePipe(&in_r, &in_w, &sa, 0)) fail(GetLastError());
        child_ends.push_back(in_r);
        SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    } else {
        HANDLE cur = GetStdHandle(STD_INPUT_HANDLE);
        if (cur && cur != INVALID_HANDLE_VALUE && DuplicateHandle(GetCurrentProcess(), cur, GetCurrentProcess(), &in_r, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
            child_ends.push_back(in_r);
        } else {
            in_r = nullptr;
        }
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
    BOOL ok = CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, nullptr, cwd ? wcwd.c_str() : nullptr, &si, &pi);
    DWORD last = GetLastError();
    if (!ok) fail(last);
    CloseHandle(pi.hThread);
    for (HANDLE h : child_ends) CloseHandle(h);   // the child's ends belong to the child now
    child_ends.clear();
    Ran r;
    auto drain = [](HANDLE h, std::string& dst) {
        char buf[8192];
        DWORD got = 0;
        while (h && ReadFile(h, buf, sizeof buf, &got, nullptr) && got > 0) dst.append(buf, got);
    };
    std::thread t_out([&] { drain(out_r, r.out); });
    std::thread t_err([&] { drain(err_r, r.err); });
    std::thread t_in;
    if (input) {
        t_in = std::thread([&] {
            size_t at = 0;
            while (at < input->size()) {
                DWORD wrote = 0;
                if (!WriteFile(in_w, input->data() + at, static_cast<DWORD>(std::min<size_t>(input->size() - at, 1 << 16)), &wrote, nullptr) || wrote == 0) break;
                at += wrote;
            }
            CloseHandle(in_w);
        });
    }
    DWORD waited = WaitForSingleObject(pi.hProcess, timeout_s > 0 ? static_cast<DWORD>(timeout_s * 1000) : INFINITE);
    if (waited == WAIT_TIMEOUT) {
        r.timed_out = true;
        TerminateProcess(pi.hProcess, 1);   // (the child this call started)
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    if (t_in.joinable()) t_in.join();
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

// text=True, errors='replace': cp1252, and universal newlines
str as_text(const std::string& raw) {
    std::u32string u = py::bytes(raw).decode("cp1252", "replace").u32();
    std::u32string out;
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') ++i;
        } else {
            out.push_back(u[i]);
        }
    }
    return str(out);
}

struct TextRun {
    int64_t returncode;
    str out, err;
};

// subprocess.run(cmd, capture_output=True, text=True, ..., timeout=timeout): TimeoutExpired past it
TextRun run_text(const py::list<str>& cmd, int64_t timeout) {
    Ran r = spawn(cmd, std::nullopt, std::nullopt, timeout);
    if (r.timed_out) throw TimeoutExpired();
    return TextRun{r.code, as_text(r.out), as_text(r.err)};
}

str decode_replace(const py::bytes& b) { return b.decode("utf-8", "replace"); }

std::string read_all_(const str& path) {
    std::ifstream f(path.utf8(), std::ios::binary);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

// open(path, 'w', encoding='utf-8', newline='\n').write(text)
void write_text(const str& path, const str& text) {
    std::ofstream f(path.utf8(), std::ios::binary | std::ios::trunc);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());
    std::string u = text.utf8();
    f.write(u.data(), static_cast<std::streamsize>(u.size()));
}

// settings.state(*parts): a path inside the state folder; its folder is created if it is not there.
str state_(const str& a, const str& b) {
    str path = py::os::path::join(py::settings::get_str(S("state")), a, b);
    py::os::makedirs(py::os::path::dirname(path), 0777, true);
    return path;
}

py::list<str> cat(const py::list<str>& a, const py::list<str>& b) {
    py::list<str> out;
    out.extend(a);
    out.extend(b);
    return out;
}

bool lower_endswith(const str& s, std::initializer_list<const char*> exts) {
    str l = s.lower();
    for (const char* e : exts) {
        if (l.endswith(S(e))) return true;
    }
    return false;
}

str fmt_code(const std::variant<int64_t, str>& c) {
    if (const int64_t* i = std::get_if<int64_t>(&c)) return py::to_str(*i);
    return std::get<str>(c);
}

}  // namespace


str Verdict::repr() const {
    return S("Verdict(") + name + S(": ") + verdict + (!reason.empty() ? S(" -- ") + reason : S("")) + S(")");
}

py::dict<str, std::optional<str>> Verdict::row() const {
    py::dict<str, std::optional<str>> d;
    d.set(S("program"), std::optional<str>(name));
    d.set(S("verdict"), std::optional<str>(verdict));
    d.set(S("reason"), std::optional<str>(reason));
    d.set(S("tier"), tier);
    return d;
}

str runtime_classes() {
    auto [javac, java_] = lane::jdk();
    (void)java_;
    str src = py::os::path::join(jback::RUNTIME(), S("crt"));
    py::list<str> files;
    for (const str& f : py::os::listdir(src)) {
        if (f.endswith(S(".java"))) files.append(py::os::path::join(src, f));
    }
    files.sort();
    py::hashlib::Hash h = py::hashlib::sha1(py::bytes(py::repr(javac).encode("utf-8")));
    for (const str& f : files) h.update(py::bytes(read_all_(f)));
    str folder = state_(S("translate"), S("java-crt-") + h.hexdigest().slice(0, 12));
    if (!py::os::path::isfile(py::os::path::join(folder, S("crt"), S("LibC.class")))) {
        py::os::makedirs(folder, 0777, true);
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("--release"), S("17"), S("-d"), folder};
        cmd.extend(files);
        TextRun r = run_text(cmd, 300);
        if (r.returncode) {
            py::shutil::rmtree(folder, true);
            throw py::RuntimeError((S("the C runtime for Java does not compile: ") + r.out + r.err).utf8());
        }
    }
    return folder;
}

Run _run(const py::list<str>& cmd, const py::bytes& stdin_, const str& cwd, int64_t timeout) {
    Ran r;
    try {
        r = spawn(cmd, std::optional<str>(cwd), std::optional<std::string>(stdin_.raw()), timeout);
    } catch (const TimeoutExpired&) {
        return Run{py::bytes(), py::bytes("(timed out)"), str("timeout")};
    }
    if (r.timed_out) return Run{py::bytes(), py::bytes("(timed out)"), str("timeout")};
    return Run{py::bytes(r.out), py::bytes(r.err), std::variant<int64_t, str>(r.code)};
}

py::bytes _norm(const py::bytes& b) { return b.replace(py::bytes("\r\n"), py::bytes("\n")); }

std::tuple<bool, str> native(const py::list<str>& files, const str& out_exe, const str& language, const std::optional<str>& std_,
                             const py::list<str>& defines, const py::list<str>& includes) {
    std::optional<str> exe = clang::clang();
    // (a missing clang is the Python's TypeError: [None] + ... joined into a command)
    if (!exe) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    py::list<str> cmd{*exe};
    cmd.extend(clang::flags(language, std_, includes, py::list<str>(), defines));
    cmd.extend(py::list<str>{S("-O0"), S("-o"), out_exe});
    cmd.extend(files);
    TextRun r = run_text(cmd, 300);
    str msg = !r.err.empty() ? r.err : r.out;
    int64_t n = msg.size();
    return {r.returncode == 0, n > 1500 ? msg.slice(n - 1500, n) : msg};
}

ir::NodeP _unit(const str& path, const str& language, const std::optional<str>& std_, const py::list<str>& includes,
                const py::list<str>& quote_includes, const py::list<str>& defines, const std::optional<py::list<str>>& own) {
    return l2::from_json(clang::l2_file(path, language, std_, includes, quote_includes, defines, own));
}

jback::Program translate(const py::list<str>& files, const str& name, const std::optional<str>& language, const std::optional<str>& std_,
                         const py::list<str>& defines, const py::list<str>& includes) {
    py::list<ir::NodeP> units;
    for (const str& f : files) {
        if (!lower_endswith(f, {".c", ".cpp", ".cc", ".cxx"})) continue;
        str lang = (language && !language->empty()) ? *language : clang::language_of(f);
        units.append(_unit(f, lang, std_, includes, py::list<str>(), defines));
    }
    for (const ir::NodeP& u : units) {
        py::list<py::ref<ir::Unsupported>> bad = ir::unsupported(u);
        if (!bad.empty()) {
            const py::ref<ir::Unsupported>& x = bad[0];
            auto unit = std::dynamic_pointer_cast<ir::Unit>(u);
            str base = py::os::path::basename(unit && unit->path ? *unit->path : S(""));
            str raw = (x->raw && !x->raw->empty()) ? S(" (") + *x->raw + S(")") : S("");
            throw jback::Refused(S("front end: ") + (x->kind ? *x->kind : S("None")) + S(" at ") + base + S(":") + py::to_str(x->line) + raw);
        }
    }
    return jback::program(units, name);
}

namespace {

// the end of a text as Python's text[-n:]
str tail(const str& s, int64_t n) { return s.size() > n ? s.slice(s.size() - n, s.size()) : s; }

}  // namespace

str _first_error(const str& text) {
    for (const str& line : py::splitlines(text)) {
        str l = line.lower();
        if (l.contains(S("error")) || l.contains(S("exception"))) {
            str t = line.strip();
            return t.size() > 300 ? t.slice(0, 300) : t;
        }
    }
    if (!text.strip().empty()) {
        py::list<str> ls = py::splitlines(text.strip());
        str t = ls[0];
        return t.size() > 300 ? t.slice(0, 300) : t;
    }
    return S("");
}

str _class_name(const str& stem) {
    str n = py::re::sub("\\W", S("_"), stem);
    if (n.empty() || n[0].isdigit()) n = S("P") + n;
    return n.slice(0, 1).upper() + n.slice_from(1);
}

std::optional<str> _compare(const Run& a, const Run& b) {
    py::bytes ao = _norm(a.out), bo = _norm(b.out);
    if (ao != bo) {
        const std::string& x = ao.raw();
        const std::string& y = bo.raw();
        int64_t n = static_cast<int64_t>(std::min(x.size(), y.size()));
        int64_t i = n;
        for (int64_t k = 0; k < n; ++k) {
            if (x[static_cast<size_t>(k)] != y[static_cast<size_t>(k)]) {
                i = k;
                break;
            }
        }
        int64_t line = 1;
        for (int64_t k = 0; k < i; ++k) {
            if (x[static_cast<size_t>(k)] == '\n') ++line;
        }
        int64_t from = i > 20 ? i - 20 : 0;
        return S("stdout differs at line ") + py::to_str(line) + S(": C ") + py::repr(ao.slice(from, i + 30)) + S(", Java ") +
               py::repr(bo.slice(from, i + 30));
    }
    if (a.code != b.code) {
        str extra = S("");
        if (!b.err.empty()) extra = S(" (") + _first_error(decode_replace(b.err)) + S(")");
        return S("exit code: C ") + fmt_code(a.code) + S(", Java ") + fmt_code(b.code) + extra;
    }
    if (_norm(a.err) != _norm(b.err)) {
        py::bytes x = _norm(a.err), y = _norm(b.err);
        return S("stderr differs: C ") + py::repr(x.slice(0, 120)) + S(", Java ") + py::repr(y.slice(0, 120));
    }
    return std::nullopt;
}

Verdict judge(const py::list<str>& files_in, const str& work, const std::optional<str>& name_in, const std::optional<str>& language_in,
              const std::optional<str>& std_, const py::list<str>& defines, const py::list<str>& includes, const py::bytes& stdin_,
              const py::list<str>& args) {
    py::list<str> files;
    for (const str& f : files_in) files.append(py::os::path::abspath(f));
    str language = (language_in && !language_in->empty()) ? *language_in : clang::language_of(files[0]);
    str name = (name_in && !name_in->empty()) ? *name_in : _class_name(std::get<0>(py::os::path::splitext(py::os::path::basename(files[0]))));
    py::os::makedirs(work, 0777, true);
    str exe = py::os::path::join(work, name + S(".exe"));
    py::list<str> srcs;
    for (const str& f : files) {
        if (!lower_endswith(f, {".h", ".hpp"})) srcs.append(f);
    }
    auto [ok, msg] = native(srcs, exe, language, std_, defines, includes);
    if (!ok) return Verdict(name, S("no-original"), S("clang: ") + _first_error(msg));
    py::list<str> run_cmd{exe};
    run_cmd.extend(args);
    Run nat = _run(run_cmd, stdin_, work);
    std::optional<jback::Program> prog_;
    try {
        prog_ = translate(files, name, language, std_, defines, includes);
    } catch (const jback::Refused& e) {
        return Verdict(name, S("refused"), str(std::string(e.what())), std::nullopt, std::nullopt, nat);
    } catch (const py::RuntimeError& e) {
        str m = str(std::string(e.what()));
        return Verdict(name, S("refused"), S("front end: ") + (m.size() > 300 ? m.slice(0, 300) : m), std::nullopt, std::nullopt, nat);
    }
    const jback::Program& prog = *prog_;
    str jdir = py::os::path::join(work, S("java"));
    py::shutil::rmtree(jdir, true);
    py::os::makedirs(jdir);
    str src = py::os::path::join(jdir, name + S(".java"));
    write_text(src, prog.source);
    auto [javac, java_] = lane::jdk();
    str crt = runtime_classes();
    str classes = py::os::path::join(jdir, S("classes"));
    py::os::makedirs(classes);
    {
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("--release"), S("17"), S("-cp"), crt, S("-d"), classes, src};
        TextRun r = run_text(cmd, 300);
        if (r.returncode) {
            return Verdict(name, S("no-compile"), S("javac: ") + _first_error(r.out + r.err), std::optional<str>(prog.tier),
                           std::optional<str>(prog.source), nat, std::nullopt, prog.notes);
        }
    }
    // (argv[0] is where the program runs from: the native one's own path, given to both)
    py::list<str> jcmd{java_, S("-Xss64m"), S("-Dc.argv0=") + exe, S("-cp"), classes + S(";") + crt, name};
    jcmd.extend(args);
    Run jvm = _run(jcmd, stdin_, work);
    std::optional<str> v = _compare(nat, jvm);
    py::list<str> gaps;
    for (const str& n : prog.notes) {
        if (n.startswith(S("gap: "))) gaps.append(n.slice_from(5));
    }
    if (v && jvm.err.raw().find("CodeBreaker gap: ") != std::string::npos) {
        // the run reached a part the translator declined: the program is refused, with that part's reason
        str text = decode_replace(jvm.err);
        str after = text.slice_from(text.find(S("CodeBreaker gap: ")) + static_cast<int64_t>(std::string("CodeBreaker gap: ").size()));
        py::list<str> ls = py::splitlines(after);
        if (ls.empty()) throw py::IndexError("list index out of range");
        return Verdict(name, S("refused"), S("gap reached: ") + ls[0], std::optional<str>(prog.tier), std::optional<str>(prog.source), nat, jvm,
                       prog.notes);
    }
    str reason = v ? *v : S("");
    if (!gaps.empty()) {   // (an explicit gap the run did not reach still says it is there)
        reason = (!reason.empty() ? reason + S("; ") : S("")) + S("gaps: ") + py::join(S("; "), gaps);
    }
    return Verdict(name, !v ? S("equivalent") : S("divergent"), reason, std::optional<str>(prog.tier), std::optional<str>(prog.source), nat, jvm,
                   prog.notes);
}

Verdict roundtrip(const py::list<str>& java_files, const str& main_class, const str& work, const str& name) {
    auto [javac, java_] = lane::jdk();
    py::shutil::rmtree(work, true);
    str src = py::os::path::join(work, S("src"));
    py::list<str> classes;
    for (const str& f : java_files) {
        // open(f, encoding='utf-8').read(): decoded strictly, universal newlines
        str raw = py::bytes(read_all_(f)).decode("utf-8");
        std::u32string u;
        const std::u32string& r32 = raw.u32();
        for (size_t i = 0; i < r32.size(); ++i) {
            if (r32[i] == U'\r') {
                u.push_back(U'\n');
                if (i + 1 < r32.size() && r32[i + 1] == U'\n') ++i;
            } else {
                u.push_back(r32[i]);
            }
        }
        str text(u);
        auto m = py::re::search("^\\s*package\\s+([\\w.]+)\\s*;", text, py::re::M);
        str pkg = m ? m->group(1) : S("");
        str d = src;
        if (!pkg.empty()) {
            for (const str& part : py::split(pkg, S("."))) d = py::os::path::join(d, part);
        }
        py::os::makedirs(d, 0777, true);
        write_text(py::os::path::join(d, py::os::path::basename(f)), text);
        classes.append(std::get<0>(py::os::path::splitext(py::os::path::basename(f))));
    }
    // the original, on the JVM
    str orig = py::os::path::join(work, S("orig"));
    py::os::makedirs(orig);
    py::list<str> staged;
    for (auto t : py::os::walk(src)) {
        for (const str& f : std::get<2>(t)) {
            if (f.endswith(S(".java"))) staged.append(py::os::path::join(std::get<0>(t), f));
        }
    }
    {
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("-d"), orig};
        cmd.extend(staged);
        TextRun r = run_text(cmd, 300);
        if (r.returncode) return Verdict(name, S("no-original"), S("javac: ") + _first_error(r.out + r.err));
    }
    Run nat = _run(py::list<str>{java_, S("-cp"), orig, main_class}, py::bytes(), work);
    // Java -> C++ (the Java lane)
    str cpp = py::os::path::join(work, S("cpp"));
    transpile::Summary s;
    try {
        s = transpile::convert(src, cpp, std::nullopt, nullptr, false);
    } catch (const py::Exception& e) {
        str m = str(std::string(e.what()));
        return Verdict(name, S("refused"), S("Java -> C++: ") + str(e.type()) + S(": ") + (m.size() > 300 ? m.slice(0, 300) : m), std::nullopt,
                       std::nullopt, nat);
    }
    py::dict<str, int64_t> declined_by = std::get<1>(s.tree_bodies);
    bool declined = !declined_by.empty();
    // C++ -> L2 (clang) -> Java
    py::list<py::ref<ir::Unit>> units;
    std::optional<jback::Program> prog_;
    try {
        for (const str& c : classes) {
            str cf = py::os::path::join(cpp, c + S(".cpp")), hf = py::os::path::join(cpp, c + S(".h"));
            ir::NodeP u = _unit(cf, S("c++"), std::optional<str>(S("c++14")), py::list<str>(), py::list<str>{cpp}, py::list<str>(),
                                std::optional<py::list<str>>(py::list<str>{cf, hf}));
            units.append(std::dynamic_pointer_cast<ir::Unit>(u));
        }
        j2x::lift(units);
        py::list<ir::NodeP> nodes;
        for (const auto& u : units) nodes.append(ir::NodeP(u));
        py::list<py::ref<ir::Unsupported>> bad;
        for (const ir::NodeP& u : nodes) {
            for (const auto& x : ir::unsupported(u)) bad.append(x);
        }
        if (!bad.empty()) {
            str raw = (bad[0]->raw && !bad[0]->raw->empty()) ? S(" (") + *bad[0]->raw + S(")") : S("");
            throw jback::Refused(S("front end: ") + (bad[0]->kind ? *bad[0]->kind : S("None")) + S(" at line ") + py::to_str(bad[0]->line) + raw);
        }
        prog_ = jback::program(nodes, name);
    } catch (const jback::Refused& e) {
        return Verdict(name, S("refused"), S("C++ -> Java: ") + str(std::string(e.what())), std::nullopt, std::nullopt, nat);
    } catch (const py::RuntimeError& e) {
        str m = str(std::string(e.what()));
        return Verdict(name, S("refused"), S("C++ -> Java: front end: ") + (m.size() > 300 ? m.slice(0, 300) : m), std::nullopt, std::nullopt, nat);
    }
    const jback::Program& prog = *prog_;
    str jdir = py::os::path::join(work, S("java"));
    py::os::makedirs(jdir);
    str out_src = py::os::path::join(jdir, name + S(".java"));
    write_text(out_src, prog.source);
    str crt = runtime_classes();
    str out_classes = py::os::path::join(jdir, S("classes"));
    py::os::makedirs(out_classes);
    TextRun r;
    {
        py::list<str> cmd{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("--release"), S("17"), S("-cp"), crt, S("-d"), out_classes, out_src};
        r = run_text(cmd, 300);
    }
    py::list<str> notes = prog.notes;
    if (declined) {
        // ['Java -> C++ declined bodies: %r' % (declined,)]: the dict of reasons
        str rep = S("{");
        bool first = true;
        for (const auto& kv : declined_by.items()) {
            rep = rep + (first ? S("") : S(", ")) + py::repr(std::get<0>(kv)) + S(": ") + py::to_str(std::get<1>(kv));
            first = false;
        }
        rep = rep + S("}");
        notes.append(S("Java -> C++ declined bodies: ") + rep);
    }
    if (r.returncode) {
        return Verdict(name, S("no-compile"), S("javac: ") + _first_error(r.out + r.err), std::optional<str>(prog.tier),
                       std::optional<str>(prog.source), nat, std::nullopt, notes);
    }
    Run jvm = _run(py::list<str>{java_, S("-cp"), out_classes + S(";") + crt, name}, py::bytes(), work);
    std::optional<str> v = _compare(nat, jvm);
    return Verdict(name, !v ? S("equivalent") : S("divergent"), v ? *v : S(""), std::optional<str>(prog.tier), std::optional<str>(prog.source), nat,
                   jvm, notes);
}

}  // namespace codebreaker::translate::judge_native_jvm

// codebreaker/translate/dotnet/judge.cpp -- typed C++ port of codebreaker.translate.dotnet.judge (see judge.hpp).

#include "codebreaker/translate/dotnet/judge.hpp"

#include <thread>
#include <vector>

#include "codebreaker/java/probe.hpp"
#include "codebreaker/verify/lanes/java.hpp"
#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::dotnet::judge {

using py::str;

namespace {

str S(const char* s) { return str(s); }

// ---- subprocess.run(args, stdout=PIPE, stderr=PIPE | STDOUT, timeout=, cwd=) (the runtime's run has no timeout)
struct Ran {
    int64_t returncode = 0;
    std::string out, err;   // raw bytes
    bool timed_out = false;
};

// Runs `cmd`, waiting at most `timeout_s` seconds. On a timeout the child this started is ended (subprocess.run's
// TimeoutExpired path). merge_err: the child's standard error goes into its standard output's pipe (stderr=STDOUT).
Ran run_(const py::list<str>& cmd, bool merge_err, int64_t timeout_s, const std::optional<str>& cwd = std::nullopt) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr, nul_in = nullptr;
    std::vector<HANDLE> child_ends;
    auto fail = [&](DWORD code) {
        for (HANDLE h : {out_r, err_r})
            if (h) CloseHandle(h);
        for (HANDLE h : child_ends) CloseHandle(h);
        py::raise_winerror(code);
    };
    nul_in = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    if (nul_in != INVALID_HANDLE_VALUE) child_ends.push_back(nul_in);
    if (!CreatePipe(&out_r, &out_w, &sa, 0)) fail(GetLastError());
    child_ends.push_back(out_w);
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    HANDLE child_err = out_w;
    if (!merge_err) {
        if (!CreatePipe(&err_r, &err_w, &sa, 0)) fail(GetLastError());
        child_ends.push_back(err_w);
        SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);
        child_err = err_w;
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = nul_in;
    si.hStdOutput = out_w;
    si.hStdError = child_err;
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
    r.returncode = static_cast<int64_t>(code);
    CloseHandle(pi.hProcess);
    if (out_r) CloseHandle(out_r);
    if (err_r) CloseHandle(err_r);
    return r;
}

// bytes.decode('utf-8', 'replace')
str decode_replace(const std::string& raw) { return py::bytes(raw).decode("utf-8", "replace"); }

// s[-n:]
str tail(const str& s, int64_t n) { return s.slice(-n, s.size()); }

}  // namespace

// ---------------------------------------------------------------------------------------------- Run
str Run::repr() const {
    return py::percent(S("Run(ok=%s, exit=%s, out=%d chars)"), py::repr(ok), py::repr(exit), out.size());
}

str _norm(const str& s) { return s.replace(S("\r\n"), S("\n")).rstrip(S("\n")); }

std::tuple<str, str> compare(const Run& orig, const Run& trans, const py::list<str>& refused) {
    if (py::truthy(refused)) {
        py::list<str> uniq = py::list<str>::from(py::sorted(py::set<str>::from(refused)));
        return {S("refused"), py::join(S("; "), uniq).slice(0, 400)};
    }
    if (!orig.ok || !orig.exit) {
        str why = py::truthy(orig.log) ? orig.log : (py::truthy(orig.err) ? orig.err : S(""));
        return {S("no-original"), tail(why, 400)};
    }
    if (!trans.ok) return {S("no-compile"), trans.log.slice(0, 600)};
    str a = _norm(orig.out), b = _norm(trans.out);
    if (!trans.exit) return {S("divergent"), S("the translation did not finish (timeout)")};
    if (a == b && orig.exit == trans.exit) return {S("equivalent"), py::percent(S("%d lines"), py::truthy(a) ? a.count(S("\n")) + 1 : 0)};
    py::list<str> al = py::split(a, S("\n")), bl = py::split(b, S("\n"));
    int64_t n = py::max(al.size(), bl.size());
    for (int64_t i = 0; i < n; ++i) {
        str x = i < al.size() ? al[i] : S("<end>");
        str y = i < bl.size() ? bl[i] : S("<end>");
        if (x != y)
            return {S("divergent"), S("line ") + py::to_str(i + 1) + S(": original ") + py::repr(x.slice(0, 160)) + S(", translation ") +
                                        py::repr(y.slice(0, 160))};
    }
    return {S("divergent"), S("exit code ") + py::repr(orig.exit) + S(", translation ") + py::repr(trans.exit)};
}

str _fresh(const str& work) {
    if (py::os::path::isdir(work)) py::shutil::rmtree(work, true);
    py::os::makedirs(work, 0777, true);
    return work;
}

// ---------------------------------------------------------------------------------------------- .NET
Run csharp(const py::list<str>& files, const str& work, const Dotnet& dotnet, const py::list<str>& args, int64_t timeout, const str& name,
           const std::optional<str>& main) {
    _fresh(work);
    str dll = py::os::path::join(work, name + S(".dll"));
    str tmp = py::os::path::join(work, S("tmp"));
    auto [ok, log] = dotnet.compile(files, dll, tmp, main);
    if (!ok) return Run(false, std::nullopt, S(""), S(""), _errors(log));
    auto [code, out, err] = dotnet.run(dll, args, timeout, py::os::path::join(work, S("tmp")));
    return Run(true, code, out, err, log);
}

str _errors(const str& log) {
    py::list<str> lines;
    for (const str& l : py::splitlines(log))
        if (l.contains(S(": error "))) lines.append(l);
    str head = py::join(S("\n"), lines.slice(0, 12));
    return py::truthy(head) ? head : tail(log, 800);
}

// ---------------------------------------------------------------------------------------------- the JVM
Run java(const str& src_dir, const str& main, const str& work, const py::list<str>& args, int64_t timeout, const py::list<str>& classpath) {
    auto [javac, jvm] = codebreaker::verify::lanes::java::jdk();
    if (!py::truthy(javac)) return Run(false, std::nullopt, S(""), S(""), S("no JDK"));
    _fresh(work);
    str classes = py::os::path::join(work, S("classes"));
    py::os::makedirs(classes);
    py::list<str> srcs;
    for (const auto& w : py::os::walk(src_dir)) {
        const str& dp = std::get<0>(w);
        for (const str& f : std::get<2>(w))
            if (f.endswith(S(".java"))) srcs.append(py::os::path::join(dp, f));
    }
    str listing = py::os::path::join(work, S("files.txt"));
    {
        py::list<str> quoted;
        for (const str& s : py::sorted(srcs)) quoted.append(S("\"") + s.replace(S("\\"), S("/")) + S("\""));
        auto f = py::open(listing, S("w"), std::optional<str>(S("utf-8")));
        f.write(py::join(S("\n"), quoted));
        f.close();
    }
    py::list<str> cpl{classes};
    cpl.extend(classpath);
    str cp = py::join(py::os::pathsep, cpl);
    Ran r = run_(py::list<str>{javac, S("-nowarn"), S("-encoding"), S("UTF-8"), S("-d"), classes, S("-cp"), cp, S("@") + listing}, true, 600);
    if (r.timed_out) throw py::TimeoutError("Command timed out after 600 seconds");   // (subprocess.TimeoutExpired)
    if (r.returncode != 0) return Run(false, std::nullopt, S(""), S(""), tail(decode_replace(r.out), 1500));
    py::list<str> cmd{jvm, S("-cp"), cp, main};
    cmd.extend(args);
    Ran p = run_(cmd, false, timeout);
    if (p.timed_out) return Run(true, std::nullopt, S(""), S("timeout"));
    return Run(true, p.returncode, decode_replace(p.out), decode_replace(p.err));
}

// ---------------------------------------------------------------------------------------------- C++
Run cpp(const str& cpp_dir, const str& main_class, const str& work, const py::list<str>& args, int64_t timeout, int64_t jobs) {
    str cxx = codebreaker::verify::lanes::java::compiler();
    if (!py::truthy(cxx)) return Run(false, std::nullopt, S(""), S(""), S("no host C++ compiler (settings.json: host_cxx)"));
    _fresh(work);
    str lib = codebreaker::java::probe::library(cxx, cpp_dir, work, std::nullopt, std::optional<int64_t>(jobs));
    if (!py::truthy(lib)) return Run(false, std::nullopt, S(""), S(""), S("the program did not compile into a library"));
    str harness = py::os::path::join(work, S("cb_main.cpp"));
    int64_t k = main_class.rfind(S("."));
    str simple = k < 0 ? main_class : main_class.slice_from(k + 1);
    {
        auto f = py::open(harness, S("w"), std::optional<str>(S("utf-8")));
        f.write(py::percent(S("#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"%s.h\"\n#include \"j2x_init.h\"\n"
                              "int main() {\n\tj2x_initAll();\n\t%s::main(JArray<JavaString >());\n\treturn 0;\n}\n"),
                            simple, simple));
        f.close();
    }
    str exe = py::os::path::join(work, S("program.exe"));
    auto [ok, why, linked] = codebreaker::java::probe::build_native(cxx, cpp_dir, harness, exe, lib);
    (void)linked;
    if (!ok) {
        py::list<str> missing;
        for (const str& f : py::sorted(py::os::listdir(cpp_dir)))
            if (f.endswith(S(".cpp")) && !py::os::path::isfile(py::os::path::join(work, S("probe"), S("_objects"), f.slice(0, f.size() - 4) + S(".o"))))
                missing.append(f);
        return Run(false, std::nullopt, S(""), S(""), why + (py::truthy(missing) ? S("; did not compile: ") + py::join(S(", "), missing) : S("")));
    }
    py::list<str> cmd{exe};
    cmd.extend(args);
    Ran p = run_(cmd, false, timeout, std::optional<str>(work));
    if (p.timed_out) return Run(true, std::nullopt, S(""), S("timeout"));
    return Run(true, p.returncode, decode_replace(p.out), decode_replace(p.err));
}

}  // namespace codebreaker::translate::dotnet::judge

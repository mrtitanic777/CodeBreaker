// codebreaker/translate/dotnet/package.cpp -- typed C++ port of codebreaker/translate/dotnet/__init__.py (see package.hpp).
#include "codebreaker/translate/dotnet/package.hpp"

#define NOMINMAX
#include <windows.h>

#include <fstream>
#include <thread>

#include "py_os.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::translate::dotnet {

using py::str;

const int64_t TIMEOUT = 300;
const py::list<str> STD_INSTALLS{str("C:\\Program Files\\dotnet\\dotnet.exe"), str("/usr/share/dotnet/dotnet"), str("/usr/local/share/dotnet/dotnet")};

// ---- subprocess.run with env, timeout and bytes ---------------------------------------------------------------------
Process run_process(const py::list<str>& args, const std::optional<py::dict<str, str>>& env_, const std::optional<py::bytes>& input,
                    std::optional<double> timeout, bool stderr_to_stdout) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, err_r = nullptr, err_w = nullptr, in_r = nullptr, in_w = nullptr;
    std::vector<HANDLE> to_close;
    auto fail = [&](DWORD code) {
        for (HANDLE h : {out_r, err_r, in_w})
            if (h) CloseHandle(h);
        for (HANDLE h : to_close) CloseHandle(h);
        py::raise_winerror(code);
    };
    if (input) {
        if (!CreatePipe(&in_r, &in_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(in_r);
        SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    } else {
        HANDLE cur = GetStdHandle(STD_INPUT_HANDLE);
        HANDLE d = nullptr;
        if (cur && cur != INVALID_HANDLE_VALUE && DuplicateHandle(GetCurrentProcess(), cur, GetCurrentProcess(), &d, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
            in_r = d;
            to_close.push_back(d);
        }
    }
    if (!CreatePipe(&out_r, &out_w, &sa, 0)) fail(GetLastError());
    to_close.push_back(out_w);
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    HANDLE child_err = out_w;
    if (!stderr_to_stdout) {
        if (!CreatePipe(&err_r, &err_w, &sa, 0)) fail(GetLastError());
        to_close.push_back(err_w);
        SetHandleInformation(err_r, HANDLE_FLAG_INHERIT, 0);
        child_err = err_w;
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = child_err;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = py::subprocess::to_wide(py::subprocess::list2cmdline(args));
    std::wstring block;
    if (env_) {
        for (const auto& kv : env_->items()) {
            block += py::subprocess::to_wide(std::get<0>(kv)) + L"=" + py::subprocess::to_wide(std::get<1>(kv));
            block.push_back(L'\0');
        }
        block.push_back(L'\0');
        if (env_->size() == 0) block.push_back(L'\0');
    }
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, CREATE_UNICODE_ENVIRONMENT, env_ ? block.data() : nullptr, nullptr, &si, &pi);
    DWORD err = GetLastError();
    if (!ok) fail(err);
    CloseHandle(pi.hThread);
    for (HANDLE h : to_close) CloseHandle(h);
    to_close.clear();
    std::string raw_out, raw_err;
    auto drain = [](HANDLE r, std::string& dst) {
        char buf[8192];
        DWORD got = 0;
        while (r && ReadFile(r, buf, sizeof buf, &got, nullptr) && got > 0) dst.append(buf, got);
    };
    std::thread t_out([&] { drain(out_r, raw_out); });
    std::thread t_err([&] { drain(err_r, raw_err); });
    std::thread t_in;
    if (in_w) {
        std::string data = input->raw();
        HANDLE w = in_w;
        in_w = nullptr;
        t_in = std::thread([w, data] {
            size_t off = 0;
            DWORD wrote = 0;
            while (off < data.size() && WriteFile(w, data.data() + off, static_cast<DWORD>(data.size() - off), &wrote, nullptr) && wrote > 0) off += wrote;
            CloseHandle(w);
        });
    }
    DWORD wait_ms = timeout ? static_cast<DWORD>(*timeout * 1000.0) : INFINITE;
    bool timed_out = WaitForSingleObject(pi.hProcess, wait_ms) == WAIT_TIMEOUT;
    if (timed_out) TerminateProcess(pi.hProcess, 1);
    WaitForSingleObject(pi.hProcess, INFINITE);
    t_out.join();
    t_err.join();
    if (t_in.joinable()) t_in.join();
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hProcess);
    if (out_r) CloseHandle(out_r);
    if (err_r) CloseHandle(err_r);
    if (timed_out) {
        std::string t = timeout ? std::to_string(*timeout) : "";
        throw TimeoutExpired("Command timed out after " + t + " seconds");
    }
    Process p;
    p.returncode = static_cast<int64_t>(code);
    p.stdout_ = py::bytes(raw_out);
    p.stderr_ = py::bytes(raw_err);
    return p;
}

// ---- the toolchain ----------------------------------------------------------------------------------------------------------
py::list<int64_t> _ver(const str& s) {
    // tuple(int(x) if x.isdigit() else 0 for x in re.split(r'[.\-]', s)[:3])
    py::list<int64_t> out;
    std::vector<py::str> parts;
    std::u32string cur;
    for (char32_t c : s.u32()) {
        if (c == U'.' || c == U'-') {
            parts.push_back(str(cur));
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(str(cur));
    for (size_t i = 0; i < parts.size() && i < 3; i++) out.append(parts[i].isdigit() ? py::int_parse(parts[i], 10) : 0);
    return out;
}

static bool ver_less(const py::list<int64_t>& a, const py::list<int64_t>& b) {   // tuple comparison
    int64_t n = (std::min)(py::len(a), py::len(b));
    for (int64_t i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return py::len(a) < py::len(b);
}

std::optional<str> exe() {
    auto p = py::os::environ_::get(str("CB_DOTNET"));
    if (p && !p->empty() && py::os::path::isfile(*p)) return *p;
    auto w = py::shutil::which(str("dotnet"));
    if (w && !w->empty()) return w;
    for (const str& s : STD_INSTALLS)
        if (py::os::path::isfile(s)) return s;
    return std::nullopt;
}

std::optional<str> home() {
    auto e = exe();
    return e ? std::optional<str>(py::os::path::dirname(py::os::path::realpath(*e))) : std::nullopt;
}

std::optional<str> _newest(const str& folder, const str& prefix) {
    py::list<str> names;
    try {
        names = py::os::listdir(folder);
    } catch (const py::OSError&) {
        return std::nullopt;
    }
    std::optional<str> best;
    py::list<int64_t> best_ver;
    for (const str& v : names) {
        if (v.size() > 0 && v.slice(0, 1).isdigit() && v.startswith(prefix)) {
            py::list<int64_t> k = _ver(v);
            if (!best || ver_less(best_ver, k)) {   // max(): the first of equals stays
                best = v;
                best_ver = k;
            }
        }
    }
    return best;
}

std::optional<str> sdk() {
    auto h = home();
    std::optional<str> v = h ? _newest(py::os::path::join(*h, str("sdk"))) : std::nullopt;
    return v ? std::optional<str>(py::os::path::join(*h, str("sdk"), *v)) : std::nullopt;
}

std::optional<str> roslyn() {
    auto s = sdk();
    std::optional<str> p = s ? std::optional<str>(py::os::path::join(*s, str("Roslyn"), str("bincore"))) : std::nullopt;
    return (p && py::os::path::isfile(py::os::path::join(*p, str("csc.dll")))) ? p : std::nullopt;
}

std::tuple<std::optional<str>, std::optional<str>> runtime() {
    auto h = home();
    std::optional<str> ref = h ? _newest(py::os::path::join(*h, str("packs"), str("Microsoft.NETCore.App.Ref"))) : std::nullopt;
    if (!ref) return {std::nullopt, std::nullopt};
    str major = py::split(*ref, str("."))[0];
    auto run_ = _newest(py::os::path::join(*h, str("shared"), str("Microsoft.NETCore.App")), major + str("."));
    return {major, run_ ? run_ : ref};   // run or ref
}

std::optional<str> refs() {
    auto h = home();
    std::optional<str> ref = h ? _newest(py::os::path::join(*h, str("packs"), str("Microsoft.NETCore.App.Ref"))) : std::nullopt;
    if (!ref) return std::nullopt;
    str base = py::os::path::join(*h, str("packs"), str("Microsoft.NETCore.App.Ref"), *ref, str("ref"));
    std::optional<str> tfm = py::os::path::isdir(base) ? _newest(base, str("net")) : std::nullopt;
    if (!tfm && py::os::path::isdir(base)) {
        for (const str& d : py::os::listdir(base))
            if (d.startswith(str("net"))) {
                tfm = d;
                break;
            }
    }
    return tfm ? std::optional<str>(py::os::path::join(base, *tfm)) : std::nullopt;
}

bool available() { return exe() && roslyn() && refs(); }

py::dict<str, str> env(const std::optional<str>& tmp) {
    py::dict<str, str> e;
    LPWCH block = GetEnvironmentStringsW();
    for (LPWCH p = block; p && *p; p += wcslen(p) + 1) {
        std::wstring entry(p);
        size_t eq = entry.find(L'=', 1);   // (a name that starts with "=" is os.environ-hidden: "=C:")
        if (entry[0] == L'=' || eq == std::wstring::npos) continue;
        std::wstring k = entry.substr(0, eq);
        for (wchar_t& c : k) c = static_cast<wchar_t>(towupper(c));
        e.set(str(std::u32string(k.begin(), k.end())), str(std::u32string(entry.begin() + eq + 1, entry.end())));
    }
    if (block) FreeEnvironmentStringsW(block);
    // dict(os.environ, DOTNET_CLI_TELEMETRY_OPTOUT='1', ...)
    e.set(str("DOTNET_CLI_TELEMETRY_OPTOUT"), str("1"));
    e.set(str("DOTNET_NOLOGO"), str("1"));
    e.set(str("DOTNET_SKIP_FIRST_TIME_EXPERIENCE"), str("1"));
    e.set(str("DOTNET_CLI_UI_LANGUAGE"), str("en"));
    e.set(str("DOTNET_gcServer"), str("0"));
    if (tmp && !tmp->empty()) {
        py::os::makedirs(*tmp, 0777, true);
        e.set(str("TEMP"), *tmp);
        e.set(str("TMP"), *tmp);
    }
    return e;
}

str runtimeconfig(const str& dll) {
    auto [major, full] = runtime();
    str path = dll.slice(0, dll.size() - 4) + str(".runtimeconfig.json");
    auto text = [](const std::optional<str>& s) { return s ? *s : str("None"); };   // '%s' % None
    std::string body = "{\"runtimeOptions\": {\"tfm\": \"net" + text(major).utf8() + ".0\", \"framework\": {\"name\": \"Microsoft.NETCore.App\", "
                       "\"version\": \"" + text(full).utf8() + "\"}, \"rollForward\": \"LatestPatch\"}}\r\n";   // (text mode writes "\n" as "\r\n")
    std::ofstream f(std::filesystem::u8path(path.utf8()), std::ios::binary);
    if (!f) throw py::FileNotFoundError("[Errno 2] No such file or directory: " + py::repr(path).utf8());   // open(path, 'w')
    f << body;
    return path;
}

std::tuple<bool, str> compile(const py::list<str>& sources, const str& out_dll, const py::list<str>& references, const str& target, bool nullable,
                              bool unsafe, bool warn, const std::optional<str>& tmp, const std::optional<str>& main) {
    py::os::makedirs(py::os::path::dirname(py::os::path::abspath(out_dll)), 0777, true);
    str rdir = refs() ? *refs() : throw py::TypeError("compile: no targeting pack (refs() is None)");
    str rsp = out_dll.slice(0, out_dll.size() - 4) + str(".rsp");
    py::list<str> args;
    for (const char* a : {"-nologo", "-nostdlib+"}) args.append(str(a));
    args.append(str("-target:") + target);
    args.append(str("-out:") + out_dll);
    for (const char* a : {"-langversion:latest", "-deterministic", "-debug-", "-optimize-", "-utf8output", "-checked-"}) args.append(str(a));
    if (nullable) args.append(str("-nullable:enable"));
    if (unsafe) args.append(str("-unsafe+"));
    if (main) args.append(str("-main:") + *main);
    if (!warn) args.append(str("-nowarn:CS8600,CS8601,CS8602,CS8603,CS8604,CS8618,CS8625,CS0168,CS0219,CS0162,CS8321,CS0649,CS0169"));
    for (const str& f : py::sorted(py::os::listdir(rdir)))
        if (f.endswith(str(".dll"))) args.append(str("-r:") + py::os::path::join(rdir, f));
    for (const str& r : references) args.append(str("-r:") + r);
    for (const str& s : sources) args.append(py::os::path::abspath(s));
    {
        std::string text;
        bool first = true;
        for (const str& a : args) {
            if (!first) text += "\r\n";   // ('\n'.join, written in text mode)
            first = false;
            text += a.contains(str(" ")) ? "\"" + a.utf8() + "\"" : a.utf8();
        }
        std::ofstream f(std::filesystem::u8path(rsp.utf8()), std::ios::binary);
        f << text;
    }
    py::list<str> cmd{*exe(), py::os::path::join(*roslyn(), str("csc.dll")), str("-noconfig"), str("@") + rsp};
    Process r = run_process(cmd, env(tmp), std::nullopt, static_cast<double>(TIMEOUT), true);
    py::os::remove(rsp);
    str log = r.stdout_.decode("utf-8", "replace");
    bool ok = r.returncode == 0 && py::os::path::isfile(out_dll);
    if (ok && target == str("exe")) runtimeconfig(out_dll);
    return {ok, log};
}

std::tuple<std::optional<int64_t>, str, str> run(const str& dll, const py::list<str>& args, double timeout, const std::optional<py::bytes>& stdin_,
                                                  const std::optional<str>& tmp) {
    py::list<str> cmd{*exe(), dll};
    for (const str& a : args) cmd.append(a);
    try {
        Process r = run_process(cmd, env(tmp), stdin_, timeout, false);
        return {r.returncode, r.stdout_.decode("utf-8", "replace"), r.stderr_.decode("utf-8", "replace")};
    } catch (const TimeoutExpired&) {
        return {std::nullopt, str(""), str("timeout")};
    }
}

}  // namespace codebreaker::translate::dotnet

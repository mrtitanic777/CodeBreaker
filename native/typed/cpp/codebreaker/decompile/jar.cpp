// codebreaker/decompile/jar.cpp -- typed C++ from codebreaker.decompile.jar (see jar.hpp).

#include "codebreaker/decompile/jar.hpp"

#include "py_settings.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace codebreaker::decompile::jar {

namespace {

using py::str;

// ---- a small subprocess.Popen for this module (the runtime has none) ----

// subprocess.list2cmdline: the command line CreateProcess is given for an argument list
std::wstring list2cmdline(const py::list<str>& args) {
    std::u32string out;
    bool first = true;
    for (const str& arg : args) {
        const std::u32string& a = arg.u32();
        if (!first) out += U' ';
        first = false;
        bool needquote = a.empty() || a.find(U' ') != std::u32string::npos || a.find(U'\t') != std::u32string::npos;
        std::u32string result;
        if (needquote) result += U'"';
        int bs = 0;
        for (char32_t c : a) {
            if (c == U'\\') {
                bs++;
            } else if (c == U'"') {
                result.append(static_cast<size_t>(bs) * 2, U'\\');
                bs = 0;
                result += U"\\\"";
            } else {
                if (bs) {
                    result.append(static_cast<size_t>(bs), U'\\');
                    bs = 0;
                }
                result += c;
            }
        }
        if (bs) result.append(static_cast<size_t>(bs), U'\\');
        if (needquote) {
            result.append(static_cast<size_t>(bs), U'\\');
            result += U'"';
        }
        out += result;
    }
    std::string u8 = str(out).utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), w.data(), n);
    return w;
}

std::wstring wide(const str& s) {
    std::string u8 = s.utf8();
    int n = MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), nullptr, 0);
    std::wstring w(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), w.data(), n);
    return w;
}

[[noreturn]] void raise_win(DWORD code) {
    std::string msg = "[WinError " + std::to_string(code) + "] the process could not be started";
    if (code == 2 || code == 3) throw py::FileNotFoundError(msg);
    if (code == 5) throw py::PermissionError(msg);
    if (code == 267) throw py::NotADirectoryError(msg);
    throw py::OSError(msg);
}

struct Process {
    HANDLE proc = nullptr;
    HANDLE in = nullptr;    // the parent's end of the child's stdin (null: the child's is the null device)
    HANDLE out = nullptr;   // the parent's end of the child's stdout+stderr
    ~Process() {
        if (in) CloseHandle(in);
        if (out) CloseHandle(out);
        if (proc) CloseHandle(proc);
    }
};

// subprocess.Popen(cmd, cwd=cwd, stdin=PIPE|DEVNULL, stdout=PIPE, stderr=STDOUT, creationflags=...)
void spawn(Process& p, const py::list<str>& cmd, const str& cwd, bool pipe_stdin, DWORD flags) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE out_r = nullptr, out_w = nullptr, in_r = nullptr, in_w = nullptr, nul = INVALID_HANDLE_VALUE;
    if (!CreatePipe(&out_r, &out_w, &sa, 0)) raise_win(GetLastError());
    SetHandleInformation(out_r, HANDLE_FLAG_INHERIT, 0);
    if (pipe_stdin) {
        if (!CreatePipe(&in_r, &in_w, &sa, 0)) raise_win(GetLastError());
        SetHandleInformation(in_w, HANDLE_FLAG_INHERIT, 0);
    } else {
        nul = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
        in_r = nul;
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in_r;
    si.hStdOutput = out_w;
    si.hStdError = out_w;
    PROCESS_INFORMATION pi{};
    std::wstring line = list2cmdline(cmd);
    std::wstring wcwd = wide(cwd);
    BOOL ok = CreateProcessW(nullptr, line.data(), nullptr, nullptr, TRUE, flags, nullptr, wcwd.c_str(), &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(out_w);
    if (in_r) CloseHandle(in_r);
    if (!ok) {
        CloseHandle(out_r);
        if (in_w) CloseHandle(in_w);
        raise_win(err);
    }
    CloseHandle(pi.hThread);
    p.proc = pi.hProcess;
    p.in = in_w;
    p.out = out_r;
}

}  // namespace

const py::re::Pattern _MANGLED_PARAM = py::re::compile(str("(\\w+(?:\\[\\])+)\\s+([^\\s,()]*[;\\[][^\\s,()]*?)(\\d+)(?=\\s*[,)])"));
const py::re::Pattern _VF_WAS = py::re::compile(str("/\\* \\$VF was: [^*]*\\*/"));

str backend_jar() { return py::settings::get_str("retromcp"); }

std::optional<str> find_java(const std::optional<str>& explicit_) {
    str exe("java.exe");                                    // (os.name == 'nt')
    str ex = (explicit_ && !explicit_->empty()) ? *explicit_ : py::settings::get_str("java");
    if (!ex.empty() && py::os::path::isfile(ex)) return ex;
    std::optional<str> jh = py::os::environ_::get(str("JAVA_HOME"));
    if (jh && !jh->empty()) {
        str cand = py::os::path::join(*jh, str("bin"), exe);
        if (py::os::path::isfile(cand)) return cand;
    }
    return py::shutil::which(str("java"));
}

bool available() {
    std::optional<str> j = find_java();
    return j.has_value() && !j->empty() && py::os::path::isfile(backend_jar());
}

std::optional<str> _find_src(const str& workspace) {
    std::optional<str> best;
    int64_t best_n = 0;
    for (const auto& w : py::os::walk(workspace)) {
        const str& root = std::get<0>(w);
        if (py::os::path::basename(root) == str("src")) {
            int64_t n = 0;
            for (const auto& w2 : py::os::walk(root)) {
                for (const str& f : std::get<2>(w2))
                    if (f.endswith(str(".java"))) n++;
            }
            if (n > best_n) {
                best = root;
                best_n = n;
            }
        }
    }
    return best_n > 0 ? best : std::nullopt;
}

std::tuple<int64_t, py::list<str>> _run(const str& java, const str& jar, const py::list<str>& args, const str& cwd,
                                        const std::optional<OnOutput>& on_output, const std::optional<str>& stdin_text) {
    py::list<str> cmd{java, str("-jar"), jar};
    cmd.extend(args);
    if (on_output) {
        py::list<str> shown{str("java"), str("-jar"), py::os::path::basename(jar)};
        shown.extend(args);
        (*on_output)(str("$ ") + py::join(str(" "), shown));
    }
    // In a windowed (console=False) PyInstaller build the parent has no console; CREATE_NO_WINDOW
    // gives the java child its own (hidden) one so its std handles are valid and no window flashes.
    DWORD creationflags = CREATE_NO_WINDOW;                 // (os.name == 'nt')
    std::optional<py::file> logf;
    try {
        logf = py::open(py::os::path::join(cwd, str("codebreaker-decompile.log")), str("a"), str("utf-8"), str("replace"));
        logf->write(str("\n$ ") + py::join(str(" "), cmd) + str("\n"));
    } catch (const py::IOError&) {
        logf = std::nullopt;
    } catch (const py::OSError&) {
        logf = std::nullopt;
    }
    Process proc;
    spawn(proc, cmd, cwd, stdin_text.has_value(), creationflags);
    // RetroMCP's setup task prompts (Scanner.nextLine) when it can't resolve the version from the arg.
    // Pre-feed the answer so it doesn't hang / crash on missing stdin; closing it makes an unknown
    // version fail fast (EOF) instead of looping forever.
    if (stdin_text) {
        // (a text-mode pipe: "\n" is written as the platform's line end, in the locale's codec)
        std::string data = stdin_text->replace(str("\n"), str("\r\n")).encode("cp1252").raw();
        DWORD wrote = 0;
        WriteFile(proc.in, data.data(), static_cast<DWORD>(data.size()), &wrote, nullptr);   // (a write that fails: passed over)
        CloseHandle(proc.in);
        proc.in = nullptr;
    }
    py::list<str> saw;
    auto line_out = [&](const std::u32string& text) {
        str line{std::u32string(text)};
        saw.append(line);
        if (logf) {
            try {
                logf->write(line + str("\n"));
            } catch (const py::IOError&) {
            } catch (const py::OSError&) {
            }
        }
        if (on_output) (*on_output)(line);
    };
    // for line in proc.stdout: -- a text-mode pipe, universal newlines ("\n", "\r" and "\r\n" end a line)
    std::u32string pending;
    char buf[4096];
    for (;;) {
        DWORD got = 0;
        if (!ReadFile(proc.out, buf, sizeof buf, &got, nullptr) || got == 0) break;
        pending += py::bytes(buf, got).decode("cp1252").u32();
        size_t start = 0, i = 0;
        while (i < pending.size()) {
            char32_t c = pending[i];
            if (c == U'\n') {
                line_out(pending.substr(start, i - start));
                start = ++i;
            } else if (c == U'\r') {
                if (i + 1 >= pending.size()) break;         // (a "\n" may follow in the next read)
                line_out(pending.substr(start, i - start));
                i += (pending[i + 1] == U'\n') ? 2 : 1;
                start = i;
            } else {
                i++;
            }
        }
        pending.erase(0, start);
    }
    if (!pending.empty()) {
        if (pending.back() == U'\r') pending.pop_back();    // (a lone "\r" at the end is a line end)
        line_out(pending);
    }
    WaitForSingleObject(proc.proc, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(proc.proc, &code);
    int64_t returncode = static_cast<int64_t>(code);
    if (logf) {
        try {
            logf->write(py::percent(str("[exit %d]\n"), returncode));
            logf->close();
        } catch (const py::IOError&) {
        } catch (const py::OSError&) {
        }
    }
    return std::make_tuple(returncode, saw);
}

str decompile(const str& version, const str& workspace_in, const std::optional<str>& java_in, const std::optional<OnOutput>& on_output,
              bool setup) {
    std::optional<str> java = find_java(java_in);
    if (!java || java->empty()) {
        throw DecompileError(
            "Java runtime not found. Install a JRE/JDK (8+) and put `java` on PATH or set JAVA_HOME. "
            "Only the decompile backend needs Java; the C++ transpiler does not.");
    }
    str jar = backend_jar();
    if (!py::os::path::isfile(jar)) throw DecompileError(py::percent(str("Bundled RetroMCP backend jar not found at: %s"), jar));

    str workspace = py::os::path::abspath(workspace_in);
    py::os::makedirs(workspace, 0777, true);

    if (setup) {
        if (on_output)
            (*on_output)(py::percent(str("[decompile] setting up Minecraft %s (downloads version + mappings)..."), version));
        // Feed the version through stdin too: TaskSetup prompts for it if the arg doesn't resolve.
        auto r = _run(*java, jar, py::list<str>{str("setup"), py::percent(str("setup=%s"), version)}, workspace, on_output,
                      version + str("\n"));
        int64_t rc = std::get<0>(r);
        const py::list<str>& out = std::get<1>(r);
        if (rc != 0) {
            str joined = py::join(str("\n"), out);
            if (joined.contains(str("NoSuchElementException")) || joined.lower().contains(str("selectversion")) ||
                joined.lower().contains(str("select version"))) {
                throw DecompileError(py::percent(
                    str("Minecraft version '%s' was not found in the configured version manifest. "
                        "Check the version id (e.g. a1.2.6, b1.6.6, c0.30-c-1900)."),
                    version));
            }
            throw DecompileError(py::percent(str("RetroMCP setup failed for version '%s' (exit code %d)."), version, rc));
        }
    }

    if (on_output) (*on_output)(str("[decompile] decompiling with Fernflower..."));
    auto r2 = _run(*java, jar, py::list<str>{str("decompile")}, workspace, on_output);
    int64_t rc = std::get<0>(r2);
    if (rc != 0) throw DecompileError(py::percent(str("RetroMCP decompile failed (exit code %d)."), rc));

    std::optional<str> src = _find_src(workspace);
    if (!src) throw DecompileError(py::percent(str("Decompile produced no Java source under %s."), workspace));
    int64_t fixed = repair_array_params(*src);
    if (on_output) {
        int64_t n = 0;
        for (const auto& w : py::os::walk(*src))
            for (const str& f : std::get<2>(w))
                if (f.endswith(str(".java"))) n++;
        (*on_output)(py::percent(str("[decompile] done: %d .java files at %s"), n, *src));
        if (fixed) (*on_output)(py::percent(str("[decompile] repaired %d mangled array parameter name(s)"), fixed));
    }
    return *src;
}

int64_t repair_array_params(const str& src) {
    int64_t count = 0;
    for (const auto& w : py::os::walk(src)) {
        const str& dp = std::get<0>(w);
        for (const str& f : std::get<2>(w)) {
            if (!f.endswith(str(".java"))) continue;
            str p = py::os::path::join(dp, f);
            str text;
            {
                py::file fh = py::open(p, str("r"), str("utf-8"), str("replace"));
                text = fh.read_text();
            }
            auto sub = _MANGLED_PARAM.subn(
                [](const py::re::Match& m) { return py::percent(str("%s var%s"), m.group(1), m.group(3)); }, text);
            str neu = std::get<0>(sub);
            int64_t n = std::get<1>(sub);
            if (n) {
                neu = _VF_WAS.sub(str(""), neu);
                {
                    py::file fh = py::open(p, str("w"), str("utf-8"));
                    fh.write(neu);
                }
                count += n;
            }
        }
    }
    return count;
}

}  // namespace codebreaker::decompile::jar

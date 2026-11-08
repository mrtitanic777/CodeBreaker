// codebreaker/java/console/worklist.cpp -- C++17 port of codebreaker.java.console.worklist (see worklist.hpp).

#include "codebreaker/java/console/worklist.hpp"

#include "py_settings.hpp"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace codebreaker::java::console::worklist {

const py::re::Pattern& _ERR() {
    static const py::re::Pattern p = py::re::compile(py::str(": error:"));
    return p;
}

const py::re::Pattern& _CAT() {
    static const py::re::Pattern p = py::re::compile(py::str("error: (.*)"));
    return p;
}

py::str _category(const py::str& text) {
    // collections.Counter: counts, in the order the keys first appeared
    py::dict<py::str, int64_t> cats;
    for (const py::str& line : py::splitlines(text)) {
        std::optional<py::re::Match> m = _CAT().search(line);
        if (m) {
            py::str c = py::re::sub(py::str("'[^']*'"), py::str("X"), m->group(1));
            cats[py::re::sub(py::str("\\d+"), py::str("N"), c).slice_to(60)] += 1;
        }
    }
    if (cats.empty()) {
        return py::str("(unknown)");
    }
    // cats.most_common(1)[0][0]: the first key with the highest count
    std::optional<py::str> best;
    int64_t best_n = 0;
    for (const auto& [key, n] : cats.items()) {
        if (!best || n > best_n) {
            best = key;
            best_n = n;
        }
    }
    return *best;
}

namespace {

// glob.glob(os.path.join(out_dir, '*.cpp')): the names there ending .cpp (any case, as on Windows),
// not hidden (a leading dot), directories too; [] where the directory cannot be listed.
// (An out_dir holding glob wildcards -- * ? [ -- is taken literally here.)
py::list<py::str> glob_cpp(const py::str& out_dir) {
    py::str dirname = std::get<0>(py::os::path::split(py::os::path::join(out_dir, py::str("*.cpp"))));
    py::list<py::str> out;
    py::list<py::str> names;
    try {
        names = py::os::listdir(dirname.empty() ? py::str(".") : dirname);
    } catch (const py::OSError&) {
        return out;
    }
    for (const py::str& name : names) {
        if (name.startswith(py::str(".")) || !name.lower().endswith(py::str(".cpp"))) {
            continue;
        }
        out.append(py::os::path::join(dirname, name));
    }
    return out;
}

std::wstring to_wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            c -= 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (c >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (c & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// subprocess.list2cmdline
std::wstring list2cmdline(const py::list<py::str>& args) {
    std::wstring result;
    bool first = true;
    for (const py::str& a : args) {
        std::wstring arg = to_wide(a);
        if (!first) {
            result.push_back(L' ');
        }
        first = false;
        bool needquote = arg.empty() || arg.find(L' ') != std::wstring::npos || arg.find(L'\t') != std::wstring::npos;
        if (needquote) {
            result.push_back(L'"');
        }
        size_t bs = 0;
        for (wchar_t c : arg) {
            if (c == L'\\') {
                ++bs;
            } else if (c == L'"') {
                result.append(bs * 2, L'\\');
                bs = 0;
                result.append(L"\\\"");
            } else {
                result.append(bs, L'\\');
                bs = 0;
                result.push_back(c);
            }
        }
        result.append(bs, L'\\');
        if (needquote) {
            result.append(bs, L'\\');
            result.push_back(L'"');
        }
    }
    return result;
}

// subprocess.run(args, stderr=PIPE, stdout=DEVNULL).stderr -- the child's stdin is ours.
py::bytes run_stderr(const py::list<py::str>& args) {
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) {
        throw py::OSError("CreatePipe failed");
    }
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    HANDLE nul_out = CreateFileW(L"NUL", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE child_in = nullptr;
    if (in == nullptr || in == INVALID_HANDLE_VALUE ||
        !DuplicateHandle(GetCurrentProcess(), in, GetCurrentProcess(), &child_in, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
        child_in = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, nullptr);
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = child_in;
    si.hStdOutput = nul_out;
    si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    std::wstring cmd = list2cmdline(args);
    BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    DWORD err = ok ? 0 : GetLastError();
    CloseHandle(wr);
    CloseHandle(nul_out);
    CloseHandle(child_in);
    if (!ok) {
        CloseHandle(rd);
        std::string msg = "[WinError " + std::to_string(err) + "]";
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
            throw py::FileNotFoundError(msg);
        }
        if (err == ERROR_ACCESS_DENIED) {
            throw py::PermissionError(msg);
        }
        throw py::OSError(msg);
    }
    std::string data;
    char buf[65536];
    DWORD got = 0;
    while (ReadFile(rd, buf, sizeof buf, &got, nullptr) && got > 0) {
        data.append(buf, got);
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(rd);
    return py::bytes(data);
}

}  // namespace

int64_t run(const py::str& out_dir, const std::optional<py::str>& cc_arg, const py::str& std_,
            const std::optional<py::str>& worklist_arg) {
    py::str cc = cc_arg ? *cc_arg : py::str();
    if (cc.empty()) {   // `if not cc`
        // from codebreaker.base import settings
        py::str host = py::settings::get_str(py::str("host_cxx"));
        cc = host.empty() ? py::str("g++") : host;   // settings.get('host_cxx') or 'g++'
    }
    py::str worklist_path = worklist_arg ? *worklist_arg : py::str("WORKLIST.md");

    py::list<py::str> cpps = py::sorted(glob_cpp(out_dir));
    std::vector<std::tuple<int64_t, py::str, py::str>> rows;
    int64_t clean = 0;
    for (const py::str& cpp : cpps) {
        py::list<py::str> argv = {cc, py::str("-std=") + std_, py::str("-fsyntax-only"), py::str("-I") + out_dir, cpp};
        py::bytes r_stderr = run_stderr(argv);
        py::str err = r_stderr.decode("utf-8", "replace");
        int64_t n = 0;
        for (const py::str& l : py::splitlines(err)) {
            if (_ERR().search(l)) {
                n += 1;
            }
        }
        if (n == 0) {
            clean += 1;
            continue;
        }
        rows.emplace_back(n, py::os::path::basename(cpp), _category(err));
    }
    std::sort(rows.begin(), rows.end());

    int64_t total = py::len(cpps);
    py::list<py::str> L = {
        py::percent("# Override worklist (%s)", out_dir),
        py::str(""),
        py::percent("%d / %d clean; %d failing. Sorted by error count (fewest first).", clean, total,
                    static_cast<int64_t>(rows.size())),
        py::percent("Regenerate: `python -m codebreaker java worklist %s`", out_dir),
        py::str(""),
        py::str("| errs | file | dominant error |"),
        py::str("|---:|---|---|"),
    };
    for (const auto& [n, file, category] : rows) {
        L.append(py::percent("| %d | %s | %s |", n, file, category));
    }
    {
        py::file f = py::open(worklist_path, py::str("w"), py::str("utf-8"));
        f.write(py::join(py::str("\n"), L) + py::str("\n"));
    }
    py::print(py::percent("clean=%d/%d  failing=%d  -> %s", clean, total, static_cast<int64_t>(rows.size()), worklist_path));
    return 0;
}

}  // namespace codebreaker::java::console::worklist

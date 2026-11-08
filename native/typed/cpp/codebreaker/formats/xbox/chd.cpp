// codebreaker/formats/xbox/chd.cpp -- C++ port of codebreaker.formats.xbox.chd (see the header).

#include "codebreaker/formats/xbox/chd.hpp"

#include "py_settings.hpp"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace codebreaker::formats::xbox::chd {

const py::str DESCRIPTION = "CHD (MAME) Xbox disc images: native header info + chdman-driven chd<->iso/verify";

const py::bytes CHD_MAGIC = py::bytes("MComprHD");

// ── native CHD v5 header ───────────────────────────────────────────────
Header read_header(const py::str& path) {
    py::bytes head;
    {
        py::file f = py::open_rb(path);
        head = f.read(124);
    }
    if (head.slice(0, 8) != CHD_MAGIC) {
        throw ChdError("not a CHD file (bad magic)");
    }
    auto [hlen, version] = py::unpack_from<int64_t, int64_t>(">II", head, 8);
    if (version != 5) {
        throw ChdError(py::percent(py::str("CHD v%d not supported here (chdman copy can upgrade to v5)"), version));
    }
    auto comps = py::unpack_from<int64_t, int64_t, int64_t, int64_t>(">4I", head, 16);
    auto [logicalbytes, mapoffset, metaoffset] = py::unpack_from<int64_t, int64_t, int64_t>(">QQQ", head, 32);
    auto [hunkbytes, unitbytes] = py::unpack_from<int64_t, int64_t>(">II", head, 56);

    auto tag = [](int64_t v) -> py::str {
        if (v == 0) {
            return py::str("none");
        }
        std::string four;
        four.push_back(static_cast<char>((v >> 24) & 0xFF));
        four.push_back(static_cast<char>((v >> 16) & 0xFF));
        four.push_back(static_cast<char>((v >> 8) & 0xFF));
        four.push_back(static_cast<char>(v & 0xFF));
        return py::bytes(four).decode("latin-1");
    };

    Header h;
    h.version = version;
    h.header_len = hlen;
    h.logical_bytes = logicalbytes;
    h.hunk_bytes = hunkbytes;
    h.unit_bytes = unitbytes;
    for (int64_t c : {std::get<0>(comps), std::get<1>(comps), std::get<2>(comps), std::get<3>(comps)}) {
        if (c) {
            h.compressors.append(tag(c));
        }
    }
    h.meta_offset = metaoffset;
    h.map_offset = mapoffset;
    return h;
}

// ── chdman driver ──────────────────────────────────────────────────────

// settings.tool(key, name=None): the program settings.json (or CB_<KEY>) names under `key`, else one on
// PATH called `name` (default: `key`). -> its path, or None.
static std::optional<py::str> tool(const py::str& key) {
    py::str p = py::settings::get_str(key);
    if (py::truthy(p) && (py::os::path::exists(p) || py::shutil::which(p))) {
        return p;
    }
    return py::shutil::which(key);
}

py::str _need_chdman() {
    std::optional<py::str> exe = tool(py::str("chdman"));
    if (!exe) {
        throw ChdError(
            "`chdman` not found \xE2\x80\x94 install MAME tools (Linux: mame-tools / "
            "rom-tools; Windows: chdman.exe ships with MAME) and, if it is "
            "not on PATH; set \"chdman\" in settings.json");
    }
    return *exe;
}

#ifdef _WIN32
// subprocess.list2cmdline (including its habit of doubling a quoted argument's trailing backslashes)
static std::u32string list2cmdline_(const py::list<py::str>& argv) {
    std::u32string result;
    for (const py::str& arg : argv) {
        std::u32string bs_buf;
        if (!result.empty()) {
            result.push_back(U' ');
        }
        bool needquote = arg.contains(py::str(" ")) || arg.contains(py::str("\t")) || arg.empty();
        if (needquote) {
            result.push_back(U'"');
        }
        for (char32_t c : arg.u32()) {
            if (c == U'\\') {
                bs_buf.push_back(c);
            } else if (c == U'"') {
                result.append(bs_buf.size() * 2, U'\\');
                bs_buf.clear();
                result += U"\\\"";
            } else {
                result += bs_buf;
                bs_buf.clear();
                result.push_back(c);
            }
        }
        if (!bs_buf.empty()) {
            result += bs_buf;
        }
        if (needquote) {
            result += bs_buf;
            result.push_back(U'"');
        }
    }
    return result;
}

static std::wstring wide_(const std::u32string& s) {
    std::wstring w;
    for (char32_t c : s) {
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (v >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (v & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// the program run with stdout and stderr into one pipe: its exit code and its output, as text
// (cp1252, universal newlines)
static std::pair<int64_t, py::str> run_process_(const py::list<py::str>& argv) {
    std::wstring cmd = wide_(list2cmdline_(argv));
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof sa;
    sa.bInheritHandle = TRUE;
    HANDLE rd = nullptr, wr = nullptr;
    if (!CreatePipe(&rd, &wr, &sa, 0)) {
        throw py::OSError("CreatePipe failed");
    }
    SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in && in != INVALID_HANDLE_VALUE) {
        SetHandleInformation(in, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
    }
    STARTUPINFOW si{};
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = in;
    si.hStdOutput = wr;
    si.hStdError = wr;
    PROCESS_INFORMATION pi{};
    BOOL ok = CreateProcessW(nullptr, &cmd[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);
    DWORD err = GetLastError();
    CloseHandle(wr);
    if (!ok) {
        CloseHandle(rd);
        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
            throw py::FileNotFoundError("[WinError " + std::to_string(err) + "] The system cannot find the file specified");
        }
        if (err == ERROR_ACCESS_DENIED) {
            throw py::PermissionError("[WinError 5] Access is denied");
        }
        throw py::OSError("[WinError " + std::to_string(err) + "] CreateProcess failed");
    }
    std::string raw;
    char buf[4096];
    DWORD got = 0;
    while (ReadFile(rd, buf, sizeof buf, &got, nullptr) && got > 0) {
        raw.append(buf, got);
    }
    CloseHandle(rd);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD code = 0;
    GetExitCodeProcess(pi.hProcess, &code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    py::str text = py::bytes(raw).decode("cp1252");
    std::u32string out;
    const std::u32string& u = text.u32();
    for (size_t i = 0; i < u.size(); ++i) {
        if (u[i] == U'\r') {
            out.push_back(U'\n');
            if (i + 1 < u.size() && u[i + 1] == U'\n') {
                ++i;
            }
        } else {
            out.push_back(u[i]);
        }
    }
    return {static_cast<int64_t>(code), py::str(out)};
}
#endif

py::str _run(const py::list<py::str>& argv) {
#ifdef _WIN32
    auto [returncode, stdout_text] = run_process_(argv);
    if (returncode != 0) {
        throw ChdError(py::percent(py::str("chdman failed:\n%s"), py::or_(stdout_text, py::str("")).strip()));
    }
    return stdout_text;
#else
    (void)argv;
    throw py::NotImplementedError("chd._run: running a program is implemented for Windows only");
#endif
}

// chd -> ISO (chdman extractdvd).
int64_t extract(const py::str& chd_path, const py::str& out_iso) {
    _run({_need_chdman(), py::str("extractdvd"), py::str("-i"), chd_path, py::str("-o"), out_iso, py::str("-f")});
    if (!py::os::path::exists(out_iso) || py::os::path::getsize(out_iso) == 0) {
        throw ChdError("chdman reported success but wrote no output");
    }
    return py::os::path::getsize(out_iso);
}

// ISO -> chd (chdman createdvd).
int64_t build(const py::str& iso_path, const py::str& out_chd) {
    if (py::os::path::exists(out_chd)) {
        py::os::remove(out_chd);
    }
    _run({_need_chdman(), py::str("createdvd"), py::str("-i"), iso_path, py::str("-o"), out_chd});
    if (!py::os::path::exists(out_chd) || py::os::path::getsize(out_chd) == 0) {
        throw ChdError("chdman reported success but wrote no output");
    }
    return py::os::path::getsize(out_chd);
}

py::str verify(const py::str& chd_path) {
    return _run({_need_chdman(), py::str("verify"), py::str("-i"), chd_path});
}

// ── CLI ────────────────────────────────────────────────────────────────
void main(const py::list<py::str>& sys_argv) {
    py::list<py::str> a = sys_argv.slice_from(1);
    if (!py::truthy(a) || a[0] == "-h" || a[0] == "--help") {
        py::print(py::str("usage:\n"
                          "  chd info   <file.chd>            native header summary (no chdman)\n"
                          "  chd toiso  <file.chd> <out.iso>  extract to ISO (needs chdman)\n"
                          "  chd tochd  <in.iso>  <out.chd>   build a DVD CHD (needs chdman)\n"
                          "  chd verify <file.chd>            chdman integrity verify"));
        return;
    }
    py::str op = a[0];
    if (op == "info") {
        Header h = read_header(a[1]);
        py::print(py::percent(py::str("CHD v%d: %s"), h.version, a[1]));
        py::print(py::percent(py::str("logical size : %d bytes (%.2f GB)"), h.logical_bytes,
                              py::truediv(h.logical_bytes, py::shl(1, 30))));
        py::print(py::percent(py::str("hunk / unit  : %d / %d bytes"), h.hunk_bytes, h.unit_bytes));
        py::print(py::percent(py::str("compressors  : %s"), py::or_(py::join(py::str(", "), h.compressors), py::str("none"))));
        py::print(py::percent(py::str("on-disk size : %d bytes"), py::os::path::getsize(a[1])));
        return;
    }
    if (op == "toiso" && py::len(a) >= 3) {
        py::print(py::percent(py::str("extracted %d bytes -> %s"), extract(a[1], a[2]), a[2]));
        return;
    }
    if (op == "tochd" && py::len(a) >= 3) {
        py::print(py::percent(py::str("built CHD (%d bytes) -> %s"), build(a[1], a[2]), a[2]));
        return;
    }
    if (op == "verify") {
        py::print(verify(a[1]).strip());
        return;
    }
    py::print(py::str("unknown/short command; run 'chd' for usage"));
    throw SystemExit{2};
}

}  // namespace codebreaker::formats::xbox::chd

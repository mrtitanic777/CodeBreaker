// codebreaker/exact/emu/native_dosexec.cpp -- C++ port of codebreaker.exact.emu.native_dosexec
// (see native_dosexec.hpp).  Calls the executive of native/dosexec (namespace cbd) directly.

#include "codebreaker/exact/emu/native_dosexec.hpp"

#include <cstdlib>
#include <mutex>
#include <string>

#include "dosexec.h"   // native/dosexec: cbd::Run, cbd::MemFS, cbd::PyErr ...
#include "cbdos.h"     // CBD_* status codes
#include "py_settings.hpp"

namespace codebreaker::exact::emu::native_dosexec {

namespace {

// _LIB = [the DLL, or None when it cannot be used here]; _WHY
bool g_loaded = false;
bool g_tried = false;
py::list<py::str> g_why;
std::recursive_mutex g_mu;   // (the first use from two threads loads the DLL once)

// raise struct.error(msg) / io.UnsupportedOperation(msg)
struct UnsupportedOperation : py::OSError {
    explicit UnsupportedOperation(const std::string& m) : py::OSError(m) {}
    const char* type() const override { return "UnsupportedOperation"; }
};

std::wstring to_wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            w.push_back(static_cast<wchar_t>(0xD800 + ((c - 0x10000) >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + ((c - 0x10000) & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

py::str from_wide(const std::wstring& w) {
    std::u32string u;
    for (size_t i = 0; i < w.size(); ++i) {
        char32_t c = static_cast<char16_t>(w[i]);
        if (c >= 0xD800 && c < 0xDC00 && i + 1 < w.size()) {
            char32_t lo = static_cast<char16_t>(w[i + 1]);
            if (lo >= 0xDC00 && lo < 0xE000) {
                c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00);
                ++i;
            }
        }
        u.push_back(c);
    }
    return py::str(u);
}

cbd::Bytes to_vec(const py::bytes& b) { return cbd::Bytes(b.raw().begin(), b.raw().end()); }

std::string repr_filename(const py::str& s) { return py::repr(s).utf8(); }

// OSError(errno, ...) is the subclass of that errno (FileNotFoundError ...), and OSError(None, msg, filename,
// winerror) the subclass of the errno CPython maps the Windows error to.
int errno_of(int err_no) {
    if (err_no >= 0) return err_no;
    switch (-err_no) {   // (CPython's winerror_to_errno, the ones a file call meets)
        case 2: case 3: case 15: case 18: case 53: case 67: case 206: return 2;                   // ENOENT
        case 5: case 19: case 32: case 33: case 65: case 82: case 108: case 109: return 13;       // EACCES
        case 80: case 183: return 17;                                                              // EEXIST
        case 267: return 20;                                                                       // ENOTDIR
        default: return 22;                                                                        // EINVAL
    }
}

// An OSError the way Python words it: OSError(err_no, msg, filename) is "[Errno N] msg: 'filename'",
// OSError(None, msg, filename, winerror) is "[WinError N] msg: 'filename'".
[[noreturn]] void raise_os(const cbd::PyErr& r, const std::string& msg) {
    if (r.err_no == 0) throw py::FileNotFoundError(msg);
    std::string text;
    if (r.err_no < 0) text = "[WinError " + std::to_string(-r.err_no) + "] " + msg;
    else text = "[Errno " + std::to_string(r.err_no) + "] " + msg;
    if (!r.filename.empty()) text += ": " + repr_filename(from_wide(r.filename));
    switch (errno_of(r.err_no)) {
        case 1:
        case 13: throw py::PermissionError(text);
        case 2: throw py::FileNotFoundError(text);
        case 17: throw py::FileExistsError(text);
        case 20: throw py::NotADirectoryError(text);
        case 21: throw py::IsADirectoryError(text);
        default: throw py::OSError(text);
    }
}

// _raise(r): the exception the executive reported
[[noreturn]] void raise_(const cbd::PyErr& r) {
    const std::string& msg = r.message;
    switch (r.kind) {
        case CBD_UC: throw dpmi::UcError(msg.empty() ? std::to_string(r.err_no) : msg);
        case CBD_OS: raise_os(r, msg);
        case CBD_STRUCT: throw py::StructError(msg);
        case CBD_UNSUPPORTED: throw UnsupportedOperation(msg);
        case CBD_ASSERT: throw py::AssertionError(msg);
        case CBD_VALUE: throw py::ValueError(msg);
        default: throw py::RuntimeError(msg);   // CBD_RUNTIME, CBD_OTHER
    }
}

bool env_off() {
    auto v = py::os::getenv(py::str("CB_NATIVE_DOSEXEC"), py::str(DEFAULT));
    std::string s = v.lower().utf8();
    return s == "0" || s == "off" || s == "no" || s == "false";
}

}  // namespace

py::str dll_path() {
    return py::os::path::join(py::settings::get_str(py::str("dosexec_bin")), py::str("cbdos.dll"));
}

void load_unicorn(const py::str& dll) {
    std::lock_guard<std::recursive_mutex> lock(g_mu);
    char err[256] = {0};
    std::wstring w = to_wide(py::os::path::abspath(dll));
    g_tried = true;
    if (!cbd::load_unicorn(w.c_str(), err, sizeof err)) {
        g_loaded = false;
        std::string m(err);
        g_why.append(py::str(m));
        throw py::RuntimeError(m);
    }
    g_loaded = true;
}

py::list<py::str> why() {
    std::lock_guard<std::recursive_mutex> lock(g_mu);
    return g_why;
}

// _load(): True when the executive can be used here
static bool load_() {
    std::lock_guard<std::recursive_mutex> lock(g_mu);
    if (g_loaded) return true;
    if (g_tried) return false;
    g_tried = true;
    auto p = py::os::getenv(py::str("CB_UNICORN_DLL"));
    if (!p || p->empty()) {
        g_why.append(py::str("no unicorn.dll path: call load_unicorn or set CB_UNICORN_DLL"));
        return false;
    }
    char err[256] = {0};
    std::wstring w = to_wide(py::os::path::abspath(*p));
    if (!cbd::load_unicorn(w.c_str(), err, sizeof err)) {
        g_why.append(py::str(std::string(err)));
        return false;
    }
    g_loaded = true;
    return true;
}

bool enabled() {
    if (env_off()) return false;
    return load_();
}

dos::Result run(const py::str& exe, const py::str& workdir, const py::str& cmdline, const py::bytes& stdin_,
                int64_t max_blocks, bool trace_calls, const py::bytes& env, std::optional<int64_t> psp,
                std::optional<int64_t> top, bool fast, const dos::MemFS* fs, bool dpmi, double fast_limit) {
    if (!load_()) throw py::RuntimeError(g_why.empty() ? std::string("the C++ DOS executive is not usable") : g_why[-1].utf8());
    py::bytes tail = cmdline.encode("latin1");

    cbd::MemFS h;   // the MemFS handed over (cbd_fs_new / cbd_fs_hide / cbd_fs_put / cbd_fs_gone)
    if (fs != nullptr) {
        for (const py::str& p : fs->hide.vec()) h.hide.push_back(to_wide(p));
        for (const auto& kv : fs->files.items()) {
            const py::bytes& b = std::get<1>(kv).view();
            h.files[to_wide(std::get<0>(kv))] = std::make_shared<cbd::Bytes>(to_vec(b));
        }
        for (const py::str& k : fs->gone) h.gone.insert(to_wide(k));
    }

    cbd::Run r;   // cbd_run: one run
    cbd::PyErr failure{0, 0, "", L""};
    bool failed = false;
    r.exe = to_wide(exe);
    r.workdir = to_wide(workdir);
    r.cmdline = to_vec(tail);
    r.stdin_data = to_vec(stdin_);
    r.env = to_vec(env);
    r.max_blocks = max_blocks;
    r.trace_calls = trace_calls;
    if (psp && *psp) r.PSP = static_cast<int>(*psp);
    if (top && *top) r.TOP = static_cast<int>(*top);
    r.fast = fast;
    r.want_dpmi = dpmi;
    r.fs = fs != nullptr ? &h : nullptr;
    r.S.fast_limit = fast_limit;
    r.S.dpmi_limit = std::strtod(py::os::getenv(py::str("DOSRUN_DPMI_LIMIT"), py::str("300")).utf8().c_str(), nullptr);   // float(...)
    r.S.drive = static_cast<int>(py::int_parse(py::os::getenv(py::str("DOSRUN_DRIVE"), py::str("2")), 10));
    r.S.debug = [] {
        auto d = py::os::getenv(py::str("DOSRUN_DEBUG"));
        return d.has_value() && !d->empty();
    }();
    try {
        try {
            r.go();
        } catch (cbd::PyErr& e) {
            failure = e;
            failed = true;
        }
    } catch (std::exception& e) {   // an internal failure of the executive: RuntimeError
        failure = cbd::PyErr{CBD_OTHER, 0, e.what(), L""};
        failed = true;
    }

    if (fs != nullptr) {   // what the run changed in the MemFS
        for (const std::wstring& k : h.touched) {   // (cbd_fs_changed / cbd_fs_entry, in key order)
            auto it = h.files.find(k);
            py::str key = from_wide(k);
            if (it != h.files.end() && it->second) {
                fs->files.set(key, py::bytearray(it->second->empty() ? "" : reinterpret_cast<const char*>(it->second->data()),
                                                 it->second->size()));
            } else {
                fs->files.pop(key, py::bytearray());
            }
        }
        py::set<py::str> gone;   // fs.gone.clear(); fs.gone.update(...)
        for (const std::wstring& g : h.gone) gone.add(from_wide(g));
        for (const py::str& old : std::vector<py::str>(fs->gone.begin(), fs->gone.end())) fs->gone.discard(old);
        fs->gone.update(gone);
    }

    if (failed) raise_(failure);

    dos::Result out;
    if (r.exit) out.exit = *r.exit;
    out.console = py::bytes(std::string(r.console.begin(), r.console.end()));
    for (uint64_t b : r.blocks) out.blocks.add(static_cast<int64_t>(b));
    for (const auto& kv : r.calls) out.calls[static_cast<int64_t>(kv.first)] = kv.second;
    out.load_seg = r.load;
    out.steps = r.n;
    return out;
}

}  // namespace codebreaker::exact::emu::native_dosexec

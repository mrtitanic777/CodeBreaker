// codebreaker/analyze/native.cpp -- typed C++ port of codebreaker.analyze.native (see the header).

#include "codebreaker/analyze/native.hpp"

#include <cstring>
#include <string>
#include <vector>

#include "py_os.hpp"
#include "py_settings.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace codebreaker::analyze::native {

const int64_t _VERSION = 5;
const py::dict<py::str, int64_t> _POINTER_KINDS = {
    {py::str("abs32"), 0}, {py::str("far"), 1}, {py::str("selector"), 2}, {py::str("offset"), 3}, {py::str("byte"), 4}};

// cbanalyze.dll's entry points (native/analyze/cbanalyze.h)
struct Library {
#ifdef _WIN32
    HMODULE module = nullptr;
#endif
    int (*cba_version)() = nullptr;
    int (*cba_init)(const wchar_t*, char*, int) = nullptr;
    void* (*cba_open)(const unsigned char*, size_t, char*, int) = nullptr;
    int (*cba_ask)(void*, const char*, long long, char**, size_t*) = nullptr;
    int (*cba_names)(void*, const unsigned char*, size_t) = nullptr;
    int (*cba_assemble)(const char*, int, char**, size_t*) = nullptr;
    void (*cba_free)(char*) = nullptr;
    void (*cba_close)(void*) = nullptr;
};

namespace {

// _LIB / _WHY of the Python: the library once loaded (or nullptr), and the reason it is not
bool g_loaded = false;
Library* g_lib = nullptr;
std::vector<py::str> g_why;
std::optional<py::str> g_capstone_dll;

// An error buffer's text: err.value.decode("ascii", "replace")
py::str error_text(const char* buf) {
    std::u32string out;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(buf); *p; ++p) {
        out.push_back(*p < 0x80 ? static_cast<char32_t>(*p) : static_cast<char32_t>(0xFFFD));
    }
    return py::str(out);
}

#ifdef _WIN32
std::wstring wide(const py::str& s) {   // (UTF-16, as ctypes' c_wchar_p)
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

template <class F> void bind(HMODULE m, const char* name, F& slot) {
    FARPROC p = GetProcAddress(m, name);
    if (!p) {
        throw py::OSError(std::string("function '") + name + "' not found");
    }
    slot = reinterpret_cast<F>(p);
}
#endif

}  // namespace

py::str dll_path() {
    return py::os::path::join(py::settings::get_str(py::str("analyze_bin")), py::str("cbanalyze.dll"));
}

void set_capstone_dll(const py::str& path) {
    g_capstone_dll = path;
}

Library* _load() {
    if (g_loaded) {
        return g_lib;
    }
    Library* lib = nullptr;
    try {
        py::str path = dll_path();
        if (!py::os::path::isfile(path)) {
            throw py::OSError(py::percent(py::str("not built: %s (native/analyze/CMakeLists.txt)"), path));
        }
        // import capstone   (no capstone to import: the caller names its dll; not an OSError, so it escapes)
        if (!g_capstone_dll) {
            throw py::Exception("No module named 'capstone'");
        }
#ifdef _WIN32
        std::unique_ptr<Library> fresh(new Library());
        // settings.load_native: the build folder goes on the DLL search path, then the DLL is loaded
        py::str dir = py::os::path::dirname(py::os::path::abspath(path));
        std::wstring wdir = wide(dir);
        std::wstring wpath = wide(path);
        AddDllDirectory(wdir.c_str());
        fresh->module = LoadLibraryExW(wpath.c_str(), nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
        if (!fresh->module) {
            throw py::OSError(py::percent(py::str("could not load %s (error %d)"), path, int64_t(GetLastError())));
        }
        bind(fresh->module, "cba_version", fresh->cba_version);
        if (fresh->cba_version() != _VERSION) {
            FreeLibrary(fresh->module);
            throw py::OSError("cbanalyze.dll is another version: rebuild native/analyze");
        }
        bind(fresh->module, "cba_init", fresh->cba_init);
        bind(fresh->module, "cba_open", fresh->cba_open);
        bind(fresh->module, "cba_ask", fresh->cba_ask);
        bind(fresh->module, "cba_names", fresh->cba_names);
        bind(fresh->module, "cba_assemble", fresh->cba_assemble);
        bind(fresh->module, "cba_free", fresh->cba_free);
        bind(fresh->module, "cba_close", fresh->cba_close);
        char err[256] = {0};
        std::wstring cap = wide(py::os::path::abspath(*g_capstone_dll));
        if (fresh->cba_init(cap.c_str(), err, 256)) {
            throw py::OSError(error_text(err));
        }
        lib = fresh.release();
#else
        throw py::OSError("cbanalyze.dll is a Windows library");
#endif
    } catch (const py::OSError& e) {
        g_why.push_back(py::str(std::string(e.what())));
        lib = nullptr;
    }
    g_lib = lib;
    g_loaded = true;
    return lib;
}

std::optional<py::str> why_not() {
    _load();
    if (!g_why.empty()) {
        return g_why[0];
    }
    return std::nullopt;
}

Library& library() {
    Library* lib = _load();
    if (lib == nullptr) {
        throw py::RuntimeError(py::percent(py::str("cbanalyze.dll: %s"), why_not().value_or(py::str("None"))));
    }
    return *lib;
}

py::bytes _blob(const Image& img) {
    if (img.kind.empty()) {
        throw py::IndexError("string index out of range");
    }
    int64_t kind0 = static_cast<int64_t>(img.kind.u32()[0]);   // ord(img.kind[0])
    py::bytes out = py::pack("<BBBB", kind0, img.bits, int64_t(img.flat ? 1 : 0), int64_t(img.pointers ? 1 : 0));
    out = out + py::pack("<I", py::len(img.spans));
    for (const Span& sp : img.spans) {
        int64_t va = std::get<0>(sp);
        const py::bytes& data = std::get<1>(sp);
        bool exe = std::get<2>(sp);
        bool wr = std::get<3>(sp);
        int64_t init = std::get<5>(sp);
        // (name or "").encode("utf-8", "surrogatepass")
        py::bytes raw = (std::get<4>(sp) && py::truthy(*std::get<4>(sp)) ? *std::get<4>(sp) : py::str("")).encode("utf-8");
        out = out + py::pack("<qIBBqI", va, py::len(data), int64_t(exe ? 1 : 0), int64_t(wr ? 1 : 0), init, py::len(raw)) + raw;
        out = out + data;
    }
    py::list<int64_t> starts = py::sorted(img.names);
    out = out + py::pack("<I", py::len(starts));
    for (int64_t s : starts) {
        out = out + py::pack("<qq", s, img.limits.get(s, s));
    }
    out = out + py::pack("<I", py::len(img.imports));
    for (int64_t a : img.imports) {
        out = out + py::pack("<q", a);
    }
    py::dict<int64_t, Pointer> ptrs = img.pointers ? *img.pointers : py::dict<int64_t, Pointer>();
    out = out + py::pack("<I", py::len(ptrs));
    for (const auto& kv : ptrs.items()) {
        int64_t site = std::get<0>(kv);
        const py::str& kind = std::get<0>(std::get<1>(kv));
        const std::optional<int64_t>& t = std::get<1>(std::get<1>(kv));
        out = out + py::pack("<qBBq", site, _POINTER_KINDS.get(kind, int64_t(4)), int64_t(t ? 1 : 0),
                             t ? *t : int64_t(0));   // t or 0
    }
    out = out + py::pack("<qI", img.data_segment ? *img.data_segment : int64_t(0), py::len(img.roots));
    for (int64_t r : img.roots) {
        out = out + py::pack("<q", r);
    }
    return out;
}

py::bytes _names(const Image& img) {
    py::bytes out;
    auto table = [&](const py::dict<int64_t, py::str>& t) {
        out = out + py::pack("<I", py::len(t));
        for (const auto& kv : t.items()) {
            py::bytes raw = std::get<1>(kv).encode("utf-8");   // name.encode("utf-8", "surrogatepass")
            out = out + py::pack("<qI", std::get<0>(kv), py::len(raw)) + raw;
        }
    };
    table(img.names);
    table(img.imports);
    return out;
}

py::str assemble(const py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, int64_t>>& stores, int64_t least) {
    Library& lib = library();
    std::string text;
    for (const auto& kv : stores.items()) {
        const auto& bo = std::get<0>(kv);
        const auto& vn = std::get<1>(kv);
        text += std::to_string(std::get<0>(bo)) + " " + std::to_string(std::get<1>(bo)) + " " +
                std::to_string(std::get<0>(vn)) + " " + std::to_string(std::get<1>(vn)) + "\n";
    }
    char* out = nullptr;
    size_t size = 0;
    lib.cba_assemble(text.c_str(), static_cast<int>(least), &out, &size);
    // json.loads(ctypes.string_at(out.value, size.value).decode("ascii")) -- the text, here
    std::string answer(out ? out : "", out ? size : 0);
    lib.cba_free(out);
    return py::str(answer);
}

Handle::Handle(const Image& img) {
    Library& lib = library();
    py::bytes blob = _blob(img);
    char err[256] = {0};
    h_ = lib.cba_open(reinterpret_cast<const unsigned char*>(blob.raw().data()), blob.raw().size(), err, 256);
    if (!h_) {
        throw py::RuntimeError(py::percent(py::str("cbanalyze.dll: %s"), error_text(err)));
    }
}

void Handle::names(const Image& img) {
    py::bytes blob = _names(img);
    if (library().cba_names(h_, reinterpret_cast<const unsigned char*>(blob.raw().data()), blob.raw().size())) {
        throw py::RuntimeError("cbanalyze.dll: the names blob is short");
    }
}

std::optional<py::str> Handle::ask(const py::str& question, int64_t arg) {
    Library& lib = library();
    char* out = nullptr;
    size_t size = 0;
    int rc = lib.cba_ask(h_, question.encode("ascii").raw().c_str(), arg, &out, &size);
    if (rc < 0) {
        throw py::ValueError(py::percent(py::str("cbanalyze.dll does not know %r"), question));
    }
    if (rc == 0) {
        return std::nullopt;
    }
    std::string answer(out ? out : "", out ? size : 0);
    lib.cba_free(out);
    return py::str(answer);
}

Handle::~Handle() {
    void* h = h_;
    h_ = nullptr;
    if (h) {
        library().cba_close(h);
    }
}

}  // namespace codebreaker::analyze::native

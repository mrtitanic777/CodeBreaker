// codebreaker/symbols/native.cpp -- typed C++ of codebreaker.symbols.native.
//
// native.py -- codebreaker/symbols/ in C++ (native/symbols: cbsymbols.dll), through ctypes (here: loaded
// directly).
//
// A thin shim: each question goes to the DLL with its text as code points and comes back as JSON,
// so callers keep the very Python values they had.
//
//     native.ask("demangle.parse", name)    the answer (a dict, a str ...), or None for no answer
//
// The switch: CB_NATIVE_SYMBOLS (default 1). The Python it replaced is deleted, so without the
// built DLL these questions fail loudly (why_not() says why).
// Build: native/symbols/CMakeLists.txt (cmake + ninja in a vcvars64 shell).

#include "codebreaker/symbols/native.hpp"

#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <limits>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <process.h>
#endif

namespace codebreaker::symbols::native {

const int64_t _VERSION = 1;
py::list<std::shared_ptr<Library>> _LIB;
py::list<py::str> _WHY;

// ------------------------------------------------------------------------------------------ json.loads
namespace {

class Parser {
  public:
    explicit Parser(const std::u32string& s) : s_(s) {}

    Json document() {
        skip_ws();
        Json v = value();
        skip_ws();
        if (i_ != s_.size()) {
            fail("Extra data");
        }
        return v;
    }

  private:
    const std::u32string& s_;
    size_t i_ = 0;

    [[noreturn]] void fail(const char* what) const {
        throw JSONDecodeError(std::string(what) + ": char " + std::to_string(i_));
    }
    void skip_ws() {
        while (i_ < s_.size() && (s_[i_] == U' ' || s_[i_] == U'\t' || s_[i_] == U'\n' || s_[i_] == U'\r')) {
            ++i_;
        }
    }
    bool take(const char32_t* word) {
        size_t n = std::char_traits<char32_t>::length(word);
        if (s_.compare(i_, n, word) == 0) {
            i_ += n;
            return true;
        }
        return false;
    }

    Json value() {
        if (i_ >= s_.size()) {
            fail("Expecting value");
        }
        char32_t c = s_[i_];
        if (c == U'"') {
            return Json{string()};
        }
        if (c == U'{') {
            return object();
        }
        if (c == U'[') {
            return array();
        }
        if (take(U"null")) {
            return Json{std::monostate()};
        }
        if (take(U"true")) {
            return Json{true};
        }
        if (take(U"false")) {
            return Json{false};
        }
        if (take(U"NaN")) {
            return Json{std::numeric_limits<double>::quiet_NaN()};
        }
        if (take(U"Infinity")) {
            return Json{std::numeric_limits<double>::infinity()};
        }
        if (take(U"-Infinity")) {
            return Json{-std::numeric_limits<double>::infinity()};
        }
        return number();
    }

    static bool digit(char32_t c) { return c >= U'0' && c <= U'9'; }

    // -?(0|[1-9]\d*)(\.\d+)?([eE][-+]?\d+)?
    Json number() {
        size_t start = i_;
        size_t j = i_;
        if (j < s_.size() && s_[j] == U'-') {
            ++j;
        }
        if (j < s_.size() && s_[j] == U'0') {
            ++j;
        } else if (j < s_.size() && s_[j] >= U'1' && s_[j] <= U'9') {
            while (j < s_.size() && digit(s_[j])) {
                ++j;
            }
        } else {
            fail("Expecting value");
        }
        bool is_float = false;
        if (j + 1 < s_.size() && s_[j] == U'.' && digit(s_[j + 1])) {
            is_float = true;
            ++j;
            while (j < s_.size() && digit(s_[j])) {
                ++j;
            }
        }
        if (j < s_.size() && (s_[j] == U'e' || s_[j] == U'E')) {
            size_t k = j + 1;
            if (k < s_.size() && (s_[k] == U'-' || s_[k] == U'+')) {
                ++k;
            }
            if (k < s_.size() && digit(s_[k])) {
                while (k < s_.size() && digit(s_[k])) {
                    ++k;
                }
                is_float = true;
                j = k;
            }
        }
        std::string text;
        for (size_t k = start; k < j; ++k) {
            text += static_cast<char>(s_[k]);
        }
        i_ = j;
        errno = 0;
        if (is_float) {
            return Json{std::strtod(text.c_str(), nullptr)};
        }
        long long v = std::strtoll(text.c_str(), nullptr, 10);
        if (errno == ERANGE) {
            throw py::OverflowError("JSON integer beyond int64");   // (the Python's ints are unbounded)
        }
        return Json{static_cast<int64_t>(v)};
    }

    int hex4(size_t at) const {
        if (at + 4 > s_.size()) {
            return -1;
        }
        int v = 0;
        for (size_t k = at; k < at + 4; ++k) {
            char32_t c = s_[k];
            int d;
            if (c >= U'0' && c <= U'9') {
                d = static_cast<int>(c - U'0');
            } else if (c >= U'a' && c <= U'f') {
                d = static_cast<int>(c - U'a') + 10;
            } else if (c >= U'A' && c <= U'F') {
                d = static_cast<int>(c - U'A') + 10;
            } else {
                return -1;
            }
            v = v * 16 + d;
        }
        return v;
    }

    py::str string() {
        ++i_;   // the opening quote
        std::u32string out;
        for (;;) {
            if (i_ >= s_.size()) {
                fail("Unterminated string starting at");
            }
            char32_t c = s_[i_++];
            if (c == U'"') {
                break;
            }
            if (c < 0x20) {
                --i_;
                fail("Invalid control character at");
            }
            if (c != U'\\') {
                out += c;
                continue;
            }
            if (i_ >= s_.size()) {
                fail("Unterminated string starting at");
            }
            char32_t e = s_[i_++];
            switch (e) {
                case U'"': out += U'"'; break;
                case U'\\': out += U'\\'; break;
                case U'/': out += U'/'; break;
                case U'b': out += U'\b'; break;
                case U'f': out += U'\f'; break;
                case U'n': out += U'\n'; break;
                case U'r': out += U'\r'; break;
                case U't': out += U'\t'; break;
                case U'u': {
                    int u = hex4(i_);
                    if (u < 0) {
                        fail("Invalid \\uXXXX escape");
                    }
                    i_ += 4;
                    if (u >= 0xD800 && u <= 0xDBFF && i_ + 1 < s_.size() && s_[i_] == U'\\' && s_[i_ + 1] == U'u') {
                        int lo = hex4(i_ + 2);
                        if (lo >= 0xDC00 && lo <= 0xDFFF) {
                            u = 0x10000 + (((u - 0xD800) << 10) | (lo - 0xDC00));
                            i_ += 6;
                        }
                    }
                    out += static_cast<char32_t>(u);
                    break;
                }
                default:
                    --i_;
                    fail("Invalid \\escape");
            }
        }
        return py::str(out);
    }

    Json array() {
        ++i_;
        Json::Array items;
        skip_ws();
        if (i_ < s_.size() && s_[i_] == U']') {
            ++i_;
            return Json{items};
        }
        for (;;) {
            skip_ws();
            items.push_back(value());
            skip_ws();
            if (i_ < s_.size() && s_[i_] == U',') {
                ++i_;
                continue;
            }
            if (i_ < s_.size() && s_[i_] == U']') {
                ++i_;
                break;
            }
            fail("Expecting ',' delimiter");
        }
        return Json{items};
    }

    Json object() {
        ++i_;
        Json::Object entries;
        skip_ws();
        if (i_ < s_.size() && s_[i_] == U'}') {
            ++i_;
            return Json{entries};
        }
        for (;;) {
            skip_ws();
            if (i_ >= s_.size() || s_[i_] != U'"') {
                fail("Expecting property name enclosed in double quotes");
            }
            py::str key = string();
            skip_ws();
            if (i_ >= s_.size() || s_[i_] != U':') {
                fail("Expecting ':' delimiter");
            }
            ++i_;
            skip_ws();
            Json v = value();
            bool found = false;
            for (auto& entry : entries) {
                if (entry.first == key) {
                    entry.second = v;
                    found = true;
                    break;
                }
            }
            if (!found) {
                entries.emplace_back(key, v);
            }
            skip_ws();
            if (i_ < s_.size() && s_[i_] == U',') {
                ++i_;
                continue;
            }
            if (i_ < s_.size() && s_[i_] == U'}') {
                ++i_;
                break;
            }
            fail("Expecting ',' delimiter");
        }
        return Json{entries};
    }
};

// ------------------------------------------------------------------------------------------ the Windows side

std::wstring wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            w += static_cast<wchar_t>(0xD800 + (v >> 10));
            w += static_cast<wchar_t>(0xDC00 + (v & 0x3FF));
        } else {
            w += static_cast<wchar_t>(c);
        }
    }
    return w;
}

// OSError(winerror) as ctypes raises it: "[WinError 193] %1 is not a valid Win32 application"
py::OSError win_error(unsigned long code) {
    wchar_t buf[512];
    unsigned long n = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, 0, buf, 512, nullptr);
    std::wstring text(buf, n);
    while (!text.empty() && (text.back() == L'\r' || text.back() == L'\n' || text.back() == L' ' || text.back() == L'.')) {
        text.pop_back();
    }
    std::u32string u;
    for (size_t k = 0; k < text.size(); ++k) {
        u += static_cast<char32_t>(text[k]);
    }
    return py::OSError(py::percent(py::str("[WinError %d] %s"), static_cast<int64_t>(code), py::str(u)));
}

// settings.state(*parts): a path inside the state folder, which is created if it is not there.
// (codebreaker.base.settings has no typed port yet: this is its `state`, over py::settings::get_str,
// which is its `get` for a str setting.)
py::str state_(const py::list<py::str>& parts) {
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& part : parts) {
        path = py::os::path::join(path, part);
    }
    py::os::makedirs(!parts.empty() ? py::os::path::dirname(path) : path, 0777, true);
    return path;
}

// settings.load_native(path): ctypes.CDLL of a native library, loaded from a copy keyed on its bytes
// (state/dll/<hash>/), so a running process never holds the build's own file open and a rebuild is never
// blocked. The original's folder stays on the DLL search path for anything it depends on.
// -> the entry points of cbsymbols.dll; a function it does not export is the AttributeError ctypes raises.
std::shared_ptr<Library> load_native(const py::str& path) {
    py::bytes data = py::open_rb(path).read();
    py::list<py::str> parts;
    parts.append(py::str("dll"));
    parts.append(py::str(py::hashlib::sha256(data).hexdigest()).slice(0, 16));
    parts.append(py::os::path::basename(path));
    py::str copy = state_(parts);
    if (!py::os::path::isfile(copy)) {
        py::str tmp = py::percent(py::str("%s.%d.tmp"), copy, static_cast<int64_t>(_getpid()));
        py::shutil::copyfile(path, tmp);
        try {
            py::os::replace(tmp, copy);
        } catch (const py::OSError&) {   // (another process put the same copy there)
            py::os::remove(tmp);
        }
    }
    AddDllDirectory(wide(py::os::path::dirname(py::os::path::abspath(path))).c_str());
    HMODULE h = LoadLibraryExW(wide(py::os::path::abspath(copy)).c_str(), nullptr,
                               LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
    if (!h) {
        throw win_error(GetLastError());
    }
    auto lib = std::make_shared<Library>();
    lib->handle = h;
    return lib;
}

// lib.<name> of ctypes: the exported function, or AttributeError
template <class F> F entry(const std::shared_ptr<Library>& lib, const char* name) {
    FARPROC p = GetProcAddress(static_cast<HMODULE>(lib->handle), name);
    if (!p) {
        throw py::AttributeError(std::string("function '") + name + "' not found");
    }
    return reinterpret_cast<F>(p);
}

}  // namespace

Json loads(const py::str& text) {
    return Parser(text.u32()).document();
}

// ------------------------------------------------------------------------------------------ the module

py::str dll_path() {
    return py::os::path::join(py::settings::get_str(py::str("symbols_bin")), py::str("cbsymbols.dll"));
}

std::shared_ptr<Library> _load() {
    if (!_LIB.empty()) {
        return _LIB[0];
    }
    std::shared_ptr<Library> lib;
    try {
        py::str path = dll_path();
        if (!py::os::path::isfile(path)) {
            throw py::OSError(py::percent(py::str("not built: %s (native/symbols/CMakeLists.txt)"), path));
        }
        lib = load_native(path);
        lib->cbs_version = entry<int (*)()>(lib, "cbs_version");
        if (lib->cbs_version() != _VERSION) {
            throw py::OSError("cbsymbols.dll is another version: rebuild native/symbols");
        }
        lib->cbs_ask = entry<int (*)(const char*, const unsigned*, size_t, char**, size_t*)>(lib, "cbs_ask");
        lib->cbs_free = entry<void (*)(char*)>(lib, "cbs_free");
    } catch (const py::OSError& e) {
        _WHY.append(py::str(std::string(e.what())));
        lib = nullptr;
    }
    _LIB.append(lib);
    return lib;
}

std::optional<py::str> why_not() {
    _load();
    if (!_WHY.empty()) {
        return _WHY[0];
    }
    return std::nullopt;
}

std::optional<Json> ask(const py::str& question, const py::str& text) {
    std::shared_ptr<Library> lib = _load();
    if (!lib) {
        throw py::RuntimeError(py::percent(py::str("cbsymbols.dll: %s"), why_not()));
    }
    // text.encode("utf-32-le", "surrogatepass"): the code points as 32-bit little-endian units
    const std::u32string& raw = text.u32();
    py::bytes q = question.encode("ascii");
    std::string question_z = q.raw();
    char* out = nullptr;
    size_t size = 0;
    int rc = lib->cbs_ask(question_z.c_str(), reinterpret_cast<const unsigned*>(raw.data()), raw.size(), &out, &size);
    if (rc < 0) {
        throw py::ValueError(py::percent(py::str("cbsymbols.dll does not know %r"), question));
    }
    if (rc == 0) {
        return std::nullopt;
    }
    struct Free {   // finally: lib.cbs_free(out)
        const std::shared_ptr<Library>& lib;
        char* out;
        ~Free() { lib->cbs_free(out); }
    } release{lib, out};
    return loads(py::bytes(std::string(out, size)).decode("ascii"));
}

}  // namespace codebreaker::symbols::native

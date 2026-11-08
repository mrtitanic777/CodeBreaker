// codebreaker/symbols/native.hpp -- typed C++ of codebreaker.symbols.native
//
// codebreaker/symbols/ in C++ (native/symbols: cbsymbols.dll), loaded here (the Python goes through ctypes).
//
// A thin shim: each question goes to the DLL with its text as code points and comes back as JSON.
//
//     native::ask("demangle.parse", name)    the answer (a dict, a str ...), or None for no answer
//
// The answer is JSON, so its type is genuinely mixed (dict, list, str, int, float, bool, None): it is the
// recursive variant Json below, the typed form of the Python values json.loads makes.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::symbols::native {

// A JSON value: None, bool, int, float, str, list, or dict (its entries in the order they were written;
// a key written twice keeps its first place and its last value, as a Python dict does).
struct Json {
    using Array = std::vector<Json>;
    using Object = std::vector<std::pair<py::str, Json>>;
    std::variant<std::monostate, bool, int64_t, double, py::str, Array, Object> value;
};

// json.JSONDecodeError (a ValueError)
struct JSONDecodeError : py::ValueError {
    JSONDecodeError() : py::ValueError() {}
    explicit JSONDecodeError(const std::string& m) : py::ValueError(m) {}
    explicit JSONDecodeError(const char* m) : py::ValueError(m) {}
    const char* type() const override { return "JSONDecodeError"; }
};

// json.loads(text) (not in the Python module: it is the json module's, over a str).
Json loads(const py::str& text);

// The loaded library: ctypes.CDLL in the Python, the entry points of cbsymbols.dll here.
struct Library {
    void* handle = nullptr;
    int (*cbs_version)() = nullptr;
    int (*cbs_ask)(const char* question, const unsigned* text, size_t n, char** answer, size_t* len) = nullptr;
    void (*cbs_free)(char* answer) = nullptr;
};

extern const int64_t _VERSION;
extern py::list<std::shared_ptr<Library>> _LIB;   // empty until loaded; then the library, or None (nullptr)
extern py::list<py::str> _WHY;                    // why it could not be loaded

py::str dll_path();

// The library, or None (nullptr) when it cannot be loaded (why_not() says why).
std::shared_ptr<Library> _load();

std::optional<py::str> why_not();

// `question` about `text` (a str): the JSON answer as values, or None (std::nullopt) for no answer
// (an answer of JSON null is a Json holding None; the Python makes no difference).
std::optional<Json> ask(const py::str& question, const py::str& text);

}  // namespace codebreaker::symbols::native

// codebreaker/symbols/native.hpp -- compat shim (docs/CPP_DESIGN.md 6.3). The Python module reached codebreaker/symbols/ in
// C++ (native/symbols: cbsymbols.dll) through ctypes: each question went to the DLL with its text as code points and came
// back as JSON. There is no DLL any more: the demangler and the headers' reader are cb/symbols/demangle and
// cb/symbols/prototypes, called directly. What is left here is what old callers still use:
//
//     native::ask("demangle.parse", name)    the answer (a dict, a str ...), or None for no answer
//     native::loads(text)                    json.loads, as the typed value Json below
//
// The answer is JSON, so its type is genuinely mixed (dict, list, str, int, float, bool, None): it is the recursive variant
// Json below, the typed form of the Python values json.loads makes. Deleted with its last old-layer consumer
// (engine/facts, formats/xex, source/module_writer).
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

// A JSON value: None, bool, int, float, str, list, or dict (its entries in the order they were written; a key written twice
// keeps its first place and its last value, as a Python dict does).
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

// `question` about `text` (a str): the JSON answer as values, or None (std::nullopt) for no answer (an answer of JSON null
// is a Json holding None; the Python made no difference). Questions: "demangle.parse", "demangle.type_name",
// "prototypes.build" (the text is lines of "<bits> TAB <folder>"); another is a ValueError.
std::optional<Json> ask(const py::str& question, const py::str& text);

}  // namespace codebreaker::symbols::native

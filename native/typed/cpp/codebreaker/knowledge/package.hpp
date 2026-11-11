// codebreaker/knowledge/package.hpp -- typed C++ port of codebreaker/knowledge/__init__.py.
//
// knowledge -- one knowledge base: what CodeBreaker knows about each platform, kept in one place.
//
//     kb::prototype("win16", "KERNEL.GLOBALALLOC")   how the headers declare it: {returns, convention, parameters, header}
//     kb::declaration("win16", "LZRead")              "int WINAPI LZRead(HFILE, void FAR *, int)"
//     kb::spelling("GETDLGITEM")                      "GetDlgItem"
//     kb::argument_bytes("win16", "USER.@171")        bytes of arguments the export removes (its code says)
//     kb::argument_bytes("win32", "FindWindowA", "USER32")
//     kb::import_name("USER", 91)                     "GetDlgItem": a numbered import's name, as a C name
//     kb::name(body_id)                               (name, where it is from) of a function's body, in any module
//     kb::name_body(body_id, "Name")                  ... given once, seen in every module with that body
//     kb::signature_matches("win32", code)            (name, library) of the runtime-library function `code` is
//     kb::struct_(...)                                a structure's layout (symbols/sdk_structs.Struct)
//     kb::api_model("win16", "GLOBALLOCK")            what the emulator's stand-in for it does
//     kb::known(platform, kind, key) / kb::learn(platform, kind, key, value, source)   anything else
//
// The base is one SQLite file keyed by platform (knowledge/store.py). Platform names may be given as the
// lanes say them: ne, pe, le, mz, powerpc, 16, 32.
//
// Differences from the Python:
//   * platform() takes a str or the ints 16 / 32 (the Python's _ALIASES holds both kinds of key).
//   * `known` / `table` give std::optional<facts::Value> / {key: Value}: a kind's rows are JSON (an
//     authored kind: knowledge/sources.py) or a Value the base holds (bytes, text or JSON). A `default`
//     is the optional overload `known(..., default)`.
//   * The Python's `open(path=None)` is `open` (a `std::shared_ptr<KnowledgeBase>`).
//   * `_bare` takes a str (the Python also passes None through; a typed name is a str).
//   * `struct` is a C++ keyword: struct_().
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/engine/facts.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "codebreaker/symbols/sdk_structs.hpp"
#include "py.hpp"

namespace codebreaker::knowledge {

using KnowledgeBase = engine::native_engine::KnowledgeBase;
using Value = engine::facts::Value;

// {platform: its title}
const py::dict<py::str, py::str>& PLATFORMS();

// The base's name for a platform, from the lane's (ne -> win16, pe -> win32, ...).
py::str platform(const py::str& name);
py::str platform(int64_t bits);   // 16 -> "win16", 32 -> "win32", else the number's text

std::shared_ptr<KnowledgeBase> open(const std::optional<py::str>& path = std::nullopt);

// ---- anything ----
std::optional<Value> known(const py::str& platform_, const py::str& kind, const py::str& key);
Value known(const py::str& platform_, const py::str& kind, const py::str& key, const Value& default_);
// Keep something found: written once, seen by every later ask in any process.
void learn(const py::str& platform_, const py::str& kind, const py::str& key, const Value& value,
           const py::str& source = py::str(""));
// {key: value} of a kind (an authored kind is loaded from its file first when the file changed).
py::dict<py::str, Value> table(const py::str& platform_, const py::str& kind);
py::dict<py::str, Value> _table(const py::str& platform_, const py::str& kind);

// ---- prototypes ----
// `KERNEL.GLOBALALLOC`, `USER32!FindWindowA` -> the function's own name; anything else as it is.
py::str _bare(const py::str& name);
// How the headers declare a Windows function, or None. Capitals do not matter.
std::optional<symbols::prototypes::Prototype> prototype(const py::str& platform_, const py::str& name);
std::optional<py::str> declaration(const py::str& platform_, const py::str& name);
py::str spelling(const py::str& name);
// Bytes of arguments an imported function removes, or None when unknown. win16: `api` as the emulator reports
// it ('USER.@171', 'USER_171'). win32: `api` the name (or '#N') and `module` the DLL exporting it -- the
// headers' size first, else what its code does (`module` is required then: the Python's default None is "").
std::optional<int64_t> argument_bytes(const py::str& platform_, const py::str& api,
                                      const std::optional<py::str>& module = std::nullopt);
// Bytes of arguments a 32-bit function takes as the headers declare it, or None.
std::optional<int64_t> declared_argument_bytes(const py::str& name);
// A numbered import's name as a C identifier, or None (unknown, or a C++ name).
std::optional<py::str> import_name(const py::str& module, int64_t ordinal);

// ---- names of bodies ----
// (name, where it is from) of a function's body (symbols/names.py digests it), or None.
std::optional<engine::native_engine::Cells> name(const py::str& body_id, const std::shared_ptr<KnowledgeBase>& kb = nullptr);
void name_body(const py::str& body_id, const py::str& name_, const py::str& source = py::str("named by hand"),
               const std::shared_ptr<KnowledgeBase>& kb = nullptr);

// ---- signatures, structures, behaviour ----
// The runtime-library functions' rows as the base keeps them (the Python: [(leading bytes, size, mask, digest,
// name, library)] -- each row a JSON list here).
py::list<Value> signatures(const py::str& platform_);
// (name, library) of the runtime-library function `code` starts with, or None.
std::optional<std::tuple<py::str, py::str>> signature_matches(const py::str& platform_, const py::bytes& code);
// The layout of a structure the headers declare (sdk_structs.Struct), or None (win32 only).
std::optional<symbols::sdk_structs::Struct> struct_(const py::str& platform_, const py::str& name_);
// What the emulator's stand-in for a system function does ({ax, dx, pop} for win16, with "arg<N>" and "heap" as
// knowledge/models/win16.json says; {argc} for win32; {answers_in} for a ring-0 service), or None when it is not
// modelled. Keys starting with "_" are left out.
std::optional<py::dict<py::str, sources::Json>> api_model(const py::str& platform_, const py::str& api);

}  // namespace codebreaker::knowledge

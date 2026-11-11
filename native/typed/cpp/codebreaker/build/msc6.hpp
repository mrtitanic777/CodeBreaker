// codebreaker/build/msc6.hpp -- interface of codebreaker.build.msc6 (typed C++; implementation pending).
//
// msc6.py -- Microsoft C 6.0a, the compiler of the 16-bit Windows modules, run for comparison.
//
//     msc6::available();
//     built = msc6::compile_many({{key, c_text}, ...}, header, "/AM /Gsw /Os");   // {key: (bytes, mask) or None}
//
// The compiler is a DOS program, run in DOSBox-X (settings dosbox_x, msc6/msvc152), every source of a call in
// ONE session. Each function is compiled as a file of its own (or a batch) and its code read out of the object file,
// with the bytes the linker fills in marked in `mask` and left out of every comparison.
//
// Typing notes:
//   * `Key`: compile_many's keys are whatever the caller numbers its sources by: a function name (str), an address
//     (int), or a (function, try-number) pair (invert/run.py: (fn, k) with fn a name or address). One variant.
//   * (bytes, mask) -> `Code`; a function the compiler refused -> std::nullopt.
//   * The Python source's __file__ (build/module.py hashes the source text into its cache key) has no C++ twin:
//     `module_file()` answers the path of the Python module (kept for that cache stamp).
//   * wine_typing's `exports` test hook (module, ordinal) -> wine16.Export is a std::function answering optional.
//   * sdk_prototypes() answers the Prototypes record (the Python's dict subclass with a `void` set).
//   * The regex constants (_PASCAL, _IMPORT, _FRAME ...) are private to the .cpp.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/symbols/wine16.hpp"
#include "py.hpp"

namespace codebreaker::build::msc6 {

// the path of build/msc6.py (the Python's __file__; used by build.module to stamp its cache) -- used by: build.module
py::str module_file();

// /Gc: a function declared without a convention is pascal, as the reconstruction's own default
extern const py::list<py::str> FLAG_SETS;   // used by: verify.authored.match16
inline constexpr int64_t BATCH = 40;        // functions to a source file when each is cut out by name

// (DOSBox-X, the compiler's folder); compiler: "msc6" or "msvc152"  [private _tools]
std::tuple<std::optional<py::str>, std::optional<py::str>> _tools(const py::str& compiler = py::str("msc6"));

bool available(const py::str& compiler = py::str("msc6"));

// (code, mask): the bytes of a piece of code, and 1 for every byte a fix-up record (or a hole) names
using Code = std::tuple<py::bytes, py::bytes>;

// (bytes, mask) of the first code segment of an OMF object file
Code object_code(const py::bytes& obj);

// (header, text) as Microsoft C 6 takes them.
std::tuple<py::str, py::str> dialect(const py::str& header, const py::str& text);

// the functions the texts declare or define with a callee-pops (__stdcall) convention
py::set<py::str> pascal_names(const py::list<py::str>& texts);

// `text` with the arguments of every call to, and the parameters of every definition of, a function in `names` in Pascal order
py::str pascal_order(const py::str& text, const py::set<py::str>& names);

// the Windows SDK's WINDOWS.H (Visual C++ 1.52c's), or None
std::optional<py::str> windows_h();

// `header` with every import by number that is really a C function declared __cdecl, not __stdcall.
// sdk: WINDOWS.H's text (default: windows_h() or "")
py::str cdecl_imports(const py::str& header, const std::optional<py::str>& sdk = std::nullopt);

// (module, ordinal) -> the Export Wine's .spec has, or None (default: wine16::exports(module)[ordinal])
using ExportLookup = std::function<std::optional<symbols::wine16::Export>(const py::str&, int64_t)>;

// (header, {name: text}) with every import by number that Wine's .spec types declared with its parameters and result width
std::tuple<py::str, py::dict<py::str, py::str>> wine_typing(const py::str& header, const py::dict<py::str, py::str>& texts,
                                                            const ExportLookup& exports = nullptr);

// `text` with the parameters of an exported function typed as Wine's .spec has them (`export`: None -> unchanged)
py::str wine_definition(const py::str& text, const std::optional<symbols::wine16::Export>& export_);

// WINDOWS.H, then `header` (whose imports by number keep their own declarations)
py::str sdk_header(const py::str& header, const std::optional<py::str>& sdk = std::nullopt);

// {API name: [parameter type, ...]} of every WINAPI function WINDOWS.H declares on a line, or None for one with a variable
// argument list (`params` unset); a function that returns nothing has its name in `void_`.   [Python: dict subclass _Prototypes]
struct Prototypes {
    py::dict<py::str, std::optional<py::list<py::str>>> params;
    py::set<py::str> void_;
};
Prototypes sdk_prototypes(const std::optional<py::str>& sdk = std::nullopt);

// `text` (in Pascal order) with each call of an import by number that the SDK declares said by its name
py::str sdk_calls(const py::str& text, const Prototypes& protos);

// `text` with each decompiler temporary (assigned once, used once, in the next statement) put back into the expression
py::str fold_temporaries(const py::str& text);

// `text` with each pair of a function's own parameters used only as a pair made the one far pointer the source had
py::str join_far_parameters(const py::str& text);

// `text` with the decompiler's frame structure taken apart into the locals a source declares
py::str split_frame(const py::str& text);

// (declarations, text): every read or write through a constant near address made one through a variable of the data segment
std::tuple<py::str, py::str> ds_globals(const py::str& text);

// (bytes, mask) of the public function `name` in an OMF object file, or None
std::optional<Code> function_code(const py::bytes& obj, const py::str& name);

// (the compiler's folder, a work folder), both by short paths, when `compiler` runs on Windows itself -- else None  [tests]
std::optional<std::tuple<py::str, py::str>> _native(const py::str& compiler);

// One emulator session compiling `files` [(stem, C text)] -> whether it ran to its end.  [tests/msc8]
bool _session(const py::str& work, const py::str& header, const py::list<std::tuple<py::str, py::str>>& files,
              const py::str& flags, const py::str& compiler, int64_t timeout);

// What a caller numbers its sources by (see the header notes).
using Key = std::variant<py::str, int64_t, std::tuple<std::variant<py::str, int64_t>, int64_t>>;

// Compile each of `sources` {key: C text} with `header` before it. -> {key: (bytes, mask) or None}
// names: {key: function name}; together: which keys share a source file; batch: keys to a file otherwise.
py::dict<Key, std::optional<Code>> compile_many(const py::dict<Key, py::str>& sources, const py::str& header,
                                                const py::str& flags, const std::optional<py::str>& work = std::nullopt,
                                                int64_t timeout = 900, const py::str& compiler = py::str("msc6"),
                                                const std::optional<py::dict<Key, py::str>>& names = std::nullopt,
                                                int64_t batch = BATCH,
                                                const std::optional<py::list<py::list<Key>>>& together = std::nullopt);

// ([(stem, text)], [[(first line, key)]]): each group's functions in one file, and the line each begins on.
// todo: {key: (cache key, text)}                                                         [tests]
std::tuple<py::list<std::tuple<py::str, py::str>>, py::list<py::list<std::tuple<int64_t, Key>>>>
_batch_files(const py::list<py::list<Key>>& groups, const py::dict<Key, std::tuple<py::str, py::str>>& todo,
             const py::str& prefix);

// the keys of the functions a refused file's errors are in -- [] when an error is not in the file itself  [tests]
py::list<Key> _refused(const py::str& path, const py::list<std::tuple<int64_t, Key>>& starts);

}  // namespace codebreaker::build::msc6

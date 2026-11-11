// codebreaker/translate/frontends/clang/package.hpp -- typed C++ port of codebreaker/translate/frontends/clang/__init__.py.
//
// The C and C++ front end: clang's typed AST, lowered into the IR (docs/TRANSLATOR_DESIGN.md, phase 5).
//
// clang parses and attributes the source (`-Xclang -ast-dump=json -fsyntax-only`): every expression's type, every implicit
// conversion, the declaration each name refers to, the overload and template instance chosen. lower turns the declarations of
// the program's own files into one ir.Unit.
//
//     auto unit = clang::lower_file("prog.c");                       // C (the language from the extension)
//     auto unit = clang::lower_file("prog.cpp", "c++", "c++17");     // C++
//     auto doc  = clang::l2_file("prog.c");                          // the same as an L2 document (cached)
//
// The target is fixed (`TRIPLE`, LLP64 Windows x64): it is the data model the judge's own clang build of the program runs
// with, so sizeof, struct layouts and `long` agree between the original and the translation. clang is found from settings.json's
// "clang", else Visual Studio's bundled LLVM, else PATH.
//
// Typing notes:
//   * The Python's `language=None`, `std=None` are std::optional<str>; the tuples/lists of options (`includes=()`) are
//     py::list<str>; `own` (None | the program's own files) is optional<list<str>>.
//   * dump() answers clang's JSON AST as a node Dict (ir::Dict), read with lower::load_tree; l2_file() answers the L2 document
//     as a Dict (the Python's dict), cached as JSON text under the state folder exactly as the Python's cache files are
//     (json.dump's default separators, ensure_ascii), with the same key (_key), so the two share a cache.
//   * clang is run with run_process (translate/dotnet/package.hpp: subprocess.run with a timeout and bytes). Its output is
//     decoded as UTF-8 (the Python's `surrogateescape` keeps undecodable bytes as lone surrogates; here they decode as
//     U+FFFD -- clang's JSON is UTF-8, so a real dump has none).
//   * _key reads the Python sources of the lowering (lower.py, ctypes.py, as `_lower.__file__` is) from the checkout:
//     `_SOURCE_DIR()` is that folder (codebreaker/translate/frontends/clang of the checkout; set_source_dir to change it).
//   * RuntimeError carries the messages of the Python's.
#pragma once

#include <optional>

#include "codebreaker/translate/ir.hpp"
#include "py.hpp"

namespace codebreaker::translate::frontends::clang {

namespace ir = codebreaker::translate::ir;

extern const py::str TRIPLE;
extern const int64_t REV;      // bump when the lowering changes what it makes of the same source
extern const py::list<py::str> _VS_LLVM;
extern const int64_t TIMEOUT;

py::str _SOURCE_DIR();
void set_source_dir(const py::str& dir);

// The clang executable, or None.
std::optional<py::str> clang();

py::str language_of(const py::str& path);

// clang's command-line options for parsing (and the judge's compiling) a source file.
py::list<py::str> flags(const py::str& language, const std::optional<py::str>& std_ = std::nullopt,
                        const py::list<py::str>& includes = py::list<py::str>(), const py::list<py::str>& quote_includes = py::list<py::str>(),
                        const py::list<py::str>& defines = py::list<py::str>());

// clang's JSON AST of one source file (a dict). Raises RuntimeError with clang's errors.
ir::Dict dump(const py::str& path, const std::optional<py::str>& language = std::nullopt, const std::optional<py::str>& std_ = std::nullopt,
              const py::list<py::str>& includes = py::list<py::str>(), const py::list<py::str>& quote_includes = py::list<py::str>(),
              const py::list<py::str>& defines = py::list<py::str>());

// One source file -> ir.Unit. `own`: the files whose declarations are the program's (default: the file itself and the headers
// beside it).
py::ref<ir::Unit> lower_file(const py::str& path, const std::optional<py::str>& language = std::nullopt, const std::optional<py::str>& std_ = std::nullopt,
                             const py::list<py::str>& includes = py::list<py::str>(), const py::list<py::str>& quote_includes = py::list<py::str>(),
                             const py::list<py::str>& defines = py::list<py::str>(), const std::optional<py::list<py::str>>& own = std::nullopt);

py::str _key(const py::str& path, const py::str& language, const std::optional<py::str>& std_, const py::list<py::str>& includes,
             const py::list<py::str>& quote_includes, const py::list<py::str>& defines, const std::optional<py::list<py::str>>& own);

// lower_file as an L2 document (a dict), cached under the state folder by the source and the lowering.
ir::Dict l2_file(const py::str& path, const std::optional<py::str>& language = std::nullopt, const std::optional<py::str>& std_ = std::nullopt,
                 const py::list<py::str>& includes = py::list<py::str>(), const py::list<py::str>& quote_includes = py::list<py::str>(),
                 const py::list<py::str>& defines = py::list<py::str>(), const std::optional<py::list<py::str>>& own = std::nullopt, bool cache = true);

// json.dumps(value) as Python writes it by default (", " and ": ", ensure_ascii) of a node Val (a Type is not JSON: TypeError).
py::str json_dumps(const ir::Val& value);

}  // namespace codebreaker::translate::frontends::clang

// codebreaker/exact/modules.hpp -- typed C++ of codebreaker.exact.modules
//
// modules.py -- the program's source files: their functions and data put together, compiled whole, checked.
//
//     python -m codebreaker exact PROJECT modules [MODULE..] [-v]
//
// The functions are written one to a file (src/fADDR.c) so that each can be compiled and compared
// alone. The original had them in modules; src/MODULES gives, one a line:
//
//     name   code-start | -   data-start   [switches]
//         name.c   a source file: its code from code-start to the next module's, its data from data-start
//         NAME.OBJ an object of the kit, linked as it is
//         -        a file of data only (src/NAME-data.c)
//
// A module's data is in src/dADDR.c (what the file defines before the function at ADDR) and src/eADDR.c
// (after it, at the file's end); its functions are the src/fADDR.c of its code. They are put together
// in order - the includes once, each extern once, the functions of other files that return something
// else than an int declared - compiled once, and every function is compared with the program, and the
// file's initialized data (_DATA) with the data segment, word for word where the linker fills a word.
//
//     modules::rows(project) -> [(name, code start, code end, data start, switches)]
//     modules::source(project, row) -> (text, [function addresses], flags) or None
//     modules::check(project, row) -> CheckResult(res=[(name, address, length, verdict)], dlen, dbad, raw)
//
// Typing notes (the Python's duck-typed objects):
//   * `prj` is the interface `Project` below: the members of exact.project.Project that modules.py reads
//     (src, path(), code_end, own_end(), switches, cfg.get(key) for the string keys it asks for, code_segments,
//     toolchain(), headers(), include, sets()[1:], image, symbols(), dgroup, loadseg, code_base(), relocs()).
//     The real Project (and the test double) implement it.  `Toolchain` is the part of exact.toolchain.Toolchain
//     it reads: `p.get(key)` for string entries, and compile().
//   * Project.sets() is `also_sets()` here: the sets after the project itself (the python's `sets()[1:]`); a set
//     is a Project, and "part is not prj" is a null pointer in set_of's answer.
//   * compare.function takes the 16-bit decoder (platforms.decoder(16), its module-global `_CS`) as a parameter in
//     the typed port of compare; check() passes the one it is given.
//   * check()'s dict(res=, dlen=, dbad=, raw=) / dict(error=) / dict(obj=True, ...) is the struct CheckResult.
//   * A toolchain's "segment_switch" is formatted with str.format(name=seg): "{name}", "{{" and "}}" are
//     understood here, any other replacement field is a ValueError.
//   * The source files are read as the Python reads them: latin-1 (the MODULES/CRTLIB lists in the locale's
//     default encoding, UTF-8 here).
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/compare.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::modules {

using Symbols = compare::Symbols;

// exact.toolchain.Toolchain, as modules.py uses it
class Toolchain {
  public:
    virtual ~Toolchain() = default;
    // tc.p.get(key): the profile's entry (a string), or nullopt when it has none
    virtual std::optional<py::str> p_get(const py::str& key) const = 0;
    // tc.compile(src, headers=, include=, switches=) -> (the object module or None, the console text)
    virtual std::tuple<std::optional<py::bytes>, py::str> compile(const py::bytes& src, const py::dict<py::str, py::bytes>& headers,
                                                                  const py::list<py::str>& include,
                                                                  const std::optional<py::str>& switches) const = 0;
};

// exact.project.Project, as modules.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::str src() const = 0;                                    // prj.src
    virtual py::str path(const py::str& name) const = 0;                // prj.path(name)
    virtual int64_t code_end() const = 0;                               // prj.code_end (0 when none)
    virtual int64_t own_end() const = 0;                                // prj.own_end()
    virtual std::optional<py::str> switches() const = 0;                // prj.switches
    virtual std::optional<py::str> cfg_str(const py::str& key) const = 0;      // prj.cfg.get(key) (a string entry)
    virtual py::list<int64_t> code_segments() const = 0;                // prj.code_segments (empty when it has none)
    virtual std::shared_ptr<Toolchain> toolchain() const = 0;           // prj.toolchain()
    virtual py::dict<py::str, py::bytes> headers() const = 0;           // prj.headers()
    virtual py::list<py::str> include() const = 0;                      // prj.include
    virtual py::list<std::shared_ptr<const Project>> also_sets() const = 0;    // prj.sets()[1:]
    // used by check()
    virtual py::bytes image() const = 0;                                // prj.image
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual int64_t dgroup() const = 0;                                 // prj.dgroup
    virtual int64_t loadseg() const = 0;                                // prj.loadseg
    virtual int64_t code_base(int64_t a) const = 0;                     // prj.code_base(a)
    virtual const py::set<int64_t>& relocs() const = 0;                 // prj.relocs()
};

// (name, code start, code end, data start, switches)
using Row = std::tuple<py::str, std::optional<int64_t>, std::optional<int64_t>, std::optional<int64_t>, std::optional<py::str>>;

py::list<Row> rows(const Project& prj);

// where the library's part begins: the first code place src/CRTLIB names ("code ADDR", "code=ADDR-", or a
// line's note that begins with its place)
std::optional<int64_t> lib_start(const Project& prj);

// [(start, end, segment)]: the functions of the file that #pragma alloc_text puts in another code segment
// (a row of src/MODULES: name start @SEGMENT)
py::list<std::tuple<int64_t, int64_t, py::str>> parts(const Project& prj, const py::str& name);

// the code starts src/MODULES gives (no ends: nothing else is asked of the project)
py::list<int64_t> starts(const Project& prj);

py::list<py::str> files_of(const py::str& src, int64_t start, int64_t end);

// {function: the type it returns} - the program's functions that return something else than an int
py::dict<py::str, py::str> deftypes(const py::str& srcdir);

// the files' texts as one source; the switches the functions' files ask for. before: {path: text put
// before its body} (a pragma).  header_tag: the profile's "file_switches" (None/empty: no header comment)
std::tuple<py::str, py::set<py::str>> combine(const py::list<py::str>& paths, const py::str& default_,
                                              const std::optional<py::str>& header_tag,
                                              const py::dict<py::str, py::str>& before = py::dict<py::str, py::str>());

// (the module's whole text, the addresses of its functions in order, the switches) or None (an object of the kit)
std::optional<std::tuple<py::str, py::list<int64_t>, py::set<py::str>>> source(const Project& prj, const Row& row);

// (the source set, its toolchain) whose files' pattern names this file - an "also" set's, else the project's.
// The set is null when it is the project itself.
std::tuple<std::shared_ptr<const Project>, std::shared_ptr<Toolchain>> set_of(const Project& prj, const py::str& mod);

// the name the project gives the first code segment (project "first_segment"), for a module that lies in it
std::optional<py::str> first_segment(const Project& prj, std::optional<int64_t> start);

// -> (the object module or None, the console text, the addresses of the module's functions)
std::tuple<std::optional<py::bytes>, py::str, py::list<int64_t>> compile_module(const Project& prj, const Row& row,
                                                                                const std::shared_ptr<Toolchain>& tc = nullptr);

// check()'s answer
struct CheckResult {
    bool obj = false;                       // an object of the kit (res, dlen, dbad empty)
    std::optional<py::str> error;           // the compiler's console text when it did not compile
    py::list<std::tuple<py::str, std::optional<int64_t>, int64_t, py::str>> res;    // (name, address, length, verdict)
    int64_t dlen = 0;
    py::list<py::str> dbad;
    std::optional<py::bytes> raw;
};

CheckResult check(const Project& prj, const Row& row, const compare::Decoder& decode, const std::optional<py::bytes>& raw = std::nullopt);

int64_t main(const Project& prj, const py::list<py::str>& argv, const compare::Decoder& decode);

}  // namespace codebreaker::exact::modules

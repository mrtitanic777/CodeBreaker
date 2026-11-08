// codebreaker/exact/batch.hpp -- typed C++ of codebreaker.exact.batch
//
// batch.py -- many candidates of a function compiled in one run.
//
// A run's cost is mostly the compiler starting its passes, not the function: N candidates in one source cost
// little more than one. Each candidate's own names - the function, what it declares extern, the structs it
// defines - are given a suffix of its own (__Q<k>) so that they do not meet; the object's names are given back
// after; each function of the object is held to the program's code as a lone compile's would be.
//
// A candidate the compiler refuses (the error's line says whose) is left out and the rest compiled again; the
// ones left out the caller proves alone (draft/prove.py's _prove mends what it can). Candidates with
// preprocessor lines other than #include, or a different file comment (MFLAGS) from the rest, are not batched.
//
// Typing notes:
//   * compile_many's `prj` and `tc` are objects of other modules (a Project, a toolchain); what it asks of them
//     is the two small interfaces below: prj.headers() / prj.include / prj.switches, and tc.compile(...).
//   * `split` and `build` return None as std::nullopt.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::batch {

extern const py::set<py::str> KEYWORDS;
extern const py::re::Pattern SUFFIX;

// What compile_many asks of a project: prj.headers(), prj.include, prj.switches.
struct Project {
    std::function<py::dict<py::str, py::bytes>()> headers;
    py::list<py::str> include;
    std::optional<py::str> switches;
};

// What compile_many asks of a toolchain: tc.compile(src, headers=, include=, switches=) ->
// (object bytes or None, what the tool printed).
class Toolchain {
  public:
    virtual ~Toolchain() = default;
    virtual std::tuple<std::optional<py::bytes>, py::str> compile(
        const py::bytes& src, const py::dict<py::str, py::bytes>& headers, const py::list<py::str>& include,
        const std::optional<py::str>& switches) = 0;
};

// (file comment and #include lines, the rest) - or None when the candidate is not one to batch
std::optional<std::tuple<py::str, py::str>> split(const py::str& src);

// the names a candidate gives the file: its function, what it declares extern, the structs it defines
py::set<py::str> own_names(const py::str& body);

py::str renamed(const py::str& body, int64_t k);

// -> (text, [(first line, last line) of each candidate], [index of each candidate in srcs]) or None
std::optional<std::tuple<py::str, py::list<std::tuple<int64_t, int64_t>>, py::list<int64_t>>> build(
    const py::list<py::str>& srcs);

py::set<int64_t> error_lines(const py::str& con);

// -> {index in srcs: (object, the function's index among the object's)} for the candidates compiled together;
// those missing are the caller's to prove alone
py::dict<int64_t, std::tuple<py::bytes, int64_t>> compile_many(const Project& prj, Toolchain& tc,
                                                               const py::list<py::str>& srcs, int64_t rounds = 3);

}  // namespace codebreaker::exact::batch

// codebreaker/exact/draft/prove.hpp -- typed C++ from codebreaker.exact.draft.prove.
//
// prove.py -- a candidate held to the program: compiled (mended where the compiler stops), compared, and the
// nearest refined by a beam search over the rewrites (REFINERS), the kind of difference choosing which first.
//
// C++ spelling notes:
//  * `prj` is forms::Prj (forms.hpp: the project, its toolchain's profile entries, the compare members, the decoder,
//    and `compile_listing`, the compile of aim's listing); `tc` is batch::Toolchain (batch.hpp: tc.compile(..)); `inv`
//    is Inv below (candidates::Inv and the `inv.LAST["regparam"]` prove_best falls back on);
//  * Python's function attributes `_prove.src` / `_prove.spent` are the second member of _prove's result and the
//    file-scope counter `spent()`;
//  * a rank is the tuple _rank gives; a reading of the beam is a Reading (rank, source, verdict);
//  * REFINERS are (name, function) pairs: the name is what the Python takes from the function's code object
//    (`_refiner_name`: the first global the lambda names that is not a helper), written out;
//  * state/exact/priors.json is read and written as the Python does (a dict of dicts of ints).
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/exact/batch.hpp"
#include "codebreaker/exact/draft/candidates.hpp"
#include "codebreaker/exact/draft/forms.hpp"
#include "py.hpp"

namespace codebreaker::exact::draft::prove {

// inv: the model's checker (candidates::Inv) and `getattr(inv, "LAST", {}).get("regparam")`
struct Inv : candidates::Inv {
    std::optional<py::dict<py::str, int64_t>> last_regparam;
};

using Rank = std::tuple<int64_t, int64_t, int64_t>;
struct Reading {
    Rank rank;
    py::str src;
    py::str verdict;
    bool operator==(const Reading& o) const { return rank == o.rank && src == o.src && verdict == o.verdict; }
};

// the model's texts [(text, register parameters or None)]
using Texts = py::list<std::tuple<py::str, std::optional<py::dict<py::str, int64_t>>>>;

extern const int64_t MAXCANDS;
extern const int64_t FUNC_BUDGET;
extern const std::tuple<double, double> HOPELESS;
extern const int64_t BEAM;
extern const int64_t REFINE_BUDGET;

// the mendings of the line the compiler stopped at, as the error says ([] when none)
py::list<py::str> compile_fixes(const py::str& src, const py::str& con);

// the source with the line the compiler stopped at mended, as the error says, or None
std::optional<py::str> compile_fix(const py::str& src, const py::str& con);

// the verdict on src - mended where the compiler says it cannot be compiled, each way the error offers, and again
// (a few rounds, a few compiles): the best of what compiles -> (verdict, the source the verdict is of)
std::tuple<py::str, py::str> _prove(const forms::Prj& prj, batch::Toolchain& tc, const py::str& src, int64_t a, int64_t ln,
                                    int64_t tries = 4, int64_t budget = 10);

// the compiles _prove and _batched have spent on the function in hand (Python's `_prove.spent`)
int64_t& spent();

// the verdict on the k-th function of the object (a batch's: batch.compile_many)
py::str _verdict(const forms::Prj& prj, const py::bytes& raw, int64_t a, int64_t ln, int64_t k = 0);

// how near a verdict is
Rank _rank(const py::str& v);

// -> (source, verdict): of each of the model's texts and their variants, the first the compiler proves identical;
// else the best of them
std::tuple<py::str, py::str> prove_texts(const forms::Prj& prj, batch::Toolchain& tc, const Inv& inv, const Texts& texts, int64_t a,
                                         int64_t ln, const py::dict<int64_t, py::str>& names);

// True: the prologue calls the stack probe; False: a frame without one; None: not a frame the rule knows
std::optional<bool> _probes(const py::bytes& code);

// a verdict too far from the program's code for the beam's rewrites to close
bool _hopeless(const py::str& v, int64_t ln);

// a candidate as the compiler sees it, for telling two apart
py::str _canon(const py::str& src);

// -> (source, verdict): of the model's text and its variants, the first the compiler proves identical; else the
// first that compiles (or the first)
std::tuple<py::str, py::str> prove_best(const forms::Prj& prj, batch::Toolchain& tc, const Inv& inv, const py::str& text, int64_t a,
                                        int64_t ln, const py::dict<int64_t, py::str>& names,
                                        const std::optional<py::dict<py::str, int64_t>>& regparam = std::nullopt);

// -> (source, verdict): a beam search over the REFINERS' rewritings (see the Python)
std::tuple<py::str, py::str> refine(const forms::Prj& prj, batch::Toolchain& tc, int64_t a, int64_t ln, const Reading& best,
                                    int64_t rounds = 6, std::optional<int64_t> budget = std::nullopt,
                                    const std::optional<py::list<Reading>>& seeds = std::nullopt);

// the rewrites, by name, in the Python's order
struct Refiner {
    py::str name;
    std::function<py::list<py::str>(const forms::Prj&, const py::str&)> fn;
};
const std::vector<Refiner>& REFINERS();

}  // namespace codebreaker::exact::draft::prove

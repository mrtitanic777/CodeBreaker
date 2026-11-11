// codebreaker/verify/teeth.hpp -- typed C++ of codebreaker.verify.teeth.
//
// teeth.py -- does the judge notice when a reconstruction is wrong?
//
//     python -m codebreaker teeth <module> [--rounds N] [--json FILE]
//
// A verdict of "behaves as the original" is worth what the judge would have said of a wrong
// reconstruction. This asks it: functions the judge has passed are each broken in one small way
// that changes what they do -- a constant one higher, a comparison turned round, a plus made a
// minus, a store left out, two arguments swapped -- the module is built with them, and they are
// judged again. A broken function the judge still passes is a hole in the judge (or the break was
// in code no input reaches, which is the same thing said of the inputs).
//
// Works for every kind of module whose functions are rebuilt from C and run beside the original
// (32-bit and 16-bit Windows, PowerPC). The broken functions of one round never reach one another,
// so each is caught, or not, on its own account. Nothing is recorded in the function store.
//
//     caught      the judge now says the function differs from the original
//     survived    the judge still passes it
//
// The share caught is given for the functions it had called VERIFIED and for those it had called
// VERIFIED (weak) apart: the first is what the strong verdict is worth.
//
// Typing notes:
//   * plant() is a generator in Python (rounds in, a list per round out); here `rounds` is a callable that gives the next
//     round (nullopt at the end) and `each` is called with each round's list as the generator would have yielded it -- the
//     next round is asked for only after `each` returns, as the Python's lazy generator is. `Said` is what the judge says
//     of one name (a template parameter: run() has the whole verdict tuple, the driver lane's self-check just the word).
//   * A round of breaks is a list of (name, what, made): `made` is the broken text (run) or the broken file (the driver lane).
//   * run() returns a Report (the Python's dict {"module", "kind", "rounds", "results", "caught"}); a result row is a
//     Result: the Python's dict has the keys name, was, now, reason, made, plus change and limits when the break compiled
//     (`compiled`); to_json() writes the report as json.dump(indent=1) does, the keys in the Python's order.
//   * `log` is the Python's `log=print`: an empty std::function is None (nothing logged).
//   * The judge of a round is run serially in this process (the emulators keep per-process state).
//   * main(argv) takes what follows the command name; the platform's `readable` (a .xex's basefile, an .xbe's PE image) is
//     base::corpus::_readable.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

#include "py.hpp"

namespace codebreaker::verify::teeth {

using Log = std::function<void(const py::str&)>;
void log_print(const py::str& line);   // print

// the five kinds of break, in the order they are tried: constant, comparison, operator, store, arguments
extern const py::list<py::str> BREAK_KINDS;

// (head with the opening brace, declarations, statements) -- breaks are made in statements.
std::tuple<py::str, py::str> _body(const py::str& text);

// each returns the body with the break made, or nullopt when it does not apply; `shadow` is the body with strings and comments blanked
std::optional<py::str> _constant(const py::str& body, const py::str& shadow);
std::optional<py::str> _comparison(const py::str& body, const py::str& shadow);
std::optional<py::str> _operator(const py::str& body, const py::str& shadow);
std::optional<py::str> _store(const py::str& body, const py::str& shadow);
std::optional<py::str> _arguments(const py::str& body, const py::str& shadow);

// [(kind of break, the function's text with it made)] -- every kind that applies.
py::list<std::tuple<py::str, py::str>> breaks(const py::str& text);

// 'old line  ->  new line' for the one line a break changed (or removed).
py::str _changed(const py::str& before, const py::str& after);

// {name: every function it can reach, itself included}
py::dict<py::str, py::set<py::str>> _reach(const py::dict<py::str, py::str>& texts);

// A function (or an entry point) and one break made in it: (name, what, made)
using Break = std::tuple<py::str, py::str, py::str>;
using Round = std::vector<Break>;

template <class Said>
struct Planted {
    py::str name, what, made;
    Said said;
    py::str outcome;
};

// The judge judged, whatever the lane. `rounds` gives the next round of breaks (the breaks of a round never reaching one
// another); judge(round) judges a round's together -> {name: what the judge said}; outcome(said) is "caught" (it noticed),
// "missed" (it passed the break) or anything else (not counted). `each` is told, round by round, what came of it.
// The lanes: C text broken and rebuilt (run, below), a 16-bit build's code (tools/gate_teeth16.py), a driver's (verify/lanes/driver.py's self_check).
template <class Said>
void plant(const std::function<std::optional<Round>()>& rounds,
           const std::function<py::dict<py::str, Said>(const Round&)>& judge,
           const std::function<py::str(const Said&)>& outcome,
           const std::function<void(const std::vector<Planted<Said>>&)>& each) {
    while (true) {
        std::optional<Round> batch = rounds();
        if (!batch) return;
        py::dict<py::str, Said> said = judge(*batch);
        std::vector<Planted<Said>> out;
        for (const Break& b : *batch) {
            const Said& s = said.at(std::get<0>(b));
            out.push_back(Planted<Said>{std::get<0>(b), std::get<1>(b), std::get<2>(b), s, outcome(s)});
        }
        each(out);
    }
}

// One function's result.
struct Result {
    py::str name;
    py::str was;                          // "verified" or "weak": what the judge had called it
    py::str made;                         // the kind of break
    py::str reason;
    py::str now;                          // "caught", "survived", "not judged" or "does not compile"
    bool compiled = true;                 // false: the Python's dict has no `change` and `limits`
    py::str change;                       // 'old line  ->  new line'
    py::str limits;                       // the verdict's limits, joined by ","
};

struct Caught {
    py::str group;                        // "verified", "weak" or "by break: <kind>"
    int64_t caught = 0;
    int64_t of = 0;
};

struct Report {
    py::str module;
    py::str kind;
    int64_t rounds = 0;
    py::list<Result> results;
    py::list<Caught> caught;              // sorted by group
};

// -> the report of a module: breaks the functions the judge passed, rebuilds, judges again. SystemExit for a module whose kind is not
// judged function by function.
Report run(const py::str& module_path, int64_t rounds = 3, const Log& log = log_print);

// json.dump(report, f, indent=1)
py::str report_json(const Report& report);

extern const py::str DOC;   // main's docstring
// python -m codebreaker teeth <module> [--rounds N] [--json FILE]
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::verify::teeth

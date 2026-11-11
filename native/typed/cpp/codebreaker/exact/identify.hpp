// codebreaker/exact/identify.hpp -- typed C++ port of codebreaker.exact.identify.
//
// identify.py -- which toolchain made the program: the one that makes the most of its sources' functions its bytes.
//
//     python -m codebreaker exact PROJECT identify [--try NAME[:VARIANT][/SWITCHES] ..] [--sample N] [--write]
//
// A program's sources - seeded from another version (exact/seed.py), drafted, or written - are compiled by each
// candidate toolchain and held to the program as the ledger holds them. The candidates are every toolchain profile
// whose sources are of the project's kind (the files' extension), each with its default switches and the
// alternatives its profile lists ("try_switches": a compiler's switches that change the code - C 3.00's -Gs),
// or those --try names. --write sets the project's toolchain, variant and switches to the best.
//
// Typing notes (the Python duck-types its `prj`):
//   * `prj` is the interface Project below: ledger::Project (what check_file reads of a project, with sources(),
//     symbols(), switches() and dir()) and the one thing identify does to a project, assigning its switches
//     (set_switches).
//   * `tcm.load(n, v)` is the loader given to score() / main(): by default toolchain::load. (The Python module calls
//     tcm.load through its module, so a caller can replace it; here it is a parameter.)
//   * main() writes what the Python prints to `out`. random.Random(0).shuffle(files) is PyRandom below (the Mersenne
//     Twister and shuffle as CPython has them; the runtime has no `random`).
//   * --write reads project.json as JSON (py::V, a dict of arbitrary shape) and writes it back as json.dump(indent=1).
#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <tuple>

#include "codebreaker/exact/ledger.hpp"
#include "py.hpp"

namespace codebreaker::exact::identify {

// a project, as identify reads and changes it
class Project : public ledger::Project {
  public:
    virtual void set_switches(const std::optional<py::str>& switches) = 0;   // prj.switches = ...
};

// (toolchain name, variant or None, switches or None)
using Candidate = std::tuple<py::str, std::optional<py::str>, std::optional<py::str>>;
// a source file as prj.sources() lists it: (address or None, path)
using File = std::tuple<std::optional<int64_t>, py::str>;

using Loader = std::function<std::shared_ptr<ledger::Toolchain>(const py::str& name, const std::optional<py::str>& variant)>;

// toolchain.load
Loader default_loader();

py::list<Candidate> candidates(const Project& prj, const std::optional<py::list<py::str>>& tries = std::nullopt);

// (how many of the sample's functions come out the program's, "N of M" or the toolchain's error)
std::tuple<int64_t, py::str> score(Project& prj, const Candidate& cand, const py::list<File>& files, const Loader& load = default_loader());

// Python's random.Random(seed).shuffle(x) for an int seed
void shuffle(int64_t seed, py::list<File>& x);

int64_t main(Project& prj, const py::list<py::str>& argv, std::ostream& out = std::cout, const Loader& load = default_loader());

}  // namespace codebreaker::exact::identify

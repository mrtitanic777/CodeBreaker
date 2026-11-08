// codebreaker/verify/verdicts.hpp -- typed C++ port of codebreaker.verify.verdicts.
//
// verdicts.py -- one verdict vocabulary: what a judge says of a function or a source pair, binary or source alike.
//
//     V::EQUIVALENT, V::DIVERGENT, ...         the words a judge gives (stored as they are)
//     V::verdict("same")        -> "equivalent"     any older spelling, as the word now (map on read)
//     V::classify(verdict, limits)              -> (class, weak reasons): what it amounts to
//     V::TITLE(), V::LABEL(), V::SAID()         how each is written for people
//     V::match("does not compile") -> "no-compile"   a byte comparison's word
//     V::Index(module, rows).at(origin)         a function's row by its L2 provenance {module, va}
//
// Three things are said, each in one set of words.
//
// 1. What a JUDGE says of a pair (equivalent, consistent, divergent, inconclusive, no-compile, no-source).
// 2. What a verdict AMOUNTS TO, best first (the function store's classes): verified, weak, identified
//    (a runtime library's function, counted on its own), unverified, not-judged, no-compile, no-source.
// 3. What a BYTE comparison says: identical, same size, different, no original extent, no-compile;
//    identified (a runtime library's: linked, not written).
//
// Older spellings are read as the words now (verdict(), klass(), match()): a store or a cache written
// before keeps meaning what it meant.
//
// Notes on the port:
//   * Python answers verdict()/klass()/match()/classify() for a str from the C++ vocabulary
//     (native/engine/verdicts.cc) and keeps "a few lines" of its own for what is not a str. Here the
//     vocabulary is this file's own tables, and the one path a str takes is the Python fallback's, which
//     the C++ is held to (tests/test_verdicts.py): the harness compares both on the same words.
//   * A word that is not a str (Python's fallback for ints and the like) has no type here: the words are
//     std::optional<py::str>, None being nullopt.
//   * Verdict (a namedtuple) is a struct; the store's row ({addr, verdict, reason, limits, detail}) is Row.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <utility>

#include "py.hpp"

namespace codebreaker::verify::verdicts {

// ---- 1. what a judge says ----
extern const py::str EQUIVALENT, CONSISTENT, DIVERGENT, INCONCLUSIVE, NO_COMPILE, NO_SOURCE;
const py::list<py::str>& VERDICTS();
const py::list<py::str>& GOOD();                     // a verdict that is a match (weak or not)
// NOT_RUN = (INCONCLUSIVE, "judge: the emulator crashed on this function", [], "")
using NotRun = std::tuple<py::str, py::str, py::list<py::str>, py::str>;
const NotRun& NOT_RUN();
const py::dict<py::str, py::str>& OLD();             // every older spelling of a judge's word -> the word now
const py::dict<py::str, py::str>& SAID();            // how a judge's word is said in a report of pairs

// A judge's word as it is now (an older spelling mapped); None stays None, an unknown word stays itself.
std::optional<py::str> verdict(const std::optional<py::str>& word);

// The verdict of a yes/no comparison of a pair.
py::str pair(bool same, bool ran = true, bool built = true);

bool good(const std::optional<py::str>& word);

// ---- 2. what it amounts to ----
extern const py::str VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED, IDENTIFIED;
const py::list<py::str>& ORDER();                    // best first
const py::list<py::str>& SCORED();
const py::list<py::str>& BUILT();                    // compiled and run
const py::dict<py::str, py::str>& TITLE();
const py::dict<py::str, py::str>& LABEL();
const py::dict<py::str, py::str>& CLASS();
const py::dict<py::str, py::str>& WEAK_LIMITS();     // limits that make a match weaker than it looks
const py::dict<py::str, py::str>& CLASS_OLD();       // every older spelling of a class -> the class now

// A class as it is now: its TITLE or LABEL, or an older spelling, read back.
std::optional<py::str> klass(const std::optional<py::str>& word);

// (class, weak reasons) for one verdict and its limits.
std::pair<py::str, py::list<py::str>> classify(const std::optional<py::str>& word,
                                               const std::optional<py::str>& limits = std::nullopt);

// ---- 3. a byte comparison ----
extern const py::str IDENTICAL, SAME_SIZE, DIFFERENT, NO_EXTENT;
const py::list<py::str>& MATCHES();                  // best first
const py::dict<py::str, py::str>& MATCH_OLD();
const py::dict<py::str, py::str>& MATCH_LABEL();

// A byte comparison's word as it is now (an older spelling mapped).
std::optional<py::str> match(const std::optional<py::str>& word);

// ---- by provenance ----
// Verdict = namedtuple("Verdict", "module addr verdict reason limits detail")
struct Verdict {
    py::str module;
    std::optional<py::str> addr;
    std::optional<py::str> verdict;
    py::str reason;
    py::str limits;
    py::str detail;
};

// A store row ({addr, verdict, reason, limits, detail}); a key the row lacks, or holds as None, is nullopt.
struct Row {
    std::optional<py::str> addr, verdict, reason, limits, detail;
};

// A store row as a Verdict, its word as it is now.
Verdict of_row(const py::str& module, const Row& row);

// An L2 provenance {module, va}.
struct Origin {
    std::optional<py::str> module, va;
};

// A module's rows by L2 provenance: at({module, va}) -> the row of the function the node stands for, or
// None. The module writer reads a function's verdict so, from the tree's node, not by its name (a
// function renamed keeps its verdict).
class Index {
  public:
    Index(const py::str& module, const py::dict<py::str, Row>& rows);
    py::str module;
    std::optional<Row> at(const std::optional<Origin>& origin) const;
    Origin origin(const py::str& addr) const;

  private:
    py::dict<std::tuple<py::str, py::str>, Row> rows_;
};

}  // namespace codebreaker::verify::verdicts

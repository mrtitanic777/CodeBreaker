// codebreaker/source/placement.hpp -- typed C++ of codebreaker.source.placement
//
// placement.py -- where a module's source goes in the source tree.
//
//     python -m codebreaker place <module> ...      where each would go, and why
//     python -m codebreaker place --all             every module of the corpus (nothing is written)
//
//     placement::resolve("comctl32.dll", path)      ("shell/comctl32", "known")
//     placement::folder("comctl32.dll", path)       the folder itself (created on demand)
//
// There is one source tree per product: the folder that holds the corpus's `design` folder. A
// module's source lives in <tree>/<component>/<module>/. Where a module already has a folder
// anywhere in the tree, that is where it goes -- someone put it there. Otherwise the component is
// worked out, most certain first:
//
//     known      the module is named in the product's table (the system's core modules)
//     kind       what the file is settles it: a virtual device driver, a real-mode program
//     installed  the folder it is installed in settles it (I/O subsystem drivers, COMMAND)
//     suffix     its extension does (.acm, .cpl, .scr, .drv)
//     prefix     its name starts like a family the table knows (mci*, ras*, ole*, ...)
//     unsorted   nothing above applies
//
// The tables are per product, in placement.json beside this file.
//
// Typing notes:
//   * placement.json is read from <checkout root>/codebreaker/source/ (the checkout root is
//     py::settings::checkout_root(); the Python takes the folder from __file__).
//   * A product's table is the struct `Table`: each of its five entries is std::optional, empty when the
//     product's entry of the file lacks the key (the Python's KeyError, raised where the entry is used).
//     An entry of another shape than the file's (a dict of strings, a list of pairs of strings) is a
//     TypeError when the table is read; the file has none.
//   * existing(root) keeps its answer per folder for the process (the module's `_existing`), and hands the
//     same dict each time.
//   * main(argv): Python's main takes argv and prints; the exit status is the returned value.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::source::placement {

// the folders the search for existing module folders passes over
extern const py::set<py::str> _SKIP;

// One product's table of placement.json.
struct Table {
    std::optional<py::dict<py::str, py::str>> known, installed, kind, suffix;
    std::optional<py::list<std::tuple<py::str, py::str>>> prefix;   // [[prefix, component], ...]
};

// The source tree of the selected product.
py::str tree_root();

// The selected product's table, or the empty one for a product the file does not know.
Table _table();

// {module stem: folder relative to the tree} for every module folder already there.
py::dict<py::str, py::str> existing(const std::optional<py::str>& root = std::nullopt);

// -> (folder relative to the tree, how it was decided).
std::tuple<py::str, py::str> resolve(const py::str& module, const py::str& path = py::str(""));

// The folder a module's source goes in.
py::str folder(const py::str& module, const py::str& path = py::str(""), bool create = true);

// python -m codebreaker place <module> ... | --all
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::source::placement

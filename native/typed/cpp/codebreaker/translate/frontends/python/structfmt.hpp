// codebreaker/translate/frontends/python/structfmt.hpp -- C++ from codebreaker.translate.frontends.python.structfmt.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::translate::frontends::python::structfmt {

// one item: (code, size, kind, lo, hi). `hi` is unsigned because 'Q' reaches 2**64 - 1, past int64;
// every other lo is within int64 and every hi is non-negative.
using Item = std::tuple<py::str, int64_t, py::str, int64_t, uint64_t>;

// -> None when the format is refused. (The Python also answers None for a non-str argument; the typed
// signature takes a py::str, so that case cannot arise.) Raises IndexError for '' as the Python does.
std::optional<py::list<Item>> parse(const py::str& fmt);

int64_t size(const py::list<Item>& items);

// the items that are values (pads are not)
py::list<Item> values(const py::list<Item>& items);

}  // namespace codebreaker::translate::frontends::python::structfmt

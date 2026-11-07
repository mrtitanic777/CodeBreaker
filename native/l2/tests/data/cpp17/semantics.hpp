// snip.hpp -- C++ from snip by CodeBreaker's Python front end.
#pragma once

#include "py.hpp"

namespace snip {

class Shape;
class Rect;
struct Segment;

struct Segment {
    int64_t idx = 0;
    py::str kind;
    int64_t flags = 0;
};
template <class F> void for_each_field(const Segment& r, F&& f) {
    f("idx", r.idx);
    f("kind", r.kind);
    f("flags", r.flags);
}
template <class F> void for_each_field_mut(Segment& r, F&& f) {
    f("idx", r.idx, true);
    f("kind", r.kind, true);
    f("flags", r.flags, true);
}

class Shape : public py::object {
public:
    int64_t w = 0;

    explicit Shape(int64_t w);
    virtual int64_t area();
    py::str describe();
    virtual py::str name();
};

class Rect : public Shape {
public:
    int64_t h = 0;

    Rect(int64_t w, int64_t h);
    int64_t area() override;
    py::str name() override;
};

std::tuple<int64_t, int64_t> divs(int64_t a, int64_t b);
py::list<std::tuple<int64_t, int64_t>> run_divs();
int64_t shifts(int64_t a, int64_t n);
py::list<int64_t> run_shifts();
std::tuple<int64_t, int64_t, py::str, py::str, py::str, py::str, py::str, py::str, py::str, py::str, int64_t, py::str> run_index();
int64_t run_index_error();
std::tuple<py::list<py::str>, py::list<py::str>, py::list<int64_t>, int64_t, int64_t> run_dict();
std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, py::str> run_struct();
py::list<int64_t> run_from_bytes();
std::tuple<py::list<int64_t>, py::list<int64_t>, py::list<int64_t>> run_alias();
Segment segment(int64_t i, int64_t flags);
std::tuple<py::list<Segment>, py::list<py::str>> run_records();
py::list<py::str> run_classes();
std::tuple<py::list<py::str>, py::list<py::str>, py::list<py::str>> run_sort();
bool probe(py::list<int64_t> xs, int64_t i);
std::tuple<bool, bool, bool> run_any_all();
int64_t check(int64_t n);
py::list<int64_t> run_raise();
std::tuple<py::str, py::str, py::str, py::str, py::str, py::str, py::list<py::str>, py::str, py::str, py::str, int64_t> run_format();
std::tuple<int64_t, int64_t> run_counter();

}  // namespace snip

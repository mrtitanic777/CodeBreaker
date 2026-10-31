// snip.cpp -- C++ from snip by CodeBreaker's Python front end.

#include "snip.hpp"

namespace snip {

Shape::Shape(int64_t w) {
    this->w = w;
}

int64_t Shape::area() {
    return w * w;
}

py::str Shape::describe() {
    return py::percent("%s:%d", name(), area());
}

py::str Shape::name() {
    return "square";
}

Rect::Rect(int64_t w, int64_t h) : Shape(w) {
    this->h = h;
}

int64_t Rect::area() {
    return w * h;
}

py::str Rect::name() {
    return "rect";
}

std::tuple<int64_t, int64_t> divs(int64_t a, int64_t b) {
    return std::make_tuple(py::floordiv(a, b), py::mod(a, b));
}

py::list<std::tuple<int64_t, int64_t>> run_divs() {
    return py::list<std::tuple<int64_t, int64_t>>{divs(-7, 2), divs(7, -2), divs(-7, -2), divs(7, 2), divs(-1, 3), divs(0, -5)};
}

int64_t shifts(int64_t a, int64_t n) {
    return py::shr(a, n);
}

py::list<int64_t> run_shifts() {
    return py::list<int64_t>{shifts(-9, 1), shifts(-1, 70), shifts(9, 2), shifts(-256, 4)};
}

std::tuple<int64_t, int64_t, py::str, py::str, py::str, py::str, py::str, py::str, py::str, py::str, int64_t, py::str> run_index() {
    py::bytes b = py::bytes("ABCDEFG");
    py::str s = "h\303\251llo w\303\266rld";
    return std::make_tuple(b[-1], b[0], b.slice(1, 3).decode("latin1", "strict"), b.slice_from(-3).decode("latin1", "strict"), b.slice(5, 100).decode("latin1", "strict"), b.slice(4, 2).decode("latin1", "strict"), s[-1], s.slice(1, 4), s.slice_to(-6), s.slice(py::none, py::none, 2), py::len(s), b.slice(py::none, py::none, -1).decode("latin1", "strict"));
}

int64_t run_index_error() {
    py::bytes b = py::bytes("AB");
    return b[2];
}

std::tuple<py::list<py::str>, py::list<py::str>, py::list<int64_t>, int64_t, int64_t> run_dict() {
    py::dict<py::str, int64_t> d;
    d.set("z", 1);
    d.set("a", 2);
    d.set("m", 3);
    d.set("a", 4);
    py::list<py::str> out;
    for (const py::str& k : d) {
        out.append(k);
    }
    d.pop(py::str("z"));
    d.set("z", 9);
    return std::make_tuple(out, py::list<py::str>::from(d.keys()), py::list<int64_t>::from(d.values()), py::len(d), d.get(py::str("q"), -1));
}

std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, py::str> run_struct() {
    py::bytes data = py::pack("<HhI", 0xBEEF, -2, 0x12345678) + py::bytes("\377\376", 2);
    auto [a, b, c] = py::unpack_from<int64_t, int64_t, int64_t>("<HhI", data, 0);
    int64_t d = py::unpack_from1<int64_t>(">H", data, 8);
    int64_t e = py::unpack_from1<int64_t>("<h", data, -2);
    return std::make_tuple(a, b, c, d, e, py::len(data), data.hex());
}

py::list<int64_t> run_from_bytes() {
    py::bytes b = py::bytes("\001\002\377\377", 4);
    return py::list<int64_t>{py::from_bytes(b.slice(0, 2), "little", false), py::from_bytes(b.slice(0, 2), "big", false), py::from_bytes(b.slice(2, 4), "little", true), py::from_bytes(b.slice(2, 4), "little", false)};
}

std::tuple<py::list<int64_t>, py::list<int64_t>, py::list<int64_t>> run_alias() {
    py::list<int64_t> a = py::list<int64_t>{1, 2};
    py::list<int64_t> b = a;
    b.append(3);
    py::list<int64_t> c = py::list<int64_t>::from(a);
    c.append(4);
    return std::make_tuple(a, b, c);
}

Segment segment(int64_t i, int64_t flags) {
    return Segment{i + 1, (flags & 1) != 0 ? "DATA" : "CODE", flags};
}

std::tuple<py::list<Segment>, py::list<py::str>> run_records() {
    py::list<Segment> segs;
    for (const auto& [i, f] : py::enumerate(py::list<int64_t>{0, 1, 0x101})) {
        segs.append(segment(i, f));
    }
    py::list<py::str> names;
    for (const Segment& s : segs) {
        if ((s.flags & 0x100) == 0) {
            names.append(s.kind);
        }
    }
    return std::make_tuple(segs, names);
}

py::list<py::str> run_classes() {
    py::list<py::ref<Shape>> shapes = py::list<py::ref<Shape>>{std::make_shared<Shape>(3), std::make_shared<Rect>(2, 5)};
    py::list<py::str> result;
    for (const py::ref<Shape>& s : shapes) {
        result.append(s->describe());
    }
    return result;
}

std::tuple<py::list<py::str>, py::list<py::str>, py::list<py::str>> run_sort() {
    py::list<py::str> words = py::list<py::str>{py::str("pear"), py::str("Fig"), py::str("apple"), py::str("fig"), py::str("Pear"), py::str("kiwi")};
    py::list<py::str> by_len = py::sorted(words, [&](const py::str& w) { return py::len(w); }, false);
    return std::make_tuple(py::sorted(words), by_len, py::sorted(words, [&](const py::str& w) { return w.lower(); }, true));
}

bool probe(py::list<int64_t> xs, int64_t i) {
    return xs[i] > 0;
}

std::tuple<bool, bool, bool> run_any_all() {
    py::list<int64_t> xs = py::list<int64_t>{3, 0, -1};
    bool found = false;
    for (int64_t x : xs) {
        if (x < 0) {
            found = true;
            break;
        }
    }
    bool every_2 = true;
    for (int64_t x : xs) {
        if (!(x > 0)) {
            every_2 = false;
            break;
        }
    }
    bool found_3 = false;
    for (int64_t i = 0; i < 5; ++i) {
        if (probe(xs, i)) {
            found_3 = true;
            break;
        }
    }
    return std::make_tuple(found, every_2, found_3);
}

int64_t check(int64_t n) {
    if (n < 0) {
        throw py::ValueError(py::percent("negative: %d", n));
    }
    return n * 2;
}

py::list<int64_t> run_raise() {
    py::list<int64_t> out;
    for (int64_t n : py::list<int64_t>{1, -1, 2}) {
        try {
            out.append(check(n));
        } catch (const py::ValueError&) {
            out.append(-100);
        }
    }
    return out;
}

std::tuple<py::str, py::str, py::str, py::str, py::str, py::str, py::list<py::str>, py::str, py::str, py::str, int64_t> run_format() {
    int64_t n = 255;
    py::str name = "NE";
    return std::make_tuple(py::str("0x") + py::format(n, "x"), py::format(n, "08X"), py::repr(name), py::percent("%-5s|%3d|%#x", name, 7, 26), py::to_str(-3), py::repr(py::bytes("a\000'", 3)), py::split(py::str("a,b,,c"), py::str(",")), py::join(py::str("-"), py::list<py::str>{py::str("x"), py::str("y")}), py::str("  pad ").strip(), py::str("MiXeD").upper(), py::str("Abc").find(py::str("c")));
}

std::tuple<int64_t, int64_t> run_counter() {
    int64_t total = 0;
    int64_t i = 0;
    while (i < 100) {
        total += i;
        i += 3;
    }
    return std::make_tuple(total, i);
}

}  // namespace snip

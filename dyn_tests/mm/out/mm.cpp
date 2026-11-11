// mm -- mm.py translated by pyengine --dynamic (C++ over py::var, native/pyrt).
// Each function keeps its Python name; the module's statements are py_init_ at the end.
#include "py_object.hpp"

namespace pymod_mm {

static void py_init_();
static py::module_def py_module_("mm", py_init_, false, "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\.claude\\worktrees\\typed-1106\\dyn_tests\\mm\\mm.py");

// module names
static py::var& Point = py_module_.global("Point");
static py::var& chosen = py_module_.global("chosen");
static py::var& loop_fn = py_module_.global("loop");
static py::var& mode = py_module_.global("mode");
static py::var& other = py_module_.global("other");
static py::var& what_fn = py_module_.global("what");
static py::Obj* what_def_ = nullptr;
static py::Obj* loop_def_ = nullptr;

static py::var Point__class;   // class Point (for super() and __class__)

static py::var Point____init__(py::var self, py::var x, py::var py_y);
static py::var what(py::var v);
static py::var loop(py::var xs);

// def __init__(self, x, y):
static py::var Point____init__(py::var self, py::var x, py::var py_y) {
    {
        py::var py_v2 = py::tuple({x, py_y});
        { auto py_u3 = py::unpack(py_v2, 2); self.setattr("x", py_u3[0]); self.setattr("y", py_u3[1]); }
    }
    return py::None;
}

// def what(v):
static py::var what(py::var v) {
    py::var first, others, rest, py_small, x, py_y, yy;
    {   // match v:
        py::var py_subj4 = v;
        bool py_matched4 = false;
        if (!py_matched4) {   // case 0 | 1 as small:
            bool py_ok5 = true;
            bool py_any6 = false;
            if (!py_any6) {
                bool py_ok7 = true;
                if (!(py_subj4 == 0)) py_ok7 = false;
                if (py_ok7) py_any6 = true;
            }
            if (!py_any6) {
                bool py_ok8 = true;
                if (!(py_subj4 == 1)) py_ok8 = false;
                if (py_ok8) py_any6 = true;
            }
            if (!py_any6) py_ok5 = false;
            if (py_ok5) py_small = py_subj4;
            if (py_ok5) {
                py_matched4 = true;
                return ("small %d"_s % py_small);
            }
        }
        if (!py_matched4) {   // case None:
            bool py_ok9 = true;
            if (!py::is(py_subj4, py::None)) py_ok9 = false;
            if (py_ok9) {
                py_matched4 = true;
                return "none"_s;
            }
        }
        if (!py_matched4) {   // case [first, *rest] if rest:
            bool py_ok10 = true;
            py::var py_seq11 = py::match_sequence_(py_subj4, 1, 0, true);
            if (py_seq11.is_none()) py_ok10 = false;
            if (py_ok10) {
                py::var py_el12 = py_seq11.getitem(py::var(0));
                if (py_ok10) first = py_el12;
            }
            if (py_ok10) rest = py_seq11.getitem(py::var(1));
            if (py_ok10 && py::truthy_(rest)) {
                py_matched4 = true;
                return py::tuple({"seq"_s, first, rest});
            }
        }
        if (!py_matched4) {   // case {'kind': 'pt', **others}:
            bool py_ok13 = true;
            py::var py_map14 = py::match_mapping_(py_subj4, py::tuple({"kind"_s}), true);
            if (py_map14.is_none()) py_ok13 = false;
            if (py_ok13) {
                py::var py_mv15 = py_map14.getitem(py::var(0));
                if (!(py_mv15 == "pt"_s)) py_ok13 = false;
            }
            if (py_ok13) others = py_map14.getitem(py::var(1));
            if (py_ok13) {
                py_matched4 = true;
                return py::tuple({"map"_s, others});
            }
        }
        if (!py_matched4) {   // case Point(0, y=yy):
            bool py_ok16 = true;
            py::var py_cls17 = py::match_class_(py_subj4, Point, 1, py::tuple({"y"_s}));
            if (py_cls17.is_none()) py_ok16 = false;
            if (py_ok16) {
                py::var py_ca18 = py_cls17.getitem(py::var(0));
                if (!(py_ca18 == 0)) py_ok16 = false;
            }
            if (py_ok16) {
                py::var py_ca19 = py_cls17.getitem(py::var(1));
                if (py_ok16) yy = py_ca19;
            }
            if (py_ok16) {
                py_matched4 = true;
                return py::tuple({"on y"_s, yy});
            }
        }
        if (!py_matched4) {   // case Point(x, y):
            bool py_ok20 = true;
            py::var py_cls21 = py::match_class_(py_subj4, Point, 2, py::tuple({}));
            if (py_cls21.is_none()) py_ok20 = false;
            if (py_ok20) {
                py::var py_ca22 = py_cls21.getitem(py::var(0));
                if (py_ok20) x = py_ca22;
            }
            if (py_ok20) {
                py::var py_ca23 = py_cls21.getitem(py::var(1));
                if (py_ok20) py_y = py_ca23;
            }
            if (py_ok20) {
                py_matched4 = true;
                return py::tuple({"pt"_s, x, py_y});
            }
        }
        if (!py_matched4) {   // case str() | bytes():
            bool py_ok24 = true;
            bool py_any25 = false;
            if (!py_any25) {
                bool py_ok26 = true;
                py::var py_cls27 = py::match_class_(py_subj4, py::t::str, 0, py::tuple({}));
                if (py_cls27.is_none()) py_ok26 = false;
                if (py_ok26) py_any25 = true;
            }
            if (!py_any25) {
                bool py_ok28 = true;
                py::var py_cls29 = py::match_class_(py_subj4, py::t::bytes, 0, py::tuple({}));
                if (py_cls29.is_none()) py_ok28 = false;
                if (py_ok28) py_any25 = true;
            }
            if (!py_any25) py_ok24 = false;
            if (py_ok24) {
                py_matched4 = true;
                return "text"_s;
            }
        }
        if (!py_matched4) {   // case _:
            bool py_ok30 = true;
            if (py_ok30) {
                py_matched4 = true;
                return "other"_s;
            }
        }
    }
    return py::None;
}

// def loop(xs):
static py::var loop(py::var xs) {
    py::var out, x;
    out = py::list({});
    for (py::var py_it31 : py::each(xs)) {
        x = py_it31;
        {   // match x:
            py::var py_subj32 = x;
            bool py_matched32 = false;
            if (!py_matched32) {   // case 'stop':
                bool py_ok33 = true;
                if (!(py_subj32 == "stop"_s)) py_ok33 = false;
                if (py_ok33) {
                    py_matched32 = true;
                    break;
                }
            }
            if (!py_matched32) {   // case _:
                bool py_ok34 = true;
                if (py_ok34) {
                    py_matched32 = true;
                    out.call("append", x);
                }
            }
        }
    }
    return out;
}

// the module's statements, run on first import
static void py_init_() {
    // class Point:
    {
        py::var py_ns1 = py::new_dict();
        py_ns1.setitem("__module__"_s, "mm"_s);
        py_ns1.setitem("__qualname__"_s, "Point"_s);
        py::at(py_ns1, "__match_args__"_s) = py::tuple({"x"_s, "y"_s});
        py_ns1.setitem("__init__"_s, py::def("__init__", "self, x, y", {}, Point____init__, "Point.__init__", "mm"));
        Point__class = py::make_class("Point", "Point", {}, py_ns1, {}, "mm");
        Point = Point__class;
    }
    // def what(v):
    what_fn = py::def("what", "v", {}, what, "what", "mm");
    what_def_ = what_fn.ref();
    // def loop(xs):
    loop_fn = py::def("loop", "xs", {}, loop, "loop", "mm");
    loop_def_ = loop_fn.ref();
    mode = "a"_s;
    {   // match mode:
        py::var py_subj35 = mode;
        bool py_matched35 = false;
        if (!py_matched35) {   // case 'a':
            bool py_ok36 = true;
            if (!(py_subj35 == "a"_s)) py_ok36 = false;
            if (py_ok36) {
                py_matched35 = true;
                chosen = py::var(1);
            }
        }
        if (!py_matched35) {   // case other:
            bool py_ok37 = true;
            if (py_ok37) other = py_subj35;
            if (py_ok37) {
                py_matched35 = true;
                chosen = other;
            }
        }
    }
}

}  // namespace pymod_mm

// am2 -- am2.py translated by pyengine --dynamic (C++ over py::var, native/pyrt).
// Each function keeps its Python name; the module's statements are py_init_ at the end.
#include "py_object.hpp"

namespace pymod_am2 {

static void py_init_();
static py::module_def py_module_("am2", py_init_, false, "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\.claude\\worktrees\\typed-1106\\dyn_tests\\am2\\am2.py");

// module names
static py::var& f_fn = py_module_.global("f");
static py::Obj* f_def_ = nullptr;

static py::var f(py::var x);

// def f(x):
static py::var f(py::var x) {
    return py::None;
}

// the module's statements, run on first import
static void py_init_() {
    // def f(x):
    f_fn = py::def("f", "x", {}, f, "f", "am2");
    f_def_ = f_fn.ref();
}

}  // namespace pymod_am2

// am -- am.py translated by pyengine --dynamic (C++ over py::var, native/pyrt).
// Each function keeps its Python name; the module's statements are py_init_ at the end.
#include "py_object.hpp"

namespace pymod_am {

static void py_init_();
static py::module_def py_module_("am", py_init_, false, "C:\\Users\\mrtit\\Documents\\GitHub\\CodeBreaker\\.claude\\worktrees\\typed-1106\\dyn_tests\\am\\am.py");

// module names
static py::var& Lock = py_module_.global("Lock");
static py::var& agen = py_module_.global("agen");
static py::var& asyncio = py_module_.global("asyncio");
static py::var& collect = py_module_.global("collect");
static py::var& guarded = py_module_.global("guarded");
static py::var& leaf = py_module_.global("leaf");
static py::var& mid = py_module_.global("mid");
static py::var& plain_fn = py_module_.global("plain");
static py::Obj* plain_def_ = nullptr;

static py::var Lock__class;   // class Lock (for super() and __class__)

static py::var leaf_impl(py::yielder& y, py::var& x);
static py::var mid_impl(py::yielder& y, py::var& x);
static py::var agen_impl(py::yielder& y, py::var& n);
static py::var collect_impl(py::yielder& y);
static py::var Lock____aenter__(py::yielder& y, py::var& self);
static py::var Lock____aexit__(py::yielder& y, py::var& self, py::var& exc);
static py::var guarded_impl(py::yielder& y, py::var& lock);
static py::var plain();

// async def leaf(x):
static py::var leaf_impl(py::yielder& y, py::var& x) {
    return (x * 2);
}

// async def mid(x):
static py::var mid_impl(py::yielder& y, py::var& x) {
    py::var a;
    a = y.await_(leaf(x));
    return (a + y.await_(leaf(a)));
}

// async def agen(n):
static py::var agen_impl(py::yielder& y, py::var& n) {
    py::var i;
    for (py::var py_it1 : py::each(py::range(n))) {
        i = py_it1;
        y.await_(asyncio.call("sleep", py::var(0)));
        y(i);
    }
    return py::None;
}

// async def collect():
static py::var collect_impl(py::yielder& y) {
    py::var out, v;
    out = py::list({});
    bool py_broke2 = false;
    {   // async for async for v in agen(3):
        py::var py_it3a = py::aiter_(agen(py::var(3)));
        for (;;) {
            py::var py_it3;
            if (!y.async_next(py_it3a, py_it3)) break;
            v = py_it3;
            if ((v == 5)) {
                { py_broke2 = true; break; }
            }
            out.call("append", v);
        }
    }
    if (!py_broke2) {
        out.call("append", "done"_s);
    }
    return (out + [&](py::var py_src) -> py::var { py::var w; py::var py_out = py::list(); { py::var py_it4a = py::aiter_(py_src); for (;;) { py::var py_it4; if (!y.async_next(py_it4a, py_it4)) break; w = py_it4; if (py::truthy_(w)) { py_out.call("append", w); } } } return py_out; }(agen(py::var(2))));
}

// async def __aenter__(self):
static py::var Lock____aenter__(py::yielder& y, py::var& self) {
    return self;
}

// async def __aexit__(self, *exc):
static py::var Lock____aexit__(py::yielder& y, py::var& self, py::var& exc) {
    return py::var(false);
}

// async def guarded(lock):
static py::var guarded_impl(py::yielder& y, py::var& lock) {
    py::var held;
    {   // async with async with lock as held, Lock():
        py::var py_mgr6 = lock;
        py::var py_exitfn6 = py::awith_exit_of(py_mgr6);
        held = y.await_(py::awith_enter(py_mgr6));
        py::var py_ret6; int py_exit6 = 0; bool py_exited6 = false; py::error py_err6{py::None};
        try {
            {   // async with ...
                py::var py_mgr7 = Lock();
                py::var py_exitfn7 = py::awith_exit_of(py_mgr7);
                y.await_(py::awith_enter(py_mgr7));
                py::var py_ret7; int py_exit7 = 0; bool py_exited7 = false; py::error py_err7{py::None};
                try {
                    { py_ret7 = y.await_(mid(py::var(1))); py_exit7 = 1; goto py_fin7; }
                } catch (py::error& e7) {
                    py_exited7 = true;
                    py_err7 = e7;
                }
                if (py_exited7) {
                    py::handling_ py_h7(py_err7);
                    if (!py::truthy_(y.await_(py::with_exit_call(py_exitfn7, py_err7)))) throw py_err7;
                }
                py_fin7:
                if (!py_exited7) y.await_(py_exitfn7(py::None, py::None, py::None));
                if (py_exit7 == 1)
                    { py_ret6 = py_ret7; py_exit6 = 1; goto py_fin6; }
            }
        } catch (py::error& e6) {
            py_exited6 = true;
            py_err6 = e6;
        }
        if (py_exited6) {
            py::handling_ py_h6(py_err6);
            if (!py::truthy_(y.await_(py::with_exit_call(py_exitfn6, py_err6)))) throw py_err6;
        }
        py_fin6:
        if (!py_exited6) y.await_(py_exitfn6(py::None, py::None, py::None));
        if (py_exit6 == 1)
            return py_ret6;
    }
    return py::None;
}

// def plain():
static py::var plain() {
    py::var inner;
    // async def inner():
    inner = py::def_coroutine("inner", "", {}, [](py::yielder& y) -> py::var {
        return py::var(1);
    }, "plain.<locals>.inner", "am");
    return inner;
}

// the module's statements, run on first import
static void py_init_() {
    asyncio = py::import("asyncio");
    // async def leaf(x):
    leaf = py::def_coroutine("leaf", "x", {}, leaf_impl, "leaf", "am");
    // async def mid(x):
    mid = py::def_coroutine("mid", "x", {}, mid_impl, "mid", "am");
    // async def agen(n):
    agen = py::def_async_generator("agen", "n", {}, agen_impl, "agen", "am");
    // async def collect():
    collect = py::def_coroutine("collect", "", {}, collect_impl, "collect", "am");
    // class Lock:
    {
        py::var py_ns5 = py::new_dict();
        py_ns5.setitem("__module__"_s, "am"_s);
        py_ns5.setitem("__qualname__"_s, "Lock"_s);
        py_ns5.setitem("__aenter__"_s, py::def_coroutine("__aenter__", "self", {}, Lock____aenter__, "Lock.__aenter__", "am"));
        py_ns5.setitem("__aexit__"_s, py::def_coroutine("__aexit__", "self, *exc", {}, Lock____aexit__, "Lock.__aexit__", "am"));
        Lock__class = py::make_class("Lock", "Lock", {}, py_ns5, {}, "am");
        Lock = Lock__class;
    }
    // async def guarded(lock):
    guarded = py::def_coroutine("guarded", "lock", {}, guarded_impl, "guarded", "am");
    // def plain():
    plain_fn = py::def("plain", "", {}, plain, "plain", "am");
    plain_def_ = plain_fn.ref();
}

}  // namespace pymod_am

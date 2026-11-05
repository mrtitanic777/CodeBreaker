// pyrt_async_tests.cpp -- coroutines, async generators, async for / async with and the match helpers,
// written in the shapes pyengine --dynamic emits (each check's Python is in the comment above it; the
// wanted text is CPython's repr of the same).
#include <cstdio>
#include <string>

#include "py_object.hpp"

static int g_fail = 0, g_n = 0;
static void check(const py::var& v, const char* want, const char* what) {
    g_n++;
    std::string got = py::repr(v).str();
    if (got != want) {
        g_fail++;
        std::printf("FAIL %s: got %s, want %s\n", what, got.c_str(), want);
    }
}
#define CHECK(expr, want) check((expr), want, #expr)
using py::var;

// def drive(c):  -- run a coroutine with no event loop: send(None) until StopIteration
//     try:
//         while True: c.send(None)
//     except StopIteration as e: return e.value
static var drive(var c) {
    try {
        for (;;) c.call("send", py::None);
    } catch (py::error& e) {
        if (e.matches(py::t::StopIteration)) return e.value.attr("value");
        throw;
    }
}

int main(int argc, char** argv) {
    py::init_runtime(argc, argv);
    try {
        // async def leaf(x): return x * 2
        var leaf = py::def_coroutine("leaf", "x", {}, [](py::yielder& y, var x) -> var { (void)y; return x * 2; });
        // async def mid(x):
        //     a = await leaf(x); b = await leaf(a); return a + b
        var mid = py::def_coroutine("mid", "x", {}, [leaf](py::yielder& y, var x) -> var {
            var a = y.await_(leaf(x));
            var b = y.await_(leaf(a));
            return a + b;
        });
        CHECK(drive(mid(var(3))), "18");
        CHECK(py::type(mid(var(1))).attr("__name__"), "'coroutine'");
        CHECK(py::import("inspect").call("iscoroutinefunction", mid), "True");
        CHECK(py::import("inspect").call("iscoroutinefunction", py::def("f", "", {}, []() { return var(py::None); })), "False");
        CHECK(py::isinstance(mid(var(1)), py::import("collections.abc").attr("Coroutine")), "True");

        // class Sus:
        //     def __await__(self):
        //         v = yield 'ping'
        //         return v + 1
        var ns = py::new_dict();
        ns.setitem("__await__"_s, py::def_generator("__await__", "self", {}, [](py::yielder& y, var self) -> var {
            (void)self;
            var v = y("ping"_s);
            return v + 1;
        }));
        var Sus = py::make_class("Sus", "Sus", {}, ns);
        // async def uses(): return await Sus()
        var uses = py::def_coroutine("uses", "", {}, [Sus](py::yielder& y) -> var { return y.await_(Sus()); });
        var c = uses();
        CHECK(c.call("send", py::None), "'ping'");
        try {
            c.call("send", var(41));
            CHECK("no StopIteration"_s, "''");
        } catch (py::error& e) {
            CHECK(e.value.attr("value"), "42");
        }
        // a finished coroutine cannot be awaited again
        try {
            c.call("send", py::None);
        } catch (py::error& e) {
            CHECK(py::repr(e.value), "\"RuntimeError('cannot reuse already awaited coroutine')\"");
        }
        // c.throw(ValueError('x')) while it awaits Sus(): raised at Sus's yield, not caught there
        c = uses();
        c.call("send", py::None);
        try {
            c.call("throw", py::t::ValueError("x"_s));
        } catch (py::error& e) {
            CHECK(py::repr(e.value), "\"ValueError('x')\"");
        }

        // async def agen(n):
        //     for i in range(n):
        //         await leaf(i)
        //         yield i
        var agen = py::def_async_generator("agen", "n", {}, [leaf](py::yielder& y, var n) -> var {
            for (var i : py::each(py::range(n))) {
                y.await_(leaf(i));
                y(i);
            }
            return py::None;
        });
        CHECK(py::type(agen(var(1))).attr("__name__"), "'async_generator'");
        CHECK(py::import("inspect").call("isasyncgenfunction", agen), "True");
        // async def collect():
        //     out = []
        //     async for v in agen(3): out.append(v)
        //     return out
        var collect = py::def_coroutine("collect", "", {}, [agen](py::yielder& y) -> var {
            var out = py::list({});
            {
                var ita = py::aiter_(agen(var(3)));
                for (;;) {
                    var it;
                    if (!y.async_next(ita, it)) break;
                    var v = it;
                    out.call("append", v);
                }
            }
            return out;
        });
        CHECK(drive(collect()), "[0, 1, 2]");
        // async def comp(): return [v * 10 async for v in agen(3) if v]
        var comp = py::def_coroutine("comp", "", {}, [agen](py::yielder& y) -> var {
            return [&](var src) -> var {
                var out = py::list({});
                { var ita = py::aiter_(src); for (;;) { var it; if (!y.async_next(ita, it)) break; var v = it; if (py::truthy_(v)) { out.call("append", v * 10); } } }
                return out;
            }(agen(var(3)));
        });
        CHECK(drive(comp()), "[10, 20]");

        // log = []
        // class ACM:
        //     async def __aenter__(self): log.append('enter'); return 'v'
        //     async def __aexit__(self, t, v, tb): log.append(('exit', t is None)); return True
        var log = py::list({});
        var ans = py::new_dict();
        ans.setitem("__aenter__"_s, py::def_coroutine("__aenter__", "self", {}, [log](py::yielder& y, var self) -> var {
            (void)y; (void)self;
            log.call("append", "enter"_s);
            return "v"_s;
        }));
        ans.setitem("__aexit__"_s, py::def_coroutine("__aexit__", "self, t, v, tb", {}, [log](py::yielder& y, var self, var t, var v, var tb) -> var {
            (void)y; (void)self; (void)v; (void)tb;
            log.call("append", py::tuple({"exit"_s, var(t.is_none())}));
            return var(true);
        }));
        var ACM = py::make_class("ACM", "ACM", {}, ans);
        // async def withs():
        //     async with ACM() as v:
        //         log.append(v)
        //         raise ValueError
        //     return 'after'
        var withs = py::def_coroutine("withs", "", {}, [ACM, log](py::yielder& y) -> var {
            var v;
            {
                var py_mgr1 = ACM();
                var py_exitfn1 = py::awith_exit_of(py_mgr1);
                v = y.await_(py::awith_enter(py_mgr1));
                var py_ret1; int py_exit1 = 0; bool py_exited1 = false; py::error py_err1{py::None};
                (void)py_ret1; (void)py_exit1;
                try {
                    log.call("append", v);
                    py::raise(py::t::ValueError);
                } catch (py::error& e1) {
                    py_exited1 = true;
                    py_err1 = e1;
                }
                if (py_exited1) {
                    py::handling_ py_h1(py_err1);
                    if (!py::truthy_(y.await_(py::with_exit_call(py_exitfn1, py_err1)))) throw py_err1;
                }
                if (!py_exited1) y.await_(py_exitfn1(py::None, py::None, py::None));
            }
            return "after"_s;
        });
        CHECK(drive(withs()), "'after'");
        CHECK(log, "['enter', 'v', ('exit', False)]");
        // an object without __aenter__ in async with
        try {
            py::awith_exit_of(var(1));
        } catch (py::error& e) {
            CHECK(py::repr(e.value), "'TypeError(\"\\'int\\' object does not support the asynchronous context manager protocol\")'");
        }

        // match: [a, *rest, b] / {'a': x, **rest} / Point(x, y) / int(n) / str is not a sequence
        CHECK(py::match_sequence_(py::list({var(1), var(2), var(3), var(4)}), 1, 1, true), "[1, [2, 3], 4]");
        CHECK(py::match_sequence_(py::tuple({var(1), var(2)}), 2, 0, false), "[1, 2]");
        CHECK(py::match_sequence_(py::tuple({var(1), var(2)}), 3, 0, false), "None");
        CHECK(py::match_sequence_("ab"_s, 2, 0, false), "None");
        CHECK(py::match_mapping_(py::dict({{"a"_s, var(1)}, {"b"_s, var(2)}}), py::tuple({"a"_s}), true), "[1, {'b': 2}]");
        CHECK(py::match_mapping_(py::dict({{"a"_s, var(1)}}), py::tuple({"z"_s}), false), "None");
        var pns = py::new_dict();
        pns.setitem("__match_args__"_s, py::tuple({"x"_s, "y"_s}));
        pns.setitem("__init__"_s, py::def("__init__", "self, x, y", {}, [](var self, var x, var y) {
            self.setattr("x", x);
            self.setattr("y", y);
            return var(py::None);
        }));
        var Point = py::make_class("Point", "Point", {}, pns);
        CHECK(py::match_class_(Point(var(1), var(2)), Point, 2, py::tuple({})), "[1, 2]");
        CHECK(py::match_class_(Point(var(1), var(2)), Point, 0, py::tuple({"y"_s})), "[2]");
        CHECK(py::match_class_(var(5), py::t::int_, 1, py::tuple({})), "[5]");
        CHECK(py::match_class_(var(5), py::t::str, 1, py::tuple({})), "None");
        try {
            py::match_class_(Point(var(1), var(2)), Point, 3, py::tuple({}));
        } catch (py::error& e) {
            CHECK(py::repr(e.value), "\"TypeError('Point() accepts 2 positional sub-patterns (3 given)')\"");
        }
    } catch (py::error& e) {
        std::printf("uncaught: %s\n", e.what());
        return 1;
    }
    std::printf("%d/%d passed\n", g_n - g_fail, g_n);
    return g_fail ? 1 : 0;
}

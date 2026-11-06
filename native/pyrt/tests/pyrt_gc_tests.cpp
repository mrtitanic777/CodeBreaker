// pyrt_gc_tests.cpp -- the cycle collector (src/gc.cc): cycles of each kind of container freed, __del__
// once (from a count reaching 0, from a collection, after a resurrection), weak references and their
// callbacks (they die before finalizers run; a reference that is garbage itself calls nothing),
// closures that refer to themselves, generators in cycles, generations and thresholds, the gc module.
// Each line is what CPython prints for the same Python.
#include <cstdio>
#include <string>

#include "py_object.hpp"
#include "py_emit.hpp"

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

static var gc, weakref;
static var log_;     // what finalizers and callbacks did, in order
static var saved;    // where Saver.__del__ puts its object

// Node: a plain class. Logged(name): its __del__ logs "del <name>". Saver: its __del__ stores itself away
static var Node, Logged, Saver;
static void make_classes() {
    log_ = py::list();
    saved = py::list();
    Node = py::make_class("Node", "Node", {}, py::new_dict());
    var ls = py::new_dict();
    ls.setitem("__init__"_s, py::def("__init__", "self, name", {}, [](var self, var name) {
        self.setattr("name", name);
        return var(py::None);
    }));
    ls.setitem("__del__"_s, py::def("__del__", "self", {}, [](var self) {
        log_.call("append", "del "_s + self.attr("name"));
        return var(py::None);
    }));
    Logged = py::make_class("Logged", "Logged", {}, ls);
    var ss = py::new_dict();
    ss.setitem("__del__"_s, py::def("__del__", "self", {}, [](var self) {
        log_.call("append", "saved"_s);
        saved.call("append", self);
        return var(py::None);
    }));
    Saver = py::make_class("Saver", "Saver", {}, ss);
}
static var collect() { return gc.call("collect"); }
static var cb(const char* what) {
    std::string w = what;
    return py::native("cb", [w](const py::CallArgs&) {
        log_.call("append", py::str_from(w));
        return var(py::None);
    });
}
static var take_log() {
    var out = py::list_from(py::list_items(log_));
    log_.call("clear");
    return out;
}
static var alive(const var& w) { return var(!w().is_none()); }

int main(int argc, char** argv) {
    py::init_runtime(argc, argv);
    try {
        gc = py::import("gc");
        weakref = py::import("weakref");
        make_classes();
        collect();

        // ---------------------------------------------------------------- the module
        CHECK(gc.call("isenabled"), "True");
        CHECK(gc.call("get_threshold"), "(2000, 10, 10)");
        gc.call("set_threshold", var(500), var(5), var(5));
        CHECK(gc.call("get_threshold"), "(500, 5, 5)");
        gc.call("set_threshold", var(2000), var(10), var(10));
        gc.call("disable");
        CHECK(gc.call("isenabled"), "False");
        gc.call("enable");
        CHECK(py::len(gc.call("get_count")), "3");
        CHECK(py::len(gc.call("get_stats")), "3");
        CHECK(gc.call("is_tracked", py::list()), "True");
        CHECK(gc.call("is_tracked", var(3)), "False");
        CHECK(gc.call("is_tracked", "text"_s), "False");
        {
            // a tuple of plain values is not followed after its first collection; one holding a list is
            var plain = py::tuple({var(1), "a"_s}), holds = py::tuple({py::list()});
            CHECK(py::tuple({gc.call("is_tracked", plain), gc.call("is_tracked", holds)}), "(True, True)");
            collect();
            CHECK(py::tuple({gc.call("is_tracked", plain), gc.call("is_tracked", holds)}), "(False, True)");
        }
        {
            var l = py::list({var(1)});
            CHECK(var(py::in(l, gc.call("get_objects"))), "True");
            var holder = py::list({l});
            CHECK(var(py::in(holder, gc.call("get_referrers", l))), "True");
            CHECK(gc.call("get_referents", holder), "[[1]]");
        }

        // ---------------------------------------------------------------- cycles
        {
            // a list that contains itself, through an instance
            var l = py::list();
            var n = Node();
            n.setattr("l", l);
            l.call("append", n);
            l.call("append", l);
            var w = weakref.call("ref", n);
            n = py::None;
            l = py::None;
            CHECK(alive(w), "True");      // (counting alone cannot free it)
            CHECK(var(collect().as_int64() >= 3), "True");
            CHECK(alive(w), "False");
        }
        {
            // two instances that point at each other, through their dicts
            var a = Node(), b = Node();
            a.setattr("other", b);
            b.setattr("other", a);
            var wa = weakref.call("ref", a), wb = weakref.call("ref", b);
            a = py::None;
            b = py::None;
            collect();
            CHECK(py::tuple({alive(wa), alive(wb)}), "(False, False)");
        }
        {
            // a dict, a tuple and a set in one cycle
            var n = Node(), m = Node();
            var d = py::dict({{"n"_s, n}});
            m.setattr("d", d);
            var t = py::tuple({m});
            var s = py::set({t});
            n.setattr("s", s);
            var w = weakref.call("ref", n);
            n = m = d = t = s = py::None;
            collect();
            CHECK(alive(w), "False");
        }
        {
            // an instance that keeps its own bound method
            var n = Node();
            var ns = py::new_dict();
            ns.setitem("m"_s, py::def("m", "self", {}, [](var self) { return self; }));
            var C = py::make_class("C", "C", {}, ns);
            var c = C();
            c.setattr("bound", c.attr("m"));
            var w = weakref.call("ref", c);
            c = py::None;
            collect();
            CHECK(alive(w), "False");
        }
        {
            // a nested function that calls itself: its cell holds it, it holds the cell
            var w;
            {
                py::cell rec_cell;
                var& rec = *rec_cell;
                rec = py::def("rec", "n", {}, [rec_cell](var n) -> var {
                    var& rec = *rec_cell;
                    return n.truthy() ? rec(n - 1) + 1 : var(0);
                });
                CHECK(rec(var(5)), "5");
                w = weakref.call("ref", rec);
            }
            CHECK(alive(w), "True");
            collect();
            CHECK(alive(w), "False");
        }
        {
            // an exception that holds itself in its args
            var n = Node();
            var exc = py::t::ValueError(n);
            n.setattr("exc", exc);
            var w = weakref.call("ref", n);
            n = exc = py::None;
            collect();
            CHECK(alive(w), "False");
        }

        // ---------------------------------------------------------------- __del__
        {
            // a count reaching 0 runs it at once
            var x = Logged("a"_s);
            x = py::None;
            CHECK(take_log(), "['del a']");
        }
        {
            // a cycle of two: each finalized once, by the collection
            var a = Logged("a"_s), b = Logged("b"_s);
            a.setattr("other", b);
            b.setattr("other", a);
            a = b = py::None;
            CHECK(take_log(), "[]");
            collect();
            var got = take_log();
            got.call("sort");
            CHECK(got, "['del a', 'del b']");
            collect();
            CHECK(take_log(), "[]");
        }
        {
            // resurrection: __del__ stores the object; it lives on, finalized, and is never finalized again
            var s = Saver();
            s.setattr("me", s);
            var w = weakref.call("ref", s);
            s = py::None;
            collect();
            CHECK(take_log(), "['saved']");
            CHECK(py::len(saved), "1");
            CHECK(gc.call("is_finalized", saved[var(0)]), "True");
            CHECK(alive(w), "False");        // (weak references died before the finalizer ran, as CPython's)
            saved.call("clear");
            collect();
            CHECK(take_log(), "[]");
        }
        {
            // a resurrected object that is dropped later is freed without a second __del__
            var s = Saver();
            s = py::None;
            CHECK(take_log(), "['saved']");
            var again = saved.call("pop");
            CHECK(gc.call("is_finalized", again), "True");
            again = py::None;
            CHECK(take_log(), "[]");
        }

        // ---------------------------------------------------------------- weak references
        {
            var n = Node();
            var w = weakref.call("ref", n);
            CHECK(var(py::is(w(), n)), "True");
            CHECK(var(py::is(weakref.call("ref", n), w)), "True");   // (plain references are shared)
            CHECK(weakref.call("getweakrefcount", n), "1");
            n = py::None;
            CHECK(w(), "None");
            CHECK(weakref.call("getweakrefcount", w), "0");
        }
        {
            // a callback, when the count reaches 0
            var n = Node();
            var w = weakref.call("ref", n, cb("gone"));
            n = py::None;
            CHECK(take_log(), "['gone']");
            CHECK(w(), "None");
        }
        {
            // in a collection: the callback runs before the finalizer
            var a = Logged("a"_s);
            a.setattr("me", a);
            var w = weakref.call("ref", a, cb("callback"));
            a = py::None;
            collect();
            CHECK(take_log(), "['callback', 'del a']");
        }
        {
            // a weak reference that is garbage too: its callback is not called
            var a = Node();
            a.setattr("me", a);
            a.setattr("w", weakref.call("ref", a, cb("never")));
            a = py::None;
            collect();
            CHECK(take_log(), "[]");
        }
        {
            try {
                weakref.call("ref", var(1));
                CHECK(var("no error"_s), "'TypeError'");
            } catch (py::error& e) {
                CHECK(py::str(e.value), "\"cannot create weak reference to 'int' object\"");
            }
        }
        {
            var n = Node();
            var w = weakref.call("ref", n);
            CHECK(var(py::hash_(w) == py::hash_(n)), "True");
            CHECK(var(w == weakref.call("ref", n, cb("x"))), "True");
            n = py::None;
            take_log();
        }
        {
            // the weak containers
            var d = weakref.call("WeakValueDictionary");
            var n = Node();
            d.setitem("a"_s, n);
            CHECK(py::len(d), "1");
            CHECK(var(py::is(d["a"_s], n)), "True");
            CHECK(var(py::in("a"_s, d)), "True");
            CHECK(d.call("keys"), "['a']");
            n = py::None;
            CHECK(py::len(d), "0");
            CHECK(var(py::in("a"_s, d)), "False");
            CHECK(d.call("get", "a"_s), "None");
        }
        {
            var d = weakref.call("WeakKeyDictionary");
            var k = Node();
            d.setitem(k, var(5));
            CHECK(d[k], "5");
            CHECK(py::len(d), "1");
            CHECK(var(py::in(var(3), d)), "False");
            k = py::None;
            CHECK(py::len(d), "0");
        }
        {
            var s = weakref.call("WeakSet");
            var a = Node(), b = Node();
            s.call("add", a);
            s.call("add", b);
            s.call("add", a);
            CHECK(py::len(s), "2");
            CHECK(var(py::in(a, s)), "True");
            a = py::None;
            CHECK(py::len(s), "1");
            s.call("discard", b);
            CHECK(py::len(s), "0");
        }
        {
            // a WeakValueDictionary held by its value and by a cycle of the value's own: all freed
            var d = weakref.call("WeakValueDictionary");
            var n = Node();
            n.setattr("d", d);
            n.setattr("me", n);
            d.setitem("me"_s, n);
            var wd = weakref.call("ref", d), wn = weakref.call("ref", n);
            d = n = py::None;
            collect();
            CHECK(py::tuple({alive(wd), alive(wn)}), "(False, False)");
        }
        {
            // weakref.finalize
            var n = Node();
            var f = weakref.call("finalize", n, cb("finalized"));
            CHECK(f.attr("alive"), "True");
            n = py::None;
            CHECK(take_log(), "['finalized']");
            CHECK(f.attr("alive"), "False");
            var m = Node();
            var g = weakref.call("finalize", m, cb("not run"));
            CHECK(py::len(g.call("detach")), "4");
            CHECK(g.call("detach"), "None");
            m = py::None;
            CHECK(take_log(), "[]");
        }

        // ---------------------------------------------------------------- generators
        {
            // a suspended generator in a cycle (an instance -> the generator -> its function -> the cell of the
            // closure -> the instance): closed by the collection, its handler runs, then all of it is freed.
            // (A cycle through what a suspended body holds on its own stack -- its arguments, its locals -- is
            // not found: that stack is C++'s)
            var w, wn;
            {
                py::cell n_cell;
                var& n = *n_cell;
                n = Node();
                var genf = py::def_generator("g", "", {}, [n_cell](py::yielder& y) -> var {
                    var& n = *n_cell;
                    try {
                        y(var(1));
                        y(n);
                    } catch (...) {
                        log_.call("append", "closed"_s);
                        throw;
                    }
                    return var(py::None);
                });
                var g = genf();
                CHECK(py::next(g), "1");
                n.setattr("g", g);
                w = weakref.call("ref", g);
                wn = weakref.call("ref", n);
            }
            CHECK(take_log(), "[]");
            collect();
            CHECK(take_log(), "['closed']");
            CHECK(py::tuple({alive(w), alive(wn)}), "(False, False)");
        }
        {
            // a generator method an instance keeps (self._it = self.walk()): the body's parameters are the
            // generator's slots (pyengine writes them as references), so the cycle through self is found
            var walk = py::def_generator("walk", "self", {}, [](py::yielder& y, var& self) -> var {
                y(var(1));
                y(self);
                return var(py::None);
            });
            var n = Node();
            n.setattr("it", walk(n));
            CHECK(py::next(n.attr("it")), "1");
            var w = weakref.call("ref", n);
            n = py::None;
            CHECK(alive(w), "True");
            collect();
            CHECK(alive(w), "False");
        }

        // ---------------------------------------------------------------- generations
        {
            // cycles made in a loop of calls are collected without gc.collect()
            var fin_count = py::list();
            var mk = py::def("mk", "i", {}, [](var i) {
                var n = Logged(py::str(i));
                n.setattr("me", n);
                return var(py::None);
            });
            for (int i = 0; i < 20000; i++) mk(var(i));
            CHECK(var(py::len_(log_) > 10000), "True");
            log_.call("clear");
            collect();
            log_.call("clear");
            CHECK(var(gc.call("get_count")[var(0)].as_int64() < 2000), "True");
        }
        {
            // gc.callbacks: start and stop of each collection
            var phases = py::list();
            var cbk = py::native("cbk", [phases](const py::CallArgs& a) {
                phases.call("append", py::tuple({a.pos.at(0), a.pos.at(1)["generation"_s]}));
                return var(py::None);
            });
            gc.attr("callbacks").call("append", cbk);
            gc.call("collect", var(1));
            gc.attr("callbacks").call("remove", cbk);
            CHECK(phases, "[('start', 1), ('stop', 1)]");
        }
        {
            // gc.freeze: the permanent generation is not collected
            var a = Node();
            a.setattr("me", a);
            var w = weakref.call("ref", a);
            a = py::None;
            gc.call("freeze");
            CHECK(var(gc.call("get_freeze_count").as_int64() > 0), "True");
            collect();
            CHECK(alive(w), "True");
            gc.call("unfreeze");
            CHECK(gc.call("get_freeze_count"), "0");
            collect();
            CHECK(alive(w), "False");
        }
        {
            // disabled: nothing is collected on its own, gc.collect() still collects
            gc.call("disable");
            var mk = py::def("mk2", "i", {}, [](var i) {
                var n = Logged(py::str(i));
                n.setattr("me", n);
                return var(py::None);
            });
            for (int i = 0; i < 5000; i++) mk(var(i));
            CHECK(py::len(log_), "0");
            collect();
            CHECK(py::len(log_), "5000");
            log_.call("clear");
            gc.call("enable");
        }
    } catch (py::error& e) {
        std::printf("uncaught: %s\n", e.what());
        return 1;
    }
    std::printf("%d/%d passed\n", g_n - g_fail, g_n);
    return g_fail ? 1 : 0;
}

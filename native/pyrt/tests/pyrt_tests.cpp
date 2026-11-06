// pyrt_tests.cpp -- the runtime's own checks: each line is a Python expression's result as CPython
// prints it, computed here through py::var.
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

int main(int argc, char** argv) {
    py::init_runtime(argc, argv);
    using py::var;
    try {
        CHECK(var(7) / var(2), "3.5");
        CHECK(py::floordiv(var(-7), var(2)), "-4");
        CHECK(var(-7) % var(3), "2");
        CHECK(py::pow(var(2), var(100)), "1267650600228229401496703205376");
        CHECK(var(INT64_MAX) + var(1), "9223372036854775808");
        CHECK(py::pow(var(2), var(64)) - var(1), "18446744073709551615");
        CHECK(var(0.1) + var(0.2), "0.30000000000000004");
        CHECK(var(1e16), "1e+16");
        CHECK(var(1.5e-5), "1.5e-05");
        CHECK("a,b,,c"_s.call("split", ","_s), "['a', 'b', '', 'c']");
        CHECK("  x y  "_s.call("split"), "['x', 'y']");
        CHECK("%5.2f|%-4d|%x|%r"_s % py::tuple({var(3.14159), var(7), var(255), "q"_s}), "\" 3.14|7   |ff|'q'\"");
        CHECK("{:>6}|{:08.3f}|{!r}"_s.call("format", "ab"_s, var(2.5), "x"_s), "\"    ab|0002.500|'x'\"");
        CHECK(py::set({var(3), var(1), var(2), var(100), var(-5)}), "{1, 2, 3, 100, -5}");
        var d = py::dict({{"b"_s, var(1)}, {"a"_s, var(2)}});
        d.setitem("c"_s, var(3));
        CHECK(d, "{'b': 1, 'a': 2, 'c': 3}");
        CHECK(py::hash(py::tuple({var(1), var(2)})), "-3550055125485641917");
        CHECK(py::hash(var(-1)), "-2");
        CHECK(py::hash(py::pow(var(2), var(61))), "1");
        var l = py::list({var(3), var(1), var(2)});
        l.call("sort", py::kw("reverse", true));
        CHECK(l, "[3, 2, 1]");
        CHECK(l.slice(var(py::None), var(py::None), var(-1)), "[1, 2, 3]");
        CHECK("héllo"_s.slice(1, 3), "'él'");
        CHECK(py::len("héllo"_s), "5");
        var re = py::import("re");
        var m = re.call("search", "(\\d+)-(\\w+)"_s, "x 12-ab y"_s);
        CHECK(m.call("groups"), "('12', 'ab')");
        CHECK(m.call("span"), "(2, 7)");
        CHECK(re.call("sub", "a(b)?"_s, "[\\1]"_s, "ab a"_s), "'[b] []'");
        var json = py::import("json");
        CHECK(json.call("dumps", py::dict({{"k"_s, py::list({var(1), var(2.5), py::None, true})}})), "'{\"k\": [1, 2.5, null, true]}'");
        CHECK(json.call("loads", "{\"a\": [1, \"x\\u00e9\"]}"_s), "{'a': [1, 'xé']}");
        var os = py::import("os");
        CHECK(os.attr("path").call("join", "a"_s, "b"_s, "c.txt"_s), "'a\\\\b\\\\c.txt'");
        CHECK(os.attr("path").call("splitext", "x/y.tar.gz"_s), "('x/y.tar', '.gz')");
        CHECK(os.attr("path").call("normpath", "a/./b/../c"_s), "'a\\\\c'");
        var st = py::import("struct");
        CHECK(st.call("pack", "<HI"_s, var(1), var(2)), "b'\\x01\\x00\\x02\\x00\\x00\\x00'");
        CHECK(st.call("unpack_from", "<h"_s, "\xfe\xff"_b), "(-2,)");
        // generators on fibers
        var gen = py::def_generator("g", "n", {}, [](py::yielder& y, var n) {
            for (var i : py::each(py::range(n))) y(i * i);
            return var(py::None);
        });
        CHECK(py::list_from(py::to_vector(gen(var(4)))), "[0, 1, 4, 9]");
        // classes, super, properties
        var ns = py::new_dict();
        ns.setitem("__init__"_s, py::def("__init__", "self, x", {}, [](var self, var x) { self.setattr("x", x); return var(py::None); }));
        ns.setitem("double"_s, py::def("double", "self", {}, [](var self) { return self.attr("x") * 2; }));
        var A = py::make_class("A", "A", {}, ns);
        var a = A(var(21));
        CHECK(a.call("double"), "42");
        CHECK(py::isinstance(a, A), "True");
        // a module the program does not have, from its source on sys.path: a package, a relative import
        // in it, a submodule found by `from pkg import name`, runpy.run_path of a file
        {
            auto put = [](const char* path, const char* text) {
                FILE* f = std::fopen(path, "wb");
                std::fputs(text, f);
                std::fclose(f);
            };
            os.call("makedirs", "srcmods\\srcpkg"_s, py::kw("exist_ok", true));
            put("srcmods\\srcpkg\\__init__.py", "\"\"\"the package\"\"\"\nfrom .leaf import twice\nVALUE = twice(21)\n");
            put("srcmods\\srcpkg\\leaf.py", "def twice(x):\n    return 2 * x\n");
            put("srcmods\\srcpkg\\other.py", "NAME = __name__\n");
            put("srcmods\\script.py", "import srcpkg\nfrom srcpkg import other\nRESULT = (srcpkg.VALUE, other.NAME, __name__)\n");
            py::import("sys").attr("path").call("insert", var(0), "srcmods"_s);
            var sp = py::import("srcpkg");
            CHECK(sp.attr("VALUE"), "42");
            CHECK(sp.attr("__doc__"), "'the package'");
            CHECK(sp.attr("leaf").attr("twice")(var(5)), "10");
            var g = py::import("runpy").call("run_path", "srcmods\\script.py"_s, py::kw("run_name", "__run__"_s));
            CHECK(g["RESULT"_s], "(42, 'srcpkg.other', '__run__')");
            py::import("shutil").call("rmtree", "srcmods"_s);
        }
        // contextlib.contextmanager is @wraps(func): the function's name and __wrapped__ on the helper
        {
            var fn = py::def("opened", "path", {}, [](var p) { return p; }, "opened", "mymod");
            var cm = py::import("contextlib").call("contextmanager", fn);
            CHECK(py::tuple({cm.attr("__name__"), cm.attr("__module__"), var(py::is(cm.attr("__wrapped__"), fn))}), "('opened', 'mymod', True)");
        }
        // exceptions
        try {
            py::list({}).call("pop");
        } catch (py::error& e) {
            CHECK(py::repr(e.value), "\"IndexError('pop from empty list')\"");
        }
        try {
            var k = py::dict({})["missing"_s];
            (void)k;
        } catch (py::error& e) {
            CHECK(py::str(e.value), "\"'missing'\"");
        }
    } catch (py::error& e) {
        std::printf("uncaught: %s\n", e.what());
        return 1;
    }
    std::printf("%d/%d passed\n", g_n - g_fail, g_n);
    return g_fail ? 1 : 0;
}

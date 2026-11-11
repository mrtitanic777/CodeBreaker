// The shim at the old header path (native/typed/cpp/codebreaker/exact/draft/shapes.hpp) over cb::exact::draft::shapes: it
// must compile with the old layer, forward, and keep the old layer's module tables (text::UCHAR, PTRSIZE, LONGINFO,
// RANGES) as the passes of forms/candidates/prove still read them. Built at /W3 (the old layer is not warning-clean).
#include <string>

#include "cb/core/testing.hpp"
#include "codebreaker/exact/draft/shapes.hpp"

namespace {

namespace old_shapes = codebreaker::exact::draft::shapes;
namespace old_text = codebreaker::exact::draft::text;
using cb::compat::from_py;

void clear_tables() {
    std::vector<std::variant<std::tuple<py::str, int64_t>, py::str>> items(old_text::UCHAR.begin(), old_text::UCHAR.end());
    for (const auto& x : items) old_text::UCHAR.discard(x);
    old_text::PTRSIZE.clear();
    old_text::LONGINFO.clear();
    std::vector<std::tuple<int64_t, int64_t, bool>> ranges(old_text::RANGES.begin(), old_text::RANGES.end());
    for (const auto& r : ranges) old_text::RANGES.discard(r);
}

CB_TEST("exact.draft.shapes.shim.forwards", [] {
    clear_tables();
    CB_CHECK_EQ(from_py(old_shapes::compare_values(py::str("    if ((a < b)) {\n        ax = 1;\n    } else {\n        ax = 0;\n    }"))),
                "    ax = (a < b);");
    const auto kept = old_shapes::goto_tails(py::str("    x = 1;"));
    CB_CHECK(!kept.has_value());
    const auto fl = old_shapes::for_loops(py::str("    i = 0;\n    while ((i < 5)) {\n        f(i);\n        i = i + 1;\n    }"));
    CB_CHECK(fl.has_value());
    CB_CHECK_EQ(from_py(*fl), "    for (i = 0; (i < 5); i = i + 1) {\n        f(i);\n    }");
    const py::list<py::str> swaps = old_shapes::adjacent_swaps(py::str("    a = 1;\n    b = 2;"));
    CB_CHECK_EQ(swaps.size(), int64_t{1});
    CB_CHECK_EQ(from_py(swaps[0]), "    b = 2;\n    a = 1;");
    CB_CHECK_EQ(from_py(old_shapes::_ternary(py::str("c"), py::str("X"), py::str("Y"))), "c ? X : Y");
});

CB_TEST("exact.draft.shapes.shim.records_what_the_passes_learn_in_the_module_tables", [] {
    clear_tables();
    CB_CHECK_EQ(from_py(old_shapes::zero_extends(py::str("    al = g_0010;\n    ah = 0;"))), "    ax = g_0010;");
    CB_CHECK_EQ(old_text::UCHAR.size(), int64_t{1});
    CB_CHECK_EQ(from_py(old_shapes::pointer_index(py::str("    bx = n;\n    bx = (bx << 1) + g_0020;\n    ax = *bx;"))), "    ax = *(g_0020 + n);");
    CB_CHECK_EQ(old_text::PTRSIZE.size(), int64_t{1});
    const auto x = old_shapes::_pair_long(py::str("p->f2"), py::str("p->f4"));
    CB_CHECK(x.has_value());
    CB_CHECK_EQ(from_py(*x), "*(long *)&p->f2");
    CB_CHECK(old_text::LONGINFO.fields.has_value() && old_text::LONGINFO.fields->size() == 1);
});

CB_TEST("exact.draft.shapes.shim.range_switches_reads_the_module_ranges", [] {
    clear_tables();
    const py::str chain("    ax = n;\n    if ((ax >= 2)) {\n        if ((ax > 3)) {\n            f();\n        } else {\n            g();\n        }\n    } else {\n        f();\n    }");
    const std::string plain = "    switch (n) {\n    default:\n        f();\n        break;\n    case 2:\n    case 3:\n        g();\n        break;\n    }";
    CB_CHECK_EQ(from_py(old_shapes::range_switches(chain)), plain);
    // the cases' arm first when the code shows it (a range the model marked "above")
    old_text::RANGES.add(std::make_tuple(int64_t{2}, int64_t{3}, true));
    CB_CHECK_NE(from_py(old_shapes::range_switches(chain)), plain);
    clear_tables();
});

}  // namespace

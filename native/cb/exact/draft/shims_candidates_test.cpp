// The shim at the old header path (native/typed/cpp/codebreaker/exact/draft/candidates.hpp) over
// cb::exact::draft::candidates: it must compile with the old layer, forward, and keep the old layer's module tables
// (FPINFO, FRAMEW ...) as the passes of prove still read them. Built at /W3 (the old layer is not warning-clean).
#include "cb/core/testing.hpp"
#include "codebreaker/exact/draft/candidates.hpp"

CB_TEST("exact.draft.candidates.shim.forwards_and_keeps_the_old_tables", [] {
    namespace old_cand = codebreaker::exact::draft::candidates;
    using cb::compat::from_py;
    CB_CHECK_EQ(from_py(old_cand::fill_frame(py::str("f(a)\nint a;\n{\n    int x;\n    x = a;\n}"), 6)), "f(a)\nint a;\n{\n    int x;\n    char _pf[4];\n    x = a;\n}");
    CB_CHECK(old_cand::void_variant(py::str("f()\n{\n    foo();\n    return bar(1);\n}")).has_value());
    // the 8087's conversion records the long it makes in the module table the later passes read
    old_cand::FPINFO.clear();
    CB_CHECK_EQ(from_py(old_cand::fp_conversions(py::str("    p->f0 = ftol(y);\n    p->f2 = dx;"), py::list<py::str>{py::str("ftol")})), "    *p = y;");
    CB_CHECK(old_cand::FPINFO.types.contains(py::str("*p")));
    old_cand::FPINFO.clear();
    // FRAMEW feeds frame_chars
    old_cand::FRAMEW.add(std::make_tuple(int64_t{-2}, py::str("byte")));
    CB_CHECK_EQ(from_py(old_cand::frame_chars(py::str("f()\n{\n    int v2;\n}"))), "f()\n{\n    char v2;\n}");
    old_cand::FRAMEW.discard(std::make_tuple(int64_t{-2}, py::str("byte")));
});

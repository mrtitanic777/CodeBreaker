// The shims at the old header paths (native/typed/cpp/codebreaker/exact/{linkwhole,data,library}.hpp) over the modules of
// this folder: they must compile with the old layer and forward to the new code. Built at /W3 (the old layer is not
// warning-clean). What needs the old toolchain, DOS emulator or omf ports (and so cannot link into cb_tests) is kept
// compiling by compat/shim_check_exact.cpp.
#include "cb/core/testing.hpp"
#include "codebreaker/exact/data.hpp"
#include "codebreaker/exact/library.hpp"
#include "codebreaker/exact/linkwhole.hpp"

namespace {

namespace old = codebreaker::exact;
using cb::compat::from_py;
using cb::compat::to_py;

// The part of an old-layer linkwhole project `compare` and `wanted` read.
class OldLinkProject final : public old::linkwhole::Project {
  public:
    old::linkwhole::LinkConfig config;
    cb::Bytes original;
    py::str dir() const override { return py::str("."); }
    py::str src() const override { return py::str("."); }
    py::list<std::tuple<int64_t, py::str>> sources() const override { return {}; }
    std::shared_ptr<old::modules::Toolchain> toolchain() const override { return nullptr; }
    py::dict<py::str, py::bytes> headers() const override { return {}; }
    py::list<py::str> include() const override { return {}; }
    std::optional<py::str> switches() const override { return std::nullopt; }
    py::list<std::shared_ptr<const old::linkwhole::Project>> sets() const override { return {}; }
    old::linkwhole::LinkConfig link() const override { return config; }
    py::bytes image() const override { return py::bytes(); }
    py::bytes file() const override { return to_py(cb::ByteView(original)); }
    py::str exe() const override { return py::str("x/PROG.EXE"); }
    old::linkemu::Symbols symbols() const override { return {}; }
};

}  // namespace

CB_TEST("exact.shims.linkwhole_config_and_compare", [] {
    OldLinkProject p;
    CB_CHECK(!old::linkwhole::wanted(p));
    p.config = old::linkwhole::link_config(py::str(R"({"linker": "L.EXE", "accept": [{"tail_zeros": 2, "why": "pad"}]})"));
    CB_CHECK(old::linkwhole::wanted(p));
    p.original = {'M', 'Z', 1, 2, 0, 0};
    const auto same = old::linkwhole::compare(p, to_py(cb::ByteView(cb::Bytes{'M', 'Z', 1, 2})));
    CB_CHECK(std::get<0>(same));
    CB_CHECK_EQ(std::get<1>(same).size(), 1);
    CB_CHECK_EQ(from_py(std::get<1>(same)[0]), "the original has 2 zero bytes more at its end (pad)");
    const auto differs = old::linkwhole::compare(p, to_py(cb::ByteView(cb::Bytes{'M', 'Z', 9, 2, 0, 0})));
    CB_CHECK(!std::get<0>(differs));
    // a damaged entry is a Python ValueError-like error for the old callers
    bool raised = false;
    try {
        (void)old::linkwhole::link_config(py::str(R"({"psp": -1})"));
    } catch (const py::Exception&) {
        raised = true;
    }
    CB_CHECK(raised);
});

CB_TEST("exact.shims.linkwhole_reclass_and_cstr", [] {
    const std::string obj_hex = "8002000000" "00";   // (not an object: reclass reads records only as far as they go)
    (void)obj_hex;
    const py::bytes none = old::linkwhole::reclass(py::bytes(std::string()), py::str("INIT"), py::list<py::str>{py::str("_TEXT")});
    CB_CHECK_EQ(none.size(), 0);
    CB_CHECK_EQ(from_py(old::data::cstr(py::bytes(std::string("a\"b\n\x01", 5)))), "\"a\\\"b\\n\\001\"");
});

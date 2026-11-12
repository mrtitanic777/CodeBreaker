// The shim at the old header path of formats.xbox.heavyiron: the old py:: calls over cb/formats/xbox/heavyiron.
#include "codebreaker/formats/xbox/heavyiron.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace hi = codebreaker::formats::xbox::heavyiron;

namespace {
py::str fixture(const char* name) { return cb::compat::to_py(cb::fs::to_text(cb::testing::data_path(std::string("formats/testdata/heavyiron/") + name))); }
}  // namespace

CB_TEST("compat.heavyiron.the_volume_listing_of_an_archive", [] {
    CB_CHECK(hi::is_archive(fixture("a1.ho")));
    CB_CHECK(!hi::is_archive(fixture("missing.ho")));
    const hi::VolumeListing vl = hi::volume_listing(fixture("a2.ho"));
    CB_CHECK_EQ(vl.entries.size(), 1);
    const auto& [name, size] = vl.entries[0];
    CB_CHECK_EQ(name.utf8(), "layer00/1234567890ABCDEF_BDE21A8D.bin");
    CB_CHECK_EQ(size, 77);
    CB_CHECK_EQ(vl.read(name).size(), 77);
    CB_CHECK_THROWS_AS(py::KeyError, (void)vl.read(py::str("layer00/nothing.bin")));
    CB_CHECK_THROWS_AS(py::FileNotFoundError, (void)hi::volume_listing(fixture("missing.ho")));
});

CB_TEST("compat.heavyiron.main_returns_the_status", [] {
    cb::testing::CapturedStd cap;
    CB_CHECK_EQ(hi::main(py::list<py::str>{py::str("info"), fixture("a2.ho")}), 0);
    CB_CHECK_EQ(hi::main(py::list<py::str>{py::str("nonsense")}), 2);
    CB_CHECK(cap.out().find("a2.ho: 10240 bytes, 1 layers, 1 assets") != std::string::npos);
});

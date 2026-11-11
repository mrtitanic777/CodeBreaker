// The shim at the old header path of formats.xbox.stfs: the old py:: class over cb/formats/xbox/stfs.
#include "codebreaker/formats/xbox/stfs.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace stfs = codebreaker::formats::xbox::stfs;

namespace {
std::filesystem::path fixture(const char* name) { return cb::testing::data_path(std::string("formats/testdata/stfs/") + name); }
}  // namespace

CB_TEST("compat.stfs.the_old_class_over_the_new_package", [] {
    cb::fs::ScopedTempDir dir("stfs-shim");
    const py::str out = cb::compat::to_py(cb::fs::to_text(dir.path() / "m.stfs"));
    const cb::Bytes tpl = cb::fs::read_file(fixture("tpl_con.bin"));
    stfs::build(cb::compat::to_py(cb::fs::to_text(fixture("fold_mixed"))), out, cb::compat::to_py(cb::ByteView(tpl)));
    const cb::Bytes made = cb::fs::read_file(dir.path() / "m.stfs");
    CB_CHECK(stfs::is_magic(py::bytes("LIVE")));
    CB_CHECK(!stfs::is_magic(py::bytes("NOPE")));

    stfs::STFS pkg(cb::compat::to_py(cb::ByteView(made)));
    CB_CHECK_EQ(pkg.magic.decode("latin-1").utf8(), "CON ");
    CB_CHECK_EQ(pkg.sex, 1);
    CB_CHECK_EQ(pkg.top_level, 0);
    CB_CHECK_EQ(pkg.alloc_blocks, 17);
    CB_CHECK_EQ(pkg.title_id.utf8(), "E21B0D08");
    CB_CHECK_EQ(pkg.content_type, 330542814);
    const auto& [start, blocks, size] = pkg.files.at(py::str("fff"));
    CB_CHECK_EQ(size, 4095);
    CB_CHECK_EQ(blocks, 1);
    CB_CHECK_EQ(pkg.read_file(py::str("fff")).size(), 4095);
    CB_CHECK_THROWS_AS(py::ValueError, (void)pkg.read_file(py::str("not here")));
    CB_CHECK_THROWS_AS(py::ValueError, (void)stfs::STFS(py::bytes("garbage")));
    CB_CHECK_EQ(pkg.extract_all(cb::compat::to_py(cb::fs::to_text(dir.path() / "x"))), pkg.files.size());
    (void)start;
});

CB_TEST("compat.stfs.resign_by_path_and_bytes", [] {
    cb::fs::ScopedTempDir dir("stfs-shim");
    const cb::Bytes tpl = cb::fs::read_file(fixture("tpl_live.bin"));
    const py::str pkg = cb::compat::to_py(cb::fs::to_text(dir.path() / "p.stfs"));
    stfs::build(cb::compat::to_py(cb::fs::to_text(fixture("fold_one"))), pkg, cb::compat::to_py(cb::ByteView(tpl)));
    const cb::Bytes kv = cb::fs::read_file(fixture("kv_good.bin"));
    const py::bytes kv_bytes = cb::compat::to_py(cb::ByteView(kv));
    const py::bytes result = stfs::resign(std::variant<py::bytes, py::str>(pkg), std::variant<py::bytes, py::str>(kv_bytes));
    CB_CHECK_EQ(result.slice(0, 4).decode("latin-1").utf8(), "CON ");   // converted from LIVE
    CB_CHECK(cb::fs::read_file(dir.path() / "p.stfs") == cb::compat::from_py(result));   // and written back over the path
    CB_CHECK_THROWS_AS(py::ValueError, (void)stfs::resign(std::variant<py::bytes, py::str>(py::bytes("x")), std::variant<py::bytes, py::str>(kv_bytes)));
});

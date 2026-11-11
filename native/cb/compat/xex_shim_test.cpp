// The shim at the old header path of formats.xex: the old py:: surface over cb/formats/xex. (basefile/basic/run use the
// settings of the running program -- the xex program's place -- and are exercised through the exe.)
#include "codebreaker/formats/xex.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace xex = codebreaker::formats::xex;

namespace {
cb::Bytes header_bytes() { return cb::fs::read_file(cb::testing::data_path("formats/testdata/pacman_ce.xex.head")); }
}  // namespace

CB_TEST("compat.xex.optional_headers_gives_entries_then_the_failure", [] {
    const cb::Bytes data = header_bytes();
    int64_t count = 0, first_key = 0;
    for (const xex::HeaderEntry& e : xex::optional_headers(cb::compat::to_py(cb::ByteView(data)))) {
        if (count == 0) first_key = std::get<1>(e);
        ++count;
    }
    CB_CHECK_EQ(count, 15);
    CB_CHECK_EQ(first_key, 767);
    // a table cut in the middle of an entry: the entries before it, then the old layer's struct.error
    cb::Bytes cut(data.begin(), data.begin() + 0x18 + 8 * 3 + 4);
    int64_t seen = 0;
    bool threw = false;
    try {
        for (const xex::HeaderEntry& e : xex::optional_headers(cb::compat::to_py(cb::ByteView(cut)))) {
            (void)e;
            ++seen;
        }
    } catch (const py::StructError&) {
        threw = true;
    }
    CB_CHECK_EQ(seen, 3);
    CB_CHECK(threw);
});

CB_TEST("compat.xex.ids_and_header", [] {
    const cb::Bytes data = header_bytes();
    const auto [media, title] = xex::ids(cb::compat::to_py(cb::ByteView(data)));
    CB_CHECK_EQ(media, 502788917);
    CB_CHECK_EQ(title, 1480657015);
    CB_CHECK_EQ(std::get<1>(xex::ids(py::bytes("not an xex"))), 0);

    cb::fs::ScopedTempDir dir("xex-shim");
    cb::fs::write_file(dir.path() / "a.xex", data);
    const py::str path = cb::compat::to_py(cb::fs::to_text(dir.path() / "a.xex"));
    CB_CHECK(xex::is_xex(path));
    const xex::Header h = xex::header(path);
    CB_CHECK_EQ(h.name->utf8(), "pacmanCE.pe");
    CB_CHECK_EQ(*h.base, 2181038080ll);
    CB_CHECK_EQ(h.imports.vec().size(), 2u);
    CB_CHECK_EQ(h.libraries.vec().size(), 12u);
    CB_CHECK_THROWS_AS(py::OSError, (void)xex::header(cb::compat::to_py(cb::fs::to_text(dir.path() / "missing.xex"))));   // (the old reader said ValueError)
});

CB_TEST("compat.xex.import_libraries_of_a_plain_image", [] {
    cb::fs::ScopedTempDir dir("xex-shim");
    CB_CHECK(!xex::import_libraries(cb::compat::to_py(cb::fs::to_text(dir.path() / "none.exe"))).has_value());
    cb::fs::write_text(dir.path() / "x.exe.json", "{\"imports\": [\"xam.xex\"]}");
    const auto libs = xex::import_libraries(cb::compat::to_py(cb::fs::to_text(dir.path() / "x.exe")));
    CB_CHECK(libs.has_value());
    CB_CHECK(std::holds_alternative<codebreaker::symbols::native::Json::Array>(libs->value));
});

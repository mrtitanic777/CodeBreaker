// The shims at the old header paths of formats.xbox.{xur,bink,chd} and formats.xbox (package): the old py:: surface over cb/formats/xbox/{xur,bink,chd,summary}.
#include "codebreaker/formats/xbox/bink.hpp"
#include "codebreaker/formats/xbox/chd.hpp"
#include "codebreaker/formats/xbox/package.hpp"
#include "codebreaker/formats/xbox/xur.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace xbox_assets_shim_test {

namespace xbox = codebreaker::formats::xbox;

static py::str fixture(const char* name) {
    return cb::compat::to_py(cb::fs::to_text(cb::testing::data_path(std::string("formats/xbox/testdata/assets/") + name)));
}

CB_TEST("compat.xbox_assets.xur_the_old_class_over_the_new_scene", [] {
    const py::list<py::str> strs = {py::str("XuiCanvas"), py::str("Press A")};
    const py::bytes strn = xbox::xur::build_strn(strs);
    CB_CHECK_EQ(strn.size(), 2 + 18 + 2 + 14);   // a length and the UTF-16BE text, each
    CB_CHECK_EQ(xbox::xur::_tag(xbox::xur::STRN).utf8(), "STRN");
    const py::bytes scene = cb::compat::to_py(cb::ByteView(cb::fs::read_file(cb::testing::data_path("formats/xbox/testdata/assets/xur.bin"))));
    const xbox::xur::XUR x(scene);
    CB_CHECK_EQ(x.version, 5);
    CB_CHECK_EQ(x.section_count, 1);
    CB_CHECK_EQ(x.data_base, 32);
    CB_CHECK_EQ(x.strings().size(), 4);
    CB_CHECK(x.section_data(xbox::xur::STRN).has_value());
    CB_CHECK(!x.section_data(0x41424344).has_value());
    CB_CHECK(xbox::xur::set_strings(scene, x.strings()) == scene);
    CB_CHECK_EQ(xbox::xur::XUR(xbox::xur::set_string(scene, 2, py::str("Press START"))).strings()[2].utf8(), "Press START");
    CB_CHECK_THROWS_AS(py::ValueError, (void)xbox::xur::XUR(py::bytes("not a scene at all, not a scene at all")));
    CB_CHECK_THROWS_AS(py::IndexError, (void)xbox::xur::set_string(scene, 9, py::str("x")));
});

CB_TEST("compat.xbox_assets.bink_the_old_header_record", [] {
    const xbox::bink::BinkHeader h = xbox::bink::read_header(fixture("t.bik"));
    CB_CHECK_EQ(h.generation, 1);
    CB_CHECK_EQ(h.codec.utf8(), "BIKi");
    CB_CHECK_EQ(h.file_size, 1000008);
    CB_CHECK_EQ(h.width, 1280);
    CB_CHECK_EQ(h.fps, 30.0);
    CB_CHECK_EQ(h.video_flags.utf8(), "0x00000000");
    CB_CHECK(xbox::bink::is_bink(py::bytes("BIKi....")));
    CB_CHECK(!xbox::bink::is_bink(py::bytes("NOPE")));
    CB_CHECK_THROWS_AS(py::ValueError, (void)xbox::bink::read_header(fixture("xur.bin")));
});

CB_TEST("compat.xbox_assets.chd_the_old_header_record_and_error", [] {
    const xbox::chd::Header h = xbox::chd::read_header(fixture("t.chd"));
    CB_CHECK_EQ(h.version, 5);
    CB_CHECK_EQ(h.logical_bytes, 4700000256LL);
    CB_CHECK_EQ(h.compressors.size(), 2);
    CB_CHECK_EQ(h.compressors[1].utf8(), "lzma");
    CB_CHECK_THROWS_AS(xbox::chd::ChdError, (void)xbox::chd::read_header(fixture("xur.bin")));
});

CB_TEST("compat.xbox_assets.package_summary_over_the_new_facts", [] {
    py::dict<py::str, py::str> match;
    match.set(py::str("format"), py::str("original Xbox executable (XBE)"));
    match.set(py::str("command"), py::str("info"));
    const xbox::Summary s = xbox::summary(fixture("xbe_demo.bin"), match);
    CB_CHECK_EQ(std::get<py::str>(s.at(py::str("title"))).utf8(), "Demo");
    CB_CHECK_EQ(std::get<py::str>(s.at(py::str("title id"))).utf8(), "4D530001");
    CB_CHECK_EQ(std::get<py::str>(xbox::_bink(fixture("t.bik")).at(py::str("width"))).utf8(), "1280");
    CB_CHECK_EQ(xbox::_BY_FORMAT().size(), 7);
    py::dict<py::str, py::str> other;
    other.set(py::str("format"), py::str("Xbox 360 executable (XEX2)"));
    CB_CHECK_EQ(xbox::summary(fixture("xbe_demo.bin"), other).size(), 0);
});

}  // namespace xbox_assets_shim_test

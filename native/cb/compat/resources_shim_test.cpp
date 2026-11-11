// The shim at the old header path of formats.resources: the old py:: records over cb/formats/resources. (read(), read_pe()
// and read_ne() reach the old PE/NE ports, which are not linked into cb_tests: they compile here and run in the exe.)
#include "codebreaker/formats/resources.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace res = codebreaker::formats::resources;

namespace {
py::bytes fixture(const char* name) {
    const cb::Bytes b = cb::fs::read_file(cb::testing::data_path(std::string("formats/testdata/resources/") + name));
    return cb::compat::to_py(cb::ByteView(b));
}
}  // namespace

CB_TEST("compat.resources.records_keep_their_old_shapes", [] {
    const res::Dialog d = res::dialog(fixture("wmsui32_dialog_2202.bin"), true);
    CB_CHECK(!d.extended);
    CB_CHECK_EQ(d.controls.vec().size(), 1u);
    CB_CHECK_EQ(std::get<py::str>(d.controls.vec()[0].class_).utf8(), "RichEdit");
    CB_CHECK_EQ(d.controls.vec()[0].id, 2235);
    CB_CHECK(d.font.has_value());
    CB_CHECK_EQ(std::get<res::DialogFontPlain>(*d.font) == res::DialogFontPlain(8, py::str("MS Shell Dlg")), true);

    const res::Menu m = res::menu(fixture("shell32_menu_226_ext.bin"), true);
    CB_CHECK(m.extended);
    const auto& items = std::get<py::list<res::ExtMenuItem>>(m.items);
    CB_CHECK_EQ(items.vec().size(), 1u);
    CB_CHECK(items.vec()[0].items.has_value());
    CB_CHECK_EQ(items.vec()[0].items->vec()[0].text.utf8(), "R&estore");

    const auto accel = res::accelerators(fixture("shell32_accelerator_2.bin"), true);
    CB_CHECK_EQ(accel.vec().size(), 3u);
    CB_CHECK_EQ(std::get<1>(accel.vec()[0]), 116);
    CB_CHECK_EQ(res::strings(fixture("wmsui32_string_1331.bin"), true).vec()[4].utf8(), "Help");
});

CB_TEST("compat.resources.version_has_13_numbers_and_decode_drops_the_date", [] {
    const res::Version v = res::version(fixture("shell32_version_1.bin"), true);
    CB_CHECK(v.fixed.has_value());
    CB_CHECK_EQ(v.fixed->vec().size(), 13u);
    CB_CHECK_EQ(v.strings.items().size(), 1u);
    CB_CHECK_EQ(v.translations.vec().size(), 2u);
    res::Resource r(res::Ident(int64_t(16)), res::Ident(int64_t(1)), 1033, 0, fixture("shell32_version_1.bin"));
    const res::Decoded d = res::decode(r, true);
    CB_CHECK(std::holds_alternative<res::Version>(d));
    CB_CHECK_EQ(std::get<res::Version>(d).fixed->vec().size(), 11u);
    // an opaque kind and a damaged body are None
    CB_CHECK(std::holds_alternative<std::monostate>(res::decode(res::Resource(res::Ident(int64_t(2)), res::Ident(int64_t(1)), 1033, 0, py::bytes("x")), true)));
    CB_CHECK(std::holds_alternative<std::monostate>(res::decode(res::Resource(res::Ident(int64_t(5)), res::Ident(int64_t(1)), 1033, 0, py::bytes("xyz")), true)));
});

CB_TEST("compat.resources.constants_groups_and_files", [] {
    CB_CHECK_EQ(res::TYPES().at(5).utf8(), "DIALOG");
    CB_CHECK_EQ(res::TYPES().items().size(), 21u);
    CB_CHECK_EQ(res::MEMBER_OF().at(res::GROUP_ICON), res::ICON);
    CB_CHECK_EQ(res::CONTROL_CLASSES().at(0x82).utf8(), "STATIC");
    res::Resource r(res::Ident(int64_t(5)), res::Ident(py::str("ABOUT")), 1033, 0, py::bytes(std::string(120, 'x')));
    CB_CHECK_EQ(r.repr().utf8(), "<DIALOG 'ABOUT' lang=1033 120 bytes>");
    CB_CHECK_EQ(res::type_name(res::Ident(int64_t(77))).utf8(), "#77");

    const py::bytes group_data = fixture("comctl32_groupcursor_102.bin");
    const auto [kind, entries] = res::group(group_data);
    CB_CHECK_EQ(kind, 2);
    CB_CHECK_EQ(entries.vec().size(), 1u);
    CB_CHECK_EQ(std::get<6>(entries.vec()[0]), 1);
    const py::bytes cur = res::icon_file(group_data, [](int64_t) { return fixture("comctl32_cursor_1.bin"); });
    CB_CHECK_EQ(cur.size(), 326);
    CB_CHECK_EQ(res::bitmap_file(fixture("comctl32_bitmap_133.bin")).size(), 336);
    CB_CHECK_THROWS_AS(py::ValueError, (void)res::group(py::bytes("junk!!")));

    const auto lib = res::type_library(fixture("stdole2_typelib.bin"));
    CB_CHECK(lib.has_value());
    CB_CHECK_EQ(lib->name.utf8(), "stdole");
    CB_CHECK(res::type_library_outline(*lib).utf8().find("interface IDispatch;") != std::string::npos);
    CB_CHECK(!res::type_library(py::bytes("nope")).has_value());
    CB_CHECK(res::same_data(py::bytes("ab"), py::bytes(std::string("ab\0", 3))));
});

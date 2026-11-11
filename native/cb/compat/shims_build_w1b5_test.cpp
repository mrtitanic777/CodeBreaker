// Tests of the staged shims of the build lane's second batch (compat/shims/codebreaker/build/{data,driver,m68k_asm,native_dos}.hpp):
// the old API over the new modules. Built at /W3 (the old layer is not warning-clean), like shims_build_test.cpp. What the shims
// ask of old code that is not linked here (the 68000 instruction table, the DOS executive, the corpus and the settings) is
// compile-checked by including them; their logic is the new modules' own, tested in cb/build/.
#include "shims/codebreaker/build/data.hpp"
#include "shims/codebreaker/build/driver.hpp"
#include "shims/codebreaker/build/m68k_asm.hpp"
#include "shims/codebreaker/build/native_dos.hpp"

#include <type_traits>

#include "cb/core/testing.hpp"

namespace old_data = codebreaker::build::data;
namespace old_driver = codebreaker::build::driver;
namespace old_m68k = codebreaker::build::m68k_asm;
namespace old_dos = codebreaker::build::native_dos;

namespace {

std::string testdata(const std::string& name) { return cb::testing::data_path("formats/testdata/" + name).string(); }

}  // namespace

CB_TEST("compat.shims.build_m68k_asm", [] {
    CB_CHECK_EQ(old_m68k::_strip(py::str("  * all comment")).utf8(), "");
    CB_CHECK_EQ(old_m68k::_strip(py::str("nop ; rest  ")).utf8(), "nop");
    CB_CHECK_EQ(old_m68k::_strip(py::str("dc.b \";\" ; x")).utf8(), "dc.b \";\"");
    // the AsmError of build/z80_asm is the one of this module too
    static_assert(std::is_same_v<old_m68k::AsmError, codebreaker::build::z80_asm::AsmError>);
});

CB_TEST("compat.shims.build_native_dos_constants", [] {
    CB_CHECK(old_dos::_PROVEN.contains(py::str("masm")));
    CB_CHECK(old_dos::_PROVEN.contains(py::str("ml")));
    CB_CHECK(!old_dos::_PROVEN.contains(py::str("link")));
    const old_dos::NativeDosError e("x");
    CB_CHECK_EQ(std::string(e.type()), "NativeDosError");
    // a refusal of the new layer reaches old code as the old NativeDosError, any other error as the matching py:: exception
    CB_CHECK_THROWS_AS(old_dos::NativeDosError, old_dos::as_old([]() -> int { throw cb::build::native_dos::NativeDosError("no"); }));
    CB_CHECK_THROWS_AS(py::ValueError, old_dos::as_old([]() -> int { throw cb::UsageError("bad stem"); }));
});

CB_TEST("compat.shims.build_data_of_real_modules", [] {
    // the new module's answers in the old vocabulary, from the real lz32.dll (whose .data holds two pointers into the code)
    const py::str path = py::str(testdata("lz32.dll"));
    const auto fn = [](int64_t va) -> std::optional<py::str> {
        if (va >= 0xBFEA1000 && va < 0xBFEA1000 + 1130) return py::str("fn_" + std::to_string(va));
        return std::nullopt;
    };
    const old_data::Data d = old_data::data32(path, fn);
    CB_CHECK_EQ(d.pointers, 2);
    CB_CHECK_EQ(d.size, 512);
    CB_CHECK(d.movable);
    CB_CHECK_EQ(d.sections.size(), 1);
    CB_CHECK_EQ(std::get<1>(d.sections[0]).utf8(), ".data");
    CB_CHECK_EQ(d.parts.size(), 1);
    CB_CHECK_EQ(std::get<0>(d.parts[0]).utf8(), "codebreaker_data_2");
    CB_CHECK(d.source.utf8().find("(void *)fn_") != std::string::npos);
    CB_CHECK(d.declarations.utf8().starts_with("struct layout_of_codebreaker_data_2;"));
    CB_CHECK_EQ(d.place(0xBFEA2010)->utf8(), "((char *)&codebreaker_data_2 + 0x10)");
    CB_CHECK(!d.place(0x1000).has_value());

    // the header and the text are rewritten as the module does
    const auto header = old_data::relocate_header(py::str("#define g (*(int *)0xbfea2004)\n#define h (*(int *)0x1)"), d);
    CB_CHECK_EQ(std::get<0>(header).utf8(),
                d.declarations.utf8() + "#define g (*(int *)((char *)&codebreaker_data_2 + 0x4))\n#define h (*(int *)0x1)");
    CB_CHECK_EQ(std::get<1>(header).size(), 1);
    CB_CHECK_EQ(std::get<1>(header)[0], 1);
    const auto text = old_data::relocate_text(py::str("int f() { return 0xbfea2004; }"), py::set<int64_t>{int64_t{0xBFEA2004}}, d, fn);
    CB_CHECK_EQ(std::get<0>(text).utf8(), "int f() { return ((int4)((char *)&codebreaker_data_2 + 0x4)); }");
    CB_CHECK_EQ(std::get<1>(text).size(), 0);

    // address_constants: every start is a key
    const auto constants = old_data::address_constants(path, py::list<int64_t>{int64_t{0xBFEA1000}, int64_t{0xBFEA1100}});
    CB_CHECK_EQ(constants.size(), 2);
    CB_CHECK(constants[int64_t{0xBFEA1000}].contains(int64_t{0xBFEA103C}));

    // layout_holds is false for bytes that are no object file
    CB_CHECK(!old_data::layout_holds(py::bytes(std::string("not an obj")), d));
    // a file that is not a PE reaches old code as the ValueError it raised
    CB_CHECK_THROWS_AS(py::ValueError, (void)old_data::data32(py::str(testdata("charmap.exe")), fn));
});

CB_TEST("compat.shims.build_data16_and_far_segments", [] {
    const py::str path = py::str(testdata("lzexpand.dll"));
    const old_data::Data d = old_data::data16(path, nullptr);
    CB_CHECK_EQ(d.size, 644);
    CB_CHECK_EQ(d.pointers, 0);
    CB_CHECK(d.source.utf8().find("The automatic data segment of") != std::string::npos);
    CB_CHECK(!d.pieces.empty());
    CB_CHECK_EQ(d.pieces[0].kind.utf8(), "bytes");
    // other segments, imports by number in a far pointer
    py::dict<int64_t, py::bytes> segs;
    segs.set(int64_t{2}, py::bytes(std::string(1882, '\0')));
    const py::str c = old_data::far_segments(segs, path, nullptr);
    CB_CHECK(c.utf8().find("struct codebreaker_segment_2_t;") != std::string::npos);
    CB_CHECK(old_data::far_segments(segs).utf8().find("p_") == std::string::npos);   // no module: bytes only
});

CB_TEST("compat.shims.build_driver_constants_and_report", [] {
    CB_CHECK_EQ(old_driver::SEGMENT_CLASSES.size(), 8);
    CB_CHECK_EQ(std::get<0>(old_driver::SEGMENT_CLASSES[1]).utf8(), "MCODE");
    CB_CHECK_EQ(std::get<1>(old_driver::SEGMENT_CLASSES[1]).utf8(), "preload iopl");
    CB_CHECK_EQ(old_driver::_TIMEOUT, 300);
    old_driver::BuildResult r;
    r.name = py::str("vkd");
    r.assembled = r.linked = true;
    r.checks.set(py::str("objects"), true);
    r.checks.set(py::str("descriptor"), false);
    r.checks.set(py::str("services"), true);
    r.checks.set(py::str("exports"), true);
    r.checks.set(py::str("module"), true);
    r.same = 0.5;
    CB_CHECK_EQ(old_driver::report(r).utf8(),
                "vkd          built  objects same descriptor DIFFERS services same exports same module same  50.0% of paired bytes equal");
    old_driver::BuildResult none;
    none.name = py::str("x");
    CB_CHECK_EQ(old_driver::report(none).utf8(), "x            does not assemble: ?");
});

CB_TEST("compat.shims.build_data_places_of_an_old_nefile", [] {
    const codebreaker::formats::ne::NEFile nef(py::str(testdata("charmap.exe")));
    const auto segments = nef.segments();
    const auto& code = segments[0];   // segment 1
    const py::bytes blob = nef.data.slice(code.file_off, code.file_off + code.length);
    const auto places = old_data::_places(nef, code, blob);
    const auto records = nef.relocations(code);
    CB_CHECK(places.size() > 100);
    // each place carries the old record that put it there: the first is the first record's own offset
    CB_CHECK_EQ(std::get<0>(places[0]), records[0].offset);
    CB_CHECK_EQ(std::get<1>(places[0]), records[0].src);
    CB_CHECK_EQ(std::get<3>(places[0]).kind.utf8(), records[0].kind.utf8());
});

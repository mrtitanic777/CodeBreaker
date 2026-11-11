// The header-only shims at the old header paths of symbols/libraries (package, coff, omf), symbols/x86 and symbols/wine16: the
// old py:: records over the new readers. They are header-only, so this is where they compile as well as run; the answers are
// those of the new modules' tests (testdata of symbols/libraries), seen through the old shapes.
#include "codebreaker/symbols/libraries/coff.hpp"
#include "codebreaker/symbols/libraries/omf.hpp"
#include "codebreaker/symbols/libraries/package.hpp"
#include "codebreaker/symbols/wine16.hpp"
#include "codebreaker/symbols/x86.hpp"

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace libs = codebreaker::symbols::libraries;
namespace old_coff = codebreaker::symbols::libraries::coff;
namespace old_omf = codebreaker::symbols::libraries::omf;
namespace old_wine = codebreaker::symbols::wine16;
namespace old_x86 = codebreaker::symbols::x86;

namespace {

std::filesystem::path path(const char* name) { return cb::testing::data_path(std::string("symbols/libraries/testdata/") + name); }
py::str text(const std::filesystem::path& p) { return py::str(cb::fs::to_text(p)); }
py::bytes bytes_of(const char* name) {
    const cb::Bytes b = cb::fs::read_file(path(name));
    return cb::compat::to_py(cb::ByteView(b));
}

}  // namespace

CB_TEST("compat.symbols.libraries.functions_keep_their_old_shape", [] {
    auto [kind16, vxd] = libs::functions(text(path("libvxd.lib")));
    CB_CHECK(std::get<int64_t>(kind16) == 16);
    CB_CHECK_EQ(vxd.size(), int64_t{13});
    CB_CHECK(vxd[0].name == py::str("_VmmHeapAllocate"));
    CB_CHECK(std::holds_alternative<py::bytes>(vxd[0].mask));
    CB_CHECK_EQ(py::len(vxd[0].code), int64_t{28});

    auto [kind_ppc, ppc] = libs::functions(text(path("xhttp.obj")));
    CB_CHECK(std::get<py::str>(kind_ppc) == py::str("powerpc"));
    CB_CHECK_EQ(ppc.size(), int64_t{8});
    const auto& ignore = std::get<py::list<int64_t>>(ppc[0].mask);   // the bits left out of each word
    CB_CHECK_EQ(ignore.size(), int64_t{8});
    CB_CHECK_EQ(ignore[7], int64_t{0x03FFFFFC});

    auto [kind32, x86] = libs::functions(text(path("lz32.lib")));
    CB_CHECK(std::get<int64_t>(kind32) == 32);
    CB_CHECK_EQ(x86.size(), int64_t{12});

    CB_CHECK_THROWS_AS(py::OSError, (void)libs::functions(text(path("no_such.lib"))));
});

CB_TEST("compat.symbols.libraries.fits_and_index", [] {
    auto [kind, atof] = libs::functions(text(path("atof.obj")));
    py::bytes code = atof[0].code;
    CB_CHECK(libs::fits(atof[0], code));
    // a linker's field may be anything; an ordinary byte may not
    std::string raw = code.raw();
    raw[19] = 0x55;
    CB_CHECK(libs::fits(atof[0], py::bytes(raw)));
    raw[10] ^= 1;
    CB_CHECK(!libs::fits(atof[0], py::bytes(raw)));

    py::list<int64_t> words;
    for (const int64_t w : {0x7C0802A6ll, 0x9421FFF0ll, 0x38600001ll, 0x48000101ll, 0x4E800020ll, 0x7C0802A6ll, 0x9421FFF0ll, 0x38600001ll, 0x48002001ll, 0x4E800020ll}) words.append(w);
    libs::Index index(words);
    py::list<int64_t> sig, ignore;
    for (const int64_t w : {0x7C0802A6ll, 0x9421FFF0ll, 0x38600001ll, 0x48000001ll, 0x4E800020ll}) sig.append(w);
    for (const int64_t m : {0ll, 0ll, 0ll, 0x03FFFFFCll, 0ll}) ignore.append(m);
    const py::list<int64_t> hits = index.find(sig, ignore);
    CB_CHECK_EQ(hits.size(), int64_t{2});
    CB_CHECK_EQ(hits[0], int64_t{0});
    CB_CHECK_EQ(hits[1], int64_t{5});
});

CB_TEST("compat.symbols.libraries.coff_and_omf", [] {
    const py::bytes obj = bytes_of("atof.obj");
    old_coff::Coff coff(obj);
    CB_CHECK_EQ(coff.machine, old_coff::I386);
    CB_CHECK_EQ(coff.sections.size(), int64_t{2});
    CB_CHECK_EQ(coff.sections[0].size, int64_t{112});
    CB_CHECK_EQ(coff.symbols.size(), int64_t{14});
    CB_CHECK(!coff.symbols[1].has_value());                         // an auxiliary record
    CB_CHECK(coff.symbols[6]->name == py::str("_atof"));
    const auto relocations = coff.relocations(coff.sections[0]);
    CB_CHECK_EQ(relocations.size(), int64_t{4});
    CB_CHECK_EQ(std::get<0>(relocations[0]), int64_t{19});
    CB_CHECK_EQ(std::get<2>(relocations[0]), int64_t{6});
    const auto functions = old_coff::functions(obj);
    CB_CHECK_EQ(functions.size(), int64_t{1});
    CB_CHECK(std::holds_alternative<py::bytearray>(functions[0].mask));
    CB_CHECK_EQ(py::len(std::get<py::bytearray>(functions[0].mask)), int64_t{112});
    CB_CHECK_THROWS_AS(py::StructError, (void)old_coff::Coff(py::bytes(obj.raw().substr(0, 10))));
    CB_CHECK_EQ(old_coff::members(bytes_of("lz32.lib")).size(), int64_t{15});
    CB_CHECK(std::get<0>(old_coff::members(bytes_of("lz32.lib"))[0]) == py::str("LZ32.dll"));

    const auto omf = old_omf::functions(bytes_of("debug.lib"));
    CB_CHECK_EQ(omf.size(), int64_t{1});
    CB_CHECK(omf[0].name == py::str("PrintFunc"));
    CB_CHECK_EQ(py::len(omf[0].mask), int64_t{71});
    CB_CHECK(old_omf::fields_only(py::bytes(std::string("\x00\x01\x02", 3))).raw() == std::string("\x00\x01\x00", 3));
});

CB_TEST("compat.symbols.x86.decodes_as_before", [] {
    const std::string code("\x8B\x05\x34\x12\x00\x00\x68\x78\x56\x34\x12\xC3", 12);
    const auto insns = old_x86::disasm(32, true, code, 0x1000);
    CB_CHECK_EQ(insns.size(), size_t{3});
    CB_CHECK(insns[0].mnemonic == "mov");
    CB_CHECK_EQ(insns[0].disp, int64_t{0x1234});
    CB_CHECK_EQ(insns[1].first_imm, int64_t{0x12345678});
    CB_CHECK(insns[1].first_is_imm);
    const auto lite = old_x86::disasm_lite(32, code, 0);
    CB_CHECK_EQ(lite.size(), size_t{3});
    CB_CHECK(lite[2].mnemonic == "ret");
    const auto first = old_x86::first_lite(32, code, 0x20);
    CB_CHECK(first.has_value());
    CB_CHECK_EQ(first->address, int64_t{0x20});
    CB_CHECK(!old_x86::first_lite(32, std::string("\xE8\x00", 2), 0).has_value());
    old_x86::set_capstone_dll(py::str("ignored"));
});

CB_TEST("compat.symbols.wine16.exports_by_the_setting", [] {
    const auto e = old_wine::read_spec(py::str("7 stub IpFindFirstLine #(word str str ptr)\n33 pascal -ret16 GetClientRect(word ptr)\n"));
    CB_CHECK_EQ(e.size(), int64_t{2});
    CB_CHECK(e.at(7).name == py::str("IpFindFirstLine"));
    CB_CHECK(e.at(7).stub);
    CB_CHECK(e.at(7).convention == py::str("pascal"));
    CB_CHECK_EQ(e.at(7).args->size(), int64_t{4});
    CB_CHECK((*e.at(7).args)[1] == py::str("str"));
    CB_CHECK(e.at(33).ret16);
    CB_CHECK_EQ(old_wine::WIDTH.at(py::str("segptr")), int64_t{4});
    CB_CHECK_EQ(old_wine::WIDTH.at(py::str("s_word")), int64_t{2});
    CB_CHECK(old_wine::POINTER.contains(py::str("str")));
    CB_CHECK(!old_wine::POINTER.contains(py::str("long")));
    CB_CHECK(old_wine::c_type(py::str("ptr")) == py::str("void __far *"));
    CB_CHECK(old_wine::c_type(py::str("word")) == py::str("uint2"));
    CB_CHECK(old_wine::c_type(py::str("s_word")) == py::str("int2"));
    CB_CHECK(old_wine::c_type(py::str("long")) == py::str("uint4"));

    // exports() reads the folder the `wine16` setting names, once
    cb::fs::ScopedTempDir tmp("wine16shim");
    std::filesystem::create_directories(tmp.path() / "user.exe16");
    cb::fs::write_file(tmp.path() / "user.exe16" / "user.spec", cb::as_bytes("1 pascal -ret16 GetMessage(ptr word word word)\n"));
    const cb::fs::ScopedEnv setting("CB_WINE16", cb::fs::to_text(tmp.path()));
    const auto user = old_wine::exports(py::str("user"));
    CB_CHECK_EQ(user.size(), int64_t{1});
    CB_CHECK(user.at(1).name == py::str("GetMessage"));
    CB_CHECK(old_wine::exports(py::str("NOPE")).empty());
});

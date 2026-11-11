// The header-only shims at codebreaker/build/asm16.hpp and msc6.hpp: the old records in and out, the new modules in between. The
// tests check that each old signature still answers what the new function answers (the modules' own tests are in build/).
#include "codebreaker/build/asm16.hpp"
#include "codebreaker/build/msc6.hpp"

#include "cb/build/asm16.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

using namespace cb;
namespace oa = codebreaker::build::asm16;
namespace om = codebreaker::build::msc6;

std::optional<py::str> old_symbols(const oa::Reference& r) {
    if (r.kind == py::str("function")) {
        if ((r.seg * 31 + r.off) % 13 == 0) return std::nullopt;
        return py::str(std::format("_f{}_{:x}", r.seg, r.off));
    }
    if (r.kind == py::str("segment")) return py::str(std::format("_seg{}", r.seg));
    if (r.kind == py::str("data")) return py::str(std::format("_d{}", r.seg));
    return py::str("_IMP_" + r.module.utf8() + "_" + (std::holds_alternative<int64_t>(r.which) ? std::to_string(std::get<int64_t>(r.which)) : std::get<py::str>(r.which).utf8()));
}

Bytes object_of(ByteView code) {
    const auto record = [](std::uint8_t kind, ByteView body) {
        Bytes r{kind, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
        append(r, body);
        r.push_back(0);
        return r;
    };
    Bytes out = record(0x80, to_bytes(std::string("\x03" "f.c", 4)));
    Bytes data{1, 0, 0};
    append(data, code);
    append(out, record(0xA0, data));
    append(out, record(0x8A, Bytes{0}));
    return out;
}

template <class F>
bool throws(F&& f) {
    try {
        f();
    } catch (const std::exception&) {
        return true;
    }
    return false;
}

}  // namespace

CB_TEST("compat.build_asm16.the_old_function_record_is_the_new_functions", [] {
    const std::string path = fs::to_text(testing::data_path("formats/testdata/lzexpand.dll"));
    const codebreaker::formats::ne::NEFile nef{py::str(path)};
    oa::Function f(nef, 1, 4, 178, oa::Publics(py::str("_f1_4")), old_symbols);
    CB_CHECK(f.text.has_value());
    CB_CHECK_EQ(f.why.utf8(), "");
    CB_CHECK_EQ(f.start, 4);
    CB_CHECK_EQ(f.end, 178);
    CB_CHECK_EQ(f.bits, 16);
    CB_CHECK_EQ(f.module.utf8(), "LZEXPAND");
    CB_CHECK_EQ(f.public_.utf8(), "_f1_4");
    CB_CHECK_EQ(f.imports.size(), 2);
    CB_CHECK_EQ(std::get<0>(*f.imports.get(py::str("_IMP_KERNEL_15"))).utf8(), "KERNEL");
    CB_CHECK_EQ(std::get<int64_t>(std::get<1>(*f.imports.get(py::str("_IMP_KERNEL_15")))), 15);
    CB_CHECK_EQ(f.externs.get(py::str("_IMP_KERNEL_17"))->utf8(), "far");
    CB_CHECK_EQ(f.mnemonic(5).utf8(), "mov    bp, sp");
    CB_CHECK(f.candidates().size() > 0);
    // the old write(): None means every instruction, a set the ones named
    const py::str all = f.write(std::nullopt);
    const py::str none = f.write(py::set<int64_t>());
    CB_CHECK(all.utf8().find("je     short L_001C") != std::string::npos);
    CB_CHECK(none.utf8() == f.text->utf8());
    // the new Function says the same
    CB_CHECK_EQ(f.impl().write(std::nullopt), all.utf8());
    // check, check_piece and differences keep their old answers ("" for the same bytes)
    const cb::Bytes want(f.impl().blob().begin() + 4, f.impl().blob().begin() + 178);
    const py::bytes object = cb::compat::to_py(cb::ByteView(object_of(want)));
    CB_CHECK_EQ(oa::check(object, f).utf8(), "");
    cb::Bytes flipped = want;
    flipped[0] ^= 0xFF;
    CB_CHECK_EQ(oa::check(cb::compat::to_py(cb::ByteView(object_of(flipped))), f).utf8(), "differs at 0004");
    CB_CHECK_EQ(oa::differences(cb::compat::to_py(cb::ByteView(flipped)), py::bytes(std::string(flipped.size(), '\0')), f).size(), 1);
    // a refused function, a segment that does not exist
    const oa::Function refused(nef, 1, 5000, 100, oa::Publics(py::str("_x")), old_symbols);
    CB_CHECK(!refused.text.has_value());
    CB_CHECK(throws([&] { oa::Function bad(nef, 99, 0, 10, oa::Publics(py::str("_x")), old_symbols); }));
});

CB_TEST("compat.build_msc6.the_old_signatures_answer_what_the_new_functions_do", [] {
    const py::bytes obj = cb::compat::to_py(cb::ByteView(object_of(cb::Bytes{0x55, 0x8B, 0xEC, 0x5D, 0xCB})));
    const om::Code code = om::object_code(obj);
    CB_CHECK_EQ(std::get<0>(code).raw(), std::string("\x55\x8B\xEC\x5D\xCB", 5));
    CB_CHECK_EQ(std::get<1>(code).raw(), std::string(5, '\0'));
    CB_CHECK(throws([&] { (void)om::function_code(obj, py::str("f")); }));   // (the object defines no segment: the old answer was a KeyError too)
    // the text passes
    py::list<py::str> texts;
    texts.append(py::str("int2 __stdcall16near f(param_1,param_2)\n{\n  USER_33((int2)&aiStack_c[0],unaff_SS,param_1);\n}"));
    texts.append(py::str("extern uint4 __far __stdcall USER_33();"));
    const py::set<py::str> names = om::pascal_names(texts);
    CB_CHECK(names.contains(py::str("f")));
    CB_CHECK_EQ(om::pascal_order(texts[0], names).utf8(), "int2 __stdcall16near f(param_2, param_1)\n{\n  USER_33(param_1, (void __far *)&aiStack_c[0]);\n}");
    const auto [header, text] = om::dialect(py::str("long long x;\n"), py::str("t"));
    CB_CHECK(header.utf8().ends_with("long x;\n\n"));
    CB_CHECK_EQ(text.utf8(), "t");
    CB_CHECK_EQ(om::fold_temporaries(py::str("int f()\n{\n  int2 iVar1;\n\n  iVar1 = f(x);\n  return iVar1;\n}")).utf8(), "int f()\n{\n\n  return f(x);\n}");
    CB_CHECK_EQ(std::get<0>(om::ds_globals(py::str("*(int2 *)0x63b2"))).utf8(), "extern char codebreaker_d63b2[];\n");
    CB_CHECK_EQ(om::sdk_header(py::str("H"), py::str("S")).utf8(), "S\nH");
    CB_CHECK_EQ(om::FLAG_SETS.size(), 9);
    CB_CHECK_EQ(om::BATCH, 40);
    // the keys of the old dict come back as they went in
    py::dict<om::Key, std::tuple<py::str, py::str>> todo;
    todo.set(om::Key(py::str("a")), std::make_tuple(py::str("k"), py::str("x\ny")));
    todo.set(om::Key(int64_t(7)), std::make_tuple(py::str("k"), py::str("t")));
    py::list<om::Key> g;
    g.append(om::Key(py::str("a")));
    g.append(om::Key(int64_t(7)));
    py::list<py::list<om::Key>> groups;
    groups.append(g);
    const auto [files, starts] = om::_batch_files(groups, todo, py::str("B"));
    CB_CHECK_EQ(files.size(), 1);
    CB_CHECK_EQ(std::get<1>(files[0]).utf8(), "x\ny\n\nt");
    CB_CHECK_EQ(std::get<0>(starts[0][1]), 6);   // the file begins at line 3; "a" has two lines and a blank one after
    // an unknown compiler is not available (compile_many runs the old DOS runner, which cannot be linked here: see shim_check_build.cpp)
    CB_CHECK(!om::available(py::str("nonesuch")));
});

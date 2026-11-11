// The shims at the old header paths (native/typed/cpp/codebreaker/invert/*.hpp, formats/xbox/crypto/format_key.hpp) over
// the modules of this folder: they must compile with the old layer and forward to the new code. Built at /W3 (the old
// layer is not warning-clean).
#include <filesystem>
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "codebreaker/formats/xbox/crypto/format_key.hpp"
#include "codebreaker/invert/early_c.hpp"
#include "codebreaker/invert/early_c_frame.hpp"
#include "codebreaker/invert/grammars.hpp"
#include "codebreaker/invert/known_differences.hpp"
#include "codebreaker/invert/quad_align.hpp"
#include "codebreaker/invert/quad_dump.hpp"

namespace {

using cb::compat::from_py;
using cb::compat::to_py;
namespace old = codebreaker::invert;

cb::Bytes hex(const char* digits) {
    cb::Bytes out;
    for (const char* p = digits; p[0] && p[1]; p += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(std::string(p, 2), nullptr, 16)));
    return out;
}

std::string testdata(const char* name) { return cb::fs::to_text(cb::testing::data_path(std::string("invert/testdata/") + name)); }

}  // namespace

CB_TEST("invert.shims.grammars_and_known_differences", [] {
    CB_CHECK_EQ(old::grammars::g_thunk(std::nullopt).size(), 4);
    CB_CHECK_EQ(old::grammars::g_bufread(std::nullopt).size(), 12);
    py::dict<py::str, int64_t> ctx;
    ctx.set(py::str("nargs"), 1);
    CB_CHECK_EQ(from_py(old::grammars::g_thunk(ctx)[2]), "extern int far SIB();\nint far F(int a0){ return SIB(a0); }");
    CB_CHECK_EQ(old::grammars::GRAMMARS.size(), 2);

    namespace kd = old::known_differences;
    CB_CHECK_EQ(kd::KB.size(), 7);
    CB_CHECK_EQ(kd::diagnose(std::optional<py::str>(py::str("ES:[bx] trailing 90"))).size(), 2);
    CB_CHECK_EQ(kd::diagnose(std::nullopt).size(), 0);
    kd::record(kd::Entry{py::str("t"), py::str("zzz"), py::str("c"), py::str("f"), py::str("m"), false, py::str("e")});
    CB_CHECK_EQ(kd::diagnose(std::optional<py::str>(py::str("ZZZ"))).size(), 1);
    CB_CHECK_EQ(from_py(kd::diagnose(std::optional<py::str>(py::str("ZZZ")))[0].id), "t");
});

CB_TEST("invert.shims.quad_files", [] {
    namespace qd = old::quad_dump;
    namespace qa = old::quad_align;
    CB_CHECK_EQ(qd::namepool_start(to_py(cb::ByteView(hex("0102414200")))), 2);
    CB_CHECK_EQ(from_py(qd::fmt_payload(to_py(cb::ByteView(hex("00ab"))))), "00 ab");
    CB_CHECK_EQ(from_py(qd::OPC.at(0x23)), "FUNC-BEGIN");
    CB_CHECK_NOTHROW(qd::main(to_py(testdata("FXU.Q"))));   // (it prints through py::print: the new text is pinned by quad_dump_test)
    {
        cb::fs::ScopedTempDir tmp("qdshim");
        cb::fs::write_file(tmp.path() / "short.q", cb::Bytes(10, 0));
        CB_CHECK_THROWS_AS(py::StructError, qd::main(to_py(cb::fs::to_text(tmp.path() / "short.q"))));   // too short for the header
        CB_CHECK_THROWS_AS(py::FileNotFoundError, qd::main(to_py(cb::fs::to_text(tmp.path() / "none.q"))));
    }
    const auto quads = qa::parse_quads(to_py(testdata("FXU.Q")));
    CB_CHECK_EQ(quads.size(), 1);
    CB_CHECK_EQ(quads[0].size(), 451);
    CB_CHECK_EQ(std::get<0>(quads[0][0]), 0x23);
    const auto funcs = qa::parse_obj_funcs(to_py(testdata("HELLO.OBJ")));
    CB_CHECK_EQ(funcs.size(), 1);
    CB_CHECK_EQ(from_py(std::get<0>(funcs[0])), "MAIN");
    CB_CHECK_EQ(std::get<2>(funcs[0]).size(), 14);
});

CB_TEST("invert.shims.early_c_frame", [] {
    namespace fr = old::early_c_frame;
    py::list<py::str> temps;
    temps.append(py::str("int"));
    py::list<std::tuple<py::str, py::str, int64_t>> locals;
    locals.append(std::make_tuple(py::str("a"), py::str("int"), int64_t{1}));
    locals.append(std::make_tuple(py::str("b"), py::str("long"), int64_t{1}));
    const auto [offsets, framesize, params, temp_bytes] = fr::layout(temps, locals);
    CB_CHECK_EQ(offsets.at(py::str("a")), 2);
    CB_CHECK_EQ(offsets.at(py::str("b")), 4);
    CB_CHECK_EQ(framesize, 8);
    CB_CHECK_EQ(temp_bytes, 2);
    CB_CHECK_EQ(fr::tsize(py::str("double"), 3), 24);
    py::list<int64_t> offs;
    offs.append(0), offs.append(2), offs.append(4);
    py::list<std::tuple<int64_t, int64_t>> copies;
    copies.append(std::make_tuple(int64_t{2}, int64_t{0})), copies.append(std::make_tuple(int64_t{4}, int64_t{2}));
    CB_CHECK_EQ(fr::infer_decl_order(offs, copies).size(), 3);
    CB_CHECK_EQ(fr::infer_decl_order(offs, copies)[0], 4);
    const auto [decls, names] = fr::emit_local_decls(offs, 6);
    CB_CHECK_EQ(decls.size(), 3);
    CB_CHECK_EQ(names.size(), 3);
    CB_CHECK_EQ(from_py(fr::fmt_decl(py::str("char"), py::str("x"), 3)), "char x[3];");
    py::list<int64_t> negative;
    negative.append(-2);
    CB_CHECK_THROWS_AS(py::OverflowError, fr::emit_local_decls(negative, 6));
});

CB_TEST("invert.shims.early_c", [] {
    namespace ec = old::early_c;
    const py::bytes add = to_py(cb::ByteView(hex("558bec8b46040346065dc3")));
    const auto [fs, params, stmts, pt, loc] = ec::recover_cfg(add);
    CB_CHECK_EQ(fs, 0);
    CB_CHECK_EQ(params.size(), 2);
    CB_CHECK_EQ(from_py(stmts[0]), "return (a + b);");
    CB_CHECK_EQ(from_py(pt), "int");
    CB_CHECK(ec::LAST.saved.has_value());
    CB_CHECK_EQ(from_py(ec::to_c(py::str("f"), 0, params, stmts)), "f(a, b)\nint a, b;\n{\n    return (a + b);\n}\n");
    // a hook of the old kind (the Python's None or an answer) reaches the model
    int asked = 0;
    ec::CALLINFO = [&](int64_t, const py::str& kind, const ec::CallTarget&) -> std::optional<int64_t> {
        ++asked;
        CB_CHECK_EQ(from_py(kind), "near");
        return std::nullopt;
    };
    (void)ec::recover(to_py(cb::ByteView(hex("558bec8b46045068100090e8f9ff83c4048be55dc3"))));
    ec::CALLINFO = nullptr;
    CB_CHECK(asked > 0);
    CB_CHECK_EQ(ec::detect_copies(py::list<py::str>{py::str("v0 = 1;"), py::str("v2 = v0;")}).size(), 1);
    CB_CHECK_EQ(from_py(ec::local_decls(4, py::list<int64_t>{0, 2})), "    int v0;\n    int v2;\n");
});

CB_TEST("invert.shims.format_key", [] {
    cb::fs::ScopedTempDir tmp("fkshim");
    cb::fs::write_file(tmp.path() / "Keys" / "custom_pub.bin", cb::Bytes{1, 2, 3});
    const std::filesystem::path before = std::filesystem::current_path();
    std::filesystem::current_path(tmp.path());
    CB_CHECK_NOTHROW(codebreaker::formats::xbox::crypto::format_key::main());   // (it prints the source through py::print)
    std::filesystem::current_path(before);
    CB_CHECK(cb::fs::read_text(tmp.path() / "Output" / "Zero" / "ExpPubKey.h").ends_with("};\n#endif\n"));
});

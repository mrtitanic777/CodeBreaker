// The shim at the old header path of decompile/sm83: the old py:: surface (Insn, decode, text, assemble, TABLE ...) over
// cb/decompile/sm83. The shims of frame16, shape16, asm_blocks and l2tree call old ports that are not linked into cb_tests
// (translate/l2's type_json): they are compiled by shim_check_decompile.cpp and run in the exe.
#include "codebreaker/decompile/sm83.hpp"

#include "cb/core/testing.hpp"

namespace sm83 = codebreaker::decompile::sm83;

namespace {
py::bytes bytes_of(std::initializer_list<int> b) {
    std::string s;
    for (int x : b) s.push_back(static_cast<char>(x));
    return py::bytes(s);
}
}  // namespace

CB_TEST("compat.sm83.decode_text_assemble", [] {
    auto i = sm83::decode(bytes_of({0xFA, 0x34, 0x12}), 0, 0x1000);
    CB_CHECK(i.has_value());
    CB_CHECK_EQ(i->pattern.utf8(), "LD A,(nn)");
    CB_CHECK_EQ(i->size, 3);
    CB_CHECK_EQ(i->values.size(), 1);
    CB_CHECK_EQ(i->values[0], 0x1234);
    CB_CHECK_EQ(i->flow.utf8(), "");
    CB_CHECK_EQ(i->text().utf8(), "LD A,($1234)");
    CB_CHECK_EQ(i->repr().utf8(), "Insn(LD A,($1234))");
    py::dict<int64_t, py::str> labels;
    labels.set(0x1234, py::str("far"));
    CB_CHECK_EQ(sm83::text(*i, labels).utf8(), "LD A,(far)");

    auto jump = sm83::decode(bytes_of({0x20, 0xF0}), 0, 0x1000);
    CB_CHECK(jump.has_value());
    CB_CHECK_EQ(jump->flow.utf8(), "branch");
    CB_CHECK(jump->target.has_value());
    CB_CHECK_EQ(*jump->target, 0x0FF2);
    CB_CHECK(!sm83::decode(bytes_of({0xD3}), 0, 0).has_value());
    CB_CHECK(!sm83::decode(bytes_of({0x3E}), 0, 0).has_value());

    CB_CHECK_EQ(sm83::assemble(py::str("LDH A,(rLY)"), 0, [] {
                    py::dict<py::str, int64_t> d;
                    d.set(py::str("rLY"), 0xFF44);
                    return d;
                }()).raw(),
                std::string("\xF0\x44"));
    CB_CHECK_THROWS_AS(py::ValueError, sm83::assemble(py::str("FOO")));
    CB_CHECK_THROWS_AS(py::KeyError, sm83::assemble(py::str("LD A,missing")));
});

CB_TEST("compat.sm83.tables", [] {
    CB_CHECK_EQ(sm83::TABLE.size(), 511);   // 255 main opcodes ($CB is the prefix) and the 256 of the CB page
    CB_CHECK(!sm83::TABLE.at(sm83::TableKey(py::bytes(), 0xD3)).has_value());
    CB_CHECK_EQ(sm83::TABLE.at(sm83::TableKey(bytes_of({0xCB}), 0x30))->utf8(), "SWAP B");
    CB_CHECK_EQ(sm83::ILLEGAL.size(), 11);
    CB_CHECK_EQ(sm83::CC.size(), 4);
    CB_CHECK_EQ(sm83::_main(0x3E)->utf8(), "LD A,n");
    CB_CHECK(!sm83::_main(0xFD).has_value());
    CB_CHECK_EQ(sm83::_cb(0x7E).utf8(), "BIT 7,(HL)");
    CB_CHECK_EQ(sm83::_flow(py::str("RET NZ")).utf8(), "cret");
    CB_CHECK_EQ(sm83::_fields(py::str("LDH (h),A")).size(), 1);
    CB_CHECK_EQ(sm83::_choices(2).size(), 25);
    CB_CHECK(sm83::_WORDS.contains(py::str("(HL+)")));
});

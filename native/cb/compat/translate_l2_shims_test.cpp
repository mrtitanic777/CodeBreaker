// Tests of the shim at the old header path of translate/l2: the old py:: API of the module model (module, function, fragment,
// comment, units, texts, Made, Assembly) over cb/translate/l2. Built at /W3 (the old layer is not warning-clean), like the
// other shim tests. What needs the old IR tree (type_json, to_json, from_json over py::ref nodes) and the L2 core library
// (validate) cannot be linked into cb_tests; those are compiled by compat/shim_check_translate.cpp.
#include "codebreaker/translate/l2.hpp"

#include "cb/core/testing.hpp"

namespace l2_old = codebreaker::translate::l2;
using l2_old::Dict;
using l2_old::Val;

namespace {

py::str S(const char* s) { return py::str(s); }
std::string text_of(const Dict& d, const char* key) { return std::get<py::str>(d.at(S(key)).v).utf8(); }

}  // namespace

CB_TEST("compat.translate.l2_module_model", [] {
    const Dict f = l2_old::function(S("sub_1000"), S("int sub_1000(void) { return 1; }"), std::nullopt, py::list<py::str>{S("warning: guessed")});
    CB_CHECK_EQ(text_of(f, "node"), "MethodDecl");
    CB_CHECK_EQ(text_of(f, "kind"), "function");
    CB_CHECK_EQ(text_of(f, "name"), "sub_1000");
    CB_CHECK(!f.contains(S("origin")));
    // **fields override what is set before them
    Dict extra;
    extra.set(S("kind"), Val(S("method")));
    extra.set(S("line"), Val(int64_t(7)));
    const Dict g = l2_old::function(S("g"), std::nullopt, std::nullopt, std::nullopt, extra);
    CB_CHECK_EQ(text_of(g, "kind"), "method");
    CB_CHECK_EQ(std::get<int64_t>(g.at(S("line")).v), 7);
    CB_CHECK(!g.contains(S("text")));

    // an origin as the dict the old code builds
    Dict origin;
    origin.set(S("module"), Val(S("lz32.dll")));
    origin.set(S("va"), Val(int64_t(0x1000)));
    const Dict placed = l2_old::function(S("p"), S("t"), origin);
    CB_CHECK_EQ(std::get<int64_t>(std::get<Dict>(placed.at(S("origin")).v).at(S("va")).v), 0x1000);

    const Dict frag = l2_old::fragment(S("mov ax, 1"), S("start"), S("masm"));
    CB_CHECK_EQ(text_of(frag, "node"), "MachineFragment");
    CB_CHECK_EQ(text_of(frag, "asm"), "mov ax, 1");
    CB_CHECK_EQ(text_of(frag, "syntax"), "masm");
    CB_CHECK(!frag.contains(S("reason")));
    const Dict sized = l2_old::fragment(std::nullopt, S("g"), std::nullopt, std::nullopt, S("hand-written"), std::nullopt, 12, S("f"));
    CB_CHECK_EQ(std::get<int64_t>(sized.at(S("length")).v), 12);
    CB_CHECK_EQ(text_of(sized, "within"), "f");
    CB_CHECK_EQ(text_of(l2_old::comment(S("status")), "text"), "status");

    py::list<Val> units{Val(f), Val(frag), Val(l2_old::comment(S("c"))), Val(g)};
    const Dict m = l2_old::module(units, S("typedef int I;\n"));
    CB_CHECK_EQ(text_of(m, "node"), "Module");
    CB_CHECK_EQ(text_of(m, "header"), "typedef int I;\n");
    CB_CHECK_EQ(l2_old::units(m).size(), 4);
    CB_CHECK_EQ(l2_old::units(m, S("MethodDecl")).size(), 2);
    CB_CHECK_EQ(l2_old::units(m, S("Comment")).size(), 1);
    const auto texts = l2_old::texts(m);
    CB_CHECK_EQ(texts.size(), 1);
    CB_CHECK_EQ(texts.at(S("sub_1000")).utf8(), "int sub_1000(void) { return 1; }");
    // a module with no header has no header key
    CB_CHECK(!l2_old::module().contains(S("header")));
    CB_CHECK_EQ(l2_old::units(l2_old::module()).size(), 0);
});

CB_TEST("compat.translate.l2_made", [] {
    py::dict<py::str, py::str> texts;
    texts.set(S("b"), S("B text"));
    texts.set(S("a"), S("A text"));
    py::list<py::str> order{S("a"), S("missing"), S("b")};
    py::dict<int64_t, py::str> by_va;
    by_va.set(0x2000, S("a"));
    l2_old::Made m(S("lz.dll"), S("/* h */"), texts, order, by_va);
    CB_CHECK_EQ(m.name.utf8(), "lz.dll");
    CB_CHECK_EQ(m.header().utf8(), "/* h */");
    CB_CHECK_EQ(m.order().size(), 3);
    CB_CHECK_EQ(m.order()[1].utf8(), "missing");
    CB_CHECK_EQ(m.texts().size(), 2);
    CB_CHECK_EQ(m.by_va().at(0x2000).utf8(), "a");
    CB_CHECK_EQ(std::get<0>(m.functions().at(0x2000)).utf8(), "a");
    m.set_text(S("a"), S("A2"));
    CB_CHECK_EQ(m.texts().at(S("a")).utf8(), "A2");
    py::dict<py::str, py::str> more;
    more.set(S("new"), S("N"));
    m.set_texts(more);
    CB_CHECK_EQ(m.order().size(), 4);   // a name the module does not have is added last
    m.add(S("late"), S("L"), 0x3000);
    CB_CHECK_EQ(m.by_va().at(0x3000).utf8(), "late");
    m.set_header(S("h2"));
    CB_CHECK_EQ(m.header().utf8(), "h2");
    // the facts a lane keeps beside the module
    Dict facts;
    facts.set(S("near"), Val(true));
    l2_old::Made with_facts(S("x"), S(""), std::nullopt, std::nullopt, std::nullopt, facts);
    CB_CHECK(with_facts.facts.contains(S("near")));
    CB_CHECK_EQ(with_facts.order().size(), 0);
});

CB_TEST("compat.translate.l2_assembly", [] {
    l2_old::Assembly L(py::list<py::str>{S("; head"), S("bits 16")}, S("nasm"), S("x.dll"));
    Dict origin;
    origin.set(S("module"), Val(S("x.dll")));
    origin.set(S("segment"), Val(int64_t(1)));
    origin.set(S("offset"), Val(int64_t(0)));
    L.part(S("seg1"), S("segment"), origin, 4);
    L.append(S(""));
    L.extend(py::list<py::str>{S("section seg1"), S("    db 1,2,3,4")});
    L.part(S("empty"));   // (a part with no lines is left out)
    L.part(S("tail"), S("end"));
    L.append(S(""));
    L.append(S("; end"));
    const Dict doc = L.document();
    const auto& units = std::get<Val::List>(doc.at(S("units")).v);
    CB_CHECK_EQ(units.size(), 3);
    const Dict& first = std::get<Dict>(units[0].v);
    CB_CHECK_EQ(text_of(first, "name"), "head");
    CB_CHECK_EQ(text_of(first, "asm"), "; head\nbits 16");
    CB_CHECK_EQ(text_of(first, "syntax"), "nasm");
    const Dict& seg = std::get<Dict>(units[1].v);
    CB_CHECK_EQ(text_of(seg, "name"), "seg1");
    CB_CHECK_EQ(text_of(seg, "asm"), "\nsection seg1\n    db 1,2,3,4");
    CB_CHECK_EQ(std::get<int64_t>(seg.at(S("length")).v), 4);
    CB_CHECK_EQ(std::get<int64_t>(std::get<Dict>(seg.at(S("origin")).v).at(S("segment")).v), 1);
    CB_CHECK_EQ(text_of(std::get<Dict>(units[2].v), "asm"), "\n; end");
});

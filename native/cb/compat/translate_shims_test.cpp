// Tests of the shims at the old header paths for the translator, verify/authored and process helpers: the old py:: API over
// the new modules. Built at /W3 (the old layer is not warning-clean), like py_bridge_test.cpp.
#include "codebreaker/translate/backends/asm.hpp"
#include "codebreaker/translate/backends/module_file.hpp"
#include "codebreaker/translate/dotnet/package.hpp"
#include "codebreaker/translate/frontends/python/pytypes.hpp"
#include "codebreaker/translate/frontends/python/structfmt.hpp"
#include "codebreaker/verify/authored/search16.hpp"
#include "codebreaker/verify/authored/shape.hpp"

#include "cb/core/testing.hpp"

namespace asm_old = codebreaker::translate::backends::asm_;
namespace mf_old = codebreaker::translate::backends::module_file;
namespace sf_old = codebreaker::translate::frontends::python::structfmt;
namespace pt_old = codebreaker::translate::frontends::python::pytypes;
namespace shape_old = codebreaker::verify::authored::shape;
namespace s16_old = codebreaker::verify::authored::search16;
namespace dn_old = codebreaker::translate::dotnet;

// ---- backends/asm ---------------------------------------------------------------------------------------------
CB_TEST("compat.translate.asm", [] {
    asm_old::Document doc;
    doc.node = py::str("Module");
    py::list<asm_old::Unit> units;
    asm_old::Unit comment;
    comment.node = py::str("Comment");
    comment.text = py::str("lz32");
    units.append(comment);
    asm_old::Unit frag;
    frag.node = py::str("MachineFragment");
    frag.asm_ = py::str("a\nb");
    units.append(frag);
    asm_old::Unit inner;
    inner.node = py::str("MachineFragment");
    inner.within = true;
    units.append(inner);
    asm_old::Unit tail;
    tail.node = py::str("MachineFragment");
    tail.asm_ = py::str("c");
    units.append(tail);
    doc.units = units;
    CB_CHECK_EQ(asm_old::module(doc).utf8(), "; lz32\na\nb\nc");
    const auto [text, first] = asm_old::lines(doc);
    CB_CHECK_EQ(text.utf8(), "; lz32\na\nb\nc");
    CB_CHECK_EQ(first.at(0), 1);
    CB_CHECK_EQ(first.at(1), 2);
    CB_CHECK(!first.contains(2));
    CB_CHECK_EQ(first.at(3), 4);
    CB_CHECK_EQ(asm_old::comment(py::str("")).utf8(), ";");

    // the one tolerated odd shape: a single unit instead of a list
    asm_old::Document single;
    single.node = py::str("Module");
    single.units = tail;
    CB_CHECK_EQ(asm_old::module(single).utf8(), "c");
    single.units = asm_old::Unit();   // the empty dict
    CB_CHECK_EQ(asm_old::module(single).utf8(), "");

    // a unit that is not assembly, a fragment with no text, a document that is not a Module: Decline
    asm_old::Unit other;
    other.node = py::str("Function");
    doc.units = py::list<asm_old::Unit>{other};
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)asm_old::module(doc));
    asm_old::Unit bare;
    bare.node = py::str("MachineFragment");
    bare.name = py::str("oops");
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)asm_old::unit(bare));
    doc.node = py::str("Unit");
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)asm_old::module(doc));
    doc.node = std::nullopt;
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)asm_old::module(doc));
});

// ---- frontends/python/structfmt --------------------------------------------------------------------------------
CB_TEST("compat.translate.structfmt", [] {
    const auto items = sf_old::parse(py::str("<HHI"));
    CB_CHECK(items.has_value());
    CB_CHECK_EQ(items->size(), 3);
    const auto& first = (*items)[0];
    CB_CHECK_EQ(std::get<0>(first).utf8(), "H");
    CB_CHECK_EQ(std::get<1>(first), 2);
    CB_CHECK_EQ(std::get<2>(first).utf8(), "int");
    CB_CHECK_EQ(std::get<3>(first), 0);
    CB_CHECK_EQ(std::get<4>(first), 65535u);
    CB_CHECK_EQ(sf_old::size(*items), 8);
    const auto q = sf_old::parse(py::str("<q4x"));
    CB_CHECK_EQ(std::get<3>((*q)[0]), INT64_MIN);
    CB_CHECK_EQ(sf_old::values(*q).size(), 1);
    CB_CHECK_EQ(std::get<2>((*q)[1]).utf8(), "pad");
    CB_CHECK_EQ(std::get<4>(sf_old::parse(py::str("<Q")).value()[0]), UINT64_MAX);
    CB_CHECK(!sf_old::parse(py::str("<P")).has_value());
    CB_CHECK(!sf_old::parse(py::str("HI")).has_value());
    CB_CHECK_EQ(sf_old::parse(py::str(""))->size(), 0);   // (the old port raised IndexError)
});

// ---- frontends/python/pytypes ----------------------------------------------------------------------------------
CB_TEST("compat.translate.pytypes", [] {
    namespace P = pt_old;
    using Big = pyengine::Big;
    const P::T a = P::INT(Big(0), Big(255)), b = P::INT(Big(1), Big(1));
    CB_CHECK_EQ(P::iadd(a, b).repr().utf8(), "int[1..256]");
    CB_CHECK_EQ(P::imul(P::u(64), P::u(64)).repr().utf8(), "int[0..340282366920938463426481119284349108225]");
    CB_CHECK_EQ(P::ixor(P::INT(Big(5), Big(5)), P::INT(Big(5), Big(5))).repr().utf8(), "int[0..7]");
    CB_CHECK(P::ixor(P::INT(Big(5), Big(5)), P::INT(Big(5), Big(5))).has_zero());
    CB_CHECK(P::u(8).fits_i64());
    CB_CHECK(!P::u(64).fits_i64());
    CB_CHECK(P::SIZE().nonneg());
    CB_CHECK_EQ(P::MEM().str(), "140737488355328");
    CB_CHECK_EQ(P::I64_MIN().str(), "-9223372036854775808");
    // a bound past int64 goes through and comes back whole
    const P::T wide = P::const_int(Big::from_decimal("123456789012345678901234567890"));
    CB_CHECK_EQ(wide.lo->str(), "123456789012345678901234567890");
    CB_CHECK_EQ(P::ineg(wide).hi->str(), "-123456789012345678901234567890");
    // a type with a name, a site and why: it round trips; `why` is not part of equality
    const P::T inst(py::str("inst"), {}, std::nullopt, std::nullopt, py::str("a.B"));
    CB_CHECK_EQ(P::opt(inst).repr().utf8(), "opt[inst a.B]");
    CB_CHECK(P::top(py::str("one")) == P::top(py::str("two")));
    CB_CHECK(P::top(py::str("one")).hash() == P::top(py::str("two")).hash());
    CB_CHECK(P::strip_opt(P::opt(inst)) == inst);
    CB_CHECK(P::tuple_of({a, P::STR()}).args.size() == 2);
    CB_CHECK_EQ(P::tuple_of({a, P::STR()}).repr().utf8(), "tuple[int[0..255], str]");
    CB_CHECK_EQ(P::meet(P::INT(), Big(0), Big(255)).repr().utf8(), "int[0..255]");
    CB_CHECK_EQ(P::widen_int(P::INT(Big(0), Big(10)), P::INT(Big(0), Big(11))).repr().utf8(), "int[0..255]");
    CB_CHECK_EQ(P::bytes_of(Big(4), py::str("bytearray")).repr().utf8(), "bytearray(<=4)");
    CB_CHECK_EQ(P::ifloordiv(P::INT(Big(-7), Big(7)), P::INT(Big(2), Big(3))).repr().utf8(), "int[-4..3]");
    CB_CHECK_THROWS_AS(py::ValueError, (void)P::opt(P::T(py::str("no such kind"))));
});

// ---- verify/authored/shape -------------------------------------------------------------------------------------
CB_TEST("compat.translate.shape", [] {
    const py::str body("\n    if (x == 1) { a(); } else if (x == 2) { b(); }\n}");
    const auto edits = shape_old::switches(body, body);
    CB_CHECK_EQ(edits.size(), 1);
    CB_CHECK_EQ(std::get<0>(edits[0]), 5);
    CB_CHECK_EQ(std::get<2>(edits[0]).utf8(), "switch (x) {\ncase 1:\n{ a(); }\nbreak;\ncase 2:\n{ b(); }\nbreak;\n}");
    const py::str s("{ a = 1; if (x) { b(); } else { c(); } w; }");
    const auto spans = shape_old::statements(s, 1, s.size() - 1);
    CB_CHECK_EQ(spans.size(), 3);
    CB_CHECK_EQ(std::get<1>(spans[0]), 8);
    const py::str head("int4 f(int4 a)\n{");
    const py::str rbody("\n    r = a;\n    return r;\n}");
    const auto ret = shape_old::returns(head, py::list<py::str>{py::str("int4 r;")}, rbody, rbody);
    CB_CHECK(ret.has_value());
    CB_CHECK_EQ(ret->utf8(), "\n    return a;\n    return r;\n}");
    CB_CHECK(!shape_old::returns(head, py::list<py::str>{py::str("int2 r;")}, rbody, rbody).has_value());
    CB_CHECK_EQ(shape_old::_returned(head)->utf8(), "int4");
    const py::str fbody("\n    t = foo(a);\n    if (t == 0) {\n    }\n}");
    const auto folds = shape_old::folds(py::list<py::str>{py::str("int4 t;")}, fbody, fbody);
    CB_CHECK_EQ(folds.size(), 1);
    CB_CHECK_EQ(std::get<2>(folds[0]).utf8(), "    if (foo(a) == 0) {");
});

// ---- verify/authored/search16 ----------------------------------------------------------------------------------
CB_TEST("compat.translate.search16", [] {
    using Verdict = s16_old::Verdict;
    const Verdict far(py::str("different"), 6, 20);
    const s16_old::Group group(py::str("mc2"), py::str("-O"), py::str("w"));
    py::dict<py::str, s16_old::Near> near;
    near.set(py::str("f"), s16_old::Near(py::str("f()\n{\n    int2 x;\n    int4 y;\n}"), far, group));
    int calls = 0;
    const s16_old::CompileGroup compile = [&](const s16_old::Group&, const py::dict<py::str, py::str>& texts) {
        ++calls;
        py::dict<py::str, Verdict> out;
        for (const auto& kv : texts.items()) {
            const bool swapped = std::get<1>(kv).utf8() == "f()\n{\n    int4 y;\n    int2 x;\n}";
            out.set(std::get<0>(kv), swapped ? Verdict(py::str("identical"), 0, 20) : far);
        }
        return out;
    };
    const s16_old::Score score = [](const Verdict& v) { return std::get<0>(v).utf8() == "identical" ? 100.0 : -static_cast<double>(std::get<1>(v)); };
    std::vector<std::string> log;
    const auto best = s16_old::search(near, compile, score, 4, 1995, [&](const py::str& line) { log.push_back(line.utf8()); });
    CB_CHECK_EQ(best.size(), 1);
    CB_CHECK_EQ(std::get<0>(best.at(py::str("f"))).utf8(), "f()\n{\n    int4 y;\n    int2 x;\n}");
    CB_CHECK_EQ(std::get<0>(std::get<1>(best.at(py::str("f")))).utf8(), "identical");
    CB_CHECK(calls >= 1);
    CB_CHECK(!log.empty());
    // a verdict word that is no comparison word is refused
    near.set(py::str("g"), s16_old::Near(py::str("g()\n{\n}"), Verdict(py::str("bogus"), 0, 0), group));
    CB_CHECK_THROWS_AS(py::ValueError, (void)s16_old::search(near, compile, score, 1, 1995, {}));
});

// ---- dotnet/package: run_process -------------------------------------------------------------------------------
CB_TEST("compat.translate.dotnet_run_process", [] {
    const auto r = dn_old::run_process(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("echo hello&& echo bye 1>&2&& exit 3")});
    CB_CHECK_EQ(r.returncode, 3);
    CB_CHECK(r.stdout_.raw().find("hello") != std::string::npos);
    CB_CHECK(r.stderr_.raw().find("bye") != std::string::npos);
    // stderr into stdout
    const auto merged = dn_old::run_process(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("echo out&& echo err 1>&2")}, std::nullopt, std::nullopt,
                                            std::nullopt, true);
    CB_CHECK(merged.stdout_.raw().find("out") != std::string::npos);
    CB_CHECK(merged.stdout_.raw().find("err") != std::string::npos);
    CB_CHECK(merged.stderr_.raw().empty());
    // stdin, and an environment on top of ours
    py::dict<py::str, py::str> env;
    env.set(py::str("CB_SHIM_TEST"), py::str("seen"));
    const auto echoed = dn_old::run_process(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("echo %CB_SHIM_TEST%")}, env);
    CB_CHECK(echoed.stdout_.raw().find("seen") != std::string::npos);
    // the program cannot be started: OSError; a timeout: TimeoutExpired
    CB_CHECK_THROWS_AS(py::FileNotFoundError, (void)dn_old::run_process(py::list<py::str>{py::str("no-such-program-cb")}));
    CB_CHECK_THROWS_AS(dn_old::TimeoutExpired, (void)dn_old::run_process(py::list<py::str>{py::str("cmd"), py::str("/c"), py::str("ping -n 6 127.0.0.1 >nul")}, std::nullopt, std::nullopt, 0.5));
});

// ---- backends/module_file ---------------------------------------------------------------------------------------
CB_TEST("compat.translate.module_file", [] {
    mf_old::Facts facts;
    facts.name = py::str("lz32.dll");
    facts.stem = py::str("lz32");
    facts.date = py::str("2026-11-11");
    facts.digest = py::str("ab12");
    facts.order = py::list<py::str>{py::str("verified"), py::str("weak")};
    facts.titles.set(py::str("verified"), py::str("VERIFIED"));
    facts.titles.set(py::str("weak"), py::str("WEAK"));
    facts.counts.set(py::str("verified"), 2);
    facts.counts.set(py::str("weak"), 1);
    facts.total = 3;
    facts.kind = py::str("pe");
    facts.header = true;

    mf_old::Document doc;
    doc.node = py::str("Module");
    mf_old::Unit fn;
    fn.node = py::str("MethodDecl");
    fn.comments = py::list<py::str>{py::str("VERIFIED")};
    fn.text = py::str("int f(void) { return 1; }");
    mf_old::Unit none;
    none.node = py::str("Comment");
    none.text = py::str("WEAK  sub_2");
    doc.units = py::list<mf_old::Unit>{fn, none};

    const auto files = mf_old::c(doc, facts, 0);
    CB_CHECK_EQ(files.size(), 1);
    CB_CHECK_EQ(std::get<0>(files[0]).utf8(), "lz32.c");
    const std::string text = std::get<1>(files[0]).utf8();
    CB_CHECK(text.starts_with("/* lz32.dll -- reconstructed source.\n *\n"));
    CB_CHECK(text.find(" *   VERIFIED                         2   66.7%\n") != std::string::npos);
    CB_CHECK(text.ends_with("#include \"lz32.h\"\n\n/* VERIFIED */\nint f(void) { return 1; }\n\n/* WEAK  sub_2 */\n"));
    CB_CHECK_EQ(mf_old::c(doc, facts, 1).size(), 2);
    CB_CHECK_EQ(mf_old::banner(facts)[0].utf8(), "/* lz32.dll -- reconstructed source.");

    // a document that is not a Module, or has a unit that is neither: Decline
    doc.node = py::str("Unit");
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)mf_old::c(doc, facts, 0));
    doc.node = py::str("Module");
    mf_old::Unit other;
    other.node = py::str("Function");
    doc.units = py::list<mf_old::Unit>{other};
    CB_CHECK_THROWS_AS(asm_old::Decline, (void)mf_old::c(doc, facts, 0));

    // the assembly file: the status as comments, then the source
    asm_old::Document source;
    source.node = py::str("Module");
    asm_old::Unit frag;
    frag.node = py::str("MachineFragment");
    frag.asm_ = py::str("mov ax, 1");
    source.units = py::list<asm_old::Unit>{frag};
    const std::string listing = mf_old::asm_(source, facts).utf8();
    CB_CHECK(listing.starts_with("; lz32.dll -- reconstructed source.\n;\n"));
    CB_CHECK(listing.ends_with(";\nmov ax, 1"));
    CB_CHECK_EQ(mf_old::asm_banner(facts).size(), 7);
});

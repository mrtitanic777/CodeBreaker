// Tests of the C++03 back end (cpp03), on real Java programs: the units the Java front end lowers (testdata/cpp03/PROG/units/N.json, as L2),
// the converter's parse of each source (src/*.java) and resolver (cfg.json), written by the C++03 and the C++17 dialects and compared with
// the recorded answer for every member (expected.txt: body, constructors, initialiser, array items, enum arguments,
// instance and static initialisers, plus what it counted and what it put on the ledger). The programs are the translator's own corpus
// (calc, sample, minecraft, modern, bank, generics, shapes, streams, textstats), the programs of its test suites (t03, t17) and `feat`,
// written to reach the branches the others do not: every literal form, shifts, float %, casts, ternaries, arrays, every loop and switch
// form, labelled jumps, string switches, switch expressions, instanceof patterns, for-each, try / catch / finally with jumps, order-
// sensitive operands, boxed types, reflection, ownership patterns, enums, constructor chains, varargs, marker arguments.
// CB_CPP03_DATA names a folder of further programs of the same shape (the Minecraft Alpha and Beta sources: ~22,000 answers).
//
// Behaviour pinned here:
//   * the back end does not rewrite the tree it reads: a call's marker arguments are dropped in the rendering only, so a second
//     rendering of the same call (a finally body is written twice) sees the full list;
//   * `new T[]{}` (an initialiser with no elements) is declined, not written `(JArrayInit<T >(), )`;
//   * the text of a decline reason for a type is the IR's own spelling of the type;
//   * a float literal is printed by the shortest-digits rule: fixed notation from 1e-4 up to 1e16, scientific outside.
#include "cb/translate/backends/cpp03.hpp"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <limits>
#include <cstdio>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/java/parse.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace c3 = cb::translate::backends::cpp03;
namespace ir = cb::translate::ir;
namespace jir = cb::java::ir;
namespace jtypes = cb::java::types;
using cb::Json;

// ---- a program: its parsed classes, resolver, lowered units
struct C3Program {
    std::string name;
    std::vector<std::string> modes;
    std::map<std::uint32_t, std::uint32_t> char_map;
    std::vector<jir::JClass> classes;
    jtypes::ResolverData data;
    std::vector<Json> units;
    std::vector<std::string> expected;   // the lines of expected.txt
};

jir::JType c3_jtype(const Json& v) {
    jir::JType t;
    t.name = v[0].get<std::string>();
    t.array_dims = v[1].get<std::uint32_t>();
    for (const Json& a : v[2]) t.args.push_back(c3_jtype(a));
    return t;
}

cb::java::typemap::NameSet c3_names(const Json& v) {
    cb::java::typemap::NameSet out;
    for (const Json& x : v) out.insert(x.get<std::string>());
    return out;
}

C3Program c3_load(const std::filesystem::path& dir) {
    C3Program p;
    const Json info = cb::json::load(dir / "case.json");
    p.name = info.at("prog").get<std::string>();
    for (const Json& m : info.at("modes")) p.modes.push_back(m.get<std::string>());
    for (const auto& [k, v] : info.at("char_map").items()) p.char_map[static_cast<std::uint32_t>(std::stoul(k))] = v.get<std::uint32_t>();
    std::vector<std::filesystem::path> sources;
    for (const auto& e : std::filesystem::directory_iterator(dir / "src")) sources.push_back(e.path());
    std::sort(sources.begin(), sources.end());   // (the files were copied with their position as a prefix)
    for (const auto& s : sources) p.classes.push_back(cb::java::parse::parse_file(s));
    const Json cfg = cb::json::load(dir / "cfg.json");
    jtypes::ResolverData& d = p.data;
    for (const auto& [cls, env] : cfg.at("members").items()) {
        jtypes::MemberEnv m;
        for (const auto& [f, t] : env.at("fields").items()) m.fields[f] = c3_jtype(t);
        for (const auto& [f, t] : env.at("methods").items()) m.methods[f] = c3_jtype(t);
        if (!env.at("parent").is_null()) m.parent = env.at("parent").get<std::string>();
        d.members[cls] = std::move(m);
    }
    for (const auto& [cls, names] : cfg.at("method_renames").items()) d.method_renames[cls] = c3_names(names);
    d.known = c3_names(cfg.at("known"));
    d.values = c3_names(cfg.at("values"));
    d.interfaces = c3_names(cfg.at("interfaces"));
    d.enums = c3_names(cfg.at("enums"));
    d.typed_enums = c3_names(cfg.at("typed_enums"));
    d.thread_like = c3_names(cfg.at("thread_like"));
    d.markers = c3_names(cfg.at("markers"));
    const Json& oc = cfg.at("own_catalog");
    d.own_catalog.return_methods = c3_names(oc.at("return_methods"));
    for (const auto& [k, v] : oc.at("scalar_methods").items()) d.own_catalog.scalar_methods[k] = v.get<std::string>();
    for (const auto& [k, v] : oc.at("retained_params").items()) {
        for (const Json& x : v) d.own_catalog.retained_params[k].insert(x.get<std::size_t>());
    }
    d.own_catalog.corpus_methods = c3_names(oc.at("corpus_methods"));
    for (std::size_t i = 0; i < info.at("units").get<std::size_t>(); ++i)
        p.units.push_back(cb::json::load(dir / "units" / (std::to_string(i) + ".json")));
    const std::string expected_text = cb::fs::read_text(dir / "expected.txt");
    for (const std::string_view line : cb::text::lines(expected_text)) {
        if (!line.empty()) p.expected.emplace_back(line);
    }
    return p;
}

const cb::java::runtime::Runtime& c3_runtime() {
    static const cb::java::runtime::Runtime r{cb::java::JavaData(cb::testing::data_path("../../data/java"))};
    return r;
}

// ---- the recorded answers
using Answers = std::vector<std::pair<std::string, Json>>;   // key -> answer

Json c3_lines(const std::optional<std::vector<std::string>>& r) {
    if (!r) return nullptr;
    return Json(*r);
}

Json c3_ctor(const std::optional<cb::java::emit::source::CtorBody>& r) {
    if (!r) return nullptr;
    using cb::java::emit::source::CtorCall;
    Json kind = r->call == CtorCall::Super ? Json("super") : r->call == CtorCall::This ? Json("this") : Json(nullptr);
    return Json::array({kind, r->super_args ? Json(*r->super_args) : Json(nullptr), r->lines});
}

void c3_queries(const std::string& tag, c3::Bodies& bodies, const std::vector<jir::JClass>& classes, Answers& out) {
    int n = 0;
    const auto emit = [&](const char* kind, const std::string& what, Json value) {
        out.emplace_back(tag + "|" + std::to_string(n++) + "|" + kind + "|" + what, std::move(value));
    };
    const auto tf = [](bool b) { return b ? "True" : "False"; };
    for (const jir::JClass& c : classes) {
        for (const jir::Method& m : c.methods) {
                if (m.is_ctor) {
                for (const bool tp : {false, true}) emit("ctor", c.name + "." + tf(tp), c3_ctor(bodies.ctor(c, &m, tp)));
            } else {
                emit("body", c.name + "." + m.name, c3_lines(bodies.body(c, m)));
            }
        }
        for (const bool tp : {false, true}) emit("ctor0", c.name + "." + tf(tp), c3_ctor(bodies.ctor(c, nullptr, tp)));
        for (const jir::Field& f : c.fields) {
            const auto init = bodies.field_init(c, f);
            emit("field_init", c.name + "." + f.name, init ? Json(*init) : Json(nullptr));
            emit("array_items", c.name + "." + f.name, c3_lines(bodies.array_items(c, f)));
        }
        for (const jir::EnumConst& e : c.enum_consts) emit("enum_args", c.name + "." + e.name, c3_lines(bodies.enum_args(c, e)));
        emit("instance_inits", c.name, c3_lines(bodies.instance_inits(c)));
        const auto blocks = bodies.static_blocks(c);
        emit("static_blocks", c.name, blocks ? Json(*blocks) : Json(nullptr));
    }
    // what does not exist
    const jir::Method* some = nullptr;
    const jir::Method* some_ctor = nullptr;
    for (const jir::JClass& c : classes) {
        for (const jir::Method& m : c.methods) {
            if (!m.is_ctor && some == nullptr) some = &m;
            if (m.is_ctor && some_ctor == nullptr) some_ctor = &m;
        }
    }
    jir::JClass missing;
    missing.name = "NoSuchClass";
    emit("body", "NoSuchClass", c3_lines(some != nullptr ? bodies.body(missing, *some) : std::nullopt));
    if (!classes.empty()) {
        jir::Field nofield;
        nofield.name = "noSuchField";
        const auto init = bodies.field_init(classes[0], nofield);
        emit("field_init", "nofield", init ? Json(*init) : Json(nullptr));
        emit("array_items", "nofield", c3_lines(bodies.array_items(classes[0], nofield)));
        jir::EnumConst nope;
        nope.name = "NOPE";
        emit("enum_args", "noconst", c3_lines(bodies.enum_args(classes[0], nope)));
    }
    emit("ctor", "NoSuchClass", c3_ctor(bodies.ctor(missing, some_ctor, false)));
    jir::Field x;
    x.name = "x";
    const auto init = bodies.field_init(missing, x);
    emit("field_init", "NoSuchClass", init ? Json(*init) : Json(nullptr));
    emit("array_items", "NoSuchClass", c3_lines(bodies.array_items(missing, x)));
    jir::EnumConst xc;
    xc.name = "x";
    emit("enum_args", "NoSuchClass", c3_lines(bodies.enum_args(missing, xc)));
    emit("instance_inits", "NoSuchClass", c3_lines(bodies.instance_inits(missing)));
    const auto blocks = bodies.static_blocks(missing);
    emit("static_blocks", "NoSuchClass", blocks ? Json(*blocks) : Json(nullptr));
}

struct C3Run {
    Answers answers;
    std::size_t desugared = 0;
    Json counts;    // [written, written_ctors, declined, reasons, class names]
    Json ledger;    // [[file, line, category, severity, source, rule, message, snippet] ...]
};

C3Run c3_run(const C3Program& p, const std::string& mode) {
    cb::Arena arena;
    std::vector<ir::Node*> units;
    for (const Json& u : p.units) units.push_back(cb::translate::l2::from_json(arena, u));
    const cb::java::typemap::TypeProfile profile;
    const jtypes::TypeResolver resolver(p.data);
    c3::Context ctx(resolver, profile, c3_runtime(), p.classes);
    cb::java::ledger::Ledger ledger;
    ctx.ledger = &ledger;
    ctx.char_map = p.char_map;
    c3::Bodies bodies(ctx, arena, units, mode == "17" ? c3::Dialect::Cpp17 : c3::Dialect::Cpp03);
    C3Run run;
    run.desugared = bodies.desugared();
    c3_queries(p.name + "|" + mode, bodies, p.classes, run.answers);
    Json declined = Json::object();
    for (const auto& [k, v] : bodies.declined()) declined[k] = v;
    Json reasons = Json::object();
    for (const auto& [k, v] : bodies.reasons()) reasons[k] = v;
    run.counts = Json::array({bodies.written(), bodies.written_ctors(), declined, reasons, bodies.class_names()});
    run.ledger = Json::array();
    for (const auto& e : ledger.events()) {
        run.ledger.push_back(Json::array({e.file, e.line, std::string(cb::java::ledger::name(e.category)),
                                          std::string(cb::java::ledger::name(e.severity)), e.source, e.rule, e.message, e.snippet}));
    }
    return run;
}

// What the program's expected.txt says for a mode: the answers in order, and the three summary lines.
struct C3Expected {
    Answers answers;
    std::optional<Json> desugar, counts, ledger;
};

C3Expected c3_expected(const C3Program& p, const std::string& mode) {
    const std::string tag = p.name + "|" + mode + "|";
    C3Expected e;
    for (const std::string& line : p.expected) {
        if (!line.starts_with(tag)) continue;
        const std::size_t tab = line.find('\t');
        const std::string key = line.substr(0, tab);
        Json value = cb::json::parse(std::string_view(line).substr(tab + 1));
        const std::string what = key.substr(tag.size());
        if (what == "desugar") e.desugar = value;
        else if (what == "counts") e.counts = value;
        else if (what == "ledger") e.ledger = value;
        else e.answers.emplace_back(key, std::move(value));
    }
    return e;
}

// Compares a run with the recorded answers. Returns the number of members that differ (the first few are printed).
std::size_t c3_compare(const C3Program& p, const std::string& mode, const C3Run& run, const C3Expected& want) {
    std::size_t bad = 0;
    const auto report = [&](const std::string& what, const Json& got, const Json& expected) {
        if (++bad <= 5) std::fprintf(stderr, "cpp03 %s|%s %s\n  got      %.600s\n  expected %.600s\n", p.name.c_str(), mode.c_str(), what.c_str(),
                                     got.dump().c_str(), expected.dump().c_str());
    };
    if (run.answers.size() != want.answers.size()) {
        ++bad;
        std::fprintf(stderr, "cpp03 %s|%s answers: %zu, expected %zu\n", p.name.c_str(), mode.c_str(), run.answers.size(), want.answers.size());
    }
    for (std::size_t i = 0; i < std::min(run.answers.size(), want.answers.size()); ++i) {
        if (run.answers[i].first != want.answers[i].first) report("key " + run.answers[i].first, run.answers[i].first, want.answers[i].first);
        else if (run.answers[i].second != want.answers[i].second) report(run.answers[i].first, run.answers[i].second, want.answers[i].second);
    }
    if (want.desugar && Json(run.desugared) != *want.desugar) report("desugar", run.desugared, *want.desugar);
    if (want.counts) {
        const Json& g = run.counts;
        const Json& w = *want.counts;
        if (g[0] != w[0] || g[1] != w[1] || g[2] != w[2] || g[4] != w[4]) report("counts", g, w);
        std::size_t got_reasons = 0, want_reasons = 0;   // (the text of a reason is the module's own; how many there are is not)
        for (const auto& [k, v] : g[3].items()) got_reasons += v.get<std::size_t>();
        for (const auto& [k, v] : w[3].items()) want_reasons += v.get<std::size_t>();
        if (got_reasons != want_reasons) report("reasons", g[3], w[3]);
    }
    if (want.ledger && run.ledger != *want.ledger) report("ledger", run.ledger, *want.ledger);
    return bad;
}

void c3_check_program(const std::filesystem::path& dir) {
    const C3Program p = c3_load(dir);
    for (const std::string& mode : p.modes) {
        if (mode == "plain") continue;   // (the C back end's)
        const C3Run run = c3_run(p, mode);
        CB_CHECK_EQ(c3_compare(p, mode, run, c3_expected(p, mode)), std::size_t(0));
    }
}

}  // namespace

CB_TEST("translate.cpp03.feat", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/feat")); });
CB_TEST("translate.cpp03.t03", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/t03")); });
CB_TEST("translate.cpp03.t17", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/t17")); });
CB_TEST("translate.cpp03.calc", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/calc")); });
CB_TEST("translate.cpp03.sample", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/sample")); });
CB_TEST("translate.cpp03.minecraft", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/minecraft")); });
CB_TEST("translate.cpp03.modernlib", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/modernlib")); });
CB_TEST("translate.cpp03.bank", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/bank")); });
CB_TEST("translate.cpp03.generics", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/generics")); });
CB_TEST("translate.cpp03.shapes", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/shapes")); });
CB_TEST("translate.cpp03.streams", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/streams")); });
CB_TEST("translate.cpp03.textstats", [] { c3_check_program(cb::testing::data_path("translate/testdata/cpp03/textstats")); });

// The big programs (not checked in): CB_CPP03_DATA=<folder of PROG/ folders>.
CB_TEST("translate.cpp03.big", [] {
    const auto root = cb::fs::get_env("CB_CPP03_DATA");
    if (!root) return;
    const std::string data_root = *root;
    for (const auto& e : std::filesystem::directory_iterator(data_root)) {
        if (!e.is_directory()) continue;
        const auto t0 = std::chrono::steady_clock::now();
        c3_check_program(e.path());
        std::fprintf(stderr, "cpp03 %s: %.1f s\n", e.path().filename().string().c_str(),
                     std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count());
    }
});

// ---- the text rules
CB_TEST("translate.cpp03.text.floats", [] {
    CB_CHECK_EQ(c3::float_literal(0.5, "F"), std::string("0.5F"));
    CB_CHECK_EQ(c3::float_literal(1.0, ""), std::string("1.0"));
    CB_CHECK_EQ(c3::float_literal(std::int64_t(3), ""), std::string("3.0"));
    CB_CHECK_EQ(c3::float_literal(100000.0, ""), std::string("100000.0"));
    CB_CHECK_EQ(c3::float_literal(1e15, ""), std::string("1000000000000000.0"));
    CB_CHECK_EQ(c3::float_literal(1e16, ""), std::string("1.0e+16"));
    CB_CHECK_EQ(c3::float_literal(1.5e-5, ""), std::string("1.5e-05"));
    CB_CHECK_EQ(c3::float_literal(0.0001, ""), std::string("0.0001"));
    CB_CHECK_EQ(c3::float_literal(-2.25, "F"), std::string("-2.25F"));
    CB_CHECK_EQ(c3::float_literal(3.141592653589793, ""), std::string("3.141592653589793"));
    CB_CHECK_EQ(c3::float_literal(1.2345e22, ""), std::string("1.2345e+22"));
    CB_CHECK_EQ(c3::float_literal(std::string("2.5"), ""), std::string("2.5"));
    CB_CHECK_THROWS_AS(c3::Decline, (void)c3::float_literal(std::string("NaN"), ""));
    CB_CHECK_THROWS_AS(c3::Decline, (void)c3::float_literal(std::numeric_limits<double>::infinity(), ""));
    CB_CHECK_THROWS_AS(c3::Decline, (void)c3::float_literal(ir::LitValue(std::monostate{}), ""));
});

CB_TEST("translate.cpp03.text.strings", [] {
    const std::map<std::uint32_t, std::uint32_t> none;
    CB_CHECK_EQ(c3::c_string_literal("a\"b\\c\n\t", none), std::string("\"a\\\"b\\\\c\\n\\t\""));
    CB_CHECK_EQ(c3::c_string_literal("\xC2\xA7x", none), std::string("\"\\247x\""));        // the section sign: one byte, 0xA7
    CB_CHECK_EQ(c3::c_string_literal("\xE2\x82\xAC", none), std::string("\"?\""));            // outside the runtime's 8 bits
    CB_CHECK_EQ(c3::c_string_literal("\x01", none), std::string("\"\\001\""));
    const std::map<std::uint32_t, std::uint32_t> font = {{'a', 'b'}, {0xA7, 'B'}};
    CB_CHECK_EQ(c3::c_string_literal("a\xC2\xA7", font), std::string("\"bB\""));
});

CB_TEST("translate.cpp03.text.types", [] {
    CB_CHECK_EQ(c3::null_of("Block*"), std::string("NULL"));
    CB_CHECK_EQ(c3::null_of("const JavaString&"), std::string("JavaString()"));
    CB_CHECK_EQ(c3::null_of("JList<int >"), std::string("JList<int >(0LL)"));
    CB_CHECK_EQ(c3::null_of("JListing"), std::string("JListing()"));
    CB_CHECK_EQ(c3::null_of(""), std::string("NULL"));
    CB_CHECK_EQ(c3::coerce("NULL", "JavaString"), std::string("JavaString()"));
    CB_CHECK_EQ(c3::coerce("NULL", "Block*"), std::string("NULL"));
    CB_CHECK_EQ(c3::coerce("x", "JavaString"), std::string("x"));
    CB_CHECK_EQ(c3::unbox("o", "JObject", "int"), std::string("(o).intValue()"));
    CB_CHECK_EQ(c3::unbox("o", "JObject", "const double&"), std::string("(o).doubleValue()"));
    CB_CHECK_EQ(c3::unbox("o", "int", "int"), std::string("o"));
    CB_CHECK_EQ(c3::unbox("o", "JObject", "JString"), std::string("o"));
    CB_CHECK_EQ(c3::zero_of("int"), std::string("0"));
    CB_CHECK_EQ(c3::zero_of("bool"), std::string("false"));
    CB_CHECK_EQ(c3::zero_of("Block*"), std::string("NULL"));
    CB_CHECK_EQ(c3::comment_safe("a */ b"), std::string("a * / b"));
    CB_CHECK(c3::is_pointer("Block* "));
    CB_CHECK(!c3::is_pointer("JavaString"));
    CB_CHECK_EQ(c3::array_element(" JArray<Block* > "), std::string("Block*"));
    CB_CHECK_EQ(c3::array_element("JArray<JArray<int > >"), std::string("JArray<int >"));
    CB_CHECK_THROWS_AS(c3::Decline, (void)c3::array_element("int"));
    CB_CHECK_EQ(c3::pointee_class("Block*").value_or("-"), std::string("Block"));
    CB_CHECK(!c3::pointee_class("JList<Block*>").has_value());
    CB_CHECK(!c3::pointee_class("Block**").has_value());
    const auto params = c3::split_params("int a, const JavaString& b, JMap<int, JavaString> m");
    CB_CHECK_EQ(params.size(), std::size_t(3));
    CB_CHECK_EQ(params[1].first, std::string("const JavaString&"));
    CB_CHECK_EQ(params[1].second, std::string("b"));
    CB_CHECK_EQ(params[2].first, std::string("JMap<int, JavaString>"));
    CB_CHECK(c3::split_params("").empty());
    CB_CHECK_EQ(c3::keep_null("x == NULL ? \"NULL\" : NULLABLE"), std::string("x == __null ? \"NULL\" : NULLABLE"));
});

// ---- what a tree says
CB_TEST("translate.cpp03.analysis", [] {
    cb::Arena arena;
    const ir::TypeP i32 = ir::i32_type();
    const auto sym = [&](const char* name) { return ir::make_sym(ir::SymKind::Local, name, std::nullopt, false, i32); };
    ir::Node* a = ir::make(arena, ir::Name{.sym = sym("a")}, i32);
    ir::Node* b = ir::make(arena, ir::Name{.sym = sym("b")}, i32);
    ir::Node* inc = ir::make(arena, ir::Unary{.op = "++", .x = ir::make(arena, ir::Name{.sym = sym("a")}, i32), .postfix = true}, i32);
    ir::Node* paren = ir::make(arena, ir::Paren{.x = a}, i32);
    CB_CHECK(c3::strip_paren(paren) == a);
    CB_CHECK(!c3::has_effects(a));
    CB_CHECK(c3::has_effects(inc));
    CB_CHECK(c3::names_written(inc).contains("a"));
    CB_CHECK(c3::names_read(a).contains("a"));
    // one operand writes what another reads: the order matters; unrelated ones do not
    const ir::Node* pair_ab[] = {inc, a};
    const ir::Node* pair_inc_b[] = {inc, b};
    CB_CHECK(c3::order_matters(pair_ab));
    CB_CHECK(!c3::order_matters(pair_inc_b));
    ir::Node* sum = ir::make(arena, ir::Binary{.op = "+", .left = a, .right = b}, i32);
    CB_CHECK_EQ(c3::operands(sum).size(), std::size_t(2));
    ir::Node* both = ir::make(arena, ir::Binary{.op = "&&", .left = a, .right = b}, ir::bool_type());
    CB_CHECK(c3::operands(both).empty());   // short-circuit: no fixed evaluation of both
    ir::Node* lit = ir::make(arena, ir::Lit{.value = std::int64_t(1)}, i32);
    CB_CHECK(c3::is_constant(lit));
    CB_CHECK(c3::is_constant(ir::make(arena, ir::Paren{.x = lit}, i32)));
    CB_CHECK(!c3::is_constant(a));
    // abrupt completion and jumps out
    ir::Node* ret = ir::make(arena, ir::Return{.expr = lit});
    ir::Node* brk = ir::make(arena, ir::Break{});
    ir::Node* loop = ir::make(arena, ir::While{.cond = lit, .body = brk});
    ir::Node* block = ir::make(arena, ir::Block{.stmts = {ir::make(arena, ir::Empty{}), ret}});
    CB_CHECK(c3::is_abrupt(block));
    CB_CHECK(!c3::is_abrupt(loop));
    const ir::Node* in_loop[] = {loop};
    const ir::Node* bare_break[] = {brk};
    const ir::Node* with_return[] = {block};
    CB_CHECK(!c3::leaves(in_loop));        // the break stays inside its loop
    CB_CHECK(c3::leaves(bare_break));      // a break out of nothing leaves
    CB_CHECK(c3::leaves(with_return));
    CB_CHECK(!c3::has_unsupported(block));
    CB_CHECK(c3::has_unsupported(ir::make(arena, ir::Block{.stmts = {ir::make(arena, ir::Unsupported{.kind = "X"})}})));
});

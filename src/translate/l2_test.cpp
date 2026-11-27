// Tests of L2 documents over the typed tree and of the module model. The documents are real: testdata/l2_python_unit.json
// is the Python front end's Unit for a small program, testdata/l2_java_units.json the Java front end's for a program of
// modern Java; both are what the producers wrote, and they must read, walk and write back as the same documents.
//
// Behaviour pinned here:
//   * a slot holding a value of the wrong kind (a string where a list is, a type with an unknown kind) is a FormatError at
//     the reader;
//   * a flag that is false, a list that is empty and a slot that is null are not written; a list slot the walk follows is always
//     written, empty or not, so no consumer meets null where it iterates;
//   * a key a node has no slot for is dropped on reading; Local's `at` of the schema has no slot in the typed tree and is dropped;
//   * a float that is not finite is written as its name and read back as that text;
//   * an integer above 2**64 is not kept (a JSON number that large is a float here).
#include "translate/l2.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <set>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/text.hpp"
#include "core/testing.hpp"

namespace {

namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using cb::Arena;
using cb::Json;

// A document without what carries no information: nulls, false flags and empty lists. (A producer may write them or not;
// the typed tree has one form for "absent".)
Json bare(const Json& j) {
    if (j.is_object()) {
        Json out = Json::object();
        for (const auto& [k, v] : j.items()) {
            if (v.is_null() || (v.is_boolean() && !v.get<bool>()) || (v.is_array() && v.empty())) continue;
            out[k] = bare(v);
        }
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(bare(v));
        return out;
    }
    return j;
}

Json parse(const char* text) { return cb::json::parse(text); }

void roundtrip_type(const char* text) {
    const Json doc = parse(text);
    const ir::TypeP t = l2::type_from_json(doc);
    CB_CHECK(t);
    CB_CHECK_EQ(l2::type_to_json(t).dump(), doc.dump());
}

}  // namespace

CB_TEST("translate.l2.types_roundtrip", [] {
    roundtrip_type(R"({"kind":"i32"})");
    roundtrip_type(R"({"kind":"char","signed":false})");
    roundtrip_type(R"({"kind":"char","enc":"utf16"})");
    roundtrip_type(R"({"kind":"void"})");
    roundtrip_type(R"({"kind":"null"})");
    roundtrip_type(R"({"kind":"dynamic"})");
    roundtrip_type(R"({"kind":"class","name":"java.util.List","args":[{"kind":"class","name":"java.lang.String"}],"nullable":true})");
    roundtrip_type(R"({"kind":"class","name":"P","record":true,"ref":false,"exception":false})");
    roundtrip_type(R"({"kind":"typevar","name":"T"})");
    roundtrip_type(R"({"kind":"wildcard","bound":"super","type":{"kind":"typevar","name":"T"}})");
    roundtrip_type(R"({"kind":"wildcard"})");
    roundtrip_type(R"({"kind":"array","elem":{"kind":"u8"},"count":16,"rank":2})");
    roundtrip_type(R"({"kind":"ptr","space":"near","to":{"kind":"i16"},"seg":"ds","ref":true})");
    roundtrip_type(R"({"kind":"ptr","space":"far","to":{"kind":"void"},"seg":{"module_segment":2}})");
    roundtrip_type(R"({"kind":"ptr","space":"near","to":{"kind":"u8"},"seg":{"import":"KERNEL.1","part":"offset"}})");
    roundtrip_type(R"({"kind":"selector","of":{"import":"USER.2","part":"segment"}})");
    roundtrip_type(R"({"kind":"selector"})");
    roundtrip_type(
        R"({"kind":"fn","conv":"pascal","params":[{"name":"hwnd","type":{"kind":"u16"},"at":{"stack":6}},{"type":{"kind":"i32"},"at":{"regs":["dx","ax"]}}],)"
        R"("result":{"type":{"kind":"i16"},"at":{"reg":"ax"}},"distance":"far","pops":8,"loadds":true,"preserves":["si","di"],"variadic":true})");
    roundtrip_type(R"({"kind":"fn","conv":"cdecl","params":[],"result":{"at":"none"}})");
    roundtrip_type(R"({"kind":"fn","conv":"register","params":[],"result":{"type":{"kind":"bool"},"at":{"flag":"CF"}}})");
    roundtrip_type(R"({"kind":"fnptr","space":"near","fn":{"kind":"fn","conv":"cdecl","params":[],"result":{"at":"none"}}})");
    roundtrip_type(
        R"({"kind":"record","size":8,"fields":[{"name":"a","offset":0,"type":{"kind":"u16"}},{"name":"_pad","offset":2,"type":{"kind":"array","elem":{"kind":"u8"},"count":6},"pad":true}],)"
        R"("name":"POINT","role":"struct","packed":true})");
});

CB_TEST("translate.l2.type_absent_and_errors", [] {
    CB_CHECK(!l2::type_from_json(Json(nullptr)));
    CB_CHECK(l2::type_to_json({}).is_null());
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"quantum"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"name":"x"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"class"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"ptr","space":"far"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"ptr","space":"sideways","to":{"kind":"i8"}})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"array","elem":{"kind":"i8"},"count":-1})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"("i32")")));
    // an array of rank 1 is the plain form
    CB_CHECK_EQ(l2::type_to_json(l2::type_from_json(parse(R"({"kind":"array","elem":{"kind":"i8"},"rank":1})"))).dump(),
                std::string(R"({"kind":"array","elem":{"kind":"i8"}})"));
});

CB_TEST("translate.l2.sym_roundtrip", [] {
    const Json doc = parse(
        R"({"kind":"method","name":"add","owner":"p.Calc","static":true,"type":{"kind":"i32"},"params":[{"kind":"i32"},{"kind":"i32"}],)"
        R"("ret":{"kind":"i32"},"varargs":true,"refs":["","out"],"flags":["extension"]})");
    const ir::SymP s = l2::sym_from_json(doc);
    CB_CHECK(s);
    CB_CHECK(s->kind == ir::SymKind::Method);
    CB_CHECK(s->is_static && s->varargs);
    CB_CHECK_EQ(s->owner.value_or(""), std::string("p.Calc"));
    CB_CHECK_EQ(s->params.size(), std::size_t(2));
    CB_CHECK(s->refs[1] == ir::PassMode::Out);
    CB_CHECK_EQ(l2::sym_to_json(s).dump(), doc.dump());
    // a method with no parameters still says so; a local does not
    CB_CHECK_EQ(l2::sym_to_json(ir::make_sym(ir::SymKind::Method, "f")).dump(), std::string(R"({"kind":"method","name":"f","params":[]})"));
    CB_CHECK_EQ(l2::sym_to_json(ir::make_sym(ir::SymKind::Local, "x")).dump(), std::string(R"({"kind":"local","name":"x"})"));
    // refs that are all by value are not written
    ir::SymP v = ir::make_sym(ir::SymKind::Function, "g");
    v->refs = {ir::PassMode::ByValue};
    CB_CHECK(!l2::sym_to_json(v).contains("refs"));
    CB_CHECK(l2::sym_to_json({}).is_null());
    CB_CHECK(!l2::sym_from_json(Json(nullptr)));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::sym_from_json(parse(R"({"kind":"banana","name":"x"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::sym_from_json(parse(R"({"kind":"local"})")));
});

CB_TEST("translate.l2.a_small_tree", [] {
    // `return a + 1;` with line and column, typed, and a symbol shared by the declaration's use
    const Json doc = parse(
        R"({"node":"Return","line":3,"col":5,"expr":{"node":"Binary","line":3,"col":12,"type":{"kind":"i32"},"op":"+",)"
        R"("left":{"node":"Name","type":{"kind":"i32"},"sym":{"kind":"param","name":"a","type":{"kind":"i32"}}},)"
        R"("right":{"node":"Lit","type":{"kind":"i32"},"value":1,"text":"1"}}})");
    Arena arena;
    ir::Node* root = l2::from_json(arena, doc);
    const auto* ret = ir::get<ir::Return>(root);
    CB_CHECK(ret != nullptr);
    CB_CHECK_EQ(root->line, 3u);
    CB_CHECK_EQ(root->col, 5u);
    const auto* sum = ir::get<ir::Binary>(ret->expr);
    CB_CHECK(sum != nullptr);
    CB_CHECK_EQ(sum->op, std::string("+"));
    CB_CHECK_EQ(ret->expr->line, 3u);
    CB_CHECK(ret->expr->type == ir::i32_type());
    const auto* name = ir::get<ir::Name>(sum->left);
    CB_CHECK(name != nullptr && name->sym && name->sym->name == "a");
    const auto* lit = ir::get<ir::Lit>(sum->right);
    CB_CHECK(lit != nullptr && std::get<std::int64_t>(lit->value) == 1);
    CB_CHECK_EQ(ir::walk(root).size(), std::size_t(4));
    CB_CHECK_EQ(l2::to_json(*root).dump(), doc.dump());
});

// A NewArray's initialiser is absent for `new int[8]` and present, empty, for `new int[]{}`: a consumer (the C++17 back end's library)
// reads the difference, so the document keeps it.
CB_TEST("translate.l2.new_array_initialiser", [] {
    Arena arena;
    const Json sized = parse(R"({"node":"NewArray","type":{"kind":"array","elem":{"kind":"i32"}},"elem":{"kind":"i32"},)"
                             R"("dims":[{"node":"Lit","type":{"kind":"i32"},"value":8,"text":"8"}]})");
    const Json written_sized = l2::to_json(*l2::from_json(arena, sized));
    CB_CHECK(!written_sized.contains("init"));
    CB_CHECK_EQ(written_sized["dims"].size(), std::size_t(1));
    const Json empty = parse(R"({"node":"NewArray","type":{"kind":"array","elem":{"kind":"i32"}},"elem":{"kind":"i32"},"init":[]})");
    const Json written_empty = l2::to_json(*l2::from_json(arena, empty));
    CB_CHECK(written_empty.contains("init"));
    CB_CHECK(written_empty["init"].is_array() && written_empty["init"].empty());
    const Json filled = parse(R"({"node":"NewArray","type":{"kind":"array","elem":{"kind":"i32"}},"elem":{"kind":"i32"},)"
                              R"("init":[{"node":"Lit","type":{"kind":"i32"},"value":1,"text":"1"}]})");
    CB_CHECK_EQ(l2::to_json(*l2::from_json(arena, filled))["init"].size(), std::size_t(1));
});

CB_TEST("translate.l2.literal_values", [] {
    Arena arena;
    const auto lit = [&](const char* value) {
        const Json doc = parse((std::string(R"({"node":"Lit","value":)") + value + "}").c_str());
        return l2::from_json(arena, doc);
    };
    CB_CHECK(std::holds_alternative<std::monostate>(ir::get<ir::Lit>(lit("null"))->value));
    CB_CHECK(std::get<bool>(ir::get<ir::Lit>(lit("true"))->value));
    CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(lit("-7"))->value), std::int64_t(-7));
    CB_CHECK_EQ(std::get<std::int64_t>(ir::get<ir::Lit>(lit("9223372036854775807"))->value), INT64_MAX);
    CB_CHECK_EQ(std::get<std::uint64_t>(ir::get<ir::Lit>(lit("18446744073709551615"))->value), UINT64_MAX);
    CB_CHECK_EQ(std::get<double>(ir::get<ir::Lit>(lit("2.5"))->value), 2.5);
    CB_CHECK_EQ(std::get<std::string>(ir::get<ir::Lit>(lit(R"("hi\n")"))->value), std::string("hi\n"));
    for (const char* v : {"-7", "18446744073709551615", "2.5", R"("x")", "true"}) {
        CB_CHECK_EQ(l2::to_json(*lit(v)).at("value").dump(), std::string(v));
    }
    // a value that is no literal
    CB_CHECK_THROWS_AS(cb::FormatError, lit("[1]"));
    // a float that is not finite is its name, as JSON has no form for it
    ir::Lit nan;
    nan.value = std::numeric_limits<double>::quiet_NaN();
    CB_CHECK_EQ(l2::to_json(*ir::make(arena, nan)).at("value").get<std::string>(), std::string("nan"));
    ir::Lit inf;
    inf.value = -std::numeric_limits<double>::infinity();
    CB_CHECK_EQ(l2::to_json(*ir::make(arena, inf)).at("value").get<std::string>(), std::string("-inf"));
});

CB_TEST("translate.l2.slots_that_hold_lists_or_nodes", [] {
    Arena arena;
    // a statement body as a bare list, and as one statement; a Block with no statements still writes its list
    const Json doc = parse(
        R"({"node":"If","cond":{"node":"Name","sym":{"kind":"local","name":"c"}},"then":[{"node":"Break"},{"node":"Continue"}],)"
        R"("orelse":{"node":"Block","stmts":[]}})");
    ir::Node* n = l2::from_json(arena, doc);
    const auto* branch = ir::get<ir::If>(n);
    CB_CHECK(branch != nullptr);
    CB_CHECK_EQ(ir::statements(branch->then).size(), std::size_t(2));
    CB_CHECK(std::holds_alternative<std::vector<ir::Node*>>(branch->then));
    CB_CHECK(ir::is<ir::Block>(std::get<ir::Node*>(branch->orelse)));
    CB_CHECK_EQ(l2::to_json(*n).dump(), doc.dump());
    // an If with nothing in its branches has neither slot
    ir::Node* bare_if = ir::make_node(arena, "If");
    CB_CHECK_EQ(l2::to_json(*bare_if).dump(), std::string(R"({"node":"If"})"));
    // a node slot given a list of statements gets one block of them
    ir::Node* lam = l2::from_json(arena, parse(R"({"node":"Try","body":[{"node":"Break"}]})"));
    const auto* t = ir::get<ir::Try>(lam);
    CB_CHECK(ir::is<ir::Block>(t->body));
    CB_CHECK_EQ(ir::get<ir::Block>(t->body)->stmts.size(), std::size_t(1));
});

CB_TEST("translate.l2.names_of_slots", [] {
    // the slots whose document name is not the member's: default, final, signed, asm, std, long
    Arena arena;
    const Json doc = parse(
        R"({"node":"Try","body":{"node":"Block","stmts":[]},"catches":[],"final":{"node":"Block","stmts":[]},"resources":[]})");
    ir::Node* n = l2::from_json(arena, doc);
    CB_CHECK(ir::get<ir::Try>(n)->finally_body != nullptr);
    CB_CHECK(bare(l2::to_json(*n)) == bare(doc));
    ir::Node* c = l2::from_json(arena, parse(R"({"node":"Case","default":true,"labels":[],"body":[]})"));
    CB_CHECK(ir::get<ir::Case>(c)->is_default);
    CB_CHECK_EQ(l2::to_json(*c).at("default").get<bool>(), true);
    ir::Node* b = l2::from_json(arena, parse(R"({"node":"Binary","op":"<","signed":true})"));
    CB_CHECK(ir::get<ir::Binary>(b)->is_signed.value_or(false));
    ir::Node* f = l2::from_json(arena, parse(R"({"node":"MachineFragment","asm":"ret","length":1,"within":"f"})"));
    CB_CHECK_EQ(ir::get<ir::MachineFragment>(f)->assembly.value_or(""), std::string("ret"));
    CB_CHECK_EQ(ir::get<ir::MachineFragment>(f)->length.value_or(0), std::uint64_t(1));
    ir::Node* u = l2::from_json(arena, parse(R"({"node":"Unit","target":{"triple":"x86_64","long":4,"std":"c11","char_signed":true}})"));
    const auto& target = ir::get<ir::Unit>(u)->target;
    CB_CHECK(target && target->long_bits == 4u && target->standard == "c11" && target->char_signed == true);
    CB_CHECK_EQ(l2::to_json(*u).at("target").at("std").get<std::string>(), std::string("c11"));
});

CB_TEST("translate.l2.origins", [] {
    Arena arena;
    for (const char* text : {R"({"module":"lz32.dll","segment":2,"offset":16})", R"({"module":"lz32.dll","va":4096})", R"({"at":[4096,4099]})"}) {
        const Json doc = parse((std::string(R"({"node":"Empty","origin":)") + text + "}").c_str());
        ir::Node* n = l2::from_json(arena, doc);
        CB_CHECK(n->origin != nullptr);
        CB_CHECK_EQ(l2::to_json(*n).dump(), doc.dump());
    }
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","origin":{"where":1}})")));
});

CB_TEST("translate.l2.errors", [] {
    Arena arena;
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"line":1})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Flux"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":7})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"([1,2])")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Block","stmts":[3]})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Name","sym":{"kind":"local"}})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Binary","overflow":"explode"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"MethodDecl","mods":"public"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","line":-1})")));
    // a key a node does not have is ignored, as a newer producer may write more
    CB_CHECK_NOTHROW(l2::from_json(arena, parse(R"({"node":"Empty","comment":"later"})")));
});

// The producers' own documents: read, walk, write back.
namespace {
void check_real(const char* relative, bool list) {
    const Json doc = cb::json::load(cb::testing::data_path(relative));
    Arena arena;
    const Json& unit = list ? doc.at(0) : doc;
    ir::Node* root = l2::from_json(arena, unit);
    CB_CHECK(ir::is<ir::Unit>(root));
    CB_CHECK(ir::walk(root).size() > 100);
    CB_CHECK(ir::unsupported(root).empty());
    const Json back = l2::to_json(*root);
    CB_CHECK(bare(back) == bare(unit));
    // writing and reading again changes nothing
    Arena arena2;
    CB_CHECK(l2::to_json(*l2::from_json(arena2, back)) == back);
}
}  // namespace

CB_TEST("translate.l2.python_front_end_unit", [] { check_real("translate/testdata/l2_python_unit.json", false); });
CB_TEST("translate.l2.java_front_end_units", [] { check_real("translate/testdata/l2_java_units.json", true); });

// ---- the module model -------------------------------------------------------------------------------------------------

CB_TEST("translate.l2.module_model", [] {
    Arena arena;
    ir::Node* f = l2::function_node(arena, {.name = "sub_1000", .text = "int sub_1000(void) { return 1; }",
                                            .origin = ir::OriginVa{"lz32.dll", 0x1000}, .comments = {"warning: guessed"}});
    ir::Node* g = l2::function_node(arena, {.name = "sub_2000"});
    ir::MachineFragment frag;
    frag.name = "start";
    frag.assembly = "mov ax, 1";
    frag.syntax = "masm";
    ir::Node* a = l2::fragment_node(arena, frag, ir::OriginSegment{"x.exe", 1, 0});
    ir::Node* c = l2::comment_node(arena, "status");
    const std::vector<ir::Node*> parts = {f, g, a, c};
    ir::Node* m = l2::module_node(arena, parts, "typedef int I;\n");
    CB_CHECK_EQ(l2::units(m).size(), std::size_t(4));
    CB_CHECK_EQ(l2::units_of<ir::MethodDecl>(m).size(), std::size_t(2));
    CB_CHECK_EQ(l2::units_of<ir::Comment>(m).size(), std::size_t(1));
    CB_CHECK(l2::units(f).empty());   // not a Module
    CB_CHECK(l2::units(nullptr).empty());
    // only the function with text is in texts
    const auto texts = l2::texts(m);
    CB_CHECK_EQ(texts.size(), std::size_t(1));
    CB_CHECK_EQ(texts.at("sub_1000"), std::string("int sub_1000(void) { return 1; }"));
    // the document form
    const Json doc = l2::to_json(*m);
    CB_CHECK_EQ(doc.at("node").get<std::string>(), std::string("Module"));
    CB_CHECK_EQ(doc.at("header").get<std::string>(), std::string("typedef int I;\n"));
    CB_CHECK_EQ(doc.at("units").size(), std::size_t(4));
    const Json& first = doc.at("units").at(0);
    CB_CHECK_EQ(first.at("kind").get<std::string>(), std::string("function"));
    CB_CHECK_EQ(first.at("name").get<std::string>(), std::string("sub_1000"));
    CB_CHECK_EQ(first.at("origin").dump(), std::string(R"({"module":"lz32.dll","va":4096})"));
    CB_CHECK_EQ(first.at("comments").dump(), std::string(R"(["warning: guessed"])"));
    CB_CHECK_EQ(doc.at("units").at(2).at("asm").get<std::string>(), std::string("mov ax, 1"));
    Arena back;
    CB_CHECK(l2::to_json(*l2::from_json(back, doc)) == doc);
});

CB_TEST("translate.l2.made", [] {
    cb::OrderedMap<std::string, std::string> texts;
    texts.insert_or_assign("b", "B text");
    texts.insert_or_assign("a", "A text");
    texts.insert_or_assign("z", "Z text");
    cb::OrderedMap<std::uint64_t, std::string> by_va;
    by_va.insert_or_assign(0x2000, "a");
    by_va.insert_or_assign(0x1000, "b");
    const std::vector<std::string> order = {"a", "b", "missing"};
    l2::Made m = l2::Made::of("lzexpand.dll", "/* header */", texts, order, by_va);
    // `order` first (a name with no text has none), then the texts that order does not name
    CB_CHECK((m.order() == std::vector<std::string>{"a", "b", "missing", "z"}));
    CB_CHECK_EQ(m.texts().size(), std::size_t(3));
    CB_CHECK_EQ(m.texts().begin()->first, std::string("a"));
    const auto places = m.by_va();
    CB_CHECK_EQ(places.size(), std::size_t(2));
    CB_CHECK_EQ(places.at(0x2000), std::string("a"));
    const auto funcs = m.functions();
    CB_CHECK_EQ(funcs.at(0x1000).first, std::string("b"));
    CB_CHECK_EQ(funcs.at(0x1000).second.value_or(""), std::string("B text"));
    // set_text keeps a function's place; a new name goes last
    m.set_text("a", "A2");
    m.set_text("fresh", "F");
    CB_CHECK_EQ(m.texts().at("a"), std::string("A2"));
    CB_CHECK_EQ(m.order().back(), std::string("fresh"));
    CB_CHECK_EQ(m.order().size(), std::size_t(5));
    cb::OrderedMap<std::string, std::string> more;
    more.insert_or_assign("missing", "now has text");
    more.insert_or_assign("z", "Z2");
    m.update_texts(more);
    CB_CHECK_EQ(m.texts().at("missing"), std::string("now has text"));
    CB_CHECK_EQ(m.order().size(), std::size_t(5));
    m.add("late", "L", 0x3000);
    CB_CHECK_EQ(m.by_va().at(0x3000), std::string("late"));
    m.set_header("h2");
    CB_CHECK_EQ(m.header(), std::string("h2"));
    // the tree: each function a MethodDecl with its place, the header the Module's
    Arena arena;
    ir::Node* doc = m.document(arena);
    CB_CHECK_EQ(l2::units_of<ir::MethodDecl>(doc).size(), std::size_t(6));
    CB_CHECK_EQ(l2::texts(doc).size(), std::size_t(m.texts().size()));
    CB_CHECK_EQ(ir::get<ir::Module>(doc)->header.value_or(""), std::string("h2"));
    const auto* a = ir::get<ir::MethodDecl>(l2::units(doc)[0]);
    CB_CHECK_EQ(a->name.value_or(""), std::string("a"));
    CB_CHECK(l2::units(doc)[0]->origin != nullptr);
    CB_CHECK(l2::units(doc)[2]->origin == nullptr);   // "missing" has no place
    // with no order: the order of the texts
    l2::Made plain = l2::Made::of("m", "", texts);
    CB_CHECK((plain.order() == std::vector<std::string>{"b", "a", "z"}));
    // a Made is a value
    l2::Made copy = plain;
    copy.set_text("b", "changed");
    CB_CHECK_EQ(plain.texts().at("b"), std::string("B text"));
});

CB_TEST("translate.l2.assembly", [] {
    // the lines before the first part are the head's; a part with no lines is left out; the fragments joined by a line
    // break are the lines joined, as they were written
    l2::Assembly out({"; head", "bits 16"}, "nasm", "x.dll");
    out.part("seg1", "segment", ir::OriginSegment{"x.dll", 1, 0}, 4);
    out.append("");
    const std::vector<std::string> more = {"section seg1", "    db 1,2,3,4"};
    out.extend(more);
    out.part("empty");
    out.part("tail", "end");
    out.append("");
    out.append("; end");
    CB_CHECK_EQ(out.lines().size(), std::size_t(7));
    Arena arena;
    const Json doc = l2::to_json(*out.document(arena));
    const Json& units = doc.at("units");
    CB_CHECK_EQ(units.size(), std::size_t(3));
    CB_CHECK_EQ(units.at(0).at("name").get<std::string>(), std::string("head"));
    CB_CHECK_EQ(units.at(0).at("reason").get<std::string>(), std::string("head"));
    CB_CHECK_EQ(units.at(1).at("name").get<std::string>(), std::string("seg1"));
    CB_CHECK_EQ(units.at(1).at("origin").dump(), std::string(R"({"module":"x.dll","segment":1,"offset":0})"));
    CB_CHECK_EQ(units.at(1).at("length").get<int>(), 4);
    CB_CHECK_EQ(units.at(2).at("name").get<std::string>(), std::string("tail"));
    std::string joined;
    for (const Json& u : units) joined += (joined.empty() ? "" : "\n") + u.at("asm").get<std::string>();
    CB_CHECK_EQ(joined, std::string("; head\nbits 16\n\nsection seg1\n    db 1,2,3,4\n\n; end"));
    for (const Json& u : units) CB_CHECK_EQ(u.at("syntax").get<std::string>(), std::string("nasm"));
    // nothing written: no units
    Arena empty_arena;
    CB_CHECK(l2::units(l2::Assembly{}.document(empty_arena)).empty());
});

// ---- the decompiler's documents ---------------------------------------------------------------------------------------

namespace {
// The first place two documents differ, as a path ("body.stmts[3].type.kind: 5 against 6"); empty when they are the same.
std::string first_difference(const Json& a, const Json& b, const std::string& at = "") {
    if (a.is_number_integer() && b.is_number_integer()) {   // (a number written signed or unsigned is the same number)
        const bool same = a.is_number_unsigned() || b.is_number_unsigned() ? (a.dump() == b.dump()) : a.get<std::int64_t>() == b.get<std::int64_t>();
        return same ? "" : at + ": " + a.dump() + " against " + b.dump();
    }
    if (a.type() != b.type()) return at + ": " + a.dump().substr(0, 80) + " against " + b.dump().substr(0, 80);
    if (a.is_object()) {
        for (const auto& [k, v] : a.items()) {
            if (!b.contains(k)) return at + "." + k + ": lost";
            if (std::string d = first_difference(v, b.at(k), at + "." + k); !d.empty()) return d;
        }
        for (const auto& [k, v] : b.items())
            if (!a.contains(k)) return at + "." + k + ": added";
        return "";
    }
    if (a.is_array()) {
        if (a.size() != b.size()) return at + ": " + std::to_string(a.size()) + " items against " + std::to_string(b.size());
        for (std::size_t i = 0; i < a.size(); ++i)
            if (std::string d = first_difference(a[i], b[i], at + "[" + std::to_string(i) + "]"); !d.empty()) return d;
        return "";
    }
    return a == b ? "" : at + ": " + a.dump().substr(0, 80) + " against " + b.dump().substr(0, 80);
}
}  // namespace

// The decompiler's own documents (lz32.dll's 93 functions and the 16-bit lzexpand.dll's 46, as the engine writes them with CB_L2=1) read into the typed tree and
// written back lose nothing: the typed tree can stand for the document wherever the document is used (docs/ENGINE_UNIFICATION.md
// item 1, step 1).
CB_TEST("translate.l2.decompiler_documents_lose_nothing", [] {
    const std::string all = cb::fs::read_text(cb::testing::data_path("translate/testdata/l2_decompiler_lz32.jsonl")) +
                            cb::fs::read_text(cb::testing::data_path("translate/testdata/l2_decompiler_lzexpand.jsonl"));
    std::size_t docs = 0, same = 0;
    std::map<std::string, std::size_t> where;   // {the first difference's path, without its indexes: how many}
    for (const std::string_view line : cb::text::lines(all)) {
        if (cb::text::trim(line).empty()) continue;
        ++docs;
        const Json doc = cb::json::parse(line);
        Arena arena;
        const Json back = l2::to_json(*l2::from_json(arena, doc));
        const std::string d = first_difference(bare(doc), bare(back));
        if (d.empty()) {
            ++same;
            continue;
        }
        std::string key;   // (the path with its indexes left out: "body.stmts[].x")
        bool in_index = false;
        for (const char c : d.substr(0, d.find(':'))) {
            if (c == '[') in_index = true;
            if (!in_index) key += c;
            if (c == ']') {
                in_index = false;
                key += "[]";
            }
        }
        ++where[key + d.substr(d.find(':'), 60)];
    }
    for (const auto& [k, n] : where) std::cout << "  " << n << " x " << k << "\n";
    CB_CHECK_EQ(docs, 93u + 46u);
    CB_CHECK_EQ(same, docs);
});

// A node's slots by their document names: found and typed, changed through the pointer (the document then says the change), the
// expression's type among them, the names in the document's order, and nothing for a name the kind does not have.
CB_TEST("translate.l2.slots_by_name", [] {
    Arena arena;
    ir::Node* n = l2::from_json(arena, parse(R"({"node":"Binary","op":"+","type":{"kind":"i32"},"left":{"node":"Name","sym":{"kind":"local","name":"a"},"type":{"kind":"i32"}},"right":{"node":"Lit","value":2,"type":{"kind":"i32"}}})"));
    const l2::SlotPtr left_slot = l2::slot(*n, "left");
    ir::Node** const* left = std::get_if<ir::Node**>(&left_slot);
    CB_CHECK(left != nullptr && **left != nullptr);
    CB_CHECK(ir::is<ir::Name>(**left));
    CB_CHECK(!std::holds_alternative<std::monostate>(l2::slot(*n, "op")));
    const l2::SlotPtr type = l2::slot(*n, "type");
    CB_CHECK(std::holds_alternative<ir::TypeP*>(type));
    CB_CHECK(std::holds_alternative<std::monostate>(l2::slot(*n, "nothing")));
    // a change through the slot is the tree's own: the document says it
    const l2::SlotPtr right = l2::slot(*n, "right");
    *std::get<ir::Node**>(right) = **left;
    CB_CHECK_EQ(l2::to_json(*n).at("right").at("node").get<std::string>(), std::string("Name"));
    const std::vector<std::string_view> keys = l2::slot_keys(*n);
    CB_CHECK(!keys.empty() && keys.front() == "type");
    CB_CHECK(std::find(keys.begin(), keys.end(), std::string_view("left")) != keys.end());
    // every slot a document of the decompiler's writes is found by its name
    const std::string all = cb::fs::read_text(cb::testing::data_path("translate/testdata/l2_decompiler_lz32.jsonl"));
    std::size_t looked = 0;
    for (const std::string_view line : cb::text::lines(all)) {
        if (cb::text::trim(line).empty()) continue;
        Arena a;
        for (ir::Node* x : ir::walk(l2::from_json(a, cb::json::parse(line)))) {
            const Json written = l2::to_json(*x);   // (held: items() views it)
            for (const auto& [k, v] : written.items()) {
                if (k == "node" || k == "line" || k == "col" || k == "origin" || k == "stk" || k == "fl" || k == "info") continue;   // (the node's own, not slots)
                ++looked;
                CB_CHECK(!std::holds_alternative<std::monostate>(l2::slot(*x, k)));
            }
        }
        if (looked > 5000) break;   // (enough documents to cover the kinds and slots they use)
    }
    CB_CHECK(looked > 50);
});

// ---- what a lowered bytecode states, in the document (docs: work/ilspy-port/l2/ILAST_TO_L2.md) -----------------------------------
//
// A tree that carries stack kinds, prefix flags, info, entities and variables writes a document, reads back as the same tree, and writes
// the same document: byte for byte. Everything the lane may state is in the schema; what is not is refused.

namespace {

class L2TestEntity : public ir::Entity {
public:
    L2TestEntity(ir::EntityKind kind, std::string key) : kind_(kind), key_(std::move(key)) {}
    std::string_view family() const override { return "test"; }
    ir::EntityKind kind() const override { return kind_; }
    std::string key() const override { return key_; }

private:
    ir::EntityKind kind_;
    std::string key_;
};

ir::EntityP entity_of(ir::EntityKind kind, std::string key) { return std::make_shared<const L2TestEntity>(kind, std::move(key)); }

// The scope of the "test" family: a key is "T:<name>", "M:<name>" or "F:<name>"; it makes the entity and counts what it was asked.
class L2TestScope : public ir::EntityScope {
public:
    std::string_view family() const override { return "test"; }
    ir::EntityP resolve(std::string_view key) const override {
        ++asked;
        if (key.size() < 3 || key[1] != ':' || key.substr(2).starts_with("missing")) return nullptr;
        const ir::EntityKind kind = key[0] == 'T' ? ir::EntityKind::Type : key[0] == 'M' ? ir::EntityKind::Method : ir::EntityKind::Field;
        return entity_of(kind, std::string(key));
    }
    mutable int asked = 0;
};

// A method with a variable, a parameter and everything the lane may state, built the way the lowering will.
struct CilMethod {
    Arena arena;
    ir::SymP num = ir::make_sym(ir::SymKind::Local, "num", std::nullopt, false, ir::i32_type());
    ir::SymP arg = ir::make_sym(ir::SymKind::Param, "arg", std::nullopt, false, ir::i32_type());
    ir::SymP callee = ir::make_sym(ir::SymKind::Method, "Add", "N.C", true);
    ir::Node* root = nullptr;

    CilMethod() {
        num->role = ir::VarRole::Temp;
        num->index = 0;
        num->stack = ir::Stack::I4;
        num->generated_name = true;
        num->entry_initialized = true;
        num->entry_used = true;
        num->info.set("cil.capture_scope", std::string("IL_0000"));
        num->info.set("cil.kind_exact", std::string("StackSlot"));
        num->info.set("cil.remove_if_redundant", true);
        arg->role = ir::VarRole::Param;
        arg->index = 1;
        arg->stack = ir::Stack::I4;
        arg->ref_readonly = true;
        callee->entity = entity_of(ir::EntityKind::Method, "M:N.C.Add");
        callee->params = {ir::i32_type()};
        callee->ret = ir::i32_type();
        callee->info.set("cil.state_machine_field", entity_of(ir::EntityKind::Field, "F:N.C.<>1__state"));

        auto expr = [&](ir::Node* n, ir::Stack s) {
            n->stack = s;
            return n;
        };
        ir::Node* ld = expr(ir::make(arena, ir::Name{.sym = num}, ir::i32_type()), ir::Stack::I4);
        ld->origin = std::make_shared<const ir::Origin>(ir::OriginSpan{2, 4});
        ir::set_flag(*ld, ir::NodeFlag::StackEmpty);
        ir::Node* ld_arg = expr(ir::make(arena, ir::Name{.sym = arg}, ir::i32_type()), ir::Stack::I4);
        ir::Binary add{.op = "+", .left = ld, .right = ld_arg, .overflow = ir::Overflow::Trap, .is_signed = false};
        add.left_stk = ir::Stack::I4;
        add.right_stk = ir::Stack::I4;
        add.lift = ir::Lift::CSharp;
        ir::Node* sum = expr(ir::make(arena, std::move(add), ir::i32_type()), ir::Stack::I4);
        ir::Call call;
        call.sym = callee;
        call.args = {sum};
        call.form = ir::CallForm::Direct;
        ir::Node* c = expr(ir::make(arena, std::move(call), ir::i32_type()), ir::Stack::I4);
        ir::set_flag(*c, ir::NodeFlag::Tail);
        c->info.set("cil.constrained", ir::class_type("N.C"));
        ir::Assign asg;
        asg.op = "+";
        asg.target = ir::make(arena, ir::Name{.sym = num}, ir::i32_type());
        asg.value = c;
        asg.eval = ir::Eval::Old;
        asg.via = ir::Via::Address;
        asg.is_signed = true;
        asg.left_stk = ir::Stack::I4;
        asg.right_stk = ir::Stack::I4;
        ir::Node* assign = ir::make(arena, std::move(asg), ir::i32_type());
        ir::Node* stmt = ir::make(arena, ir::ExprStmt{.expr = assign});
        ir::MethodDecl m;
        m.name = "M";
        m.kind = "method";
        m.params = {ir::make(arena, ir::Param{.sym = arg, .type = ir::i32_type()})};
        m.body = ir::make(arena, ir::Block{.stmts = {stmt}});
        m.locals = {arg, num};
        m.captures = {num};
        m.scope = "IL_0000";
        root = ir::make(arena, std::move(m));
        root->info.set("cil.code_size", std::int64_t{42});
        root->info.set("cil.seq_points", std::vector<std::int64_t>{0, 2, 4});
        root->info.set("cil.move_next", entity_of(ir::EntityKind::Method, "M:N.C.MoveNext"));
        root->info.set("cil.async_return_type", ir::make_type(ir::ClassType{.name = "System.Int32"}, entity_of(ir::EntityKind::Type, "T:System.Int32")));
    }
};

// Writes, reads back (with the scope) and writes again.
ir::Node* again(Arena& into, const ir::Node& root, const std::shared_ptr<const ir::EntityScope>& scope, Json* first = nullptr) {
    const Json doc = l2::to_json(root);
    if (first != nullptr) *first = doc;
    ir::Node* back = l2::from_json(into, doc, scope);
    CB_CHECK_EQ(l2::to_json(*back).dump(), doc.dump());
    return back;
}

}  // namespace

CB_TEST("translate.l2.cil.stack_flags_info_entities_and_variables_round_trip", [] {
    CilMethod m;
    const auto scope = std::make_shared<L2TestScope>();
    Arena arena;
    Json doc;
    ir::Node* back = again(arena, *m.root, scope, &doc);
    CB_CHECK_EQ(scope->asked, 4);   // each entity resolved once: M:N.C.Add, F:N.C.<>1__state, M:N.C.MoveNext, T:System.Int32
    // the tables: each variable and entity once, a use by its index
    CB_CHECK_EQ(doc.at("vars").size(), std::size_t{3});    // arg, num, callee
    CB_CHECK_EQ(doc.at("entities").size(), std::size_t{4});
    CB_CHECK(doc.at("entities").at(0).at("family") == "test");
    // the method
    auto* md = ir::get<ir::MethodDecl>(back);
    CB_CHECK(md != nullptr);
    CB_CHECK_EQ(md->locals.size(), std::size_t{2});
    CB_CHECK_EQ(md->captures.size(), std::size_t{1});
    CB_CHECK(md->captures[0] == md->locals[1]);   // the variable is one object across the document
    CB_CHECK_EQ(*md->scope, std::string("IL_0000"));
    CB_CHECK_EQ(*back->info.get_as<std::int64_t>("cil.code_size"), std::int64_t{42});
    CB_CHECK((*back->info.get_as<std::vector<std::int64_t>>("cil.seq_points") == std::vector<std::int64_t>{0, 2, 4}));
    CB_CHECK(ir::same_entity(*back->info.get_as<ir::EntityP>("cil.move_next"), entity_of(ir::EntityKind::Method, "M:N.C.MoveNext")));
    const ir::TypeP ret = *back->info.get_as<ir::TypeP>("cil.async_return_type");
    CB_CHECK(ret == ir::class_type("System.Int32") && ret->entity != nullptr);
    // the variable: its role, index, stack kind and facts; used by the parameter list and by the names in the body
    const ir::SymP num = md->locals[1];
    CB_CHECK(num->role == ir::VarRole::Temp && num->index == 0 && num->stack == ir::Stack::I4);
    CB_CHECK(num->generated_name && num->entry_initialized && num->entry_used && !num->ref_readonly);
    CB_CHECK_EQ(*num->info.get_as<std::string>("cil.capture_scope"), std::string("IL_0000"));
    CB_CHECK_EQ(*num->info.get_as<std::string>("cil.kind_exact"), std::string("StackSlot"));
    CB_CHECK(md->locals[0]->ref_readonly && md->locals[0]->role == ir::VarRole::Param);
    std::size_t names = 0;
    for (ir::Node* n : ir::walk(back)) {
        if (auto* name = ir::get<ir::Name>(n)) {
            ++names;
            CB_CHECK(name->sym == md->locals[0] || name->sym == num);   // not a copy
        }
        if (auto* p = ir::get<ir::Param>(n)) CB_CHECK(p->sym == md->locals[0]);
    }
    CB_CHECK_EQ(names, std::size_t{3});
    // the nodes: stack kind, flags, the new slots
    for (ir::Node* n : ir::walk(back)) {
        if (auto* b = ir::get<ir::Binary>(n)) {
            CB_CHECK(n->stack == ir::Stack::I4);
            CB_CHECK(b->left_stk == ir::Stack::I4 && b->right_stk == ir::Stack::I4 && b->lift == ir::Lift::CSharp);
            CB_CHECK(b->overflow == ir::Overflow::Trap && b->is_signed == false);
        } else if (auto* c = ir::get<ir::Call>(n)) {
            CB_CHECK(c->form == ir::CallForm::Direct && ir::has_flag(*n, ir::NodeFlag::Tail) && !ir::has_flag(*n, ir::NodeFlag::Volatile));
            CB_CHECK(c->sym->entity != nullptr && ir::same_entity(c->sym->entity, entity_of(ir::EntityKind::Method, "M:N.C.Add")));
            CB_CHECK(c->sym->info.get_as<ir::EntityP>("cil.state_machine_field") != nullptr);
            CB_CHECK(n->info.get_as<ir::TypeP>("cil.constrained") != nullptr);
        } else if (auto* a = ir::get<ir::Assign>(n)) {
            CB_CHECK(a->eval == ir::Eval::Old && a->via == ir::Via::Address && a->is_signed == true && a->op == "+");
        } else if (ir::is<ir::Name>(n) && n->origin) {
            CB_CHECK((std::get<ir::OriginSpan>(*n->origin) == ir::OriginSpan{2, 4}));
            CB_CHECK(ir::has_flag(*n, ir::NodeFlag::StackEmpty));
        }
    }
});

// A symbol that is neither a variable nor an entity's is written whole at every use, as it always was: the existing documents are unchanged.
CB_TEST("translate.l2.cil.an_ordinary_symbol_is_still_written_whole_at_each_use", [] {
    Arena arena;
    ir::SymP s = ir::make_sym(ir::SymKind::Local, "a", std::nullopt, false, ir::i32_type());
    ir::Node* block = ir::make(arena, ir::Block{.stmts = {ir::make(arena, ir::ExprStmt{.expr = ir::make(arena, ir::Name{.sym = s}, ir::i32_type())}),
                                                         ir::make(arena, ir::ExprStmt{.expr = ir::make(arena, ir::Name{.sym = s}, ir::i32_type())})}});
    const Json doc = l2::to_json(*block);
    CB_CHECK(!doc.contains("vars") && !doc.contains("entities") && !doc.contains("stk") && !doc.contains("fl") && !doc.contains("info"));
    CB_CHECK_EQ(doc.at("stmts").at(0).at("expr").at("sym").at("name").get<std::string>(), std::string("a"));
    Arena arena2;
    ir::Node* back = l2::from_json(arena2, doc);
    const auto names = ir::walk(back);
    CB_CHECK(ir::get<ir::Name>(names[2])->sym != ir::get<ir::Name>(names[4])->sym);   // two uses, two symbols, as before
});

// Every new kind and slot that is not in the method above.
CB_TEST("translate.l2.cil.the_new_kinds_round_trip", [] {
    Arena arena;
    auto name = [&](const char* n) {
        ir::Node* x = ir::make(arena, ir::Name{.sym = ir::make_sym(ir::SymKind::Local, n)}, ir::i32_type());
        x->stack = ir::Stack::I4;
        return x;
    };
    std::vector<ir::Node*> stmts;
    // Region of two blocks, a goto and a break with a value
    ir::Node* brk = ir::make(arena, ir::Break{.label = "IL_0000", .value = name("v")});
    ir::Node* jump = ir::make(arena, ir::Goto{.label = "IL_0004"});
    ir::Node* b1 = ir::make(arena, ir::Labeled{.label = "IL_0000", .body = ir::make(arena, ir::Block{.stmts = {jump}})});
    ir::Node* b2 = ir::make(arena, ir::Labeled{.label = "IL_0004", .body = ir::make(arena, ir::Block{.stmts = {brk}})});
    stmts.push_back(ir::make(arena, ir::Region{.kind = ir::RegionKind::DoWhile, .blocks = {b1, b2}, .entry = "IL_0004"}));
    // BlockExpr, ThrowExpr, OptionalChain/Unwrap, Default, SizeOf, StackAlloc
    ir::Node* be = ir::make(arena, ir::BlockExpr{.role = "array_init", .stmts = {ir::make(arena, ir::Empty{.note = "n", .kind = ir::EmptyKind::Pop})}, .result = name("r")}, ir::i32_type());
    ir::Node* thr = ir::make(arena, ir::ThrowExpr{.x = nullptr}, ir::any_type());
    ir::Node* unwrap = ir::make(arena, ir::OptionalUnwrap{.x = name("a"), .ref_in = true}, ir::i32_type());
    ir::Node* chain = ir::make(arena, ir::OptionalChain{.x = unwrap}, ir::i32_type());
    ir::Node* dflt = ir::make(arena, ir::Default{}, ir::class_type("N.S"));
    ir::Node* sz = ir::make(arena, ir::SizeOf{.of = ir::class_type("N.S")}, ir::i32_type());
    ir::Node* alloc = ir::make(arena, ir::StackAlloc{.elem = ir::i32_type(), .count = name("n"), .span = true}, ir::class_type("System.Span", {ir::i32_type()}));
    // PatternMatch with a sub-pattern and a Destructure around it, a Fixed
    ir::PatternMatch pm;
    pm.var = ir::make_sym(ir::SymKind::Local, "p");
    pm.var->role = ir::VarRole::PatternBinding;
    pm.tested = name("t");
    pm.test = ir::class_type("N.R");
    pm.check_not_null = true;
    pm.deconstruct = ir::make_sym(ir::SymKind::Method, "Deconstruct", "N.R");
    pm.is_tuple = true;
    pm.subs = {name("s")};
    ir::Node* match = ir::make(arena, std::move(pm), ir::bool_type());
    ir::Node* destr = ir::make(arena, ir::Destructure{.pattern = match, .conversions = ir::make(arena, ir::BlockExpr{}, ir::void_type()), .assignments = ir::make(arena, ir::BlockExpr{}, ir::void_type())});
    ir::Node* fixed = ir::make(arena, ir::Fixed{.var = ir::make(arena, ir::Local{.sym = ir::make_sym(ir::SymKind::Local, "q"), .init = name("e")}), .body = ir::make(arena, ir::Block{})});
    for (ir::Node* e : {be, thr, chain, dflt, sz, alloc}) stmts.push_back(ir::make(arena, ir::ExprStmt{.expr = e}));
    stmts.push_back(destr);
    stmts.push_back(fixed);
    // Try with a fault block and a Catch with a filter; a Switch with ranges, a null label, lifted; a Cond-like Cast with the conversion kinds
    ir::Catch c;
    c.sym = ir::make_sym(ir::SymKind::Local, "ex");
    c.sym->role = ir::VarRole::CatchVar;
    c.types = {ir::class_type("System.Exception")};
    c.filter = name("f");
    c.body = ir::make(arena, ir::Block{});
    ir::Node* cn = ir::make(arena, std::move(c));
    cn->info.set("cil.spec_range", std::vector<std::int64_t>{4, 9});
    ir::Try t;
    t.body = ir::make(arena, ir::Block{});
    t.catches = {cn};
    t.fault_body = ir::make(arena, ir::Block{});
    stmts.push_back(ir::make(arena, std::move(t)));
    ir::Case k1;
    k1.labels = {ir::make(arena, ir::Lit{.value = std::int64_t{1}}, ir::i32_type())};
    k1.ranges = {{1, 5}, {std::numeric_limits<std::int64_t>::min(), std::numeric_limits<std::int64_t>::max()}};
    k1.null_label = true;
    ir::Switch sw;
    sw.expr = name("sel");
    sw.cases = {ir::make(arena, std::move(k1))};
    sw.lifted = true;
    sw.of = ir::i32_type();
    stmts.push_back(ir::make(arena, std::move(sw)));
    ir::Cast cast;
    cast.x = name("x");
    cast.conv = ir::ConvKind::FloatToInt;
    cast.from_stk = ir::Stack::F8;
    cast.from_signed = true;
    cast.lift = ir::Lift::ThreeValued;
    cast.overflow = ir::Overflow::Trap;
    ir::Node* cast_node = ir::make(arena, std::move(cast), ir::i32_type());
    cast_node->info.set("cil.target_r", true);
    stmts.push_back(ir::make(arena, ir::ExprStmt{.expr = cast_node}));
    ir::Binary coalesce{.op = "??", .left = name("l"), .right = name("r2")};
    coalesce.coalesce = ir::Coalesce::NullableValue;
    coalesce.sym = ir::make_sym(ir::SymKind::Method, "op_Equality", "N.C", true);
    ir::Unary neg{.op = "~", .x = name("u")};
    neg.left_stk = ir::Stack::I8;
    neg.lift = ir::Lift::CSharp;
    ir::Index idx;
    idx.arr = name("arr");
    idx.index = name("i");
    idx.elem = ir::i32_type();
    ir::MethodRef mr;
    mr.target = name("o");
    mr.mode = "virtual_delegate";
    mr.of = ir::class_type("System.Func");
    ir::Await aw;
    aw.x = name("task");
    aw.awaiter = ir::make_sym(ir::SymKind::Method, "GetAwaiter", "N.T");
    aw.result = ir::make_sym(ir::SymKind::Method, "GetResult", "N.A");
    for (ir::Node* e : {ir::make(arena, std::move(coalesce), ir::i32_type()), ir::make(arena, std::move(neg), ir::i32_type()), ir::make(arena, std::move(idx), ir::i32_type()),
                        ir::make(arena, std::move(mr), ir::any_type()), ir::make(arena, std::move(aw), ir::i32_type())}) {
        stmts.push_back(ir::make(arena, ir::ExprStmt{.expr = e}));
    }
    ir::Node* root = ir::make(arena, ir::Block{.stmts = stmts});
    Arena arena2;
    ir::Node* back = again(arena2, *root, nullptr);
    CB_CHECK_EQ(ir::walk(back).size(), ir::walk(root).size());
    // a few slots by name, read back
    std::map<std::string_view, int> kinds;
    for (ir::Node* n : ir::walk(back)) ++kinds[ir::kind_name(*n)];
    for (const char* kind : {"Region", "BlockExpr", "ThrowExpr", "OptionalChain", "OptionalUnwrap", "Default", "SizeOf", "StackAlloc", "PatternMatch", "Destructure", "Fixed"}) CB_CHECK(kinds[kind] >= 1);
    for (ir::Node* n : ir::walk(back)) {
        if (auto* sw2 = ir::get<ir::Switch>(n)) {
            CB_CHECK(sw2->lifted && sw2->of == ir::i32_type());
            auto* case1 = ir::get<ir::Case>(sw2->cases[0]);
            CB_CHECK(case1->null_label && case1->ranges.size() == 2 && case1->ranges[0] == std::make_pair(std::int64_t{1}, std::int64_t{5}));
            CB_CHECK(case1->ranges[1].first == std::numeric_limits<std::int64_t>::min() && case1->ranges[1].second == std::numeric_limits<std::int64_t>::max());
        } else if (auto* r = ir::get<ir::Region>(n)) {
            CB_CHECK(r->kind == ir::RegionKind::DoWhile && *r->entry == "IL_0004" && r->blocks.size() == 2);
        } else if (auto* cc = ir::get<ir::Catch>(n)) {
            CB_CHECK(cc->filter != nullptr);
            CB_CHECK((*n->info.get_as<std::vector<std::int64_t>>("cil.spec_range") == std::vector<std::int64_t>{4, 9}));
        } else if (auto* tr = ir::get<ir::Try>(n)) {
            CB_CHECK(tr->fault_body != nullptr && tr->finally_body == nullptr);
        } else if (auto* cs = ir::get<ir::Cast>(n)) {
            CB_CHECK(cs->conv == ir::ConvKind::FloatToInt && cs->from_stk == ir::Stack::F8 && cs->from_signed == true && cs->lift == ir::Lift::ThreeValued);
        } else if (auto* bn = ir::get<ir::Binary>(n)) {
            CB_CHECK(bn->coalesce == ir::Coalesce::NullableValue && bn->sym != nullptr);
        } else if (auto* pm2 = ir::get<ir::PatternMatch>(n)) {
            CB_CHECK(pm2->var->role == ir::VarRole::PatternBinding && pm2->check_not_null && pm2->is_tuple && pm2->subs.size() == 1 && pm2->test == ir::class_type("N.R"));
        } else if (auto* b = ir::get<ir::Break>(n)) {
            CB_CHECK(b->value != nullptr);
        } else if (auto* ix = ir::get<ir::Index>(n)) {
            CB_CHECK(ix->elem == ir::i32_type());
        } else if (auto* em = ir::get<ir::Empty>(n)) {
            CB_CHECK(em->kind == ir::EmptyKind::Pop && *em->note == "n");
        }
    }
});

CB_TEST("translate.l2.cil.every_intrinsic_name_round_trips", [] {
    Arena arena;
    for (const ir::IntrinsicName& row : ir::intrinsic_names()) {
        ir::Intrinsic in;
        in.name = std::string(row.name);
        in.args = {ir::make(arena, ir::Lit{.value = std::int64_t{1}}, ir::i32_type())};
        in.targs = {ir::i32_type()};
        in.syms = {ir::make_sym(ir::SymKind::Field, "f", "N.C")};
        in.strs = {"s"};
        in.imms = {7, -1};
        in.argmeta = {ir::ArgMeta{.name = "a", .flags = 3, .type = ir::class_type("N.T")}, ir::ArgMeta{}};
        ir::Node* n = ir::make(arena, std::move(in), ir::i32_type());
        n->stack = ir::Stack::O;
        for (ir::NodeFlag f : ir::all_node_flags()) {
            if (row.flags.find(ir::to_text(f)) != std::string_view::npos) ir::set_flag(*n, f);
        }
        Arena arena2;
        ir::Node* back = again(arena2, *n, nullptr);
        auto* got = ir::get<ir::Intrinsic>(back);
        CB_CHECK(got != nullptr && got->name == row.name && got->imms == std::vector<std::int64_t>({7, -1}) && got->argmeta.size() == 2 && *got->argmeta[0].name == "a");
        CB_CHECK(got->argmeta[0].flags == 3u && !got->argmeta[1].name && got->argmeta[1].flags == 0);
    }
    // a name that has no row is refused, writing and reading: it is how a name needs its row
    ir::Intrinsic bad;
    bad.name = "cil.invented";
    ir::Node* n = ir::make(arena, std::move(bad));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::to_json(*n));
    Arena arena2;
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena2, parse(R"({"node":"Intrinsic","name":"cil.invented"})")));
    CB_CHECK_NOTHROW(l2::from_json(arena2, parse(R"({"node":"Intrinsic","name":"mem.copy"})")));
});

// The schema is the rule for what a document may carry: a key, a flag or a name it does not list is refused by the reader, and by the writer.
CB_TEST("translate.l2.cil.what_the_schema_does_not_list_is_refused", [] {
    Arena arena;
    // info: an unknown key, a key on a thing it does not belong on, a node kind it may not sit on, a value of another kind
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","info":{"cil.invented":1}})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","info":{"cil.pinned":true}})")));      // a type's key
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","info":{"cil.code_size":3}})")));      // not on an Empty
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"MethodDecl","info":{"cil.code_size":"3"}})")));
    CB_CHECK_NOTHROW(l2::from_json(arena, parse(R"({"node":"MethodDecl","info":{"cil.code_size":3}})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"i32","info":{"cil.code_size":3}})")));
    CB_CHECK_NOTHROW(l2::type_from_json(parse(R"({"kind":"i32","info":{"cil.native_int":true}})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::sym_from_json(parse(R"({"kind":"local","name":"a","info":{"cil.native_int":true}})")));
    CB_CHECK_NOTHROW(l2::sym_from_json(parse(R"({"kind":"local","name":"a","info":{"cil.remove_if_redundant":true}})")));
    // flags, stack kinds, enumerated slots
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","fl":["volatile","sticky"]})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","fl":"volatile"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Empty","stk":"i5"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Call","form":"sideways"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Binary","op":"+","lift":"maybe"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Cast","conv":"melt"})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Region","kind":"circle"})")));
    CB_CHECK_NOTHROW(l2::from_json(arena, parse(R"({"node":"Region","kind":"loop","blocks":[]})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, parse(R"({"node":"Case","ranges":[[1]]})")));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::sym_from_json(parse(R"({"kind":"local","name":"a","role":"boss"})")));
    // the writer: a tree that holds a key or a flag bit that is not in the tables does not write a document nobody can read
    ir::Node* bad = ir::make(arena, ir::Empty{});
    bad->info.set("cil.invented", true);
    CB_CHECK_THROWS_AS(cb::FormatError, l2::to_json(*bad));
    bad->info = ir::InfoP{};
    bad->flags = 1u << 30;
    CB_CHECK_THROWS_AS(cb::FormatError, l2::to_json(*bad));
    bad->flags = 0;
    ir::Node* decl = ir::make(arena, ir::MethodDecl{});
    decl->info.set("cil.code_size", std::string("not an int"));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::to_json(*decl));
    // the other lanes' documents may state none of it, and read and write as before (the real ones are the tests above)
    CB_CHECK_NOTHROW(l2::from_json(arena, parse(R"({"node":"Binary","op":"+","signed":true})")));
});

// Entities: the table, the scope, and what happens when the scope cannot make one.
CB_TEST("translate.l2.cil.entities_need_their_scope", [] {
    CilMethod m;
    const Json doc = l2::to_json(*m.root);
    Arena arena;
    // no scope: the document names entities, so it cannot be read
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, doc));
    // a scope that cannot make one of them: the error names its key
    Json missing = doc;
    missing["entities"][1]["key"] = "F:missing";
    try {
        l2::from_json(arena, missing, std::make_shared<L2TestScope>());
        CB_CHECK(false);
    } catch (const cb::FormatError& e) {
        CB_CHECK(std::string(e.what()).find("F:missing") != std::string::npos);
    }
    // a scope of another family; an entity that resolves to another kind than the table says
    Json other_family = doc;
    other_family["entities"][0]["family"] = "clr";
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, other_family, std::make_shared<L2TestScope>()));
    Json other_kind = doc;
    other_kind["entities"][0]["kind"] = "field";
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, other_kind, std::make_shared<L2TestScope>()));
    // an index that is not in the table
    Json beyond = doc;
    beyond["vars"][2]["e"] = 99;
    CB_CHECK_THROWS_AS(cb::FormatError, l2::from_json(arena, beyond, std::make_shared<L2TestScope>()));
    // a Module keeps its scope (it holds the entities' compilation); a document without entities needs none
    ir::Node* module = ir::make(arena, ir::Module{.units = {m.root}});
    const auto scope = std::make_shared<L2TestScope>();
    Arena arena2;
    ir::Node* back = l2::from_json(arena2, l2::to_json(*module), scope);
    CB_CHECK(ir::get<ir::Module>(back)->scope == scope);
    CB_CHECK_NOTHROW(l2::from_json(arena2, parse(R"({"node":"Module","units":[],"scope_refs":["System.Private.CoreLib"]})")));
    CB_CHECK_EQ(ir::get<ir::Module>(l2::from_json(arena2, parse(R"({"node":"Module","scope_refs":["a.dll","b.dll"]})")))->scope_refs.size(), std::size_t{2});
    // a type or symbol alone writes its entity inline and cannot be read alone
    const ir::TypeP t = ir::make_type(ir::ClassType{.name = "N.C"}, entity_of(ir::EntityKind::Type, "T:N.C"));
    const Json alone = l2::type_to_json(t);
    CB_CHECK_EQ(alone.at("e").at("key").get<std::string>(), std::string("T:N.C"));
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(alone));
});

// The new types, and the origin that is a byte range.
CB_TEST("translate.l2.cil.the_new_types_and_the_span_origin_round_trip", [] {
    roundtrip_type(R"({"kind":"tuple","elems":[{"kind":"i32"},{"kind":"class","name":"System.String"}],"names":["id",null]})");
    roundtrip_type(R"({"kind":"tuple","elems":[{"kind":"i32"}]})");
    roundtrip_type(R"({"kind":"mod","inner":{"kind":"i32"},"required":true,"modifier":{"kind":"class","name":"System.Runtime.CompilerServices.IsVolatile"}})");
    roundtrip_type(R"({"kind":"typevar","name":"T","index":1,"owner":"System.Collections.Generic.Dictionary`2","of_method":true})");
    roundtrip_type(R"({"kind":"ptr","space":"flat","to":{"kind":"i32"},"managed":true})");
    roundtrip_type(R"({"kind":"fn","conv":"thiscall","params":[],"result":{"at":"none"},"has_this":true,"explicit_this":true})");
    roundtrip_type(R"({"kind":"i32","info":{"cil.native_int":true}})");
    CB_CHECK_THROWS_AS(cb::FormatError, l2::type_from_json(parse(R"({"kind":"tuple","elems":[{"kind":"i32"}],"names":["a","b"]})")));
    Arena arena;
    ir::Node* n = ir::make(arena, ir::Empty{});
    n->origin = std::make_shared<const ir::Origin>(ir::OriginSpan{10, 14});
    const Json doc = l2::to_json(*n);
    CB_CHECK_EQ(doc.at("origin").dump(), std::string(R"({"start":10,"end":14})"));
    Arena arena2;
    CB_CHECK((std::get<ir::OriginSpan>(*l2::from_json(arena2, doc)->origin) == ir::OriginSpan{10, 14}));
    // the other origins read as before
    CB_CHECK_NOTHROW(l2::from_json(arena2, parse(R"({"node":"Empty","origin":{"module":"a.dll","va":4096}})")));
    CB_CHECK_NOTHROW(l2::from_json(arena2, parse(R"({"node":"Empty","origin":{"at":[1,2]}})")));
});

// The schema (config/translate/l2.schema.json) and the tables of ir.cpp say the same.
// The slots the CIL lane's lowering needed beyond the design's table: a Lambda's method, mods, nested functions and result type; a
// SwitchExpr's lifted and of; a Destructure's init stores; an info value that is a list of types (a generic context).
CB_TEST("translate.l2.cil.the_lane_slots_round_trip", [] {
    Arena arena;
    ir::Node* local_fn = ir::make(arena, ir::MethodDecl{.name = "Local", .kind = "method", .mods = {"local"}});
    ir::Lambda l;
    l.sym = ir::make_sym(ir::SymKind::Method, "<M>b__0_0", "N.C");
    l.mods = {"async", "expression_tree"};
    l.nested = {local_fn};
    l.ret = ir::i32_type();
    ir::SymP v = ir::make_sym(ir::SymKind::Local, "v");
    ir::Node* store = ir::make(arena, ir::Assign{.target = ir::make(arena, ir::Name{.sym = v}, ir::i32_type()),
                                                 .value = ir::make(arena, ir::Lit{.value = std::int64_t{1}}, ir::i32_type())}, ir::i32_type());
    ir::Node* destr = ir::make(arena, ir::Destructure{.init = {store}, .pattern = ir::make(arena, ir::Name{.sym = v}, ir::i32_type())});
    ir::Node* sw = ir::make(arena, ir::SwitchExpr{.expr = ir::make(arena, ir::Name{.sym = v}, ir::i32_type()), .lifted = true, .of = ir::i32_type()},
                            ir::i32_type());
    l.body = ir::make(arena, ir::Block{.stmts = {destr, ir::make(arena, ir::ExprStmt{.expr = sw})}});
    ir::Node* root = ir::make(arena, std::move(l), ir::class_type("System.Func", {ir::i32_type()}));
    root->info.set("cil.class_tparams", std::vector<ir::TypeP>{ir::typevar_type("T"), ir::typevar_type("U")});
    root->info.set("cil.method_tparams", std::vector<ir::TypeP>{});
    root->info.set("cil.name", std::string("Lambda"));
    Arena arena2;
    ir::Node* back = again(arena2, *root, nullptr);
    const auto* lb = ir::get<ir::Lambda>(back);
    CB_CHECK(lb != nullptr);
    CB_CHECK(lb->sym != nullptr && lb->sym->name == "<M>b__0_0");
    CB_CHECK_EQ(lb->mods.size(), std::size_t{2});
    CB_CHECK_EQ(lb->nested.size(), std::size_t{1});
    CB_CHECK(lb->ret == ir::i32_type());
    const auto* tps = back->info.get_as<std::vector<ir::TypeP>>("cil.class_tparams");
    CB_CHECK(tps != nullptr && tps->size() == 2);
    CB_CHECK(back->info.get_as<std::vector<ir::TypeP>>("cil.method_tparams") != nullptr);
    const auto* block = ir::get<ir::Block>(lb->body);
    CB_CHECK(block != nullptr && block->stmts.size() == 2);
    const auto* d = ir::get<ir::Destructure>(block->stmts[0]);
    CB_CHECK(d != nullptr && d->init.size() == 1);
    const auto* swb = ir::get<ir::SwitchExpr>(ir::get<ir::ExprStmt>(block->stmts[1])->expr);
    CB_CHECK(swb != nullptr && swb->lifted && swb->of == ir::i32_type());
    // the walk enters the nested functions and the init stores
    std::size_t methods = 0, assigns = 0;
    for (ir::Node* n : ir::walk(back)) {
        methods += ir::is<ir::MethodDecl>(n) ? 1 : 0;
        assigns += ir::is<ir::Assign>(n) ? 1 : 0;
    }
    CB_CHECK_EQ(methods, std::size_t{1});
    CB_CHECK_EQ(assigns, std::size_t{1});
});

CB_TEST("translate.l2.cil.the_schema_lists_what_ir_cpp_lists", [] {
    const Json schema = cb::json::load(cb::testing::data_path("../config/translate/l2.schema.json"));
    const Json& defs = schema.at("$defs");
    // InfoKeys and IntrinsicNames are the tables of ir.cpp
    const Json& keys = defs.at("InfoKeys").at("x-keys");
    CB_CHECK_EQ(keys.size(), ir::info_keys().size());
    for (std::size_t i = 0; i < keys.size(); ++i) {
        const ir::InfoKey& k = ir::info_keys()[i];
        CB_CHECK_EQ(keys.at(i).at("key").get<std::string>(), std::string(k.key));
        CB_CHECK_EQ(keys.at(i).at("doc").get<std::string>(), std::string(k.doc));
        CB_CHECK_EQ(keys.at(i).at("general").get<bool>(), k.general);
        const char* on = k.on == ir::InfoOn::Node ? "InfoOnNode" : k.on == ir::InfoOn::Type ? "InfoOnType" : "InfoOnSym";
        CB_CHECK(defs.at(on).at("properties").contains(std::string(k.key)));
    }
    const Json& names = defs.at("IntrinsicNames").at("x-names");
    CB_CHECK_EQ(names.size(), ir::intrinsic_names().size());
    std::set<std::string> in_schema;
    for (const Json& n : defs.at("Intrinsic").at("properties").at("name").at("enum")) in_schema.insert(n.get<std::string>());
    for (std::size_t i = 0; i < names.size(); ++i) {
        const ir::IntrinsicName& n = ir::intrinsic_names()[i];
        CB_CHECK_EQ(names.at(i).at("name").get<std::string>(), std::string(n.name));
        CB_CHECK_EQ(names.at(i).at("why").get<std::string>(), std::string(n.why));
        CB_CHECK(in_schema.contains(std::string(n.name)));
    }
    CB_CHECK_EQ(in_schema.size(), names.size());
    // every enum's every name, and every flag, is in the schema; every node kind the tree can make has a definition and is in the Node dispatch
    auto in_enum = [&](const char* def, std::string_view text) {
        for (const Json& v : defs.at(def).at("enum")) {
            if (v.get<std::string>() == text) return true;
        }
        return false;
    };
    for (int i = 0; i <= static_cast<int>(ir::Stack::Void); ++i) CB_CHECK(in_enum("Stack", ir::to_text(static_cast<ir::Stack>(i))));
    for (int i = 0; i <= static_cast<int>(ir::Lift::ThreeValued); ++i) CB_CHECK(in_enum("Lift", ir::to_text(static_cast<ir::Lift>(i))));
    for (int i = 0; i <= static_cast<int>(ir::Coalesce::NullableValue); ++i) CB_CHECK(in_enum("Coalesce", ir::to_text(static_cast<ir::Coalesce>(i))));
    for (int i = 0; i <= static_cast<int>(ir::ConvKind::ExprTree); ++i) CB_CHECK(in_enum("ConvKind", ir::to_text(static_cast<ir::ConvKind>(i))));
    for (int i = 0; i <= static_cast<int>(ir::Eval::New); ++i) CB_CHECK(in_enum("Eval", ir::to_text(static_cast<ir::Eval>(i))));
    for (int i = 0; i <= static_cast<int>(ir::Via::Dynamic); ++i) CB_CHECK(in_enum("Via", ir::to_text(static_cast<ir::Via>(i))));
    for (int i = 0; i <= static_cast<int>(ir::CallForm::Indirect); ++i) CB_CHECK(in_enum("CallForm", ir::to_text(static_cast<ir::CallForm>(i))));
    for (int i = 0; i <= static_cast<int>(ir::RegionKind::Switch); ++i) CB_CHECK(in_enum("RegionKind", ir::to_text(static_cast<ir::RegionKind>(i))));
    for (int i = 0; i <= static_cast<int>(ir::EmptyKind::Pop); ++i) CB_CHECK(in_enum("EmptyKind", ir::to_text(static_cast<ir::EmptyKind>(i))));
    for (int i = 0; i <= static_cast<int>(ir::VarRole::DeconTemp); ++i) CB_CHECK(in_enum("VarRole", ir::to_text(static_cast<ir::VarRole>(i))));
    for (ir::NodeFlag f : ir::all_node_flags()) {
        bool found = false;
        for (const Json& v : defs.at("NodeFlags").at("items").at("enum")) found = found || v.get<std::string>() == ir::to_text(f);
        CB_CHECK(found);
    }
    std::set<std::string> dispatch;
    for (const Json& v : defs.at("Node").at("properties").at("node").at("enum")) dispatch.insert(v.get<std::string>());
    Arena arena;
    for (const std::string& kind : dispatch) {
        CB_CHECK(ir::make_node(arena, kind) != nullptr);   // (a kind the schema lists is a kind the tree has ...)
        CB_CHECK(defs.contains(kind));
    }
    CB_CHECK_EQ(dispatch.size(), std::variant_size_v<ir::Kind>);   // (... and the other way)
});

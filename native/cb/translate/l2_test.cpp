// Tests of L2 documents over the typed tree and of the module model. The documents are real: testdata/l2_python_unit.json
// is the Python front end's Unit for a small program, testdata/l2_java_units.json the Java front end's for a program of
// modern Java; both are what the producers wrote, and they must read, walk and write back as the same documents.
//
// DIVERGES from codebreaker/translate/l2.py (where a document was a dict of whatever its producer put there):
//   * a slot holding a value of the wrong kind (a string where a list is, a type with an unknown kind) is a FormatError at
//     the reader; the Python kept the raw value in the slot and failed later, in whoever used it;
//   * a flag that is false, a list that is empty and a slot that is null are not written (the Python wrote the ones its
//     producer set); a list slot the walk follows is always written, empty or not, so no consumer meets null where it iterates;
//   * a key a node has no slot for is dropped on reading (a Python document kept none either, but its `slots()` was a list a
//     subclass could extend); Local's `at` of the schema has no slot in the typed tree and is dropped;
//   * a float that is not finite is written as its name and read back as that text; the Python wrote repr() and read it back
//     the same way;
//   * an integer above 2**64 is not kept (a JSON number that large is a float here).
#include "cb/translate/l2.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

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

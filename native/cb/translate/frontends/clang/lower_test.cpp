// Tests of the clang front end: the type spellings (ctypes) and the lowering of clang's JSON AST into the IR.
//
// The real inputs: testdata/clang_<program>.tree.json is what `clang -Xclang -ast-dump=json` wrote for a C or C++ program
// (the repo's C and C++ corpus, the CodingTeacher lessons, generated programs; the file paths rewritten to C:\proj) and
// testdata/clang_<program>.l2.json the L2 document the original lowering (codebreaker/translate/frontends/clang/lower.py)
// made of it; clang_cases.json says how each is lowered (main file, language, own files). The lowering here must give the
// same document. The helper cases (string and character literals, widths, spellings, layouts, type parsing) were run through
// the original too and reviewed.
//
// DIVERGES from the original:
//   * a tree that is not what clang writes (a member missing, a value of the wrong kind) is a ParseError inside, and the
//     declaration or expression it was found in becomes an Unsupported with the reason as `raw`; the Python caught
//     TypeError, KeyError, ValueError and IndexError at the same places;
//   * constants are int64: an unsigned 64-bit constant past INT64_MAX, or an intermediate result that leaves int64, is no
//     constant (the Python's integers grew without end); sizeof/alignof above INT64_MAX is written as an unsigned Lit;
//   * a typedef that names itself is a ParseError after 40 nested names (the Python reached its recursion limit);
//   * a function-try-block constructor with member initialisers gets them in a Block before the try; the Python failed with
//     an AttributeError;
//   * `__attribute__((...))` in a type spelling is dropped whatever it nests (the Python's pattern allowed one level);
//   * the dump is not changed: the Python wrote `_f`, `_l`, `_c` and `_rb` into every node of the tree it was given.
#include "cb/translate/frontends/clang/lower.hpp"

#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/frontends/clang/ctypes.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace cl = cb::translate::frontends::clang;
namespace ct = cb::translate::frontends::clang::ctypes;
namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using cb::Arena;
using cb::Json;

// A document without what carries no information (nulls, false flags, empty lists), keys in order.
Json clang_test_normal(const Json& j) {
    if (j.is_object()) {
        std::map<std::string, Json> sorted;
        for (const auto& [k, v] : j.items()) {
            if (v.is_null() || (v.is_boolean() && !v.get<bool>()) || (v.is_array() && v.empty())) continue;
            if (k == "raw") continue;   // the text of an error is for people: not compared
            sorted.emplace(k, clang_test_normal(v));
        }
        Json out = Json::object();
        for (auto& [k, v] : sorted) out[k] = std::move(v);
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(clang_test_normal(v));
        return out;
    }
    return j;
}

// Where two normal documents first differ, as a path with both sides (empty: they do not).
std::string clang_test_difference(const Json& a, const Json& b, const std::string& path) {
    if (a == b) return {};
    if (a.is_object() && b.is_object()) {
        for (const auto& [k, v] : a.items()) {
            if (!b.contains(k)) return path + "/" + k + ": only in the first: " + v.dump().substr(0, 200);
            if (std::string d = clang_test_difference(v, b.at(k), path + "/" + k); !d.empty()) return d;
        }
        for (const auto& [k, v] : b.items()) {
            if (!a.contains(k)) return path + "/" + k + ": only in the second: " + v.dump().substr(0, 200);
        }
    }
    if (a.is_array() && b.is_array()) {
        if (a.size() != b.size()) return path + ": " + std::to_string(a.size()) + " items against " + std::to_string(b.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (std::string d = clang_test_difference(a[i], b[i], path + "/" + std::to_string(i)); !d.empty()) return d;
        }
    }
    return path + ": " + a.dump().substr(0, 200) + " against " + b.dump().substr(0, 200);
}

std::size_t clang_test_count_unsupported(const Json& j) {
    std::size_t n = 0;
    if (j.is_object()) {
        if (j.contains("node") && j.at("node") == "Unsupported") ++n;
        for (const auto& [k, v] : j.items()) n += clang_test_count_unsupported(v);
    } else if (j.is_array()) {
        for (const Json& v : j) n += clang_test_count_unsupported(v);
    }
    return n;
}

Json clang_test_load(const std::string& relative) { return cb::json::load(cb::testing::data_path("translate/frontends/clang/testdata/" + relative)); }

cl::Options clang_test_options(const Json& c) {
    cl::Options o;
    o.main_path = c.at("path").get<std::string>();
    o.language = c.at("language").get<std::string>();
    if (!c.at("std").is_null()) o.standard = c.at("std").get<std::string>();
    if (!c.at("own").is_null()) o.own = c.at("own").get<std::vector<std::string>>();
    return o;
}

// The type of each name the test's resolver knows.
ir::TypeP clang_test_names(const std::string& name, const std::vector<ir::TypeP>& args, const std::optional<std::string>&) {
    std::string dotted;
    for (std::size_t i = 0; i < name.size(); ++i) {
        if (name.compare(i, 2, "::") == 0) {
            dotted += '.';
            ++i;
        } else {
            dotted += name[i];
        }
    }
    if (dotted == "Pair" || dotted == "geo.Vec") return ir::class_type(dotted);
    if (dotted == "uint8_t") return ir::prim_type(ir::Prim::U8);
    if (dotted == "size_t") return ir::prim_type(ir::Prim::U64);
    if (dotted == "Vec") return ir::class_type("Vec", args);
    return {};
}

std::string clang_test_parse(const std::string& spelling) {
    ct::Parser parser(ct::Model::for_triple("x86_64-pc-windows-msvc"), clang_test_names);
    try {
        return cl::spell(ct::normalise(parser.parse(spelling)));
    } catch (const cb::ParseError&) {
        return "ERR parse";
    } catch (const cb::FormatError&) {
        return "ERR format";
    }
}

std::string clang_test_literal(const std::string& spelling) {
    try {
        return cl::string_literal_value(spelling);
    } catch (const cb::ParseError&) {
        return "ERR";
    }
}

}  // namespace

// ---- the real programs -------------------------------------------------------------------------------------------------

CB_TEST("translate.clang.lower.programs_give_the_reviewed_documents", [] {
    const Json cases = clang_test_load("clang_cases.json");
    CB_CHECK(cases.size() >= 5);
    for (const Json& c : cases) {
        const std::string name = c.at("name").get<std::string>();
        const Json tree = clang_test_load("clang_" + name + ".tree.json");
        const Json golden = clang_test_normal(clang_test_load("clang_" + name + ".l2.json"));
        Arena arena;
        cl::Lowered out = cl::lower(arena, tree, clang_test_options(c));
        const Json made = clang_test_normal(l2::to_json(*out.unit));
        CB_CHECK_EQ(clang_test_difference(made, golden, name), std::string());
        CB_CHECK_EQ(out.refused.size(), clang_test_count_unsupported(golden));
        // the document reads back and writes the same
        Arena again;
        CB_CHECK(clang_test_normal(l2::to_json(*l2::from_json(again, made))) == made);
    }
});

CB_TEST("translate.clang.lower.the_unit_says_what_it_is", [] {
    const Json tree = clang_test_load("clang_ct_mymath_c.tree.json");
    Arena arena;
    cl::Options options;
    options.main_path = "C:\\proj\\mymath.c";
    cl::Lowered out = cl::lower(arena, tree, options);
    const auto* unit = ir::get<ir::Unit>(out.unit);
    CB_CHECK(unit != nullptr);
    CB_CHECK_EQ(*unit->path, std::string("c:\\proj\\mymath.c"));   // as os.path.normcase(os.path.abspath(..)) writes it
    CB_CHECK_EQ(unit->target->pointer.value(), 8u);
    CB_CHECK_EQ(unit->target->long_bits.value(), 4u);
    CB_CHECK_EQ(unit->target->language.value(), std::string("c"));
    CB_CHECK(!unit->types.empty());
    const auto* first = ir::get<ir::MethodDecl>(unit->types[0]);
    CB_CHECK(first != nullptr && first->kind == "function");
});

CB_TEST("translate.clang.lower.a_tree_that_is_not_clangs_is_refused_where_it_is_found", [] {
    // an IntegerLiteral with no value, and a record with no id: each is one Unsupported, the rest of the unit lowers
    const char* dump = R"JSON({"kind":"TranslationUnitDecl","inner":[
      {"id":"0x1","kind":"FunctionDecl","name":"f","loc":{"file":"C:/proj/a.c","line":1,"col":5},
       "range":{"begin":{"file":"C:/proj/a.c","line":1,"col":1},"end":{"line":3,"col":1}},"type":{"qualType":"int ()"},
       "inner":[{"kind":"CompoundStmt","loc":{"line":1,"col":9},"inner":[
         {"kind":"ReturnStmt","loc":{"line":2,"col":3},"inner":[{"kind":"IntegerLiteral","loc":{"line":2,"col":10},"type":{"qualType":"int"}}]}]}]},
      {"id":"0x2","kind":"FunctionDecl","name":"g","loc":{"line":5,"col":5},"type":{"qualType":"int ()"},
       "inner":[{"kind":"CompoundStmt","inner":[{"kind":"ReturnStmt","inner":[{"kind":"IntegerLiteral","value":"7","type":{"qualType":"int"}}]}]}]}]})JSON";
    Arena arena;
    cl::Options options;
    options.main_path = "C:/proj/a.c";
    cl::Lowered out = cl::lower_text(arena, dump, options);
    CB_CHECK_EQ(out.refused.size(), std::size_t{1});
    CB_CHECK_EQ(*out.refused[0].kind, std::string("expr:IntegerLiteral"));
    CB_CHECK_EQ(out.refused[0].line, 2u);
    const auto* unit = ir::get<ir::Unit>(out.unit);
    CB_CHECK_EQ(unit->types.size(), std::size_t{2});
    // text that is not JSON
    CB_CHECK_THROWS_AS(cb::Error, (void)cl::lower_text(arena, "{not json", options));
});

// ---- literals, widths, spellings ---------------------------------------------------------------------------------------

CB_TEST("translate.clang.lower.string_literals", [] {
    CB_CHECK_EQ(clang_test_literal("\"\""), std::string());
    CB_CHECK_EQ(clang_test_literal("\"abc\""), std::string("abc"));
    CB_CHECK_EQ(clang_test_literal("\"a\\nb\""), std::string("a\nb"));
    CB_CHECK_EQ(clang_test_literal("\"\\x41\\101\\0z\""), std::string("AA\0z", 4));
    CB_CHECK_EQ(clang_test_literal("\"tab\\there\""), std::string("tab\there"));
    CB_CHECK_EQ(clang_test_literal("\"a\\\\b\\\"c\""), std::string("a\\b\"c"));
    CB_CHECK_EQ(clang_test_literal("\"\\q\""), std::string("q"));
    // one character per byte, written as UTF-8 of the latin-1 character: a UTF-8 e-acute is two characters
    CB_CHECK_EQ(clang_test_literal("\"\xC3\xA9\""), std::string("\xC3\x83\xC2\xA9"));
    CB_CHECK_EQ(clang_test_literal("\"\\xffz\""), std::string("\xC3\xBF" "z"));
    // a wide literal: its UTF-16 code units, one character each
    CB_CHECK_EQ(clang_test_literal("L\"hi\\x1234\""), std::string("hi\xE1\x88\xB4"));
    CB_CHECK_EQ(clang_test_literal("u\"a\\u00e9\\n\""), std::string("a\xC3\xA9\n"));
    CB_CHECK_EQ(clang_test_literal("L\"\\0101\""), std::string("\b1"));
    // not literals the front end reads
    CB_CHECK_EQ(clang_test_literal("U\"x\""), std::string("ERR"));
    CB_CHECK_EQ(clang_test_literal("\"a\\\""), std::string("ERR"));   // a backslash with nothing after it
    CB_CHECK_EQ(clang_test_literal("no quote"), std::string("ERR"));
});

CB_TEST("translate.clang.lower.character_text_and_widths", [] {
    CB_CHECK_EQ(cl::char_literal_text('A').value(), std::string("'A'"));
    CB_CHECK_EQ(cl::char_literal_text('\n').value(), std::string("'\\n'"));
    CB_CHECK_EQ(cl::char_literal_text('\'').value(), std::string("'\\''"));
    CB_CHECK_EQ(cl::char_literal_text('\\').value(), std::string("'\\\\'"));
    CB_CHECK_EQ(cl::char_literal_text('"').value(), std::string("'\"'"));
    CB_CHECK(!cl::char_literal_text(127).has_value());
    CB_CHECK(!cl::char_literal_text(200).has_value());
    using ir::Prim;
    CB_CHECK_EQ(cl::wrap_to_type(200, ir::prim_type(Prim::I8)), std::int64_t{-56});
    CB_CHECK_EQ(cl::wrap_to_type(-1, ir::prim_type(Prim::U8)), std::int64_t{255});
    CB_CHECK_EQ(cl::wrap_to_type(200, ir::prim_type(Prim::Char, true)), std::int64_t{-56});
    CB_CHECK_EQ(cl::wrap_to_type(200, ir::prim_type(Prim::Char, false)), std::int64_t{200});
    CB_CHECK_EQ(cl::wrap_to_type(70000, ir::prim_type(Prim::I16)), std::int64_t{4464});
    CB_CHECK_EQ(cl::wrap_to_type(-1, ir::prim_type(Prim::U16)), std::int64_t{65535});
    CB_CHECK_EQ(cl::wrap_to_type(std::int64_t{1} << 31, ir::prim_type(Prim::I32)), std::int64_t{-2147483648LL});
    CB_CHECK_EQ(cl::wrap_to_type(-1, ir::prim_type(Prim::U32)), std::int64_t{4294967295LL});
    CB_CHECK_EQ(cl::wrap_to_type(7, ir::bool_type()), std::int64_t{1});
    CB_CHECK_EQ(cl::wrap_to_type(5, ir::prim_type(Prim::I64)), std::int64_t{5});
    CB_CHECK_EQ(cl::wrap_to_type(3, ir::prim_type(Prim::F64)), std::int64_t{3});   // not an integer type: unchanged
    CB_CHECK_EQ(cl::wrap_to_type(9, ir::TypeP()), std::int64_t{9});
});

CB_TEST("translate.clang.ctypes.parse_and_spell", [] {
    const std::map<std::string, std::string> expected = {
        {"int", "i32"},
        {"unsigned long long", "u64"},
        {"const char *", "char*"},
        {"char *const *", "char**"},
        {"int [4]", "i32[4]"},
        {"int (*)[4]", "i32[4]*"},
        {"int (*)(int, char *)", "i32(i32,char*)*"},
        {"void (int, ...)", "void(i32)"},
        {"geo::Vec &", "geo.Vec&"},
        {"Vec<int>", "Vec<i32>"},
        {"unsigned char", "u8"},
        {"long", "i32"},          // LLP64: long is 4 bytes
        {"long double", "f64"},   // and long double is a double
        {"wchar_t", "u16"},
        {"uint8_t [16]", "u8[16]"},
        {"struct Pair", "Pair"},
        {"int (void)", "i32()"},
        {"int (*[3])(void)", "i32()*[3]"},
        {"const Pair &&", "Pair&"},
        {"signed char", "i8"},
        {"short unsigned", "u16"},
        {"bool", "bool"},
    };
    for (const auto& [spelling, spelled] : expected) CB_CHECK_EQ(clang_test_parse(spelling), spelled);
    // what cannot be read
    CB_CHECK_EQ(clang_test_parse("int $"), std::string("ERR parse"));
    CB_CHECK_EQ(clang_test_parse("Unknown"), std::string("ERR parse"));
    CB_CHECK_EQ(clang_test_parse("int [4"), std::string("ERR parse"));
    CB_CHECK_EQ(clang_test_parse("int int2 int"), std::string("ERR parse"));
    CB_CHECK_EQ(clang_test_parse("__int128"), std::string("ERR format"));
    // the model decides the open widths
    ct::Parser linux_parser(ct::Model::for_triple("x86_64-pc-linux-gnu"), clang_test_names);
    CB_CHECK_EQ(cl::spell(linux_parser.parse("long")), std::string("i64"));
    CB_CHECK_EQ(cl::spell(linux_parser.parse("wchar_t")), std::string("i32"));
    CB_CHECK_EQ(cl::spell(linux_parser.parse("long double")), std::string("f80"));
});

CB_TEST("translate.clang.ctypes.tokens", [] {
    using V = std::vector<std::string>;
    CB_CHECK(ct::tokens("struct (unnamed struct at C:\\x\\t.c:3:9) *") == (V{"struct", "__unnamed_3_9", "*"}));
    CB_CHECK(ct::tokens("int (*)(void) __attribute__((cdecl))") == (V{"int", "(", "*", ")", "(", "void", ")"}));
    CB_CHECK(ct::tokens("(lambda at x.cpp:3:4) &&") == (V{"__lambda", "&&"}));
    CB_CHECK(ct::tokens("a::b<c, 4>") == (V{"a", "::", "b", "<", "c", ",", "4", ">"}));
    CB_CHECK(ct::tokens("   ").empty());
    CB_CHECK_THROWS_AS(cb::ParseError, (void)ct::tokens("int @"));
    CB_CHECK_EQ(ct::anon_key(3, 9), std::string("3_9"));
});

CB_TEST("translate.clang.ctypes.layouts_and_decay", [] {
    const ct::Layouts layouts(ct::Model::for_triple("x86_64-pc-windows-msvc"), [](const ir::TypeP&) { return ir::TypeP(); });
    const auto i8 = ir::prim_type(ir::Prim::I8);
    const auto i16 = ir::prim_type(ir::Prim::I16);
    const auto i32 = ir::prim_type(ir::Prim::I32);
    const auto i64 = ir::prim_type(ir::Prim::I64);
    const auto f64 = ir::prim_type(ir::Prim::F64);
    const ir::TypeP s = layouts.lay_out("S", {{"c", i8}, {"i", i32}, {"d", f64}, {"s", i16}});
    const auto* record = std::get_if<ir::RecordType>(&s->body);
    CB_CHECK(record != nullptr);
    CB_CHECK_EQ(record->size, std::uint64_t{24});
    CB_CHECK_EQ(record->fields[1].offset, std::uint64_t{4});
    CB_CHECK_EQ(record->fields[3].offset, std::uint64_t{16});
    CB_CHECK_EQ(layouts.record_align(*record), std::uint64_t{8});
    const ir::TypeP u = layouts.lay_out("U", {{"c", i8}, {"i", i64}}, true);
    CB_CHECK_EQ(std::get<ir::RecordType>(u->body).size, std::uint64_t{8});
    CB_CHECK_EQ(std::get<ir::RecordType>(u->body).fields[1].offset, std::uint64_t{0});
    CB_CHECK_EQ(layouts.size(ir::pointer_to(i8)), std::uint64_t{8});
    CB_CHECK_EQ(layouts.size(ir::make_type(ir::ArrayType{.elem = i32, .count = 5})), std::uint64_t{20});
    CB_CHECK_EQ(layouts.size(ir::prim_type(ir::Prim::F80)), std::uint64_t{16});
    CB_CHECK_EQ(layouts.align(ir::void_type()), std::uint64_t{1});
    // a class the resolver has no record for has no size
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)layouts.size(ir::class_type("Missing")));
    // decay and normal form
    const ir::TypeP fn = ct::fn_type(i32, {i8}, false);
    CB_CHECK_EQ(cl::spell(ct::decay(fn)), std::string("i32(i8)*"));
    CB_CHECK_EQ(cl::spell(ct::decay(ir::make_type(ir::ArrayType{.elem = i32, .count = 3}))), std::string("i32*"));
    CB_CHECK_EQ(cl::spell(ct::normalise(ir::pointer_to(fn))), std::string("i32(i8)*"));
    CB_CHECK(std::holds_alternative<ir::FnPtrType>(ct::normalise(ir::pointer_to(fn))->body));
});

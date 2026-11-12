// Tests of the typed client of the L2 core library. The library itself (native/l2, cbl2.dll) is a separate program that has
// its own tests; these check what this client adds -- the protocol, the answers' shapes, the errors -- on inputs whose answers
// are the C back end's rules as tests/translate/test_c_backend.py states them (PrintC's: a rule per case). They need the DLL
// (cmake --build native/l2/build --target cbl2, or CB_L2_DLL naming it); without it they say so and pass.
//
// DIVERGES from native_l2.py / the old port: the library's errors are LibraryError (Declined, Cpp17Declined) carrying the
// library's own word, not Python exception classes; the arguments' tagged forms ({"$t"}, {"$s"}, {"$d"}) are passed as the
// Json they are rather than as Python tuples, sets and dicts; the schema is told to the library once per distinct text; none of
// the Python's caches that made the ctypes crossing cheaper ($keep, $put, known_text) exist (nothing crosses a process).
#include "cb/translate/native_l2.hpp"

#include <cstdlib>
#include <iostream>
#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace nl = cb::translate::native_l2;
using cb::Json;

// The library, or null (with a note on stderr) when it is not built.
const nl::Library* library() {
    static const std::optional<nl::Library> lib = []() -> std::optional<nl::Library> {
        std::filesystem::path dll = cb::testing::data_path("../l2/build/bin/cbl2.dll");
        if (const auto env = cb::fs::get_env("CB_L2_DLL")) dll = cb::fs::from_text(*env);
        std::error_code ec;
        if (!std::filesystem::is_regular_file(dll, ec)) {
            std::cerr << "native_l2 tests: " << dll.string() << " is not built; skipped\n";
            return std::nullopt;
        }
        return nl::Library::load(dll);
    }();
    return lib ? &*lib : nullptr;
}

Json parse(const char* text) { return cb::json::parse(text); }

Json lit(std::int64_t v, const char* kind = "i32", const char* text = nullptr) {
    Json j{{"node", "Lit"}, {"type", Json{{"kind", kind}}}, {"value", v}};
    if (text != nullptr) j["text"] = text;
    return j;
}
Json name(const char* n, const char* kind = "i32") {
    return Json{{"node", "Name"}, {"type", Json{{"kind", kind}}}, {"sym", Json{{"kind", "local"}, {"name", n}}}};
}
Json binary(const char* op, Json a, Json b) { return Json{{"node", "Binary"}, {"op", op}, {"left", std::move(a)}, {"right", std::move(b)}}; }
Json joined(const char* op, Json a, Json b) {   // a condition the structure joined, as the decompiler's printer marks one
    return binary(op, Json{{"node", "Paren"}, {"x", std::move(a)}}, Json{{"node", "Paren"}, {"x", std::move(b)}});
}
Json ptr(Json to, const char* space = "far") { return Json{{"kind", "ptr"}, {"space", space}, {"to", std::move(to)}}; }

// A function document with this body.
Json function_doc(Json body, std::optional<std::string> mods = std::nullopt) {
    Json doc = parse(
        R"({"node":"MethodDecl","name":"f","kind":"function","ret":{"kind":"i16"},"params":[],)"
        R"("sym":{"kind":"function","name":"f","type":{"kind":"fn","conv":"cdecl","distance":"far","params":[],"result":{"at":"none"}}}})");
    doc["body"] = std::move(body);
    if (mods) doc["mods"] = Json::array({*mods});
    return doc;
}

Json normalised(const Json& j) {   // (what the typed tree keeps: no nulls, no false flags, no empty lists)
    if (j.is_object()) {
        Json out = Json::object();
        for (const auto& [k, v] : j.items()) {
            if (v.is_null() || (v.is_boolean() && !v.get<bool>()) || (v.is_array() && v.empty())) continue;
            out[k] = normalised(v);
        }
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(normalised(v));
        return out;
    }
    return j;
}

const std::vector<std::string>& moved_all() {
    static const std::vector<std::string> all = {
        "far", "near_calls", "far_from_stack", "frame_through_ss", "pieces", "segment_registers", "null_is_zero",
        "global_pointer_steps", "one_name_for_globals", "far_result", "model_word", "joined_arguments", "far_arguments",
        "void_result", "byte_result", "export", "loadds", "selector_structs", "struct_gaps", "own_segment_calls",
        "open_arguments", "interrupts", "far_addresses", "far_casts", "entry_sp", "stack_arguments", "far_locals",
        "far_concat_casts", "buffer_fields", "far_reads", "near_to_far", "stack_segment_stores", "callee_pops",
        "caller_frame_slots", "selector_imports", "segment_values", "code_segment_reads", "names_used", "heading_facts",
        "int_result", "register_arguments", "code_pointers", "compare_as_numbers", "keep_frame", "globals_through",
        "global_widths", "same_cast", "fit_calls", "struct_fields", "wide_address", "homed_arguments", "uint8_result"};
    return all;
}

}  // namespace

CB_TEST("translate.native_l2.loading", [] {
    CB_CHECK_THROWS_AS(cb::NotFoundError, nl::Library::load(cb::testing::data_path("translate/testdata/no_such_cbl2.dll")));
    CB_CHECK_EQ(nl::Library::default_dll("C:/repo").generic_string(), std::string("C:/repo/native/l2/build/bin/cbl2.dll"));
    CB_CHECK_EQ(nl::Library::default_dll("C:/repo", "D:/bin").generic_string(), std::string("D:/bin/cbl2.dll"));
});

CB_TEST("translate.native_l2.numbers_in_their_natural_base", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    // up to 10 in decimal; past it, whichever base ends in a run of 0s/9s or 0s/Fs
    const std::vector<std::pair<std::int64_t, const char*>> cases = {{0, "0"},      {9, "9"},     {10, "10"},    {11, "0xb"},    {500, "500"},
                                                                     {1000, "1000"}, {0x400, "0x400"}, {0x104, "0x104"}, {0xfff, "0xfff"}, {-0x400, "-0x400"}};
    for (const auto& [value, text] : cases) CB_CHECK_EQ(lib->expr(lit(value)), std::string(text));
    CB_CHECK_EQ(lib->expr(Json{{"node", "Lit"}, {"type", ptr(Json{{"kind", "i16"}}, "flat")}, {"value", 0}}), std::string("0x0"));
    CB_CHECK_EQ(lib->expr(lit(0xff0f, "u32", "0xff0fU")), std::string("0xff0fU"));   // the spelling, where L2 has it
});

CB_TEST("translate.native_l2.brackets_and_conditions", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    const Json a = name("a"), b = name("b"), d = name("d");
    CB_CHECK_EQ(lib->expr(binary("-", binary("+", a, b), d)), std::string("(a + b) - d"));   // same precedence, other operator
    CB_CHECK_EQ(lib->expr(binary("+", binary("+", a, b), d)), std::string("a + b + d"));     // the same associative one
    CB_CHECK_EQ(lib->expr(binary("-", a, binary("-", b, d))), std::string("a - (b - d)"));
    CB_CHECK_EQ(lib->expr(binary("&&", binary("<", a, b), binary("==", b, d))), std::string("a < b && b == d"));
    const Json cast = Json{{"node", "Cast"}, {"type", Json{{"kind", "u16"}}}, {"x", binary("<", a, b)}};
    CB_CHECK_EQ(lib->expr(cast), std::string("(uint2)(a < b)"));
    const Json deref = Json{{"node", "Unary"}, {"op", "*"}, {"x", Json{{"node", "Cast"}, {"type", ptr(Json{{"kind", "i16"}}, "flat")}, {"x", lit(70)}}}};
    CB_CHECK_EQ(lib->expr(deref), std::string("*(int2 *)0x46"));

    const Json x = binary("!=", name("a"), lit(0)), y = binary("!=", name("b"), lit(0)), z = binary("!=", name("d"), lit(0));
    CB_CHECK_EQ(lib->condition(x), std::string("(a != 0)"));
    CB_CHECK_EQ(lib->condition(joined("&&", x, y)), std::string("((a != 0) && (b != 0))"));
    CB_CHECK_EQ(lib->condition(joined("&&", joined("&&", x, y), joined("||", x, z))), std::string("(((a != 0) && (b != 0)) && ((a != 0 || (d != 0))))"));
    CB_CHECK_EQ(lib->condition(joined("&&", x, y), true), std::string("(a != 0 && (b != 0))"));   // a loop's: comma separated
});

CB_TEST("translate.native_l2.function_text", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    const Json cond = binary("==", name("x"), lit(0));
    const Json body = Json::array(
        {parse(R"({"node":"Local","sym":{"kind":"local","name":"x"},"type":{"kind":"i16"}})"),
         Json{{"node", "If"}, {"cond", cond}, {"then", parse(R"({"node":"Goto","label":"LAB_1"})")}},
         Json{{"node", "If"}, {"cond", cond}, {"then", Json::array({Json{{"node", "Return"}, {"expr", lit(1)}}})},
              {"orelse", Json::array({Json{{"node", "If"}, {"cond", cond}, {"then", Json::array({Json{{"node", "Return"}, {"expr", lit(2)}}})}}})}},
         Json{{"node", "If"}, {"cond", cond}, {"then", Json::array()}, {"orelse", Json::array({parse(R"({"node":"Block","stmts":[]})")})}},
         Json{{"node", "Return"}, {"expr", lit(0)}}});
    const std::string text = lib->function(function_doc(body, "__stdcall16far"));
    CB_CHECK(text.starts_with("int2 __stdcall16far f(void)"));
    CB_CHECK(text.find("  int2 x;") != std::string::npos);
    CB_CHECK(text.find("  if (x == 0) goto LAB_1;") != std::string::npos);
    CB_CHECK(text.find("  else if (x == 0) {") != std::string::npos);
    CB_CHECK(text.find("  else {\n  }") != std::string::npos);
    // the heading as its own entry point says it
    CB_CHECK_EQ(lib->signature(function_doc(body, "__stdcall16far")), std::string("int2 __stdcall16far f(void)"));
    CB_CHECK_EQ(lib->type_text(parse(R"({"kind":"i16"})")), std::string("int2"));
    CB_CHECK_EQ(lib->type_text(parse(R"({"kind":"i16"})"), "x"), std::string("int2 x"));
});

CB_TEST("translate.native_l2.passes_moved_onto_the_tree", [] {
    // c_text's far and near-call passes done on the tree: a four-byte pointer is written __far where it is declared and in the
    // cast assigned to it; a call through a word is near
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    const Json word = Json{{"node", "Unary"}, {"op", "*"}, {"x", Json{{"node", "Cast"}, {"type", ptr(Json{{"kind", "class"}, {"name", "xunknown2"}}, "near")}, {"x", lit(0x18)}}}};
    const Json call = Json{{"node", "Call"},
                           {"target", Json{{"node", "Unary"}, {"op", "*"},
                                           {"x", Json{{"node", "Cast"}, {"type", Json{{"kind", "fnptr"}, {"space", "far"}, {"fn", Json{{"kind", "fn"}}}}}, {"x", word}}}}},
                           {"args", Json::array()}};
    const Json p = Json{{"node", "Name"}, {"sym", Json{{"kind", "local"}, {"name", "p"}}}, {"type", ptr(Json{{"kind", "i16"}})}};
    const Json q = Json{{"node", "Name"}, {"sym", Json{{"kind", "param"}, {"name", "q"}}}, {"type", ptr(Json{{"kind", "void"}})}};
    const Json set_p = Json{{"node", "Assign"}, {"target", p}, {"value", Json{{"node", "Cast"}, {"type", ptr(Json{{"kind", "i16"}}, "near")}, {"x", q}}}};
    const Json set_data = Json{{"node", "Assign"}, {"target", p}, {"value", Json{{"node", "Cast"}, {"type", ptr(Json{{"kind", "i16"}}, "near")}, {"x", lit(0x40)}}}};
    Json doc = parse(R"({"node":"MethodDecl","name":"f","kind":"function","ret":{"kind":"void"},"origin":{"module":"","segment":1,"offset":0}})");
    doc["params"] = Json::array({Json{{"node", "Param"}, {"sym", Json{{"kind", "param"}, {"name", "q"}}}, {"type", ptr(Json{{"kind", "void"}})}}});
    doc["body"] = Json::array({Json{{"node", "Local"}, {"sym", Json{{"kind", "local"}, {"name", "p"}}}, {"type", ptr(Json{{"kind", "i16"}})}},
                               Json{{"node", "ExprStmt"}, {"expr", set_p}}, Json{{"node", "ExprStmt"}, {"expr", set_data}},
                               Json{{"node", "ExprStmt"}, {"expr", call}}});
    const std::string plain = lib->function(doc);
    const std::string moved = lib->function(doc, moved_all());
    CB_CHECK(plain.find("__far") == std::string::npos && plain.find("__near") == std::string::npos);   // (as the decompiler prints it)
    CB_CHECK(moved.find("void __far *q") != std::string::npos);
    CB_CHECK(moved.find("  int2 __far *p;") != std::string::npos);
    CB_CHECK(moved.find("p = (int2 __far *)q;") != std::string::npos);
    // (a data address: DS and it)
    CB_CHECK(moved.find("p = (int2 __far *)MK_FP(codebreaker_ds(), 0x40);") != std::string::npos);
    CB_CHECK(moved.find("(*(code __near *)*(xunknown2 *)0x18)();") != std::string::npos);
    // the notes' far set, when given, rules
    const std::string only_p = lib->function(doc, moved_all(), std::set<std::string>{"p"});
    CB_CHECK(only_p.find("void *q") != std::string::npos);
    // a 32-bit function: no segments, nothing moves
    Json flat = doc;
    flat["origin"] = parse(R"({"module":"","va":4198400})");
    CB_CHECK(lib->function(flat, moved_all()).find("__far") == std::string::npos);
});

CB_TEST("translate.native_l2.facts_come_back", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    Json facts = Json::object();
    const Json doc = function_doc(Json::array({Json{{"node", "Return"}, {"expr", lit(0)}}}));
    const std::string text = lib->function(doc, {}, std::nullopt, &facts);
    CB_CHECK(text.starts_with("int2"));
    CB_CHECK(facts.is_object());
    // asked for again with the facts it gave: the same text
    CB_CHECK_EQ(lib->function(doc, {}, std::nullopt, &facts), text);
});

CB_TEST("translate.native_l2.declines_and_errors", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    // a node the back end has no C for is a decline the caller catches, not a failure
    const Json lambda = parse(R"({"node":"Lambda","params":[],"body":{"node":"Lit","value":1}})");
    bool declined = false;
    try {
        (void)lib->expr(lambda);
    } catch (const nl::Declined& e) {
        declined = true;
        CB_CHECK_EQ(e.type(), std::string("Decline"));
    }
    CB_CHECK(declined);
    // a document with no "node": the library's own word for it
    bool failed = false;
    try {
        (void)lib->expr(parse(R"({"line":1})"));
    } catch (const nl::LibraryError& e) {
        failed = true;
        CB_CHECK(!e.type().empty());
    }
    CB_CHECK(failed);
    CB_CHECK_THROWS_AS(nl::LibraryError, lib->call("no_such_entry", Json::array()));
});

CB_TEST("translate.native_l2.small_entries", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    // the arguments a 32-bit definition takes (none known for a __cdecl one)
    CB_CHECK_EQ(lib->takes32("int4 CopyLZFile(int4 a1,int4 a2)").value_or(-1), std::int64_t(2));
    CB_CHECK_EQ(lib->takes32("int4 f(void)").value_or(-1), std::int64_t(0));
    CB_CHECK(!lib->takes32("int4 __cdecl LZDone(void)"));
    // the words an argument takes on the stack
    CB_CHECK_EQ(lib->argument_words("(uint4)x", Json::object()).value_or(-1), std::int64_t(2));
    CB_CHECK_EQ(lib->argument_words("&x", Json::object()).value_or(-1), std::int64_t(1));
    // the arguments of a call as written
    const nl::ArgumentList args = lib->arguments_at("*MK_FP(iVar2,piVar1 + 1)", 6);
    CB_CHECK((args.arguments == std::vector<std::string>{"iVar2", "piVar1 + 1"}));
    CB_CHECK_EQ(args.end, std::int64_t(24));
});

CB_TEST("translate.native_l2.round_trip_as_the_typed_tree_does", [] {
    // The library's reader and writer and the typed tree's agree on what a document says: through each, the same document.
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    const Json docs = parse(R"([
      {"node":"MethodDecl","name":"sub_1_0000","kind":"function","origin":{"module":"m.dll","segment":1,"offset":0},
       "sym":{"kind":"function","name":"sub_1_0000","static":true,"type":{"kind":"fn","conv":"stdcall","distance":"far","pops":4,
         "params":[{"name":"a1","type":{"kind":"ptr","space":"far","to":{"kind":"u8"}},"at":{"stack":6}}],
         "result":{"type":{"kind":"i16"},"at":{"reg":"AX"}}}},
       "ret":{"kind":"i16"},"mods":["__stdcall16far"],
       "params":[{"node":"Param","sym":{"kind":"param","name":"a1"},"type":{"kind":"ptr","space":"far","to":{"kind":"u8"}}}],
       "body":[{"node":"Local","sym":{"kind":"local","name":"axStack_c"},"type":{"kind":"array","count":8,"elem":{"kind":"class","name":"xunknown1"}}},
               {"node":"If","cond":{"node":"Binary","op":"==","signed":true,"left":{"node":"Name","sym":{"kind":"param","name":"a1"}},
                 "right":{"node":"Lit","value":0,"text":"0x0"}},"then":{"node":"Goto","label":"LAB_1"}},
               {"node":"Labeled","label":"LAB_1","body":[]},
               {"node":"Return","expr":{"node":"Lit","value":1.5}}],
       "comments":["WARNING: a note"]},
      {"node":"Unit","path":"a.c","lang":"c","types":[{"node":"ClassDecl","name":"S","kind":"struct",
         "layout":{"kind":"record","size":8,"name":"S","fields":[{"name":"a","offset":0,"type":{"kind":"i32"}}]},"members":[]}]},
      {"node":"Module","units":[{"node":"MachineFragment","name":"g","syntax":"wasm","asm":"ret"}]},
      {"node":"Lit","value":"it's \u00e9 \ud83d\ude00"}
    ])");
    for (const Json& doc : docs) {
        const std::string through_library = lib->roundtrip_text(doc);
        cb::Arena arena;
        const Json through_tree = cb::translate::l2::to_json(*cb::translate::l2::from_json(arena, doc));
        CB_CHECK(normalised(Json::parse(through_library)) == normalised(through_tree));
    }
});

CB_TEST("translate.native_l2.schema_check", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    const std::string schema = cb::fs::read_text(cb::testing::data_path("../../codebreaker/translate/l2.schema.json"));
    const nl::Validation ok = lib->validate(parse(R"({"node":"Return","expr":{"node":"Lit","type":{"kind":"i32"},"value":1}})"), schema);
    CB_CHECK(ok.valid);
    // a node the schema does not know, a slot it does not have, a type of a kind it has not
    CB_CHECK(!lib->validate(parse(R"({"node":"Flux"})"), schema).valid);
    const nl::Validation extra = lib->validate(parse(R"({"node":"Lit","value":1,"junk":true})"), schema);
    CB_CHECK(!extra.valid);
    CB_CHECK(!extra.why.empty());
    CB_CHECK(!lib->validate(parse(R"({"node":"Lit","type":{"kind":"quantum"},"value":1})"), schema).valid);
    // told the schema once, asked again
    CB_CHECK(lib->validate(parse(R"({"node":"Break"})"), schema).valid);
});

CB_TEST("translate.native_l2.module_entries", [] {
    const nl::Library* lib = library();
    if (lib == nullptr) return;
    // a Module: its functions as C (one that is still text is a comment saying so), and what stayed assembly
    const Json fn = function_doc(Json::array({Json{{"node", "Return"}, {"expr", lit(0)}}}));
    const Json doc = Json{{"node", "Module"},
                          {"units", Json::array({fn, parse(R"({"node":"MachineFragment","name":"g","syntax":"masm","asm":"mov ax, 1"})"),
                                                 parse(R"({"node":"MethodDecl","name":"h","kind":"function","text":"int h(void) { return 1; }"})")})}};
    const nl::ModuleText module = lib->module(doc, {});
    CB_CHECK(module.text.find("int2 f(void)") != std::string::npos);
    CB_CHECK(module.text.find("  return 0;") != std::string::npos);
    CB_CHECK(module.text.find("/* g: kept as its instructions") != std::string::npos);
    CB_CHECK(module.text.find("/* h: not in L2") != std::string::npos);
    CB_CHECK_EQ(module.assembly.size(), std::size_t(1));
    CB_CHECK_EQ(module.assembly.at("g"), std::string("mov ax, 1"));
    // a fragment as the C of its module holds it
    CB_CHECK_EQ(lib->fragment(parse(R"({"node":"MachineFragment","name":"g","syntax":"masm","asm":"mov ax, 1"})")),
                std::string("/* g: kept as its instructions --  */\n"));
});

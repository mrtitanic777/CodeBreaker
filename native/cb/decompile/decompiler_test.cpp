// Tests of cb/decompile/decompiler.hpp. The native program is stood in for by cb_fake_decompiler.exe (decompile/fake_decompiler_main.cpp), which
// reads the description as the real one does and writes the blocks, notes and bodies the real one writes -- and, when told to, dies on a
// function, fails on one, or writes the notes. The L2 library is stood in for by a function that answers as the real one does (a text and the
// facts it found; a decline; an error). What is asked of the rest of CodeBreaker (`Environment`) is answered with nothing.
//
// What the tests cover: the blocks and notes of the program's output; the facts as the library takes them and what it leaves in them; a run
// (kept by what it was made from, made again only for the functions a changed description touches, in chunks of 64, resumed after a death,
// the environment it is given); a function's C from its L2 document (written, declined, an error that is not a decline); what a 16-bit
// function's L2 document says (placeholders, calls, arguments); a DOS program and a NES cartridge made C through the whole path, and the
// command. The 16-bit and 32-bit lanes are proved against the product (the six modules of the report in the commit message).
//
// Notes: the `timeout=` of the runs is gone (the process API has one for a run that needs it; nothing here did);
// a note of the program's that is not well formed is a FormatError;
// the asm-blocks pass is run through asm_blocks::place with its own decoder; a function
// that dies with the governor's note and no block written before it is skipped.
#include "cb/decompile/decompiler.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <nlohmann/json.hpp>

#include "cb/base/converted.hpp"
#include "cb/base/corpus.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/decompiler_internal.hpp"
#include "cb/translate/l2.hpp"


namespace dc = cb::decompile::decompiler;
namespace dsc = cb::decompile::describe;
namespace ct = cb::decompile::c_text;
namespace nl = cb::translate::native_l2;
namespace stdfs = std::filesystem;
using cb::Json;
using std::string;
using std::vector;

namespace {

// ---- the L2 library, stood in for ---------------------------------------------------------------------------------------------

// A document that stands for a function; its name says what the library does with it.
string dt_document(const string& name, int params = 0) {
    Json doc = Json::parse(R"({"node":"MethodDecl","kind":"function","ret":{"kind":"i16"},"params":[],"body":[]})");
    doc["name"] = name;
    doc["origin"] = Json{{"module", "m.dll"}, {"segment", 1}, {"offset", 0}};
    for (int i = 0; i < params; ++i)
        doc["params"].push_back(Json{{"node", "Param"}, {"sym", Json{{"kind", "param"}, {"name", "a" + std::to_string(i + 1)}}},
                                     {"type", Json{{"kind", i == 1 ? "i32" : "i16"}}}});
    return doc.dump();
}

int dt_answer(int status, const Json& answer, char** out, std::size_t* out_length) {
    const string text = answer.dump();
    *out = static_cast<char*>(std::malloc(text.size() + 1));
    std::memcpy(*out, text.c_str(), text.size() + 1);
    *out_length = text.size();
    return status;
}

int dt_call(const char* name, const char* arguments, std::size_t length, char** out, std::size_t* out_length) {
    const Json args = Json::parse(string(arguments, length));
    const string fn = name;
    if (fn == "function") {
        const string n = args.at(0).value("name", "");
        if (n == "declined") return dt_answer(1, Json{{"type", "Decline"}, {"message", "no"}}, out, out_length);
        if (n == "keyerror")
            return dt_answer(1, Json{{"type", "KeyError"}, {"message", "k"}, {"facts", Json{{"declared", "int partial(void)"}}}}, out, out_length);
        if (n == "broken") return dt_answer(1, Json{{"type", "IndexError"}, {"message", "i"}}, out, out_length);
        Json facts = args.at(3).is_object() ? args.at(3) : Json::object();
        facts["declared"] = "int " + n + "(void)";
        facts["registers"] = "ecx";
        facts["structs_used"] = Json{{"$s", Json::array({"struct_1"})}};
        facts["novalue"] = true;
        facts["interrupts"] = Json{{"$s", Json::array({Json{{"$t", Json::array({0x21, Json{{"$t", Json::array({"ax", "bx"})}}})}}})}};
        return dt_answer(0, Json::array({"int " + n + "(void) { }", facts}), out, out_length);
    }
    if (fn == "type_text") {
        const Json& type = args.at(0);
        const string kind = type.is_object() ? type.value("kind", "") : "";
        const string var = args.at(1).get<string>();
        const string base = kind == "i32" ? "int4" : kind == "ptr" ? "char __far *" : "int2";
        return dt_answer(0, base + " " + var, out, out_length);
    }
    return dt_answer(1, Json{{"type", "KeyError"}, {"message", "no such entry point " + fn}}, out, out_length);
}
void dt_free(char* p) { std::free(p); }

nl::Library& dt_library() {
    static nl::Library lib(&dt_call, &dt_free);
    return lib;
}

class Nothing final : public dc::Environment {
public:
    dsc::Knowledge& knowledge() override { return knowledge_; }
    const nl::Library& library() override { return dt_library(); }
    std::map<string, string> import_dlls(const stdfs::path&) override { return {}; }
    std::optional<std::uint32_t> argument_bytes(std::string_view, std::string_view) override { return std::nullopt; }
    std::optional<std::vector<cb::decompile::struct_text::Structure>> recover_structures(const stdfs::path&) override { return std::nullopt; }
    dsc::PowerPcFacts powerpc_facts(const stdfs::path&) override { return {}; }
    vector<dc::Va> powerpc_functions(const stdfs::path&) override { return {}; }

private:
    dsc::NoKnowledge knowledge_;
};

stdfs::path dt_fake_program() {
    return cb::fs::executable_path().parent_path() / "cb_fake_decompiler.exe";
}

// A folder, a context over it, a decompiler over that.
struct Fixture {
    cb::fs::ScopedTempDir dir{"cb-decompiler-test"};
    cb::base::cache::Cache cache;
    Nothing environment;
    dc::Decompiler decompiler;
    stdfs::path description;
    stdfs::path log;

    static cb::base::cache::Config cache_config(const stdfs::path& root) {
        cb::base::cache::Config c;
        c.state_dir = root / "cache";
        c.shared_dir = root / "shared";
        return c;
    }
    static dc::Context context(const stdfs::path& root, cb::base::cache::Cache& cache, Nothing& environment, const dc::Policy& policy,
                               const string& threads) {
        dc::Context c;
        c.settings.checkout = stdfs::path(CB_SOURCE_DIR).parent_path();
        c.settings.program = dt_fake_program();
        c.settings.languages = root / "languages";
        stdfs::create_directories(c.settings.languages);
        cb::fs::write_text(c.settings.languages / "x86.ldefs", "languages");
        c.settings.struct_recovery = "all";
        c.settings.threads = threads;
        c.policy = policy;
        c.cache = &cache;
        c.environment = &environment;
        c.limits.workers.workers = 2;
        c.limits.min_free_gb = 0.0;
        c.limits.slot_dir = root / "slots";
        c.scratch = root / "scratch";
        return c;
    }

    explicit Fixture(const dc::Policy& policy = {}, const string& threads = {})
        : cache(cache_config(dir.path())), decompiler(context(dir.path(), cache, environment, policy, threads)) {
        description = dir.path() / "description.txt";
        log = dir.path() / "fake.log";
        cb::fs::write_text(log, "");
        cb::fs::set_env("FAKE_DECOMPILER_LOG", cb::fs::to_text(log));
    }
    ~Fixture() {
        for (const char* name : {"FAKE_DECOMPILER_LOG", "FAKE_DECOMPILER_DIE", "FAKE_DECOMPILER_NOTES", "FAKE_DECOMPILER_L2", "FAKE_DECOMPILER_FAIL"})
            cb::fs::set_env(name, std::nullopt);
    }

    // The description of functions: `F <hex> <name> -1 <extra>` each, and an import and a call-through-pointer line.
    void describe(const vector<std::pair<dc::Va, string>>& functions, const string& extra = "") {
        string text = "arch\tx86:LE:32:default:windows\nX\t7f000000\tImported\t-1\n";
        for (const auto& [va, name] : functions) text += "F\t" + cb::decompile::decompiler::internal::hex(va) + "\t" + name + "\t-1" + extra + "\n";
        cb::fs::write_text(description, text);
    }
    vector<string> runs() const {
        vector<string> out;
        const string content = cb::fs::read_text(log);
        for (const std::string_view line : cb::text::lines(content)) out.emplace_back(line);
        return out;
    }
};

bool dt_has(const string& text, const string& part) { return text.find(part) != string::npos; }

string dt_names(const vector<string>& v) { return cb::text::join(v, ","); }

// ---- the blocks ---------------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.decompiler.scan_blocks", [] {
    const string out =
        "/*@@ alpha 401000 */\nint alpha(void)\n{\n}\n"
        "/*@@ beta 401010 */\nint beta(void)\n{\n}\n"
        "/*@@ not a block */\n"                 // (not a header: it stays part of beta)
        "/*@@ gamma 4010x0 */\ntext\n"           // (nor is this: not hexadecimal)
        "/*@@ delta ff */\nlast";
    const vector<dc::Block> blocks = dc::scan_blocks(out);
    CB_CHECK_EQ(blocks.size(), std::size_t{3});
    CB_CHECK_EQ(blocks[0].name, "alpha");
    CB_CHECK_EQ(blocks[0].va, 0x401000);
    CB_CHECK_EQ(out.substr(blocks[0].body, blocks[0].end - blocks[0].body), "int alpha(void)\n{\n}\n");
    CB_CHECK_EQ(blocks[1].name, "beta");
    // (any line that starts `/*@@ ` ends the block before it, whether or not it is a header itself: its text belongs to no block)
    CB_CHECK_EQ(out.substr(blocks[1].body, blocks[1].end - blocks[1].body), "int beta(void)\n{\n}\n");
    CB_CHECK_EQ(blocks[2].name, "delta");
    CB_CHECK_EQ(blocks[2].va, 0xff);
    CB_CHECK_EQ(blocks[2].end, out.size());
    CB_CHECK(dc::scan_blocks("").empty());
    CB_CHECK(dc::scan_blocks("/*@@ cut 10 */").empty());   // (a header the output ends in has no body: the program was cut off)
});

// ---- the notes ----------------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.decompiler.split_notes", [] {
    cb::Arena arena;
    const string doc = dt_document("f");
    const dc::Split s = dc::split_notes(arena,
                                        "/*@G 401000 4 4 */\n/*@G 402000 2 */\n/*@P pv 4 */\n/*@P pv 2 */\n/*@S struct_1 8 0:4:int4 4:4:char* */\n/*@L2 " + doc + " */\nint f(void)\n{\n}\n");
    CB_CHECK_EQ(s.text, "int f(void)\n{\n}\n");
    CB_CHECK_EQ(s.notes.globals.size(), std::size_t{2});
    CB_CHECK_EQ(s.notes.globals[0].address, 0x401000);
    CB_CHECK_EQ(s.notes.globals[0].bytes, 4);
    CB_CHECK_EQ(s.notes.globals[0].elem, 4);
    CB_CHECK_EQ(s.notes.globals[1].elem, 0);   // (no width given for what it points at)
    CB_CHECK_EQ(s.notes.pointers.size(), std::size_t{1});
    CB_CHECK_EQ(s.notes.pointers.at("pv"), 4);   // (the first says: a later one does not change it)
    CB_CHECK_EQ(s.notes.structs.size(), std::size_t{1});
    CB_CHECK_EQ(s.notes.structs[0].name, "struct_1");
    CB_CHECK_EQ(s.notes.structs[0].bytes, 8);
    CB_CHECK_EQ(s.notes.structs[0].fields.size(), std::size_t{2});
    CB_CHECK_EQ(s.notes.structs[0].fields[1].offset, 4);
    CB_CHECK_EQ(s.notes.structs[0].fields[1].type, "char*");
    CB_CHECK(s.notes.l2 != nullptr);
    CB_CHECK(!s.notes.l2_exact_json);
    CB_CHECK_EQ(cb::translate::l2::to_json(*s.notes.l2).at("name").get<string>(), "f");

    // nothing before the C: the text is as it was; notes are only on the lines before it
    const dc::Split plain = dc::split_notes(arena, "int g(void)\n{\n  /*@G 1 2 3 */\n}\n");
    CB_CHECK_EQ(plain.text, "int g(void)\n{\n  /*@G 1 2 3 */\n}\n");
    CB_CHECK(plain.notes.globals.empty());
    CB_CHECK_EQ(dc::split_notes(arena, "/*@G 401000 4 4 */\n").text, "");
    CB_CHECK_EQ(dc::split_notes(arena, "/*@G 401000 4 4 */").text, "");

    // a document with an integer of 2^63 or more is kept as the program wrote it
    string big = dt_document("f");
    big.insert(big.size() - 1, R"(,"extra":18446744073709551615)");
    const dc::Split exact = dc::split_notes(arena, "/*@L2 " + big + " */\nx");
    CB_CHECK(exact.notes.l2_exact_json.has_value());
    CB_CHECK(dt_has(*exact.notes.l2_exact_json, "18446744073709551615"));

    // damaged notes are format errors
    CB_CHECK_THROWS_AS(cb::FormatError, dc::split_notes(arena, "/*@S struct_1 8 0-4-int4 */\nx"));
    CB_CHECK_THROWS_AS(cb::FormatError, dc::split_notes(arena, "/*@G zz 4 4 */\nx"));
    CB_CHECK_THROWS_AS(cb::FormatError, dc::split_notes(arena, "/*@G 401000 */\nx"));
    CB_CHECK_THROWS_AS(cb::ParseError, dc::split_notes(arena, "/*@L2 {not json */\nx"));
});

// ---- the facts the back end is given, and what it leaves in them --------------------------------------------------------------------

CB_TEST("decompile.decompiler.facts_json", [] {
    using cb::decompile::decompiler::internal::facts_json;
    ct::ModuleFacts flat;
    flat.path = ct::ModuleFacts::Path::Flat32;
    flat.functions = {"b", "a"};
    flat.struct_sizes = {{"struct_1", 8}};
    flat.called_through = {"pfn"};
    flat.aliases = {{"iRam1", "iRam1_2"}};
    flat.takes = {{"f", 2}};
    cb::decompile::struct_text::PlannedParam planned;
    planned.structure = "struct_1";
    planned.fields = {{4, {"int4", 4}}, {0, {"char", 1}}};
    flat.struct_fields = std::map<string, cb::decompile::struct_text::PlannedParam>{{"a1", planned}};
    flat.plans.insert_or_assign("frame", R"({"a":1})");
    const Json j = facts_json(flat);
    CB_CHECK_EQ(j.at("functions").dump(), R"({"$s":["a","b"]})");
    CB_CHECK_EQ(j.at("struct_sizes").dump(), R"({"struct_1":8})");
    CB_CHECK_EQ(j.at("takes").dump(), R"({"f":2})");
    CB_CHECK_EQ(j.at("without_notes").dump(), "false");
    // a structure's fields: the offsets are not text, so the dict is tagged
    CB_CHECK_EQ(j.at("struct_fields").dump(), R"({"a1":{"$t":["struct_1",{"$d":[[0,{"$t":["char",1]}],[4,{"$t":["int4",4]}]]}]}})");
    CB_CHECK_EQ(j.at("frame").dump(), R"({"a":1})");   // (the plans the module carries are given back to the back end)

    ct::ModuleFacts win16;
    win16.path = ct::ModuleFacts::Path::Win16;
    win16.distance = "far";
    win16.selector = 0x1000;
    win16.have_code = true;
    win16.widths_far = {{"a1", 4}};
    win16.loads = {6, 4};
    win16.ss_stores = {-2};
    win16.selector_values = {{0x2000, {"KERNEL_91", dsc::SelectorUse::Offset}}};
    const Json w = facts_json(win16);
    CB_CHECK_EQ(w.at("distance").get<string>(), "far");
    CB_CHECK(w.at("model").is_null());
    CB_CHECK_EQ(w.at("selector").get<int>(), 0x1000);
    CB_CHECK_EQ(w.at("widths").dump(), R"({"$d":[[true,{"a1":4}],[false,{}]]})");
    CB_CHECK_EQ(w.at("loads").dump(), R"({"$s":[4,6]})");
    CB_CHECK_EQ(w.at("ss_stores").dump(), "[-2]");
    CB_CHECK_EQ(w.at("selector_values").dump(), R"({"$d":[[8192,{"$t":["KERNEL_91","offset"]}]]})");
    win16.have_code = false;
    CB_CHECK_EQ(facts_json(win16).at("widths").dump(), "{}");

    ct::ModuleFacts ppc;
    ppc.path = ct::ModuleFacts::Path::PowerPc;
    ppc.functions = {"main"};
    ppc.plans.insert_or_assign("frame", "{}");
    const Json p = facts_json(ppc);
    CB_CHECK_EQ(p.dump(), R"({"lane":"powerpc","functions":{"$s":["main"]}})");   // (and no plans: the path has none)
});

CB_TEST("decompile.decompiler.facts_text", [] {
    using cb::decompile::decompiler::internal::facts_json;
    using cb::decompile::decompiler::internal::facts_text;
    ct::ModuleFacts own;   // a 32-bit module's facts, each held in the function's own
    own.path = ct::ModuleFacts::Path::Flat32;
    own.functions = {"g", "h \"q\" caf\xC3\xA9"};
    own.called_through = {"cRam00403020"};
    own.pointer_globals = {"pcRam00403010"};
    own.takes = {{"g", 2}};
    own.struct_sizes = {{"struct_1", 12}};
    own.aliases = {{"uRam00403000", "uRam00403000_1"}};
    own.without_notes = true;
    own.plans.insert_or_assign("frame", R"({"a": [1, 2]})");
    CB_CHECK_EQ(facts_text(own), facts_json(own).dump(-1, ' ', true));
    // the same facts with the module-wide parts shared: the same text, the second time from what the first wrote
    auto shared = std::make_shared<ct::FlatShared>();
    shared->functions = own.functions;
    shared->called_through = own.called_through;
    shared->pointer_globals = own.pointer_globals;
    shared->takes = own.takes;
    ct::ModuleFacts held = own;
    held.functions.clear();
    held.called_through.clear();
    held.pointer_globals.clear();
    held.takes.clear();
    held.shared = shared;
    CB_CHECK_EQ(facts_text(held), facts_text(own));
    held.struct_sizes["struct_2"] = 4;
    own.struct_sizes["struct_2"] = 4;
    CB_CHECK_EQ(facts_text(held), facts_text(own));
    CB_CHECK_EQ(facts_json(held).dump(), facts_json(own).dump());
});

CB_TEST("decompile.decompiler.apply_left", [] {
    using cb::decompile::decompiler::internal::apply_left;
    const Json left = Json::parse(R"json({
      "declared": "int f(int a1)", "registers": "edx", "structs_used": {"$s": ["struct_2", "struct_1"]}, "novalue": true,
      "interrupts": {"$s": [{"$t": [33, {"$t": ["ax", "bx"]}]}, {"$t": [16, []]}]},
      "selector_decls": ["extern char x;"],
      "selector_shared": {"struct_1": {"f": {"$d": [[0, "int2"], [2, "int4"]]}}},
      "selector_imports": {"KERNEL_91": "segment"},
      "names": {"called": {"$s": ["g"]}, "called_through": {"$s": []}, "globals": {"$s": [{"$t": ["i", "00401000"]}]}, "segments": {"$s": [1]}, "mk_fp": true},
      "heading": {"params": {"$t": ["int a1", [{"$t": ["a1", "int"]}]]}, "types": {"int": "int4"}, "calls": [{"$t": ["g", ["a1", "0"]]}],
                  "declaration": "int f();", "fastcall": false},
      "frame": {"x": {"$d": [[1, 2]]}}, "code_reads": {"y": 1}
    })json");
    ct::ModuleFacts m;
    m.selector_decls = {"old"};   // (the back end says this function's declarations afresh)
    m.structs_used = {"old"};
    m.plans.insert_or_assign("segment_values", "{}");
    apply_left(m, left);
    CB_CHECK_EQ(*m.declared, "int f(int a1)");
    CB_CHECK_EQ(*m.registers, "edx");
    CB_CHECK(m.structs_used == (std::set<string>{"struct_1", "struct_2"}));
    CB_CHECK(m.novalue);
    CB_CHECK_EQ(m.interrupts.size(), std::size_t{2});
    CB_CHECK_EQ(m.interrupts.rbegin()->vector, 33u);
    CB_CHECK_EQ(dt_names(m.interrupts.rbegin()->registers), "ax,bx");
    CB_CHECK_EQ(dt_names(m.selector_decls), "extern char x;");
    CB_CHECK_EQ(m.selector_shared.at("struct_1").at("f").at(2), "int4");
    CB_CHECK_EQ(m.selector_imports.at("KERNEL_91"), "segment");
    CB_CHECK(m.names.has_value());
    CB_CHECK(m.names->called == (std::set<string>{"g"}));
    CB_CHECK(m.names->globals.contains({"i", "00401000"}));
    CB_CHECK(m.names->segments.contains(1));
    CB_CHECK(m.names->mk_fp);
    CB_CHECK(m.heading.has_value());
    CB_CHECK_EQ(m.heading->params->text, "int a1");
    CB_CHECK_EQ(m.heading->params->list.at(0).second, "int");
    CB_CHECK_EQ(m.heading->types.at("int"), "int4");
    CB_CHECK_EQ(m.heading->calls.at(0).second.size(), std::size_t{2});
    CB_CHECK_EQ(m.heading->declaration, "int f();");
    CB_CHECK(!m.heading->fastcall);
    CB_CHECK_EQ(m.plans.at("frame"), R"({"x":{"$d":[[1,2]]}})");
    CB_CHECK_EQ(m.plans.at("code_reads"), R"({"y":1})");
    CB_CHECK(m.plans.contains("segment_values"));   // (the module's own plans stay)

    // what it did not say is not said: empty answers leave the facts as they were
    ct::ModuleFacts n;
    n.declared = "kept";
    n.registers = "ecx";
    n.names = ct::NamesUsed{{"old"}, {}, {}, {}, false};
    apply_left(n, Json::parse(R"({"registers": null, "names": {}, "structs_used": {"$s": []}})"));
    CB_CHECK_EQ(*n.declared, "kept");
    CB_CHECK_EQ(*n.registers, "ecx");
    CB_CHECK(n.names.has_value() && n.names->called.contains("old"));
    CB_CHECK(n.structs_used.empty());
    CB_CHECK(!n.novalue);
});

// ---- what a 16-bit function's L2 document says --------------------------------------------------------------------------------------

namespace {

// A function document with these calls and names in its body: `calls` {callee, number of arguments}, `uses` names read.
cb::translate::ir::Node* dt_function(cb::Arena& arena, const vector<std::pair<string, int>>& calls, const vector<string>& uses = {}) {
    Json body = Json::array();
    for (const auto& [callee, n] : calls) {
        Json args = Json::array();
        for (int i = 0; i < n; ++i) args.push_back(Json{{"node", "Lit"}, {"value", i}});
        body.push_back(Json{{"node", "ExprStmt"},
                            {"expr", Json{{"node", "Call"}, {"target", Json{{"node", "Name"}, {"sym", Json{{"kind", "function"}, {"name", callee}}}}}, {"args", args}}}});
    }
    for (const string& u : uses) body.push_back(Json{{"node", "ExprStmt"}, {"expr", Json{{"node", "Name"}, {"sym", Json{{"kind", "local"}, {"name", u}}}}}});
    Json doc = Json::parse(dt_document("caller", 2));
    doc["body"] = body;
    return cb::translate::l2::from_json(arena, doc);
}

}  // namespace

CB_TEST("decompile.decompiler.placeholders", [] {
    cb::Arena arena;
    cb::translate::ir::Node* doc = dt_function(arena, {{"func_0x10000100", 2}, {"printf", 1}, {"func_0x10000100", 3}, {"xfunc_0x10000200", 0}, {"func_0x1000030", 0}});
    CB_CHECK(dc::placeholders("", doc) == (std::set<dc::Va>{0x10000100}));   // (only a name that is exactly func_0x<8 hex> is a place not yet known)
    // without a document, its text: a call, the name standing as a word
    CB_CHECK(dc::placeholders("x = func_0x10000100(1);\ny = func_0x10000200 (2);\nz = func_0x10000300;\nw = nfunc_0x10000400();\nv = func_0x100005000();",
                              nullptr) == (std::set<dc::Va>{0x10000100, 0x10000200}));
    CB_CHECK(dc::placeholders("nothing", nullptr).empty());
    CB_CHECK(dc::placeholders("nothing", dt_function(arena, {{"printf", 1}})).empty());
});

CB_TEST("decompile.decompiler.calls_and_names", [] {
    cb::Arena arena;
    cb::translate::ir::Node* doc = dt_function(arena, {{"sub_1_0010", 2}, {"sub_1_0010", 0}, {"other", 1}}, {"func_0x10000100", "local"});
    CB_CHECK(dc::calls("", doc, "sub_1_0010"));
    CB_CHECK(!dc::calls("", doc, "func_0x10000100"));   // (named, not called)
    CB_CHECK(dc::mentions("", doc, "func_0x10000100"));
    CB_CHECK(dc::mentions("", doc, "sub_1_0010"));        // (a callee's name is a name)
    CB_CHECK(!dc::mentions("", doc, "nowhere"));
    const vector<std::int64_t> arguments = dc::call_arguments("", "sub_1_0010", doc);
    CB_CHECK_EQ(arguments.size(), std::size_t{2});
    CB_CHECK_EQ(arguments[0], 2);
    CB_CHECK_EQ(arguments[1], 0);
    CB_CHECK(dc::call_arguments("", "nowhere", doc).empty());

    // from the text where there is no document
    const string text = "void f(void)\n{\n  g(1, h(2, 3), a[1, 2]);\n  g();\n  g ( x );\n  gg(1);\n  xg(1);\n}\n";
    CB_CHECK(dc::calls(text, nullptr, "g"));
    CB_CHECK(!dc::calls(text, nullptr, "h2"));
    CB_CHECK(dc::mentions(text, nullptr, "gg"));
    const vector<std::int64_t> from_text = dc::call_arguments(text, "g", nullptr);
    CB_CHECK_EQ(from_text.size(), std::size_t{3});
    CB_CHECK_EQ(from_text[0], 3);   // (g(1, h(2, 3), a[1, 2]): commas inside the brackets are not its)
    CB_CHECK_EQ(from_text[1], 0);   // (g(): none)
    CB_CHECK_EQ(from_text[2], 1);
    CB_CHECK_EQ(dc::call_arguments(text, "h", nullptr).at(0), 2);
});

CB_TEST("decompile.decompiler.own_arguments", [] {
    cb::Arena arena;
    // from the document: the parameters' types as the library writes them, and a far pointer by the notes
    cb::translate::ir::Node* doc = dt_function(arena, {});   // (two parameters: an int2 and an int4)
    cb::OrderedMap<string, std::int64_t> pointers;
    const std::optional<dc::ArgumentSizes> sizes = dc::own_arguments(dt_library(), "", "caller", doc, &pointers);
    CB_CHECK(sizes.has_value());
    CB_CHECK_EQ(dt_names(sizes->sizes), "2,4");
    pointers.insert_or_assign("a1", 4);   // a1 holds a far pointer
    CB_CHECK_EQ(dt_names(dc::own_arguments(dt_library(), "", "caller", doc, &pointers)->sizes), "4p,4");

    // from the text: the declaration before the first brace
    const string text = "int2 __cdecl16far sub_1_0000(int2 a1, char __far *a2, uint4 a3, int2 a4)\n{\n  return sub_1_0000(1);\n}\n";
    const std::optional<dc::ArgumentSizes> from_text = dc::own_arguments(dt_library(), text, "sub_1_0000", nullptr, nullptr);
    CB_CHECK(from_text.has_value());
    CB_CHECK_EQ(dt_names(from_text->sizes), "2,4p,4,2");
    CB_CHECK(dc::own_arguments(dt_library(), "int g(void)\n{\n}\n", "sub_1_0000", nullptr, nullptr) == std::nullopt);
    CB_CHECK_EQ(dc::own_arguments(dt_library(), "int g(void)\n{\n}\n", "g", nullptr, nullptr)->sizes.size(), std::size_t{0});   // (void: none)
    CB_CHECK(dc::own_arguments(dt_library(), text, "sub_1_0000", nullptr, nullptr) == from_text);
});

CB_TEST("decompile.decompiler.by_their_names", [] {
    const std::map<string, string> known{{"10000100", "sub_1_0100"}};
    CB_CHECK_EQ(dc::by_their_names("a(func_0x10000100); b(func_0x10000200); c(func_0x100001000); xfunc_0x10000100", known),
                "a(sub_1_0100); b(func_0x10000200); c(func_0x100001000); xfunc_0x10000100");
    CB_CHECK_EQ(dc::by_their_names("no placeholders", known), "no placeholders");
    CB_CHECK_EQ(dc::by_their_names("func_0x10000100", {}), "func_0x10000100");
});

// ---- running the native program -------------------------------------------------------------------------------------------------------

CB_TEST("decompile.decompiler.run", [] {
    Fixture f;
    f.describe({{0x401010, "beta"}, {0x401000, "alpha"}});
    const dc::Run r = f.decompiler.run(f.description);
    CB_CHECK_EQ(dt_names(r.order), "alpha,beta");   // (by address)
    CB_CHECK_EQ(r.by_va.at(0x401010), "beta");
    CB_CHECK_EQ(r.texts.size(), std::size_t{2});
    CB_CHECK_EQ(r.texts.at("alpha"), "int alpha(void)\n{\n  return " + std::to_string(0x401000) + ";\n}");   // (the text is trimmed)
    CB_CHECK(r.notes.at("alpha").l2 == nullptr);
    CB_CHECK(f.runs().size() == 1);
    // the environment the program is given: its languages, one thread, structures asked for, no L2 notes
    CB_CHECK(dt_has(f.runs()[0], "threads=1 structs=1 l2= max="));

    // the same again: kept (nothing is run)
    const dc::Run again = f.decompiler.run(f.description);
    CB_CHECK_EQ(again.texts.size(), r.texts.size());
    for (const auto& [name, text] : r.texts) CB_CHECK_EQ(again.texts.at(name), text);   // (the same text, whatever order it was filled in)
    CB_CHECK(again.order == r.order);
    CB_CHECK_EQ(f.runs().size(), std::size_t{1});

    // one function's line changed: only it is made again, the other kept
    f.describe({{0x401010, "beta"}, {0x401000, "alpha"}}, "\t__cdecl16far");
    const dc::Run changed = f.decompiler.run(f.description, {});
    CB_CHECK_EQ(changed.texts.size(), std::size_t{2});
    CB_CHECK_EQ(f.runs().size(), std::size_t{2});   // (both lines changed: both made again)
    cb::fs::write_text(f.description, cb::fs::read_text(f.description) + "O\t401014\tcall-through-pointer\t0\n");
    (void)f.decompiler.run(f.description);
    CB_CHECK_EQ(f.runs().size(), std::size_t{3});
    CB_CHECK(dt_has(f.runs()[2], "--at 401010"));    // (the one call through a pointer sits in beta's range: only beta is made again)
    CB_CHECK(!dt_has(f.runs()[2], "--at 401000"));

    // `only`: those functions alone, as the program is asked for them
    const std::vector<dc::Va> only{0x401000};
    const dc::Run one = f.decompiler.run(f.description, only);
    CB_CHECK_EQ(one.texts.size(), std::size_t{1});
    CB_CHECK_EQ(one.order.at(0), "alpha");
});

CB_TEST("decompile.decompiler.run_chunks", [] {
    Fixture f;
    vector<std::pair<dc::Va, string>> functions;
    for (int i = 0; i < 150; ++i) functions.emplace_back(0x401000 + 0x10 * i, "f" + std::to_string(i));
    f.describe(functions);
    const dc::Run r = f.decompiler.run(f.description);
    CB_CHECK_EQ(r.texts.size(), std::size_t{150});
    CB_CHECK_EQ(r.order.front(), "f0");
    CB_CHECK_EQ(r.order.back(), "f149");
    CB_CHECK_EQ(f.runs().size(), std::size_t{3});   // 64 + 64 + 22: a process to each chunk
    for (const string& line : f.runs()) CB_CHECK(dt_has(line, "--at "));
    CB_CHECK(dt_has(f.runs()[0] + f.runs()[1] + f.runs()[2], "threads=1"));
});

CB_TEST("decompile.decompiler.run_threads_by_hand", [] {
    Fixture f({}, "4");   // the setting says: one process, four threads of its own, as it says
    vector<std::pair<dc::Va, string>> functions;
    for (int i = 0; i < 100; ++i) functions.emplace_back(0x401000 + 0x10 * i, "f" + std::to_string(i));
    f.describe(functions);
    const dc::Run r = f.decompiler.run(f.description);
    CB_CHECK_EQ(r.texts.size(), std::size_t{100});
    CB_CHECK_EQ(f.runs().size(), std::size_t{1});
    CB_CHECK(dt_has(f.runs()[0], "threads=4"));
});

CB_TEST("decompile.decompiler.run_survives_a_death", [] {
    Fixture f;
    f.describe({{0x401000, "a"}, {0x401010, "b"}, {0x401020, "c"}, {0x401030, "d"}});
    cb::fs::set_env("FAKE_DECOMPILER_DIE", "401020");
    const dc::Run r = f.decompiler.run(f.description);
    CB_CHECK_EQ(dt_names(r.order), "a,b,d");   // c is left out; what it died before and after are made
    CB_CHECK(dt_has(r.stderr_text, "decompiler died on 401020; left out"));
    CB_CHECK(f.runs().size() >= 3);
    CB_CHECK(dt_has(f.runs()[1], "--at 401020 401030"));   // (from the next it had not written)

    // a module with a function the program gives up on: no text for it
    cb::fs::set_env("FAKE_DECOMPILER_DIE", std::nullopt);
    cb::fs::set_env("FAKE_DECOMPILER_FAIL", "401010");
    Fixture g;
    g.describe({{0x401000, "a"}, {0x401010, "b"}});
    CB_CHECK_EQ(dt_names(g.decompiler.run(g.description).order), "a");
});

CB_TEST("decompile.decompiler.run_the_environment", [] {
    // the notes ask for L2, and are kept apart from a run that did not ask
    Fixture f;
    f.describe({{0x401000, "a"}});
    const dc::RunOptions asking{.max_instructions = 5000, .l2 = true, .write = false};
    (void)f.decompiler.run(f.description, {}, asking);
    CB_CHECK(dt_has(f.runs().back(), "l2=1 max=5000"));
    const size_t runs = f.runs().size();
    (void)f.decompiler.run(f.description, {}, dc::RunOptions{});
    CB_CHECK_EQ(f.runs().size(), runs + 1);   // (a text made with the notes is another text: made again without)
    // `bytes` lines are keyed by what the files hold, not where they are
    const stdfs::path code = f.dir.path() / "code.bin";
    cb::fs::write_text(code, "abcd");
    cb::fs::write_text(f.description, cb::fs::read_text(f.description) + "bytes\t1000\t" + cb::fs::to_text(code) + "\t0\t4\n");
    (void)f.decompiler.run(f.description);
    const size_t before = f.runs().size();
    const stdfs::path moved = f.dir.path() / "elsewhere.bin";
    stdfs::copy_file(code, moved);
    cb::fs::write_text(f.description, "arch\tx86:LE:32:default:windows\nX\t7f000000\tImported\t-1\nF\t401000\ta\t-1\nbytes\t1000\t" + cb::fs::to_text(moved) + "\t0\t4\n");
    (void)f.decompiler.run(f.description);
    CB_CHECK_EQ(f.runs().size(), before);   // (the same bytes in another place: the same description)
    cb::fs::write_text(moved, "abce");
    (void)f.decompiler.run(f.description);
    CB_CHECK_EQ(f.runs().size(), before + 1);   // (other bytes: another)
});

CB_TEST("decompile.decompiler.run_notes_and_l2", [] {
    Fixture f;
    f.describe({{0x401000, "a"}, {0x401010, "b"}});
    cb::fs::set_env("FAKE_DECOMPILER_NOTES", "1");
    const stdfs::path l2 = f.dir.path() / "l2.json";
    cb::fs::write_text(l2, dt_document("f"));
    cb::fs::set_env("FAKE_DECOMPILER_L2", cb::fs::to_text(l2));
    // the notes are taken off the C; the document is a node
    const dc::Run kept = f.decompiler.run(f.description, {}, dc::RunOptions{.l2 = true, .write = false});
    CB_CHECK_EQ(kept.texts.at("a"), "int a(void)\n{\n  return " + std::to_string(0x401000) + ";\n}");
    CB_CHECK_EQ(kept.notes.at("a").globals.at(0).address, 0x401000);
    CB_CHECK_EQ(kept.notes.at("a").pointers.at("a_p"), 4);
    CB_CHECK_EQ(kept.notes.at("a").structs.at(0).name, "struct_a");
    CB_CHECK(kept.notes.at("a").l2 != nullptr);
    CB_CHECK(!kept.notes.at("a").from_l2);
    // asked to write, each function's C is the back end's, from its document
    const dc::Run written = f.decompiler.run(f.description, {}, dc::RunOptions{.l2 = true, .write = true});
    CB_CHECK_EQ(written.texts.at("a"), "\nint f(void) { }");
    CB_CHECK(written.notes.at("a").from_l2);
    CB_CHECK_EQ(written.notes.at("b").from_l2, true);
    // a document the back end declines leaves the C the program printed (a fixture of its own: the kept text has the other document)
    Fixture g;
    g.describe({{0x401000, "a"}, {0x401010, "b"}});
    cb::fs::write_text(l2, dt_document("declined"));
    const dc::Run declined = g.decompiler.run(g.description, {}, dc::RunOptions{.l2 = true, .write = true});
    CB_CHECK_EQ(declined.texts.at("a"), "int a(void)\n{\n  return " + std::to_string(0x401000) + ";\n}");
    CB_CHECK(!declined.notes.at("a").from_l2);
});

CB_TEST("decompile.decompiler.c_from_l2", [] {
    Fixture f;
    cb::Arena arena;
    ct::ModuleFacts facts;
    facts.path = ct::ModuleFacts::Path::Flat32;
    facts.functions = {"g"};

    // no document: the printed C stays, whatever it is (and nothing to answer when there is no C)
    ct::FunctionNotes without;
    CB_CHECK_EQ(f.decompiler.c_from_l2("printed", without, std::nullopt, &facts).value(), "printed");
    CB_CHECK(!f.decompiler.c_from_l2(std::nullopt, without, std::nullopt, &facts).has_value());

    // a document: its C, and what the back end found of it in the facts the module gave it
    ct::FunctionNotes notes;
    notes.l2 = f.decompiler.node_of(Json::parse(dt_document("g")));
    const std::optional<string> written = f.decompiler.c_from_l2("printed", notes, std::nullopt, &facts);
    CB_CHECK_EQ(written.value(), "\nint g(void) { }");
    CB_CHECK(notes.from_l2);
    CB_CHECK_EQ(*facts.declared, "int g(void)");
    CB_CHECK_EQ(*facts.registers, "ecx");
    CB_CHECK(facts.structs_used.contains("struct_1"));
    CB_CHECK(facts.novalue);
    CB_CHECK_EQ(facts.interrupts.size(), std::size_t{1});
    CB_CHECK(!facts.left.has_value());   // (the 32-bit module's facts are not kept as text: only the 16-bit path reads them, for flags())
    ct::ModuleFacts facts16;
    facts16.path = ct::ModuleFacts::Path::Win16;
    ct::FunctionNotes notes16;
    notes16.l2 = notes.l2;
    CB_CHECK(f.decompiler.c_from_l2("printed", notes16, std::nullopt, &facts16).has_value());
    CB_CHECK(facts16.left.has_value());   // (the facts as the back end left them, for flags())
    CB_CHECK(dt_has(*facts16.left, "\"declared\":\"int g(void)\""));
    // without module facts it is only the text
    ct::FunctionNotes bare;
    bare.l2 = notes.l2;
    CB_CHECK_EQ(f.decompiler.c_from_l2("printed", bare, std::nullopt, nullptr).value(), "\nint g(void) { }");

    // declined: the printed C stays; a KeyError of the library's too, with the facts it left; any other error is an error
    ct::FunctionNotes declined;
    declined.l2 = f.decompiler.node_of(Json::parse(dt_document("declined")));
    ct::ModuleFacts declined_facts;
    CB_CHECK_EQ(f.decompiler.c_from_l2("printed", declined, std::nullopt, &declined_facts).value(), "printed");
    CB_CHECK(!declined.from_l2);
    ct::FunctionNotes missing;
    missing.l2 = f.decompiler.node_of(Json::parse(dt_document("keyerror")));
    ct::ModuleFacts missing_facts;
    missing_facts.path = ct::ModuleFacts::Path::Win16;
    CB_CHECK_EQ(f.decompiler.c_from_l2("printed", missing, std::nullopt, &missing_facts).value(), "printed");
    CB_CHECK(missing_facts.left.has_value() && dt_has(*missing_facts.left, "int partial(void)"));
    ct::FunctionNotes broken;
    broken.l2 = f.decompiler.node_of(Json::parse(dt_document("broken")));
    CB_CHECK_THROWS_AS(nl::LibraryError, f.decompiler.c_from_l2("printed", broken, std::nullopt, nullptr));

    // a document with an integer past 64 bits is given to the library as the program wrote it
    ct::FunctionNotes exact;
    exact.l2 = notes.l2;
    exact.l2_exact_json = dt_document("exact");
    CB_CHECK_EQ(f.decompiler.c_from_l2("printed", exact, std::nullopt, nullptr).value(), "\nint exact(void) { }");
});

CB_TEST("decompile.decompiler.generation", [] {
    Fixture f;
    const dc::Decompiler::Generation g = f.decompiler.generation();
    CB_CHECK_EQ(g.revision, dc::kRevision);
    CB_CHECK(!g.program.empty());
    CB_CHECK(!g.languages.empty());
    CB_CHECK(f.decompiler.available());
    CB_CHECK_EQ(f.decompiler.structs_for(f.description), "1");
    dc::Context c = Fixture::context(f.dir.path(), f.cache, f.environment, {}, "");
    c.settings.struct_recovery = " 32 ";
    dc::Decompiler thirty_two(c);
    f.describe({{0x401000, "a"}});
    CB_CHECK_EQ(thirty_two.structs_for(f.description), "1");   // (a 32-bit description)
    cb::fs::write_text(f.description, "arch\tx86:LE:16:Protected Mode:default\nF\t1\ta\n");
    CB_CHECK_EQ(thirty_two.structs_for(f.description), "0");
    c.settings.struct_recovery = "off";
    CB_CHECK_EQ(dc::Decompiler(c).structs_for(f.description), "0");
    c.settings.program = f.dir.path() / "missing.exe";
    CB_CHECK(!dc::Decompiler(c).available());
});

CB_TEST("decompile.decompiler.settings", [] {
    cb::fs::ScopedTempDir dir("cb-decompiler-settings");
    cb::fs::write_text(dir.path() / "settings.json", R"({"decompiler_bin": "tools/bin", "struct_recovery": "32", "threads": 4})");
    const dc::Settings s = dc::Settings::from_config(cb::config::Config::load(dir.path()));
    CB_CHECK(cb::fs::same_path(s.program, dir.path() / "tools" / "bin" / "decompiler.exe"));
    CB_CHECK(cb::fs::same_path(s.languages, dir.path() / "native" / "decompiler" / "languages") || cb::fs::get_env("CB_LANGUAGES_DIR").has_value());
    CB_CHECK(cb::fs::same_path(s.checkout, dir.path()));
    CB_CHECK_EQ(s.struct_recovery, "32");
    CB_CHECK_EQ(s.threads, "4");
    cb::fs::write_text(dir.path() / "settings.json", R"({"struct_recovery": "", "threads": 0})");
    const dc::Settings unset = dc::Settings::from_config(cb::config::Config::load(dir.path()));
    CB_CHECK_EQ(unset.struct_recovery, "off");   // (empty says off, as the setting always did)
    CB_CHECK_EQ(unset.threads, "");              // (0 says: the processes share the functions)
    const dc::Policy policy = dc::Policy::from_environment();
    CB_CHECK(policy.tree_images() == (policy.asm_blocks || policy.frame16));
});

// ---- the lanes ------------------------------------------------------------------------------------------------------------------------

CB_TEST("decompile.decompiler.dos_program_as_c", [] {
    Fixture f;
    // a .COM program: mov ah, 4Ch / int 21h
    const stdfs::path program = f.dir.path() / "TINY.COM";
    cb::fs::write_file(program, cb::Bytes{0xB4, 0x4C, 0xCD, 0x21});
    const dc::AsC c = f.decompiler.as_c(program);
    CB_CHECK(c.text.starts_with("/* TINY.COM -- decompiled by CodeBreaker; not built, not judged. */\n\n"));
    CB_CHECK(dt_has(c.text, "int entry(void)"));
    CB_CHECK_EQ(c.decompiled, std::size_t{1});
    CB_CHECK_EQ(c.found, std::size_t{1});
    CB_CHECK(c.text.ends_with("}\n"));
    const dc::DosProgramC d = f.decompiler.decompile_dos(program);
    CB_CHECK_EQ(d.functions.order.size(), std::size_t{1});
    CB_CHECK(dt_has(d.functions.texts.at(d.functions.order[0]), "DOS_Exit(AL = return code)"));   // (the interrupt's service named: AH=4Ch)
    CB_CHECK(!d.description->lines.empty());
});

CB_TEST("decompile.decompiler.nes_cartridge_as_c", [] {
    Fixture f;
    cb::Bytes rom{'N', 'E', 'S', 0x1A, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    rom.resize(16 + 16384, 0xEA);   // NOPs
    rom[16 + 16384 - 4] = 0x00;     // the reset vector at $FFFC: $C000
    rom[16 + 16384 - 3] = 0xC0;
    rom[16] = 0x60;                 // RTS
    const stdfs::path cartridge = f.dir.path() / "game.nes";
    cb::fs::write_file(cartridge, rom);
    const dc::AsC c = f.decompiler.as_c(cartridge);
    CB_CHECK(c.text.starts_with("/* game.nes -- decompiled by CodeBreaker; not built, not judged. */\n\n"));
    CB_CHECK(c.decompiled >= 1);
    CB_CHECK_EQ(c.found, c.decompiled);
});

CB_TEST("decompile.decompiler.not_an_executable", [] {
    Fixture f;
    const stdfs::path file = f.dir.path() / "notes.txt";
    cb::fs::write_text(file, "this is text");
    CB_CHECK_THROWS_AS(cb::UsageError, f.decompiler.as_c(file));
});

CB_TEST("decompile.decompiler.command", [] {
    Fixture f;
    const stdfs::path program = f.dir.path() / "TINY.COM";
    cb::fs::write_file(program, cb::Bytes{0xB4, 0x4C, 0xCD, 0x21});
    cb::base::Corpus corpus(cb::base::Products{});
    const cb::base::converted::Store store(f.dir.path() / "Converted");

    cb::testing::CapturedStd captured;
    CB_CHECK_EQ(dc::command(f.decompiler, corpus, store, {}), 2);   // no arguments: the usage
    CB_CHECK(dt_has(captured.out(), "codebreaker decompile <module> [--out FILE]"));

    const string out_file = cb::fs::to_text(f.dir.path() / "tiny.c");
    const std::vector<string> with_out{cb::fs::to_text(program), "--out", out_file};
    CB_CHECK_EQ(dc::command(f.decompiler, corpus, store, with_out), 0);
    CB_CHECK(dt_has(cb::fs::read_text(f.dir.path() / "tiny.c"), "decompiled by CodeBreaker"));
    CB_CHECK(dt_has(captured.out(), "TINY.COM: 1 of 1 functions -> " + out_file));

    // without --out: the binary's project folder, which says what was done
    CB_CHECK_EQ(dc::command(f.decompiler, corpus, store, std::vector<string>{cb::fs::to_text(program)}), 0);
    const cb::base::converted::Project project = store.project(program);
    CB_CHECK(stdfs::exists(project.src() / "tiny.c"));
    CB_CHECK(dt_has(cb::fs::read_text(project.manifest_path()), "functions as C (readable; not built or judged)"));

    CB_CHECK_THROWS_AS(cb::UsageError, dc::command(f.decompiler, corpus, store, std::vector<string>{cb::fs::to_text(f.dir.path() / "nothing.dll")}));
    CB_CHECK_THROWS_AS(cb::UsageError, dc::command(f.decompiler, corpus, store, std::vector<string>{cb::fs::to_text(program), "--out"}));
});

}  // namespace

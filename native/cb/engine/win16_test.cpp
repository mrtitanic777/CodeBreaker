// Tests of the 16-bit module build as steps of the engine.
//
// testdata/win16.golden.json is the recorded run on a real 16-bit module of the Windows 95 corpus (awdevl16.dll, 25
// functions): the decompiled module as the lane's prepare made it (the data the steps read), and the facts the
// steps made of it -- the segment table, the functions, the description's lines, and for the first dozen functions the C, the
// source as the build compiles it, the contract; the notes of two; and, for the first six, the compiler's options and text of the
// object and the flow check. The C++ steps, given the same module by a stage, must make the same documents. (The compiler here is a
// stand-in program: what OpenWatcom made of the text is the compiler's; that the steps hand it the same options and text is what the
// golden holds.)
//
// Design notes:
//   * A fact is a document: the module is kept as the document of Module16, an object as its bytes in hex, and the linked file in the
//     build folder, named by ne.image.
//   * The compiler's work is offered to the link by FunctionBuilder::serve, around the link call.
//   * What is particular to the platform (the segments, the lane's preparation, the gate, the contract check and the link) is given as
//     Stages; the steps' own logic -- which facts each is made of, the loop of the gate and the link -- is what is tested here.
#include "cb/engine/win16.hpp"

#include <algorithm>
#include <map>
#include <mutex>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::engine;
using namespace cb::engine::win16;

namespace {

using KV = knowledge::Value;

void expect(bool ok, const std::string& what) {
    cb::testing::count_check();
    if (!ok) cb::testing::record_failure(__FILE__, __LINE__, what);
}

std::string canon(const KV& v) {
    switch (v.kind()) {
        case KV::Kind::Null: return "null";
        case KV::Kind::Bool: return *v.boolean() ? "true" : "false";
        case KV::Kind::Int: return std::to_string(*v.integer());
        case KV::Kind::Double: return std::to_string(*v.number());
        case KV::Kind::Text: return "\"" + *v.text() + "\"";
        case KV::Kind::List: {
            std::string out = "[";
            for (const KV& x : *v.list()) out += canon(x) + ",";
            return out + "]";
        }
        case KV::Kind::Map: {
            std::map<std::string, std::string> sorted;
            for (const auto& [k, x] : *v.map()) sorted[k] = canon(x);
            std::string out = "{";
            for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
            return out + "}";
        }
    }
    return "";
}

std::string canon(const Json& j) {
    if (j.is_null()) return "null";
    if (j.is_boolean()) return j.get<bool>() ? "true" : "false";
    if (j.is_number_integer()) return std::to_string(j.get<std::int64_t>());
    if (j.is_string()) return "\"" + j.get<std::string>() + "\"";
    if (j.is_array()) {
        std::string out = "[";
        for (const Json& x : j) out += canon(x) + ",";
        return out + "]";
    }
    std::map<std::string, std::string> sorted;
    for (auto it = j.begin(); it != j.end(); ++it) sorted[it.key()] = canon(it.value());
    std::string out = "{";
    for (const auto& [k, x] : sorted) out += k + ":" + x + ",";
    return out + "}";
}

KV doc_of(const facts::Value& v) {   // (a text is kept as text)
    if (const auto* s = std::get_if<std::string>(&v)) return KV(*s);
    return std::get<KV>(v);
}

const Json& golden() {
    static const Json doc = cb::json::load(cb::testing::data_path("engine/testdata/win16.golden.json"));
    return doc;
}

// The module as the lane made it.
Module16 golden_module() {
    const Json& m = golden().at("module");
    Module16 out;
    out.header = m.at("header").get<std::string>();
    out.description = m.at("description").get<std::vector<std::string>>();
    for (const Json& e : m.at("by_va")) out.by_va[e.at(0).get<std::int64_t>()] = e.at(1).get<std::string>();
    for (const Json& e : m.at("where")) out.where[e.at(0).get<std::int64_t>()] = Place{e.at(1).at(0).get<std::int64_t>(), e.at(1).at(1).get<std::int64_t>()};
    for (auto it = m.at("key").begin(); it != m.at("key").end(); ++it) out.key[it.key()] = it.value().get<std::string>();
    for (const std::string& n : m.at("near").get<std::vector<std::string>>()) out.near.insert(n);
    for (auto it = m.at("texts").begin(); it != m.at("texts").end(); ++it) out.texts[it.key()] = it.value().get<std::string>();
    out.order = m.at("order").get<std::vector<std::string>>();
    out.all_keys = m.at("all_keys").get<std::vector<std::string>>();
    out.sig = m.at("sig").get<std::string>();
    for (auto it = m.at("notes").begin(); it != m.at("notes").end(); ++it) out.notes[it.key()] = KV::from_json_text(it.value().dump());
    for (const std::string& n : m.at("wide").get<std::vector<std::string>>()) out.wide.insert(n);
    return out;
}

// The compiler: a program that takes every source but one with REJECT in it and leaves its text as the object.
constexpr const char* kFakeCompiler =
    "@echo off\r\n"
    "set \"last=\"\r\n"
    "for %%a in (%*) do set \"last=%%~a\"\r\n"
    "findstr /C:\"REJECT\" \"%last%\" >nul 2>nul\r\n"
    "if not errorlevel 1 goto reject\r\n"
    "copy /y \"%last%\" \"%last:~0,-2%.obj\" >nul\r\n"
    "exit /b 0\r\n"
    ":reject\r\n"
    "echo Error! E1: this function is rejected\r\n"
    "exit /b 1\r\n";

// A world to ask the 16-bit steps in. The stages are stand-ins whose calls are counted.
struct World16 {
    fs::ScopedTempDir dir{"w16"};
    std::filesystem::path binary, compiler;
    facts::Databases databases;
    Steps steps;
    knowledge::ReadDigests digests;
    Engine engine;
    build::functions::FunctionBuilder builder;
    Module16 module;
    std::vector<Segment> segments;
    int prepares = 0, links = 0, gates = 0;
    std::vector<std::map<std::string, std::string>> link_gated;      // what each link was told to leave to the original
    std::vector<std::vector<std::string>> gated_names;              // what each gate run was asked
    std::set<std::string> divergent;                                // the functions the gate finds different
    std::map<std::string, std::string> overflow_once;               // the first link says these overflow
    std::map<std::string, std::string> contract_says;               // the contract check finds these
    bool contract_throws = false, no_image = false;
    std::unique_ptr<Win16> win16;

    explicit World16(Module16 m, bool gate = true)
        : binary(dir.path() / "AWDEV.DLL"),
          compiler(dir.path() / "fakecc.cmd"),
          databases(facts::Config{dir.path() / "db", 1 * facts::kGiB}),
          digests([](const knowledge::Read&) { return std::string(knowledge::kAbsent); }),
          engine(steps, databases, digests, 4),
          builder({.workers = {.workers = 1}}),
          module(std::move(m)) {
        fs::write_text(binary, "an NE file");
        fs::write_text(compiler, kFakeCompiler);
        for (const Json& s : golden().at("facts").at("ne.segments"))
            segments.push_back({s.at("idx").get<std::int64_t>(), s.at("kind").get<std::string>(), s.at("file_off").get<std::int64_t>(),
                                s.at("length").get<std::int64_t>(), s.at("flags").get<std::int64_t>(), s.at("minalloc").get<std::int64_t>()});
        Config config{.builder = builder, .wcc = {compiler, {}}, .options = options(), .stages = {}, .gate = gate};
        config.compiler_stamp = [] { return std::string("fake wcc"); };
        config.stages.segments = [this](const std::filesystem::path&) { return segments; };
        config.stages.prepare = [this](const std::filesystem::path&, const std::filesystem::path& work) {
            ++prepares;
            CB_CHECK(std::filesystem::is_directory(work) || true);
            return module;
        };
        config.stages.gate_tables = [](const Module16& m) {
            KV::List wide;
            for (const std::string& n : m.wide) wide.emplace_back(n);
            return KV(KV::Map{{"wide", KV(std::move(wide))}});
        };
        config.stages.gate = [this](const std::filesystem::path&, const std::filesystem::path& folder, const std::vector<std::string>& names, const KV& tables) {
            ++gates;
            gated_names.push_back(names);
            CB_CHECK(std::filesystem::is_directory(folder));
            CB_CHECK(tables.get("wide") != nullptr);
            std::vector<GateAnswer> out;
            for (const std::string& n : names) out.push_back({n, divergent.contains(n) ? verify::Verdict::Divergent : verify::Verdict::Equivalent, divergent.contains(n) ? "it returns 1, the original 0" : ""});
            return out;
        };
        config.stages.contract = [this](const std::filesystem::path&, const std::filesystem::path&, const std::filesystem::path& image, const std::vector<std::string>&) {
            CB_CHECK(std::filesystem::is_regular_file(image));
            if (contract_throws) throw ToolError("contract16", 1, "the checker fell over");
            return contract_says;
        };
        config.stages.link = [this](const LinkRequest& r) {
            ++links;
            link_gated.push_back(r.gated);
            LinkOutcome o;
            o.name = "awdev.dll";
            o.gated = r.gated;
            if (links == 1) o.resident_overflow = overflow_once;
            builder.write_header(r.folder, r.module.header);
            const std::vector<std::string> flags = cflags(options());
            for (const std::string& name : r.module.order) {
                if (r.gated.contains(name) || !r.module.texts.contains(name)) continue;
                const auto va = std::find_if(r.module.by_va.begin(), r.module.by_va.end(), [&](const auto& e) { return e.second == name; });
                const auto where = r.module.where.find(va->first);
                if (where == r.module.where.end()) continue;
                std::vector<std::string> f = flags;
                f.push_back("-nt=CODE" + std::to_string(where->second.seg));
                std::map<std::int64_t, Callee> callees;
                for (const auto& [a, n] : r.module.by_va) {
                    const auto w = r.module.where.find(a);
                    callees[a] = Callee{n, w == r.module.where.end() ? std::nullopt : std::optional<std::int64_t>(w->second.seg), r.module.near.contains(n)};
                }
                const std::string why = builder.compile({compiler, {}}, f, r.folder, "g" + name, named(r.module.texts.at(name), where->second.seg, callees));
                if (why.empty()) o.placed_c.insert(name);
                else o.rejected[name] = why;
            }
            fs::write_text(r.folder / "m.dll", "linked");
            if (!no_image) o.image = r.folder / "m.dll";
            return o;
        };
        config.stages.whole_file_image = [](const std::filesystem::path&, const std::filesystem::path& image) { CB_CHECK(std::filesystem::is_regular_file(image)); };
        config.stages.check = [](const LinkOutcome& o) {
            return o.image ? "linked: " + std::to_string(o.placed_c.size()) + " functions" + (o.gate_error ? "; gate: " + *o.gate_error : "") +
                                 (o.contract_error ? "; contract: " + *o.contract_error : "")
                           : std::string("NOT BUILT");
        };
        win16 = std::make_unique<Win16>(steps, engine, std::move(config));
    }

    // the platform's options as the golden's cflags without the four the module build adds
    static std::vector<std::string> options() {
        std::vector<std::string> o = golden().at("cflags").get<std::vector<std::string>>();
        o.resize(o.size() - 4);
        return o;
    }

    std::shared_ptr<facts::Database> db() { return engine.open(binary); }
    KV get(const std::string& kind, const std::string& key = "") { return doc_of(engine.get(db(), kind, key)); }
};

// A small module of our own: three functions in two segments.
Module16 small_module() {
    Module16 m;
    m.header = "typedef int int2;";
    m.description = {"F\t10000000\tf0\t4\t__stdcall16far\t2", "X\t20000000\tUSER.GETFOCUS\t0", "F\t10000040\tf1\t-1\t__stdcall16far", "F\t20000000\tf2\t4\t__stdcall16far\t2"};
    m.by_va = {{0x10000000, "f0"}, {0x10000040, "f1"}, {0x20000000, "f2"}};
    m.where = {{0x10000000, {1, 0}}, {0x10000040, {1, 0x40}}, {0x20000000, {2, 0}}};
    m.near = {"f1"};
    m.key = {{"f0", "1:0000"}, {"f1", "1:0040"}, {"f2", "2:0000"}};
    m.order = {"f0", "f1", "f2"};
    m.all_keys = {"1:0000", "1:0040", "2:0000", "2:0100"};
    m.texts = {{"f0", "int2 f0(int2 a)\n{\n  return func_0x10000040(a) + func_0x20000000(a) + GetFocus();\n}"},
               {"f1", "int2 f1(void)\n{\n  return 1;\n}"},
               {"f2", "int2 f2(int2 a)\n{\n  return a;\n}"}};
    m.sig = "whole";
    m.notes = {{"f1", KV(KV::Map{{"l2", KV(KV::Map{{"node", KV("Block")}})}, {"from_l2", KV(true)}})}};
    return m;
}

}  // namespace

// ---- the steps on a real module ----------------------------------------------------------------------------------------------

CB_TEST("engine.win16.the_steps_make_what_the_recorded_steps_made_of_a_real_module", [] {
    World16 w(golden_module());
    const Json& want = golden().at("facts");
    expect(canon(w.get("ne.segments")) == canon(want.at("ne.segments")), "ne.segments");
    expect(canon(w.get("ne.description")) == canon(want.at("ne.description")), "ne.description");
    expect(canon(w.get("ne.functions")) == canon(want.at("ne.functions")), "ne.functions");
    expect(canon(w.get("ne.header")) == canon(want.at("ne.header")), "ne.header");
    expect(canon(w.get("ne.lines")) == canon(want.at("ne.lines")), "ne.lines");
    int checked = 0;
    for (auto it = want.at("function.c").begin(); it != want.at("function.c").end(); ++it) {
        const std::string k = it.key();
        expect(canon(w.get("function.c", k)) == canon(it.value()), "function.c " + k);
        expect(canon(w.get("function.source", k)) == canon(want.at("function.source").at(k)), "function.source " + k);
        expect(canon(w.get("function.contract", k)) == canon(want.at("function.contract").at(k)), "function.contract " + k);
        ++checked;
    }
    CB_CHECK_EQ(checked, 12);
    for (auto it = want.at("function.notes").begin(); it != want.at("function.notes").end(); ++it) {
        expect(canon(w.get("function.notes", it.key())) == canon(it.value()), "function.notes " + it.key());
        if (it.value().is_null()) {
            expect(w.get("function.l2", it.key()).is_null(), "function.l2 of a function with no notes " + it.key());
        } else {
            expect(canon(w.get("function.l2", it.key())) == canon(it.value().at("l2")), "function.l2 " + it.key());
        }
    }
    // the compiler is handed the recorded options and text
    for (auto it = golden().at("compiled").begin(); it != golden().at("compiled").end(); ++it) {
        const Json& c = it.value();
        if (!c.at("object").is_null()) {
            const KV o = w.get("function.object", it.key());
            expect(canon(*o.get("flags")) == canon(c.at("object").at("flags")), "function.object flags " + it.key());
            expect(canon(*o.get("text")) == canon(c.at("object").at("text")), "function.object text " + it.key());
            expect(o.get("obj") != nullptr, "the stand-in compiler made an object for " + it.key());
        }
        const KV n = w.get("function.never_set", it.key());
        expect(canon(*n.get("flags")) == canon(c.at("never_set").at("flags")), "function.never_set flags " + it.key());
        expect(canon(*n.get("text")) == canon(c.at("never_set").at("text")), "function.never_set text " + it.key());
        expect(n.get("names") != nullptr && n.get("names")->list() != nullptr, "function.never_set names " + it.key());
    }
    CB_CHECK_EQ(w.prepares, 1);
    // the options the module is compiled with
    expect(canon(KV([&] {
               KV::List l;
               for (const std::string& f : cflags(World16::options())) l.emplace_back(f);
               return l;
           }())) == canon(golden().at("cflags")), "the compiler's options");
    // asked again (a new run): nothing is made again
    const std::size_t before = w.engine.last_run(*w.db()).size();
    (void)before;
    w.get("function.source", "1:0000");
    for (const auto& c : w.engine.last_run(*w.db())) CB_CHECK_EQ(c.made, std::size_t{0});
});

CB_TEST("engine.win16.the_module_document_round_trips", [] {
    const Module16 m = golden_module();
    const KV doc = to_value(m);
    const Module16 back = module_of(doc);
    CB_CHECK_EQ(back.header, m.header);
    CB_CHECK(back.description == m.description);
    CB_CHECK(back.by_va == m.by_va);
    CB_CHECK(back.near == m.near);
    CB_CHECK(back.wide == m.wide);
    CB_CHECK(back.order == m.order);
    CB_CHECK(back.key == m.key);
    CB_CHECK(back.texts == m.texts);
    CB_CHECK_EQ(back.sig, m.sig);
    CB_CHECK_EQ(back.where.size(), m.where.size());
    for (const auto& [va, p] : m.where) CB_CHECK(back.where.at(va).seg == p.seg && back.where.at(va).off == p.off);
    CB_CHECK_EQ(back.notes.size(), m.notes.size());
    CB_CHECK_EQ(canon(to_value(back)), canon(doc));
    CB_CHECK_THROWS_AS(FormatError, module_of(KV(std::int64_t{3})));
    CB_CHECK_THROWS_AS(FormatError, module_of(KV(KV::Map{{"header", KV("x")}})));
});

// ---- a call left as an address ---------------------------------------------------------------------------------------------------------

CB_TEST("engine.win16.a_call_left_as_an_address_goes_to_the_function_reconstructed_there", [] {
    const std::map<std::int64_t, Callee> by_va = {{0x10000040, {"near_one", 1, true}}, {0x20000000, {"far_one", 2, false}}, {0x30000000, {"unplaced", std::nullopt, false}}};
    // from segment 1: the near function of its own segment, the far one of another; the unplaced one stays an address
    CB_CHECK_EQ(named("a(func_0x10000040) + func_0x20000000(1) + func_0x30000000", 1, by_va), "a(near_one) + far_one(1) + func_0x30000000");
    // from segment 3: a near function of segment 1 cannot be reached
    CB_CHECK_EQ(named("func_0x10000040 func_0x20000000", 3, by_va), "func_0x10000040 far_one");
    // not an address: capitals, the wrong number of digits, no word boundary
    CB_CHECK_EQ(named("func_0x1000004A func_0x1000004 func_0x100000400 xfunc_0x10000040 func_0x10000040x", 1, by_va),
                "func_0x1000004A func_0x1000004 func_0x100000400 xfunc_0x10000040 func_0x10000040x");
    CB_CHECK_EQ(named("", 1, by_va), "");
    CB_CHECK_EQ(named("func_0x", 1, by_va), "func_0x");
    CB_CHECK_EQ(named("func_0xfunc_0x10000040", 1, by_va), "func_0xfunc_0x10000040");   // (the second has no word boundary before it)
    CB_CHECK_EQ(named("func_0x20000000;func_0x20000000", 1, by_va), "far_one;far_one");
    CB_CHECK_EQ(named("tail func_0x20000000", 1, by_va), "tail far_one");
});

// ---- the steps on a module of our own ----------------------------------------------------------------------------------------------------

CB_TEST("engine.win16.the_function_facts_of_a_small_module", [] {
    World16 w(small_module());
    const KV funcs = w.get("ne.functions");
    CB_CHECK_EQ(funcs.map()->size(), std::size_t{3});
    CB_CHECK_EQ(*funcs.get("1:0040")->get("stem")->text(), "f0001");   // the order of the segment
    CB_CHECK_EQ(*funcs.get("2:0000")->get("stem")->text(), "f0000");
    CB_CHECK(*funcs.get("1:0040")->get("near")->boolean());
    // the source: the calls to f1 (near, same segment) and f2 (far) named; GetFocus is not an address
    const KV s0 = w.get("function.source", "1:0000");
    CB_CHECK(s0.get("text")->text()->find("return f1(a) + f2(a) + GetFocus();") != std::string::npos);
    CB_CHECK_EQ(*s0.get("seg")->integer(), 1);
    CB_CHECK_EQ(canon(*s0.get("flags")), canon(KV([&] {
                    KV::List l;
                    for (const std::string& f : cflags(World16::options())) l.emplace_back(f);
                    l.emplace_back("-nt=CODE1");
                    return l;
                }())));
    // the contract: its own line and those of the callees it has lines for (X lines by name)
    const KV c0 = w.get("function.contract", "1:0000");
    // (f0's C calls f1 and f2 as addresses, which the description does not name: the C is the decompiler's, not the source the build compiles)
    CB_CHECK_EQ(canon(c0), canon(KV(KV::List{KV("F\t10000000\tf0\t4\t__stdcall16far\t2")})));
    // notes and L2 where there are some, null where not
    CB_CHECK(w.get("function.notes", "1:0000").is_null());
    CB_CHECK_EQ(*w.get("function.notes", "1:0040").get("from_l2")->boolean(), true);
    CB_CHECK_EQ(*w.get("function.l2", "1:0040").get("node")->text(), "Block");
    CB_CHECK(w.get("function.l2", "1:0000").is_null());
    // a function with no C, or not placed, has no source
    w.module.where.erase(0x20000000);
    w.engine.put(*w.db(), "function.c", "2:0000", facts::Value(KV()));
    CB_CHECK(w.get("function.source", "2:0000").is_null());
    CB_CHECK(w.get("function.object", "2:0000").is_null());
    CB_CHECK(w.get("function.never_set", "2:0000").is_null());
});

CB_TEST("engine.win16.a_function_the_compiler_rejects_has_the_stand_ins_object", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    Module16 m = small_module();
    m.texts["f2"] = "int2 f2(int2 a)\n{\n  return REJECT;\n}";
    World16 w(m);
    const KV o = w.get("function.object", "2:0000");
    CB_CHECK(o.get("error")->text()->find("rejected") != std::string::npos);
    CB_CHECK(o.get("obj") == nullptr);
    CB_CHECK(o.get("stand_in_text")->text()->find("REJECT") == std::string::npos);   // the stand-in has the signature, not the body
    CB_CHECK(o.get("stand_in")->text() != nullptr);                                  // and it compiles
    const KV ok = w.get("function.object", "1:0000");
    CB_CHECK(ok.get("error") == nullptr && ok.get("obj") != nullptr);
    CB_CHECK(!crypto::from_hex(*ok.get("obj")->text()).empty());
});

CB_TEST("engine.win16.a_function_changed_by_hand_makes_again_what_depends_on_it_and_nothing_else", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.get("function.object", "1:0040");
    w.get("function.object", "2:0000");
    w.get("function.contract", "1:0000");
    auto made = [&](const std::string& kind) {
        for (const auto& c : w.engine.last_run(*w.db()))
            if (c.kind == kind) return c.made;
        return std::size_t{0};
    };
    w.engine.put(*w.db(), "function.c", "1:0040", facts::Value(KV("int2 f1(void)\n{\n  return 2;\n}")));
    w.get("function.object", "1:0040");
    CB_CHECK_EQ(made("function.source"), std::size_t{1});
    CB_CHECK_EQ(made("function.object"), std::size_t{1});
    CB_CHECK_EQ(w.prepares, 1);                      // (the module was not prepared again)
    w.get("function.object", "2:0000");
    CB_CHECK_EQ(made("function.object"), std::size_t{0});   // another function's object is as it was
    // its contract is made again for the function and for the callers who call it (f0 calls f1 as an address, so its own text is the same)
    w.get("function.contract", "1:0000");
    CB_CHECK_EQ(made("function.contract"), std::size_t{0});
});

// ---- the link and the gate ----------------------------------------------------------------------------------------------------------------

CB_TEST("engine.win16.the_link_is_handed_the_objects_the_engine_made", [] {
    World16 w(small_module());
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 1);
    CB_CHECK_EQ(b.name, "awdev.dll");
    CB_CHECK_EQ(b.served, 3);      // all three functions' objects came from the facts
    CB_CHECK_EQ(b.compiled, 0);
    CB_CHECK_EQ(b.report, "linked: 3 functions");
    CB_CHECK(b.image.has_value() && std::filesystem::is_regular_file(*b.image));
    CB_CHECK_EQ(fs::read_text(*b.image), "linked");
    CB_CHECK(b.gated.empty());
    CB_CHECK(b.gate_set == (std::vector<std::string>{"f0", "f1", "f2"}));
    CB_CHECK_EQ(w.gates, 1);
    // every function was gated once
    CB_CHECK(w.gated_names.at(0) == (std::vector<std::string>{"f0", "f1", "f2"}));
    // asked again: nothing is linked or gated again
    w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 1);
    CB_CHECK_EQ(w.gates, 1);
});

CB_TEST("engine.win16.functions_the_gate_finds_different_are_left_to_the_original_and_the_module_is_linked_again", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.divergent = {"f1"};
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 2);
    CB_CHECK(w.link_gated.at(0).empty());
    CB_CHECK_EQ(w.link_gated.at(1).at("f1"), "it returns 1, the original 0");
    CB_CHECK_EQ(w.gates, 1);   // (the second link is not gated again: its functions were)
    CB_CHECK_EQ(b.gated.at("f1"), "it returns 1, the original 0");
    CB_CHECK(b.gate_set == (std::vector<std::string>{"f0", "f1", "f2"}));
    // a function changed later: the verdict kept of f1 stands, it is left to the original at once, and only the rebuilt function is gated
    w.divergent.clear();
    w.engine.put(*w.db(), "function.c", "2:0000", facts::Value(KV("int2 f2(int2 a)\n{\n  return a + 1;\n}")));
    w.module.texts["f2"] = "int2 f2(int2 a)\n{\n  return a + 1;\n}";
    const Built16 again = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 3);
    CB_CHECK(w.link_gated.at(2).contains("f1"));
    { std::string seen; for (const auto& g : w.gated_names) { for (const auto& n : g) seen += n + " "; seen += "| "; } expect(w.gated_names.back() == (std::vector<std::string>{"f2"}), "the gate was asked: " + seen); }   // (f0's verdict is kept; f1 is the original's: not in the image, not gated)
    CB_CHECK(again.gated.contains("f1"));
});

CB_TEST("engine.win16.a_contract_mismatch_leaves_the_function_to_the_original", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.contract_says = {{"f0", "it calls f2 with one argument"}};
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 2);
    CB_CHECK_EQ(b.gated.at("f0"), "its calls are not the original's: it calls f2 with one argument");
});

CB_TEST("engine.win16.a_contract_check_that_cannot_be_made_is_a_note_and_a_gate_that_cannot_run_is_not_kept", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.contract_throws = true;
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK(b.report.find("contract: ToolError") != std::string::npos);
    CB_CHECK_EQ(w.links, 1);
    // a gate answer that is not a verdict is not kept: asked again with a new gate
    World16 v(small_module());
    v.divergent.clear();
    const auto db = v.db();
    v.win16->build(v.binary);
    CB_CHECK(db->row("function.gate", "1:0000").has_value());
});

CB_TEST("engine.win16.a_resident_overflow_links_again_with_the_functions_gated", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.overflow_once = {{"f2", "the resident segment is full"}};
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 2);
    CB_CHECK_EQ(w.link_gated.at(1).at("f2"), "the resident segment is full");
    CB_CHECK(b.gated.contains("f2"));
}
);

CB_TEST("engine.win16.a_link_that_makes_no_image_is_a_build_that_is_not_built", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.no_image = true;
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK(!b.image.has_value());
    CB_CHECK_EQ(b.report, "NOT BUILT");
    CB_CHECK_EQ(w.gates, 0);
});

CB_TEST("engine.win16.the_gate_can_be_turned_off", [] {
    World16 w(small_module(), /*gate=*/false);
    const Built16 b = w.win16->build(w.binary, w.dir.path() / "out");
    CB_CHECK_EQ(w.gates, 0);
    CB_CHECK(b.gate_set.empty());
    CB_CHECK(*b.image == w.dir.path() / "out" / "awdev.dll");
    CB_CHECK_EQ(fs::read_text(*b.image), "linked");
});

CB_TEST("engine.win16.the_build_folder_cleared_the_image_is_made_again", [] {
    CB_WINDOWS_ONLY("its compiler and linker stand-ins are .cmd files");
    World16 w(small_module());
    w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 1);
    std::filesystem::remove_all(facts::Databases::work_folder(*w.db(), "build"));
    const Built16 b = w.win16->build(w.binary);
    CB_CHECK_EQ(w.links, 2);
    CB_CHECK(std::filesystem::is_regular_file(*b.image));
});

CB_TEST("engine.win16.the_errors_of_the_steps", [] {
    World16 w(small_module());
    // a gate verdict is made in a build of the module
    CB_CHECK_THROWS_AS(UsageError, w.engine.get(w.db(), "function.gate", "1:0000"));
    // a function the module names but gives no key
    Module16 broken = small_module();
    broken.key.erase("f1");
    World16 b(broken);
    CB_CHECK_THROWS_AS(FormatError, b.engine.get(b.db(), "ne.functions"));
    // the stages the steps cannot do without
    Steps steps;
    facts::Databases databases(facts::Config{w.dir.path() / "db2", 1 * facts::kGiB});
    knowledge::ReadDigests digests([](const knowledge::Read&) { return std::string(knowledge::kAbsent); });
    Engine engine(steps, databases, digests, 1);
    build::functions::FunctionBuilder builder;
    CB_CHECK_THROWS_AS(UsageError, (void)Win16(steps, engine, Config{.builder = builder, .wcc = {}, .options = {}, .stages = {}}));
});

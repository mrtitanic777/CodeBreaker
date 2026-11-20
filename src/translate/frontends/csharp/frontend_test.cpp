// Tests of the C# front end's tool runner and of the C# kit's pairs, on the repo's C# corpus (tests/translate/corpus/csharp: the
// programs the kit was written against) and on programs written here.
//
// CsTreeDump is a C# program, so the tree it dumps for a corpus file is
// the checked-in one (testdata/csharp_<program>.dump.json), and its lowering the checked-in L2.
// Every test needs the .NET SDK and passes without it (a machine with none has no C# front end to test).
#include "translate/frontends/csharp/frontend.hpp"

#include <algorithm>
#include <chrono>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "translate/dotnet/pairs.hpp"
#include "translate/l2.hpp"

namespace {

namespace cs = cb::translate::frontends::csharp;
namespace dn = cb::translate::dotnet;
namespace ir = cb::translate::ir;
namespace stdfs = std::filesystem;
using cb::Json;

stdfs::path csf_state_folder() {
    const stdfs::path p = cb::testing::data_path("../state/work/csharp-front-tests");
    cb::fs::make_dirs(p);
    return p;
}

stdfs::path csf_corpus(const std::string& name = "") {
    return cb::testing::data_path("../tests/translate/corpus/csharp") / (name.empty() ? "" : name + ".cs");
}

stdfs::path csf_tool_source() { return cb::testing::data_path("../config/translate/frontends/csharp/CsTreeDump.cs"); }

// The SDK of this machine, if it has one that can compile and run.
const dn::Sdk* csf_installed() {
    static const std::optional<dn::Sdk> sdk = dn::Sdk::find();
    return sdk && sdk->available() ? &*sdk : nullptr;
}

cs::Frontend csf_frontend_of(const dn::Sdk& sdk) { return cs::Frontend(sdk, csf_tool_source(), csf_state_folder()); }

// A folder for one test, new, under the state folder.
struct CsfWork {
    stdfs::path path;
    explicit CsfWork(const std::string& name) : path(csf_state_folder() / name) {
        std::error_code ec;
        stdfs::remove_all(path, ec);
        cb::fs::make_dirs(path);
    }
    ~CsfWork() {
        std::error_code ec;
        stdfs::remove_all(path, ec);
    }
};

std::vector<stdfs::path> csf_corpus_files() { return cs::sources(csf_corpus()); }

}  // namespace

CB_TEST("translate.csharp.frontend.sources_lists_cs_files_sorted", [] {
    const auto files = csf_corpus_files();
    CB_CHECK(files.size() >= 10);
    CB_CHECK(std::is_sorted(files.begin(), files.end(), [](const stdfs::path& a, const stdfs::path& b) { return a.native() < b.native(); }));
    for (const auto& f : files) CB_CHECK_EQ(f.extension().string(), std::string(".cs"));
    CB_CHECK_EQ(cs::sources(csf_corpus("Shapes")).size(), std::size_t(1));
});

CB_TEST("translate.csharp.frontend.the_tool_is_compiled_once_and_kept", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    const stdfs::path dll = frontend.tool();
    CB_CHECK(stdfs::is_regular_file(dll));
    const auto time = cb::fs::last_write_time(dll);
    CB_CHECK(frontend.tool() == dll);
    CB_CHECK(cb::fs::last_write_time(dll) == time);   // not compiled again
    CB_CHECK(dll.parent_path().filename().string().starts_with("csdump-"));
});

CB_TEST("translate.csharp.frontend.dumps_are_the_recorded_trees", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("dumps");
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    for (const char* program : {"Linq", "Numbers", "Records", "Shapes"}) {
        const cs::Dumped dumped = frontend.dump({csf_corpus(program)}, work.path / program);
        CB_CHECK(dumped.errors().empty());
        CB_CHECK_EQ(dumped.files.size(), std::size_t(1));
        CB_CHECK_EQ(dumped.files[0].name, std::string(program));
        Json golden = cb::json::load(cb::testing::data_path(std::string("translate/frontends/csharp/testdata/csharp_") + program + ".dump.json"));
        Json tree = dumped.files[0].tree;
        // the tree names the source file by its absolute path, which is where the checkout was when the golden was made
        CB_CHECK(tree.at("path").get<std::string>().ends_with(std::string(program) + ".cs"));
        CB_CHECK(golden.at("path").get<std::string>().ends_with(std::string(program) + ".cs"));
        tree.erase("path");
        golden.erase("path");
        CB_CHECK(tree == golden);
    }
});

CB_TEST("translate.csharp.frontend.lowers_every_corpus_program_to_valid_l2", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("lower");
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    cb::Arena arena;
    const auto files = csf_corpus_files();
    const cs::Program program = frontend.lower(arena, files, work.path);
    CB_CHECK_EQ(program.units.size(), files.size());
    CB_CHECK(program.refused.empty());
    for (ir::Node* unit : program.units) {
        CB_CHECK(ir::get<ir::Unit>(unit)->lang == std::string("csharp"));
        const Json doc = cb::translate::l2::to_json(*unit);       // round-trips, the fields included
        cb::Arena again;
        CB_CHECK(cb::translate::l2::to_json(*cb::translate::l2::from_json(again, doc)) == doc);
    }
    // the units come in order of the file name
    std::vector<std::string> names;
    for (ir::Node* unit : program.units) names.push_back(*ir::get<ir::Unit>(unit)->path);
    CB_CHECK(std::is_sorted(names.begin(), names.end()));
});

CB_TEST("translate.csharp.frontend.the_bound_tree_says_what_the_compiler_resolved", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("bound");
    cb::Arena arena;
    const cs::Program program = csf_frontend_of(*sdk).lower(arena, {csf_corpus("Shapes")}, work.path);
    std::set<std::string> properties;
    bool select_of_property = false;
    for (ir::Node* n : ir::walk(program.units.at(0))) {
        if (const auto* p = ir::get<ir::Property>(n); p != nullptr && p->sym) properties.insert(p->sym->name);
        if (const auto* s = ir::get<ir::Select>(n); s != nullptr && s->sym && s->sym->kind == ir::SymKind::Property) select_of_property = true;
    }
    for (const char* name : {"Id", "Name", "W", "H", "Made"}) CB_CHECK(properties.contains(name));
    CB_CHECK(select_of_property);   // a property read is a Select of a property symbol
});

CB_TEST("translate.csharp.frontend.a_compile_error_is_reported_with_its_place", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("error");
    const stdfs::path bad = work.path / "Bad.cs";
    cb::fs::write_text(bad, "class P {\n    static void Main() { int x = \"text\"; }\n}\n");
    cb::Arena arena;
    try {
        csf_frontend_of(*sdk).lower(arena, {bad}, work.path / "out");
        CB_CHECK(false);
    } catch (const cs::CompileError& e) {
        const std::string what = e.what();
        CB_CHECK(what.find("C# did not compile") != std::string::npos);
        CB_CHECK(what.find("Bad.cs") != std::string::npos);
        CB_CHECK(what.find(":2 ") != std::string::npos);
    }
});

CB_TEST("translate.csharp.frontend.what_cannot_be_carried_is_refused_with_its_reason", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("refused");
    const stdfs::path src = work.path / "Refused.cs";
    cb::fs::write_text(src,
                       "using System.Collections.Generic; using System.Threading.Tasks;\n"
                       "static class P {\n"
                       "    static IEnumerable<int> Count() { yield return 1; }\n"
                       "    static async Task<int> Later() { await Task.Delay(1); return 2; }\n"
                       "    static (int, int) Pair() => (1, 2);\n"
                       "    static void Main() { int Local() => 3; System.Console.WriteLine(Local()); }\n"
                       "}\n");
    cb::Arena arena;
    const cs::Program program = csf_frontend_of(*sdk).lower(arena, {src}, work.path / "out");
    std::set<std::string> kinds;
    for (const cs::Refused& r : program.refused) {
        kinds.insert(r.kind);
        CB_CHECK(r.where.starts_with("Refused:"));
    }
    for (const char* kind : {"tuple", "local-function"}) CB_CHECK(kinds.contains(kind));
    // (an iterator and an async method are lowered since 2026-11-15: yield and await are IR nodes, not refusals)
    for (const char* kind : {"iterator", "await"}) CB_CHECK(!kinds.contains(kind));
});

CB_TEST("translate.csharp.pairs.every_corpus_program_round_trips_to_equivalent", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    for (const stdfs::path& file : csf_corpus_files()) {
        CsfWork work("roundtrip-" + file.stem().string());
        cb::Arena arena;
        const dn::pairs::Pairs pairs{*sdk, frontend, arena};
        const dn::pairs::Verdict v = pairs.csharp_to_csharp({file}, work.path);
        CB_CHECK(v.verdict == dn::judge::Verdict::Equivalent);
        if (v.verdict != dn::judge::Verdict::Equivalent) std::fprintf(stderr, "%s: %s\n", file.stem().string().c_str(), v.detail.c_str());
        CB_CHECK(!v.files.empty());
        // the lines each program prints
        static const std::map<std::string, std::string> kLines = {{"Accounts", "12 lines"}, {"Bank", "12 lines"}, {"Generics", "8 lines"}, {"Inventory", "6 lines"}, {"Linq", "6 lines"},
                                                                  {"Numbers", "12 lines"}, {"Records", "12 lines"}, {"Shapes", "5 lines"}, {"Sieve", "7 lines"}, {"Text", "10 lines"}};
        CB_CHECK(kLines.contains(file.stem().string()) && v.detail == kLines.at(file.stem().string()));
    }
});

// The second back end out of the same L2: the language features (iterators, async/await, patterns, queries, nullable arithmetic ...) go
// through the hub into C# again and the written C# runs beside the original (docs/TRANSLATOR_DESIGN.md, phase 2).
CB_TEST("translate.csharp.pairs.language_features_round_trip_to_equivalent", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    const stdfs::path dir = cb::testing::data_path("../tests/translate/corpus/csharp_features");
    std::size_t programs = 0;
    for (const auto& entry : stdfs::directory_iterator(dir)) {
        if (entry.path().extension() != ".cs") continue;
        ++programs;
        CsfWork work("features-" + entry.path().stem().string());
        cb::Arena arena;
        const dn::pairs::Pairs pairs{*sdk, frontend, arena};
        const dn::pairs::Verdict v = pairs.csharp_to_csharp({entry.path()}, work.path);
        CB_CHECK(v.verdict == dn::judge::Verdict::Equivalent);
        if (v.verdict != dn::judge::Verdict::Equivalent) std::fprintf(stderr, "%s: %s\n", entry.path().stem().string().c_str(), v.detail.c_str());
    }
    CB_CHECK(programs >= 10);
});

CB_TEST("translate.csharp.pairs.a_refused_construct_is_a_refused_verdict", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    CsfWork work("pair-refused");
    const stdfs::path src = work.path / "Gap.cs";
    cb::fs::write_text(src,
                       "using System.Collections.Generic;\n"
                       "static class P {\n"
                       "    static (int, int) Pair() => (1, 2);\n"
                       "    static void Main() { System.Console.WriteLine(Pair().Item1); }\n"
                       "}\n");
    cb::Arena arena;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    const dn::pairs::Verdict v = dn::pairs::Pairs{*sdk, frontend, arena}.csharp_to_csharp({src}, work.path / "out");
    CB_CHECK(v.verdict == dn::judge::Verdict::Refused);
    CB_CHECK(!v.refused.empty());
    // the detail, word for word
    CB_CHECK_EQ(v.detail, std::string("Gap:3 tuple: a tuple; P.Pair tuple: a tuple"));
});

// Java -> C#: the Java lane's front end (javac's tree), the C# back end with Java's library, and the two programs run beside each other.
// The programs are the kit's Java corpus, judged equivalent with these line counts.
CB_TEST("translate.csharp.pairs.java_programs_round_trip_to_equivalent", [] {
    const dn::Sdk* sdk = csf_installed();
    const stdfs::path jdk = "C:/Program Files/Java/jdk-21/bin";
    const std::string javac = cb::fs::get_env("CB_JAVAC").value_or((jdk / "javac.exe").string());
    const std::string java = cb::fs::get_env("CB_JAVA").value_or((jdk / "java.exe").string());
    std::error_code ec;
    if (sdk == nullptr || !stdfs::is_regular_file(javac, ec) || !stdfs::is_regular_file(java, ec)) return;
    static const cb::java::apimap::Registry registry = cb::java::apimap::Registry::load(cb::testing::data_path("../config/java/api_maps"));
    dn::pairs::JavaSide side;
    side.tools = {.java = java, .javac = javac, .treedump_source = cb::testing::data_path("../config/translate/frontends/java/TreeDump.java"), .state = csf_state_folder()};
    side.registry = &registry;
    side.runtime_dir = cb::testing::data_path("../config/translate/dotnet/runtime");
    side.jdk = dn::judge::Jdk{.javac = javac, .jvm = java};
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    struct Case {
        const char* folder;
        const char* main;
        const char* lines;
    };
    for (const Case& c : {Case{"calc", "CalcMain", "17 lines"}, Case{"fixtures", "sample.FixtureMain", "15 lines"}, Case{"modern", "Library", "20 lines"}}) {
        CsfWork work(std::string("java-") + c.folder);
        cb::Arena arena;
        const dn::pairs::Verdict v = dn::pairs::Pairs{*sdk, frontend, arena}.java_to_csharp(cb::testing::data_path(std::string("../tests/translate/corpus/java/") + c.folder), c.main,
                                                                                            work.path, side);
        if (v.verdict != dn::judge::Verdict::Equivalent) std::fprintf(stderr, "%s: %s\n", c.folder, v.detail.c_str());
        CB_CHECK(v.verdict == dn::judge::Verdict::Equivalent);
        CB_CHECK(v.detail == c.lines);
        CB_CHECK(v.refused.empty());
    }
});

// C# -> C++03: what the C++03 path cannot carry is refused with the reason, before anything is built (the converter and host compiler of
// the Java lane are not needed for that, so stand-ins are given; building and running the C++ is the Java lane's host).
CB_TEST("translate.csharp.pairs.what_the_cpp03_path_cannot_carry_is_refused", [] {
    const dn::Sdk* sdk = csf_installed();
    if (sdk == nullptr) return;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    dn::pairs::CppSide cpp;
    cpp.converter = [](const stdfs::path&, const stdfs::path& out_dir, std::span<ir::Node* const>) {
        cb::fs::write_text(out_dir / "JavaRT.h", "// stand-in\n");
        return dn::cs2cpp::Converted{};
    };
    for (const auto& [program, kind] : {std::pair<const char*, const char*>{"Accounts", "runtime"}, {"Records", "record"}}) {
        CsfWork work(std::string("cpp-") + program);
        cb::Arena arena;
        const dn::pairs::Verdict v = dn::pairs::Pairs{*sdk, frontend, arena}.csharp_to_cpp({csf_corpus(program)}, work.path, cpp);
        CB_CHECK(v.verdict == dn::judge::Verdict::Refused);
        CB_CHECK(!v.translation.has_value());
        CB_CHECK(v.original.has_value() && v.original->ok);
        bool found = false;
        for (const dn::pairs::Refusal& r : v.refused) found = found || r.kind == kind;
        CB_CHECK(found);
    }
});

// A proof run on programs of any size, off by default: CB_CS_PROOF_OUT names a folder, CB_CS_PROOF_SPECS a
// list of NAME=SOURCE (a folder or a file) separated by `;`. Each program is dumped and lowered (compile errors do not stop it: the
// dump of a program that does not bind is a tree like any other), and its L2 documents and refusals are written under
// <out>/<name>/native_l2.json and native_refused.json, with the time each step took.
CB_TEST("translate.csharp.frontend.proof_run", [] {
    const auto out = cb::fs::get_env("CB_CS_PROOF_OUT");
    const auto specs = cb::fs::get_env("CB_CS_PROOF_SPECS");
    const dn::Sdk* sdk = csf_installed();
    if (!out || !specs || sdk == nullptr) return;
    const cs::Frontend frontend = csf_frontend_of(*sdk);
    for (const std::string_view spec : cb::text::split(*specs, ';')) {
        const std::size_t eq = spec.find('=');
        if (eq == std::string_view::npos) continue;
        const std::string name(spec.substr(0, eq));
        const stdfs::path dir = stdfs::path(*out) / name;
        const auto files = cs::sources(stdfs::path(std::string(spec.substr(eq + 1))));
        const auto t0 = std::chrono::steady_clock::now();
        const cs::Dumped dumped = frontend.dump(files, dir / "native_dump");
        const auto t1 = std::chrono::steady_clock::now();
        if (const auto repeat = cb::fs::get_env("CB_CS_PROOF_REPEAT")) {   // the same dump again, for timing: the tool, the run, the reading
            for (int i = 0; i < std::stoi(*repeat); ++i) {
                const auto a = std::chrono::steady_clock::now();
                (void)frontend.tool();
                const auto b = std::chrono::steady_clock::now();
                frontend.run(files, dir / "native_dump");
                const auto c = std::chrono::steady_clock::now();
                (void)cs::Frontend::load(dir / "native_dump");
                const auto d = std::chrono::steady_clock::now();
                std::fprintf(stderr, "  %s timing: tool %.3fs run %.3fs load %.3fs\n", name.c_str(), std::chrono::duration<double>(b - a).count(),
                             std::chrono::duration<double>(c - b).count(), std::chrono::duration<double>(d - c).count());
            }
        }
        cb::Arena arena;
        Json docs = Json::object();
        Json refused = Json::array();
        for (const cs::DumpedFile& f : dumped.files) {
            const cs::Lowered one = cs::lower(arena, f.tree, f.name);
            docs[f.name] = cb::translate::l2::to_json(*one.unit);
            for (const cs::Refusal& r : one.refused)
                refused.push_back(Json::array({f.name + ":" + std::to_string(r.line), r.kind ? Json(*r.kind) : Json(nullptr), r.why}));
        }
        if (const auto repeat = cb::fs::get_env("CB_CS_PROOF_REPEAT")) {   // the lowering alone, 20 times over, for timing
            const auto a = std::chrono::steady_clock::now();
            for (int i = 0; i < 20; ++i) {
                cb::Arena again;
                for (const cs::DumpedFile& f : dumped.files) (void)cs::lower(again, f.tree, f.name);
            }
            std::fprintf(stderr, "  %s lowering x20: %.3fs\n", name.c_str(), std::chrono::duration<double>(std::chrono::steady_clock::now() - a).count());
        }
        const auto t2 = std::chrono::steady_clock::now();
        cb::json::save(dir / "native_l2.json", docs, cb::json::Style::Compact);
        cb::json::save(dir / "native_refused.json", refused, cb::json::Style::Compact);
        std::fprintf(stderr, "native %s files %zu errors %zu refused %zu dump %.2fs lower %.2fs\n", name.c_str(), files.size(), dumped.errors().size(), refused.size(),
                     std::chrono::duration<double>(t1 - t0).count(), std::chrono::duration<double>(t2 - t1).count());
    }
});

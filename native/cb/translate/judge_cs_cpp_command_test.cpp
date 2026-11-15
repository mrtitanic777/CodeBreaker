// cb-unity: alone (test helpers named for this module)
// Tests of `judge-cs-cpp`: the command's shape, the score table, the C# -> C++17 back end on a program's checked-in IR (no SDK needed), and,
// where the machine has the .NET SDK and a C++ compiler, whole programs judged by running the C# and its C++17 translation side by side.
#include "cb/translate/judge_cs_cpp_command.hpp"

#include <algorithm>
#include <iostream>

#include <nlohmann/json.hpp>

#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/backends/csharp_cpp17.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace jcc = cb::translate::judge_cs_cpp;
namespace stdfs = std::filesystem;
namespace backend = cb::translate::backends::csharp_cpp17;
using cb::Json;

jcc::Environment jcct_environment() { return {cb::config::Config::load(cb::testing::data_path("../.."))}; }

stdfs::path jcct_work(const std::string& name) {
    const stdfs::path p = cb::testing::data_path("../../state/work/judge-cs-cpp-tests") / name;
    std::error_code ec;
    stdfs::remove_all(p, ec);
    cb::fs::make_dirs(p);
    return p;
}

// The machine has what the end-to-end tests need: the .NET SDK and a C++ compiler.
struct JcctTools {
    std::optional<cb::translate::dotnet::Sdk> sdk;
    std::optional<jcc::Cxx> cxx;
    bool ok() const { return sdk && sdk->available() && cxx; }
};
JcctTools jcct_tools() {
    JcctTools t;
    t.sdk = cb::translate::dotnet::Sdk::find();
    t.cxx = jcc::find_cxx("");
    return t;
}

}  // namespace

CB_TEST("translate.judge_cs_cpp.the_command_is_named_and_documented", [] {
    const cb::cli::Command c = jcc::command(jcct_environment());
    CB_CHECK_EQ(c.name, std::string("judge-cs-cpp"));
    CB_CHECK_EQ(c.area, std::string("Translate"));
    CB_CHECK(static_cast<bool>(c.handler));
    CB_CHECK(!c.summary.empty());
});

CB_TEST("translate.judge_cs_cpp.a_missing_compiler_is_none", [] {
    CB_CHECK(!jcc::find_cxx("no-such-folder/no-such-compiler.exe").has_value());
});

CB_TEST("translate.judge_cs_cpp.the_table_has_a_row_per_program", [] {
    jcc::Score a{.program = "Alpha", .lowers = true, .writes = true, .compiles = true, .agrees = true};
    jcc::Score b{.program = "Beta", .lowers = true, .writes = false, .detail = "declined: Beta.Run goto: a goto into a block | with a bar"};
    const std::string t = jcc::table({a, b});
    CB_CHECK(t.starts_with("| program | lowers | writes | compiles | agrees | detail |\n"));
    CB_CHECK(t.find("| Alpha | yes | yes | yes | yes |  |\n") != std::string::npos);
    CB_CHECK(t.find("| Beta | yes | NO | - | - | declined: Beta.Run goto: a goto into a block \\| with a bar |\n") != std::string::npos);
});

// The back end on the checked-in IR of the C# corpus: Shapes (classes, an interface, abstract members, properties, a static counter).
CB_TEST("translate.judge_cs_cpp.the_back_end_writes_shapes_from_its_ir", [] {
    const Json docs = cb::json::parse(cb::fs::read_text(cb::testing::data_path("translate/testdata/csharp_shapes.l2.json")));
    cb::Arena arena;
    std::vector<cb::translate::ir::Node*> units;
    for (const Json& u : docs) units.push_back(cb::translate::l2::from_json(arena, u));
    for (auto* u : units) cb::translate::ir::get<cb::translate::ir::Unit>(u)->lang = "csharp";
    const backend::Output out = backend::write(units);
    for (const auto& g : out.declined) std::cerr << g.text() << "\n";
    CB_CHECK(out.declined.empty());
    CB_CHECK(out.source.find("#include \"csrt.hpp\"") != std::string::npos);
    CB_CHECK(out.source.find("namespace Corpus::Shapes {") != std::string::npos);
    CB_CHECK(out.source.find("struct Rect : Shape") != std::string::npos);
    CB_CHECK(out.source.find("struct Shape : virtual cs::Object, IShape") != std::string::npos);
    CB_CHECK(out.source.find("virtual cs::String get_Name() = 0;") != std::string::npos);
    CB_CHECK(out.source.find("int main()") != std::string::npos);
});

CB_TEST("translate.judge_cs_cpp.the_back_end_declines_what_it_cannot_write_and_says_where", [] {
    const Json docs = cb::json::parse(cb::fs::read_text(cb::testing::data_path("translate/testdata/csharp_shapes.l2.json")));
    cb::Arena arena;
    std::vector<cb::translate::ir::Node*> units;
    for (const Json& u : docs) units.push_back(cb::translate::l2::from_json(arena, u));
    // a statement the front end could not lower stays Unsupported in the IR: the back end names it and writes nothing
    for (auto* n : cb::translate::ir::walk(units[0])) {
        if (auto* e = cb::translate::ir::get<cb::translate::ir::ExprStmt>(n); e != nullptr && cb::translate::ir::is<cb::translate::ir::Call>(e->expr)) {
            e->expr = cb::translate::ir::make(arena, cb::translate::ir::Unsupported{.kind = "await", .raw = "await x"});
            break;
        }
    }
    const backend::Output out = backend::write(units);
    CB_CHECK(out.source.empty());
    CB_CHECK(!out.declined.empty());
    CB_CHECK_EQ(out.declined[0].kind, std::string("await"));
    CB_CHECK(!out.declined[0].where.empty());
});

// Whole programs: C# on the .NET runtime beside the C++17 on the host compiler, the same output and exit code.
CB_TEST("translate.judge_cs_cpp.a_program_with_wrapping_arithmetic_and_text_agrees", [] {
    const JcctTools tools = jcct_tools();
    if (!tools.ok()) return;
    const stdfs::path work = jcct_work("small");
    const stdfs::path src = work / "Small.cs";
    cb::fs::write_text(src,
                       "using System;\n"
                       "using System.Collections.Generic;\n"
                       "public static class Program {\n"
                       "    public static void Main() {\n"
                       "        int big = int.MaxValue; big += 1;\n"
                       "        var xs = new List<string>(); xs.Add(\"b\"); xs.Add(\"a\");\n"
                       "        Console.WriteLine(\"wrap \" + big + \" \" + (-7 / 2) + \" \" + (-7 % 3) + \" \" + (1 << 33) + \" \" + string.Join(\",\", xs));\n"
                       "        double d = 2.5e9; Console.WriteLine(\"conv \" + (int)d + \" \" + 0.1 + \" \" + 1e20 + \" \" + true);\n"
                       "    }\n"
                       "}\n");
    const jcc::Score s = jcc::judge_program(*tools.sdk, jcct_environment(), *tools.cxx, "Small", {src}, work / "run");
    if (!s.agrees.value_or(false)) std::cerr << s.detail << "\n";
    CB_CHECK(s.lowers.value_or(false));
    CB_CHECK(s.writes.value_or(false));
    CB_CHECK(s.compiles.value_or(false));
    CB_CHECK(s.agrees.value_or(false));
});

CB_TEST("translate.judge_cs_cpp.a_program_that_throws_and_catches_agrees", [] {
    const JcctTools tools = jcct_tools();
    if (!tools.ok()) return;
    const stdfs::path work = jcct_work("throws");
    const stdfs::path src = work / "Throws.cs";
    cb::fs::write_text(src,
                       "using System;\n"
                       "public class Oops : Exception { public Oops(string m) : base(m) { } }\n"
                       "public static class Program {\n"
                       "    static int Div(int a, int b) { return a / b; }\n"
                       "    public static void Main() {\n"
                       "        try { Div(1, 0); } catch (DivideByZeroException e) { Console.WriteLine(\"caught \" + e.GetType().Name); }\n"
                       "        try { throw new Oops(\"x\"); } catch (Oops e) { Console.WriteLine(e.Message); } finally { Console.WriteLine(\"done\"); }\n"
                       "    }\n"
                       "}\n");
    const jcc::Score s = jcc::judge_program(*tools.sdk, jcct_environment(), *tools.cxx, "Throws", {src}, work / "run");
    if (!s.agrees.value_or(false)) std::cerr << s.detail << "\n";
    CB_CHECK(s.agrees.value_or(false));
});

// Every program of the C# corpus (and the two written for this back end: tests/translate/corpus/csharp_cpp) lowers, writes, compiles and agrees;
// the scoreboard in docs/TRANSLATOR_DESIGN.md lists each one's row.
CB_TEST("translate.judge_cs_cpp.the_corpus_programs_agree", [] {
    const JcctTools tools = jcct_tools();
    if (!tools.ok()) return;
    const stdfs::path corpus = cb::testing::data_path("../../tests/translate/corpus");
    for (const char* path : {"csharp/Shapes", "csharp/Accounts", "csharp/Bank", "csharp/Generics", "csharp/Inventory", "csharp/Linq", "csharp/Numbers", "csharp/Records", "csharp/Sieve", "csharp/Text",
                             "csharp_cpp/Collections", "csharp_cpp/Misc"}) {
        const std::string name = stdfs::path(path).stem().string();
        const stdfs::path work = jcct_work(name);
        const jcc::Score s = jcc::judge_program(*tools.sdk, jcct_environment(), *tools.cxx, name, {corpus / (std::string(path) + ".cs")}, work);
        if (!s.agrees.value_or(false)) std::cerr << name << ": " << s.detail << "\n";
        CB_CHECK(s.agrees.value_or(false));
    }
});

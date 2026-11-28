// Tests of the C# back end's plain lowering (dotnet/decompiler/csharp/l2_syntax_builder): L2 units with no IL facts written as C# through
// ILSpy's syntax tree and output visitor.
//
// The corpus programs are the C# front end's lowering of tests/translate/corpus/csharp (translate/testdata/csharp_NAME.l2.json, the inputs
// the old writer's goldens are made from). The expected C# (dotnet/testdata/l2_csharp/NAME.cs.txt) was reviewed against the old writer's
// golden: the same program, in the output visitor's layout. Each was also compiled with Roslyn and run against its original, with the same
// output (work/stage15/judge_cs.sh).
//
// With CB_L2CS_OUT set to a folder, each corpus test writes the C# it made there (NAME.cs), to compare with the old writer's golden.
#include "dotnet/decompiler/csharp/l2_syntax_builder.hpp"

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "translate/l2.hpp"

namespace {

namespace dcs = cb::dotnet::decompiler::csharp;
namespace ir = cb::translate::ir;

void l2cs_check_corpus(const std::string& name) {
    const cb::Json docs = cb::json::load(cb::testing::data_path("translate/testdata/csharp_" + name + ".l2.json"));
    cb::Arena arena;
    std::vector<ir::Node*> units;
    for (const cb::Json& doc : docs) units.push_back(cb::translate::l2::from_json(arena, doc));
    dcs::L2CSharpProgram p(units);
    const auto& files = p.files();
    CB_CHECK_EQ(files.size(), std::size_t{1});
    if (files.empty()) return;
    if (const auto out = cb::fs::get_env("CB_L2CS_OUT")) {
        cb::fs::make_dirs(*out);
        cb::fs::write_text(std::filesystem::path(*out) / (name + ".cs"), files[0].text);
    }
    const auto golden = cb::testing::data_path("dotnet/testdata/l2_csharp/" + name + ".cs.txt");
    if (std::filesystem::exists(golden)) CB_CHECK_EQ(files[0].text, cb::fs::read_text(golden));
    else CB_CHECK(false);
    for (const dcs::L2Gap& g : p.ledger()) std::fprintf(stderr, "%s: %s %s: %s\n", name.c_str(), g.where.c_str(), g.kind.c_str(), g.why.c_str());
    CB_CHECK(p.ledger().empty());
}

}  // namespace

CB_TEST("dotnet.l2_csharp.corpus.accounts", [] { l2cs_check_corpus("accounts"); });
CB_TEST("dotnet.l2_csharp.corpus.bank", [] { l2cs_check_corpus("bank"); });
CB_TEST("dotnet.l2_csharp.corpus.generics", [] { l2cs_check_corpus("generics"); });
CB_TEST("dotnet.l2_csharp.corpus.inventory", [] { l2cs_check_corpus("inventory"); });
CB_TEST("dotnet.l2_csharp.corpus.linq", [] { l2cs_check_corpus("linq"); });
CB_TEST("dotnet.l2_csharp.corpus.numbers", [] { l2cs_check_corpus("numbers"); });
CB_TEST("dotnet.l2_csharp.corpus.records", [] { l2cs_check_corpus("records"); });
CB_TEST("dotnet.l2_csharp.corpus.shapes", [] { l2cs_check_corpus("shapes"); });
CB_TEST("dotnet.l2_csharp.corpus.sieve", [] { l2cs_check_corpus("sieve"); });
CB_TEST("dotnet.l2_csharp.corpus.text", [] { l2cs_check_corpus("text"); });

// ---- Java units, through the rule set java (translate/rules/java_csharp) -----------------------------------------------------------
//
// The javac front end's units of the C# writer's Java programs (translate/testdata/csharp_java/NAME.l2.json, the inputs of
// translate.csharp_java.*), written through the rule set and the plain lowering. The expected text (dotnet/testdata/l2_csharp_java/
// NAME.cs.txt: each file under a marker line, the runtime files left out; NAME.ledger.txt) was compared with the old writer's: the
// programs with a Main build with Roslyn and print what the old translation printed (work/stage15/judge_java.sh).

namespace {

const cb::java::apimap::Registry& l2cs_registry() {
    static const cb::java::apimap::Registry reg = cb::java::apimap::Registry::load(cb::testing::data_path("../config/java/api_maps"));
    return reg;
}

void l2cs_check_java(const std::string& name) {
    const cb::Json docs = cb::json::load(cb::testing::data_path("translate/testdata/csharp_java/" + name + ".l2.json"));
    cb::Arena arena;
    std::vector<ir::Node*> units;
    for (const cb::Json& doc : docs) units.push_back(cb::translate::l2::from_json(arena, doc));
    dcs::L2CSharpOptions options;
    options.java_registry = &l2cs_registry();
    options.runtime_dir = cb::testing::data_path("../config/translate/dotnet/runtime");
    dcs::L2CSharpProgram p(units, options);
    std::string text, ledger;
    for (const dcs::L2File& f : p.files()) {
        if (f.name == "JavaRt.cs" || f.name == "JavaCollections.cs" || f.name == "JavaStreams.cs" || f.name == "JavaCharTypes.cs") continue;
        text += "// ==== " + f.name + " ====\n" + f.text;
    }
    for (const dcs::L2Gap& g : p.ledger()) ledger += g.where + "\t" + g.kind + "\t" + g.why + "\n";
    if (const auto out = cb::fs::get_env("CB_L2CS_OUT")) {
        const std::filesystem::path dir = std::filesystem::path(*out) / "java" / name;
        cb::fs::make_dirs(dir);
        for (const dcs::L2File& f : p.files()) cb::fs::write_text(dir / f.name, f.text);
        cb::fs::write_text(std::filesystem::path(*out) / "java" / (name + ".cs.txt"), text);
        cb::fs::write_text(std::filesystem::path(*out) / "java" / (name + ".ledger.txt"), ledger);
    }
    const auto golden = cb::testing::data_path("dotnet/testdata/l2_csharp_java/" + name + ".cs.txt");
    CB_CHECK(std::filesystem::exists(golden));
    if (!std::filesystem::exists(golden)) return;
    CB_CHECK_EQ(text, cb::fs::read_text(golden));
    CB_CHECK_EQ(ledger, cb::fs::read_text(cb::testing::data_path("dotnet/testdata/l2_csharp_java/" + name + ".ledger.txt")));
}

}  // namespace

CB_TEST("dotnet.l2_csharp.java.calc", [] { l2cs_check_java("calc"); });
CB_TEST("dotnet.l2_csharp.java.fixtures", [] { l2cs_check_java("fixtures"); });
CB_TEST("dotnet.l2_csharp.java.modern", [] { l2cs_check_java("modern"); });
CB_TEST("dotnet.l2_csharp.java.features", [] { l2cs_check_java("features"); });
CB_TEST("dotnet.l2_csharp.java.calls", [] { l2cs_check_java("calls"); });
CB_TEST("dotnet.l2_csharp.java.enum_object", [] { l2cs_check_java("enum_object"); });
CB_TEST("dotnet.l2_csharp.java.exception_cause", [] { l2cs_check_java("exception_cause"); });
CB_TEST("dotnet.l2_csharp.java.hash_order", [] { l2cs_check_java("hash_order"); });
CB_TEST("dotnet.l2_csharp.java.inner_outer", [] { l2cs_check_java("inner_outer"); });
CB_TEST("dotnet.l2_csharp.java.boxed_null", [] { l2cs_check_java("boxed_null"); });

// Tests of dotnet/decompiler's IL AST (stage 4 of docs/DOTNET_DECOMPILER_PLAN.md): every method of the test assemblies read into the
// ILAst as ILSpy's own ILReader reads it (testdata/oracle/DecompilerDump, mode ilast, with no references resolved, so the golden does
// not depend on the .NET installed); and the pieces under it (LongSet).
#include "dotnet/decompiler/il/il_ast_dump.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/util/long_set.hpp"

namespace cb::dotnet::decompiler {
namespace {

std::filesystem::path testdata(std::string_view relative) { return testing::data_path(std::string("dotnet/testdata/") + std::string(relative)); }

std::string first_difference(const std::string& actual, const std::string& expected) {
    std::istringstream a(actual), e(expected);
    std::string al, el;
    for (int line = 1;; ++line) {
        const bool ha = static_cast<bool>(std::getline(a, al)), he = static_cast<bool>(std::getline(e, el));
        if (!ha && !he) return "(the same lines; a difference in line endings)";
        if (al != el || ha != he) return std::format("line {}:\n  ours:  {}\n  ILSpy: {}", line, ha ? al : "<end>", he ? el : "<end>");
    }
}

class NoReferences final : public metadata::IAssemblyResolver {
public:
    std::shared_ptr<metadata::MetadataFile> resolve(const metadata::IAssemblyReference&) override { return nullptr; }
    std::shared_ptr<metadata::MetadataFile> resolve_module(const metadata::MetadataFile&, std::string_view) override { return nullptr; }
};

void check_il_ast(const std::filesystem::path& input, const std::filesystem::path& golden) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(input);
    NoReferences resolver;
    const typesystem::DecompilerTypeSystem ts(module, resolver);
    const std::string actual = il::dump_il_ast(ts);
    const std::string expected = testing::regold_golden(golden, actual);
    CB_CHECK(actual == expected);
    if (actual != expected) testing::record_failure(__FILE__, __LINE__, first_difference(actual, expected));
}

CB_TEST("dotnet.decompiler.il.pe32_matches_ilspy", [] { check_il_ast(testdata("inputs/Sample.dll"), testdata("golden/Sample.ilast.txt")); });
CB_TEST("dotnet.decompiler.il.pe32plus_matches_ilspy", [] { check_il_ast(testdata("inputs/Sample64.dll"), testdata("golden/Sample64.ilast.txt")); });

// ad hoc: CB_DN_IL_IN=<assembly> CB_DN_IL_OUT=<text> writes our dump of any assembly (references not resolved)
CB_TEST("dotnet.decompiler.il.wip_dump", [] {
    const auto in = fs::get_env("CB_DN_IL_IN");
    const auto out = fs::get_env("CB_DN_IL_OUT");
    if (!in || !out) return;
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(std::filesystem::path(*in));
    NoReferences resolver;
    const typesystem::DecompilerTypeSystem ts(module, resolver);
    std::ofstream(std::filesystem::path(*out), std::ios::binary) << il::dump_il_ast(ts);
});

CB_TEST("dotnet.decompiler.util.long_set", [] {
    using util::LongInterval;
    using util::LongSet;
    const LongSet labels(LongInterval(0, 5));
    CB_CHECK(labels.contains(0) && labels.contains(4) && !labels.contains(5));
    const LongSet rest = labels.invert();
    CB_CHECK_EQ(rest.to_string(), std::string("[long.MinValue..0),[5..long.MaxValue]"));
    CB_CHECK(rest.union_with(labels).set_equals(LongSet::universe()));
    CB_CHECK_EQ(LongSet::universe().count(), std::numeric_limits<std::uint64_t>::max());
    CB_CHECK_EQ(labels.add_offset(10).to_string(), std::string("[10..15)"));
    CB_CHECK(LongSet(std::vector<LongInterval>{LongInterval(3, 4), LongInterval(1, 3)}).set_equals(LongSet(LongInterval(1, 4))));
    CB_CHECK(labels.intersect_with(LongSet(LongInterval(3, 9))).set_equals(LongSet(LongInterval(3, 5))));
});

}  // namespace
}  // namespace cb::dotnet::decompiler

// Tests of dotnet/decompiler's type system (stage 3 of docs/DOTNET_DECOMPILER_PLAN.md): the test assemblies' type systems as ILSpy's own
// builds them (testdata/oracle/TypeSystemDump, with no references resolved, so the golden does not depend on the .NET installed), and
// the parsers under it (System.Reflection.Metadata's TypeName, target framework names, System.Decimal's digits).
#include "dotnet/decompiler/typesystem/type_system_dump.hpp"

#include <sstream>

#include "core/testing.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"
#include "dotnet/decompiler/util/decimal.hpp"
#include "dotnet/metadata/type_name.hpp"

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

void check_type_system(std::string_view name) {
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(testdata(std::string("inputs/") + std::string(name) + ".dll"));
    NoReferences resolver;
    const typesystem::DecompilerTypeSystem ts(module, resolver);
    const std::string actual = typesystem::dump_type_system(ts);
    const std::string expected = testing::regold_golden(testdata(std::string("golden/") + std::string(name) + ".typesystem.txt"), actual);
    CB_CHECK(actual == expected);
    if (actual != expected) testing::record_failure(__FILE__, __LINE__, first_difference(actual, expected));
}

CB_TEST("dotnet.decompiler.typesystem.pe32_matches_ilspy", [] { check_type_system("Sample"); });
CB_TEST("dotnet.decompiler.typesystem.pe32plus_matches_ilspy", [] { check_type_system("Sample64"); });

CB_TEST("dotnet.metadata.type_name", [] {
    using cb::dotnet::metadata::TypeName;
    const auto t = TypeName::try_parse("System.Collections.Generic.Dictionary`2[[System.Int32, mscorlib],[System.String]][], Lib, Version=1.2.3.4");
    CB_CHECK(t != nullptr);
    CB_CHECK(t->is_sz_array());
    const TypeName& generic = t->get_element_type();
    CB_CHECK(generic.is_constructed_generic_type());
    CB_CHECK_EQ(generic.get_generic_type_definition().full_name(), std::string("System.Collections.Generic.Dictionary`2"));
    CB_CHECK_EQ(generic.get_generic_arguments().size(), std::size_t{2});
    CB_CHECK_EQ(generic.get_generic_arguments()[0]->assembly_name()->name(), std::string("mscorlib"));
    CB_CHECK_EQ(t->assembly_name()->full_name(), std::string("Lib, Version=1.2.3.4"));
    CB_CHECK_EQ(generic.full_name(), std::string("System.Collections.Generic.Dictionary`2[[System.Int32, mscorlib],[System.String]]"));
    const auto nested = TypeName::try_parse("Outer`1+Inner*&");
    CB_CHECK(nested != nullptr && nested->is_by_ref() && nested->get_element_type().is_pointer());
    const TypeName& inner = nested->get_element_type().get_element_type();
    CB_CHECK(inner.is_nested());
    CB_CHECK_EQ(inner.name(), std::string("Inner"));
    CB_CHECK_EQ(inner.declaring_type().full_name(), std::string("Outer`1"));
    CB_CHECK(TypeName::try_parse("") == nullptr);
    CB_CHECK(TypeName::try_parse("A[") == nullptr);
    CB_CHECK(TypeName::try_parse("A[,]")->get_array_rank() == 2);
});

CB_TEST("dotnet.decompiler.metadata.target_framework", [] {
    const auto [id, version] = metadata::parse_target_framework(".NETCoreApp,Version=v8.0");
    CB_CHECK(id == metadata::TargetFrameworkIdentifier::NET);
    CB_CHECK_EQ(version.to_string(), std::string("8.0.0"));
    CB_CHECK(metadata::parse_target_framework(".NETFramework,Version=v4.7.2").first == metadata::TargetFrameworkIdentifier::NETFramework);
    CB_CHECK(metadata::parse_target_framework("").second.to_string() == "0.0.0.0");
    const auto module = metadata::PEFile::open(testdata("inputs/Sample.dll"));
    CB_CHECK_EQ(metadata::detect_target_framework_id(*module), std::string(".NETCoreApp,Version=v9.0"));
});

CB_TEST("dotnet.decompiler.util.decimal", [] {
    CB_CHECK_EQ(util::Decimal(150, 0, 0, false, 2).to_string(), std::string("1.50"));
    CB_CHECK_EQ(util::Decimal(5, 0, 0, true, 3).to_string(), std::string("-0.005"));
    CB_CHECK_EQ(util::Decimal(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, false, 0).to_string(), std::string("79228162514264337593543950335"));
    CB_CHECK_EQ(util::Decimal(0, 0, 0, false, 0).to_string(), std::string("0"));
});

}  // namespace
}  // namespace cb::dotnet::decompiler

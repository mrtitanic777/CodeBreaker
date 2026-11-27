// Tests of dotnet/decompiler's ILAst transforms (stages 5 and 6 of docs/DOTNET_DECOMPILER_PLAN.md): every method's ILAst after the
// first N transforms of the pipeline, as ILSpy's own pipeline leaves it (testdata/oracle/DecompilerDump, mode `transforms N`).
#include <cstdio>
#include <fstream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "dotnet/decompiler/decompiler_settings.hpp"
#include "dotnet/decompiler/il/il_ast_dump.hpp"
#include "dotnet/decompiler/il/transforms/il_transform_pipeline.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"

namespace cb::dotnet::decompiler {
namespace {

class NoReferences final : public metadata::IAssemblyResolver {
public:
    std::shared_ptr<metadata::MetadataFile> resolve(const metadata::IAssemblyReference&) override { return nullptr; }
    std::shared_ptr<metadata::MetadataFile> resolve_module(const metadata::MetadataFile&, std::string_view) override { return nullptr; }
};

std::string dump_after(const typesystem::DecompilerTypeSystem& ts, int count) {
    const DecompilerSettings settings;
    const bool trace = fs::get_env("CB_DN_IL_TRACE").has_value();
    return il::dump_il_ast(ts, [&](il::ILFunction* function) {
        if (trace) std::fprintf(stderr, "%s\n", function->name.c_str());
        auto all = il::transforms::get_il_transforms();
        all.resize(std::min<std::size_t>(all.size(), static_cast<std::size_t>(count)));
        il::transforms::ILTransformContext context(function, ts, settings);
        function->run_transforms(all, context);
    });
}

// ad hoc: CB_DN_IL_IN=<assembly> CB_DN_IL_OUT=<text> CB_DN_IL_STEPS=<N> writes our dump after N transforms
CB_TEST("dotnet.decompiler.il.transforms_wip_dump", [] {
    const auto in = fs::get_env("CB_DN_IL_IN");
    const auto out = fs::get_env("CB_DN_IL_OUT");
    const auto steps = fs::get_env("CB_DN_IL_STEPS");
    if (!in || !out || !steps) return;
    const std::shared_ptr<metadata::PEFile> module = metadata::PEFile::open(std::filesystem::path(*in));
    NoReferences resolver;
    const typesystem::DecompilerTypeSystem ts(module, resolver);
    std::ofstream(std::filesystem::path(*out), std::ios::binary) << dump_after(ts, std::stoi(*steps));
});

}  // namespace
}  // namespace cb::dotnet::decompiler

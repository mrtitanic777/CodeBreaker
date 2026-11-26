// dotnet/decompiler/typesystem/decompiler_type_system.hpp -- the compilation the decompiler works in: the main assembly, every assembly it
// references (followed through type forwarders), the modules of a multi-module assembly, and a minimal corlib for the known types none
// of them defines.
//
//     auto file = PEFile::open("Game.exe");
//     UniversalAssemblyResolver resolver(file->file_name(), false, detect_target_framework_id(*file));
//     DecompilerTypeSystem ts(file, resolver);
//     ts.main_metadata_module().get_type_definition(TopLevelTypeName("Game", "Program"));
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: DecompilerTypeSystem), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). GetOptions(DecompilerSettings) comes with the settings; loading is synchronous (upstream's tasks only
// overlap the file reads, the result is the same).
#pragma once

#include <memory>
#include <vector>

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"

namespace cb::dotnet::decompiler::typesystem {

class DecompilerTypeSystem final : public SimpleCompilation {
public:
    // The files stay alive as long as the type system (it keeps them).
    DecompilerTypeSystem(std::shared_ptr<const metadata::MetadataFile> main_module, metadata::IAssemblyResolver& assembly_resolver,
                         TypeSystemOptions type_system_options = TypeSystemOptions::Default);
    ~DecompilerTypeSystem() override;

    const MetadataModule& main_metadata_module() const { return *main_module_; }
    TypeSystemOptions type_system_options() const override { return type_system_options_; }
    // The referenced files that were found and loaded, in the order they were.
    const std::vector<std::shared_ptr<metadata::MetadataFile>>& referenced_files() const { return referenced_files_; }

private:
    std::shared_ptr<const metadata::MetadataFile> main_file_;
    std::vector<std::shared_ptr<metadata::MetadataFile>> referenced_files_;
    std::vector<std::unique_ptr<IModuleReference>> module_references_;
    TypeSystemOptions type_system_options_;
    const MetadataModule* main_module_ = nullptr;
};

}  // namespace cb::dotnet::decompiler::typesystem

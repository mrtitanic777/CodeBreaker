// dotnet/decompiler/typesystem/decompiler_type_system.cpp -- loading the main assembly and its references into one compilation.
#include "dotnet/decompiler/typesystem/decompiler_type_system.hpp"

#include <deque>
#include <unordered_map>
#include <unordered_set>

#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/metadata/universal_assembly_resolver.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

// The references the compilation pulls in, not yet loaded: an assembly reference or a module (file) name, with the file that names it.
struct PendingReference {
    bool is_assembly;
    const metadata::MetadataFile* parent;
    std::shared_ptr<const metadata::IAssemblyReference> assembly;
    std::string module_name;
};

}  // namespace

DecompilerTypeSystem::DecompilerTypeSystem(std::shared_ptr<const metadata::MetadataFile> main_module, metadata::IAssemblyResolver& assembly_resolver,
                                           TypeSystemOptions type_system_options)
    : main_file_(std::move(main_module)), type_system_options_(type_system_options) {
    if (main_file_ == nullptr) throw UsageError("DecompilerTypeSystem: a null main module");
    const metadata::MetadataFile& main = *main_file_;
    const srm::MetadataReader& main_metadata = main.metadata();

    // the referenced assemblies and the type forwarders' assemblies (.NET Core and PCL binaries need them)
    std::vector<std::shared_ptr<metadata::MetadataFile>> referenced_assemblies;
    std::deque<PendingReference> queue;
    std::unordered_set<std::string> in_queue;   // "A:" + an assembly's full name, "M:" + a module's name
    const auto add_to_queue = [&](PendingReference r) {
        const std::string key = r.is_assembly ? "A:" + r.assembly->full_name() : "M:" + r.module_name;
        if (in_queue.insert(key).second) queue.push_back(std::move(r));
    };
    const auto [identifier, version] = metadata::parse_target_framework(metadata::detect_target_framework_id(main));
    for (const srm::ModuleReferenceHandle h : metadata::get_module_references(main_metadata)) {
        try {
            const std::string module_name(main_metadata.get_string(main_metadata.get_module_reference(h).name()));
            for (const srm::AssemblyFileHandle file_handle : main_metadata.assembly_files()) {
                const srm::AssemblyFile file = main_metadata.get_assembly_file(file_handle);
                if (main_metadata.get_string(file.name()) == module_name && file.contains_metadata()) {
                    add_to_queue({false, &main, nullptr, module_name});
                    break;
                }
            }
        } catch (const srm::BadImageFormatError&) {
        }
    }
    for (const srm::AssemblyReferenceHandle h : main_metadata.assembly_references())
        add_to_queue({true, &main, std::make_shared<metadata::AssemblyReference>(main_metadata, h), {}});
    while (!queue.empty()) {
        const PendingReference asm_ref = std::move(queue.front());
        queue.pop_front();
        std::shared_ptr<metadata::MetadataFile> asm_ = asm_ref.is_assembly ? assembly_resolver.resolve(*asm_ref.assembly)
                                                                           : assembly_resolver.resolve_module(*asm_ref.parent, asm_ref.module_name);
        if (asm_ != nullptr) {
            referenced_assemblies.push_back(asm_);
            const srm::MetadataReader& md = asm_->metadata();
            for (const srm::ExportedTypeHandle h : md.exported_types()) {
                const srm::EntityHandle implementation = md.get_exported_type(h).implementation();
                switch (implementation.kind()) {
                case srm::HandleKind::AssemblyReference:
                    add_to_queue({true, asm_.get(), std::make_shared<metadata::AssemblyReference>(md, srm::AssemblyReferenceHandle(implementation)), {}});
                    break;
                case srm::HandleKind::AssemblyFile:
                    add_to_queue({false, asm_.get(), nullptr, std::string(md.get_string(md.get_assembly_file(srm::AssemblyFileHandle(implementation)).name()))});
                    break;
                default: break;
                }
            }
        }
        if (queue.empty()) {
            // .NET Core and .NET 5+: the compile-time-only references the metadata does not name (DllImport, MarshalAs ... live there)
            switch (identifier) {
            case metadata::TargetFrameworkIdentifier::NETCoreApp:
            case metadata::TargetFrameworkIdentifier::NETStandard:
            case metadata::TargetFrameworkIdentifier::NET:
                for (const char* item : {"System.Runtime.InteropServices", "System.Runtime.CompilerServices.Unsafe"}) {
                    const bool existing = std::any_of(referenced_assemblies.begin(), referenced_assemblies.end(),
                                                      [&](const std::shared_ptr<metadata::MetadataFile>& f) { return f->name() == item; });
                    if (!existing) {
                        const std::string version3 = std::to_string(version.major) + "." + std::to_string(version.minor) + "." +
                                                     std::to_string(version.build < 0 ? 0 : version.build);
                        add_to_queue({true, &main,
                                      std::make_shared<metadata::AssemblyNameReference>(
                                          metadata::AssemblyNameReference::parse(std::string(item) + ", Version=" + version3 + ".0, Culture=neutral")),
                                      {}});
                    }
                }
                break;
            default: break;
            }
        }
    }
    const bool net7_or_later = identifier == metadata::TargetFrameworkIdentifier::NET && (version.major > 7 || (version.major == 7 && version.minor >= 0));
    if (!net7_or_later) type_system_options_ = type_system_options_ & ~TypeSystemOptions::NativeIntegersWithoutAttribute;

    module_references_.push_back(module_reference_with_options(main, type_system_options_));
    const IModuleReference* main_module_with_options = module_references_.back().get();

    // one module per assembly name: of the same name, the highest version is kept (where the first was)
    std::vector<const IModuleReference*> referenced_with_options;
    std::unordered_map<std::string, std::pair<srm::Version, std::size_t>> reference_assembly_version_map;
    for (const std::shared_ptr<metadata::MetadataFile>& file : referenced_assemblies) {
        if (file->is_assembly()) {
            const srm::Version new_file_version = file->metadata().get_assembly_definition().version();
            if (const auto it = reference_assembly_version_map.find(file->name()); it != reference_assembly_version_map.end()) {
                if (new_file_version >= it->second.first) {
                    module_references_.push_back(module_reference_with_options(*file, type_system_options_));
                    referenced_with_options[it->second.second] = module_references_.back().get();
                    it->second.first = new_file_version;
                }
                continue;
            }
            reference_assembly_version_map.emplace(file->name(), std::make_pair(new_file_version, referenced_with_options.size()));
        }
        module_references_.push_back(module_reference_with_options(*file, type_system_options_));
        referenced_with_options.push_back(module_references_.back().get());
    }

    // the known types none of the files defines (primitive types for the IL reader, Task<T> for async ...) come from a minimal corlib
    std::vector<const KnownTypeReference*> missing_known_types;
    for (const KnownTypeReference* known_type : KnownTypeReference::all()) {
        if (known_type == nullptr) continue;
        const TopLevelTypeName name = known_type->type_name();
        bool missing = main.get_type_definition(name).is_nil();
        for (std::size_t i = 0; missing && i < referenced_assemblies.size(); ++i) missing = referenced_assemblies[i]->get_type_definition(name).is_nil();
        if (missing) missing_known_types.push_back(known_type);
    }
    if (!missing_known_types.empty()) {
        module_references_.push_back(minimal_corlib::create_with_types(std::move(missing_known_types)));
        referenced_with_options.push_back(module_references_.back().get());
    }
    referenced_files_ = std::move(referenced_assemblies);
    init(*main_module_with_options, referenced_with_options);
    main_module_ = dynamic_cast<const MetadataModule*>(&SimpleCompilation::main_module());
}

DecompilerTypeSystem::~DecompilerTypeSystem() = default;

}  // namespace cb::dotnet::decompiler::typesystem

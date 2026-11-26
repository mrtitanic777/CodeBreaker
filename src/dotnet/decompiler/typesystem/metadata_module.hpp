// dotnet/decompiler/typesystem/metadata_module.hpp -- one module of a compilation read through the type system: its definitions (made once
// per row), what its references (TypeRef, MemberRef, MethodSpec, TypeSpec, standalone signatures) resolve to in the compilation, its
// namespaces, and its assembly and module attributes.
//
//     const IModuleReference& ref = *module_reference_with_options(pe_file, TypeSystemOptions::Default);
//     SimpleCompilation compilation(ref, {...});
//     const auto& module = static_cast<const MetadataModule&>(compilation.main_module());
//     const IMethod* m = module.resolve_method(member_ref_handle, GenericContext());
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: MetadataModule; TypeSystem.Implementation: MetadataNamespace;
// Metadata.MetadataFile.WithOptions), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Where upstream makes a new object per call (a VarArgInstanceMethod or FakeMethod per resolve, an attribute list per GetAttributes),
// the port makes it once and keeps it (in the module, the entity or the compilation's arena), so memory stays bounded. The Uncached
// option is therefore read as cached.
#pragma once

#include <array>
#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/typesystem/attributes.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/metadata_entities.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"

namespace cb::dotnet::decompiler::typesystem {

class MetadataNamespace;

class MetadataModule final : public IModule {
public:
    MetadataModule(const ICompilation& compilation, const metadata::MetadataFile& file, TypeSystemOptions options);
    ~MetadataModule() override;

    // IModule
    const ICompilation& compilation() const override { return *compilation_; }
    const metadata::MetadataFile* metadata_file() const override { return file_; }
    bool is_main_module() const override { return &compilation_->main_module() == this; }
    std::string assembly_name() const override { return assembly_name_; }
    srm::Version assembly_version() const override { return assembly_version_; }
    std::string full_assembly_name() const override { return full_assembly_name_; }
    std::string name() const override { return assembly_name_; }
    SymbolKind symbol_kind() const override { return SymbolKind::Module; }
    const INamespace& root_namespace() const override;
    std::vector<const ITypeDefinition*> top_level_type_definitions() const override;
    std::vector<const ITypeDefinition*> type_definitions() const override;
    // A top-level type of the module, or what a type forwarder of that name forwards to (nullptr for none).
    const ITypeDefinition* get_type_definition(const TopLevelTypeName& top_level_type_name) const override;
    std::vector<const IAttribute*> get_assembly_attributes() const override;
    std::vector<const IAttribute*> get_module_attributes() const override;
    // This module, or one named in an [InternalsVisibleTo] of this assembly (case-insensitively).
    bool internals_visible_to(const IModule& module) const override;

    TypeSystemOptions type_system_options() const { return options_; }
    const srm::MetadataReader& metadata() const { return *metadata_; }
    const TypeProvider& type_provider() const { return type_provider_; }
    Nullability nullable_context() const { return nullable_context_; }
    std::string get_string(srm::StringHandle name) const { return std::string(metadata_->get_string(name)); }
    const ITypeDefinition* get_type_definition(std::string_view namespace_name, std::string_view name, int type_parameter_count) const;

    // The definition of a row (nullptr for a nil handle); BadImageFormatError for a row past the table.
    const ITypeDefinition* get_definition(srm::TypeDefinitionHandle handle) const;
    const IField* get_definition(srm::FieldDefinitionHandle handle) const;
    const IMethod* get_definition(srm::MethodDefinitionHandle handle) const;
    const IProperty* get_definition(srm::PropertyDefinitionHandle handle) const;
    const IEvent* get_definition(srm::EventDefinitionHandle handle) const;

    // The module an assembly or module reference names in the compilation (nullptr for one it does not have, or a nil handle).
    const IModule* resolve_module(srm::AssemblyReferenceHandle handle) const;
    const IModule* resolve_module(srm::ModuleReferenceHandle handle) const;
    // The module a TypeRef's resolution scope names (this module for a scope in it).
    const IModule* get_declaring_module(srm::TypeReferenceHandle handle) const;

    // The type of a TypeDef, TypeRef, TypeSpec or ExportedType, with the attributes of its owner applied; UnknownType for a nil handle;
    // BadImageFormatError for a handle of another kind.
    const IType* resolve_type(srm::EntityHandle type_ref_def_spec, const GenericContext& context, const std::vector<srm::CustomAttributeHandle>* type_attributes = nullptr,
                              Nullability nullable_context = Nullability::Oblivious) const;
    const IType* resolve_type(srm::EntityHandle type_ref_def_spec, const GenericContext& context, TypeSystemOptions custom_options,
                              const std::vector<srm::CustomAttributeHandle>* type_attributes = nullptr, Nullability nullable_context = Nullability::Oblivious) const;
    // The method a MethodDef, MemberRef or MethodSpec stands for (a made-up FakeMethod where the declaring type has none that matches;
    // a VarArgInstanceMethod for a vararg call); UsageError for a nil handle, BadImageFormatError for one of another kind.
    const IMethod* resolve_method(srm::EntityHandle method_reference, const GenericContext& context) const;
    // Any entity a handle stands for (nullptr for a handle that is no entity); varargs are not expanded.
    const IEntity* resolve_entity(srm::EntityHandle entity_handle, const GenericContext& context = GenericContext()) const;
    // A calli's signature, and a method body's locals.
    std::pair<srm::SignatureHeader, const FunctionPointerType*> decode_method_signature(srm::StandaloneSignatureHandle handle, const GenericContext& generic_context) const;
    std::vector<const IType*> decode_local_signature(srm::StandaloneSignatureHandle handle, const GenericContext& generic_context) const;

    // The type of a known attribute in the compilation, and the attribute of it without arguments (one per module).
    const IType* get_attribute_type(KnownAttribute attr) const;
    const IAttribute* make_attribute(KnownAttribute type) const;

    // OnlyPublicAPI leaves out what other assemblies cannot see.
    bool include_internal_members() const { return !any(options_, TypeSystemOptions::OnlyPublicAPI); }
    bool is_visible_field(std::uint16_t field_attributes) const;
    bool is_visible_method(std::uint16_t method_attributes) const;
    // [NullablePublicOnly]: nullability is decoded only for entities this visible.
    bool should_decode_nullable_attributes(const IEntity* entity) const;
    TypeSystemOptions options_for_entity(const IEntity* entity) const;

    // The forwarded type an exported type names (an UnknownType where it is not found).
    const IType* resolve_forwarded_type(const srm::ExportedType& forwarder) const;

private:
    const IType* resolve_declaring_type(srm::EntityHandle declaring_type_reference, const GenericContext& context) const;
    const IType* introduce_tuple_types(const IType* type) const;
    const IMethod* resolve_method_definition(srm::MethodDefinitionHandle method_def_handle, bool expand_var_args) const;
    const IMethod* resolve_method_specification(srm::MethodSpecificationHandle method_spec_handle, const GenericContext& context, bool expand_var_args) const;
    const IMethod* resolve_method_reference(srm::MemberReferenceHandle member_ref_handle, const GenericContext& context,
                                            const std::vector<const IType*>* method_type_arguments, bool expand_var_args) const;
    const IMethod* create_fake_method(const IType* declaring_type, const std::string& name, const srm::MethodSignature<const IType*>& signature) const;
    const IField* resolve_field_reference(srm::MemberReferenceHandle member_reference_handle, const GenericContext& context) const;
    const IModule* resolve_module_of(const srm::ExportedType& type) const;
    const std::vector<std::string>& get_internals_visible_to() const;
    void add_type_forwarder_attributes(AttributeListBuilder& b) const;
    // A made object, kept by the module for as long as it lives.
    template <class T>
    const T* keep(std::unique_ptr<T> object) const;
    // The cache of resolves that make objects: (kind, token, the context's two parameter lists) -> what it resolved to.
    using ResolveKey = std::tuple<int, std::uint32_t, const void*, const void*>;
    template <class T, class Make>
    const T* cached_resolve(ResolveKey key, Make&& make) const;

    const ICompilation* compilation_;
    const metadata::MetadataFile* file_;
    const srm::MetadataReader* metadata_;
    TypeSystemOptions options_;
    TypeProvider type_provider_;
    Nullability nullable_context_ = Nullability::Oblivious;
    Accessibility min_accessibility_for_nrt_ = Accessibility::None;
    std::string assembly_name_, full_assembly_name_;
    srm::Version assembly_version_;
    std::unique_ptr<MetadataNamespace> root_namespace_;

    std::unique_ptr<std::atomic<const MetadataTypeDefinition*>[]> type_defs_;
    std::unique_ptr<std::atomic<const MetadataField*>[]> field_defs_;
    std::unique_ptr<std::atomic<const MetadataMethod*>[]> method_defs_;
    std::unique_ptr<std::atomic<const MetadataProperty*>[]> property_defs_;
    std::unique_ptr<std::atomic<const MetadataEvent*>[]> event_defs_;
    std::unique_ptr<std::atomic<const IModule*>[]> referenced_assemblies_;
    std::size_t type_def_count_ = 0, field_def_count_ = 0, method_def_count_ = 0, property_def_count_ = 0, event_def_count_ = 0, assembly_ref_count_ = 0;

    mutable std::array<std::atomic<const IType*>, kKnownAttributeCount> known_attribute_types_{};
    mutable std::array<std::atomic<const IAttribute*>, kKnownAttributeCount> known_attributes_{};
    mutable std::once_flag internals_visible_to_once_, assembly_attributes_once_, module_attributes_once_;
    mutable std::vector<std::string> internals_visible_to_;
    mutable AttributeList assembly_attributes_, module_attributes_;

    mutable std::mutex owned_mutex_;
    mutable std::vector<std::shared_ptr<const void>> owned_;
    mutable std::mutex resolve_mutex_;
    mutable std::map<ResolveKey, const void*> resolved_;
};

// A namespace of one module (its metadata NamespaceDefinition).
class MetadataNamespace final : public INamespace {
public:
    MetadataNamespace(const MetadataModule& module, const INamespace* parent, std::string full_name, srm::NamespaceDefinitionHandle ns);

    const INamespace* parent_namespace() const override { return parent_; }
    std::string full_name() const override { return full_name_; }
    std::string name() const override { return name_; }
    std::string extern_alias() const override { return {}; }
    std::vector<const INamespace*> child_namespaces() const override;
    std::vector<const ITypeDefinition*> types() const override;
    std::vector<const IModule*> contributing_modules() const override { return {module_}; }
    SymbolKind symbol_kind() const override { return SymbolKind::Namespace; }
    const ICompilation& compilation() const override { return module_->compilation(); }
    const INamespace* get_child_namespace(std::string_view name) const override;
    const ITypeDefinition* get_type_definition(std::string_view name, int type_parameter_count) const override;

private:
    const MetadataModule* module_;
    srm::NamespaceDefinitionHandle ns_;
    const INamespace* parent_;
    std::string full_name_, name_;
    mutable std::once_flag children_once_;
    mutable std::vector<std::unique_ptr<MetadataNamespace>> owned_children_;
    mutable std::vector<const INamespace*> children_;
};

// MetadataFile.WithOptions: a reference that resolves to a new MetadataModule of the file in the compilation that resolves it.
std::unique_ptr<IModuleReference> module_reference_with_options(const metadata::MetadataFile& file, TypeSystemOptions options);

}  // namespace cb::dotnet::decompiler::typesystem

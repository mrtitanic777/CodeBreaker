// dotnet/decompiler/typesystem/compilation.hpp -- a compilation: the main module and the modules it references, seen as one program
// (one merged namespace tree, one place to look up the known types), and the options the type system reads metadata with. Also the
// stand-in corlib a compilation gets when its references lack the known types (System.Object, System.Int32 ...).
//
//     SimpleCompilation c(main_module_reference, {corlib_reference});
//     c.find_type(KnownTypeCode::String);           // System.String from whichever module defines it
//     c.root_namespace().get_child_namespace("System");
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TypeSystemOptions, SimpleTypeResolveContext; TypeSystem.Implementation:
// SimpleCompilation, KnownTypeCache, MergedNamespace, MinimalCorlib), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Not ported: CacheManager (nothing reads it), NameComparer (always ordinal).
//
// Ownership: the modules an IModuleReference resolves to belong to the compilation (its arena keeps them).
#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/type_arena.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"

namespace cb::dotnet::decompiler::typesystem {

// What the type system decodes from metadata into C# concepts (dynamic, tuples, nint, nullability ...).
enum class TypeSystemOptions : std::uint32_t {
    None = 0,
    Dynamic = 1,
    Tuple = 2,
    ExtensionMethods = 4,
    OnlyPublicAPI = 8,
    Uncached = 0x10,
    DecimalConstants = 0x20,
    KeepModifiers = 0x40,
    ReadOnlyStructsAndParameters = 0x80,
    RefStructs = 0x100,
    UnmanagedConstraints = 0x200,
    NullabilityAnnotations = 0x400,
    ReadOnlyMethods = 0x800,
    NativeIntegers = 0x1000,
    FunctionPointers = 0x2000,
    ScopedRef = 0x4000,
    NativeIntegersWithoutAttribute = 0x8000,
    RefReadOnlyParameters = 0x10000,
    ParamsCollections = 0x20000,
    FirstClassSpanTypes = 0x40000,
    ExtensionMembers = 0x80000,
    RuntimeAsync = 0x100000,
    Default = Dynamic | Tuple | ExtensionMethods | DecimalConstants | ReadOnlyStructsAndParameters | RefStructs | UnmanagedConstraints |
              NullabilityAnnotations | ReadOnlyMethods | NativeIntegers | FunctionPointers | ScopedRef | NativeIntegersWithoutAttribute |
              RefReadOnlyParameters | ParamsCollections | FirstClassSpanTypes | ExtensionMembers | RuntimeAsync,
};
constexpr TypeSystemOptions operator|(TypeSystemOptions a, TypeSystemOptions b) {
    return static_cast<TypeSystemOptions>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}
constexpr TypeSystemOptions operator&(TypeSystemOptions a, TypeSystemOptions b) {
    return static_cast<TypeSystemOptions>(static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b));
}
constexpr TypeSystemOptions operator~(TypeSystemOptions a) { return static_cast<TypeSystemOptions>(~static_cast<std::uint32_t>(a)); }
// Any of the flags set (upstream's (options & flag) != 0).
constexpr bool any(TypeSystemOptions options, TypeSystemOptions flags) { return (options & flags) != TypeSystemOptions::None; }

// A resolve context of a compilation, module, type and member, as far as each is known.
class SimpleTypeResolveContext final : public ITypeResolveContext {
public:
    explicit SimpleTypeResolveContext(const ICompilation& compilation) : compilation_(&compilation) {}
    explicit SimpleTypeResolveContext(const IModule& module) : compilation_(&module.compilation()), current_module_(&module) {}
    explicit SimpleTypeResolveContext(const IEntity& entity);

    const ICompilation& compilation() const override { return *compilation_; }
    const IModule* current_module() const override { return current_module_; }
    const ITypeDefinition* current_type_definition() const override { return current_type_definition_; }
    const IMember* current_member() const override { return current_member_; }
    SimpleTypeResolveContext with_current_type_definition(const ITypeDefinition* type_definition) const;
    SimpleTypeResolveContext with_current_member(const IMember* member) const;

private:
    const ICompilation* compilation_;
    const IModule* current_module_ = nullptr;
    const ITypeDefinition* current_type_definition_ = nullptr;
    const IMember* current_member_ = nullptr;
};

// The known types of a compilation, each looked up once (an UnknownType when no module has it).
class KnownTypeCache {
public:
    explicit KnownTypeCache(const ICompilation& compilation) : compilation_(compilation) {}
    const IType* find_type(KnownTypeCode type_code) const;

private:
    const IType* search_type(KnownTypeCode type_code) const;
    const ICompilation& compilation_;
    mutable std::array<std::atomic<const IType*>, kKnownTypeCodeCount> known_types_{};
};

class SimpleCompilation : public ICompilation {
public:
    // UsageError when a reference does not resolve.
    SimpleCompilation(const IModuleReference& main_assembly, const std::vector<const IModuleReference*>& assembly_references);
    ~SimpleCompilation() override;

    // UsageError before init.
    const IModule& main_module() const override;
    const std::vector<const IModule*>& modules() const override;
    const std::vector<const IModule*>& referenced_modules() const override;
    const INamespace& root_namespace() const override;
    // The root namespace for no alias; extern aliases are not supported (nullptr).
    const INamespace* get_namespace_for_extern_alias(std::string_view alias) const override;
    const IType* find_type(KnownTypeCode type_code) const override;
    TypeSystemOptions type_system_options() const override { return TypeSystemOptions::Default; }
    TypeArena& arena() const override { return arena_; }
    std::string to_string() const;   // "[SimpleCompilation main]"

protected:
    SimpleCompilation();
    void init(const IModuleReference& main_assembly, const std::vector<const IModuleReference*>& assembly_references);
    virtual const INamespace* create_root_namespace() const;

private:
    mutable TypeArena arena_;
    const IModule* main_module_ = nullptr;
    std::unique_ptr<KnownTypeCache> known_type_cache_;
    std::vector<const IModule*> assemblies_, referenced_assemblies_;
    bool initialized_ = false;
    mutable std::once_flag root_once_;
    mutable const INamespace* root_namespace_ = nullptr;
};

// Namespaces of the same name in several modules, seen as one.
class MergedNamespace final : public INamespace {
public:
    MergedNamespace(const ICompilation& compilation, std::vector<const INamespace*> namespaces, std::string extern_alias = {});
    MergedNamespace(const INamespace& parent_namespace, std::vector<const INamespace*> namespaces);

    std::string extern_alias() const override { return extern_alias_; }
    std::string full_name() const override { return namespaces_[0]->full_name(); }
    std::string name() const override { return namespaces_[0]->name(); }
    const INamespace* parent_namespace() const override { return parent_namespace_; }
    std::vector<const ITypeDefinition*> types() const override;
    SymbolKind symbol_kind() const override { return SymbolKind::Namespace; }
    const ICompilation& compilation() const override { return *compilation_; }
    std::vector<const IModule*> contributing_modules() const override;
    std::vector<const INamespace*> child_namespaces() const override;
    const INamespace* get_child_namespace(std::string_view name) const override;
    // The first public one of the namespaces' types of that name, else the last one found.
    const ITypeDefinition* get_type_definition(std::string_view name, int type_parameter_count) const override;
    std::string to_string() const;

private:
    struct Children {
        std::vector<std::string> names;   // in the order first seen
        std::vector<const INamespace*> namespaces;
    };
    const Children& get_child_namespaces() const;

    std::string extern_alias_;
    const ICompilation* compilation_;
    const INamespace* parent_namespace_ = nullptr;
    std::vector<const INamespace*> namespaces_;
    mutable std::once_flag children_once_;
    mutable Children children_;
};

// The known types alone, without members: what a compilation falls back on for the known types none of its modules defines.
namespace minimal_corlib {
// A reference to a corlib of every known type, or of these ones.
const IModuleReference& instance();
std::unique_ptr<IModuleReference> create_with_types(std::vector<const KnownTypeReference*> types);
}

}  // namespace cb::dotnet::decompiler::typesystem

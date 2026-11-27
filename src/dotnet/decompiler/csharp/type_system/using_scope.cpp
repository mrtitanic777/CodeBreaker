#include "dotnet/decompiler/csharp/type_system/using_scope.hpp"

#include <stdexcept>

#include "dotnet/decompiler/typesystem/type_arena.hpp"

namespace cb::dotnet::decompiler::csharp::type_system {

using typesystem::IModule;
using typesystem::INamespace;
using typesystem::ITypeDefinition;
using typesystem::SymbolKind;

namespace {

// NamespaceDeclaration.BuildQualifiedName: the two names joined with a dot, an empty one dropped.
std::string build_qualified_name(const std::string& name1, const std::string& name2) {
    if (name1.empty()) return name2;
    if (name2.empty()) return name1;
    return name1 + "." + name2;
}

// A namespace that has no types or child namespaces: the one a namespace declaration names when the module has no such namespace.
class DummyNamespace final : public INamespace {
public:
    DummyNamespace(const INamespace* parent_namespace, std::string name) : parent_namespace_(parent_namespace), name_(std::move(name)) {}

    std::string extern_alias() const override { return {}; }
    std::string full_name() const override { return build_qualified_name(parent_namespace_->full_name(), name_); }
    std::string name() const override { return name_; }
    SymbolKind symbol_kind() const override { return SymbolKind::Namespace; }
    const INamespace* parent_namespace() const override { return parent_namespace_; }
    std::vector<const INamespace*> child_namespaces() const override { return {}; }
    std::vector<const ITypeDefinition*> types() const override { return {}; }
    std::vector<const IModule*> contributing_modules() const override { return {}; }
    const typesystem::ICompilation& compilation() const override { return parent_namespace_->compilation(); }
    const INamespace* get_child_namespace(std::string_view) const override { return nullptr; }
    const ITypeDefinition* get_type_definition(std::string_view, int) const override { return nullptr; }

private:
    const INamespace* parent_namespace_;
    std::string name_;
};

}  // namespace

UsingScope::UsingScope(const CSharpTypeResolveContext& context, const INamespace* ns, std::vector<const INamespace*> usings)
    : parent_context_(context), ns_(ns), usings_(std::move(usings)) {
    if (ns == nullptr) throw std::invalid_argument("namespace");
}

const std::vector<std::pair<std::string, semantics::ResolveResult*>>& UsingScope::using_aliases() const {
    static const std::vector<std::pair<std::string, semantics::ResolveResult*>> none;
    return none;
}

const std::vector<std::string>& UsingScope::extern_aliases() const {
    static const std::vector<std::string> none;
    return none;
}

bool UsingScope::has_alias(const std::string&) const { return false; }

UsingScope* UsingScope::with_nested_namespace(const std::string& simple_name) const {
    const INamespace* child = ns_->get_child_namespace(simple_name);
    if (child == nullptr) {
        child = ns_->compilation().arena().keep(std::make_unique<DummyNamespace>(ns_, simple_name));
    }
    return il::make<UsingScope>(parent_context_.with_using_scope(this), child, std::vector<const INamespace*>{});
}

bool UsingScope::try_get_resolve_cache(const std::string& identifier, semantics::ResolveResult*& result) const {
    const std::lock_guard lock(mutex_);
    const auto it = resolve_cache_.find(identifier);
    if (it == resolve_cache_.end()) return false;
    result = it->second;
    return true;
}

void UsingScope::try_add_resolve_cache(const std::string& identifier, semantics::ResolveResult* result) const {
    const std::lock_guard lock(mutex_);
    resolve_cache_.emplace(identifier, result);
}

const UsingScope::ExtensionMethodGroups* UsingScope::all_extension_methods() const {
    const std::lock_guard lock(mutex_);
    return all_extension_methods_.get();
}

const UsingScope::ExtensionMethodGroups& UsingScope::get_or_set_all_extension_methods(ExtensionMethodGroups groups) const {
    const std::lock_guard lock(mutex_);
    if (all_extension_methods_ == nullptr) all_extension_methods_ = std::make_unique<ExtensionMethodGroups>(std::move(groups));
    return *all_extension_methods_;
}

}  // namespace cb::dotnet::decompiler::csharp::type_system

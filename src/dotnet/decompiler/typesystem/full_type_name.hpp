// dotnet/decompiler/typesystem/full_type_name.hpp -- the name of a type without its assembly: a top-level type's namespace, name and type
// parameter count, then the names of the types nested in it. What ILSpy finds types by and writes their names from.
//
//     FullTypeName n("System.Collections.Generic.Dictionary`2+KeyCollection");
//     n.top_level_type_name().namespace_() == "System.Collections.Generic";
//     n.name() == "KeyCollection";   n.reflection_name() == "System.Collections.Generic.Dictionary`2+KeyCollection";
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem: TopLevelTypeName, TopLevelTypeNameComparer, FullTypeName, FullTypeNameComparer;
// ReflectionHelper.SplitTypeParameterCountFromReflectionName), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). The ordinal comparers are operator== and std::hash; the case-insensitive ones come with their users.
#pragma once

#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::dotnet::decompiler::typesystem {

// "List`1" -> "List" and 1; a name without a valid count stays as it is (count 0). As upstream, the count is read as int.TryParse
// reads it (white space and a sign allowed).
std::string split_type_parameter_count_from_reflection_name(std::string_view reflection_name, int& type_parameter_count);
std::string split_type_parameter_count_from_reflection_name(std::string_view reflection_name);

class TopLevelTypeName {
public:
    TopLevelTypeName() = default;
    TopLevelTypeName(std::string namespace_name, std::string name, int type_parameter_count = 0)
        : ns_(std::move(namespace_name)), name_(std::move(name)), type_parameter_count_(type_parameter_count) {}
    // From a reflection name ("System.Collections.Generic.List`1").
    explicit TopLevelTypeName(std::string_view reflection_name);

    const std::string& namespace_() const { return ns_; }
    const std::string& name() const { return name_; }
    int type_parameter_count() const { return type_parameter_count_; }
    std::string reflection_name() const;
    std::string to_string() const { return reflection_name(); }
    friend bool operator==(const TopLevelTypeName&, const TopLevelTypeName&) = default;

private:
    std::string ns_, name_;
    int type_parameter_count_ = 0;
};

class FullTypeName {
public:
    FullTypeName() = default;
    FullTypeName(TopLevelTypeName top_level) : top_level_(std::move(top_level)) {}   // NOLINT: implicit, as upstream's conversion
    // From a reflection name, nested types after '+' ("Outer`1+Inner").
    explicit FullTypeName(std::string_view reflection_name);

    const TopLevelTypeName& top_level_type_name() const { return top_level_; }
    bool is_nested() const { return nested_.has_value(); }
    int nesting_level() const { return nested_ ? static_cast<int>(nested_->size()) : 0; }
    const std::string& name() const { return nested_ ? nested_->back().name : top_level_.name(); }
    std::string reflection_name() const;
    // Namespace, name and nested names joined by '.', without type parameter counts.
    std::string full_name() const;
    int type_parameter_count() const;
    const std::string& get_nested_type_name(int nesting_level) const;
    int get_nested_type_additional_type_parameter_count(int nesting_level) const;
    // The type this one is nested in (UsageError for a top-level type).
    FullTypeName get_declaring_type() const;
    FullTypeName nested_type(std::string name, int additional_type_parameter_count) const;
    std::string to_string() const { return reflection_name(); }
    friend bool operator==(const FullTypeName& a, const FullTypeName& b);

private:
    struct NestedTypeName {
        std::string name;
        int additional_type_parameter_count = 0;
        friend bool operator==(const NestedTypeName&, const NestedTypeName&) = default;
    };
    FullTypeName(TopLevelTypeName top, std::vector<NestedTypeName> nested) : top_level_(std::move(top)), nested_(std::move(nested)) {}

    TopLevelTypeName top_level_;
    std::optional<std::vector<NestedTypeName>> nested_;   // nullopt for a top-level type (upstream's null array)
};

}  // namespace cb::dotnet::decompiler::typesystem

template <>
struct std::hash<cb::dotnet::decompiler::typesystem::TopLevelTypeName> {
    std::size_t operator()(const cb::dotnet::decompiler::typesystem::TopLevelTypeName& n) const noexcept {
        return std::hash<std::string>()(n.name()) ^ std::hash<std::string>()(n.namespace_()) ^ static_cast<std::size_t>(n.type_parameter_count());
    }
};
template <>
struct std::hash<cb::dotnet::decompiler::typesystem::FullTypeName> {
    std::size_t operator()(const cb::dotnet::decompiler::typesystem::FullTypeName& n) const noexcept {
        std::size_t h = std::hash<cb::dotnet::decompiler::typesystem::TopLevelTypeName>()(n.top_level_type_name());
        for (int i = 0; i < n.nesting_level(); ++i) h = h * 31 + (std::hash<std::string>()(n.name()) ^ static_cast<std::size_t>(n.type_parameter_count()));
        return h;
    }
};

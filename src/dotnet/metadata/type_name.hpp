// dotnet/metadata/type_name.hpp -- a type's name as reflection writes it ("Namespace.Outer+Inner`1[[System.Int32, mscorlib]][], Assembly,
// Version=1.0.0.0, Culture=neutral, PublicKeyToken=null"), parsed into its parts: generic arguments, array ranks, pointers and
// references, the nesting, and the assembly name. Custom attribute blobs spell System.Type arguments this way.
//
//     std::shared_ptr<const TypeName> t = TypeName::try_parse("System.Collections.Generic.List`1[[System.Int32]][]");
//     t->is_array();                            // true; t->get_element_type()->is_constructed_generic_type()
//     t->get_element_type()->get_generic_type_definition()->full_name() == "System.Collections.Generic.List`1"
//
// Ported from System.Reflection.Metadata (TypeName, TypeNameParser, TypeNameParserHelpers, TypeNameParseOptions, AssemblyNameInfo,
// AssemblyNameParser, AssemblyNameFormatter; dotnet/runtime v9.0.0), MIT, the .NET Foundation (see NOTICE), decision for decision. Text
// is UTF-8; the characters the grammar uses are ASCII, and white space is .NET's char.IsWhiteSpace.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/error.hpp"

namespace cb::dotnet::metadata {

// A type name whose assembly name is not one (FileLoadException: "The given assembly name was invalid."). .NET throws it even from
// TryParse.
class InvalidAssemblyNameError : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(InvalidAssemblyNameError)
};

// System.Reflection.AssemblyNameFlags (the bits an assembly name sets).
enum class AssemblyNameFlags : std::uint32_t { None = 0, PublicKey = 1, Retargetable = 0x100 };

// An assembly name as a type name spells it.
class AssemblyNameInfo {
public:
    // The parts of "Name, Version=..., Culture=..., PublicKeyToken=..."; nullopt for text that is not one.
    static std::optional<AssemblyNameInfo> try_parse(std::string_view assembly_name);

    const std::string& name() const { return name_; }
    // The version's parts as given (2 to 4 of them; none when the name has no version).
    const std::vector<std::uint16_t>& version() const { return version_; }
    const std::optional<std::string>& culture_name() const { return culture_name_; }   // "" for neutral
    std::uint32_t flags() const { return flags_; }
    // The public key (flags has PublicKey) or its token; nullopt when the name has neither.
    const std::optional<std::vector<std::uint8_t>>& public_key_or_token() const { return public_key_or_token_; }
    // The display name, written back as .NET writes it ("Name, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null").
    const std::string& full_name() const { return full_name_; }

private:
    std::string name_;
    std::vector<std::uint16_t> version_;
    std::optional<std::string> culture_name_;
    std::uint32_t flags_ = 0;
    std::optional<std::vector<std::uint8_t>> public_key_or_token_;
    std::string full_name_;
};

class TypeName {
public:
    // The parsed name; nullptr for text that is not a type name (TypeName.TryParse with the default options: at most 20 nodes);
    // InvalidAssemblyNameError for a name whose assembly part is not an assembly name.
    static std::shared_ptr<const TypeName> try_parse(std::string_view type_name);

    const std::string& assembly_qualified_name() const { return assembly_qualified_name_; }
    const AssemblyNameInfo* assembly_name() const { return assembly_name_ ? &*assembly_name_ : nullptr; }
    // The type this one is nested in; UsageError for a type that is not nested.
    const TypeName& declaring_type() const;
    const std::string& full_name() const { return full_name_; }
    const std::string& name() const { return name_; }
    bool is_array() const { return rank_or_modifier_ == kSZArray || rank_or_modifier_ > 0; }
    bool is_constructed_generic_type() const { return !generic_arguments_.empty(); }
    bool is_simple() const { return element_or_generic_type_ == nullptr; }
    bool is_by_ref() const { return rank_or_modifier_ == kByRef; }
    bool is_nested() const { return declaring_type_ != nullptr; }
    bool is_sz_array() const { return rank_or_modifier_ == kSZArray; }
    bool is_pointer() const { return rank_or_modifier_ == kPointer; }
    bool is_variable_bound_array_type() const { return rank_or_modifier_ >= 1; }
    int get_node_count() const;
    // UsageError where the type has none.
    const TypeName& get_element_type() const;
    const TypeName& get_generic_type_definition() const;
    int get_array_rank() const;
    const std::vector<std::shared_ptr<const TypeName>>& get_generic_arguments() const { return generic_arguments_; }

    static constexpr int kSZArray = -1;
    static constexpr int kPointer = -2;
    static constexpr int kByRef = -3;

    // (the parser's constructor: `full_name` is the whole spelled name; nested_name_length > 0 cuts a declaring type's name from it)
    TypeName(std::optional<std::string> full_name, std::optional<AssemblyNameInfo> assembly_name, std::shared_ptr<const TypeName> element_or_generic_type,
             std::shared_ptr<const TypeName> declaring_type, std::vector<std::shared_ptr<const TypeName>> generic_arguments, int rank_or_modifier,
             int nested_name_length);

private:
    int rank_or_modifier_;
    std::shared_ptr<const TypeName> element_or_generic_type_, declaring_type_;
    std::vector<std::shared_ptr<const TypeName>> generic_arguments_;
    std::optional<AssemblyNameInfo> assembly_name_;
    std::string full_name_, name_, assembly_qualified_name_;
};

}  // namespace cb::dotnet::metadata

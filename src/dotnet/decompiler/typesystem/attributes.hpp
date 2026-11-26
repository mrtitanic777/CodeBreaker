// dotnet/decompiler/typesystem/attributes.hpp -- an entity's attributes as C# sees them: the custom attributes of the metadata (decoded
// when first read), plus the ones the metadata keeps as flags and C# writes as attributes ([Serializable], [DllImport(...)],
// [StructLayout(...)], [MarshalAs(...)], security declarations ...), minus the ones C# writes as syntax ([Extension], [Dynamic],
// [ParamArray] ...) where the type system turns them into that.
//
//     AttributeListBuilder b(module);
//     b.add(KnownAttribute::Serializable);
//     b.add(type_definition.get_custom_attributes(), SymbolKind::TypeDefinition);
//     AttributeList list = b.build();             // list.attributes: what get_attributes() returns
//
// Ported from ILSpy (ICSharpCode.Decompiler.TypeSystem.Implementation: DefaultAttribute, CustomAttribute, AttributeListBuilder,
// AttributeBuilder, DecimalConstantHelper; TypeSystem: ComHelper; SRMExtensions.GetNullableContext), Copyright AlphaSierraPapa for the
// ILSpy team, MIT (see NOTICE in dotnet/decompiler).
//
// Ownership: an attribute list owns the attributes made for it (the module's shared argument-less attributes it only points to); an entity
// keeps its list, made the first time it is asked for, where upstream makes a new one on every call.
#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "dotnet/decompiler/typesystem/known_attributes.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::typesystem {

class MetadataModule;

using AttributeTypedArgument = srm::CustomAttributeTypedArgument<const IType*>;
using AttributeNamedArgument = srm::CustomAttributeNamedArgument<const IType*>;
using AttributeArgumentValue = srm::CustomAttributeArgumentValue<const IType*>;

// An attribute given by its type (or constructor) and arguments.
class DefaultAttribute final : public IAttribute {
public:
    // UsageError for a null type.
    DefaultAttribute(const IType* attribute_type, std::vector<AttributeTypedArgument> fixed_arguments, std::vector<AttributeNamedArgument> named_arguments);
    // UsageError for a null constructor, or a count of arguments that is not its parameters'.
    DefaultAttribute(const IMethod* constructor, std::vector<AttributeTypedArgument> fixed_arguments, std::vector<AttributeNamedArgument> named_arguments);

    const IType* attribute_type() const override { return attribute_type_; }
    bool has_decode_errors() const override { return false; }
    // The constructor given, else the attribute type's constructor whose parameter types are the fixed arguments' types (nullptr for none).
    const IMethod* constructor() const override;
    const std::vector<AttributeTypedArgument>& fixed_arguments() const override { return fixed_arguments_; }
    const std::vector<AttributeNamedArgument>& named_arguments() const override { return named_arguments_; }

private:
    const IType* attribute_type_;
    mutable std::atomic<const IMethod*> constructor_{nullptr};
    std::vector<AttributeTypedArgument> fixed_arguments_;
    std::vector<AttributeNamedArgument> named_arguments_;
};

// A custom attribute of the metadata; its value blob is decoded when first read (an unreadable one gives no arguments and
// has_decode_errors).
class CustomAttribute final : public IAttribute {
public:
    CustomAttribute(const MetadataModule& module, const IMethod* attr_ctor, srm::CustomAttributeHandle handle);

    const IType* attribute_type() const override;
    const IMethod* constructor() const override { return constructor_; }
    bool has_decode_errors() const override;
    const std::vector<AttributeTypedArgument>& fixed_arguments() const override;
    const std::vector<AttributeNamedArgument>& named_arguments() const override;
    srm::CustomAttributeHandle handle() const { return handle_; }

    // The field or property a named argument sets (the last one of that name), nullptr for none.
    static const IMember* member_for_named_argument(const IType* attribute_type, const AttributeNamedArgument& named_argument);

private:
    void decode_value() const;

    const MetadataModule* module_;
    const IMethod* constructor_;
    srm::CustomAttributeHandle handle_;
    mutable std::once_flag decoded_;
    mutable std::vector<AttributeTypedArgument> fixed_arguments_;
    mutable std::vector<AttributeNamedArgument> named_arguments_;
    mutable bool has_decode_errors_ = false;
};

// Attributes, and the ones among them this list owns.
struct AttributeList {
    std::vector<const IAttribute*> attributes;
    std::vector<std::unique_ptr<const IAttribute>> owned;
};

// The builder of one attribute with arguments (named arguments are a field's when the type has a field of that name, else a property's).
class AttributeBuilder {
public:
    AttributeBuilder(const MetadataModule& module, KnownAttribute attribute_type);
    AttributeBuilder(const MetadataModule& module, const IType* attribute_type);
    void add_fixed_arg(AttributeTypedArgument arg) { fixed_args_.push_back(std::move(arg)); }
    void add_fixed_arg(KnownTypeCode type, AttributeArgumentValue value);
    void add_fixed_arg(const TopLevelTypeName& type, AttributeArgumentValue value);
    void add_fixed_arg(const IType* type, AttributeArgumentValue value) { fixed_args_.push_back({type, std::move(value)}); }
    void add_named_arg(std::string name, KnownTypeCode type, AttributeArgumentValue value);
    void add_named_arg(std::string name, const TopLevelTypeName& type, AttributeArgumentValue value);
    void add_named_arg(std::string name, const IType* type, AttributeArgumentValue value);
    std::unique_ptr<const IAttribute> build();

private:
    const ICompilation* compilation_;
    const IType* attribute_type_;
    std::vector<AttributeTypedArgument> fixed_args_;
    std::vector<AttributeNamedArgument> named_args_;
};

class AttributeListBuilder {
public:
    explicit AttributeListBuilder(const MetadataModule& module) : module_(&module) {}

    void add(std::unique_ptr<const IAttribute> attr);
    // The module's shared attribute of that type, without arguments.
    void add(KnownAttribute type);
    void add(KnownAttribute type, KnownTypeCode arg_type, AttributeArgumentValue arg_value);
    void add(KnownAttribute type, const TopLevelTypeName& arg_type, AttributeArgumentValue arg_value);
    void add(KnownAttribute type, std::vector<AttributeTypedArgument> fixed_arguments);
    // [MarshalAs(...)] from a marshalling descriptor (nothing for a nil one).
    void add_marshal_info(srm::BlobHandle marshal_info);
    // The custom attributes that C# writes as such for that kind of target.
    void add(const std::vector<srm::CustomAttributeHandle>& attributes, SymbolKind target);
    // Security declarations as attributes (unreadable ones are left out).
    void add_security_attributes(const std::vector<srm::DeclarativeSecurityAttributeHandle>& security_declarations);
    void add_security_attributes(const srm::DeclarativeSecurityAttribute& sec_decl);

    // Whether C# shows an attribute of that type on that kind of target as syntax rather than as an attribute.
    bool ignore_attribute(const TopLevelTypeName& attribute_type, SymbolKind target) const;
    // A custom known attribute among the handles that is not ignored for the target (`attribute` must be a custom attribute).
    bool has_attribute(const srm::MetadataReader& metadata, const std::vector<srm::CustomAttributeHandle>& custom_attributes, KnownAttribute attribute,
                       SymbolKind symbol_kind) const;
    std::unique_ptr<const IAttribute> get_attribute(const srm::MetadataReader& metadata, const std::vector<srm::CustomAttributeHandle>& custom_attributes,
                                                    KnownAttribute attribute, SymbolKind symbol_kind) const;

    AttributeList build() { return std::move(list_); }

private:
    bool ignore_attribute(const IType* attribute_type, SymbolKind target) const;
    std::unique_ptr<const IAttribute> convert_marshal_info(srm::BlobReader marshal_info) const;
    std::unique_ptr<const IAttribute> read_xml_security_attribute(srm::BlobReader& reader, const AttributeTypedArgument& security_action) const;
    std::unique_ptr<const IAttribute> read_binary_security_attribute(srm::BlobReader& reader, const AttributeTypedArgument& security_action) const;

    const MetadataModule* module_;
    AttributeList list_;
};

// The first of `list`'s attributes whose type is that known attribute (nullptr for none).
const IAttribute* find_attribute(const std::vector<const IAttribute*>& list, KnownAttribute attribute);
// AttributeListBuilder.GetAttribute against an entity's kept list: the attribute in `list` made from the first of the handles that is
// that custom known attribute and not ignored for the target (nullptr for none).
const IAttribute* find_custom_attribute(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& handles, KnownAttribute attribute,
                                        SymbolKind target, const std::vector<const IAttribute*>& list);

// [DecimalConstant(...)]: a decimal constant (a field or optional parameter whose value C# keeps in this attribute).
namespace decimal_constant_helper {
bool allows_decimal_constants(const MetadataModule& module);
bool is_decimal_constant(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& attribute_handles);
// The decimal, or null (monostate) when the attribute's arguments are not one.
ConstantValue get_decimal_constant_value(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& attribute_handles);
}

// [NullableContext(n)]'s n among the attributes (nullopt for none, or one that does not read).
std::optional<Nullability> get_nullable_context(const std::vector<srm::CustomAttributeHandle>& custom_attributes, const srm::MetadataReader& metadata);

// COM interop: an interface marked [ComImport], and the [CoClass] it is made by (UnknownType for none).
namespace com_helper {
bool is_com_import(const ITypeDefinition* type_definition);
const IType* get_co_class(const ITypeDefinition* type_definition);
}

// A constant as an attribute argument's value (a string becomes UTF-8; a decimal, which no attribute argument can be, null).
AttributeArgumentValue to_attribute_value(const ConstantValue& value);

}  // namespace cb::dotnet::decompiler::typesystem

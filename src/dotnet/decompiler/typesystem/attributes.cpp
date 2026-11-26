// dotnet/decompiler/typesystem/attributes.cpp -- attributes: the metadata's, the ones made from flags, the list builder.
#include "dotnet/decompiler/typesystem/attributes.hpp"

#include "dotnet/decompiler/metadata/assembly_references.hpp"
#include "dotnet/decompiler/metadata/custom_attribute_decoder.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"
#include "dotnet/decompiler/typesystem/compilation.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/metadata/custom_attribute_decoder.hpp"

namespace cb::dotnet::decompiler::typesystem {
namespace {

constexpr const char* kInteropServices = "System.Runtime.InteropServices";

bool is_method_like(SymbolKind kind) {
    switch (kind) {
    case SymbolKind::Method: case SymbolKind::Operator: case SymbolKind::Constructor: case SymbolKind::Destructor: case SymbolKind::Accessor: return true;
    default: return false;
    }
}

}  // namespace

// ---- DefaultAttribute ----------------------------------------------------------------------------------------------------

DefaultAttribute::DefaultAttribute(const IType* attribute_type, std::vector<AttributeTypedArgument> fixed_arguments, std::vector<AttributeNamedArgument> named_arguments)
    : attribute_type_(attribute_type), fixed_arguments_(std::move(fixed_arguments)), named_arguments_(std::move(named_arguments)) {
    if (attribute_type == nullptr) throw UsageError("DefaultAttribute: a null attribute type");
}

DefaultAttribute::DefaultAttribute(const IMethod* constructor, std::vector<AttributeTypedArgument> fixed_arguments, std::vector<AttributeNamedArgument> named_arguments)
    : attribute_type_(nullptr), fixed_arguments_(std::move(fixed_arguments)), named_arguments_(std::move(named_arguments)) {
    if (constructor == nullptr) throw UsageError("DefaultAttribute: a null constructor");
    constructor_.store(constructor);
    attribute_type_ = constructor->declaring_type() != nullptr ? constructor->declaring_type() : &SpecialType::unknown_type();
    if (fixed_arguments_.size() != constructor->parameters().size())
        throw UsageError("Positional argument count must match the constructor's parameter count");
}

const IMethod* DefaultAttribute::constructor() const {
    if (const IMethod* ctor = constructor_.load(std::memory_order_acquire)) return ctor;
    const std::size_t count = fixed_arguments_.size();
    const IMethod* ctor = nullptr;
    for (const IMethod* candidate : attribute_type_->get_constructors([count](const IMethod* m) { return m->parameters().size() == count; })) {
        bool same = true;
        for (std::size_t i = 0; i < count && same; ++i) same = type_equals(candidate->parameters()[i]->type(), fixed_arguments_[i].type);
        if (same) {
            ctor = candidate;
            break;
        }
    }
    constructor_.store(ctor, std::memory_order_release);
    return ctor;
}

// ---- CustomAttribute -----------------------------------------------------------------------------------------------------

CustomAttribute::CustomAttribute(const MetadataModule& module, const IMethod* attr_ctor, srm::CustomAttributeHandle handle)
    : module_(&module), constructor_(attr_ctor), handle_(handle) {}

const IType* CustomAttribute::attribute_type() const { return constructor_->declaring_type(); }

void CustomAttribute::decode_value() const {
    std::call_once(decoded_, [this] {
        try {
            const srm::MetadataReader& metadata = module_->metadata();
            srm::CustomAttributeValue<const IType*> value = metadata.get_custom_attribute(handle_).decode_value(module_->type_provider());
            fixed_arguments_ = std::move(value.fixed_arguments);
            named_arguments_ = std::move(value.named_arguments);
        } catch (const metadata::EnumUnderlyingTypeResolveError&) {
            fixed_arguments_.clear();
            named_arguments_.clear();
            has_decode_errors_ = true;   // never tried again
        } catch (const srm::BadImageFormatError&) {
            fixed_arguments_.clear();
            named_arguments_.clear();
            has_decode_errors_ = true;
        }
    });
}

bool CustomAttribute::has_decode_errors() const {
    decode_value();
    return has_decode_errors_;
}

const std::vector<AttributeTypedArgument>& CustomAttribute::fixed_arguments() const {
    decode_value();
    return fixed_arguments_;
}

const std::vector<AttributeNamedArgument>& CustomAttribute::named_arguments() const {
    decode_value();
    return named_arguments_;
}

const IMember* CustomAttribute::member_for_named_argument(const IType* attribute_type, const AttributeNamedArgument& named_argument) {
    const std::string name = named_argument.name.value_or(std::string());
    switch (named_argument.kind) {
    case srm::CustomAttributeNamedArgumentKind::Field: {
        const std::vector<const IField*> fields = attribute_type->get_fields([&](const IField* f) { return f->name() == name; });
        return fields.empty() ? nullptr : fields.back();
    }
    case srm::CustomAttributeNamedArgumentKind::Property: {
        const std::vector<const IProperty*> properties = attribute_type->get_properties([&](const IProperty* p) { return p->name() == name; });
        return properties.empty() ? nullptr : properties.back();
    }
    }
    return nullptr;
}

// ---- AttributeBuilder ----------------------------------------------------------------------------------------------------

AttributeBuilder::AttributeBuilder(const MetadataModule& module, KnownAttribute attribute_type)
    : AttributeBuilder(module, module.get_attribute_type(attribute_type)) {}

AttributeBuilder::AttributeBuilder(const MetadataModule& module, const IType* attribute_type)
    : compilation_(&module.compilation()), attribute_type_(attribute_type) {}

void AttributeBuilder::add_fixed_arg(KnownTypeCode type, AttributeArgumentValue value) { add_fixed_arg(compilation_->find_type(type), std::move(value)); }

void AttributeBuilder::add_fixed_arg(const TopLevelTypeName& type, AttributeArgumentValue value) {
    add_fixed_arg(find_type(*compilation_, FullTypeName(type)), std::move(value));
}

void AttributeBuilder::add_named_arg(std::string name, KnownTypeCode type, AttributeArgumentValue value) {
    add_named_arg(std::move(name), compilation_->find_type(type), std::move(value));
}

void AttributeBuilder::add_named_arg(std::string name, const TopLevelTypeName& type, AttributeArgumentValue value) {
    add_named_arg(std::move(name), find_type(*compilation_, FullTypeName(type)), std::move(value));
}

void AttributeBuilder::add_named_arg(std::string name, const IType* type, AttributeArgumentValue value) {
    const bool is_field = !attribute_type_->get_fields([&](const IField* f) { return f->name() == name; }, GetMemberOptions::ReturnMemberDefinitions).empty();
    named_args_.push_back({std::move(name), is_field ? srm::CustomAttributeNamedArgumentKind::Field : srm::CustomAttributeNamedArgumentKind::Property, type,
                           std::move(value)});
}

std::unique_ptr<const IAttribute> AttributeBuilder::build() {
    return std::make_unique<DefaultAttribute>(attribute_type_, std::move(fixed_args_), std::move(named_args_));
}

// ---- AttributeListBuilder ------------------------------------------------------------------------------------------------

void AttributeListBuilder::add(std::unique_ptr<const IAttribute> attr) {
    list_.attributes.push_back(attr.get());
    list_.owned.push_back(std::move(attr));
}

void AttributeListBuilder::add(KnownAttribute type) { list_.attributes.push_back(module_->make_attribute(type)); }

void AttributeListBuilder::add(KnownAttribute type, KnownTypeCode arg_type, AttributeArgumentValue arg_value) {
    add(type, std::vector<AttributeTypedArgument>{{module_->compilation().find_type(arg_type), std::move(arg_value)}});
}

void AttributeListBuilder::add(KnownAttribute type, const TopLevelTypeName& arg_type, AttributeArgumentValue arg_value) {
    add(type, std::vector<AttributeTypedArgument>{{find_type(module_->compilation(), FullTypeName(arg_type)), std::move(arg_value)}});
}

void AttributeListBuilder::add(KnownAttribute type, std::vector<AttributeTypedArgument> fixed_arguments) {
    add(std::make_unique<DefaultAttribute>(module_->get_attribute_type(type), std::move(fixed_arguments), std::vector<AttributeNamedArgument>{}));
}

void AttributeListBuilder::add_marshal_info(srm::BlobHandle marshal_info) {
    if (marshal_info.is_nil()) return;
    add(convert_marshal_info(module_->metadata().get_blob_reader(marshal_info)));
}

std::unique_ptr<const IAttribute> AttributeListBuilder::convert_marshal_info(srm::BlobReader marshal_info) const {
    AttributeBuilder b(*module_, KnownAttribute::MarshalAs);
    const IType* unmanaged_type_type = find_type(module_->compilation(), FullTypeName(TopLevelTypeName(kInteropServices, "UnmanagedType")));
    int type = marshal_info.read_byte();
    b.add_fixed_arg(unmanaged_type_type, std::int32_t{type});
    int size = 0;
    switch (type) {
    case 0x1e:   // FixedArray
        size = marshal_info.try_read_compressed_integer().value_or(0);
        b.add_named_arg("SizeConst", KnownTypeCode::Int32, std::int32_t{size});
        if (marshal_info.remaining_bytes() > 0) {
            type = marshal_info.read_byte();
            if (type != 0x66) b.add_named_arg("ArraySubType", unmanaged_type_type, std::int32_t{type});   // (0x66: None)
        }
        break;
    case 0x1d:   // SafeArray
        if (marshal_info.remaining_bytes() > 0) {
            const int var_type = marshal_info.read_byte();
            if (var_type != 0)   // VarEnum.VT_EMPTY
                b.add_named_arg("SafeArraySubType", TopLevelTypeName(kInteropServices, "VarEnum"), std::int32_t{var_type});
        }
        break;
    case 0x2a: {   // NATIVE_TYPE_ARRAY
        type = marshal_info.remaining_bytes() > 0 ? marshal_info.read_byte() : 0x66;   // (Cecil uses NativeType.None as the default)
        if (type != 0x50) b.add_named_arg("ArraySubType", unmanaged_type_type, std::int32_t{type});   // (0x50: Max)
        const int size_parameter_index = marshal_info.try_read_compressed_integer().value_or(-1);
        size = marshal_info.try_read_compressed_integer().value_or(-1);
        const int size_parameter_multiplier = marshal_info.try_read_compressed_integer().value_or(-1);
        if (size >= 0) b.add_named_arg("SizeConst", KnownTypeCode::Int32, std::int32_t{size});
        if (size_parameter_multiplier != 0 && size_parameter_index >= 0)
            b.add_named_arg("SizeParamIndex", KnownTypeCode::Int16, static_cast<std::int16_t>(size_parameter_index));
        break;
    }
    case 0x2c: {   // CustomMarshaler
        marshal_info.read_serialized_string();   // guid
        marshal_info.read_serialized_string();   // unmanaged type
        const std::optional<std::string> managed_type = marshal_info.read_serialized_string();
        const std::optional<std::string> cookie = marshal_info.read_serialized_string();
        if (managed_type) b.add_named_arg("MarshalType", KnownTypeCode::String, *managed_type);
        if (cookie && !cookie->empty()) b.add_named_arg("MarshalCookie", KnownTypeCode::String, *cookie);
        break;
    }
    case 0x17:   // FixedSysString
        b.add_named_arg("SizeConst", KnownTypeCode::Int32, std::int32_t{marshal_info.read_compressed_integer()});
        break;
    default: break;
    }
    return b.build();
}

void AttributeListBuilder::add(const std::vector<srm::CustomAttributeHandle>& attributes, SymbolKind target) {
    const srm::MetadataReader& metadata = module_->metadata();
    for (const srm::CustomAttributeHandle handle : attributes) {
        const srm::CustomAttribute attribute = metadata.get_custom_attribute(handle);
        // attribute types are not open generic: no generic context is needed
        const IMethod* ctor = module_->resolve_method(attribute.constructor(), GenericContext());
        if (ignore_attribute(ctor->declaring_type(), target)) continue;
        add(std::make_unique<CustomAttribute>(*module_, ctor, handle));
    }
}

bool AttributeListBuilder::ignore_attribute(const IType* attribute_type, SymbolKind target) const {
    if (attribute_type->declaring_type() != nullptr || attribute_type->type_parameter_count() != 0) return false;
    return ignore_attribute(TopLevelTypeName(attribute_type->namespace_(), attribute_type->name()), target);
}

bool AttributeListBuilder::ignore_attribute(const TopLevelTypeName& attribute_type, SymbolKind target) const {
    const std::string& ns = attribute_type.namespace_();
    const std::string& name = attribute_type.name();
    if (ns == "System.Runtime.CompilerServices") {
        const TypeSystemOptions options = module_->type_system_options();
        if (name == "DynamicAttribute") return any(options, TypeSystemOptions::Dynamic);
        if (name == "NativeIntegerAttribute") return any(options, TypeSystemOptions::NativeIntegers);
        if (name == "TupleElementNamesAttribute") return any(options, TypeSystemOptions::Tuple);
        if (name == "ExtensionAttribute") return any(options, TypeSystemOptions::ExtensionMethods);
        if (name == "DecimalConstantAttribute")
            return any(options, TypeSystemOptions::DecimalConstants) && (target == SymbolKind::Field || target == SymbolKind::Parameter);
        if (name == "IsReadOnlyAttribute") {
            switch (target) {
            case SymbolKind::TypeDefinition:
            case SymbolKind::Parameter: return any(options, TypeSystemOptions::ReadOnlyStructsAndParameters);
            case SymbolKind::Method:
            case SymbolKind::Accessor: return any(options, TypeSystemOptions::ReadOnlyMethods);
            case SymbolKind::ReturnType:
            case SymbolKind::Property:
            case SymbolKind::Indexer:
            case SymbolKind::Field: return true;   // "ref readonly" is currently always active
            default: return false;
            }
        }
        if (name == "IsByRefLikeAttribute") return any(options, TypeSystemOptions::RefStructs) && target == SymbolKind::TypeDefinition;
        if (name == "IsUnmanagedAttribute") return any(options, TypeSystemOptions::UnmanagedConstraints) && target == SymbolKind::TypeParameter;
        if (name == "NullableAttribute") return any(options, TypeSystemOptions::NullabilityAnnotations);
        if (name == "NullableContextAttribute")
            return any(options, TypeSystemOptions::NullabilityAnnotations) && (target == SymbolKind::TypeDefinition || is_method_like(target));
        if (name == "ScopedRefAttribute") return any(options, TypeSystemOptions::ScopedRef) && target == SymbolKind::Parameter;
        if (name == "RequiresLocationAttribute") return any(options, TypeSystemOptions::RefReadOnlyParameters) && target == SymbolKind::Parameter;
        if (name == "ParamCollectionAttribute") return any(options, TypeSystemOptions::ParamsCollections) && target == SymbolKind::Parameter;
        return false;
    }
    if (ns == "System") return name == "ParamArrayAttribute" && target == SymbolKind::Parameter;
    return false;
}

bool AttributeListBuilder::has_attribute(const srm::MetadataReader& metadata, const std::vector<srm::CustomAttributeHandle>& custom_attributes,
                                         KnownAttribute attribute, SymbolKind symbol_kind) const {
    for (const srm::CustomAttributeHandle h : custom_attributes)
        if (is_known_attribute(metadata.get_custom_attribute(h), metadata, attribute)) return !ignore_attribute(get_type_name(attribute), symbol_kind);
    return false;
}

std::unique_ptr<const IAttribute> AttributeListBuilder::get_attribute(const srm::MetadataReader& metadata,
                                                                      const std::vector<srm::CustomAttributeHandle>& custom_attributes, KnownAttribute attribute,
                                                                      SymbolKind symbol_kind) const {
    for (const srm::CustomAttributeHandle h : custom_attributes) {
        const srm::CustomAttribute attr = metadata.get_custom_attribute(h);
        if (is_known_attribute(attr, metadata, attribute) && !ignore_attribute(get_type_name(attribute), symbol_kind)) {
            const IMethod* ctor = module_->resolve_method(attr.constructor(), GenericContext());
            return std::make_unique<CustomAttribute>(*module_, ctor, h);
        }
    }
    return nullptr;
}

void AttributeListBuilder::add_security_attributes(const std::vector<srm::DeclarativeSecurityAttributeHandle>& security_declarations) {
    const srm::MetadataReader& metadata = module_->metadata();
    for (const srm::DeclarativeSecurityAttributeHandle sec_decl : security_declarations) {
        if (sec_decl.is_nil()) continue;
        try {
            add_security_attributes(metadata.get_declarative_security_attribute(sec_decl));
        } catch (const metadata::EnumUnderlyingTypeResolveError&) {
            // resolve errors are ignored
        } catch (const srm::BadImageFormatError&) {
            // invalid security declarations are ignored
        }
    }
}

void AttributeListBuilder::add_security_attributes(const srm::DeclarativeSecurityAttribute& sec_decl) {
    const IType* security_action_type = find_type(module_->compilation(), FullTypeName(TopLevelTypeName("System.Security.Permissions", "SecurityAction")));
    const AttributeTypedArgument security_action{security_action_type, std::int32_t{sec_decl.action()}};
    srm::BlobReader reader = module_->metadata().get_blob_reader(sec_decl.permission_set());
    if (reader.read_byte() == '.') {
        // binary attributes
        const int attribute_count = reader.read_compressed_integer();
        for (int i = 0; i < attribute_count; ++i) add(read_binary_security_attribute(reader, security_action));
    } else {
        // the .NET 1.0 way: an XML-encoded attribute
        reader.reset();
        add(read_xml_security_attribute(reader, security_action));
    }
}

std::unique_ptr<const IAttribute> AttributeListBuilder::read_xml_security_attribute(srm::BlobReader& reader, const AttributeTypedArgument& security_action) const {
    const std::string xml = util::utf8(reader.read_utf16(reader.remaining_bytes()));
    AttributeBuilder b(*module_, KnownAttribute::PermissionSet);
    b.add_fixed_arg(security_action);
    b.add_named_arg("XML", KnownTypeCode::String, xml);
    return b.build();
}

std::unique_ptr<const IAttribute> AttributeListBuilder::read_binary_security_attribute(srm::BlobReader& reader, const AttributeTypedArgument& security_action) const {
    const std::optional<std::string> attribute_type_name = reader.read_serialized_string();
    // (upstream: a null name gives a null type, and DefaultAttribute's ArgumentNullException; here the declaration is unreadable)
    if (!attribute_type_name) throw srm::BadImageFormatError(0, "a security attribute without a type name");
    const IType* attribute_type = module_->type_provider().get_type_from_serialized_name(*attribute_type_name);
    reader.read_compressed_integer();   // (the specification seems to be wrong here; this is Cecil's reading)
    const int num_named = reader.read_compressed_integer();
    metadata::CustomAttributeDecoder<const TypeProvider> decoder(module_->type_provider(), module_->metadata());
    std::vector<AttributeNamedArgument> named_args = decoder.decode_named_arguments(reader, num_named);
    return std::make_unique<DefaultAttribute>(attribute_type, std::vector<AttributeTypedArgument>{security_action}, std::move(named_args));
}

const IAttribute* find_attribute(const std::vector<const IAttribute*>& list, KnownAttribute attribute) {
    for (const IAttribute* a : list)
        if (is_known_type(a->attribute_type(), attribute)) return a;
    return nullptr;
}

const IAttribute* find_custom_attribute(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& handles, KnownAttribute attribute,
                                        SymbolKind target, const std::vector<const IAttribute*>& list) {
    const srm::MetadataReader& metadata = module.metadata();
    const AttributeListBuilder b(module);
    for (const srm::CustomAttributeHandle h : handles) {
        if (!is_known_attribute(metadata.get_custom_attribute(h), metadata, attribute) || b.ignore_attribute(get_type_name(attribute), target)) continue;
        for (const IAttribute* a : list)
            if (const auto* ca = dynamic_cast<const CustomAttribute*>(a); ca != nullptr && ca->handle() == h) return a;
        return nullptr;
    }
    return nullptr;
}

// ---- DecimalConstantHelper -----------------------------------------------------------------------------------------------

namespace decimal_constant_helper {

bool allows_decimal_constants(const MetadataModule& module) { return any(module.type_system_options(), TypeSystemOptions::DecimalConstants); }

bool is_decimal_constant(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& attribute_handles) {
    return has_known_attribute(attribute_handles, module.metadata(), KnownAttribute::DecimalConstant);
}

namespace {

std::optional<util::Decimal> try_decode_decimal_constant_attribute(const MetadataModule& module, const srm::CustomAttribute& attribute) {
    const srm::CustomAttributeValue<const IType*> attr_value = attribute.decode_value(module.type_provider());
    if (attr_value.fixed_arguments.size() != 5) return std::nullopt;
    // (byte scale, byte sign, uint hi, uint mid, uint low) or (byte scale, byte sign, int hi, int mid, int low)
    const auto* scale = std::get_if<std::uint8_t>(&attr_value.fixed_arguments[0].value);
    const auto* sign = std::get_if<std::uint8_t>(&attr_value.fixed_arguments[1].value);
    if (scale == nullptr || sign == nullptr) return std::nullopt;
    const auto& a = attr_value.fixed_arguments;
    if (const auto* hi = std::get_if<std::uint32_t>(&a[2].value)) {
        const auto* mid = std::get_if<std::uint32_t>(&a[3].value);
        const auto* lo = std::get_if<std::uint32_t>(&a[4].value);
        if (mid != nullptr && lo != nullptr) return util::Decimal(*lo, *mid, *hi, *sign != 0, *scale);
    }
    if (const auto* hi = std::get_if<std::int32_t>(&a[2].value)) {
        const auto* mid = std::get_if<std::int32_t>(&a[3].value);
        const auto* lo = std::get_if<std::int32_t>(&a[4].value);
        if (mid != nullptr && lo != nullptr)
            return util::Decimal(static_cast<std::uint32_t>(*lo), static_cast<std::uint32_t>(*mid), static_cast<std::uint32_t>(*hi), *sign != 0, *scale);
    }
    return std::nullopt;
}

}  // namespace

ConstantValue get_decimal_constant_value(const MetadataModule& module, const std::vector<srm::CustomAttributeHandle>& attribute_handles) {
    const srm::MetadataReader& metadata = module.metadata();
    for (const srm::CustomAttributeHandle h : attribute_handles) {
        const srm::CustomAttribute attribute = metadata.get_custom_attribute(h);
        if (is_known_attribute(attribute, metadata, KnownAttribute::DecimalConstant)) {
            if (const std::optional<util::Decimal> d = try_decode_decimal_constant_attribute(module, attribute)) return *d;
            return std::monostate();
        }
    }
    return std::monostate();
}

}  // namespace decimal_constant_helper

// ---- GetNullableContext, ComHelper ---------------------------------------------------------------------------------------

std::optional<Nullability> get_nullable_context(const std::vector<srm::CustomAttributeHandle>& custom_attributes, const srm::MetadataReader& metadata) {
    for (const srm::CustomAttributeHandle handle : custom_attributes) {
        const srm::CustomAttribute custom_attribute = metadata.get_custom_attribute(handle);
        if (!is_known_attribute(custom_attribute, metadata, KnownAttribute::NullableContext)) continue;
        srm::CustomAttributeValue<const IType*> value;
        try {
            value = custom_attribute.decode_value(minimal_corlib_type_provider());
        } catch (const srm::BadImageFormatError&) {
            continue;
        } catch (const metadata::EnumUnderlyingTypeResolveError&) {
            continue;
        }
        if (value.fixed_arguments.size() == 1) {
            if (const auto* b = std::get_if<std::uint8_t>(&value.fixed_arguments[0].value); b != nullptr && *b <= 2) return static_cast<Nullability>(*b);
        }
    }
    return std::nullopt;
}

namespace com_helper {

bool is_com_import(const ITypeDefinition* type_definition) {
    return type_definition != nullptr && type_definition->kind() == TypeKind::Interface && type_definition->has_attribute(KnownAttribute::ComImport);
}

const IType* get_co_class(const ITypeDefinition* type_definition) {
    if (type_definition == nullptr) return &SpecialType::unknown_type();
    const IAttribute* co_class_attribute = type_definition->get_attribute(KnownAttribute::CoClass);
    if (co_class_attribute != nullptr && co_class_attribute->fixed_arguments().size() == 1) {
        if (const auto* ty = std::get_if<const IType*>(&co_class_attribute->fixed_arguments()[0].value); ty != nullptr && *ty != nullptr) return *ty;
    }
    return &SpecialType::unknown_type();
}

}  // namespace com_helper

AttributeArgumentValue to_attribute_value(const ConstantValue& value) {
    return std::visit(
        [](const auto& v) -> AttributeArgumentValue {
            using V = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<V, std::u16string>) return util::utf8(v);
            else if constexpr (std::is_same_v<V, util::Decimal>) return std::monostate();
            else return v;
        },
        value);
}

}  // namespace cb::dotnet::decompiler::typesystem

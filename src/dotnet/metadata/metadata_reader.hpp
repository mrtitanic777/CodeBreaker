// dotnet/metadata/metadata_reader.hpp -- the ECMA-335 metadata of an assembly or module (Partition II, chapter 24): the metadata
// root, its streams (#~ or #- tables, #Strings, #US, #GUID, #Blob) and a view of every table row, as System.Reflection.Metadata's
// MetadataReader gives them. This is what the ILSpy port (dotnet/decompiler) reads assemblies with.
//
//     MetadataReader md(metadata_bytes);                       // the bytes the CLR header's metadata directory points at
//     for (TypeDefinitionHandle h : md.type_definitions()) {
//         TypeDefinition t = md.get_type_definition(h);
//         md.get_string(t.name());                              // "Circle"
//         for (FieldDefinitionHandle f : t.get_fields()) ...
//     }
//
// The reader does not own the bytes. Every read checks its bounds: a damaged image throws BadImageFormatError (blob_reader.hpp).
// Signatures are decoded by signature_decoder.hpp, custom attribute values by custom_attribute_decoder.hpp; the views'
// decode_* members are defined there.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "core/bytes.hpp"
#include "dotnet/metadata/blob_reader.hpp"
#include "dotnet/metadata/handles.hpp"
#include "dotnet/metadata/tables.hpp"

namespace cb::dotnet::metadata {

class MetadataReader;

enum class HeapIndex : std::uint8_t { UserString = 0, String = 1, Blob = 2, Guid = 3 };
enum class MetadataKind : std::uint8_t { Ecma335 = 0, WindowsMetadata = 1, ManagedWindowsMetadata = 2 };

struct Guid {
    std::array<std::uint8_t, 16> bytes{};
    std::string to_string() const;   // "D" format: 8-4-4-4-12 lower-case hex, the first three groups little-endian
    friend bool operator==(const Guid&, const Guid&) = default;
};

struct Version {
    std::uint16_t major = 0, minor = 0, build = 0, revision = 0;
    std::string to_string() const;   // "1.0.0.0"
    friend auto operator<=>(const Version&, const Version&) = default;
};

// The handles of a type's fields, methods, parameters, properties or events: a run of rows, through the Ptr table where the
// metadata has one (an uncompressed #- stream).
template <class H>
class HandleList {
public:
    HandleList() = default;
    HandleList(const MetadataReader* md, std::uint32_t first, std::uint32_t count, TableIndex ptr_table, bool indirect)
        : md_(md), first_(first), count_(count), ptr_(ptr_table), indirect_(indirect) {}
    class iterator {
    public:
        using value_type = H;
        using difference_type = std::ptrdiff_t;
        iterator() = default;
        iterator(const HandleList* list, std::uint32_t i) : list_(list), i_(i) {}
        H operator*() const { return (*list_)[i_]; }
        iterator& operator++() { ++i_; return *this; }
        iterator operator++(int) { iterator t = *this; ++i_; return t; }
        friend bool operator==(const iterator& a, const iterator& b) { return a.i_ == b.i_; }

    private:
        const HandleList* list_ = nullptr;
        std::uint32_t i_ = 0;
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, count_); }
    std::uint32_t size() const { return count_; }
    bool empty() const { return count_ == 0; }
    H operator[](std::uint32_t i) const;

private:
    const MetadataReader* md_ = nullptr;
    std::uint32_t first_ = 0, count_ = 0;
    TableIndex ptr_ = TableIndex::Module;
    bool indirect_ = false;
};

// The rows of a whole table (MetadataReader::type_definitions() ...): rows 1 .. count.
template <class H>
class TableRows {
public:
    explicit TableRows(std::uint32_t count = 0) : count_(count) {}
    class iterator {
    public:
        using value_type = H;
        using difference_type = std::ptrdiff_t;
        iterator() = default;
        explicit iterator(std::uint32_t row) : row_(row) {}
        H operator*() const { return H(row_); }
        iterator& operator++() { ++row_; return *this; }
        iterator operator++(int) { iterator t = *this; ++row_; return t; }
        friend bool operator==(const iterator& a, const iterator& b) { return a.row_ == b.row_; }

    private:
        std::uint32_t row_ = 1;
    };
    iterator begin() const { return iterator(1); }
    iterator end() const { return iterator(count_ + 1); }
    std::uint32_t size() const { return count_; }
    bool empty() const { return count_ == 0; }

private:
    std::uint32_t count_ = 0;
};

// ---- the views of the rows ------------------------------------------------------------------------------------------------
// Each is a reader and a row; it reads its cells when asked (System.Reflection.Metadata's structs of the same names).

struct TypeLayout {
    std::int32_t size = 0;
    std::int32_t packing_size = 0;
    bool is_default() const { return size == 0 && packing_size == 0; }
};

class ModuleDefinition {
public:
    ModuleDefinition(const MetadataReader* md) : md_(md) {}
    int generation() const;
    StringHandle name() const;
    GuidHandle mvid() const;
    GuidHandle generation_id() const;
    GuidHandle base_generation_id() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
};

class TypeReference {
public:
    TypeReference(const MetadataReader* md, TypeReferenceHandle h) : md_(md), h_(h) {}
    EntityHandle resolution_scope() const;
    StringHandle name() const;
    StringHandle namespace_() const;

private:
    const MetadataReader* md_;
    TypeReferenceHandle h_;
};

class TypeDefinition {
public:
    TypeDefinition(const MetadataReader* md, TypeDefinitionHandle h) : md_(md), h_(h) {}
    TypeDefinitionHandle handle() const { return h_; }
    std::uint32_t attributes() const;   // TypeAttributes
    StringHandle name() const;
    StringHandle namespace_() const;
    EntityHandle base_type() const;
    bool is_nested() const { return (attributes() & 0x7) >= 2; }   // a nested visibility (TypeAttributes.VisibilityMask: 2 .. 7)
    TypeDefinitionHandle get_declaring_type() const;
    TypeLayout get_layout() const;
    HandleList<FieldDefinitionHandle> get_fields() const;
    HandleList<MethodDefinitionHandle> get_methods() const;
    HandleList<PropertyDefinitionHandle> get_properties() const;
    HandleList<EventDefinitionHandle> get_events() const;
    std::vector<TypeDefinitionHandle> get_nested_types() const;
    std::vector<InterfaceImplementationHandle> get_interface_implementations() const;
    std::vector<GenericParameterHandle> get_generic_parameters() const;
    std::vector<MethodImplementationHandle> get_method_implementations() const;
    std::vector<DeclarativeSecurityAttributeHandle> get_declarative_security_attributes() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    TypeDefinitionHandle h_;
};

class FieldDefinition {
public:
    FieldDefinition(const MetadataReader* md, FieldDefinitionHandle h) : md_(md), h_(h) {}
    std::uint16_t attributes() const;   // FieldAttributes
    StringHandle name() const;
    BlobHandle signature() const;
    TypeDefinitionHandle get_declaring_type() const;
    ConstantHandle get_default_value() const;
    int get_offset() const;   // FieldLayout, -1 when it has none
    int get_relative_virtual_address() const;   // FieldRVA, 0 when it has none
    BlobHandle get_marshalling_descriptor() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    FieldDefinitionHandle h_;
};

struct MethodImport {
    std::uint16_t attributes = 0;   // MethodImportAttributes
    StringHandle name;
    ModuleReferenceHandle module;
};

class MethodDefinition {
public:
    MethodDefinition(const MetadataReader* md, MethodDefinitionHandle h) : md_(md), h_(h) {}
    int relative_virtual_address() const;
    std::uint16_t impl_attributes() const;   // MethodImplAttributes
    std::uint16_t attributes() const;   // MethodAttributes
    StringHandle name() const;
    BlobHandle signature() const;
    HandleList<ParameterHandle> get_parameters() const;
    std::vector<GenericParameterHandle> get_generic_parameters() const;
    TypeDefinitionHandle get_declaring_type() const;
    MethodImport get_import() const;
    std::vector<DeclarativeSecurityAttributeHandle> get_declarative_security_attributes() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    MethodDefinitionHandle h_;
};

class Parameter {
public:
    Parameter(const MetadataReader* md, ParameterHandle h) : md_(md), h_(h) {}
    std::uint16_t attributes() const;   // ParameterAttributes
    int sequence_number() const;
    StringHandle name() const;
    ConstantHandle get_default_value() const;
    BlobHandle get_marshalling_descriptor() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    ParameterHandle h_;
};

class InterfaceImplementation {
public:
    InterfaceImplementation(const MetadataReader* md, InterfaceImplementationHandle h) : md_(md), h_(h) {}
    EntityHandle interface_() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    InterfaceImplementationHandle h_;
};

enum class MemberReferenceKind : std::uint8_t { Method = 0, Field = 1 };

class MemberReference {
public:
    MemberReference(const MetadataReader* md, MemberReferenceHandle h) : md_(md), h_(h) {}
    EntityHandle parent() const;
    StringHandle name() const;
    BlobHandle signature() const;
    MemberReferenceKind get_kind() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_method_signature(Provider& provider, const Context& context) const;
    template <class Provider, class Context>
    auto decode_field_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    MemberReferenceHandle h_;
};

class Constant {
public:
    Constant(const MetadataReader* md, ConstantHandle h) : md_(md), h_(h) {}
    ConstantTypeCode type_code() const;
    EntityHandle parent() const;
    BlobHandle value() const;

private:
    const MetadataReader* md_;
    ConstantHandle h_;
};

class CustomAttribute {
public:
    CustomAttribute(const MetadataReader* md, CustomAttributeHandle h) : md_(md), h_(h) {}
    EntityHandle parent() const;
    EntityHandle constructor() const;
    BlobHandle value() const;
    template <class Provider>
    auto decode_value(Provider& provider) const;

private:
    const MetadataReader* md_;
    CustomAttributeHandle h_;
};

class DeclarativeSecurityAttribute {
public:
    DeclarativeSecurityAttribute(const MetadataReader* md, DeclarativeSecurityAttributeHandle h) : md_(md), h_(h) {}
    std::uint16_t action() const;   // DeclarativeSecurityAction
    EntityHandle parent() const;
    BlobHandle permission_set() const;

private:
    const MetadataReader* md_;
    DeclarativeSecurityAttributeHandle h_;
};

enum class StandaloneSignatureKind : std::uint8_t { Method = 0, LocalVariables = 1 };

class StandaloneSignature {
public:
    StandaloneSignature(const MetadataReader* md, StandaloneSignatureHandle h) : md_(md), h_(h) {}
    BlobHandle signature() const;
    StandaloneSignatureKind get_kind() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_method_signature(Provider& provider, const Context& context) const;
    template <class Provider, class Context>
    auto decode_local_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    StandaloneSignatureHandle h_;
};

struct EventAccessors {
    MethodDefinitionHandle adder, remover, raiser;
    std::vector<MethodDefinitionHandle> others;
};

class EventDefinition {
public:
    EventDefinition(const MetadataReader* md, EventDefinitionHandle h) : md_(md), h_(h) {}
    std::uint16_t attributes() const;   // EventAttributes
    StringHandle name() const;
    EntityHandle type() const;
    EventAccessors get_accessors() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    EventDefinitionHandle h_;
};

struct PropertyAccessors {
    MethodDefinitionHandle getter, setter;
    std::vector<MethodDefinitionHandle> others;
};

class PropertyDefinition {
public:
    PropertyDefinition(const MetadataReader* md, PropertyDefinitionHandle h) : md_(md), h_(h) {}
    std::uint16_t attributes() const;   // PropertyAttributes
    StringHandle name() const;
    BlobHandle signature() const;
    ConstantHandle get_default_value() const;
    PropertyAccessors get_accessors() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    PropertyDefinitionHandle h_;
};

class MethodImplementation {
public:
    MethodImplementation(const MetadataReader* md, MethodImplementationHandle h) : md_(md), h_(h) {}
    TypeDefinitionHandle type() const;
    EntityHandle method_body() const;
    EntityHandle method_declaration() const;

private:
    const MetadataReader* md_;
    MethodImplementationHandle h_;
};

class ModuleReference {
public:
    ModuleReference(const MetadataReader* md, ModuleReferenceHandle h) : md_(md), h_(h) {}
    StringHandle name() const;

private:
    const MetadataReader* md_;
    ModuleReferenceHandle h_;
};

class TypeSpecification {
public:
    TypeSpecification(const MetadataReader* md, TypeSpecificationHandle h) : md_(md), h_(h) {}
    BlobHandle signature() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    template <class Provider, class Context>
    auto decode_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    TypeSpecificationHandle h_;
};

class AssemblyDefinition {
public:
    AssemblyDefinition(const MetadataReader* md) : md_(md) {}
    std::uint32_t hash_algorithm() const;   // AssemblyHashAlgorithm
    Version version() const;
    std::uint32_t flags() const;   // AssemblyFlags
    BlobHandle public_key() const;
    StringHandle name() const;
    StringHandle culture() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;
    std::vector<DeclarativeSecurityAttributeHandle> get_declarative_security_attributes() const;

private:
    const MetadataReader* md_;
};

class AssemblyReference {
public:
    AssemblyReference(const MetadataReader* md, AssemblyReferenceHandle h) : md_(md), h_(h) {}
    Version version() const;
    std::uint32_t flags() const;
    BlobHandle public_key_or_token() const;
    StringHandle name() const;
    StringHandle culture() const;
    BlobHandle hash_value() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    AssemblyReferenceHandle h_;
};

class AssemblyFile {
public:
    AssemblyFile(const MetadataReader* md, AssemblyFileHandle h) : md_(md), h_(h) {}
    bool contains_metadata() const;
    StringHandle name() const;
    BlobHandle hash_value() const;

private:
    const MetadataReader* md_;
    AssemblyFileHandle h_;
};

class ExportedType {
public:
    ExportedType(const MetadataReader* md, ExportedTypeHandle h) : md_(md), h_(h) {}
    std::uint32_t attributes() const;   // TypeAttributes
    bool is_forwarder() const;
    int get_type_definition_id() const;
    StringHandle name() const;
    StringHandle namespace_() const;
    EntityHandle implementation() const;

private:
    const MetadataReader* md_;
    ExportedTypeHandle h_;
};

class ManifestResource {
public:
    ManifestResource(const MetadataReader* md, ManifestResourceHandle h) : md_(md), h_(h) {}
    std::int64_t offset() const;
    std::uint32_t attributes() const;   // ManifestResourceAttributes
    StringHandle name() const;
    EntityHandle implementation() const;

private:
    const MetadataReader* md_;
    ManifestResourceHandle h_;
};

class GenericParameter {
public:
    GenericParameter(const MetadataReader* md, GenericParameterHandle h) : md_(md), h_(h) {}
    int index() const;
    std::uint16_t attributes() const;   // GenericParameterAttributes
    StringHandle name() const;
    EntityHandle parent() const;
    std::vector<GenericParameterConstraintHandle> get_constraints() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    GenericParameterHandle h_;
};

class GenericParameterConstraint {
public:
    GenericParameterConstraint(const MetadataReader* md, GenericParameterConstraintHandle h) : md_(md), h_(h) {}
    GenericParameterHandle parameter() const;
    EntityHandle type() const;
    std::vector<CustomAttributeHandle> get_custom_attributes() const;

private:
    const MetadataReader* md_;
    GenericParameterConstraintHandle h_;
};

class MethodSpecification {
public:
    MethodSpecification(const MetadataReader* md, MethodSpecificationHandle h) : md_(md), h_(h) {}
    EntityHandle method() const;
    BlobHandle signature() const;
    template <class Provider, class Context>
    auto decode_signature(Provider& provider, const Context& context) const;

private:
    const MetadataReader* md_;
    MethodSpecificationHandle h_;
};

// A namespace of the module (System.Reflection.Metadata's NamespaceDefinition): its last name part, its parent, the namespaces in it and
// its top-level types and exported types, in table order. The root is the global namespace (name "").
struct NamespaceDefinitionHandle {
    std::uint32_t index = 0;   // 1-based; 0 is nil, 1 the root
    bool is_nil() const { return index == 0; }
    friend bool operator==(NamespaceDefinitionHandle, NamespaceDefinitionHandle) = default;
};

class NamespaceDefinition {
public:
    NamespaceDefinition(const MetadataReader* md, NamespaceDefinitionHandle h) : md_(md), h_(h) {}
    std::string_view name() const;        // the last part ("Generic" of System.Collections.Generic)
    std::string_view full_name() const;   // the whole name
    NamespaceDefinitionHandle parent() const;
    const std::vector<NamespaceDefinitionHandle>& namespace_definitions() const;
    const std::vector<TypeDefinitionHandle>& type_definitions() const;
    const std::vector<ExportedTypeHandle>& exported_types() const;

private:
    const MetadataReader* md_;
    NamespaceDefinitionHandle h_;
};

// ---- the reader ---------------------------------------------------------------------------------------------------------

class MetadataReader {
public:
    // Read the metadata root at the start of `metadata` (BSJB ...). BadImageFormatError when it is not metadata.
    explicit MetadataReader(ByteView metadata);
    MetadataReader(const MetadataReader&) = delete;
    MetadataReader& operator=(const MetadataReader&) = delete;

    ByteView block() const { return md_; }
    const std::string& metadata_version() const { return version_; }
    MetadataKind metadata_kind() const { return kind_; }
    bool is_assembly() const { return row_count(TableIndex::Assembly) > 0; }
    bool is_minimal_delta() const { return minimal_delta_; }
    bool uses_ptr_tables() const { return uncompressed_; }   // a #- stream

    // ---- the tables' shape (MetadataReaderExtensions)
    std::uint32_t row_count(TableIndex t) const { return tables_[static_cast<std::size_t>(t)].rows; }
    int get_table_row_count(TableIndex t) const { return static_cast<int>(row_count(t)); }
    int get_table_row_size(TableIndex t) const { return tables_[static_cast<std::size_t>(t)].row_size; }
    int get_table_metadata_offset(TableIndex t) const { return static_cast<int>(tables_[static_cast<std::size_t>(t)].offset); }
    int get_heap_metadata_offset(HeapIndex h) const { return static_cast<int>(heaps_[static_cast<std::size_t>(h)].offset); }
    int get_heap_size(HeapIndex h) const { return static_cast<int>(heaps_[static_cast<std::size_t>(h)].size); }
    bool is_sorted(TableIndex t) const { return (sorted_ >> static_cast<int>(t) & 1) != 0; }

    // ---- the heaps
    std::string_view get_string(StringHandle h) const;   // UTF-8, as stored
    std::string_view get_string(Handle h) const { return get_string(StringHandle(h.value())); }
    bool string_equals(StringHandle h, std::string_view value) const { return get_string(h) == value; }
    bool string_is_empty(StringHandle h) const { return h.is_nil() || get_string(h).empty(); }
    std::u16string get_user_string(UserStringHandle h) const;
    ByteView get_blob_bytes(BlobHandle h) const;
    BlobReader get_blob_reader(BlobHandle h) const;
    Guid get_guid(GuidHandle h) const;
    // The next entry of a heap after `h` (nil at the end): what walks a heap entry by entry.
    StringHandle get_next_handle(StringHandle h) const;
    UserStringHandle get_next_handle(UserStringHandle h) const;
    BlobHandle get_next_handle(BlobHandle h) const;

    // ---- the tables' rows
    TableRows<TypeDefinitionHandle> type_definitions() const { return TableRows<TypeDefinitionHandle>(row_count(TableIndex::TypeDef)); }
    TableRows<TypeReferenceHandle> type_references() const { return TableRows<TypeReferenceHandle>(row_count(TableIndex::TypeRef)); }
    TableRows<FieldDefinitionHandle> field_definitions() const { return TableRows<FieldDefinitionHandle>(row_count(TableIndex::Field)); }
    TableRows<MethodDefinitionHandle> method_definitions() const { return TableRows<MethodDefinitionHandle>(row_count(TableIndex::MethodDef)); }
    TableRows<MemberReferenceHandle> member_references() const { return TableRows<MemberReferenceHandle>(row_count(TableIndex::MemberRef)); }
    TableRows<CustomAttributeHandle> custom_attributes() const { return TableRows<CustomAttributeHandle>(row_count(TableIndex::CustomAttribute)); }
    TableRows<DeclarativeSecurityAttributeHandle> declarative_security_attributes() const {
        return TableRows<DeclarativeSecurityAttributeHandle>(row_count(TableIndex::DeclSecurity));
    }
    TableRows<EventDefinitionHandle> event_definitions() const { return TableRows<EventDefinitionHandle>(row_count(TableIndex::Event)); }
    TableRows<PropertyDefinitionHandle> property_definitions() const { return TableRows<PropertyDefinitionHandle>(row_count(TableIndex::Property)); }
    TableRows<AssemblyReferenceHandle> assembly_references() const { return TableRows<AssemblyReferenceHandle>(row_count(TableIndex::AssemblyRef)); }
    TableRows<AssemblyFileHandle> assembly_files() const { return TableRows<AssemblyFileHandle>(row_count(TableIndex::File)); }
    TableRows<ExportedTypeHandle> exported_types() const { return TableRows<ExportedTypeHandle>(row_count(TableIndex::ExportedType)); }
    TableRows<ManifestResourceHandle> manifest_resources() const { return TableRows<ManifestResourceHandle>(row_count(TableIndex::ManifestResource)); }
    TableRows<ModuleReferenceHandle> module_references() const { return TableRows<ModuleReferenceHandle>(row_count(TableIndex::ModuleRef)); }

    ModuleDefinition get_module_definition() const { return ModuleDefinition(this); }
    AssemblyDefinition get_assembly_definition() const;   // BadImageFormatError (an InvalidOperation upstream) when not an assembly
    TypeReference get_type_reference(TypeReferenceHandle h) const { return TypeReference(this, h); }
    TypeDefinition get_type_definition(TypeDefinitionHandle h) const { return TypeDefinition(this, h); }
    FieldDefinition get_field_definition(FieldDefinitionHandle h) const { return FieldDefinition(this, h); }
    MethodDefinition get_method_definition(MethodDefinitionHandle h) const { return MethodDefinition(this, h); }
    Parameter get_parameter(ParameterHandle h) const { return Parameter(this, h); }
    InterfaceImplementation get_interface_implementation(InterfaceImplementationHandle h) const { return InterfaceImplementation(this, h); }
    MemberReference get_member_reference(MemberReferenceHandle h) const { return MemberReference(this, h); }
    Constant get_constant(ConstantHandle h) const { return Constant(this, h); }
    CustomAttribute get_custom_attribute(CustomAttributeHandle h) const { return CustomAttribute(this, h); }
    DeclarativeSecurityAttribute get_declarative_security_attribute(DeclarativeSecurityAttributeHandle h) const {
        return DeclarativeSecurityAttribute(this, h);
    }
    StandaloneSignature get_standalone_signature(StandaloneSignatureHandle h) const { return StandaloneSignature(this, h); }
    EventDefinition get_event_definition(EventDefinitionHandle h) const { return EventDefinition(this, h); }
    PropertyDefinition get_property_definition(PropertyDefinitionHandle h) const { return PropertyDefinition(this, h); }
    MethodImplementation get_method_implementation(MethodImplementationHandle h) const { return MethodImplementation(this, h); }
    ModuleReference get_module_reference(ModuleReferenceHandle h) const { return ModuleReference(this, h); }
    TypeSpecification get_type_specification(TypeSpecificationHandle h) const { return TypeSpecification(this, h); }
    AssemblyReference get_assembly_reference(AssemblyReferenceHandle h) const { return AssemblyReference(this, h); }
    AssemblyFile get_assembly_file(AssemblyFileHandle h) const { return AssemblyFile(this, h); }
    ExportedType get_exported_type(ExportedTypeHandle h) const { return ExportedType(this, h); }
    ManifestResource get_manifest_resource(ManifestResourceHandle h) const { return ManifestResource(this, h); }
    GenericParameter get_generic_parameter(GenericParameterHandle h) const { return GenericParameter(this, h); }
    GenericParameterConstraint get_generic_parameter_constraint(GenericParameterConstraintHandle h) const {
        return GenericParameterConstraint(this, h);
    }
    MethodSpecification get_method_specification(MethodSpecificationHandle h) const { return MethodSpecification(this, h); }

    NamespaceDefinitionHandle get_namespace_definition_root_handle() const { return NamespaceDefinitionHandle{1}; }
    NamespaceDefinition get_namespace_definition_root() const { return NamespaceDefinition(this, NamespaceDefinitionHandle{1}); }
    NamespaceDefinition get_namespace_definition(NamespaceDefinitionHandle h) const { return NamespaceDefinition(this, h); }

    // The custom attributes of any entity, in table order.
    std::vector<CustomAttributeHandle> get_custom_attributes(EntityHandle parent) const;

    // ---- the raw cells (what the views are made of)
    // A cell as its integer: a fixed value, a heap offset, a row, or a coded index's raw value.
    std::uint32_t cell(TableIndex t, std::uint32_t row, int column) const;
    // A coded index cell, decoded into the row it names (nil for an unused tag or row 0).
    EntityHandle coded_cell(TableIndex t, std::uint32_t row, int column) const;
    // The raw value a coded index of `kind` has for `h` (what the sorted tables are ordered by); nullopt when `h` cannot be one.
    std::optional<std::uint32_t> encode_coded(CodedIndex kind, EntityHandle h) const;
    // The rows of `t` whose `column` equals `value`, in row order (binary search where the table is sorted).
    std::vector<std::uint32_t> rows_where(TableIndex t, int column, std::uint32_t value) const;
    // The first such row, 0 when there is none.
    std::uint32_t first_row_where(TableIndex t, int column, std::uint32_t value) const;
    // A member list of a parent row (TypeDef's FieldList and MethodList, MethodDef's ParamList, EventMap's EventList,
    // PropertyMap's PropertyList): the run from its start to the next row's start, through the Ptr table if there is one.
    template <class H>
    HandleList<H> member_list(TableIndex parent, std::uint32_t row, int column, TableIndex member, TableIndex ptr) const;
    // The row of the Field, MethodDef, Param, Event or Property table that the `i`th (0-based) entry from `first` names.
    std::uint32_t member_at(TableIndex ptr, bool indirect, std::uint32_t first, std::uint32_t i) const;
    // The parent row whose member list holds `member` (the declaring type of a field or method): 0 when none does.
    std::uint32_t list_owner(TableIndex parent, int column, TableIndex member, TableIndex ptr, std::uint32_t member_row) const;
    // The nested types of an enclosing type (from NestedClass, in its order).
    const std::vector<TypeDefinitionHandle>& nested_types_of(TypeDefinitionHandle enclosing) const;

    struct NamespaceNode {
        std::string full_name, name;
        NamespaceDefinitionHandle parent;
        std::vector<NamespaceDefinitionHandle> namespaces;
        std::vector<TypeDefinitionHandle> types;
        std::vector<ExportedTypeHandle> exported_types;
    };
    const NamespaceNode& namespace_node(NamespaceDefinitionHandle h) const;

private:
    struct Table {
        std::uint32_t rows = 0;
        std::uint32_t offset = 0;   // from the start of the metadata
        int row_size = 0;
        std::array<std::uint8_t, 10> column_offset{};
        std::array<std::uint8_t, 10> column_size{};
    };
    struct Heap {
        std::uint32_t offset = 0, size = 0;
    };
    const Table& table(TableIndex t) const { return tables_[static_cast<std::size_t>(t)]; }
    void read_tables_stream(std::uint32_t offset, std::uint32_t size);
    int index_size(TableIndex t) const;
    int coded_size(CodedIndex kind) const;
    std::uint32_t read_at(std::uint64_t offset, int size) const;
    ByteView heap(HeapIndex h) const;
    bool heap_value(HeapIndex h, std::uint32_t offset, std::uint32_t& value_offset, std::uint32_t& value_size) const;

    ByteView md_;
    std::string version_;
    MetadataKind kind_ = MetadataKind::Ecma335;
    bool uncompressed_ = false, minimal_delta_ = false;
    std::uint8_t heap_sizes_ = 0;
    std::uint64_t valid_ = 0, sorted_ = 0;
    std::array<Table, kTableCount> tables_{};
    std::array<Heap, 4> heaps_{};

    bool column_in_order(TableIndex t, int column) const;
    mutable std::mutex order_mutex_;
    mutable std::array<std::array<std::int8_t, 10>, kTableCount> order_{};   // per column: 0 not yet known, 1 in order, 2 not
    NamespaceDefinitionHandle namespace_for(const std::string& full_name) const;
    mutable std::once_flag namespaces_once_;
    mutable std::vector<NamespaceNode> namespaces_;
    mutable std::once_flag nested_once_;
    mutable std::unordered_map<std::uint32_t, std::vector<TypeDefinitionHandle>> nested_;
    mutable std::once_flag ptr_once_[5];
    mutable std::vector<std::uint32_t> ptr_inverse_[5];   // Field, MethodDef, Param, Event, Property row -> its Ptr row
};

// ---- the templates' bodies -------------------------------------------------------------------------------------------------

template <class H>
H HandleList<H>::operator[](std::uint32_t i) const {
    return H(md_->member_at(ptr_, indirect_, first_, i));
}

template <class H>
HandleList<H> MetadataReader::member_list(TableIndex parent, std::uint32_t row, int column, TableIndex member, TableIndex ptr) const {
    const bool indirect = row_count(ptr) > 0;
    const std::uint32_t total = indirect ? row_count(ptr) : row_count(member);
    std::uint32_t first = cell(parent, row, column);
    std::uint32_t last = row < row_count(parent) ? cell(parent, row + 1, column) : total + 1;
    if (first == 0 || first > total + 1) first = total + 1;   // as upstream: an out-of-range start is an empty list
    if (last > total + 1) last = total + 1;
    const std::uint32_t count = last > first ? last - first : 0;
    return HandleList<H>(this, first, count, ptr, indirect);
}

}  // namespace cb::dotnet::metadata

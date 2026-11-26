// dotnet/metadata/handles.hpp -- handles: how one part of the metadata names another (a row of a table, or an entry of a heap),
// as System.Reflection.Metadata names them, which is what the ILSpy port calls.
//
//     EntityHandle h = entity_handle(0x02000005);   // TypeDef row 5 (the token of ldtoken, call ...)
//     h.kind() == HandleKind::TypeDefinition;
//     TypeDefinitionHandle t(h);                    // the typed handle (checks the kind)
//     get_token(t) == 0x02000005;
//
// A row handle is a token: the table in the top byte, the row (1-based; 0 is nil) below. A heap handle is an offset into
// its heap (a #GUID handle is an index, 1-based). `Handle` holds either.
#pragma once

#include <compare>
#include <cstdint>
#include <functional>

#include "core/error.hpp"

namespace cb::dotnet::metadata {

// What a handle names: a table (the value is the table's number) or a heap.
enum class HandleKind : std::uint8_t {
    ModuleDefinition = 0x00, TypeReference = 0x01, TypeDefinition = 0x02, FieldDefinition = 0x04, MethodDefinition = 0x06,
    Parameter = 0x08, InterfaceImplementation = 0x09, MemberReference = 0x0A, Constant = 0x0B, CustomAttribute = 0x0C,
    DeclarativeSecurityAttribute = 0x0E, StandaloneSignature = 0x11, EventDefinition = 0x14, PropertyDefinition = 0x17,
    MethodImplementation = 0x19, ModuleReference = 0x1A, TypeSpecification = 0x1B, AssemblyDefinition = 0x20,
    AssemblyReference = 0x23, AssemblyFile = 0x26, ExportedType = 0x27, ManifestResource = 0x28, GenericParameter = 0x2A,
    MethodSpecification = 0x2B, GenericParameterConstraint = 0x2C,
    Document = 0x30, MethodDebugInformation = 0x31, LocalScope = 0x32, LocalVariable = 0x33, LocalConstant = 0x34,
    ImportScope = 0x35, CustomDebugInformation = 0x37,
    UserString = 0x70, Blob = 0x71, Guid = 0x72, String = 0x78, NamespaceDefinition = 0x7C,
};

// A row of any table: the token.
class EntityHandle {
public:
    constexpr EntityHandle() = default;
    constexpr explicit EntityHandle(std::uint32_t token) : token_(token) {}
    constexpr EntityHandle(HandleKind kind, std::uint32_t row) : token_(static_cast<std::uint32_t>(kind) << 24 | (row & 0xFFFFFF)) {}
    constexpr HandleKind kind() const { return static_cast<HandleKind>(token_ >> 24); }
    constexpr std::uint32_t row() const { return token_ & 0xFFFFFF; }
    constexpr std::uint32_t token() const { return token_; }
    constexpr bool is_nil() const { return row() == 0; }
    friend constexpr auto operator<=>(const EntityHandle&, const EntityHandle&) = default;

private:
    std::uint32_t token_ = 0;
};

// A row of one table.
template <HandleKind K>
class RowHandle {
public:
    static constexpr HandleKind kKind = K;
    constexpr RowHandle() = default;
    constexpr explicit RowHandle(std::uint32_t row) : row_(row) {}
    // From an entity handle of this kind (UsageError for another kind; a nil handle of any kind is nil).
    explicit RowHandle(EntityHandle h) : row_(h.row()) {
        if (!h.is_nil() && h.kind() != K) throw UsageError("dotnet metadata: a handle of another kind");
    }
    constexpr std::uint32_t row() const { return row_; }
    constexpr bool is_nil() const { return row_ == 0; }
    constexpr operator EntityHandle() const { return EntityHandle(K, row_); }
    friend constexpr auto operator<=>(const RowHandle&, const RowHandle&) = default;

private:
    std::uint32_t row_ = 0;
};

using ModuleDefinitionHandle = RowHandle<HandleKind::ModuleDefinition>;
using TypeReferenceHandle = RowHandle<HandleKind::TypeReference>;
using TypeDefinitionHandle = RowHandle<HandleKind::TypeDefinition>;
using FieldDefinitionHandle = RowHandle<HandleKind::FieldDefinition>;
using MethodDefinitionHandle = RowHandle<HandleKind::MethodDefinition>;
using ParameterHandle = RowHandle<HandleKind::Parameter>;
using InterfaceImplementationHandle = RowHandle<HandleKind::InterfaceImplementation>;
using MemberReferenceHandle = RowHandle<HandleKind::MemberReference>;
using ConstantHandle = RowHandle<HandleKind::Constant>;
using CustomAttributeHandle = RowHandle<HandleKind::CustomAttribute>;
using DeclarativeSecurityAttributeHandle = RowHandle<HandleKind::DeclarativeSecurityAttribute>;
using StandaloneSignatureHandle = RowHandle<HandleKind::StandaloneSignature>;
using EventDefinitionHandle = RowHandle<HandleKind::EventDefinition>;
using PropertyDefinitionHandle = RowHandle<HandleKind::PropertyDefinition>;
using MethodImplementationHandle = RowHandle<HandleKind::MethodImplementation>;
using ModuleReferenceHandle = RowHandle<HandleKind::ModuleReference>;
using TypeSpecificationHandle = RowHandle<HandleKind::TypeSpecification>;
using AssemblyDefinitionHandle = RowHandle<HandleKind::AssemblyDefinition>;
using AssemblyReferenceHandle = RowHandle<HandleKind::AssemblyReference>;
using AssemblyFileHandle = RowHandle<HandleKind::AssemblyFile>;
using ExportedTypeHandle = RowHandle<HandleKind::ExportedType>;
using ManifestResourceHandle = RowHandle<HandleKind::ManifestResource>;
using GenericParameterHandle = RowHandle<HandleKind::GenericParameter>;
using MethodSpecificationHandle = RowHandle<HandleKind::MethodSpecification>;
using GenericParameterConstraintHandle = RowHandle<HandleKind::GenericParameterConstraint>;

// An entry of a heap: its offset (#Strings, #Blob, #US) or its index (#GUID, 1-based). 0 is nil.
template <HandleKind K>
class HeapHandle {
public:
    static constexpr HandleKind kKind = K;
    constexpr HeapHandle() = default;
    constexpr explicit HeapHandle(std::uint32_t offset) : offset_(offset) {}
    constexpr std::uint32_t offset() const { return offset_; }
    constexpr bool is_nil() const { return offset_ == 0; }
    friend constexpr auto operator<=>(const HeapHandle&, const HeapHandle&) = default;

private:
    std::uint32_t offset_ = 0;
};
using StringHandle = HeapHandle<HandleKind::String>;
using BlobHandle = HeapHandle<HandleKind::Blob>;
using GuidHandle = HeapHandle<HandleKind::Guid>;
using UserStringHandle = HeapHandle<HandleKind::UserString>;

// Any handle: a row or a heap entry.
class Handle {
public:
    constexpr Handle() = default;
    constexpr Handle(EntityHandle h) : kind_(h.kind()), value_(h.row()) {}
    template <HandleKind K>
    constexpr Handle(RowHandle<K> h) : kind_(K), value_(h.row()) {}
    template <HandleKind K>
    constexpr Handle(HeapHandle<K> h) : kind_(K), value_(h.offset()) {}
    constexpr HandleKind kind() const { return kind_; }
    constexpr std::uint32_t value() const { return value_; }   // the row, or the heap offset
    constexpr bool is_nil() const { return value_ == 0; }
    constexpr bool is_entity() const { return static_cast<std::uint8_t>(kind_) < 0x70; }
    constexpr EntityHandle entity() const { return EntityHandle(kind_, value_); }
    friend constexpr auto operator<=>(const Handle&, const Handle&) = default;

private:
    HandleKind kind_ = HandleKind::ModuleDefinition;
    std::uint32_t value_ = 0;
};

// System.Reflection.Metadata.Ecma335.MetadataTokens: tokens and handles back and forth.
constexpr std::uint32_t get_token(EntityHandle h) { return h.token(); }
constexpr std::uint32_t get_token(Handle h) {
    return h.kind() == HandleKind::UserString ? 0x70000000u | h.value() : static_cast<std::uint32_t>(h.kind()) << 24 | h.value();
}
template <HandleKind K>
constexpr std::uint32_t get_token(RowHandle<K> h) { return EntityHandle(h).token(); }
constexpr EntityHandle entity_handle(std::uint32_t token) { return EntityHandle(token); }
constexpr std::uint32_t get_row_number(EntityHandle h) { return h.row(); }
template <HandleKind K>
constexpr std::uint32_t get_heap_offset(HeapHandle<K> h) { return h.offset(); }

}  // namespace cb::dotnet::metadata

template <>
struct std::hash<cb::dotnet::metadata::EntityHandle> {
    std::size_t operator()(const cb::dotnet::metadata::EntityHandle& h) const noexcept { return std::hash<std::uint32_t>()(h.token()); }
};
template <cb::dotnet::metadata::HandleKind K>
struct std::hash<cb::dotnet::metadata::RowHandle<K>> {
    std::size_t operator()(const cb::dotnet::metadata::RowHandle<K>& h) const noexcept { return std::hash<std::uint32_t>()(h.row()); }
};
template <cb::dotnet::metadata::HandleKind K>
struct std::hash<cb::dotnet::metadata::HeapHandle<K>> {
    std::size_t operator()(const cb::dotnet::metadata::HeapHandle<K>& h) const noexcept { return std::hash<std::uint32_t>()(h.offset()); }
};
template <>
struct std::hash<cb::dotnet::metadata::Handle> {
    std::size_t operator()(const cb::dotnet::metadata::Handle& h) const noexcept {
        return std::hash<std::uint64_t>()(static_cast<std::uint64_t>(h.kind()) << 32 | h.value());
    }
};

// dotnet/metadata/tables.hpp -- the shape of the ECMA-335 metadata tables (Partition II, chapter 22): which tables there are,
// what columns each has, and the coded indices that point from one table into several.
//
//     const TableSchema& s = schema(TableIndex::TypeDef);    // its columns: Flags, TypeName, TypeNamespace, Extends ...
//     coded_index_tables(CodedIndex::TypeDefOrRef)           // {TypeDef, TypeRef, TypeSpec}: the tag is the position
//
// The reader (metadata_reader.hpp) lays the columns out for one assembly (their widths depend on the row counts and the
// heap sizes) and reads cells through this description. Names follow System.Reflection.Metadata's TableIndex, which is what
// the ILSpy port calls.
#pragma once

#include <cstdint>
#include <span>
#include <string_view>

namespace cb::dotnet::metadata {

enum class TableIndex : std::uint8_t {
    Module = 0x00, TypeRef = 0x01, TypeDef = 0x02, FieldPtr = 0x03, Field = 0x04, MethodPtr = 0x05, MethodDef = 0x06,
    ParamPtr = 0x07, Param = 0x08, InterfaceImpl = 0x09, MemberRef = 0x0A, Constant = 0x0B, CustomAttribute = 0x0C,
    FieldMarshal = 0x0D, DeclSecurity = 0x0E, ClassLayout = 0x0F, FieldLayout = 0x10, StandAloneSig = 0x11, EventMap = 0x12,
    EventPtr = 0x13, Event = 0x14, PropertyMap = 0x15, PropertyPtr = 0x16, Property = 0x17, MethodSemantics = 0x18,
    MethodImpl = 0x19, ModuleRef = 0x1A, TypeSpec = 0x1B, ImplMap = 0x1C, FieldRva = 0x1D, EncLog = 0x1E, EncMap = 0x1F,
    Assembly = 0x20, AssemblyProcessor = 0x21, AssemblyOS = 0x22, AssemblyRef = 0x23, AssemblyRefProcessor = 0x24,
    AssemblyRefOS = 0x25, File = 0x26, ExportedType = 0x27, ManifestResource = 0x28, NestedClass = 0x29, GenericParam = 0x2A,
    MethodSpec = 0x2B, GenericParamConstraint = 0x2C,
    // the Portable PDB tables (in a standalone .pdb, or a #~ that carries debug information)
    Document = 0x30, MethodDebugInformation = 0x31, LocalScope = 0x32, LocalVariable = 0x33, LocalConstant = 0x34,
    ImportScope = 0x35, StateMachineMethod = 0x36, CustomDebugInformation = 0x37,
};
inline constexpr int kTableCount = 0x38;   // table numbers 0 .. 0x37 (0x2D .. 0x2F are unused)

// The kinds of coded index (II.24.2.6): an index into one of several tables, its low bits saying which.
enum class CodedIndex : std::uint8_t {
    TypeDefOrRef, HasConstant, HasCustomAttribute, HasFieldMarshal, HasDeclSecurity, MemberRefParent, HasSemantics,
    MethodDefOrRef, MemberForwarded, Implementation, CustomAttributeType, ResolutionScope, TypeOrMethodDef,
    HasCustomDebugInformation,
};

// A column's kind: a fixed-size integer, an index into a heap, an index into one table, or a coded index.
enum class ColumnKind : std::uint8_t { U8x2, U16, U32, String, Guid, Blob, Table, Coded };

struct Column {
    std::string_view name;
    ColumnKind kind;
    std::uint8_t target = 0;   // the TableIndex of a Table column, the CodedIndex of a Coded one
};

struct TableSchema {
    std::string_view name;
    std::span<const Column> columns;
};

// The columns of a table (an empty schema for a number no table has).
const TableSchema& schema(TableIndex table);
// The tables a coded index can point into, by tag; kNoTable where a tag is unused (CustomAttributeType 0, 1, 4).
inline constexpr std::uint8_t kNoTable = 0xFF;
std::span<const std::uint8_t> coded_index_tables(CodedIndex kind);
// The number of tag bits of a coded index.
int coded_index_tag_bits(CodedIndex kind);

// Whether a table number is one a metadata stream may hold.
bool is_valid_table(int number);

}  // namespace cb::dotnet::metadata

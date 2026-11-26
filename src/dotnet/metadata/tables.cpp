// dotnet/metadata/tables.cpp -- the columns of every metadata table and the members of every coded index (ECMA-335 II.22,
// II.24.2.6, and the Portable PDB specification's tables).
#include "dotnet/metadata/tables.hpp"

#include <array>

namespace cb::dotnet::metadata {
namespace {

constexpr std::uint8_t T(TableIndex t) { return static_cast<std::uint8_t>(t); }
constexpr std::uint8_t C(CodedIndex c) { return static_cast<std::uint8_t>(c); }
using K = ColumnKind;
using TI = TableIndex;
using CI = CodedIndex;

constexpr Column kModule[] = {{"Generation", K::U16}, {"Name", K::String}, {"Mvid", K::Guid}, {"EncId", K::Guid}, {"EncBaseId", K::Guid}};
constexpr Column kTypeRef[] = {{"ResolutionScope", K::Coded, C(CI::ResolutionScope)}, {"TypeName", K::String}, {"TypeNamespace", K::String}};
constexpr Column kTypeDef[] = {{"Flags", K::U32}, {"TypeName", K::String}, {"TypeNamespace", K::String},
                               {"Extends", K::Coded, C(CI::TypeDefOrRef)}, {"FieldList", K::Table, T(TI::Field)},
                               {"MethodList", K::Table, T(TI::MethodDef)}};
constexpr Column kFieldPtr[] = {{"Field", K::Table, T(TI::Field)}};
constexpr Column kField[] = {{"Flags", K::U16}, {"Name", K::String}, {"Signature", K::Blob}};
constexpr Column kMethodPtr[] = {{"Method", K::Table, T(TI::MethodDef)}};
constexpr Column kMethodDef[] = {{"RVA", K::U32}, {"ImplFlags", K::U16}, {"Flags", K::U16}, {"Name", K::String}, {"Signature", K::Blob},
                                 {"ParamList", K::Table, T(TI::Param)}};
constexpr Column kParamPtr[] = {{"Param", K::Table, T(TI::Param)}};
constexpr Column kParam[] = {{"Flags", K::U16}, {"Sequence", K::U16}, {"Name", K::String}};
constexpr Column kInterfaceImpl[] = {{"Class", K::Table, T(TI::TypeDef)}, {"Interface", K::Coded, C(CI::TypeDefOrRef)}};
constexpr Column kMemberRef[] = {{"Class", K::Coded, C(CI::MemberRefParent)}, {"Name", K::String}, {"Signature", K::Blob}};
constexpr Column kConstant[] = {{"Type", K::U8x2}, {"Parent", K::Coded, C(CI::HasConstant)}, {"Value", K::Blob}};
constexpr Column kCustomAttribute[] = {{"Parent", K::Coded, C(CI::HasCustomAttribute)}, {"Type", K::Coded, C(CI::CustomAttributeType)},
                                       {"Value", K::Blob}};
constexpr Column kFieldMarshal[] = {{"Parent", K::Coded, C(CI::HasFieldMarshal)}, {"NativeType", K::Blob}};
constexpr Column kDeclSecurity[] = {{"Action", K::U16}, {"Parent", K::Coded, C(CI::HasDeclSecurity)}, {"PermissionSet", K::Blob}};
constexpr Column kClassLayout[] = {{"PackingSize", K::U16}, {"ClassSize", K::U32}, {"Parent", K::Table, T(TI::TypeDef)}};
constexpr Column kFieldLayout[] = {{"Offset", K::U32}, {"Field", K::Table, T(TI::Field)}};
constexpr Column kStandAloneSig[] = {{"Signature", K::Blob}};
constexpr Column kEventMap[] = {{"Parent", K::Table, T(TI::TypeDef)}, {"EventList", K::Table, T(TI::Event)}};
constexpr Column kEventPtr[] = {{"Event", K::Table, T(TI::Event)}};
constexpr Column kEvent[] = {{"EventFlags", K::U16}, {"Name", K::String}, {"EventType", K::Coded, C(CI::TypeDefOrRef)}};
constexpr Column kPropertyMap[] = {{"Parent", K::Table, T(TI::TypeDef)}, {"PropertyList", K::Table, T(TI::Property)}};
constexpr Column kPropertyPtr[] = {{"Property", K::Table, T(TI::Property)}};
constexpr Column kProperty[] = {{"Flags", K::U16}, {"Name", K::String}, {"Type", K::Blob}};
constexpr Column kMethodSemantics[] = {{"Semantics", K::U16}, {"Method", K::Table, T(TI::MethodDef)},
                                       {"Association", K::Coded, C(CI::HasSemantics)}};
constexpr Column kMethodImpl[] = {{"Class", K::Table, T(TI::TypeDef)}, {"MethodBody", K::Coded, C(CI::MethodDefOrRef)},
                                  {"MethodDeclaration", K::Coded, C(CI::MethodDefOrRef)}};
constexpr Column kModuleRef[] = {{"Name", K::String}};
constexpr Column kTypeSpec[] = {{"Signature", K::Blob}};
constexpr Column kImplMap[] = {{"MappingFlags", K::U16}, {"MemberForwarded", K::Coded, C(CI::MemberForwarded)}, {"ImportName", K::String},
                               {"ImportScope", K::Table, T(TI::ModuleRef)}};
constexpr Column kFieldRva[] = {{"RVA", K::U32}, {"Field", K::Table, T(TI::Field)}};
constexpr Column kEncLog[] = {{"Token", K::U32}, {"FuncCode", K::U32}};
constexpr Column kEncMap[] = {{"Token", K::U32}};
constexpr Column kAssembly[] = {{"HashAlgId", K::U32}, {"MajorVersion", K::U16}, {"MinorVersion", K::U16}, {"BuildNumber", K::U16},
                                {"RevisionNumber", K::U16}, {"Flags", K::U32}, {"PublicKey", K::Blob}, {"Name", K::String},
                                {"Culture", K::String}};
constexpr Column kAssemblyProcessor[] = {{"Processor", K::U32}};
constexpr Column kAssemblyOS[] = {{"OSPlatformID", K::U32}, {"OSMajorVersion", K::U32}, {"OSMinorVersion", K::U32}};
constexpr Column kAssemblyRef[] = {{"MajorVersion", K::U16}, {"MinorVersion", K::U16}, {"BuildNumber", K::U16}, {"RevisionNumber", K::U16},
                                   {"Flags", K::U32}, {"PublicKeyOrToken", K::Blob}, {"Name", K::String}, {"Culture", K::String},
                                   {"HashValue", K::Blob}};
constexpr Column kAssemblyRefProcessor[] = {{"Processor", K::U32}, {"AssemblyRef", K::Table, T(TI::AssemblyRef)}};
constexpr Column kAssemblyRefOS[] = {{"OSPlatformId", K::U32}, {"OSMajorVersion", K::U32}, {"OSMinorVersion", K::U32},
                                     {"AssemblyRef", K::Table, T(TI::AssemblyRef)}};
constexpr Column kFile[] = {{"Flags", K::U32}, {"Name", K::String}, {"HashValue", K::Blob}};
constexpr Column kExportedType[] = {{"Flags", K::U32}, {"TypeDefId", K::U32}, {"TypeName", K::String}, {"TypeNamespace", K::String},
                                    {"Implementation", K::Coded, C(CI::Implementation)}};
constexpr Column kManifestResource[] = {{"Offset", K::U32}, {"Flags", K::U32}, {"Name", K::String},
                                        {"Implementation", K::Coded, C(CI::Implementation)}};
constexpr Column kNestedClass[] = {{"NestedClass", K::Table, T(TI::TypeDef)}, {"EnclosingClass", K::Table, T(TI::TypeDef)}};
constexpr Column kGenericParam[] = {{"Number", K::U16}, {"Flags", K::U16}, {"Owner", K::Coded, C(CI::TypeOrMethodDef)}, {"Name", K::String}};
constexpr Column kMethodSpec[] = {{"Method", K::Coded, C(CI::MethodDefOrRef)}, {"Instantiation", K::Blob}};
constexpr Column kGenericParamConstraint[] = {{"Owner", K::Table, T(TI::GenericParam)}, {"Constraint", K::Coded, C(CI::TypeDefOrRef)}};
// Portable PDB
constexpr Column kDocument[] = {{"Name", K::Blob}, {"HashAlgorithm", K::Guid}, {"Hash", K::Blob}, {"Language", K::Guid}};
constexpr Column kMethodDebugInformation[] = {{"Document", K::Table, T(TI::Document)}, {"SequencePoints", K::Blob}};
constexpr Column kLocalScope[] = {{"Method", K::Table, T(TI::MethodDef)}, {"ImportScope", K::Table, T(TI::ImportScope)},
                                  {"VariableList", K::Table, T(TI::LocalVariable)}, {"ConstantList", K::Table, T(TI::LocalConstant)},
                                  {"StartOffset", K::U32}, {"Length", K::U32}};
constexpr Column kLocalVariable[] = {{"Attributes", K::U16}, {"Index", K::U16}, {"Name", K::String}};
constexpr Column kLocalConstant[] = {{"Name", K::String}, {"Signature", K::Blob}};
constexpr Column kImportScope[] = {{"Parent", K::Table, T(TI::ImportScope)}, {"Imports", K::Blob}};
constexpr Column kStateMachineMethod[] = {{"MoveNextMethod", K::Table, T(TI::MethodDef)}, {"KickoffMethod", K::Table, T(TI::MethodDef)}};
constexpr Column kCustomDebugInformation[] = {{"Parent", K::Coded, C(CI::HasCustomDebugInformation)}, {"Kind", K::Guid}, {"Value", K::Blob}};

constexpr std::array<TableSchema, kTableCount> kSchemas = {{
    {"Module", kModule}, {"TypeRef", kTypeRef}, {"TypeDef", kTypeDef}, {"FieldPtr", kFieldPtr}, {"Field", kField},
    {"MethodPtr", kMethodPtr}, {"MethodDef", kMethodDef}, {"ParamPtr", kParamPtr}, {"Param", kParam},
    {"InterfaceImpl", kInterfaceImpl}, {"MemberRef", kMemberRef}, {"Constant", kConstant}, {"CustomAttribute", kCustomAttribute},
    {"FieldMarshal", kFieldMarshal}, {"DeclSecurity", kDeclSecurity}, {"ClassLayout", kClassLayout}, {"FieldLayout", kFieldLayout},
    {"StandAloneSig", kStandAloneSig}, {"EventMap", kEventMap}, {"EventPtr", kEventPtr}, {"Event", kEvent},
    {"PropertyMap", kPropertyMap}, {"PropertyPtr", kPropertyPtr}, {"Property", kProperty}, {"MethodSemantics", kMethodSemantics},
    {"MethodImpl", kMethodImpl}, {"ModuleRef", kModuleRef}, {"TypeSpec", kTypeSpec}, {"ImplMap", kImplMap}, {"FieldRVA", kFieldRva},
    {"ENCLog", kEncLog}, {"ENCMap", kEncMap}, {"Assembly", kAssembly}, {"AssemblyProcessor", kAssemblyProcessor},
    {"AssemblyOS", kAssemblyOS}, {"AssemblyRef", kAssemblyRef}, {"AssemblyRefProcessor", kAssemblyRefProcessor},
    {"AssemblyRefOS", kAssemblyRefOS}, {"File", kFile}, {"ExportedType", kExportedType}, {"ManifestResource", kManifestResource},
    {"NestedClass", kNestedClass}, {"GenericParam", kGenericParam}, {"MethodSpec", kMethodSpec},
    {"GenericParamConstraint", kGenericParamConstraint},
    {"", {}}, {"", {}}, {"", {}},
    {"Document", kDocument}, {"MethodDebugInformation", kMethodDebugInformation}, {"LocalScope", kLocalScope},
    {"LocalVariable", kLocalVariable}, {"LocalConstant", kLocalConstant}, {"ImportScope", kImportScope},
    {"StateMachineMethod", kStateMachineMethod}, {"CustomDebugInformation", kCustomDebugInformation},
}};

constexpr std::uint8_t N = kNoTable;
constexpr std::uint8_t kTypeDefOrRef[] = {T(TI::TypeDef), T(TI::TypeRef), T(TI::TypeSpec)};
constexpr std::uint8_t kHasConstant[] = {T(TI::Field), T(TI::Param), T(TI::Property)};
constexpr std::uint8_t kHasCustomAttribute[] = {
    T(TI::MethodDef), T(TI::Field), T(TI::TypeRef), T(TI::TypeDef), T(TI::Param), T(TI::InterfaceImpl), T(TI::MemberRef),
    T(TI::Module), T(TI::DeclSecurity), T(TI::Property), T(TI::Event), T(TI::StandAloneSig), T(TI::ModuleRef), T(TI::TypeSpec),
    T(TI::Assembly), T(TI::AssemblyRef), T(TI::File), T(TI::ExportedType), T(TI::ManifestResource), T(TI::GenericParam),
    T(TI::GenericParamConstraint), T(TI::MethodSpec)};
constexpr std::uint8_t kHasFieldMarshal[] = {T(TI::Field), T(TI::Param)};
constexpr std::uint8_t kHasDeclSecurity[] = {T(TI::TypeDef), T(TI::MethodDef), T(TI::Assembly)};
constexpr std::uint8_t kMemberRefParent[] = {T(TI::TypeDef), T(TI::TypeRef), T(TI::ModuleRef), T(TI::MethodDef), T(TI::TypeSpec)};
constexpr std::uint8_t kHasSemantics[] = {T(TI::Event), T(TI::Property)};
constexpr std::uint8_t kMethodDefOrRef[] = {T(TI::MethodDef), T(TI::MemberRef)};
constexpr std::uint8_t kMemberForwarded[] = {T(TI::Field), T(TI::MethodDef)};
constexpr std::uint8_t kImplementation[] = {T(TI::File), T(TI::AssemblyRef), T(TI::ExportedType)};
constexpr std::uint8_t kCustomAttributeType[] = {N, N, T(TI::MethodDef), T(TI::MemberRef), N};
constexpr std::uint8_t kResolutionScope[] = {T(TI::Module), T(TI::ModuleRef), T(TI::AssemblyRef), T(TI::TypeRef)};
constexpr std::uint8_t kTypeOrMethodDef[] = {T(TI::TypeDef), T(TI::MethodDef)};
constexpr std::uint8_t kHasCustomDebugInformation[] = {
    T(TI::MethodDef), T(TI::Field), T(TI::TypeRef), T(TI::TypeDef), T(TI::Param), T(TI::InterfaceImpl), T(TI::MemberRef),
    T(TI::Module), T(TI::DeclSecurity), T(TI::Property), T(TI::Event), T(TI::StandAloneSig), T(TI::ModuleRef), T(TI::TypeSpec),
    T(TI::Assembly), T(TI::AssemblyRef), T(TI::File), T(TI::ExportedType), T(TI::ManifestResource), T(TI::GenericParam),
    T(TI::GenericParamConstraint), T(TI::MethodSpec), T(TI::Document), T(TI::LocalScope), T(TI::LocalVariable),
    T(TI::LocalConstant), T(TI::ImportScope)};

struct CodedShape {
    std::span<const std::uint8_t> tables;
    int bits;
};
constexpr CodedShape kCoded[] = {
    {kTypeDefOrRef, 2}, {kHasConstant, 2}, {kHasCustomAttribute, 5}, {kHasFieldMarshal, 1}, {kHasDeclSecurity, 2},
    {kMemberRefParent, 3}, {kHasSemantics, 1}, {kMethodDefOrRef, 1}, {kMemberForwarded, 1}, {kImplementation, 2},
    {kCustomAttributeType, 3}, {kResolutionScope, 2}, {kTypeOrMethodDef, 1}, {kHasCustomDebugInformation, 5},
};

}  // namespace

const TableSchema& schema(TableIndex table) {
    static constexpr TableSchema kNone{"", {}};
    const auto i = static_cast<std::size_t>(table);
    return i < kSchemas.size() ? kSchemas[i] : kNone;
}

std::span<const std::uint8_t> coded_index_tables(CodedIndex kind) { return kCoded[static_cast<std::size_t>(kind)].tables; }

int coded_index_tag_bits(CodedIndex kind) { return kCoded[static_cast<std::size_t>(kind)].bits; }

bool is_valid_table(int number) { return number >= 0 && number < kTableCount && !kSchemas[static_cast<std::size_t>(number)].name.empty(); }

}  // namespace cb::dotnet::metadata

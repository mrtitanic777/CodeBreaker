// dotnet/decompiler/ilspycmd/metadata_table_dumper.cpp -- see metadata_table_dumper.hpp.
#include "dotnet/decompiler/ilspycmd/metadata_table_dumper.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "core/error.hpp"
#include "dotnet/decompiler/ilspycmd/dotnet_enum.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/metadata/metadata_file.hpp"
#include "dotnet/decompiler/util/char_info.hpp"
#include "dotnet/decompiler/util/dotnet_path.hpp"

namespace cb::dotnet::decompiler::ilspycmd {
namespace {

namespace srm = cb::dotnet::metadata;
using srm::TableIndex;

constexpr const char* kNewLine = "\r\n";

// The Cor tables the GUI's metadata view supports; EnC and Portable-PDB tables are out of scope.
constexpr std::array kSupportedTables = {
    TableIndex::Module,        TableIndex::TypeRef,         TableIndex::TypeDef,          TableIndex::FieldPtr,         TableIndex::Field,
    TableIndex::MethodPtr,     TableIndex::MethodDef,       TableIndex::ParamPtr,         TableIndex::Param,            TableIndex::InterfaceImpl,
    TableIndex::MemberRef,     TableIndex::Constant,        TableIndex::CustomAttribute,  TableIndex::FieldMarshal,     TableIndex::DeclSecurity,
    TableIndex::ClassLayout,   TableIndex::FieldLayout,     TableIndex::StandAloneSig,    TableIndex::EventMap,         TableIndex::EventPtr,
    TableIndex::Event,         TableIndex::PropertyMap,     TableIndex::PropertyPtr,      TableIndex::Property,         TableIndex::MethodSemantics,
    TableIndex::MethodImpl,    TableIndex::ModuleRef,       TableIndex::TypeSpec,         TableIndex::ImplMap,          TableIndex::FieldRva,
    TableIndex::Assembly,      TableIndex::AssemblyRef,     TableIndex::File,             TableIndex::ExportedType,     TableIndex::ManifestResource,
    TableIndex::NestedClass,   TableIndex::GenericParam,    TableIndex::MethodSpec,       TableIndex::GenericParamConstraint,
};

bool is_supported(TableIndex table) { return std::find(kSupportedTables.begin(), kSupportedTables.end(), table) != kSupportedTables.end(); }

std::string format_hex(std::uint32_t value) {
    char buffer[16];
    std::snprintf(buffer, sizeof buffer, "0x%08X", value);
    return buffer;
}

std::string format_rid(std::int64_t value) { return std::to_string(value); }

// A row: its RID and the columns (name, text) after the RID and the token.
struct Row {
    int rid = 0;
    std::vector<std::pair<std::string, std::string>> columns;   // "RID", "Token", then the table's
};

using Column = std::pair<std::string, std::string>;

// Turns what the tables hold into the text MetadataTableDumper.FormatValue makes of it.
class Formatter {
public:
    explicit Formatter(const srm::MetadataReader& metadata) : md_(metadata) {}

    std::string string(srm::StringHandle h) const { return h.is_nil() ? "" : std::string(md_.get_string(h)); }
    std::string blob(srm::BlobHandle h) const { return h.is_nil() ? "nil" : format_hex(srm::get_heap_offset(h)); }
    std::string guid(srm::GuidHandle h) const { return h.is_nil() ? "nil" : md_.get_guid(h).to_string(); }
    std::string entity(srm::EntityHandle h) const { return h.is_nil() ? "nil" : format_hex(srm::get_token(h)); }
    template <srm::HandleKind K>
    std::string entity(srm::RowHandle<K> h) const {
        return entity(srm::EntityHandle(h));
    }
    static std::string number(std::int64_t value) { return std::to_string(value); }
    static std::string boolean(bool value) { return value ? "true" : "false"; }
    static std::string enumeration(const EnumInfo& info, std::uint64_t value) { return enum_to_string(info, value); }

private:
    const srm::MetadataReader& md_;
};

// Rows made one at a time (BuildRow): the RID, the token, and the columns.
class RowBuilder {
public:
    explicit RowBuilder(std::vector<Row>& rows) : rows_(rows) {}
    void add(int rid, std::uint32_t token, std::vector<Column> columns) {
        Row row;
        row.rid = rid;
        row.columns.reserve(columns.size() + 2);
        row.columns.emplace_back("RID", std::to_string(rid));
        row.columns.emplace_back("Token", format_hex(token));
        for (Column& c : columns) row.columns.push_back(std::move(c));
        rows_.push_back(std::move(row));
    }
    void add(int rid, TableIndex table, std::vector<Column> columns) {
        // tables without an SRM handle type get their token synthesized from table id and RID
        add(rid, (static_cast<std::uint32_t>(table) << 24) | static_cast<std::uint32_t>(rid), std::move(columns));
    }
    template <srm::HandleKind K>
    void add(int rid, srm::RowHandle<K> handle, std::vector<Column> columns) {
        add(rid, srm::get_token(handle), std::move(columns));
    }

private:
    std::vector<Row>& rows_;
};

std::vector<Row> load_rows(const srm::MetadataReader& md, TableIndex table) {
    using namespace decompiler::metadata;
    std::vector<Row> rows;
    RowBuilder build(rows);
    const Formatter f(md);
    int rid = 0;
    const auto range = [&](TableIndex t) {
        std::vector<int> rids;
        for (int r = 1; r <= md.get_table_row_count(t); ++r) rids.push_back(r);
        return rids;
    };

    switch (table) {
        case TableIndex::Module:
            if (md.get_table_row_count(TableIndex::Module) > 0) {
                const srm::ModuleDefinition module = md.get_module_definition();
                build.add(1, TableIndex::Module,
                          {{"Generation", Formatter::number(module.generation())},
                           {"Name", f.string(module.name())},
                           {"Mvid", f.guid(module.mvid())},
                           {"GenerationId", f.guid(module.generation_id())},
                           {"BaseGenerationId", f.guid(module.base_generation_id())}});
            }
            break;
        case TableIndex::TypeRef:
            for (const srm::TypeReferenceHandle h : md.type_references()) {
                const srm::TypeReference type_ref = md.get_type_reference(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"ResolutionScope", f.entity(type_ref.resolution_scope())},
                           {"Name", f.string(type_ref.name())},
                           {"Namespace", f.string(type_ref.namespace_())}});
            }
            break;
        case TableIndex::TypeDef: {
            std::unordered_map<std::uint32_t, std::pair<int, int>> list_columns;
            for (const auto& [type, field_list, method_list] : get_type_def_list_columns(md)) list_columns[type.row()] = {field_list, method_list};
            for (const srm::TypeDefinitionHandle h : md.type_definitions()) {
                const srm::TypeDefinition type_def = md.get_type_definition(h);
                const auto [field_list, method_list] = list_columns[h.row()];
                build.add(static_cast<int>(h.row()), h,
                          {{"Attributes", Formatter::enumeration(enums::type_attributes(), type_def.attributes())},
                           {"Name", f.string(type_def.name())},
                           {"Namespace", f.string(type_def.namespace_())},
                           {"BaseType", f.entity(type_def.base_type())},
                           {"FieldList", format_rid(field_list)},
                           {"MethodList", format_rid(method_list)}});
            }
            break;
        }
        case TableIndex::Field:
            for (const srm::FieldDefinitionHandle h : md.field_definitions()) {
                const srm::FieldDefinition field = md.get_field_definition(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Attributes", Formatter::enumeration(enums::field_attributes(), field.attributes())},
                           {"Name", f.string(field.name())},
                           {"Signature", f.blob(field.signature())}});
            }
            break;
        case TableIndex::MethodDef: {
            std::unordered_map<std::uint32_t, int> param_lists;
            for (const auto& [method, param_list] : get_method_def_param_lists(md)) param_lists[method.row()] = param_list;
            for (const srm::MethodDefinitionHandle h : md.method_definitions()) {
                const srm::MethodDefinition method = md.get_method_definition(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"RVA", format_hex(static_cast<std::uint32_t>(method.relative_virtual_address()))},
                           {"ImplAttributes", Formatter::enumeration(enums::method_impl_attributes(), method.impl_attributes())},
                           {"Attributes", Formatter::enumeration(enums::method_attributes(), method.attributes())},
                           {"Name", f.string(method.name())},
                           {"Signature", f.blob(method.signature())},
                           {"ParamList", format_rid(param_lists[h.row()])}});
            }
            break;
        }
        case TableIndex::Param:
            for (const int r : range(table)) {
                const srm::ParameterHandle h(static_cast<std::uint32_t>(r));
                const srm::Parameter param = md.get_parameter(h);
                build.add(r, TableIndex::Param,
                          {{"Attributes", Formatter::enumeration(enums::parameter_attributes(), param.attributes())},
                           {"SequenceNumber", Formatter::number(param.sequence_number())},
                           {"Name", f.string(param.name())}});
            }
            break;
        case TableIndex::InterfaceImpl:
            for (const auto& [cls, iface] : get_interface_impl_rows(md)) build.add(++rid, table, {{"Class", f.entity(cls)}, {"Interface", f.entity(iface)}});
            break;
        case TableIndex::MemberRef:
            for (const srm::MemberReferenceHandle h : md.member_references()) {
                const srm::MemberReference member_ref = md.get_member_reference(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Parent", f.entity(member_ref.parent())}, {"Name", f.string(member_ref.name())}, {"Signature", f.blob(member_ref.signature())}});
            }
            break;
        case TableIndex::Constant:
            for (const int r : range(table)) {
                const srm::Constant constant = md.get_constant(srm::ConstantHandle(static_cast<std::uint32_t>(r)));
                build.add(r, TableIndex::Constant,
                          {{"TypeCode", Formatter::enumeration(enums::constant_type_code(), static_cast<std::uint64_t>(constant.type_code()))},
                           {"Parent", f.entity(constant.parent())},
                           {"Value", f.blob(constant.value())}});
            }
            break;
        case TableIndex::CustomAttribute:
            for (const srm::CustomAttributeHandle h : md.custom_attributes()) {
                const srm::CustomAttribute attribute = md.get_custom_attribute(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Parent", f.entity(attribute.parent())}, {"Constructor", f.entity(attribute.constructor())}, {"Value", f.blob(attribute.value())}});
            }
            break;
        case TableIndex::FieldMarshal:
            for (const auto& [parent, native_type] : get_field_marshals(md)) build.add(++rid, table, {{"Parent", f.entity(parent)}, {"NativeType", f.blob(native_type)}});
            break;
        case TableIndex::DeclSecurity:
            for (const srm::DeclarativeSecurityAttributeHandle h : md.declarative_security_attributes()) {
                const srm::DeclarativeSecurityAttribute decl_security = md.get_declarative_security_attribute(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Action", Formatter::enumeration(enums::declarative_security_action(), decl_security.action())},
                           {"Parent", f.entity(decl_security.parent())},
                           {"PermissionSet", f.blob(decl_security.permission_set())}});
            }
            break;
        case TableIndex::ClassLayout:
            for (const auto& [packing_size, class_size, parent] : get_class_layouts(md))
                build.add(++rid, table, {{"PackingSize", Formatter::number(packing_size)}, {"ClassSize", Formatter::number(class_size)}, {"Parent", f.entity(parent)}});
            break;
        case TableIndex::FieldLayout:
            for (const auto& [offset, field] : get_field_layout_rows(md)) build.add(++rid, table, {{"Offset", Formatter::number(offset)}, {"Field", f.entity(field)}});
            break;
        case TableIndex::StandAloneSig:
            for (const int r : range(table)) {
                const srm::StandaloneSignature signature = md.get_standalone_signature(srm::StandaloneSignatureHandle(static_cast<std::uint32_t>(r)));
                build.add(r, TableIndex::StandAloneSig, {{"Signature", f.blob(signature.signature())}});
            }
            break;
        case TableIndex::EventMap:
            for (const auto& [parent, event_list] : get_event_maps(md)) build.add(++rid, table, {{"Parent", f.entity(parent)}, {"EventList", f.entity(event_list)}});
            break;
        case TableIndex::Event:
            for (const srm::EventDefinitionHandle h : md.event_definitions()) {
                const srm::EventDefinition event_def = md.get_event_definition(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Attributes", Formatter::enumeration(enums::event_attributes(), event_def.attributes())},
                           {"Name", f.string(event_def.name())},
                           {"Type", f.entity(event_def.type())}});
            }
            break;
        case TableIndex::PropertyMap:
            for (const auto& [parent, property_list] : get_property_maps(md)) build.add(++rid, table, {{"Parent", f.entity(parent)}, {"PropertyList", f.entity(property_list)}});
            break;
        case TableIndex::Property:
            for (const srm::PropertyDefinitionHandle h : md.property_definitions()) {
                const srm::PropertyDefinition property = md.get_property_definition(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Attributes", Formatter::enumeration(enums::property_attributes(), property.attributes())},
                           {"Name", f.string(property.name())},
                           {"Signature", f.blob(property.signature())}});
            }
            break;
        case TableIndex::MethodSemantics:
            for (const MethodSemanticsRow& row : get_method_semantics(md))
                build.add(++rid, table,
                          {{"Semantics", Formatter::enumeration(enums::method_semantics_attributes(), row.semantics)},
                           {"Method", f.entity(row.method)},
                           {"Association", f.entity(row.association)}});
            break;
        case TableIndex::MethodImpl:
            for (const int r : range(table)) {
                const srm::MethodImplementation method_impl = md.get_method_implementation(srm::MethodImplementationHandle(static_cast<std::uint32_t>(r)));
                build.add(r, TableIndex::MethodImpl,
                          {{"Class", f.entity(method_impl.type())},
                           {"MethodBody", f.entity(method_impl.method_body())},
                           {"MethodDeclaration", f.entity(method_impl.method_declaration())}});
            }
            break;
        case TableIndex::ModuleRef:
            for (const srm::ModuleReferenceHandle h : get_module_references(md)) {
                const srm::ModuleReference module_ref = md.get_module_reference(h);
                build.add(static_cast<int>(h.row()), h, {{"Name", f.string(module_ref.name())}});
            }
            break;
        case TableIndex::TypeSpec:
            for (const srm::TypeSpecificationHandle h : get_type_specifications(md)) {
                const srm::TypeSpecification type_spec = md.get_type_specification(h);
                build.add(static_cast<int>(h.row()), h, {{"Signature", f.blob(type_spec.signature())}});
            }
            break;
        case TableIndex::ImplMap:
            for (const ImplMapRow& row : get_impl_maps(md))
                build.add(++rid, table,
                          {{"MappingFlags", Formatter::enumeration(enums::method_import_attributes(), row.mapping_flags)},
                           {"MemberForwarded", f.entity(row.member_forwarded)},
                           {"ImportName", f.string(row.import_name)},
                           {"ImportScope", f.entity(row.import_scope)}});
            break;
        case TableIndex::FieldRva:
            for (const auto& [rva, field] : get_field_rvas(md)) build.add(++rid, table, {{"RVA", format_hex(static_cast<std::uint32_t>(rva))}, {"Field", f.entity(field)}});
            break;
        case TableIndex::Assembly:
            if (md.is_assembly()) {
                const srm::AssemblyDefinition assembly = md.get_assembly_definition();
                build.add(1, TableIndex::Assembly,
                          {{"HashAlgorithm", Formatter::enumeration(enums::assembly_hash_algorithm(), assembly.hash_algorithm())},
                           {"Version", assembly.version().to_string()},
                           {"Flags", Formatter::enumeration(enums::assembly_flags(), assembly.flags())},
                           {"PublicKey", f.blob(assembly.public_key())},
                           {"Name", f.string(assembly.name())},
                           {"Culture", f.string(assembly.culture())}});
            }
            break;
        case TableIndex::AssemblyRef:
            for (const srm::AssemblyReferenceHandle h : md.assembly_references()) {
                const srm::AssemblyReference assembly_ref = md.get_assembly_reference(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Version", assembly_ref.version().to_string()},
                           {"Flags", Formatter::enumeration(enums::assembly_flags(), assembly_ref.flags())},
                           {"PublicKeyOrToken", f.blob(assembly_ref.public_key_or_token())},
                           {"Name", f.string(assembly_ref.name())},
                           {"Culture", f.string(assembly_ref.culture())},
                           {"HashValue", f.blob(assembly_ref.hash_value())}});
            }
            break;
        case TableIndex::File:
            for (const srm::AssemblyFileHandle h : md.assembly_files()) {
                const srm::AssemblyFile file = md.get_assembly_file(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"ContainsMetadata", Formatter::boolean(file.contains_metadata())}, {"Name", f.string(file.name())}, {"HashValue", f.blob(file.hash_value())}});
            }
            break;
        case TableIndex::ExportedType:
            for (const srm::ExportedTypeHandle h : md.exported_types()) {
                const srm::ExportedType exported_type = md.get_exported_type(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Attributes", Formatter::enumeration(enums::type_attributes(), exported_type.attributes())},
                           {"Name", f.string(exported_type.name())},
                           {"Namespace", f.string(exported_type.namespace_())},
                           {"Implementation", f.entity(exported_type.implementation())},
                           {"IsForwarder", Formatter::boolean(exported_type.is_forwarder())}});
            }
            break;
        case TableIndex::ManifestResource:
            for (const srm::ManifestResourceHandle h : md.manifest_resources()) {
                const srm::ManifestResource resource = md.get_manifest_resource(h);
                build.add(static_cast<int>(h.row()), h,
                          {{"Offset", Formatter::number(resource.offset())},
                           {"Attributes", Formatter::enumeration(enums::manifest_resource_attributes(), resource.attributes())},
                           {"Name", f.string(resource.name())},
                           {"Implementation", f.entity(resource.implementation())}});
            }
            break;
        case TableIndex::NestedClass:
            for (const auto& [nested, enclosing] : get_nested_classes(md)) build.add(++rid, table, {{"NestedClass", f.entity(nested)}, {"EnclosingClass", f.entity(enclosing)}});
            break;
        case TableIndex::GenericParam:
            for (const int r : range(table)) {
                const srm::GenericParameterHandle h(static_cast<std::uint32_t>(r));
                const srm::GenericParameter generic_param = md.get_generic_parameter(h);
                build.add(r, TableIndex::GenericParam,
                          {{"Number", Formatter::number(generic_param.index())},
                           {"Attributes", Formatter::enumeration(enums::generic_parameter_attributes(), generic_param.attributes())},
                           {"Owner", f.entity(generic_param.parent())},
                           {"Name", f.string(generic_param.name())}});
            }
            break;
        case TableIndex::MethodSpec:
            for (const srm::MethodSpecificationHandle h : get_method_specifications(md)) {
                const srm::MethodSpecification method_spec = md.get_method_specification(h);
                build.add(static_cast<int>(h.row()), h, {{"Method", f.entity(method_spec.method())}, {"Instantiation", f.blob(method_spec.signature())}});
            }
            break;
        case TableIndex::GenericParamConstraint:
            for (const int r : range(table)) {
                const srm::GenericParameterConstraint constraint = md.get_generic_parameter_constraint(srm::GenericParameterConstraintHandle(static_cast<std::uint32_t>(r)));
                build.add(r, TableIndex::GenericParamConstraint, {{"Owner", f.entity(constraint.parameter())}, {"Constraint", f.entity(constraint.type())}});
            }
            break;
        case TableIndex::EventPtr:
        case TableIndex::FieldPtr:
        case TableIndex::MethodPtr:
        case TableIndex::ParamPtr:
        case TableIndex::PropertyPtr:
            for (const srm::EntityHandle target : get_ptr_rows(md, table)) build.add(++rid, table, {{"Target", f.entity(target)}});
            break;
        default:
            throw UsageError("Table " + MetadataTableDumper::table_to_string(table) + " is listed as supported but has no loader.");
    }
    return rows;
}

// A string's length in UTF-16 units (string.Length).
std::size_t length(const std::string& text) { return util::utf16(text).size(); }

std::string pad_right(const std::string& text, std::size_t width) {
    const std::size_t n = length(text);
    return n >= width ? text : text + std::string(width - n, ' ');
}

// string.TrimEnd(): white space off the end.
std::string trim_end(std::string text) {
    while (!text.empty() && (text.back() == ' ' || text.back() == '\t' || text.back() == '\n' || text.back() == '\r' || text.back() == '\v' || text.back() == '\f')) text.pop_back();
    return text;
}

void write_console_table(std::ostream& output, const std::vector<Row>& rows) {
    if (rows.empty()) {
        output << "0 rows" << kNewLine;
        return;
    }
    const std::vector<Column>& first = rows[0].columns;
    std::vector<std::size_t> widths;
    for (const Column& c : first) widths.push_back(length(c.first));
    for (const Row& row : rows)
        for (std::size_t i = 0; i < row.columns.size(); ++i) widths[i] = std::max(widths[i], length(row.columns[i].second));

    std::string header, rule;
    for (std::size_t i = 0; i < first.size(); ++i) {
        if (i != 0) {
            header += "  ";
            rule += "  ";
        }
        header += pad_right(first[i].first, widths[i]);
        rule += std::string(widths[i], '-');
    }
    output << trim_end(header) << kNewLine << rule << kNewLine;
    for (const Row& row : rows) {
        std::string line;
        for (std::size_t i = 0; i < row.columns.size(); ++i) {
            if (i != 0) line += "  ";
            line += pad_right(row.columns[i].second, widths[i]);
        }
        output << trim_end(line) << kNewLine;
    }
}

// A string as System.Text.Json writes it with JavaScriptEncoder.Default: everything outside Basic Latin, the control characters and
// < > & ' + and the back quote as \uXXXX (upper case), " and \ and the short control escapes with a backslash.
std::string json_string(const std::string& text) {
    static const char* hex = "0123456789ABCDEF";
    std::string out = "\"";
    for (const char16_t c : util::utf16(text)) {
        switch (c) {
            case u'"': out += "\\\""; continue;
            case u'\\': out += "\\\\"; continue;
            case u'\b': out += "\\b"; continue;
            case u'\f': out += "\\f"; continue;
            case u'\n': out += "\\n"; continue;
            case u'\r': out += "\\r"; continue;
            case u'\t': out += "\\t"; continue;
            default: break;
        }
        const bool plain = c >= 0x20 && c < 0x7F && c != u'<' && c != u'>' && c != u'&' && c != u'\'' && c != u'+' && c != u'`';
        if (plain) {
            out += static_cast<char>(c);
        } else {
            out += "\\u";
            for (int shift = 12; shift >= 0; shift -= 4) out += hex[(c >> shift) & 0xF];
        }
    }
    out += '"';
    return out;
}

void write_json(std::ostream& output, const std::string& assembly_file_name, TableIndex table, const std::vector<Row>& rows) {
    std::string json = "{";
    json += std::string(kNewLine) + "  \"assembly\": " + json_string(assembly_file_name) + ",";
    json += std::string(kNewLine) + "  \"table\": " + json_string(MetadataTableDumper::table_to_string(table)) + ",";
    json += std::string(kNewLine) + "  \"rowCount\": " + std::to_string(rows.size()) + ",";
    if (rows.empty()) {
        json += std::string(kNewLine) + "  \"rows\": []";
    } else {
        json += std::string(kNewLine) + "  \"rows\": [";
        for (std::size_t r = 0; r < rows.size(); ++r) {
            json += std::string(kNewLine) + "    {";
            const std::vector<Column>& columns = rows[r].columns;
            for (std::size_t c = 0; c < columns.size(); ++c) {
                json += std::string(kNewLine) + "      " + json_string(columns[c].first) + ": ";
                json += c == 0 ? columns[c].second : json_string(columns[c].second);   // the RID is a number
                if (c + 1 < columns.size()) json += ",";
            }
            json += std::string(kNewLine) + "    }";
            if (r + 1 < rows.size()) json += ",";
        }
        json += std::string(kNewLine) + "  ]";
    }
    json += std::string(kNewLine) + "}";
    output << json << kNewLine;
}

// int.TryParse(text, NumberStyles.HexNumber, Invariant): white space around hex digits; the low 32 bits as a (signed) int.
bool try_parse_hex_int(std::string_view text, std::int32_t* value) {
    const auto is_space = [](char c) { return c == ' ' || (c >= '\t' && c <= '\r'); };
    while (!text.empty() && is_space(text.front())) text.remove_prefix(1);
    while (!text.empty() && is_space(text.back())) text.remove_suffix(1);
    if (text.empty()) return false;
    std::uint64_t v = 0;
    for (const char c : text) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        else return false;
        v = v * 16 + static_cast<std::uint64_t>(digit);
        if (v > 0xFFFFFFFFull) return false;
    }
    *value = static_cast<std::int32_t>(static_cast<std::uint32_t>(v));
    return true;
}

// int.TryParse(text, NumberStyles.None, Invariant): digits only, no larger than int.MaxValue.
bool try_parse_decimal_int(std::string_view text, std::int32_t* value) {
    if (text.empty()) return false;
    std::int64_t v = 0;
    for (const char c : text) {
        if (c < '0' || c > '9') return false;
        v = v * 10 + (c - '0');
        if (v > 0x7FFFFFFF) return false;
    }
    *value = static_cast<std::int32_t>(v);
    return true;
}

}  // namespace

std::string MetadataTableDumper::supported_table_names() {
    std::string text;
    for (const TableIndex t : kSupportedTables) {
        if (!text.empty()) text += ", ";
        char buffer[8];
        std::snprintf(buffer, sizeof buffer, "%02X", static_cast<unsigned>(t));
        text += table_to_string(t) + " (0x" + buffer + ")";
    }
    return text;
}

std::string MetadataTableDumper::table_to_string(TableIndex table) { return enum_to_string(enums::table_index(), static_cast<std::uint64_t>(table)); }

bool MetadataTableDumper::try_parse_table_name(std::string_view name, TableIndex* table) {
    // accept the ECMA-335 table number, decimal or 0x-prefixed hex, as an alternative to the table name
    std::int32_t number = 0;
    const bool hex = name.size() >= 2 && name[0] == '0' && (name[1] == 'x' || name[1] == 'X');
    const bool is_number = hex ? try_parse_hex_int(name.substr(2), &number) : try_parse_decimal_int(name, &number);
    if (is_number) {
        *table = static_cast<TableIndex>(static_cast<std::uint8_t>(number));
        return is_supported(*table);
    }
    std::uint64_t value = 0;
    if (!enum_try_parse(enums::table_index(), name, true, 8, false, &value)) return false;
    *table = static_cast<TableIndex>(static_cast<std::uint8_t>(value));
    return is_supported(*table);
}

int MetadataTableDumper::dump_table(const std::string& assembly_file_name, std::ostream& output, TableIndex table, bool as_json) {
    const std::shared_ptr<decompiler::metadata::PEFile> module = decompiler::metadata::PEFile::open(util::path::from_text(assembly_file_name));
    const std::vector<Row> rows = load_rows(module->metadata(), table);
    if (as_json)
        write_json(output, assembly_file_name, table, rows);
    else
        write_console_table(output, rows);
    return 0;
}

}  // namespace cb::dotnet::decompiler::ilspycmd

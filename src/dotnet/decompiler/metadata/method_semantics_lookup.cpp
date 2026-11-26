// dotnet/decompiler/metadata/method_semantics_lookup.cpp -- the accessor and backing-field indexes.
#include "dotnet/decompiler/metadata/method_semantics_lookup.hpp"

#include <algorithm>
#include <string>
#include <unordered_set>

#include "core/error.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"

namespace cb::dotnet::decompiler::metadata {

MethodSemanticsLookup::MethodSemanticsLookup(const srm::MetadataReader& metadata, std::uint16_t filter) {
    if ((filter & static_cast<std::uint16_t>(MethodSemanticsAttributes::Other)) != 0)
        throw UsageError("SRM doesn't provide access to 'other' accessors");
    entries_.reserve(static_cast<std::size_t>(metadata.get_table_row_count(srm::TableIndex::MethodSemantics)));
    const auto add_entry = [&](MethodSemanticsAttributes semantics, srm::MethodDefinitionHandle method, srm::EntityHandle association) {
        if ((static_cast<std::uint16_t>(semantics) & filter) == 0 || method.is_nil()) return;
        entries_.push_back({semantics, method.row(), association});
    };
    for (const srm::PropertyDefinitionHandle prop_handle : metadata.property_definitions()) {
        const srm::PropertyAccessors accessors = metadata.get_property_definition(prop_handle).get_accessors();
        add_entry(MethodSemanticsAttributes::Getter, accessors.getter, prop_handle);
        add_entry(MethodSemanticsAttributes::Setter, accessors.setter, prop_handle);
    }
    for (const srm::EventDefinitionHandle event_handle : metadata.event_definitions()) {
        const srm::EventAccessors accessors = metadata.get_event_definition(event_handle).get_accessors();
        add_entry(MethodSemanticsAttributes::Adder, accessors.adder, event_handle);
        add_entry(MethodSemanticsAttributes::Remover, accessors.remover, event_handle);
        add_entry(MethodSemanticsAttributes::Raiser, accessors.raiser, event_handle);
    }
    std::stable_sort(entries_.begin(), entries_.end(), [](const Entry& a, const Entry& b) { return a.method_row < b.method_row; });
}

std::pair<srm::EntityHandle, MethodSemanticsAttributes> MethodSemanticsLookup::get_semantics(srm::MethodDefinitionHandle method) const {
    const auto it = std::lower_bound(entries_.begin(), entries_.end(), method.row(), [](const Entry& e, std::uint32_t row) { return e.method_row < row; });
    if (it != entries_.end() && it->method_row == method.row()) return {it->association, it->semantics};
    return {srm::EntityHandle(), MethodSemanticsAttributes::None};
}

namespace {

bool is_compiler_generated(srm::FieldDefinitionHandle field, const srm::MetadataReader& metadata) {
    static const typesystem::TopLevelTypeName kCompilerGenerated("System.Runtime.CompilerServices", "CompilerGeneratedAttribute");
    for (const srm::CustomAttributeHandle h : metadata.get_field_definition(field).get_custom_attributes())
        if (is_known_type(get_attribute_type(metadata.get_custom_attribute(h), metadata), metadata, kCompilerGenerated)) return true;
    return false;
}

}  // namespace

PropertyAndEventBackingFieldLookup::PropertyAndEventBackingFieldLookup(const srm::MetadataReader& metadata) {
    std::unordered_map<std::string, std::vector<srm::FieldDefinitionHandle>> name_to_field_map;
    std::unordered_set<std::string> event_names;
    for (const srm::TypeDefinitionHandle tdh : metadata.type_definitions()) {
        const srm::TypeDefinition type = metadata.get_type_definition(tdh);
        for (const srm::FieldDefinitionHandle fdh : type.get_fields())
            name_to_field_map[std::string(metadata.get_string(metadata.get_field_definition(fdh).name()))].push_back(fdh);
        for (const srm::PropertyDefinitionHandle pdh : type.get_properties()) {
            const std::string name(metadata.get_string(metadata.get_property_definition(pdh).name()));
            // the C# compiler's backing field is "<Name>k__BackingField"
            if (const auto it = name_to_field_map.find("<" + name + ">k__BackingField"); it != name_to_field_map.end()) {
                for (const srm::FieldDefinitionHandle field_handle : it->second) property_lookup_[field_handle.row()] = pdh;
            } else if (const auto it2 = name_to_field_map.find("_" + name); it2 != name_to_field_map.end()) {
                for (const srm::FieldDefinitionHandle field_handle : it2->second)
                    if (is_compiler_generated(field_handle, metadata)) property_lookup_[field_handle.row()] = pdh;
            }
        }
        // the names of all the type's events first, so that a custom event "X" does not take the field of a field-like event "XEvent"
        for (const srm::EventDefinitionHandle edh : type.get_events()) event_names.insert(std::string(metadata.get_string(metadata.get_event_definition(edh).name())));
        for (const srm::EventDefinitionHandle edh : type.get_events()) {
            const std::string name(metadata.get_string(metadata.get_event_definition(edh).name()));
            if (const auto it = name_to_field_map.find(name); it != name_to_field_map.end()) {
                for (const srm::FieldDefinitionHandle field_handle : it->second) event_lookup_[field_handle.row()] = edh;
            } else {
                const std::string name_with_suffix = name + "Event";
                if (!event_names.contains(name_with_suffix)) {
                    if (const auto it2 = name_to_field_map.find(name_with_suffix); it2 != name_to_field_map.end())
                        for (const srm::FieldDefinitionHandle field_handle : it2->second) event_lookup_[field_handle.row()] = edh;
                }
            }
        }
        event_names.clear();
        name_to_field_map.clear();
    }
}

bool PropertyAndEventBackingFieldLookup::is_property_backing_field(srm::FieldDefinitionHandle field, srm::PropertyDefinitionHandle& handle) const {
    const auto it = property_lookup_.find(field.row());
    if (it == property_lookup_.end()) {
        handle = {};
        return false;
    }
    handle = it->second;
    return true;
}

bool PropertyAndEventBackingFieldLookup::is_event_backing_field(srm::FieldDefinitionHandle field, srm::EventDefinitionHandle& handle) const {
    const auto it = event_lookup_.find(field.row());
    if (it == event_lookup_.end()) {
        handle = {};
        return false;
    }
    handle = it->second;
    return true;
}

}  // namespace cb::dotnet::decompiler::metadata

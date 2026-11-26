// dotnet/decompiler/metadata/method_semantics_lookup.hpp -- two indexes over a module's metadata: which property or event a method is an
// accessor of (get_X, add_Y ...), and which property or event a field is the compiler-made backing field of (<X>k__BackingField, an
// event's field of the event's name).
//
//     auto [owner, kind] = file.method_semantics_lookup().get_semantics(method);   // kind None: not an accessor
//     srm::PropertyDefinitionHandle p;
//     file.property_and_event_backing_field_lookup().is_property_backing_field(field, p);
//
// Ported from ILSpy (ICSharpCode.Decompiler.Metadata: MethodSemanticsLookup, PropertyAndEventBackingFieldLookup), Copyright AlphaSierraPapa
// for the ILSpy team, MIT (see NOTICE in dotnet/decompiler).
#pragma once

#include <unordered_map>
#include <utility>
#include <vector>

#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/metadata/metadata_reader.hpp"

namespace cb::dotnet::decompiler::metadata {

namespace srm = cb::dotnet::metadata;
using typesystem::MethodSemanticsAttributes;

class MethodSemanticsLookup {
public:
    // The getters, setters, adders and removers (Raiser too when asked); UsageError when asked for Other, which SRM does not give.
    explicit MethodSemanticsLookup(const srm::MetadataReader& metadata,
                                   std::uint16_t filter = static_cast<std::uint16_t>(MethodSemanticsAttributes::Getter) |
                                                          static_cast<std::uint16_t>(MethodSemanticsAttributes::Setter) |
                                                          static_cast<std::uint16_t>(MethodSemanticsAttributes::Adder) |
                                                          static_cast<std::uint16_t>(MethodSemanticsAttributes::Remover));
    // The property or event and the accessor kind; a nil handle and None for a method that is no accessor.
    std::pair<srm::EntityHandle, MethodSemanticsAttributes> get_semantics(srm::MethodDefinitionHandle method) const;

private:
    struct Entry {
        MethodSemanticsAttributes semantics;
        std::uint32_t method_row;
        srm::EntityHandle association;
    };
    std::vector<Entry> entries_;   // by method row
};

class PropertyAndEventBackingFieldLookup {
public:
    explicit PropertyAndEventBackingFieldLookup(const srm::MetadataReader& metadata);
    bool is_property_backing_field(srm::FieldDefinitionHandle field, srm::PropertyDefinitionHandle& handle) const;
    bool is_event_backing_field(srm::FieldDefinitionHandle field, srm::EventDefinitionHandle& handle) const;

private:
    std::unordered_map<std::uint32_t, srm::PropertyDefinitionHandle> property_lookup_;   // by field row
    std::unordered_map<std::uint32_t, srm::EventDefinitionHandle> event_lookup_;
};

}  // namespace cb::dotnet::decompiler::metadata

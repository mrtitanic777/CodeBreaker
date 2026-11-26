// dotnet/decompiler/disassembler/entity_processor.cpp -- SortByNameProcessor.
#include "dotnet/decompiler/disassembler/entity_processor.hpp"

#include <algorithm>
#include <format>
#include <string>

#include "dotnet/decompiler/disassembler/disassembler_signature_type_provider.hpp"
#include "dotnet/decompiler/il/instruction_output_extensions.hpp"
#include "dotnet/decompiler/metadata/metadata_extensions.hpp"
#include "dotnet/decompiler/srm_extensions.hpp"

namespace cb::dotnet::decompiler::disassembler {
namespace {

template <class H, class Key>
std::vector<H> sorted(std::vector<H> items, Key key) {
    std::vector<std::pair<std::string, H>> keyed;
    keyed.reserve(items.size());
    for (const H h : items) keyed.emplace_back(key(h), h);
    std::stable_sort(keyed.begin(), keyed.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (std::size_t i = 0; i < keyed.size(); ++i) items[i] = keyed[i].second;
    return items;
}

}  // namespace

std::vector<srm::InterfaceImplementationHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::InterfaceImplementationHandle> items) {
    const srm::MetadataReader& md = module.metadata();
    return sorted(std::move(items), [&](srm::InterfaceImplementationHandle h) {
        return metadata::to_il_name_string(get_full_type_name(md.get_interface_implementation(h).interface_(), md));
    });
}

std::vector<srm::TypeDefinitionHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::TypeDefinitionHandle> items) {
    const srm::MetadataReader& md = module.metadata();
    return sorted(std::move(items), [&](srm::TypeDefinitionHandle h) { return metadata::to_il_name_string(get_full_type_name(h, md)); });
}

std::vector<srm::MethodDefinitionHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::MethodDefinitionHandle> items) {
    return sorted(std::move(items), [&](srm::MethodDefinitionHandle h) {
        PlainTextOutput output;
        const srm::MethodDefinition definition = module.metadata().get_method_definition(h);
        output.write(module.metadata().get_string(definition.name()));   // the name first, without the return type
        DisassemblerSignatureTypeProvider provider(module, output);
        const auto signature = definition.decode_signature(provider, metadata::MetadataGenericContext(h, module));
        if (signature.generic_parameter_count > 0) output.write(std::format("`{}", signature.generic_parameter_count));
        il::write_parameter_list(output, signature);
        return output.text();
    });
}

std::vector<srm::PropertyDefinitionHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::PropertyDefinitionHandle> items) {
    return sorted(std::move(items), [&](srm::PropertyDefinitionHandle h) {
        return std::string(module.metadata().get_string(module.metadata().get_property_definition(h).name()));
    });
}

std::vector<srm::EventDefinitionHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::EventDefinitionHandle> items) {
    return sorted(std::move(items), [&](srm::EventDefinitionHandle h) {
        return std::string(module.metadata().get_string(module.metadata().get_event_definition(h).name()));
    });
}

std::vector<srm::FieldDefinitionHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::FieldDefinitionHandle> items) {
    return sorted(std::move(items), [&](srm::FieldDefinitionHandle h) {
        return std::string(module.metadata().get_string(module.metadata().get_field_definition(h).name()));
    });
}

std::vector<srm::CustomAttributeHandle> SortByNameProcessor::process(const metadata::MetadataFile& module, std::vector<srm::CustomAttributeHandle> items) {
    const srm::MetadataReader& md = module.metadata();
    return sorted(std::move(items), [&](srm::CustomAttributeHandle h) {
        return metadata::to_il_name_string(get_full_type_name(get_declaring_type(md.get_custom_attribute(h).constructor(), md), md));
    });
}

}  // namespace cb::dotnet::decompiler::disassembler

// dotnet/decompiler/disassembler/entity_processor.hpp -- a step between the metadata and the disassembler's output that may reorder (or
// drop) a type's members, attributes and interfaces; and the one that sorts them by name.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: IEntityProcessor, SortByNameProcessor), Copyright AlphaSierraPapa for the ILSpy
// team, MIT (see NOTICE in dotnet/decompiler). Sorting compares the keys ordinally (upstream's OrderBy uses the current culture's string
// comparison), and keeps equal keys in their order, as OrderBy does.
#pragma once

#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::disassembler {

namespace srm = cb::dotnet::metadata;

class IEntityProcessor {
public:
    virtual ~IEntityProcessor() = default;
    virtual std::vector<srm::InterfaceImplementationHandle> process(const metadata::MetadataFile& module, std::vector<srm::InterfaceImplementationHandle> items) = 0;
    virtual std::vector<srm::TypeDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::TypeDefinitionHandle> items) = 0;
    virtual std::vector<srm::MethodDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::MethodDefinitionHandle> items) = 0;
    virtual std::vector<srm::PropertyDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::PropertyDefinitionHandle> items) = 0;
    virtual std::vector<srm::EventDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::EventDefinitionHandle> items) = 0;
    virtual std::vector<srm::FieldDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::FieldDefinitionHandle> items) = 0;
    virtual std::vector<srm::CustomAttributeHandle> process(const metadata::MetadataFile& module, std::vector<srm::CustomAttributeHandle> items) = 0;
};

class SortByNameProcessor final : public IEntityProcessor {
public:
    std::vector<srm::InterfaceImplementationHandle> process(const metadata::MetadataFile& module, std::vector<srm::InterfaceImplementationHandle> items) override;
    std::vector<srm::TypeDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::TypeDefinitionHandle> items) override;
    std::vector<srm::MethodDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::MethodDefinitionHandle> items) override;
    std::vector<srm::PropertyDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::PropertyDefinitionHandle> items) override;
    std::vector<srm::EventDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::EventDefinitionHandle> items) override;
    std::vector<srm::FieldDefinitionHandle> process(const metadata::MetadataFile& module, std::vector<srm::FieldDefinitionHandle> items) override;
    std::vector<srm::CustomAttributeHandle> process(const metadata::MetadataFile& module, std::vector<srm::CustomAttributeHandle> items) override;
};

}  // namespace cb::dotnet::decompiler::disassembler

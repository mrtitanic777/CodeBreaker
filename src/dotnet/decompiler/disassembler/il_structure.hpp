// dotnet/decompiler/disassembler/il_structure.hpp -- a method body's nesting: its try blocks, handlers and filters, and the loops found
// from backward branches, as a tree the disassembler indents by.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: ILStructureType, ILStructure), Copyright AlphaSierraPapa for the ILSpy team, MIT
// (see NOTICE in dotnet/decompiler).
#pragma once

#include <memory>
#include <vector>

#include "dotnet/decompiler/metadata/metadata_file.hpp"

namespace cb::dotnet::decompiler::disassembler {

namespace srm = cb::dotnet::metadata;

enum class ILStructureType : std::uint8_t { Root, Loop, Try, Handler, Filter };

class ILStructure {
public:
    // The whole body's tree.
    ILStructure(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& generic_context,
                const srm::MethodBodyBlock& body);
    ILStructure(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& generic_context,
                ILStructureType type, int start_offset, int end_offset, const srm::ExceptionRegion& handler = {});
    ILStructure(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const metadata::MetadataGenericContext& generic_context,
                ILStructureType type, int start_offset, int end_offset, int loop_entry_point);

    const metadata::MetadataFile& module;
    srm::MethodDefinitionHandle method_handle;
    metadata::MetadataGenericContext generic_context;
    ILStructureType type;
    int start_offset, end_offset;
    srm::ExceptionRegion exception_handler;
    int loop_entry_point_offset = 0;
    std::vector<std::unique_ptr<ILStructure>> children;

    // The innermost structure holding `offset`.
    const ILStructure& get_innermost(int offset) const;

private:
    bool add_nested_structure(std::unique_ptr<ILStructure> s);
    void sort_children();
};

}  // namespace cb::dotnet::decompiler::disassembler

// dotnet/decompiler/disassembler/method_body_disassembler.hpp -- a method body as IL text: the header comments, .maxstack, .locals, and
// each instruction with its operand, indented by try blocks, handlers and loops.
//
// Ported from ILSpy (ICSharpCode.Decompiler.Disassembler: MethodBodyDisassembler), Copyright AlphaSierraPapa for the ILSpy team, MIT (see
// NOTICE in dotnet/decompiler). Not yet ported: sequence points and local names from a PDB (IDebugInfoProvider comes with the debug
// information; without one, as upstream without one, nothing is written for them).
#pragma once

#include "dotnet/decompiler/disassembler/disassembler_signature_type_provider.hpp"
#include "dotnet/decompiler/disassembler/il_structure.hpp"
#include "dotnet/decompiler/util/bit_set.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/metadata/il_opcode.hpp"

namespace cb::dotnet::decompiler::disassembler {

using srm::ILOpCode;

class MethodBodyDisassembler {
public:
    MethodBodyDisassembler(ITextOutput& output, util::CancellationToken cancellation_token) : output_(output), cancellation_token_(cancellation_token) {}
    virtual ~MethodBodyDisassembler() = default;

    bool detect_control_structure = true;
    bool show_sequence_points = false;
    bool show_metadata_tokens = false;
    bool show_metadata_tokens_in_base10 = false;
    bool show_raw_rva_offset_and_bytes = false;

    virtual void disassemble(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle);
    void write_exception_handlers(const metadata::MetadataFile& module, srm::MethodDefinitionHandle handle, const srm::MethodBodyBlock& body);

protected:
    virtual void write_instruction(ITextOutput& output, const metadata::MetadataFile& file, srm::MethodDefinitionHandle method_handle, srm::BlobReader& blob,
                                   int method_rva);

private:
    void disassemble_locals_block(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body);
    void write_structure_header(const ILStructure& s);
    void write_structure_body(const ILStructure& s, const util::BitSet& branch_targets, srm::BlobReader& body, int method_rva);
    void write_structure_footer(const ILStructure& s);
    void write_rva(srm::BlobReader blob, int offset, ILOpCode op);
    void write_op_code(ILOpCode op);
    void write_metadata_token(srm::EntityHandle handle, bool space_before);
    void write_metadata_token(std::optional<srm::Handle> handle, int metadata_token, bool space_before);

    ITextOutput& output_;
    util::CancellationToken cancellation_token_;
    const metadata::MetadataFile* module_ = nullptr;
    const srm::MetadataReader* metadata_ = nullptr;
    metadata::MetadataGenericContext generic_context_;
    std::optional<DisassemblerSignatureTypeProvider> signature_decoder_;
};

}  // namespace cb::dotnet::decompiler::disassembler

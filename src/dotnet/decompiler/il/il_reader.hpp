// dotnet/decompiler/il/il_reader.hpp -- reads a method body's IL into the ILAst: each IL instruction becomes an instruction node, the
// evaluation stack becomes stack slot variables (merged where branches meet), basic blocks become Blocks in BlockContainers nested by
// the exception regions (BlockBuilder), and the result is an ILFunction.
//
//     ILArena arena;  ILArenaScope scope(arena);
//     ILReader reader(module);
//     reader.use_ref_locals_for_accurate_order_of_evaluation = true;
//     ILFunction* function = reader.read_il(method_handle, body);
//     function->write_to(output, ILAstWritingOptions{});
//
// Ported from ILSpy (ICSharpCode.Decompiler.IL: ILReader), Copyright AlphaSierraPapa for the ILSpy team, MIT (see NOTICE in
// dotnet/decompiler). Not yet ported: debug information (the PDB's local names and extra type information; use_debug_symbols reads
// nothing until it is).
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/metadata_module.hpp"
#include "dotnet/decompiler/typesystem/type_provider.hpp"
#include "dotnet/decompiler/util/cancellation.hpp"
#include "dotnet/metadata/pe_reader.hpp"

namespace cb::dotnet::decompiler::il {

namespace srm = cb::dotnet::metadata;
using typesystem::GenericContext;
using typesystem::MetadataModule;

class ILReader {
public:
    explicit ILReader(const MetadataModule& module);
    ~ILReader();
    ILReader(const ILReader&) = delete;
    ILReader& operator=(const ILReader&) = delete;

    bool use_debug_symbols = false;
    // Loads `this` through ldobj.if.ref where a call's target is a reference that might change before the call (C# 11's order).
    bool use_ref_locals_for_accurate_order_of_evaluation = false;
    std::vector<std::string> warnings;
    // Candidate places for sequence points: empty-stack offsets, nops, the instructions after calls.
    std::vector<int> sequence_point_candidates;

    // The method's body as an ILFunction, made in the thread's current ILArena.
    ILFunction* read_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, GenericContext generic_context = GenericContext(),
                        ILFunctionKind kind = ILFunctionKind::TopLevelFunction, const util::CancellationToken& cancellation_token = {});
    // The IL with the stack types at each block's start, then the exception handlers (ILSpy's "typed IL" view).
    void write_typed_il(srm::MethodDefinitionHandle method, const srm::MethodBodyBlock& body, ITextOutput& output,
                        GenericContext generic_context = GenericContext(), const util::CancellationToken& cancellation_token = {});

    // Converts `inst` to `expected_type` the way IL converts implicitly (I4 to I, F4 to F8, a reference to a pointer ...), adding
    // a warning where IL would not.
    static ILInstruction* cast(ILInstruction* inst, StackType expected_type, std::vector<std::string>* warnings, int il_offset);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace cb::dotnet::decompiler::il

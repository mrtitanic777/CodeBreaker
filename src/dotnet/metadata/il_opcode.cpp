// dotnet/metadata/il_opcode.cpp -- the branch helpers of System.Reflection.Metadata's ILOpCodeExtensions.
#include "dotnet/metadata/il_opcode.hpp"

#include "core/error.hpp"

namespace cb::dotnet::metadata {

int get_branch_operand_size(ILOpCode op) {
    const int size = get_branch_operand_size_or_zero(op);
    if (size == 0) throw UsageError("dotnet metadata: an opcode that is not a branch");
    return size;
}

}  // namespace cb::dotnet::metadata

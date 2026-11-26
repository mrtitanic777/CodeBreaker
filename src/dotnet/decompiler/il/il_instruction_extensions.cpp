// dotnet/decompiler/il/il_instruction_extensions.cpp -- see il_instruction_extensions.hpp.
#include "dotnet/decompiler/il/il_instruction_extensions.hpp"

namespace cb::dotnet::decompiler::il {

ILInstruction* get_next_sibling(ILInstruction* instruction) {
    if (instruction == nullptr || instruction->parent() == nullptr) return nullptr;
    if (instruction->child_index() + 1 >= instruction->parent()->get_child_count()) return nullptr;
    return instruction->parent()->get_child(instruction->child_index() + 1);
}

}  // namespace cb::dotnet::decompiler::il

// recompile/pcode_emit_vmx.hpp -- the vector user operations of the Xenon's description (AltiVec and VMX128: vectorAddFloatingPoint,
// vectorPermute, loadVectorLeftIndexed ...) as C++ for the p-code translator (pcode_emit).
//
//     std::optional<vmx::Written> w = vmx::emit("vectorAddFloatingPoint", {a, b}, current, &why);
//
// The description names these operations and gives them no p-code; each is written here as XenonRecomp writes the instruction (SIMDe on
// the context's vectors), so a title's vector code computes what it computed before: the same rounding, the non-Java mode's flushed
// denormals (the flush mode the recompiler sets for them), the same estimates. A vector value is the context's PPCVRegister as an unsigned
// 128-bit integer (its bytes in the context's order: the element-reversed host order the runtime keeps), and the SIMDe expressions read and
// write it with __builtin_bit_cast. What the table has no form for is declined, and the recompiler writes the instruction.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::recompile::pcode_emit::vmx {

// One input of the operation: its value as a C++ expression, the constant when the description passes one (an immediate), its size.
struct Arg {
    std::string value;
    std::optional<std::uint64_t> constant;
    unsigned size = 0;
};

struct Written {
    std::string text;           // a __uint128_t expression (a uint8_t one, the packed field, for vectorCompareSetCR6), or a statement
    bool statement = false;     // the operation has no output: `text` is a statement (a store)
    bool vmx = false;           // it computes with the vector unit's floating point: the recompiler sets the VMX flush mode first
};

// Whether the table has the operation.
bool known(std::string_view name);

// The operation over `args`; `output` is the output register's current value (vectorPackD3D inserts into it), empty without one. Nullopt
// (and `why`) when the table has no form for these arguments.
std::optional<Written> emit(std::string_view name, const std::vector<Arg>& args, const std::string& output, std::string* why);

}  // namespace cb::recompile::pcode_emit::vmx

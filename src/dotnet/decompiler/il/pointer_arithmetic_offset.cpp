// dotnet/decompiler/il/pointer_arithmetic_offset.cpp -- see pointer_arithmetic_offset.hpp.
#include "dotnet/decompiler/il/pointer_arithmetic_offset.hpp"

#include <cstdint>

#include "dotnet/decompiler/il/il_instruction_extensions.hpp"
#include "dotnet/decompiler/il/il_type_extensions.hpp"
#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il {

ILInstruction* PointerArithmeticOffset::detect(ILInstruction* byte_offset_inst, const IType* pointer_element_type, bool check_for_overflow,
                                               bool unwrap_zero_extension) {
    if (pointer_element_type == nullptr) return nullptr;
    if (auto* conv = as<Conv>(byte_offset_inst); conv != nullptr && conv->input_type == StackType::I8 && conv->result_type() == StackType::I) {
        byte_offset_inst = conv->argument();
    }
    std::optional<int> element_size = compute_size_of(*pointer_element_type);
    if (element_size == 1) {
        return byte_offset_inst;
    } else if (auto* mul = as<BinaryNumericInstruction>(byte_offset_inst); mul != nullptr && mul->op == BinaryNumericOperator::Mul) {
        if (mul->is_lifted()) return nullptr;
        if (mul->check_for_overflow != check_for_overflow) return nullptr;
        auto* size_of = as<SizeOf>(mul->right()->unwrap_conv(ConversionKind::SignExtend));
        if ((element_size.has_value() && *element_size > 0 && mul->right()->match_ldc_i(static_cast<std::int64_t>(*element_size))) ||
            (size_of != nullptr &&
             typesystem::NormalizeTypeVisitor::type_erasure().equivalent_types(size_of->type(), pointer_element_type))) {
            ILInstruction* count_offset_inst = mul->left();
            if (unwrap_zero_extension) {
                count_offset_inst = count_offset_inst->unwrap_conv(ConversionKind::ZeroExtend);
            }
            return count_offset_inst;
        }
    } else if (auto* size_of = as<SizeOf>(byte_offset_inst->unwrap_conv(ConversionKind::SignExtend));
               size_of != nullptr && typesystem::type_equals(size_of->type(), pointer_element_type)) {
        return with_il_range(make<LdcI4>(1), byte_offset_inst);
    } else if (std::int64_t val = 0; byte_offset_inst->match_ldc_i(&val)) {
        // If the offset is a constant, it's possible that the compiler constant-folded the multiplication.
        if (element_size.has_value() && *element_size > 0 && (val % *element_size == 0) && val > 0) {
            val /= *element_size;
            if (val <= INT32_MAX) {
                return with_il_range(make<LdcI4>(static_cast<std::int32_t>(val)), byte_offset_inst);
            }
        }
    }
    return nullptr;
}

std::optional<int> PointerArithmeticOffset::compute_size_of(const IType& type) {
    const IType* underlying = get_enum_underlying_type(type);
    const typesystem::ITypeDefinition* definition = underlying->get_definition();
    if (definition == nullptr) return std::nullopt;
    switch (definition->known_type_code()) {
        case KnownTypeCode::Boolean:
        case KnownTypeCode::SByte:
        case KnownTypeCode::Byte: return 1;
        case KnownTypeCode::Char:
        case KnownTypeCode::Int16:
        case KnownTypeCode::UInt16: return 2;
        case KnownTypeCode::Int32:
        case KnownTypeCode::UInt32:
        case KnownTypeCode::Single: return 4;
        case KnownTypeCode::Int64:
        case KnownTypeCode::UInt64:
        case KnownTypeCode::Double: return 8;
        case KnownTypeCode::Decimal: return 16;
        default: return std::nullopt;
    }
}

bool PointerArithmeticOffset::is_fixed_variable(ILInstruction* inst) {
    if (auto* ldloca = as<LdLoca>(inst)) {
        return ldloca->variable()->capture_scope == nullptr;  // locals are fixed if uncaptured
    }
    if (auto* ldflda = as<LdFlda>(inst)) {
        return is_fixed_variable(ldflda->target());
    }
    return inst->result_type() == StackType::I;
}

}  // namespace cb::dotnet::decompiler::il

// dotnet/decompiler/il/il_type_extensions.cpp -- see il_type_extensions.hpp.
#include "dotnet/decompiler/il/il_type_extensions.hpp"

#include "dotnet/decompiler/il/instructions.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/typesystem/types.hpp"

namespace cb::dotnet::decompiler::il {

StackType get_stack_type(PrimitiveType primitive_type) {
    switch (primitive_type) {
        case PrimitiveType::I1:
        case PrimitiveType::U1:
        case PrimitiveType::I2:
        case PrimitiveType::U2:
        case PrimitiveType::I4:
        case PrimitiveType::U4:
            return StackType::I4;
        case PrimitiveType::I8:
        case PrimitiveType::U8:
            return StackType::I8;
        case PrimitiveType::I:
        case PrimitiveType::U:
            return StackType::I;
        case PrimitiveType::R4:
            return StackType::F4;
        case PrimitiveType::R8:
        case PrimitiveType::R:
            return StackType::F8;
        case PrimitiveType::Ref:  // ByRef
            return StackType::Ref;
        case PrimitiveType::Unknown:
            return StackType::Unknown;
        default:
            return StackType::O;
    }
}

Sign get_sign(PrimitiveType primitive_type) {
    switch (primitive_type) {
        case PrimitiveType::I1:
        case PrimitiveType::I2:
        case PrimitiveType::I4:
        case PrimitiveType::I8:
        case PrimitiveType::R4:
        case PrimitiveType::R8:
        case PrimitiveType::R:
        case PrimitiveType::I:
            return Sign::Signed;
        case PrimitiveType::U1:
        case PrimitiveType::U2:
        case PrimitiveType::U4:
        case PrimitiveType::U8:
        case PrimitiveType::U:
            return Sign::Unsigned;
        default:
            return Sign::None;
    }
}

bool has_opposite_sign(PrimitiveType primitive_type) {
    switch (primitive_type) {
        case PrimitiveType::I1:
        case PrimitiveType::I2:
        case PrimitiveType::I4:
        case PrimitiveType::I8:
        case PrimitiveType::U1:
        case PrimitiveType::U2:
        case PrimitiveType::U4:
        case PrimitiveType::U8:
        case PrimitiveType::I:
        case PrimitiveType::U:
            return true;
        default:
            return false;
    }
}

int get_size(PrimitiveType type) {
    switch (type) {
        case PrimitiveType::I1:
        case PrimitiveType::U1:
            return 1;
        case PrimitiveType::I2:
        case PrimitiveType::U2:
            return 2;
        case PrimitiveType::I4:
        case PrimitiveType::U4:
        case PrimitiveType::R4:
            return 4;
        case PrimitiveType::I8:
        case PrimitiveType::R8:
        case PrimitiveType::U8:
        case PrimitiveType::R:
            return 8;
        case PrimitiveType::I:
        case PrimitiveType::U:
        case PrimitiveType::Ref:
            return native_int_size;
        default:
            return 0;
    }
}

bool is_small_integer_type(PrimitiveType type) { return get_size(type) < 4; }

bool is_integer_type(PrimitiveType primitive_type) { return is_integer_type(get_stack_type(primitive_type)); }

bool is_float_type(PrimitiveType type) {
    switch (type) {
        case PrimitiveType::R4:
        case PrimitiveType::R8:
        case PrimitiveType::R:
            return true;
        default:
            return false;
    }
}

const IType* infer_type(ILInstruction* inst, const ICompilation* compilation) {
    using namespace typesystem;
    const IType* unknown = &SpecialType::unknown_type();
    switch (inst->op_code) {
        case OpCode::NewObj: {
            const IType* declaring = static_cast<NewObj*>(inst)->method->declaring_type();
            return declaring != nullptr ? declaring : unknown;
        }
        case OpCode::NewArr: {
            auto* new_arr = static_cast<NewArr*>(inst);
            if (compilation != nullptr) return make_array_type(*compilation, new_arr->type(), new_arr->indices().count());
            return unknown;
        }
        case OpCode::Call: return static_cast<Call*>(inst)->method->return_type();
        case OpCode::CallVirt: return static_cast<CallVirt*>(inst)->method->return_type();
        case OpCode::CallIndirect: return static_cast<CallIndirect*>(inst)->function_pointer_type()->return_type;
        case OpCode::UserDefinedLogicOperator: return static_cast<UserDefinedLogicOperator*>(inst)->method()->return_type();
        case OpCode::LdObj: return static_cast<LdObj*>(inst)->type();
        case OpCode::StObj: return static_cast<StObj*>(inst)->type();
        case OpCode::LdLoc: return static_cast<LdLoc*>(inst)->variable()->type();
        case OpCode::StLoc: return static_cast<StLoc*>(inst)->variable()->type();
        case OpCode::LdLoca: return make_by_reference_type(static_cast<LdLoca*>(inst)->variable()->type());
        case OpCode::LdFlda: return make_by_reference_type(static_cast<LdFlda*>(inst)->field()->type());
        case OpCode::LdsFlda: return make_by_reference_type(static_cast<LdsFlda*>(inst)->field()->type());
        case OpCode::LdElema: {
            auto* ldelema = static_cast<LdElema*>(inst);
            if (const auto* array_type = dynamic_cast<const ArrayType*>(infer_type(ldelema->array(), compilation))) {
                if (is_compatible_type_for_memory_access(*array_type->element_type(), *ldelema->type()))
                    return make_by_reference_type(array_type->element_type());
            }
            return make_by_reference_type(ldelema->type());
        }
        case OpCode::Comp: {
            if (compilation == nullptr) return unknown;
            switch (static_cast<Comp*>(inst)->lifting_kind) {
                case ComparisonLiftingKind::None:
                case ComparisonLiftingKind::CSharp:
                    return compilation->find_type(KnownTypeCode::Boolean);
                case ComparisonLiftingKind::ThreeValuedLogic:
                    return nullable_type::create(*compilation, compilation->find_type(KnownTypeCode::Boolean));
            }
            return unknown;
        }
        case OpCode::BinaryNumericInstruction: {
            auto* bni = static_cast<BinaryNumericInstruction*>(inst);
            if (bni->is_lifted()) return unknown;
            switch (bni->op) {
                case BinaryNumericOperator::BitAnd:
                case BinaryNumericOperator::BitOr:
                case BinaryNumericOperator::BitXor: {
                    const IType* left = infer_type(bni->left(), compilation);
                    const IType* right = infer_type(bni->right(), compilation);
                    if (left->equals(right) && (is_csharp_primitive_integer_type(*left) || is_csharp_native_integer_type(*left) ||
                                                is_known_type(left, KnownTypeCode::Boolean)))
                        return left;
                    return unknown;
                }
                default:
                    return unknown;
            }
        }
        case OpCode::DefaultValue: return static_cast<DefaultValue*>(inst)->type();
        case OpCode::ILFunction: {
            auto* func = static_cast<ILFunction*>(inst);
            if (func->delegate_type != nullptr) return func->delegate_type;
            return unknown;
        }
        default:
            return unknown;
    }
}

}  // namespace cb::dotnet::decompiler::il

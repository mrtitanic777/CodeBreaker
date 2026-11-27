// dotnet/decompiler/il/expression_instructions.cpp -- see expression_instructions.hpp.
#include "dotnet/decompiler/il/expression_instructions.hpp"

#include <algorithm>
#include <cassert>
#include <format>
#include <stdexcept>

#include "dotnet/decompiler/csharp/resolver/csharp_operators.hpp"
#include "dotnet/decompiler/disassembler/disassembler_helpers.hpp"
#include "dotnet/decompiler/il/control_flow_instructions.hpp"
#include "dotnet/decompiler/il/instruction_output_il.hpp"
#include "dotnet/decompiler/il/patterns.hpp"
#include "dotnet/decompiler/il/semantic_helper.hpp"
#include "dotnet/decompiler/il/transforms/il_extraction.hpp"
#include "dotnet/decompiler/typesystem/tuple_type.hpp"
#include "dotnet/decompiler/typesystem/type_system.hpp"
#include "dotnet/decompiler/typesystem/type_system_extensions.hpp"
#include "dotnet/decompiler/util/char_info.hpp"

namespace cb::dotnet::decompiler::il {

namespace {
[[noreturn]] void index_out_of_range() { throw std::out_of_range("child index"); }

bool conv_needs_sign(StackType input_type, PrimitiveType target_type, bool check_for_overflow) {
    return check_for_overflow || (!is_float_type(input_type) && is_float_type(target_type));
}

void write_operand_string(ITextOutput& output, const std::string& value) {
    disassembler::write_operand(output, std::u16string_view(util::utf16(value)));
}
}  // namespace

// ---- BinaryNumericInstruction

BinaryNumericInstruction::BinaryNumericInstruction(BinaryNumericOperator op_, ILInstruction* left, ILInstruction* right, bool check_overflow,
                                                   Sign sign_)
    : BinaryNumericInstruction(op_, left, right, left->result_type(), right->result_type(), check_overflow, sign_) {}

BinaryNumericInstruction::BinaryNumericInstruction(BinaryNumericOperator op_, ILInstruction* left, ILInstruction* right,
                                                   StackType left_input, StackType right_input, bool check_overflow, Sign sign_,
                                                   bool lifted)
    : BinaryInstruction(OpCode::BinaryNumericInstruction, left, right),
      check_for_overflow(check_overflow),
      sign(sign_),
      left_input_type(left_input),
      right_input_type(right_input),
      op(op_),
      is_lifted_(lifted),
      result_type_(compute_result_type(op_, left_input, right_input)) {}

StackType BinaryNumericInstruction::compute_result_type(BinaryNumericOperator op, StackType left, StackType right) {
    // Based on Table 2: Binary Numeric Operations
    // also works for Table 5: Integer Operations
    // and for Table 7: Overflow Arithmetic Operations
    if (left == right || op == BinaryNumericOperator::ShiftLeft || op == BinaryNumericOperator::ShiftRight) {
        // Shift op codes use Table 6
        return left;
    }
    if (left == StackType::Ref || right == StackType::Ref) {
        if (left == StackType::Ref && right == StackType::Ref) {
            // sub(&, &) = I
            assert(op == BinaryNumericOperator::Sub);
            return StackType::I;
        }
        // add/sub with I or I4 and &
        assert(op == BinaryNumericOperator::Add || op == BinaryNumericOperator::Sub);
        return StackType::Ref;
    }
    return StackType::Unknown;
}

void BinaryNumericInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    if (!is_lifted_) {
        assert(left_input_type == left()->result_type());
        assert(right_input_type == right()->result_type());
    }
}

InstructionFlags BinaryNumericInstruction::compute_flags() const {
    auto flags = BinaryInstruction::compute_flags();
    if (check_for_overflow || (op == BinaryNumericOperator::Div || op == BinaryNumericOperator::Rem)) flags |= InstructionFlags::MayThrow;
    return flags;
}

InstructionFlags BinaryNumericInstruction::direct_flags() const {
    if (check_for_overflow || (op == BinaryNumericOperator::Div || op == BinaryNumericOperator::Rem))
        return BinaryInstruction::direct_flags() | InstructionFlags::MayThrow;
    return BinaryInstruction::direct_flags();
}

std::string BinaryNumericInstruction::get_operator_name(BinaryNumericOperator op) {
    switch (op) {
        case BinaryNumericOperator::Add: return "add";
        case BinaryNumericOperator::Sub: return "sub";
        case BinaryNumericOperator::Mul: return "mul";
        case BinaryNumericOperator::Div: return "div";
        case BinaryNumericOperator::Rem: return "rem";
        case BinaryNumericOperator::BitAnd: return "bit.and";
        case BinaryNumericOperator::BitOr: return "bit.or";
        case BinaryNumericOperator::BitXor: return "bit.xor";
        case BinaryNumericOperator::ShiftLeft: return "bit.shl";
        case BinaryNumericOperator::ShiftRight: return "bit.shr";
        default: throw std::out_of_range("BinaryNumericOperator");
    }
}

void BinaryNumericInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write("." + get_operator_name(op));
    if (check_for_overflow) output.write(".ovf");
    if (sign == Sign::Unsigned)
        output.write(".unsigned");
    else if (sign == Sign::Signed)
        output.write(".signed");
    output.write('.');
    write(output, result_type_);
    if (is_lifted_) output.write(".lifted");
    output.write('(');
    left()->write_to(output, options);
    output.write(", ");
    right()->write_to(output, options);
    output.write(')');
}

ILInstruction* BinaryNumericInstruction::clone() const {
    auto* clone = make<BinaryNumericInstruction>(*this);
    clone_children_into(*clone);
    return clone;
}

bool BinaryNumericInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<BinaryNumericInstruction>(other);
    return o != nullptr && left()->perform_match(o->left(), match) && right()->perform_match(o->right(), match) &&
           check_for_overflow == o->check_for_overflow && sign == o->sign && op == o->op && is_lifted_ == o->is_lifted_;
}

// ---- BitNot

BitNot::BitNot(ILInstruction* arg) : UnaryInstruction(OpCode::BitNot, arg), underlying_result_type_(arg->result_type()) {}

BitNot::BitNot(ILInstruction* arg, bool lifted, StackType stack_type)
    : UnaryInstruction(OpCode::BitNot, arg), is_lifted_(lifted), underlying_result_type_(stack_type) {}

void BitNot::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(is_lifted_ == (result_type() == StackType::O));
    assert(is_lifted_ || result_type() == underlying_result_type_);
}

void BitNot::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (is_lifted_) output.write(".lifted");
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}

ILInstruction* BitNot::clone() const {
    auto* clone = make<BitNot>(*this);
    clone_children_into(*clone);
    return clone;
}

bool BitNot::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<BitNot>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && is_lifted_ == o->is_lifted_ &&
           underlying_result_type_ == o->underlying_result_type_;
}

// ---- Ckfinite

ILInstruction* Ckfinite::clone() const {
    auto* clone = make<Ckfinite>(*this);
    clone_children_into(*clone);
    return clone;
}

bool Ckfinite::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Ckfinite>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

// ---- Conv

Conv::Conv(ILInstruction* argument, PrimitiveType target, bool check_overflow, Sign input_sign_)
    : Conv(argument, argument->result_type(), input_sign_, target, check_overflow) {}

Conv::Conv(ILInstruction* argument, StackType input, Sign input_sign_, PrimitiveType target, bool check_overflow, bool lifted)
    : UnaryInstruction(OpCode::Conv, argument),
      kind(ConversionKind::Invalid),
      check_for_overflow(check_overflow),
      input_type(input),
      input_sign(conv_needs_sign(input, target, check_overflow) ? input_sign_ : Sign::None),
      target_type(target),
      is_lifted_(lifted) {
    assert(!(conv_needs_sign(input, target, check_overflow) && input_sign_ == Sign::None));
    kind = get_conversion_kind(target, input_type, input_sign);
    // (an invalid conversion can happen with invalid IL or missing references)
}

void Conv::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument()->result_type() == (is_lifted_ ? StackType::O : input_type));
    assert(!(is_lifted_ && kind == ConversionKind::StopGCTracking));
}

ConversionKind Conv::get_conversion_kind(PrimitiveType target, StackType input, Sign input_sign) {
    switch (target) {
        case PrimitiveType::I1:
        case PrimitiveType::I2:
        case PrimitiveType::U1:
        case PrimitiveType::U2:
            switch (input) {
                case StackType::I4:
                case StackType::I8:
                case StackType::I:
                    return ConversionKind::Truncate;
                case StackType::F4:
                case StackType::F8:
                    return ConversionKind::FloatToInt;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::I4:
        case PrimitiveType::U4:
            switch (input) {
                case StackType::I4:
                    return ConversionKind::Nop;
                case StackType::I:
                case StackType::I8:
                    return ConversionKind::Truncate;
                case StackType::F4:
                case StackType::F8:
                    return ConversionKind::FloatToInt;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::I8:
        case PrimitiveType::U8:
            switch (input) {
                case StackType::I4:
                case StackType::I:
                    if (input_sign == Sign::None) return target == PrimitiveType::I8 ? ConversionKind::SignExtend : ConversionKind::ZeroExtend;
                    return input_sign == Sign::Signed ? ConversionKind::SignExtend : ConversionKind::ZeroExtend;
                case StackType::I8:
                    return ConversionKind::Nop;
                case StackType::F4:
                case StackType::F8:
                    return ConversionKind::FloatToInt;
                case StackType::Ref:
                case StackType::O:
                    return ConversionKind::StopGCTracking;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::I:
        case PrimitiveType::U:
            switch (input) {
                case StackType::I4:
                    if (input_sign == Sign::None) return target == PrimitiveType::I ? ConversionKind::SignExtend : ConversionKind::ZeroExtend;
                    return input_sign == Sign::Signed ? ConversionKind::SignExtend : ConversionKind::ZeroExtend;
                case StackType::I:
                    return ConversionKind::Nop;
                case StackType::I8:
                    return ConversionKind::Truncate;
                case StackType::F4:
                case StackType::F8:
                    return ConversionKind::FloatToInt;
                case StackType::Ref:
                case StackType::O:
                    return ConversionKind::StopGCTracking;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::R4:
            switch (input) {
                case StackType::I4:
                case StackType::I:
                case StackType::I8:
                    return ConversionKind::IntToFloat;
                case StackType::F4:
                    return ConversionKind::Nop;
                case StackType::F8:
                    return ConversionKind::FloatPrecisionChange;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::R:
        case PrimitiveType::R8:
            switch (input) {
                case StackType::I4:
                case StackType::I:
                case StackType::I8:
                    return ConversionKind::IntToFloat;
                case StackType::F4:
                    return ConversionKind::FloatPrecisionChange;
                case StackType::F8:
                    return ConversionKind::Nop;
                default:
                    return ConversionKind::Invalid;
            }
        case PrimitiveType::Ref:
            // There's no "conv.ref" in IL, but IL allows these conversions implicitly, whereas we represent them explicitly in the ILAst.
            switch (input) {
                case StackType::I4:
                case StackType::I:
                case StackType::I8:
                    return ConversionKind::StartGCTracking;
                case StackType::O:
                    return ConversionKind::ObjectInterior;
                default:
                    return ConversionKind::Invalid;
            }
        default:
            return ConversionKind::Invalid;
    }
}

void Conv::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (check_for_overflow) output.write(".ovf");
    if (input_sign == Sign::Unsigned)
        output.write(".unsigned");
    else if (input_sign == Sign::Signed)
        output.write(".signed");
    if (is_lifted_) output.write(".lifted");
    output.write(' ');
    write(output, input_type);
    output.write("->");
    write(output, target_type);
    output.write(' ');
    switch (kind) {
        case ConversionKind::SignExtend: output.write("<sign extend>"); break;
        case ConversionKind::ZeroExtend: output.write("<zero extend>"); break;
        case ConversionKind::Invalid: output.write("<invalid>"); break;
        default: break;
    }
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}

InstructionFlags Conv::compute_flags() const {
    auto flags = UnaryInstruction::compute_flags();
    if (check_for_overflow) flags |= InstructionFlags::MayThrow;
    return flags;
}

ILInstruction* Conv::unwrap_conv(ConversionKind conversion_kind) {
    if (kind == conversion_kind && !is_lifted_) return argument()->unwrap_conv(conversion_kind);
    return this;
}

ILInstruction* Conv::clone() const {
    auto* clone = make<Conv>(*this);
    clone_children_into(*clone);
    return clone;
}

bool Conv::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Conv>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && check_for_overflow == o->check_for_overflow && kind == o->kind &&
           input_sign == o->input_sign && target_type == o->target_type && is_lifted_ == o->is_lifted_;
}

// ---- Comp

bool is_equality_or_inequality(ComparisonKind kind) { return kind == ComparisonKind::Equality || kind == ComparisonKind::Inequality; }

ComparisonKind negate(ComparisonKind kind) {
    switch (kind) {
        case ComparisonKind::Equality: return ComparisonKind::Inequality;
        case ComparisonKind::Inequality: return ComparisonKind::Equality;
        case ComparisonKind::LessThan: return ComparisonKind::GreaterThanOrEqual;
        case ComparisonKind::LessThanOrEqual: return ComparisonKind::GreaterThan;
        case ComparisonKind::GreaterThan: return ComparisonKind::LessThanOrEqual;
        case ComparisonKind::GreaterThanOrEqual: return ComparisonKind::LessThan;
    }
    throw std::out_of_range("ComparisonKind");
}

csharp::syntax::BinaryOperatorType to_binary_operator_type(ComparisonKind kind) {
    using csharp::syntax::BinaryOperatorType;
    switch (kind) {
        case ComparisonKind::Equality: return BinaryOperatorType::Equality;
        case ComparisonKind::Inequality: return BinaryOperatorType::InEquality;
        case ComparisonKind::LessThan: return BinaryOperatorType::LessThan;
        case ComparisonKind::LessThanOrEqual: return BinaryOperatorType::LessThanOrEqual;
        case ComparisonKind::GreaterThan: return BinaryOperatorType::GreaterThan;
        case ComparisonKind::GreaterThanOrEqual: return BinaryOperatorType::GreaterThanOrEqual;
    }
    throw std::out_of_range("ComparisonKind");
}

std::string get_token(ComparisonKind kind) {
    switch (kind) {
        case ComparisonKind::Equality: return "==";
        case ComparisonKind::Inequality: return "!=";
        case ComparisonKind::LessThan: return "<";
        case ComparisonKind::LessThanOrEqual: return "<=";
        case ComparisonKind::GreaterThan: return ">";
        case ComparisonKind::GreaterThanOrEqual: return ">=";
    }
    throw std::out_of_range("ComparisonKind");
}

Comp::Comp(ComparisonKind kind, Sign sign_, ILInstruction* left, ILInstruction* right)
    : BinaryInstruction(OpCode::Comp, left, right),
      lifting_kind(ComparisonLiftingKind::None),
      input_type(left->result_type()),
      sign(sign_),
      kind_(kind) {
    assert(left->result_type() == right->result_type());
}

Comp::Comp(ComparisonKind kind, ComparisonLiftingKind lifting, StackType input, Sign sign_, ILInstruction* left, ILInstruction* right)
    : BinaryInstruction(OpCode::Comp, left, right), lifting_kind(lifting), input_type(input), sign(sign_), kind_(kind) {}

void Comp::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    if (lifting_kind == ComparisonLiftingKind::None) {
        assert(left()->result_type() == input_type);
        assert(right()->result_type() == input_type);
    } else {
        assert(left()->result_type() == input_type || left()->result_type() == StackType::O);
        assert(right()->result_type() == input_type || right()->result_type() == StackType::O);
    }
}

void Comp::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    ILInstruction* arg = nullptr;
    if (options.use_logic_operation_sugar && match_logic_not(&arg)) {
        output.write("logic.not(");
        arg->write_to(output, options);
        output.write(')');
        return;
    }
    write(output, op_code);
    output.write('.');
    write(output, input_type);
    switch (sign) {
        case Sign::Signed: output.write(".signed"); break;
        case Sign::Unsigned: output.write(".unsigned"); break;
        default: break;
    }
    switch (lifting_kind) {
        case ComparisonLiftingKind::CSharp: output.write(".lifted[C#]"); break;
        case ComparisonLiftingKind::ThreeValuedLogic: output.write(".lifted[3VL]"); break;
        default: break;
    }
    output.write('(');
    left()->write_to(output, options);
    output.write(' ');
    output.write(get_token(kind_));
    output.write(' ');
    right()->write_to(output, options);
    output.write(')');
}

Comp* Comp::logic_not(ILInstruction* arg) { return make<Comp>(ComparisonKind::Equality, Sign::None, arg, make<LdcI4>(0)); }

Comp* Comp::logic_not(ILInstruction* arg, bool is_lifted) {
    auto lifting = is_lifted ? ComparisonLiftingKind::ThreeValuedLogic : ComparisonLiftingKind::None;
    return make<Comp>(ComparisonKind::Equality, lifting, StackType::I4, Sign::None, arg, make<LdcI4>(0));
}

bool Comp::can_inline_into_slot(int, ILInstruction* expression_being_moved) {
    // ExpressionBuilder translates comp.o(a op b) for op not in (==, !=) into Unsafe.As(ref a) op Unsafe.As(ref b), which requires that a
    // and b are variables and not expressions. Returning false in those cases prevents inlining. However if one of the arguments is
    // LdNull, then we don't need the Unsafe.As trickery, and can always inline.
    if (is_equality_or_inequality(kind_) || input_type != StackType::O) {
        // OK, won't need Unsafe.As.
        return true;
    }
    const IField* static_field = nullptr;
    if (is<LdLoc>(expression_being_moved) || expression_being_moved->match_lds_fld(&static_field)) {
        // OK, can use variable/field name with Unsafe.As(ref x)
        return true;
    }
    if (sign != Sign::Signed && (is<LdNull>(expression_being_moved) || is<LdNull>(left()) || is<LdNull>(right()))) {
        // OK, this is the "compare with null" special case that doesn't need Unsafe.As()
        return true;
    }
    return false;
}

ILInstruction* Comp::clone() const {
    auto* clone = make<Comp>(*this);
    clone_children_into(*clone);
    return clone;
}

bool Comp::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<Comp>(other);
    return o != nullptr && left()->perform_match(o->left(), match) && right()->perform_match(o->right(), match) && kind_ == o->kind_ &&
           sign == o->sign && lifting_kind == o->lifting_kind;
}

// ---- CallInstruction, Call, CallVirt, NewObj

const SlotInfo CallInstruction::arguments_slot{"Arguments", true};

CallInstruction* CallInstruction::create(OpCode op_code, const IMethod* method) {
    switch (op_code) {
        case OpCode::Call: return make<Call>(method);
        case OpCode::CallVirt: return make<CallVirt>(method);
        case OpCode::NewObj: return make<NewObj>(method);
        default: throw std::invalid_argument("Not a valid call opcode");
    }
}

CallInstruction::CallInstruction(OpCode op_code, const IMethod* method_) : ILInstruction(op_code), method(method_), arguments_(this, 0) {
    if (method_ == nullptr) throw std::invalid_argument("method");
}

CallInstruction::CallInstruction(const CallInstruction& other)
    : ILInstruction(other),
      method(other.method),
      is_tail(other.is_tail),
      constrained_to(other.constrained_to),
      il_stack_was_empty(other.il_stack_was_empty),
      arguments_(this, 0) {}

void CallInstruction::clone_arguments_into(CallInstruction& clone) const {
    std::vector<ILInstruction*> cloned;
    for (ILInstruction* arg : arguments_) cloned.push_back(arg->clone());
    clone.arguments_.add_range(cloned);
}

bool CallInstruction::is_instance_call() const { return !(method->is_static() || op_code == OpCode::NewObj); }

const IParameter* CallInstruction::get_parameter(int argument_index) const {
    int first_param_index = (method->is_static() || op_code == OpCode::NewObj) ? 0 : 1;
    if (argument_index < first_param_index) return nullptr;  // asking for 'this' parameter
    return method->parameters().at(static_cast<std::size_t>(argument_index - first_param_index));
}

StackType CallInstruction::result_type() const {
    if (op_code == OpCode::NewObj) return get_stack_type(*method->declaring_type());
    return get_stack_type(*method->return_type());
}

StackType CallInstruction::expected_type_for_this_pointer(const IType& declaring_type, const IType* constrained) {
    if (constrained != nullptr) return StackType::Ref;
    if (declaring_type.kind() == typesystem::TypeKind::TypeParameter) return StackType::Ref;
    std::optional<bool> is_reference = declaring_type.is_reference_type();
    if (!is_reference.has_value()) return StackType::Unknown;
    return *is_reference ? StackType::O : StackType::Ref;
}

void CallInstruction::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    int first_argument = (op_code != OpCode::NewObj && !method->is_static()) ? 1 : 0;
    assert(static_cast<int>(method->parameters().size()) + first_argument == arguments_.count());
    if (first_argument == 1) {
        assert(arguments_[0]->result_type() == expected_type_for_this_pointer(*method->declaring_type(), constrained_to) &&
               "Stack type mismatch in 'this' argument in call");
    }
    for (std::size_t i = 0; i < method->parameters().size(); ++i) {
        assert(arguments_[first_argument + static_cast<int>(i)]->result_type() == get_stack_type(*method->parameters()[i]->type()) &&
               "Stack type mismatch in parameter in call");
    }
}

InstructionFlags CallInstruction::compute_flags() const {
    InstructionFlags flags = InstructionFlags::None;
    for (ILInstruction* arg : arguments_) flags = flags | arg->flags();
    return flags | InstructionFlags::MayThrow | InstructionFlags::SideEffect;
}

void CallInstruction::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    if (constrained_to != nullptr) {
        output.write("constrained[");
        il::write_to(*constrained_to, output);
        output.write("].");
    }
    if (is_tail) output.write("tail.");
    write(output, op_code);
    output.write(' ');
    il::write_to(*method, output);
    output.write('(');
    for (int i = 0; i < arguments_.count(); i++) {
        if (i > 0) output.write(", ");
        arguments_[i]->write_to(output, options);
    }
    output.write(')');
}

bool CallInstruction::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = dynamic_cast<CallInstruction*>(other);
    return o != nullptr && op_code == o->op_code && method->equals(o->method) && is_tail == o->is_tail &&
           typesystem::type_equals(constrained_to, o->constrained_to) &&
           patterns::ListMatch::do_match(arguments_, o->arguments_, match);
}

bool Call::is_lifted() const {
    return dynamic_cast<const csharp::resolver::ILiftedOperator*>(method) != nullptr;
}

StackType Call::underlying_result_type() const {
    if (const auto* lifted_operator = dynamic_cast<const csharp::resolver::ILiftedOperator*>(method))
        return get_stack_type(*lifted_operator->non_lifted_return_type());
    return get_stack_type(*method->return_type());
}

ILInstruction* Call::clone() const {
    auto* clone = make<Call>(*this);
    clone_arguments_into(*clone);
    return clone;
}

ILInstruction* CallVirt::clone() const {
    auto* clone = make<CallVirt>(*this);
    clone_arguments_into(*clone);
    return clone;
}

StackType NewObj::result_type() const { return get_stack_type(*method->declaring_type()); }

ILInstruction* NewObj::clone() const {
    auto* clone = make<NewObj>(*this);
    clone_arguments_into(*clone);
    return clone;
}

// ---- CallIndirect

const SlotInfo CallIndirect::function_pointer_slot{"FunctionPointer", true};
const SlotInfo CallIndirect::argument_slot{"Argument", true, true};

CallIndirect::CallIndirect(bool is_instance, bool has_explicit_this, const FunctionPointerType* function_pointer_type,
                           ILInstruction* function_pointer, const std::vector<ILInstruction*>& args)
    : ILInstruction(OpCode::CallIndirect),
      arguments(this, 1),
      is_instance_(is_instance),
      has_explicit_this_(has_explicit_this),
      function_pointer_type_(function_pointer_type) {
    set_function_pointer(function_pointer);
    arguments.add_range(args);
}

ILInstruction* CallIndirect::clone() const {
    ILInstruction* cloned_pointer = function_pointer_->clone();
    std::vector<ILInstruction*> cloned_arguments;
    for (ILInstruction* inst : arguments) cloned_arguments.push_back(inst->clone());
    auto* clone = make<CallIndirect>(is_instance_, has_explicit_this_, function_pointer_type_, cloned_pointer, cloned_arguments);
    clone->add_il_range(*this);
    return clone;
}

StackType CallIndirect::result_type() const { return get_stack_type(*function_pointer_type_->return_type); }

void CallIndirect::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(static_cast<std::size_t>(arguments.count()) == function_pointer_type_->parameter_types.size() + (is_instance_ ? 1 : 0));
}

void CallIndirect::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("call.indirect ");
    il::write_to(*function_pointer_type_->return_type, output);
    output.write('(');
    function_pointer_->write_to(output, options);
    int first_argument = is_instance_ ? 1 : 0;
    if (first_argument == 1) {
        output.write(", ");
        arguments[0]->write_to(output, options);
    }
    std::size_t pairs = std::min(static_cast<std::size_t>(arguments.count()), function_pointer_type_->parameter_types.size());
    for (std::size_t i = 0; i < pairs; ++i) {
        output.write(", ");
        arguments[static_cast<int>(i)]->write_to(output, options);
        output.write(" : ");
        il::write_to(*function_pointer_type_->parameter_types[i], output);
    }
    if (arguments.count() > 0) output.write(')');
}

ILInstruction* CallIndirect::get_child(int index) const {
    if (index == 0) return function_pointer_;
    return arguments[index - 1];
}

void CallIndirect::set_child(int index, ILInstruction* value) {
    if (index == 0)
        set_function_pointer(value);
    else
        arguments.set(index - 1, value);
}

const SlotInfo& CallIndirect::get_child_slot(int index) const {
    if (index == 0) return function_pointer_slot;
    return argument_slot;
}

InstructionFlags CallIndirect::compute_flags() const {
    auto flags = direct_flags();
    flags |= function_pointer_->flags();
    for (ILInstruction* inst : arguments) flags |= inst->flags();
    return flags;
}

bool CallIndirect::equal_signature(const CallIndirect& other) const {
    if (is_instance_ != other.is_instance_) return false;
    if (has_explicit_this_ != other.has_explicit_this_) return false;
    return function_pointer_type_->equals(other.function_pointer_type_);
}

bool CallIndirect::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<CallIndirect>(other);
    return o != nullptr && equal_signature(*o) && patterns::ListMatch::do_match(arguments, o->arguments, match) &&
           function_pointer_->perform_match(o->function_pointer_, match);
}

// ---- LdLoc

LdLoc::LdLoc(ILVariable* variable) : SimpleInstruction(OpCode::LdLoc), variable_(variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
}

void LdLoc::connected() {
    ILInstruction::connected();
    variable_->add_load_instruction(this);
}

void LdLoc::disconnected() {
    variable_->remove_load_instruction(this);
    ILInstruction::disconnected();
}

void LdLoc::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    variable_->write_to(output);
}

ILInstruction* LdLoc::clone() const { return make<LdLoc>(*this); }

bool LdLoc::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdLoc>(other);
    return o != nullptr && variable_ == o->variable_;
}

void LdLoc::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
}

// ---- LdLoca

LdLoca::LdLoca(ILVariable* variable) : SimpleInstruction(OpCode::LdLoca), variable_(variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
}

void LdLoca::connected() {
    ILInstruction::connected();
    variable_->add_address_instruction(this);
}

void LdLoca::disconnected() {
    variable_->remove_address_instruction(this);
    ILInstruction::disconnected();
}

void LdLoca::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    variable_->write_to(output);
}

ILInstruction* LdLoca::clone() const { return make<LdLoca>(*this); }

bool LdLoca::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdLoca>(other);
    return o != nullptr && variable_ == o->variable_;
}

void LdLoca::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
}

// ---- StLoc

const SlotInfo StLoc::value_slot{"Value", true};

StLoc::StLoc(ILVariable* variable, ILInstruction* value) : ILInstruction(OpCode::StLoc), variable_(variable) {
    if (variable == nullptr) throw std::invalid_argument("variable");
    set_value(value);
}

void StLoc::connected() {
    ILInstruction::connected();
    variable_->add_store_instruction(this);
}

void StLoc::disconnected() {
    variable_->remove_store_instruction(this);
    ILInstruction::disconnected();
}

ILInstruction* StLoc::get_child(int index) const {
    if (index == 0) return value_;
    index_out_of_range();
}

void StLoc::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_value(value);
    index_out_of_range();
}

const SlotInfo& StLoc::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    index_out_of_range();
}

void StLoc::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    variable_->write_to(output);
    output.write('(');
    value_->write_to(output, options);
    output.write(')');
}

ILInstruction* StLoc::clone() const {
    auto* clone = make<StLoc>(*this);
    clone->set_value(value_->clone());
    return clone;
}

bool StLoc::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<StLoc>(other);
    return o != nullptr && variable_ == o->variable_ && value_->perform_match(o->value_, match);
}

void StLoc::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(phase <= ILPhase::InILReader || is_descendant_of(variable_->function()));
    assert(phase <= ILPhase::InILReader || variable_->function()->variables()[variable_->index_in_function()] == variable_);
    assert(value_->result_type() == variable_->stack_type);
}

// ---- AddressOf

const SlotInfo AddressOf::value_slot{"Value", true};

AddressOf::AddressOf(ILInstruction* value, const IType* type) : ILInstruction(OpCode::AddressOf), type_(type) { set_value(value); }

ILInstruction* AddressOf::get_child(int index) const {
    if (index == 0) return value_;
    index_out_of_range();
}

void AddressOf::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_value(value);
    index_out_of_range();
}

const SlotInfo& AddressOf::get_child_slot(int index) const {
    if (index == 0) return value_slot;
    index_out_of_range();
}

void AddressOf::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    value_->write_to(output, options);
    output.write(')');
}

ILInstruction* AddressOf::clone() const {
    auto* clone = make<AddressOf>(*this);
    clone->set_value(value_->clone());
    return clone;
}

bool AddressOf::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<AddressOf>(other);
    return o != nullptr && value_->perform_match(o->value_, match) && type_->equals(o->type_);
}

// ---- ThreeValuedBoolAnd, ThreeValuedBoolOr

void ThreeValuedBoolAnd::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(left()->result_type() == StackType::I4 || left()->result_type() == StackType::O);
}

ILInstruction* ThreeValuedBoolAnd::clone() const {
    auto* clone = make<ThreeValuedBoolAnd>(*this);
    clone_children_into(*clone);
    return clone;
}

bool ThreeValuedBoolAnd::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<ThreeValuedBoolAnd>(other);
    return o != nullptr && left()->perform_match(o->left(), match) && right()->perform_match(o->right(), match);
}

void ThreeValuedBoolOr::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(left()->result_type() == StackType::I4 || left()->result_type() == StackType::O);
}

ILInstruction* ThreeValuedBoolOr::clone() const {
    auto* clone = make<ThreeValuedBoolOr>(*this);
    clone_children_into(*clone);
    return clone;
}

bool ThreeValuedBoolOr::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<ThreeValuedBoolOr>(other);
    return o != nullptr && left()->perform_match(o->left(), match) && right()->perform_match(o->right(), match);
}

// ---- NullableUnwrap, NullableRewrap

NullableUnwrap::NullableUnwrap(StackType unwrapped_type, ILInstruction* argument, bool ref_input_)
    : UnaryInstruction(OpCode::NullableUnwrap, argument), ref_input(ref_input_), result_type_(unwrapped_type) {
    if (unwrapped_type == StackType::Ref) assert(ref_input_);
}

void NullableUnwrap::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    if (ref_input)
        assert(argument()->result_type() == StackType::Ref && "nullable.unwrap expects reference to nullable type as input");
    else
        assert(argument()->result_type() == StackType::O && "nullable.unwrap expects nullable type as input");
#ifndef NDEBUG
    bool found = false;
    for (ILInstruction* a : ancestors())
        if (is<NullableRewrap>(a)) found = true;
    assert(found);
#endif
}

void NullableUnwrap::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    output.write("nullable.unwrap.");
    if (ref_input) output.write("refinput.");
    write(output, result_type_);
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}

ILInstruction* NullableUnwrap::clone() const {
    auto* clone = make<NullableUnwrap>(*this);
    clone_children_into(*clone);
    return clone;
}

bool NullableUnwrap::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<NullableUnwrap>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

void NullableRewrap::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument()->has_flag(InstructionFlags::MayUnwrapNull));
}

InstructionFlags NullableRewrap::compute_flags() const {
    // Convert MayUnwrapNull flag to ControlFlow flag. Also, remove EndpointUnreachable flag, because the end-point is reachable through
    // the implicit nullable.unwrap branch.
    constexpr InstructionFlags flags_to_remove = InstructionFlags::MayUnwrapNull | InstructionFlags::EndPointUnreachable;
    return (argument()->flags() & ~flags_to_remove) | InstructionFlags::ControlFlow;
}

bool NullableRewrap::prepare_extract(int child_index, transforms::ExtractionContext& ctx) {
    return UnaryInstruction::prepare_extract(child_index, ctx) && !any(ctx.flags_being_moved & InstructionFlags::MayUnwrapNull);
}

bool NullableRewrap::can_inline_into_slot(int child_index, ILInstruction* expression_being_moved) {
    // Inlining into nullable.rewrap is OK unless the expression being inlined contains a nullable.wrap that isn't being re-wrapped
    // within the expression being inlined.
    return UnaryInstruction::can_inline_into_slot(child_index, expression_being_moved) &&
           !expression_being_moved->has_flag(InstructionFlags::MayUnwrapNull);
}

ILInstruction* NullableRewrap::clone() const {
    auto* clone = make<NullableRewrap>(*this);
    clone_children_into(*clone);
    return clone;
}

bool NullableRewrap::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<NullableRewrap>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match);
}

// ---- constants

void LdStr::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    write_operand_string(output, value);
}
ILInstruction* LdStr::clone() const { return make<LdStr>(*this); }
bool LdStr::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdStr>(other);
    return o != nullptr && value == o->value;
}

void LdStrUtf8::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    write_operand_string(output, value);
}
ILInstruction* LdStrUtf8::clone() const { return make<LdStrUtf8>(*this); }
bool LdStrUtf8::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdStrUtf8>(other);
    return o != nullptr && value == o->value;
}

void LdcI4::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    disassembler::write_operand(output, static_cast<long long>(value));
}
ILInstruction* LdcI4::clone() const { return make<LdcI4>(*this); }
bool LdcI4::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdcI4>(other);
    return o != nullptr && value == o->value;
}

void LdcI8::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    disassembler::write_operand(output, static_cast<long long>(value));
}
ILInstruction* LdcI8::clone() const { return make<LdcI8>(*this); }
bool LdcI8::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdcI8>(other);
    return o != nullptr && value == o->value;
}

void LdcF4::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    disassembler::write_operand(output, value);
}
ILInstruction* LdcF4::clone() const { return make<LdcF4>(*this); }
bool LdcF4::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdcF4>(other);
    return o != nullptr && value == o->value;
}

void LdcF8::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    disassembler::write_operand(output, value);
}
ILInstruction* LdcF8::clone() const { return make<LdcF8>(*this); }
bool LdcF8::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdcF8>(other);
    return o != nullptr && value == o->value;
}

void LdcDecimal::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    output.write(value.to_string());
}
ILInstruction* LdcDecimal::clone() const { return make<LdcDecimal>(*this); }
bool LdcDecimal::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdcDecimal>(other);
    return o != nullptr && value == o->value;
}

ILInstruction* LdNull::clone() const { return make<LdNull>(*this); }
bool LdNull::perform_match(ILInstruction* other, patterns::Match&) { return as<LdNull>(other) != nullptr; }

// ---- method pointers and tokens

void LdFtn::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (method_ != nullptr) {
        output.write(' ');
        il::write_to(*method_, output);
    }
}
ILInstruction* LdFtn::clone() const { return make<LdFtn>(*this); }
bool LdFtn::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdFtn>(other);
    return o != nullptr && typesystem::member_equals(method_, o->method_);
}

void LdVirtFtn::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (method_ != nullptr) {
        output.write(' ');
        il::write_to(*method_, output);
    }
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}
ILInstruction* LdVirtFtn::clone() const {
    auto* clone = make<LdVirtFtn>(*this);
    clone_children_into(*clone);
    return clone;
}
bool LdVirtFtn::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdVirtFtn>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && typesystem::member_equals(method_, o->method_);
}

void LdVirtDelegate::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
    if (method_ != nullptr) {
        output.write(' ');
        il::write_to(*method_, output);
    }
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}
ILInstruction* LdVirtDelegate::clone() const {
    auto* clone = make<LdVirtDelegate>(*this);
    clone_children_into(*clone);
    return clone;
}
bool LdVirtDelegate::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<LdVirtDelegate>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && type_->equals(o->type_) &&
           typesystem::member_equals(method_, o->method_);
}

void LdTypeToken::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*type_, output);
}
ILInstruction* LdTypeToken::clone() const { return make<LdTypeToken>(*this); }
bool LdTypeToken::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdTypeToken>(other);
    return o != nullptr && type_->equals(o->type_);
}

void LdMemberToken::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    output.write(' ');
    il::write_to(*member_, output);
}
ILInstruction* LdMemberToken::clone() const { return make<LdMemberToken>(*this); }
bool LdMemberToken::perform_match(ILInstruction* other, patterns::Match&) {
    auto* o = as<LdMemberToken>(other);
    return o != nullptr && member_->equals(o->member_);
}

// ---- StringToInt

const SlotInfo StringToInt::argument_slot{"Argument", true};

StringToInt::StringToInt(ILInstruction* argument, std::vector<MapEntry> map, const IType* expected_type)
    : ILInstruction(OpCode::StringToInt), map_(std::move(map)), expected_type_(expected_type) {
    set_argument(argument);
}

StringToInt::StringToInt(ILInstruction* argument, const std::vector<std::optional<std::string>>& map, const IType* expected_type)
    : StringToInt(argument, [&map] {
          std::vector<MapEntry> dict;
          for (std::size_t i = 0; i < map.size(); i++) dict.emplace_back(map[i], static_cast<int>(i));
          return dict;
      }(), expected_type) {}

ILInstruction* StringToInt::get_child(int index) const {
    if (index == 0) return argument_;
    index_out_of_range();
}

void StringToInt::set_child(int index, ILInstruction* value) {
    if (index == 0) return set_argument(value);
    index_out_of_range();
}

const SlotInfo& StringToInt::get_child_slot(int index) const {
    if (index == 0) return argument_slot;
    index_out_of_range();
}

void StringToInt::check_invariant(ILPhase phase) {
    ILInstruction::check_invariant(phase);
    assert(argument_->result_type() == StackType::O);
}

void StringToInt::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    output.write("string.to.int ");
    il::write_to(*expected_type_, output);
    output.write('(');
    argument_->write_to(output, options);
    output.write(", { ");
    int i = 0;
    for (const auto& entry : map_) {
        if (i > 0) output.write(", ");
        if (!entry.first.has_value())
            output.write(std::format("[null] = {}", entry.second));
        else
            output.write(std::format("[\"{}\"] = {}", *entry.first, entry.second));
        i++;
    }
    output.write(" })");
}

ILInstruction* StringToInt::clone() const {
    auto* clone = make<StringToInt>(*this);
    clone->set_argument(argument_->clone());
    return clone;
}

bool StringToInt::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<StringToInt>(other);
    return o != nullptr && argument_->perform_match(o->argument_, match);
}

// ---- ExpressionTreeCast

StackType ExpressionTreeCast::result_type() const { return get_stack_type(*type_); }

void ExpressionTreeCast::write_to_core(ITextOutput& output, const ILAstWritingOptions& options) const {
    write_il_range(output, options);
    write(output, op_code);
    if (is_checked_) output.write(".checked");
    output.write(' ');
    il::write_to(*type_, output);
    output.write('(');
    argument()->write_to(output, options);
    output.write(')');
}

ILInstruction* ExpressionTreeCast::clone() const {
    auto* clone = make<ExpressionTreeCast>(*this);
    clone_children_into(*clone);
    return clone;
}

bool ExpressionTreeCast::perform_match(ILInstruction* other, patterns::Match& match) {
    auto* o = as<ExpressionTreeCast>(other);
    return o != nullptr && argument()->perform_match(o->argument(), match) && type_->equals(o->type_) && is_checked_ == o->is_checked_;
}

// ---- the generated Match* methods of these classes (out parameters are pointers, set to null when the match fails)

bool ILInstruction::match_ckfinite(ILInstruction** argument) const {
    if (auto* inst = as<Ckfinite>(this)) {
        *argument = inst->argument();
        return true;
    }
    *argument = nullptr;
    return false;
}

bool ILInstruction::match_ld_loc(ILVariable** variable) const {
    if (auto* inst = as<LdLoc>(this)) {
        *variable = inst->variable();
        return true;
    }
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_ld_loca(ILVariable** variable) const {
    if (auto* inst = as<LdLoca>(this)) {
        *variable = inst->variable();
        return true;
    }
    *variable = nullptr;
    return false;
}

bool ILInstruction::match_st_loc(ILVariable** variable, ILInstruction** value) const {
    if (auto* inst = as<StLoc>(this)) {
        *variable = inst->variable();
        *value = inst->value();
        return true;
    }
    *variable = nullptr;
    *value = nullptr;
    return false;
}

bool ILInstruction::match_address_of(ILInstruction** value, const IType** type) const {
    if (auto* inst = as<AddressOf>(this)) {
        *value = inst->value();
        if (type != nullptr) *type = inst->type();
        return true;
    }
    *value = nullptr;
    if (type != nullptr) *type = nullptr;
    return false;
}

bool ILInstruction::match_three_valued_bool_and(ILInstruction** left, ILInstruction** right) const {
    if (auto* inst = as<ThreeValuedBoolAnd>(this)) {
        *left = inst->left();
        *right = inst->right();
        return true;
    }
    *left = nullptr;
    *right = nullptr;
    return false;
}

bool ILInstruction::match_three_valued_bool_or(ILInstruction** left, ILInstruction** right) const {
    if (auto* inst = as<ThreeValuedBoolOr>(this)) {
        *left = inst->left();
        *right = inst->right();
        return true;
    }
    *left = nullptr;
    *right = nullptr;
    return false;
}

bool ILInstruction::match_nullable_rewrap(ILInstruction** argument) const {
    if (auto* inst = as<NullableRewrap>(this)) {
        *argument = inst->argument();
        return true;
    }
    *argument = nullptr;
    return false;
}

bool ILInstruction::match_ld_str(std::string* value) const {
    if (auto* inst = as<LdStr>(this)) {
        *value = inst->value;
        return true;
    }
    *value = std::string();
    return false;
}

bool ILInstruction::match_ld_str_utf8(std::string* value) const {
    if (auto* inst = as<LdStrUtf8>(this)) {
        *value = inst->value;
        return true;
    }
    *value = std::string();
    return false;
}

bool ILInstruction::match_ldc_i4(std::int32_t* value) const {
    if (auto* inst = as<LdcI4>(this)) {
        *value = inst->value;
        return true;
    }
    *value = 0;
    return false;
}

bool ILInstruction::match_ldc_i8(std::int64_t* value) const {
    if (auto* inst = as<LdcI8>(this)) {
        *value = inst->value;
        return true;
    }
    *value = 0;
    return false;
}

bool ILInstruction::match_ldc_f4(float* value) const {
    if (auto* inst = as<LdcF4>(this)) {
        *value = inst->value;
        return true;
    }
    *value = 0;
    return false;
}

bool ILInstruction::match_ldc_f8(double* value) const {
    if (auto* inst = as<LdcF8>(this)) {
        *value = inst->value;
        return true;
    }
    *value = 0;
    return false;
}

bool ILInstruction::match_ldc_decimal(util::Decimal* value) const {
    if (auto* inst = as<LdcDecimal>(this)) {
        *value = inst->value;
        return true;
    }
    *value = util::Decimal();
    return false;
}

bool ILInstruction::match_ld_null() const { return as<LdNull>(this) != nullptr; }

bool ILInstruction::match_ld_ftn(const IMethod** method) const {
    if (auto* inst = as<LdFtn>(this)) {
        *method = inst->method();
        return true;
    }
    *method = nullptr;
    return false;
}

bool ILInstruction::match_ld_virt_ftn(ILInstruction** argument, const IMethod** method) const {
    if (auto* inst = as<LdVirtFtn>(this)) {
        *argument = inst->argument();
        *method = inst->method();
        return true;
    }
    *argument = nullptr;
    *method = nullptr;
    return false;
}

bool ILInstruction::match_ld_virt_delegate(ILInstruction** argument, const IType** type, const IMethod** method) const {
    if (auto* inst = as<LdVirtDelegate>(this)) {
        *argument = inst->argument();
        *type = inst->type();
        *method = inst->method();
        return true;
    }
    *argument = nullptr;
    *type = nullptr;
    *method = nullptr;
    return false;
}

bool ILInstruction::match_ld_type_token(const IType** type) const {
    if (auto* inst = as<LdTypeToken>(this)) {
        *type = inst->type();
        return true;
    }
    *type = nullptr;
    return false;
}

bool ILInstruction::match_ld_member_token(const IMember** member) const {
    if (auto* inst = as<LdMemberToken>(this)) {
        *member = inst->member();
        return true;
    }
    *member = nullptr;
    return false;
}

}  // namespace cb::dotnet::decompiler::il
